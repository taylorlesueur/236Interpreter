#pragma once

#include <string>
#include <vector>
#include <set>
#include <utility>
#include "Header.h"
#include "Tuple.h"

using namespace std;

class Relation {
    public:
        //some constructors
        Relation(){}; //default contructor
        Relation(string name, Header header){
            this->name = name;
            this->header = header;
            set<Tuple> tupleSet;
        }; //parameterized cosntructor
        
        string toString(){
            string total = "";
            //cout << "Tostring called" << endl;
            if (tupleSet.size() == 0) {
                //cout << "Empty tuple set for relation: " << name << endl;
            }
            for (const Tuple& t : tupleSet){

                for (unsigned int i = 0; i < header.getHeaderVect().size(); i++){
                    if (i == header.getHeaderVect().size() - 1) {
                        total += "  " + header.getHeaderVect().at(i) + "=" + t.getTupleVect().at(i) + "\n";
                    }
                    else{
                        //cout << "Header vect at " << i << " is: " << header.getHeaderVect().at(i) << endl;
                        total += "  " + header.getHeaderVect().at(i) + "=" + t.getTupleVect().at(i) + ", ";
                    }
                }
            }
            return total;
        };
        
        void addTuple(Tuple newTuple){
            tupleSet.insert(newTuple);
        };
        
        string getName(){
            return name;
        };
        
        Header getHeader(){
            return header;
        };
        
        set<Tuple> getTupleSet(){
            return tupleSet;
        };
        
        Relation select(int column1, string stringVal){ //type 1
        //cout << "Running select 1 with params: " << column1 << ", " << stringVal << endl;
            
            Relation newRelation(name, header);
            
            for (const Tuple& t: tupleSet) {
                //cout << "first select. stringVal is: " << stringVal << " and the tuple column element here is: " << curTupleVect.at(column1) << endl;
                if(t.getTupleVect().at(column1) == stringVal) {
                    //cout << "first select. MAtch found and tuple added" << endl;
                    newRelation.addTuple(t);
                }
            }
            //cout << "First select finished And newRelation is: " << endl << newRelation.toString() << endl;
            return newRelation;
        };
        
        Relation select(int column1, int column2) { //type 2
            Relation newRelation(name, header);
            
           // cout << "Running select 2 with params: " << column1 << ", " << column2 << endl;
            for (const Tuple& t: tupleSet) {
                if(t.getTupleVect().at(column1) == t.getTupleVect().at(column2)) {
                    newRelation.addTuple(t);
                }
            }
            //cout << "Second select finished And newRelation is: " << endl << newRelation.toString() << endl;
            return newRelation;
        };
        
        
        Relation project(vector<int>& columnIndex){

            vector<string> newHeaderVect;
            //cout << endl << "column indices are: ";
            for (unsigned int i = 0; i < columnIndex.size(); i++){
               // cout << columnIndex.at(i) << ", ";
                newHeaderVect.push_back(header.getHeaderVect().at(columnIndex.at(i)));
            }
            Header newHeader(newHeaderVect);
            Relation newRelation(name, newHeader);
            
            //add correct tuple columns into the new relation
            for (const Tuple& t: tupleSet) {
                vector<string> newTupleVect;
                for (unsigned int i = 0; i < columnIndex.size(); i++){
                    newTupleVect.push_back(t.getTupleVect().at(columnIndex.at(i)));
                }
                Tuple newTuple(newTupleVect);
                newRelation.addTuple(newTuple);
            }
            return newRelation;
        };
        
        
        Relation renamee(Header& newHeader){

            Relation newRelation(name, newHeader);
            for (const Tuple& t: tupleSet) {
                newRelation.addTuple(t);
            }
            return newRelation;
        };
        
        
        void join(Relation& toJoin){
            header = combineHeaders(header, toJoin.header);
            set<Tuple> oldSet = tupleSet;
            tupleSet.clear();
            for (const Tuple& t: oldSet) {
                for (const Tuple& join: toJoin.tupleSet) {
                    if (isJoinable(&t, &join, colCombine)) {
                        Tuple newTuple = combineTuples(t, join, colCombine);
                        tupleSet.insert(newTuple);
                    }
                }
            }
            colCombine.clear(); //clear the private member variable
        };
        

        Header combineHeaders(const Header& header1, const Header& header2){

            vector<string> newHeaderVect = header1.getHeaderVect();
            
            for(unsigned int i = 0; i < header2.getHeaderVect().size(); i++){
                bool match = false;
                for (unsigned int j = 0; j < header1.getHeaderVect().size(); j++){
                    if (header2.getHeaderVect().at(i) == header1.getHeaderVect().at(j)){
                        match = true;
                        pair<int,int> matchCols = {(signed)j, (signed)i};
                        colCombine.push_back(matchCols);
                    }
                }
                if(match == false) {
                    newHeaderVect.push_back(header2.getHeaderVect().at(i));
                }
            }
            Header newHeader(newHeaderVect);
            return newHeader;
        };
        
        bool isJoinable(const Tuple* tuple1, const Tuple* tuple2, vector<pair<int,int>>& colShouldMatch){
            for(unsigned int colset = 0; colset < colShouldMatch.size(); colset++){
                if (tuple1->getTupleVect().at(colShouldMatch.at(colset).first) != tuple2->getTupleVect().at(colShouldMatch.at(colset).second)) {
                    return false;
                }
            }
            return true;
        };
        
        Tuple combineTuples(const Tuple& tuple1, const Tuple& tuple2, vector<pair<int,int>>& colShouldMatch){
            
            vector<string> newTupleVect = tuple1.getTupleVect();
            
            for(unsigned int tuple2col = 0; tuple2col < tuple2.getTupleVect().size(); tuple2col++) {
                bool matchcol = false;
                for (unsigned int matchColIndex = 0; matchColIndex < colShouldMatch.size(); matchColIndex++){
                    if ((signed)tuple2col == colShouldMatch.at(matchColIndex).second) {
                        matchcol = true;
                    }
                }
                if(matchcol != true){
                    newTupleVect.push_back(tuple2.getTupleVect().at(tuple2col));
                }
            }
            Tuple newTuple(newTupleVect);
            return newTuple;
        };
        
        
        bool unionn(Relation& toBeJoined){
            bool runAgain = false;
            for(const Tuple& toJoin : toBeJoined.tupleSet){
                if (tupleSet.insert(toJoin).second) {
                    runAgain = true;
                    cout << "  ";
                    for (unsigned int i = 0; i < header.getHeaderVect().size(); i++){
                        if(i == header.getHeaderVect().size() - 1){
                            cout << header.getHeaderVect().at(i) << "=" << toJoin.getTupleVect().at(i) << endl;
                        }
                        else{
                            cout << header.getHeaderVect().at(i) << "=" << toJoin.getTupleVect().at(i) + ", ";
                        }
                    }
                }
            }
            return runAgain;
        };
        
        
        

    
    private:
        string name;
        Header header;
        set<Tuple> tupleSet;
        vector <pair<int, int>> colCombine; //modified only by headermatch
        
};
