//
// Created by Taylor LeSueur on 9/14/20.
//

#include "Token.h"

//the default constructor. Initializes everything to null/0
Token::Token() {
    tokenVal = "";
    numLine = 0;
    type = UNDEFINED;
}

//The parameterized constructor. Accepts the type, then value, then line number
Token::Token(TokenType type, string tokenVal, int numLine) {
    this->type = type;
    this->tokenVal = tokenVal;
    this->numLine = numLine;
}

//This method returns a string in the specified token format, no spaces
string Token::toString() {

    string tokenInfo = "(";
    
    //switch statement to check the type and add that to the empty string
    switch(this->type) {
        case COMMA :
            tokenInfo += "COMMA";
            break;
        case PERIOD :
            tokenInfo += "PERIOD";
            break;
        case Q_MARK :
            tokenInfo += "Q_MARK";
            break;
        case LEFT_PAREN :
            tokenInfo += "LEFT_PAREN";
            break;
        case RIGHT_PAREN :
            tokenInfo += "RIGHT_PAREN";
            break;
        case COLON :
            tokenInfo += "COLON";
            break;
        case COLON_DASH :
            tokenInfo += "COLON_DASH";
            break;
        case MULTIPLY :
            tokenInfo += "MULTIPLY";
            break;
        case ADD :
            tokenInfo += "ADD";
            break;
        case SCHEMES :
            tokenInfo += "SCHEMES";
            break;
        case FACTS :
            tokenInfo += "FACTS";
            break;
        case RULES :
            tokenInfo += "RULES";
            break;
        case QUERIES :
            tokenInfo += "QUERIES";
            break;
        case ID :
            tokenInfo += "ID";
            break;
        case STRING :
            tokenInfo += "STRING";
            break;
        case COMMENT :
            tokenInfo += "COMMENT";
            break;
        case UNDEFINED :
            tokenInfo += "UNDEFINED";
            break;
        case EOF1 :
            tokenInfo += "EOF";
            break;
    }
    //concatenates the rest of the info to the string
    tokenInfo += ",\"" + this->tokenVal + "\"," + to_string(this->numLine) + ")";
    return tokenInfo;
}

string Token::getTokenVal(){
    return tokenVal;
}
