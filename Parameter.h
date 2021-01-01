#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Parameter {
    public:
        Parameter(){}; //default constructor
        //~Parameter();
        virtual string toString() = 0;
        bool isConstant(){
            return isConstantVal;
        };
        virtual string getParamVal() = 0;
    protected:
        bool isConstantVal;
        

        
};


class StringParameter : public Parameter {
    public:
        StringParameter() {paramVal = ""; isConstantVal = true;}; //default constructor
        StringParameter(string paramVal){
            this->paramVal = paramVal;
            isConstantVal = true;
        };
        string toString() {
            return paramVal;
        };
        string getParamVal(){
            return paramVal;
        };
    private:
        string paramVal;
        
};

class IDParameter : public Parameter {
    public:
        IDParameter() {paramVal = ""; isConstantVal = false;}; //default constructor
        IDParameter(string paramVal){
            this->paramVal = paramVal;
            isConstantVal = false;
        };
        ~IDParameter(){};
        string toString() {
            return paramVal;
        };
        string getParamVal() {
            return "Error: requesting paramVal of ID parameter - not string";
        };
    private:
        string paramVal;
        
};

class Expression : public Parameter {
    public:
        Expression() {leftParam = NULL; rightParam = NULL; op = ""; isConstantVal = false;}; //default constructor
        Expression(Parameter* leftParam, Parameter* rightParam, string op){
            this->leftParam = leftParam;
            this->rightParam = rightParam;
            this->op = op;
            isConstantVal = false;
        };
        string toString() {
            string total = "("+ leftParam->toString() + op + rightParam->toString() + ")";
            return total;
        };
        string getParamVal() {
            return "Error: requesting paramVal of Expression parameter - not string";
        };
    private:
        Parameter* leftParam;
        Parameter* rightParam;
        string op;
};