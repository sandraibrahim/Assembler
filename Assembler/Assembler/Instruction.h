//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once
#include "stdafx.h"

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   		// end instruction.
    };

    // Parse the Instruction.
    InstructionType ParseInstruction(string& a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    //Helper
    // Determine if instruction is an Assembler Instruction or End Instruction
    char returnAssemblerInstruction(string opCode);

    // To access the label
    inline string &GetLabel( ) {

        return m_Label;
    };

    // To access the opcode
    inline string& getOpCode() {
        return m_OpCode;
    };

    // To access operand 1
    inline string& getOper1() {
        return m_Operand1;
    };

    // To access operand 2
    inline string& getOper2() {
        return m_Operand2;
    };

    // To access the original instruction
    inline string& getOGInstr() {
        return m_instruction;
    };

    // To determine if a label is blank.
    inline bool isLabel( ) {

        return !m_Label.empty();
    };



private:
    // The elemements of a instruction
    string m_Label;            // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand.
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    InstructionType m_type; // The type of instruction.
};


