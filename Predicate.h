#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Parameter.h"

using namespace std;

class Predicate {
    public:
        Predicate(){ //default constructor
            name = "";
        };
        Predicate(string name, vector<Parameter*> parameterList){
            this->name = name;
            this->parameterList = parameterList;
        }; 
        string toString(){
            
            string total = name + "(";
            for (unsigned int i = 0; i < parameterList.size(); i++){
                total += parameterList.at(i)->toString();
                if(i < parameterList.size() - 1){
                    total+= ",";
                }
            }
            total += ")";
            
            return total;
        };
        vector<Parameter*> getParameterList() {
            return parameterList;
        }
        string getName(){
            return name;
        };
    private:
        string name;
        vector <Parameter*> parameterList;
        
};