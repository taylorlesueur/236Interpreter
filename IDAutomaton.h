//
// Created by Taylor LeSueur on 9/15/20.
//

#ifndef PROJECT1_IDAUTOMATON_H
#define PROJECT1_IDAUTOMATON_H

#include <string>
#include <vector>
#include <iostream>
#include "Automaton.h"

using namespace std;

//Inheriting from Automaton.
class IDAutomaton : public Automaton {

public:
    string id;
    //some constructors. In the cpp file these call the Automaton constructo
    IDAutomaton(){
        id = "";
        type = ID;
    };
    
    //this method reads and returns the line number of a match or 0 if there is no match
    int Read(const string& input){

        int inputRead = 0;
        string i_d = "";
        
        if (isalpha(input[0])){
            i_d += input[0];
            inputRead++;
            for (unsigned int i = 1; i < input.length(); ++i){
                if (isalnum(input[i])){
                    i_d += input[i];
                    inputRead++;
                }
                else { //accepting state with space
                    //cout << "Found complete ID: " << input.substr(0,inputRead) << endl << "Input read: " << inputRead << endl;
                    break;
                }

            }
        }
        return inputRead;
    };
};


#endif //PROJECT1_IDAUTOMATON_H
