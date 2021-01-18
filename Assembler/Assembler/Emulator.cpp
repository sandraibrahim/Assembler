//
//      Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"


/*
bool emulator::insertMemory(int a_location, long long a_contents)

NAME

    bool emulator::insertMemory(int a_location, long long a_contents)

SYNOPSIS

    bool emulator::insertMemory(int a_location, long long a_contents)
    a_location --> location to store contents
    a_contents --> contents to be stored

DESCRIPTION

    This function will insert the contents into the correct memory location it is supposed to be located in. It
    will only do so if it is in the memory location. 

RETURNS
    
    true --> if it successfully stored the contents into its location
    false --> location was outside of the memory range

*/
bool emulator::insertMemory(int a_location, long long a_contents) {
    
    //makes sure it can properly fit in the memory range and stores it
    if (a_location >= 0 && a_location < MEMSZ)
    {
        m_memory[a_location] = a_contents;
        return true;
    }
    else
    {
        cerr << "Location outside memory range" << endl;
        return false;
    }
}

/*
bool emulator::runProgram()

NAME

    bool emulator::runProgram()

SYNOPSIS

   bool emulator::runProgram()
    NONE

DESCRIPTION

    This function will go through the memory vector and look at each opcode. It will go through a switch
    statement and enter the case that it identifies will and complete the instruction the opcode is asking it 
    to do. If it is given an invalid opcode it will report an error. 

RETURNS

    true --> if it successfully emulated the program
    exits if it does not

*/
bool emulator::runProgram() {
    int loc = 100;
    cout << "Results from emulating program:" << endl;
    cout << endl;
    while (loc < MEMSZ) {
        long long int instr = m_memory[loc];
        int OpCode = instr / 10000000000;       // seperates opcode from contents
        int addr1 = instr / 100000 % 100000;    // seperates addr1 from contents
        int addr2 = instr % 100000;             // seperates addr2 from contents
        int input;

        //Evaluate OpCode
        switch (OpCode)
        {
            //(01) - ADD 
        case 1:
            m_memory[addr1] = m_memory[addr1] + m_memory[addr2];
            break;
            //(02) - SUBTRACT
        case 2:
            m_memory[addr1] = m_memory[addr1] - m_memory[addr2];
            break;
            //(03) - MULTIPLY
        case 3:
            m_memory[addr1] = m_memory[addr1] * m_memory[addr2];
            break;
            //(04) - DIVIDE
        case 4:
            m_memory[addr1] = m_memory[addr1] / m_memory[addr2];
            break;
            //(05) - COPY
        case 5:
            m_memory[addr1] = m_memory[addr2];
            break;
            //(07) - READ  
        case 7:
            cout << "? ";
            cin >> input;
            m_memory[addr1] = input;
            break;
            //(08) - WRITE
        case 8:
            cout << m_memory[addr1] << endl;
            break;
            //(09) - BRANCH
        case 9:
            loc = addr1;
            loc--;
            break;
            //(10) - BRANCH MINUS
        case 10:
            if (m_memory[addr2] < 0) {
                loc = addr1;
                loc--;
            }
            break;
            //(11) - BRANCH ZERO
        case 11:
            if (m_memory[addr2] == 0) {
                loc = addr1;
                loc--;
            }
            break;
            //(12) - BRANCH POSITIVE
        case 12:
            if (m_memory[addr2] > 0) {
                loc = addr1;
                loc--;
            }
            break;
            //(13) - HALT
        case 13:
            cout << endl;
            cout << "End of emulation" << endl;
            cout << "      __" << endl;
            cout << "  ___( o)>" << endl;
            cout << "  \\ <_. )" << endl;
            cout << "   `---'   " << endl;
            cout << "What did the duck detective say to his partner?" << endl;
            cout << "Let's QUACK this case!" << endl;
            cout << "Hope that quacked you up! Thank you Professor Miller!" << endl;
            cout << "Waddle out!" << endl;
            cout << endl;
            return true;
        default:
            cerr << "Illegal opcode" << endl;
            exit(1);
        }
    loc++;
    }
}