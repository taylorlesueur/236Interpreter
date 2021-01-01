//
// Created by Taylor LeSueur on 9/15/20.
//

#include "MatcherAutomaton.h"

//Default constructor calls the default constructor of the Automaton class
MatcherAutomaton::MatcherAutomaton() : Automaton(){
    toMatch = "";
}

//Constructor that calls the constructor of the automaton class
MatcherAutomaton::MatcherAutomaton(string toMatch, TokenType type) : Automaton(type){
    this->toMatch = toMatch;
}

//This reads through the input file and checks for a match
int MatcherAutomaton::Read(const std::string& input) {
    bool isMatch = true;
    int inputRead = 0;
    for (int i = 0; i < (int)toMatch.size() && isMatch; i++) {
        if (input[i] != toMatch[i]) {
            isMatch = false;
        }
    }
    if (isMatch) {

        inputRead = toMatch.size();
    }
    return inputRead;
}
