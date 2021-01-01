//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_MATCHERAUTOMATON_H
#define PROJECT1_MATCHERAUTOMATON_H

#include <string>
#include <vector>
#include "Automaton.h"

using namespace std;

//Inheriting from Automaton.
class MatcherAutomaton : public Automaton {

public:
    string toMatch;
    //some constructors. In the cpp file these call the Automaton constructo
    MatcherAutomaton();
    MatcherAutomaton(string toMatch, TokenType type);
    
    //this method reads and returns the line number of a match or 0 if there is no match
    int Read(const string& input);
};


#endif //PROJECT1_MATCHERAUTOMATON_H
