//
//		Emulator class - supports the emulation of VC1620 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H
#include "stdafx.h"
#include <assert.h>
class emulator {

public:

    const static int MEMSZ = 100000;	// The size of the memory of the VC1620.
    emulator() {

         m_memory.resize(MEMSZ, 0);
    }

    // Records instructions and data into VC1620 memory.
    bool insertMemory(int a_location, long long a_contents);
    
    // Runs the VC1620 program recorded in memory.
    bool runProgram( );

private:

    vector<long long> m_memory;         // Memory for the VC1620
};

#endif

