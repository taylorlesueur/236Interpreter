//
// Created by Taylor LeSueur on 9/14/20.
//

#include "Lexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <cctype>


//Default constructor
Lexer::Lexer() {
    tokens = {};
    machines = {};
    inputFile = "";
}

//parameterized cosntructor. Sets the inputFile char vector private member to the one passed in from main
//intansiates the FSA's and pushes them into the machines vector of automaton pointers.
Lexer::Lexer(string input) {
    
    //we are setting the private data member equal to the input file vector passed in from main
    this->inputFile = input;
    
    //initialize these vectors to null. We will fill them later
    tokens = {};
    machines = {};
    
    //Here we are creating a ton of FSA objects and pushing them into a machine vector of pointers
    Automaton* matchCom = new MatcherAutomaton(",", COMMA);
    machines.push_back(matchCom);

    Automaton* matchPer = new MatcherAutomaton(".", PERIOD);
    machines.push_back(matchPer);

    Automaton* matchQ = new MatcherAutomaton("?", Q_MARK);
    machines.push_back(matchQ);

    Automaton* matchLeft = new MatcherAutomaton("(", LEFT_PAREN);
    machines.push_back(matchLeft);

    Automaton* matchRight = new MatcherAutomaton(")", RIGHT_PAREN);
    machines.push_back(matchRight);

    Automaton* matchCol = new MatcherAutomaton(":", COLON);
    machines.push_back(matchCol);

    Automaton* matchColDash = new MatcherAutomaton(":-", COLON_DASH);
    machines.push_back(matchColDash);

    Automaton* matchMultiply = new MatcherAutomaton("*", MULTIPLY);
    machines.push_back(matchMultiply);

    Automaton* matchAdd = new MatcherAutomaton("+", ADD);
    machines.push_back(matchAdd);

    Automaton* matchSch = new MatcherAutomaton("Schemes", SCHEMES);
    machines.push_back(matchSch);

    Automaton* matchFac = new MatcherAutomaton("Facts", FACTS);
    machines.push_back(matchFac);

    Automaton* matchRul = new MatcherAutomaton("Rules", RULES);
    machines.push_back(matchRul);

    Automaton* matchQue = new MatcherAutomaton("Queries", QUERIES);
    machines.push_back(matchQue);
    
    Automaton* IDAuto = new IDAutomaton();
    machines.push_back(IDAuto);
    
    Automaton* StringAuto = new StringAutomaton();
    machines.push_back(StringAuto);
    
    Automaton* CommentAuto = new CommentAutomaton();
    machines.push_back(CommentAuto);
    
    undefAuto = new UndefAutomaton();
    
}

vector <Token*>  Lexer::getTokens(){
    return tokens;
}

void Lexer::print() {
    for (unsigned int i = 0; i < tokens.size(); ++i){
        
        cout << tokens.at(i)->toString() << endl;
    }
    cout << "Total Tokens = " << tokens.size() << endl;
}

void Lexer::run() {
        
        
        int lineNumber = 1;
        //This while loops keep going as long as there are characters in the input string. Need to remove characters so not infinite loop
        while(inputFile.size() > 0) {
            if (inputFile[0] == '\n'){
                ++lineNumber;
                inputFile = inputFile.substr(1,inputFile.size());
                continue;
            }
            //initialize these two variables for the for loop
            if (isspace(inputFile[0])){
                inputFile = inputFile.substr(1,inputFile.size());
                continue;
            }
            
            int maxRead = 0;
            int maxMachine = 0;
            int maxNewLines = 0;
            
            //Loops through all the FSA in our machines vector
            for (unsigned int i = 0; i < machines.size(); ++i) {
                int FSAread = (machines.at(i))->Read(inputFile); //returns 0 if no match, num chars read if match.
                
                if (FSAread == -1) { //test for invalid string or comment so undefined
                    maxRead = -1;
                    break;
                }

                //Update the maximum read value if more than the current value and then update max machine to reference the current finite-state automaton
                else if (FSAread > maxRead) {
                    maxRead = FSAread;
                    maxMachine = i;
                    maxNewLines = (machines.at(i))->NewLinesRead();
                }
            }
            if (maxRead > 0) {
                //generate a token to push into token vector
                string tokenInput = inputFile.substr(0,maxRead);
                Token* newToken = (machines.at(maxMachine))->CreateToken(tokenInput, lineNumber);
                lineNumber += maxNewLines;
                tokens.push_back(newToken); //Stores the new token pointer in vector
            }
            else if (maxRead == -1) { //undefined reached EOF before end of comment or string
                maxRead = inputFile.size();
                string tokenInput = inputFile;
                int undefStart = lineNumber;
                for(unsigned int i = 0; i < tokenInput.size(); i++){
                    if (tokenInput[i] == '\n'){
                       lineNumber++;
                    }
                }
                Token* newToken = undefAuto->CreateToken(tokenInput, undefStart); //FIXME!!! Invalid token needs to be made so change machine not i
                tokens.push_back(newToken);
            }
            else { //No machine accepted the input, so it is invalid
                maxRead = 1;
                string tokenInput = inputFile.substr(0,maxRead);
                Token* newToken = undefAuto->CreateToken(tokenInput, lineNumber); //FIXME!!! Invalid token needs to be made so change machine not i
                tokens.push_back(newToken);
            }
            
            inputFile.erase(0,maxRead);//erases from the beginning the size of the string read
        }
        
        Token* newToken = new Token(EOF1, "", lineNumber);
        tokens.push_back(newToken);
}
