#ifndef PROJECT1_LEXER_H
#define PROJECT1_LEXER_H

#include <string>
#include <vector>
#include "Token.h"
#include "Automaton.h"
#include "MatcherAutomaton.h"
#include "IDAutomaton.h"
#include "StringAutomaton.h"
#include "CommentAutomaton.h"
#include "UndefAutomaton.h"

using namespace std;

class Lexer {

public:
    Lexer(); //default constructor
    Lexer(string input); //parameterized constuctor recieves char vect of input file
    void run();
    void print();
    vector <Token*> getTokens();
private:
    vector<Token*> tokens; //this is a vector of tokens 
    vector<Automaton*> machines;//this is a vector of automaton pointers to enable polymorphism
    string inputFile; // this is initialized in the parameterized constructor
    UndefAutomaton* undefAuto;

};
#endif //PROJECT1_LEXER_H
