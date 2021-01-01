#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include "Database.h"
#include "DatalogProgram.h"
#include "Graph.h"


using namespace std;

class Interpreter{
    public:
        //some constructors
        Interpreter(){}; //default contructor
        Interpreter(DatalogProgram& datalogProgram){
            //cout << "Started constructor!" << endl;
            this->datalogProgram = datalogProgram;
            for (unsigned int i = 0; i < datalogProgram.getSchemeList().size(); i++){
                Predicate curScheme = datalogProgram.getSchemeList().at(i);
                vector<string> headerVect;
                for (unsigned int j = 0; j < curScheme.getParameterList().size(); j++) {
                    headerVect.push_back(curScheme.getParameterList().at(j)->toString());
                }
                Header newHeader(headerVect);
                Relation newRelation(curScheme.getName(),newHeader);
                myDatabase.addRelation(newRelation);
            }
            for (unsigned int i = 0; i < datalogProgram.getFactList().size(); i++) {
                Predicate curFact = datalogProgram.getFactList().at(i);
                vector<string> parameterVect;
                for (unsigned int j = 0; j < curFact.getParameterList().size(); j++) {
                    parameterVect.push_back(curFact.getParameterList().at(j)->toString());
                }
                Tuple newTuple(parameterVect);
                myDatabase.addTupleToRelation(newTuple,curFact.getName());
            }

        }; //parameterized cosntructor
        
        
        Relation evaluatePredicate(Predicate& p){
            Relation myTable = myDatabase.getRelation(p.getName());
            map<string, int> variableMap;
            vector<string> projRenameVariables;
            vector<Parameter*> paramList = p.getParameterList();
            for (unsigned int i = 0; i < paramList.size(); i++){
                //variableMap.insert(pair<string,int>(paramList.at(i)->toString(), i));
                if (paramList.at(i)->isConstant()){
                    myTable = myTable.select(i, paramList.at(i)->toString());
                }
                else {
                    if (variableMap.find(paramList.at(i)->toString()) != variableMap.end()) {
                        int colIndex = variableMap.find(paramList.at(i)->toString())->second;
                        myTable = myTable.select(i,colIndex);
                    }
                    else {
                        variableMap.insert(pair<string,int>(paramList.at(i)->toString(), i));
                        projRenameVariables.push_back(paramList.at(i)->toString());
                    }
                }
            }
            vector<int> projectColumns;
            for (unsigned int i = 0; i < projRenameVariables.size(); i++){
                projectColumns.push_back((variableMap.at(projRenameVariables.at(i))));
            }

            myTable = myTable.project(projectColumns);
            
            Header newHeader(projRenameVariables);
            myTable = myTable.renamee(newHeader);
            
            return myTable;
        };
        
        
        void evaluateAllQueries(){
            cout << "Query Evaluation" << endl;
            vector<Relation> relationVect;
            for (unsigned int i = 0; i < datalogProgram.getQueryList().size(); i++){
                Predicate curQuery = datalogProgram.getQueryList().at(i);
                Relation outputRelation = evaluatePredicate(curQuery);

                relationVect.push_back(outputRelation);
            }

            for (unsigned int i = 0; i < datalogProgram.getQueryList().size(); i++){
                Predicate curQuery = datalogProgram.getQueryList().at(i);
                unsigned int relationSize = relationVect.at(i).getTupleSet().size();
                if(relationSize != 0){
                    cout << curQuery.toString() << "? Yes(" << relationSize << ")" << endl;
                    cout << relationVect.at(i).toString();
                }
                else {
                    cout << curQuery.toString() << "? No" << endl;
                }
                
            }
        };
        
        Relation joinRelations(vector<Relation>& relationVect){
            Relation result = relationVect.at(0);
            for(unsigned int i = 1; i < relationVect.size(); i++){
                result.join(relationVect.at(i));
            }
            return result;
        };
        
        void evaluateAllRules(){
            vector<Relation> relationVect;
            cout << "Rule Evaluation" << endl;
            //fixed point algorithm
            int passNum = 0; 
            bool addedTuple;
            do {
                addedTuple = false;
                passNum++;
                for (unsigned int i = 0; i < datalogProgram.getRuleList().size(); i++){
                    cout << datalogProgram.getRuleList().at(i).toString() << endl;
                    if (evaluateRule(datalogProgram.getRuleList().at(i))){
                        addedTuple = true;
                    }
                }
            } while(addedTuple == true);
            
            cout << endl << "Schemes populated after " << passNum << " passes through the Rules." << endl << endl;
        };
        
        
        bool evaluateRule(Rule& myrule){
            vector<Relation> relationVect;

            for(unsigned int i = 0; i < myrule.getBodyPreds().size(); i++){
                relationVect.push_back(evaluatePredicate(myrule.getBodyPreds().at(i)));
            }
   
            Relation resultRelation = joinRelations(relationVect);
            //cout << resultRelation.toString() << endl;
            
            vector<Parameter*> headParams = myrule.getHeadPred().getParameterList();
            vector<string> headColumns;
            for(unsigned int i = 0; i < headParams.size(); i++){
                headColumns.push_back(headParams.at(i)->toString());
            }
            vector<string> resultHeadVect = resultRelation.getHeader().getHeaderVect();
            vector<int> newHeadCols;
            
            Header newHeader(headColumns);
            Relation headRelation(myrule.getHeadPred().getName(), newHeader);
            //cout << headRelation.toString() << endl;
            
            for(unsigned int i = 0; i < headColumns.size(); i++){
                for (unsigned int j = 0; j < resultHeadVect.size(); j++){
                    if(resultHeadVect.at(j) == headColumns.at(i)){
                        newHeadCols.push_back((signed)j);
                    }
                }
            }

            resultRelation = resultRelation.project(newHeadCols);
            return myDatabase.unionn(myrule.getHeadPred().getName(), resultRelation);
        };
        
        void evaluateRulesOptimized(){
            Graph dependencyGraph = buildDependencyGraph();
            Graph revDependencyGraph = reverseDependencyGraph();
            vector<int> postorder = revDependencyGraph.dfsForestPost();
            vector<set<int>> forest = dependencyGraph.dfsForest(postorder);
            
            cout << "Dependency Graph" << endl;
            cout << dependencyGraph.toString() << endl;
            
            vector<Relation> relationVect;
            cout << "Rule Evaluation" << endl;
            
            for(unsigned int i = 0; i < forest.size(); i++){
                cout << "SCC: ";
                if (dependencyGraph.isTrivial(forest.at(i))) {
                    cout << "R" << *forest.at(i).begin() << endl;
                    cout << datalogProgram.getRuleList().at(*forest.at(i).begin()).toString() << endl;
                    evaluateRule(datalogProgram.getRuleList().at(*forest.at(i).begin()));
                    cout << "1 passes: R" << *forest.at(i).begin() << endl;
                }
                
                else { //fixed point algorithm for non-trivial SCC
                    vector<Rule> tempRules;
                    string total = "";
                    for (int s : forest.at(i)){
                        tempRules.push_back(datalogProgram.getRuleList().at(s));
                        total += "R" + to_string(s) + ",";
                    }
                    total.pop_back();
                    cout << total << endl;
                    int passNum = 0; 
                    bool addedTuple;
                    do {
                        addedTuple = false;
                        passNum++;
                        for (unsigned int j = 0; j < tempRules.size(); j++){
                            cout << tempRules.at(j).toString() << endl;
                            if (evaluateRule(tempRules.at(j))){
                                addedTuple = true;
                            }
                        }
                    } while(addedTuple == true);
                    cout << passNum << " passes: " << total << endl;
                }
            }
            
            cout << endl;
        };
        
        Graph buildDependencyGraph(){
            Graph dependencyGraph;
            for (unsigned int i = 0; i < datalogProgram.getRuleList().size(); i++){
                dependencyGraph.addNode(i);
            }
            for (unsigned int i = 0; i < datalogProgram.getRuleList().size(); i++){
                for(unsigned int j = 0; j < datalogProgram.getRuleList().size(); j++){
                    for(unsigned int k = 0; k < datalogProgram.getRuleList().at(j).getBodyPreds().size(); k++){
                        if(datalogProgram.getRuleList().at(j).getBodyPreds().at(k).getName() == datalogProgram.getRuleList().at(i).getHeadPred().getName()){
                            dependencyGraph.addDependency(j,i);
                            //cout << "R" << i << " is dependent on R" << j << endl;
                        }
                    }
                }
            }
            /*
            vector<int> postorder = dependencyGraph.dfsForestPost();
            for (unsigned int i = 0; i < postorder.size(); i++){
                cout << postorder.at(i) << ",";
            }
            cout << endl;
            */
            return dependencyGraph;
        };
        
        Graph reverseDependencyGraph(){
            Graph revDependencyGraph;
            for (unsigned int i = 0; i < datalogProgram.getRuleList().size(); i++){
                revDependencyGraph.addNode(i);
            }
            for (unsigned int i = 0; i < datalogProgram.getRuleList().size(); i++){
                for(unsigned int j = 0; j < datalogProgram.getRuleList().size(); j++){
                    for(unsigned int k = 0; k < datalogProgram.getRuleList().at(j).getBodyPreds().size(); k++){
                        if(datalogProgram.getRuleList().at(j).getBodyPreds().at(k).getName() == datalogProgram.getRuleList().at(i).getHeadPred().getName()){
                            revDependencyGraph.addDependency(i,j);
                        }
                    }
                }
            }
            /*
            vector<int> postorder = revDependencyGraph.dfsForestPost();
            for (unsigned int i = 0; i < postorder.size(); i++){
                cout << postorder.at(i) << ",";
            }
            cout << endl;
            */
            return revDependencyGraph;
        };
        
        
        /*
        void testJoin(){
            vector<string> h1vect = {"x", "a"};
            vector<string> h2vect = {"y", "b"};
            vector<string> h3vect = {"z", "c"};
            Relation alpha("alpha", h1vect);
            Relation beta("beta", h2vect);
            Relation charlie("charlie",h3vect);
            
            //vector<string> tuple1 = {"1","2","5"};
            //vector<string> tuple2 = {"1","4","1"};
            //vector<string> tuple3 = {"2","3","2"};
            //vector<string> tuple4 = {"3","3","2"};
            //vector<string> tuple5 = {"6","7","4"};
            
            Tuple tup1({"eggs","bacon"});
            Tuple tup2({"oatmeal","apple"});
            
            alpha.addTuple(tup1);
            alpha.addTuple(tup2);
           
            Tuple tup3({"sandwhich","banana"});
            Tuple tup4({"pasta","soup"});

            
            beta.addTuple(tup3);
            beta.addTuple(tup4);
            
            Tuple tup5({"pasta","broccoli"});
            
            charlie.addTuple(tup5);
            
            vector<Relation> relVect;
            relVect.push_back(alpha);
            relVect.push_back(beta);
            relVect.push_back(charlie);
            
            alpha.join(beta);
            alpha.join(charlie);
            cout << alpha.toString();
            
            Relation result = joinRelations(relVect);
            cout << result.toString();
        };
        */
        
    
    private:
        DatalogProgram datalogProgram;
        Database myDatabase;
};
