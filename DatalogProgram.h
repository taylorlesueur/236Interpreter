#pragma once

#include "Token.h"
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "Predicate.h"
#include "Rule.h"


using namespace std;

class DatalogProgram {
    public:
        DatalogProgram(){
        }; //default constructor
        void print(){ 
           cout << "Schemes(" << schemeList.size() << "):" << endl;
           for (unsigned int i = 0; i < schemeList.size(); i++){
               cout << "  " << schemeList.at(i).toString() << endl;
           }
           
           cout << "Facts(" << factList.size() << "):" << endl;
           for (unsigned int i = 0; i < factList.size(); i++){
               cout << "  " << factList.at(i).toString() << "." << endl;
           }
           
           cout << "Rules(" << ruleList.size() << "):" << endl;
           for (unsigned int i = 0; i < ruleList.size(); i++){
               cout << "  " << ruleList.at(i).toString() << endl;
           }
           
           cout << "Queries(" << queryList.size() << "):" << endl;
           for (unsigned int i = 0; i < queryList.size(); i++){
               cout << "  " << queryList.at(i).toString() << "?" << endl;
           }
           
           cout << "Domain(" << domainList.size() << "):" << endl;
           for (auto it = domainList.begin(); it != domainList.end(); ++it){
               cout << "  " << *it << endl;
           }
            
        };
        
        void addScheme(Predicate schemePred){
           schemeList.push_back(schemePred);
        };
        void addFact(Predicate factPred){
           factList.push_back(factPred);
        };
        void addRule(Rule myRule){
           ruleList.push_back(myRule);
        };
        void addQuery(Predicate queryPred){
           queryList.push_back(queryPred);
        };
        void calculateDomain(){
           for(unsigned int i = 0; i < factList.size(); i++){
               vector <Parameter*> paramList = factList.at(i).getParameterList();
               for (unsigned int j = 0; j < paramList.size(); j++){
                   domainList.insert(paramList.at(j)->getParamVal());
               }
           }
        }; 
        
        vector <Predicate> getSchemeList(){
            return schemeList;
        };
        vector <Predicate> getFactList(){
            return factList;
        };
        vector <Rule> getRuleList(){
            return ruleList;
        };
        vector <Predicate> getQueryList(){
            return queryList;
        };
        
    private:
        vector <Predicate> schemeList;
        vector <Predicate> factList;
        vector <Rule> ruleList;
        vector <Predicate> queryList;
        set <string> domainList;
        
};