//
//      Implementation of the Instruction class.
//
#include "stdafx.h"
#include "Errors.h"
#include "Instruction.h"

/*
Instruction::InstructionType Instruction::ParseInstruction(string& a_line)

NAME

    Instruction::InstructionType Instruction::ParseInstruction(string& a_line)
SYNOPSIS

    Instruction::InstructionType Instruction::ParseInstruction(string& a_line)
    a_line --> instruction line that needs to be parsed

DESCRIPTION

    This function will recieve an instruction that needs to be parsed. It will assign values to different variables
    according to the line it is given and determine what kind of instruction it is. (i.e machine language, end instruction, 
    assembly instruction, or a comment instruction)

RETURNS

    The instruction type (machine language, end instruction, assembly instruction, or a comment instruction)

*/
Instruction::InstructionType Instruction::ParseInstruction(string& a_line) {

    //Save the original statement
    m_instruction = a_line;

    //Get rid any comments
    size_t iSemiColon = a_line.find(';');
    if (iSemiColon != string::npos) {
        a_line.erase(iSemiColon);
    }

    bool isLabel = !(a_line[0] == ' ' || a_line[0] == '\t');

    //label opcode operand1 operand 2
    replace(a_line.begin(), a_line.end(), ',', ' ');

    istringstream instr(a_line);

    string a, b, c, d, e;
    instr >> a >> b >> c >> d >> e;
    if (a=="")
    {
        return InstructionType::ST_Comment;
    }
    //If line is a label, different values will be assigned to m_label, m_opcode, etc.
    if (isLabel)
    {
        m_Label = a;
        m_OpCode = b;
        m_Operand1 = c;
        m_Operand2 = d;
        if (!e.empty())
        {
            Errors::RecordError(string("Error: Too many arguments"));
        }

        //Determine if instruction is an assembler instruction (Pass in label)
        //e == end statement
        //a == other assembler statement
        //n == not an assembler instruction
        char assemblerInst = returnAssemblerInstruction(m_OpCode);
        if (assemblerInst == 'e') {
            return InstructionType::ST_End;
        }
        if (assemblerInst == 'a') {
            return InstructionType::ST_AssemblerInstr;
        }
    }
    else
    {
        m_Label = "";
        m_OpCode = a;
        m_Operand1 = b;
        m_Operand2 = c;

        //Determine if instruction is an assembler instruction (Pass in Operand_1)
        //e == end statement
        //a == other assembler statement
        //n == not an assembler instruction
        char assemblerInst = returnAssemblerInstruction(m_OpCode);
        if (assemblerInst == 'e') {
            m_type = ST_End;
            return InstructionType::ST_End;
        }
        if (assemblerInst == 'a') {
            m_type = ST_AssemblerInstr;
            return InstructionType::ST_AssemblerInstr;
        }
    }
    m_type = ST_MachineLanguage;
    return InstructionType::ST_MachineLanguage;
}

/*
int Instruction::LocationNextInstruction(int a_loc)

NAME

    int Instruction::LocationNextInstruction(int a_loc)

SYNOPSIS

    int Instruction::LocationNextInstruction(int a_loc)
    a_loc --> current location

DESCRIPTION

    This function will find the location of the next instruction based on the type of current
    instruction. If it is ORG or DS, the location will be the current location + the operand in
    the instruction casted to an integer. If it is a comment instruction, it will be the current 
    location + 0. And if it is any other instruction, it will be the current location + 1.

RETURNS

    The location of the next instruction (int).

*/
int Instruction::LocationNextInstruction(int a_loc) {
    string loweroper = m_OpCode;

    //lowercase loweroper
    transform(loweroper.begin(), loweroper.end(), loweroper.begin(), ::tolower);
    
    // returns location based on circumstance
    if (m_Label == "") {
        if (loweroper == "org") {
            return a_loc + stoi(m_Operand1);
        }
    }
    else if (loweroper == "ds") {
            return a_loc + stoi(m_Operand1);
    }
    if (m_type != ST_Comment) {
        return a_loc + 1;
    }
    return a_loc;
}

/*
char Instruction::returnAssemblerInstruction(string opCode)

NAME

    char Instruction::returnAssemblerInstruction(string opCode)
    the code indicated

SYNOPSIS

    char Instruction::returnAssemblerInstruction(string opCode)
    opCode --> current opCode

DESCRIPTION

    This function will identify what type of assembly instruction is indicated. It will return a char flag
    that indicated which it is. 

RETURNS

    The char flag that indicated which assembly instruction it is: 
    e --> end instruction
    a --> dc, ds, org
    n --> none

*/
char Instruction::returnAssemblerInstruction(string opCode)
{
    string lowerop = opCode;
    transform(lowerop.begin(), lowerop.end(), lowerop.begin(), ::tolower);

    if (lowerop == "end") {
        return 'e';
    }
    else if (lowerop == "dc" || lowerop == "ds" || lowerop == "org") { 
        return 'a'; 
    }
    return 'n';
}
