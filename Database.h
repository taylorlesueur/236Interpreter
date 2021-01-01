#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include "Relation.h"

using namespace std;

class Database{
    public:
        //some constructors
        Database(){}; //default contructor
       /*
        Database(){

        }; //parameterized cosntructor
        */
        bool addRelation(Relation newRelation){ //returns true if relation added successfully
            //cout << "Adding relation: " << newRelation.toString() << endl;
            if(relationMap.find(newRelation.getName()) != relationMap.end()){
                return false;
            }
            relationMap.insert(pair<string,Relation>(newRelation.getName(), newRelation));
            //cout << "Relation name is: " << newRelation.getName() << endl;
            return true;
        };
        
        void addTupleToRelation(Tuple newTuple, string relationName){
            //cout << "Trying to add tuple: " << newTuple.toString() << " to relation: " << relationName << endl;
            if (relationMap.find(relationName) == relationMap.end()) {
                //cout << "The relation cannot be found in the map" << endl;
            }
            relationMap.at(relationName).addTuple(newTuple);
            //cout << "Successfully added the tuple" << endl;
        };
        
        map<string,Relation> getRelationMap(){
            return relationMap;
        };
        
        Relation getRelation(string relationName){
            return relationMap.at(relationName);
        };
        
        bool unionn(string name, Relation& toBeJoined){
            return relationMap.at(name).unionn(toBeJoined);
        };
        
    
    private:
        map <string,Relation> relationMap;
        
};
