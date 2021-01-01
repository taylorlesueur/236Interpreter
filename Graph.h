#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>


using namespace std;

class Graph {
    public:
        Graph(){}; //default constructor 
        Graph(map<int,set<int>> adjacencyList){
            this->adjacencyList = adjacencyList;
        };
        string toString(){
            string total = "";
            for(auto it : adjacencyList){
                total += "R" + to_string(it.first) + ":";
                for(auto t : it.second){
                    total += "R" + to_string(t) + ",";
                }
                if(total.back() == ','){
                    total.pop_back();
                }
                total += "\n";
            }
            return total;
        };
        
        bool isTrivial(set<int> SEC){
            if (SEC.size() != 1){
                return false;
            }
            int key = *SEC.begin();
            for(auto t : adjacencyList.at(key)) {
                if(t == key ){
                    return false;
                }
            }
            return true;
        };
        
        set<int> getAdjacentVerts(int vertex){
            return adjacencyList.at(vertex);
        };
        
        void addNode(int ruleNum){
            set<int> newSet;
            adjacencyList.insert(pair<int, set<int>>(ruleNum,newSet));
        };
        
        void addDependency(int ruleNum, int dependentNum){
            adjacencyList.at(ruleNum).insert(dependentNum);
        };
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        vector<int> dfsPost(int vertex){
            vector <int> postOrder;
            visitedList.at(vertex) = true; //mark v
            for (auto w : adjacencyList.at(vertex)){ //for each vertex w adjacent from v
                if(visitedList.at(w) != true){
                    vector <int> post = dfsPost(w); //recursion
                    for (unsigned int i = 0; i < post.size(); i++){ //add the postorder to the overall postorder
                        postOrder.push_back(post.at(i));
                        //cout << "adding " << post.at(i) << " to postorder" << endl;
                    }
                }
            }
            postOrder.push_back(vertex);
            return postOrder;
        };
        
        
        vector<int> dfsForestPost(){
            //initialize the visitedList to the right size, all false
            visitedList.clear();
            vector<int> postOrder;
            for (auto vertex : adjacencyList){
                visitedList.push_back(false);
            }
            
            //the actual dfsForest algorithm for postorder
            for(auto vertex: adjacencyList){
                if(visitedList.at(vertex.first) != true){ //if the node isnt marked
                    vector<int> post = dfsPost(vertex.first);
                    //add the postorder returned by the one dfs tree to the overall postorder
                    for (unsigned int i = 0; i < post.size(); i++){
                        postOrder.push_back(post.at(i));
                    }
                }
            }
            return postOrder;
        };
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        void dfsTree(int vertex, set<int>& outputSCC){
            visitedList.at(vertex) = true; //mark v
            //cout << vertex << " visited in tree" << endl;
            
            set<int>adjacentVerts = adjacencyList.at(vertex);
            for (auto w : adjacentVerts){ //for each vertex w adjacent from v
                if(visitedList.at(w) == false){
                    dfsTree(w, outputSCC); //recursion
                }
            }
            outputSCC.insert(vertex);
            //cout << vertex << " added to SEC" << endl;
        };
        
        
        vector<set<int>> dfsForest(vector<int> postorder){
            visitedList.clear();
            for (unsigned int i = 0; i < postorder.size(); i++){
                visitedList.push_back(false);
                //cout << i << " initalized to false" << endl;
            }
            
            vector<set<int>> forest;
            int size = postorder.size();
            for(int i = size - 1; i > -1; i--){
                //cout << "vertex is " << postorder.at(i) << endl;
                set<int> tempSCC;
                if(visitedList.at(postorder.at(i)) != true){
                    //cout << "calling dfsTree on vertex: " << postorder.at(i) << endl;
                    dfsTree(postorder.at(i),tempSCC);
                    forest.push_back(tempSCC);
                }
            }
            return forest;
        };
        

    private:
        map<int,set<int>> adjacencyList;
        vector<bool> visitedList;
};

