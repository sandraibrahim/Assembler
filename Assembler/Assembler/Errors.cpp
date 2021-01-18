//
//      Implementation of the Error class.
//
#include "stdafx.h"
#include "Errors.h"
vector<string> Errors::m_ErrorMsgs;

/*
void Errors::DisplayErrors()

NAME

    void Errors::DisplayErrors()

SYNOPSIS

    void Errors::DisplayErrors()
    NONE

DESCRIPTION

    This function will display all the errors in the vector, and clear vector when finished.

RETURNS

    void

*/
void Errors::DisplayErrors() {
    for (size_t i = 0; i < m_ErrorMsgs.size(); i++) {
        cerr << m_ErrorMsgs[i] << endl;
    }
    m_ErrorMsgs.clear();
}