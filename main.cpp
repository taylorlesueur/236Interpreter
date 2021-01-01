#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "Token.h"
#include "Lexer.h"
//#include "Automaton.h"
//#include "MatcherAutomaton.h"
//#include "IDAutomaton.h"
#include "Parser.h"
//#include "Relation.h"
//#include "Database.h"
//#include "DatalogProgram.h"
#include "Interpreter.h"


using namespace std;

int main(int argc, char* argv[]) {
    
    string inputFileName = argv[1];
    //creates an inputstream instance called fin
    ifstream fin (inputFileName);
    //this string will be the entire input file in characters.
    string inputString;
    

    //Checks if file opened properly. If it did, loops through the whole input file and 
    //stores each character into a string
    if (fin.is_open()) {
      // cout << inputFileName << " File opened" << endl;
       char addChar;
       while (fin.get(addChar)){
           inputString += addChar;
       }
    }
    
    //if the fin did not open we had an error reading/opening the file
    else {
        cout << "Error. File did not open" << endl;
    }
 
    Lexer myLexer(inputString);
    myLexer.run();
    
    vector <Token*> tokens = myLexer.getTokens();
   
    Parser myParse(tokens);
    
    try {
         myParse.parse();
    } catch(string msg){
        cout << "Failure!" << endl;
        cout << "  " <<  msg << endl;
    }
    
    //cout << "Its relation time: " << endl << endl;
    DatalogProgram myDatalogProgram = myParse.getDatalogProgram();
    Interpreter myInterpreter(myDatalogProgram);
    myInterpreter.evaluateRulesOptimized();
    //myInterpreter.testJoin();
    myInterpreter.evaluateAllQueries();
    //cout << myInterpreter.reverseDependencyGraph().toString() << endl;
    //myInterpreter.findSECS();
    //myInterpreter.testJoin();
    
    return 0;
}
