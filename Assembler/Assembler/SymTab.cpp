//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )


NAME

    SymbolTable::AddSymbol( const string &a_symbol, int a_loc )

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    a_symbol --> symbol to be added
    a_loc --> location to add the symbol in memory

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.

RETURNS

    void

*/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
void SymbolTable::DisplaySymbolTable()


NAME

    void SymbolTable::DisplaySymbolTable()

SYNOPSIS

    void SymbolTable::DisplaySymbolTable()
    NO PARAMETERS

DESCRIPTION

    This function will display the full symbol table in the format: Symbol#, Symbol, Location

RETURNS

    void

*/
void SymbolTable::DisplaySymbolTable(){
    //iterator to go through the symbol table
    map<string, int>::iterator itr;

    //Formatting
    cout << setw(10) << "Symbol#" << setw(10) << "Symbol" << setw(10) << "Location" << endl;

    // for symbol number
    int count = 0;

    //iterate through map and print out symbol table formatted
    for (itr = m_symbolTable.begin(); itr != m_symbolTable.end(); ++itr) {
        cout << setw(10) << count << setw(10) << itr->first << setw(10) << itr->second << endl;
        count++;
    }
    cout << endl;
}

/*
bool SymbolTable::LookupSymbol(const string& a_symbol)


NAME

    bool SymbolTable::LookupSymbol(const string& a_symbol)

SYNOPSIS

    bool SymbolTable::LookupSymbol(const string& a_symbol)
    a_symbol --> the symbol to lookup

DESCRIPTION

    This function will lookup the symbol that it is given to see if it is in the table

RETURNS

    true --> if the symbol was found in table
    false --> if the symbol was not found in table

*/
bool SymbolTable::LookupSymbol(const string& a_symbol){
    //iterator for map
    map<string, int>::iterator itr;

    //find the symbol in the table
    itr = m_symbolTable.find(a_symbol);

    //if the iterator equals the end of the map, the symbol was not found
    if (itr == m_symbolTable.end()) {
        return false;
    }
    else {
        return true;
    }
    
}

/*
string SymbolTable::FindLocation(const string& a_symbol)


NAME

    string SymbolTable::FindLocation(const string& a_symbol)

SYNOPSIS

    string SymbolTable::FindLocation(const string& a_symbol)
    a_symbol --> the symbol to find in table

DESCRIPTION

    This function will find the symbol that it is given in the symbol table. Since this will only be called if the symbol
    was already known to be in the symbol table, it does not account for that.

RETURNS

    The string version of the location of the given symbol

*/
string SymbolTable::FindLocation(const string& a_symbol) {
    // iterator for map
    map<string, int>::iterator itr;

    //find the given symbol in the map
    itr = m_symbolTable.find(a_symbol); 

    //return the string version of the location
    return to_string(itr->second);
}