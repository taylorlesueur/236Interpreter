//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_STRINGAUTOMATON_H
#define PROJECT1_STRINGAUTOMATON_H

#include <string>
#include <vector>
#include <iostream>
#include "Automaton.h"

using namespace std;

//Inheriting from Automaton.
class StringAutomaton : public Automaton {

public:
    //some constructors. In the cpp file these call the Automaton constructo
    StringAutomaton(){
        type = STRING;
    };
    
    //this method reads and returns the line number of a match or 0 if there is no match
    int Read(const string& input){

        int inputRead = 0;
        newLines = 0;
        bool invalid = true;
        
        if ((input[0]) == '\''){

            inputRead++;
            
            for (unsigned int i = 1; i < input.length(); i++){
                
                if (input[i] == '\n'){
                    newLines++;
                }
                
                if (input[i] == '\'') { //closing paren
                    inputRead++;
                    i++;
                    
                    if (i<input.size() && input[i] == '\'' ) { //escape sequence?
                        inputRead++;
                        //cout << "Hit double quote: " << newLines << endl;
                        continue;
                    }
                    
                     //not escape sequence - real closing paren
                        //cout << "Hit the closing parentheses" << endl;
                       // cout << "Input read is: " << inputRead << endl;
                        invalid = false;
                        return inputRead;
                    
                }
                else {
                    inputRead++;
                }
            }
            if (invalid == true) {
            //cout << "Hit undefined:" << newLines << endl;
            return -1;
        }
        }
        
        
        return inputRead;
    }
};


#endif //PROJECT1_STRINGAUTOMATON_H
