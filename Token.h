//
// Created by Taylor LeSueur on 9/14/20.
//

#ifndef PROJECT1_TOKEN_H
#define PROJECT1_TOKEN_H

#include <string>

using namespace std;

//An enum of token types. The End of file is EOF1 because the other is a reserved word
enum TokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, EOF1};

class Token {
    public:
        //some constructors
        Token(); //default contructor
        Token(TokenType type, string tokenVal, int numLine); //parameterized cosntructor
        
        //this method returns a string in the specified token format with no space
        string toString();
        TokenType getType(){return this->type;};
        string getTokenVal();
    
    private:
        string tokenVal; //this is the actual string of the token itself ie ","
        int numLine; // this is the linnumber of the token starting
        TokenType type; //this is the enum of the type of token
};


#endif //PROJECT1_TOKEN_H
