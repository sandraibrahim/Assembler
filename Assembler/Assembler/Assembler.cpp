//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}
// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler()
{
}

/*
void Assembler::PassI()

NAME

    void Assembler::PassI()

SYNOPSIS

   void Assembler::PassI()
   NONE

DESCRIPTION

    This function will establish the locations of the labels. It will get the line from the sourcer file and first parse
    the instruction. It will then determind if it is an end instruction, machine language instructions, or assembly instruction. 
    If the instruction is end, it will just return. If the instruction has a label, it will record the label and location and add it into
    the symbol table. At the end of this function, it will display the full symbole table that is completely filled out.

RETURNS

    void

*/
void Assembler::PassI()
{
    
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if (st == Instruction::ST_End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.  Currently this is only comments.
        if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr)
        {
            continue;
        }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {

            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }

    // Displays symbol table
    m_symtab.DisplaySymbolTable();
    
}

/*
void Assembler::PassII()

NAME

    void Assembler::PassII()

SYNOPSIS

   void Assembler::PassII()
   NONE

DESCRIPTION

    The PassII function is responsible for parsing through each instruction given to the program in the 
    source file and commiting these instructions in a specific format to memory. For Every line, we make sure to
    parse each instruction and determine what that does to the location of the instruction/memory. Each instruction 
    is then passed to the emulator's memory using the insertMemory() function. For every iteration, we also output 
    the original statement as well as the contents to be stored into the emulators memory

    Unfortunately PassII does contain a few inefficiencies, however due to my circumstances this is the best that I could do :(

RETURNS

    void

*/
void Assembler::PassII() {
    m_facc.rewind();

    int curr_loc = 0;
    // Map of Op Codes
    map<string, string> OpCodesMap{
        {"add", "01"},
        {"sub", "02"},
        {"mult", "03"},
        {"div", "04"},
        {"copy", "05"},
        {"read", "07"},
        {"write", "08"},
        {"b", "09"},
        {"bm", "10"},
        {"bz", "11"},
        {"bp", "12"},
        {"halt", "13"},
        {"ds", "15"},
        {"org", "16"},
        {"end", "17"}
    };
    map<string, string>::iterator it;
    string curr_content = "";
    string curr_OpCode = "";
    string curr_Oper1 = "";
    string curr_Oper2 = "";
    string loc_of_symbol = "";
    string curr_instruction = "";
    cout << setw(15) << "Location " << setw(15) << "Contents " << setw(25) << "Original Statement" << endl;

    // Successively process each line of source code.
    for (; ; ) {
        Errors::InitErrorReporting();
        if (curr_loc > 999 || curr_loc < 0) {
            Errors::RecordError(string("Error: Insufficient Memory for Translation"));
            Errors::DisplayErrors();
            break;
        }
        curr_content = "";

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {
            Errors::RecordError(string("Error: Missing END statement"));
            Errors::DisplayErrors();
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);
        curr_instruction = m_inst.getOGInstr();     // gets original instruction

        //Check if instruction is a ST_Comment or ST_END
        if (st == Instruction::ST_Comment || st == Instruction::ST_End) {
            cout << setw(52) << curr_instruction << endl;
            if (st == Instruction::ST_End) {
                if (m_facc.GetNextLine(line)) {
                    Errors::RecordError(string("Error: END statement not the last line"));
                    Errors::DisplayErrors();
                }
                break;
            }
            continue;
        }

        // Check label length
        if (m_inst.GetLabel().length() > 10) {
            //If it's too long, record the error.
            Errors::RecordError(string("Error: Label too long"));
        }
        else {
            // Check if it starts with a number.
            char labelstart = m_inst.GetLabel()[0];
            if (labelstart >= '0' && labelstart <= '9') {
                //If yes, record the error.
                Errors::RecordError(string("Error: Label starts with number"));
            }
        }

        curr_OpCode = lower_case(m_inst.getOpCode());       // lowewercases opcode
        curr_Oper1 = lower_case(m_inst.getOper1());         // lowewercases operand 1
        curr_Oper2 = lower_case(m_inst.getOper2());         // lowewercases operand 2

        // finds opcode
        it = OpCodesMap.find(curr_OpCode);

        // if dc, appends 2 0s to the content
        if (curr_OpCode == "dc") {
            curr_content = curr_content + "00";
        }
        // if not in map, it records an error for an illegal opcode
        else if (it == OpCodesMap.end()) {
            Errors::RecordError(string("Error: Illegal OpCode"));
        }
        // if ds or org, it will print out formatted output
        else if (it->second == "15" || it->second == "16") {
            cout << setw(10) << right << curr_loc << setw(45) << right << curr_instruction << endl;

            // finds next location if the operand is valid, continue to next iteration (no content)
            if ((curr_OpCode == "org" || curr_OpCode == "ds") && (!isNumber(curr_Oper1))) {
                Errors::RecordError(string("Error: Invalid Operand"));
            }
            else {
                curr_loc = m_inst.LocationNextInstruction(curr_loc);
            }
            continue;
        }
        // any valid opcode, assigns opcode to content
        else {
            curr_content = curr_content + it->second;
        }
        
        // if its dc, add the zeros to the beginning of content and move on to the next iteration
        if (curr_OpCode == "dc") {
            if (curr_Oper1.length() > 10) {
                Errors::RecordError(string("Error: Invalid Location"));
            }
            curr_content += add_zeros(curr_Oper1, 10);
            cout << setw(10) << right << curr_loc << setw(21) << right << curr_content << setw(24) << right << curr_instruction << endl;

            if ((curr_OpCode == "org" || curr_OpCode == "ds") && (!isNumber(curr_Oper1))) {
                Errors::RecordError(string("Error: Invalid Operand"));
            }
            else {
                curr_loc = m_inst.LocationNextInstruction(curr_loc);
            }
            
            Errors::DisplayErrors();
            continue;
        }

        // adds zeroes and correct operand 1 to the content, if invalid location it records an error
        if (m_symtab.LookupSymbol(curr_Oper1)) {
            loc_of_symbol = m_symtab.FindLocation(curr_Oper1);
            if (loc_of_symbol.length() > 5) {
                Errors::RecordError(string("Error: Invalid Location"));
            }
            curr_content += add_zeros(loc_of_symbol,5);
        }
        else if(isNumber(curr_Oper1)){
            if (curr_Oper1.length() > 5) {
                Errors::RecordError(string("Error: Invalid Location"));
            }
            curr_content += add_zeros(curr_Oper1, 5);
        }

        // adds zeroes and correct operand 2 to the content, if invalid location it records an error
        if (m_symtab.LookupSymbol(curr_Oper2)) {
            loc_of_symbol = m_symtab.FindLocation(curr_Oper2);
            if (loc_of_symbol.length() > 5) {
                Errors::RecordError(string("Error: Invalid Location"));
            }
            curr_content += add_zeros(loc_of_symbol, 5);
        }
        else if (isNumber(curr_Oper2)) {
            if (curr_Oper2.length() > 5) {
                Errors::RecordError(string("Error: Invalid Location"));
            }
            curr_content += add_zeros(curr_Oper2, 5);
        }

        //Insert into memory for emulator
        if (curr_OpCode!= "dc" && curr_OpCode != "ds" && curr_OpCode != "org" && curr_OpCode != "end") {
            m_emul.insertMemory(curr_loc, stoll(curr_content));
        }

        //----OUTPUT----
        cout << setw(10) << right << curr_loc << setw(21) << right << curr_content << setw(65) << curr_instruction << endl;

        // Compute the location of the next instruction.
        if ((curr_OpCode == "org" || curr_OpCode == "ds") && (!isNumber(curr_Oper1))) {
            Errors::RecordError(string("Error: Invalid Operand"));
        }
        else {
            curr_loc = m_inst.LocationNextInstruction(curr_loc);
        }
        Errors::DisplayErrors();
    }
    
}

/*
void Assembler::RunProgramInEmulator()

NAME

    void Assembler::RunProgramInEmulator()

SYNOPSIS

   void Assembler::RunProgramInEmulator()
   NONE

DESCRIPTION

    This function will run the program in the emulator.

RETURNS

    void

*/
void Assembler::RunProgramInEmulator() {
    m_emul.runProgram();        //runs program in emulator
}