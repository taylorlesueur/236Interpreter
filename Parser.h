#pragma once

#include "Token.h"
#include <vector>
#include <string>
#include <iostream>
#include "DatalogProgram.h"

using namespace std;

class Parser {
    public:
        Parser(); //default constructor
        Parser(vector <Token*> tokenVect); //parameterized constructor
        
        vector<Token*> parse(); //start the parsing
        void parseHelper(); //to complete unit tests
        
        bool match(TokenType type);
        string toString();
        
        //functions for each production in the grammar
        void parseDatalog();
        void parseSchemeList();
        void parseFactList();
        void parseRuleList();
        void parseQueryList();
        Predicate parseScheme();
        Predicate parseFact();
        Rule parseRule();
        Predicate parseQuery();
        Predicate parseHeadPredicate();
        Predicate parsePredicate();
        void parsePredicateList();
        void parseParameterList();
        void parseStringList();
        void parseIdList();
        Parameter* parseParameter();
        Parameter* parseExpression();
        string parseOperator();
        DatalogProgram getDatalogProgram();
        
        
        
    private:
        vector <Token*> tokenVect;
        int tokenIndex;
        DatalogProgram datalogProgram;
        
        //container variables to be cleared after each use
        vector<Parameter*> idList;
        vector<Parameter*> stringList;
        vector<Predicate> rulePreds;
        vector<Parameter*> paramList;
       
};