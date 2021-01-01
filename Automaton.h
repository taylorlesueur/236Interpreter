//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_AUTOMATON_H
#define PROJECT1_AUTOMATON_H

#include <vector>
#include <string>
#include "Token.h"

using namespace std;


class Automaton {
protected:
    int newLines = 0;
    TokenType type;
public:
    Automaton(){type = UNDEFINED;};
    Automaton(TokenType type) {this->type = type;};
    virtual int Read(const string& input) = 0;
    virtual Token* CreateToken(string input, int lineNumber) {
        return new Token(type, input, lineNumber); }
    virtual int NewLinesRead() const { return newLines; }

};


#endif //PROJECT1_AUTOMATON_H
