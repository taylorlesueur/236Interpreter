#pragma once

#include <string>
#include <vector>

using namespace std;

class Header {
    public:
        //some constructors
        Header(){}; //default contructor
        Header(vector<string> headerVect){
          this->headerVect = headerVect;  
        }; //parameterized cosntructor
        
        //this method returns a string in the specified token format with no space
        string toString(){
            string total = "";
            for (unsigned int i = 0; i < headerVect.size(); i++){
                total += headerVect.at(i) + ", ";
            }
            return total;
        };
        vector<string> getHeaderVect() const{
            return headerVect;
        };
    
    private:
        vector<string> headerVect;
};
