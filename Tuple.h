#pragma once

#include <string>
#include <vector>

using namespace std;

class Tuple {
    public:
        //some constructors
        Tuple(){}; //default contructor
        Tuple(vector<string> tupleVect){
          this->tupleVect = tupleVect;  
        }; //parameterized cosntructor
        
        bool operator < (const Tuple & other) const {
            return (this->tupleVect < other.tupleVect);
            
        };

        string toString(){
            string total = "";
            for (unsigned int i = 0; i < tupleVect.size(); i++){
                total += tupleVect.at(i) + ", ";
            }
            return total;
        };
        vector<string> getTupleVect() const{
            return tupleVect;
        };
    
    private:
        vector<string> tupleVect;
};
