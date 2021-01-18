/*
 * Assembler main program.
 */
#include "stdafx.h"     // This must be present if you use precompiled headers which you will use. 
#include "Assembler.h"

 /*
 int main( int argc, char *argv[] )

 NAME

     int main( int argc, char *argv[] )

 SYNOPSIS

    int main( int argc, char *argv[] )
    NONE

 DESCRIPTION

     This is the main function that calls Pass 1, display symbol table,  Pass 2 and the emulator.

 RETURNS

     returns 0 indicating all is well

 */
int main( int argc, char *argv[] )
{
    Assembler assem( argc, argv );

    // Establish the location of the labels:
    assem.PassI( );

    // Display the symbol table.
    assem.DisplaySymbolTable();

    //Formatting
    cout << "-----------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Press Enter to continue...";
    cin.ignore();
    cout << endl;

    // Output the translation.
    assem.PassII( );

    //Formatting
    cout << "-----------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Press Enter to continue...";
    cin.ignore();
    cout << endl;

    // Run the emulator on theVC1620 program that was generated in Pass II.
    assem.RunProgramInEmulator();
   
    // Terminate indicating all is well.  If there is an unrecoverable error, the 
    // program will terminate at the point that it occurred with an exit(1) call.
    return 0;
}
