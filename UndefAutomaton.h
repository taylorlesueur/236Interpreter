//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_UNDEFAUTOMATON_H
#define PROJECT1_UNDEFAUTOMATON_H

#include <string>
#include <vector>
#include <iostream>
#include "Automaton.h"

using namespace std;

//Inheriting from Automaton.
class UndefAutomaton : public Automaton {

public:

    //some constructors. In the cpp file these call the Automaton constructo
    UndefAutomaton(){
        type = UNDEFINED;
    };
    
    //this method reads and returns the line number of a match or 0 if there is no match
    int Read(const string& input){

        int inputRead = 0;
        
        return inputRead;
    };
};


#endif //PROJECT1_UNDEFAUTOMATON_H
