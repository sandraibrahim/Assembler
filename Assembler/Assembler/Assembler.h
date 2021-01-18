//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 
#include "stdafx.h"
#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "Errors.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

    // convert any given word to all lowercase letters (to handle case issues)
    inline string lower_case(string word) {
        string lower_word = word;
        for_each(lower_word.begin(), lower_word.end(), [](char& c) {
            c = ::tolower(c);
        });
        return lower_word;
    }

    // adds correct amount of zeros needed for contents
    inline string add_zeros(string location, int max_zeros) {
        if (location.length() == max_zeros) {
            return location;
        }
        else {
            while (location.length() < max_zeros) {
                location = "0" + location;
            }
        }
        return location;
    }

    // checks if a string is a number and can be safely converted to an int
    inline bool isNumber(string location)
    {
        for (int i = 0; i < location.length(); i++)
            if (isdigit(location[i]) == false) {
                return false;
            }
        return true;
    }

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
};

