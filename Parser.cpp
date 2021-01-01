#include "Parser.h"

Parser::Parser() {
    
}

Parser::Parser(vector <Token*> tokenVect){
    this->tokenVect = tokenVect;
    this->tokenIndex = 0;
}

//****This function handles all the parsing at the top level
vector<Token*> Parser::parse(){
    //cout << "Start parsing" << endl;
    parseDatalog();
    datalogProgram.calculateDomain();
    //datalogProgram.print();
    return tokenVect;
}

//This function can be used to help unit test
void Parser::parseHelper(){
    cout << "Testing parsing" << endl;
}

bool Parser::match(TokenType type){
    while (tokenVect.at(tokenIndex)->getType() == COMMENT) {
        tokenIndex++;
    }
    if (type == tokenVect.at(tokenIndex)->getType()){
        //cout << "Its  a match: " << tokenVect.at(tokenIndex)->toString() << endl;
        tokenIndex++;
        return true;
    }
    //cout << "Its not a match: " << tokenVect.at(tokenIndex)->toString() << endl;
    return false;
}

string Parser::toString(){
    return "";
}

//****A bunch of parse functions for each production*****/////
////////////////////////////////////////////////////////////////
void Parser::parseDatalog(){
    if (!match(SCHEMES)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(COLON)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    
    Predicate firstScheme = parseScheme();
    datalogProgram.addScheme(firstScheme);
    parseSchemeList();

    //match(FACTS); //This is parsed as follow set of SchemeList
    if (!match(COLON)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    
    parseFactList();
    
    //match(RULES); //Parsed as follow set of FactList
    if (!match(COLON)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    parseRuleList();
    //match(QUERIES); //parsed as follow set of RuleList
    if (!match(COLON)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Predicate firstQuery = parseQuery();
    datalogProgram.addQuery(firstQuery);
    parseQueryList();
    //match(EOF1);//parsed as follow set of QueryList
}
////////////////////////////////////////////////////////////
void Parser::parseSchemeList(){
    //lambda production or base case
    if (match(FACTS)){
        return;
    }
    Predicate newScheme = parseScheme();
    datalogProgram.addScheme(newScheme);
    
    parseSchemeList();
}
/////////////////////////////////////////////////////////////
void Parser::parseFactList(){
    if (match(RULES)){
        return;
    }
    datalogProgram.addFact(parseFact());
    
    parseFactList();
}
//////////////////////////////////////////////////////////////
void Parser::parseRuleList(){
    if (match(QUERIES)){
        return;
    }
    datalogProgram.addRule(parseRule());
    parseRuleList();
}
///////////////////////////////////////////////////////////////
void Parser::parseQueryList(){
    if (match(EOF1)){
        //cout << "Success!" << endl;
        return;
    }
    Predicate newQuery = parseQuery();
    datalogProgram.addQuery(newQuery);
    parseQueryList();
}
//////////////////////////////////////////////////////////////
Predicate Parser::parseScheme(){
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string name = tokenVect.at(tokenIndex - 1)->getTokenVal();
    //cout << endl << "The scheme name is: " << name << endl;
    if (!match(LEFT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string idVal = tokenVect.at(tokenIndex - 1)->getTokenVal();
    Parameter* firstID = new IDParameter(idVal);
    idList.push_back(firstID);
    
    parseIdList();
    
    Predicate scheme(name,idList); ///
    idList.clear();
    
    /*//FixMe! to avoid memory leaks
    for(unsigned int i = 0; i < idList.size(); i++){
        delete idList.at(i);
        idList.erase(i);
        }
        */
    
    return scheme;
    /*
    if (!match(RIGHT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    */
}
////////////////////////////////////////////////////////////
Predicate Parser::parseFact(){
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string name = tokenVect.at(tokenIndex - 1)->getTokenVal();
    if (!match(LEFT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(STRING)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string stringVal = tokenVect.at(tokenIndex - 1)->getTokenVal();
    Parameter* firstString = new StringParameter(stringVal);
    stringList.push_back(firstString);
    
    parseStringList();
    
    Predicate newFact(name,stringList); ///
    stringList.clear();
    /* //follow set for stringList so already parsed there
    if (!match(RIGHT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    */
    if (!match(PERIOD)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    return newFact;
}
///////////////////////////////////////////////////////////////
Rule Parser::parseRule(){
    Predicate headPred = parseHeadPredicate();
    
    if (!match(COLON_DASH)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Predicate firstPred = parsePredicate();
    rulePreds.push_back(firstPred);
    parsePredicateList();
    
    Rule myRule(headPred, rulePreds);
    rulePreds.clear();
    ////FIXME^^ memory leak right here
    return myRule;
    /*
    if (!match(PERIOD)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    *///follow set of predicateList
}
////////////////////////////////////////////////////////////////
Predicate Parser::parseQuery(){
    Predicate myQuery = parsePredicate();
    if (!match(Q_MARK)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    return myQuery;
}
/////////////////////////////////////////////////////////////////
Predicate Parser::parseHeadPredicate(){
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string headPredName = tokenVect.at(tokenIndex - 1)->getTokenVal();
    if (!match(LEFT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* firstHeadPredID = new IDParameter(tokenVect.at(tokenIndex - 1)->getTokenVal());
    idList.push_back(firstHeadPredID);

    parseIdList();
    
    Predicate headPred(headPredName, idList);
    idList.clear();
    return headPred;
    ///FIXME memory leaks here^^
    /* //follow set of parseIDList
    if (!match(RIGHT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    */
}
///////////////////////////////////////////////////////////////////
Predicate Parser::parsePredicate(){
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    string name = tokenVect.at(tokenIndex - 1)->getTokenVal();
    if (!match(LEFT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* firstParam = parseParameter();
    paramList.push_back(firstParam);
    parseParameterList();
    
    Predicate myPred(name, paramList);
    paramList.clear();
    return myPred;
    /* //FOLLOW set for parameterList so already parsed there
    if (!match(RIGHT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    */
}
/////////////////////////////////////////////////////////////////////
void Parser::parsePredicateList(){
    //follow set to stop recurring, base case
    if (match(PERIOD)){
        return;
    }
    //First set
    if (!match(COMMA)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Predicate nextPred = parsePredicate();
    rulePreds.push_back(nextPred);
    parsePredicateList();
}
//////////////////////////////////////////////////////////////////////
void Parser::parseParameterList(){
    //follow set base case to stop recursion and return to predicate
    if (match(RIGHT_PAREN)){
        return;
    }
    //First set
    if (!match(COMMA)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* nextParam = parseParameter();
    paramList.push_back(nextParam);
    parseParameterList();
}
/////////////////////////////////////////////////////////////////////////
void Parser::parseStringList(){
    //follow set or base case
    if (match(RIGHT_PAREN)){
        return;
    }
    //First set of string list
    if (!match(COMMA)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(STRING)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* nextString = new StringParameter(tokenVect.at(tokenIndex - 1)->getTokenVal());
    stringList.push_back(nextString);
    
    parseStringList();
}
///////////////////////////////////////////////////////////////////////////
void Parser::parseIdList(){
    //lamda production or base case. FOLLOW SET
    if (match(RIGHT_PAREN)){
        return;
    }
    //other productions and recursion
    if (!match(COMMA)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    if (!match(ID)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* nextID = new IDParameter(tokenVect.at(tokenIndex - 1)->getTokenVal());
    idList.push_back(nextID);
    
    parseIdList();
}
////////////////////////////////////////////////////////////////////////
Parameter* Parser::parseParameter(){
    if (match(STRING)){
        Parameter* stringParam = new StringParameter(tokenVect.at(tokenIndex - 1)->getTokenVal());
        return stringParam;
    }
    if (match(ID)){
        Parameter* idParam = new IDParameter(tokenVect.at(tokenIndex - 1)->getTokenVal());
        return idParam;
    }
    if (match(LEFT_PAREN)){
        return parseExpression();
    }
    throw(tokenVect.at(tokenIndex)->toString());

}
////////////////////////////////////////////////////////////////////////////
Parameter* Parser::parseExpression(){
    /* //follow set of parseParameter, so already parsed in that function
    if (!match(LEFT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    */
    Parameter* leftParam = parseParameter();
    string op = parseOperator();
    Parameter* rightParam = parseParameter();
    if (!match(RIGHT_PAREN)){
        throw(tokenVect.at(tokenIndex)->toString());
    }
    Parameter* myExpression = new Expression(leftParam, rightParam, op);
    return myExpression;
}
////////////////////////////////////////////////////////////////////////////
string Parser::parseOperator(){
    if (match(ADD)){
        return "+";
    }
    if (match(MULTIPLY)){
        return "*";
    }
    throw(tokenVect.at(tokenIndex)->toString());
}
//////////////////////////////////////////////////////////////////////////////
DatalogProgram Parser::getDatalogProgram(){
    return this->datalogProgram;
};