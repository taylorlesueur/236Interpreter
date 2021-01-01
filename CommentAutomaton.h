//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_COMMENTAUTOMATON_H
#define PROJECT1_COMMENTAUTOMATON_H

#include <string>
#include <vector>
#include <iostream>
#include "Automaton.h"

using namespace std;

//Inheriting from Automaton.
class CommentAutomaton : public Automaton {

public:
    //some constructors. In the cpp file these call the Automaton constructo
    CommentAutomaton(){
        type = COMMENT;
    };
    
    //this method reads and returns the line number of a match or 0 if there is no match
    int Read(const string& input){

        int inputRead = 0;
        if (input[0] == '#') {
            inputRead++;
            
            if (input.length() < 2) { //so we don't get seg faults...
                return 0;
            }
            
            if (input[1] == '|') { //block comments
                for (unsigned int i = 2; i < input.length(); i++){
                    if (input[i] == '\n'){
                        newLines++;
                    }
                    else if (input[i] == '|') {
                        if (input [i+1] == EOF) { //reject state undefined
                            return -1;
                        }
                        else if (input [i+1] == '#') { //correct case, final state
                            inputRead++;
                            break;
                        }
                        else {
                            inputRead++;
                        }
                    }
                    else {
                        inputRead++;
                    }
                }
            }
            else { //single line comments
                unsigned int i = 1;
                while (i < input.length() && input[i] != '\n' && input[i] != EOF) {
                    inputRead++;
                    i++;
                }
            }
        }
        return inputRead;
    };
};


#endif //PROJECT1_COMMENTAUTOMATON_H
