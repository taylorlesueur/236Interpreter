#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Predicate.h"

using namespace std;

class Rule {
    public:
        Rule(){}; //default constructor 
        Rule(Predicate headPred, vector <Predicate> bodyPreds){
            this->headPred = headPred;
            this->bodyPreds = bodyPreds;
        };
        string toString(){
            string total = headPred.toString();
            total += " :- ";
            for (unsigned int i = 0; i < bodyPreds.size(); i++){
                total += bodyPreds.at(i).toString();
                if (i == bodyPreds.size() - 1){
                    total += ".";
                }
                else {
                    total += ",";
                }
            }
            return total;
        };
        
        vector<Predicate> getBodyPreds(){
            return bodyPreds;
        };
        
        Predicate getHeadPred(){
            return headPred;
        };
    private:
        Predicate headPred;
        vector <Predicate> bodyPreds;
        
};