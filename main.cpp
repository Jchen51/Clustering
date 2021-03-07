//
//  main.cpp
//  Clustering
//
//  Created by Jason Chen on 2/22/18.
//  Copyright © 2018 Jason Chen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

struct node{
    double value;
    vector<double> myvalues;
    node *left = NULL;
    node *right = NULL;
};

struct connection{
    int head;
    int tail;
};

class tree{
    void insert(vector<double> value);
private:
    node *root;
};

std::vector<double> getlineandsplit(string str){
    vector<string> result;
    
    vector<double> res;
    
    stringstream lineStream(str);
    string cell;
    //cout << str << endl;
    
    //seeds
    //while(getline(lineStream, cell, '\t')){
    //iris
    while(getline(lineStream, cell, ',')){
        if (!lineStream && cell.empty()){
            //result.push_back("");
            continue;
        }
        else{
        result.push_back(cell);
        }
    }
    getline(lineStream, cell);
    result.push_back(cell);

    //PROBLEM
    //comment out for seeds
    if (!lineStream && cell.empty()){
        result.push_back("");
    }
    
    
    /*
    
    for(int i = 0; i < result.size();i++){
        cout << result[i] << endl;
    }
    */
    
    
    for(int i = 0; i < result.size()-1;i++){
        if (result[i] != ""){
            res.push_back(stof(result[i]));
            //cout << stof(result[i]) << endl;
        }
    }
    
    //cout << res.size() << endl;
    
    return res;
}

double calcdist(vector<double> a, vector<double> b, long size){
    double res = 0, temp;
    for(int i = 0 ;i < size; i++){
        temp = a[i]-b[i];
        res += pow(temp, 2);
    }
    return sqrt(res);
}

void printtree(node* root, int depth){
    cout << root->value << endl;
    if (root->left == NULL){
        /*
        for (int i = 0; i <= depth;i++){
            cout << "-";
        }
        cout << "NULL" << endl;
         */
    }else{
        for (int i = 0; i <= depth;i++){
            cout << "-";
        }
        printtree(root->left, depth+1);
    }
    if (root->right == NULL){
        /*
        for (int i = 0; i <= depth;i++){
            cout << "-";
        }
        cout << "NULL" << endl;
         */
    }else{
        for (int i = 0; i <= depth;i++){
            cout << "-";
        }
        printtree(root->right, depth+1);
    }
}

int counttree(node* root){
    int temp = 1;
    if ((root->left == NULL)&&(root->right == NULL)){
        return temp;
    }else{
        if ((root->left != NULL)){
            if (root->left->value == root->value)
                temp += counttree(root->left)-1;
            else
                temp += counttree(root->left);
        }
            
        if ((root->right != NULL)){
            if (root->right->value == root->value)
                temp+= counttree(root->right)-1;
            else
                temp += counttree(root->right);
        }
        return temp;
    }
}

unsigned long iter_factorial(unsigned long n)
{
    unsigned long ret = 1;
    for(unsigned long i = 1; i <= n; ++i)
        ret *= i;
    return ret;
}

unsigned long fact(unsigned long start, unsigned long stop){
    unsigned long ret = start;
    for(unsigned long i = start-1; i > stop;i--){
        ret *= i;
    }
    return ret;
}

int counttreevalues(node* root){
    int temp = root->value;
    if ((root->left == NULL)&&(root->right == NULL)){
        return temp;
    }else{
        if ((root->left != NULL)){
            if (root->left->value == root->value)
                temp += counttree(root->left)-root->value;
            else
                temp += counttree(root->left);
        }
        
        if ((root->right != NULL)){
            if (root->right->value == root->value)
                temp+= counttree(root->right)-root->value;
            else
                temp += counttree(root->right);
        }
        return temp;
    }
}



//=========================================SINGLE LINKAGE=============================================
/*
int main(int argc, const char * argv[]) {
    
    srand(unsigned(time(0)));
    //srand((unsigned)time(NULL));
    
    vector<double> input;
    vector<vector<double> > variables;
    
    
    vector<string> attributes;
    //ifstream ifs ("miniseeds.txt", ifstream::in);
    ifstream ifs ("seeds_dataset.txt", ifstream::in);
    //ifstream ifs ("iris2.data", ifstream::in);
    
    string line;
    long positive;
    
    getline(ifs, line);
    input = getlineandsplit(line);
    cout << "input: " << input.size() << endl;
    positive = input.size()-1;
    variables.push_back(input);
    //amount of successvariables
    vector<double> successvariables;
    
    //building total sets of cleaned data
    while(getline(ifs, line)){
        //cout << "getline: " << line << endl;
        //needed for iris
        //line = line + ",";
        
        input = getlineandsplit(line);
        //cout << input.size() << endl;
        if (input.size() == positive+1){
            variables.push_back(input);
        }
        
    }
    
    
    //checking how many different success variables
    unsigned long clusters;
    bool found = false;
    for (int i = 0; i < variables.size();i++){
        found = false;
        for(int j = 0; j < successvariables.size();j++){
            if (variables[i][positive] == successvariables[j])
                found = true;
        }
        if (!found){
            successvariables.push_back(variables[i][positive]);
        }
    }
    clusters = successvariables.size();
    cout << "success: " << clusters << endl;
    cout << "starting: " << variables.size() << endl;
    
    //build connections table
    int correctconnections[variables.size()][variables.size()];
    int myconnections[variables.size()][variables.size()];
    vector<vector<int> > correctstack;
    for (int i = 0; i < variables.size();i++){
        for (int j = 0; j < variables.size();j++){
            correctconnections[i][j] = 0;
            myconnections[i][j] = 0;
        }
    }
    
    //create the "correct clustering"
    double correct[successvariables.size()];
    for(int i = 0; i < successvariables.size();i++){
        vector<int> temp;
        correctstack.push_back(temp);
        correct[i] = 0;
    }
    
    for(int i = 0; i < variables.size();i++){
        for(int j = 0; j < successvariables.size();j++){
            if(variables[i][positive] == successvariables[j]){
                
                //create connections on each success variable
                correctstack[j].push_back(i);
                
                
                correct[j]++;
                break;
            }
        }
    }

    
    //create connections on each success variable
    for (int i = 0; i < correctstack.size();i++){
        
        //only connection if more than 2
        if (correctstack[i].size() > 1){
            
            //connect all things in the stack
            for (int j = 0; j < correctstack[i].size();j++){
                for (int k = j+1; k < correctstack[i].size();k++){
                    cout << "j: " << correctstack[i][j] << ",k: "<< correctstack[i][k] << endl;
                    correctconnections[correctstack[i][j]][correctstack[i][k]] = 1;
                    correctconnections[correctstack[i][k]][correctstack[i][j]] = 1;
                }
            }
        }
    }
    
    cout << "corrects: ";
    for(int i = 0; i < successvariables.size();i++){
        cout << correct[i] << ",";
    }
    cout << endl;
    
    
    
    //keep repeating function until currentclusters <= clusters;
    unsigned long currentclusters = variables.size();
    
    //for testing purposes
    //unsigned long currentclusters = clusters + 2;
    
    unsigned long mysize = variables.size();
    
    //build distance matrix
    double distances[mysize][mysize];
    
    //keep a "closest" table
    double mintable[mysize][2];
    
    for(int i = 0; i < mysize; i++){
        mintable[i][1] = 0;
    }
    
    
    //cout << variables.size() << endl;
    
    //populate tables (first time)
    double amount = 0;
    for (int rows = 0; rows < variables.size(); rows++){
        bool first = true;
        for(int columns = 0; columns < variables.size(); columns++){
            if (columns == rows){
                distances[rows][columns] = 0;
            }
            else{
                amount = calcdist(variables[rows], variables[columns], positive);
                if (first){
                    mintable[rows][0] = columns;
                    mintable[rows][1] = amount;
                    first = false;
                }
                else if (mintable[rows][1] > amount){
                    mintable[rows][1] = amount;
                    mintable[rows][0] = columns;
                }
                distances[rows][columns] = amount;
            }
            //cout << amount << ",";
        }
    }
    
    //vector of nodes to create tree
    
    vector<node*> mynodes;
    
    
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        for(int j = 0; j < variables.size(); j++){
            cout << "[" << distances[i][j] << "],";
        }
        cout << endl;
    }
    cout << endl;
    //begin loop
    
    int test;
    test = 0;
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        cout << mintable[i][0] << "," << mintable[i][1] << endl;
        if (mintable[i][0] != -1){
            test++;
        }
    }
    
    vector<connection *> smallconnections;
    

    //----------------------------------BEGIN LOOP----------------------------------------
    //while(currentclusters > clusters){
    while (test > clusters){
    //starts off at 220,
    //while(mynodes.size() > clusters){
        
        connection* tempconnection = new connection;
        
    //create the closest cluster table
        //starting points
        double smallestdist;
        int a, b;
        smallestdist = mintable[0][1];
        a = 0;
        b = mintable[0][0];
        
        
    //double smallestdist = mintable[0][1];
    //a is the row, b is the column
    //int a = 0, b = mintable[0][0];
        
        
    for(int i = 0; i < variables.size(); i++){
     
        //calculate smallset distance
        if ((smallestdist > mintable[i][1])&&(mintable[i][1] > -1)){
            a = i;
            b = mintable[i][0];
            smallestdist = mintable[i][1];
        }
        //cout << i << ".";
        //cout << mintable[i][0] << "," << mintable[i][1] << endl;
    }
    
    
    
    cout << "smallestdist: " << a << "," << b << "," << smallestdist << endl;
    
    //cout << "secondsmallestdist: " << temp << "," << temploc << "," << tempsmallest << endl;
    
    //cout << "secondsmallest: " << seconda << "," << secondb << "," << secondsmallestdist << endl;
    
      
    
    int removed = max(a, b);
    int head = min(a, b);
    //mintable[head][0] = 0;
        
        tempconnection->head = head;
        tempconnection->tail = removed;
        smallconnections.push_back(tempconnection);
    
        
        
    //change table to update all with "removed" node
    for (int i = 0; i < variables.size();i++){
            //if ((mintable[i][0] == removed)||(mintable[i][0]==head)){
                if (distances[head][i] > distances[removed][i]){
                    distances[head][i] = distances[removed][i];
                    
                    
                }
                if (distances[i][head] > distances[i][removed]){
                    distances[i][head] = distances[i][removed];
                }
       // }
    }
        
    
    //check if closest is removed, change to minimum of removed and head
    for (int i = 0; i < variables.size();i++){
        
        //if the closest WAS the "to be removed"
        if (mintable[i][0] == removed){
            
            //head is closer than removed
            if (distances[head][i] < distances[removed][i]){
                mintable[i][0] = head;
                mintable[i][1] = distances[head][i];
            }
            
            //removed was closer than head
            else{
                mintable[i][0] = head;
                mintable[i][1] = distances[removed][i];
                distances[head][i] = distances[removed][i];
                distances[i][head] = distances[i][removed];
            }
        }
        
        //if the closest was head, but removed is closer
        if (mintable[i][0] == head){
            
            //removed was closer than head
            if (mintable[i][1] > distances[removed][i]){
                mintable[i][1] = distances[removed][i];
                distances[head][i] = distances[removed][i];
                distances[i][head] = distances[i][removed];
            }
        }
    }
    
    //remove b from the mintable
    mintable[removed][0] = -1;
    mintable[removed][1] = -1;
    
    
    //recompute head's closest in mintable
    double min;
    int minloc;
         
        //assigning first check variable
        for (int i = 0; i < variables.size();i++){
            if ((i!= head) && (mintable[i][1] > 0)){
                min = mintable[i][1];
                minloc = i;
                break;
            }
        }
        
    //check the rest
    for(int i = 1; i < variables.size(); i++){
        if((min > distances[i][head])&&(i != head)&&(distances[head][i] > -1)&&(i != removed)){
            min = distances[i][head];
            minloc = i;
        }
    }
    mintable[head][0] = minloc;
    mintable[head][1] = min;
    
    //remove b from the table
    // i = rows j = columns
    for(int i = 0; i < variables.size();i++){
        for(int j = 0; j < variables.size();j++){
            if ((i == removed) || (j == removed)){
                distances[i][j] = -1;
            }
        }
    }
    
    
    cout << "-----------------------creating node with " << head << " and " << removed << "------------------------" << endl;
        
        int leftnodeloc = -1, rightnodeloc = -1;
        bool leftnode = false, rightnode = false;
        node *l;
        node *r;
        node *root = new node;
        root->value = head;
        
        //check if already in the node list
        for(int i = 0; i < mynodes.size();i++){
            if (mynodes[i]->value == head){
                leftnode = true;
                leftnodeloc = i;
            }
            else if (mynodes[i]->value == removed){
                rightnode = true;
                rightnodeloc = i;
            }
        }
        
        
        //if the left and the right are already in the list
        //remove right and left from list
        if((leftnode)&&(rightnode)){
            cout << "left and right" << endl;
            root->right = mynodes[rightnodeloc];
            root->left = mynodes[leftnodeloc];
            mynodes.erase(mynodes.begin()+rightnodeloc);
            if (rightnodeloc < leftnodeloc){
                mynodes.erase(mynodes.begin()+leftnodeloc-1);
            }
            else{
                mynodes.erase(mynodes.begin()+leftnodeloc);
            }
            currentclusters--;
        }
        //left no right
        else if (rightnode){
            cout << "right" << endl;
            root->right = mynodes[rightnodeloc];
            l = new node;
            l->value = head;
            root->left = l;
            mynodes.erase(mynodes.begin()+rightnodeloc);
            currentclusters--;
        }
        //right no left
        else if (leftnode){
            cout << "left" << endl;
            root->left = mynodes[leftnodeloc];
            r = new node;
            r->value = removed;
            root->right = r;
            mynodes.erase(mynodes.begin()+leftnodeloc);
            currentclusters--;
        }else{
            cout << "NEW" << endl;
            //create 3 nodes
            //left (smaller)
            
            l = new node;
            l->value = head;
            
            //right (larger)
            
            r = new node;
            r->value = removed;
            
            //root node (uses least variable)
            
            
            root->left = l;
            root->right = r;
            currentclusters--;
        }
        
        

        
        //add root node to vector list
        
        mynodes.push_back(root);
    
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        for(int j = 0; j < variables.size(); j++){
            cout << "[" << distances[i][j] << "],";
        }
        cout << endl;
    }
    cout << endl;
     
    
        cout << "-----------------------------------------------------------------------------------" << endl;
        
        test = 0;
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        cout << mintable[i][0] << "," << mintable[i][1] << endl;
        if (mintable[i][1] != -1){
            test++;
        }
    }

     
        cout << mynodes.size() << endl;
        for (int i = 0; i < mynodes.size();i++){
            printtree(mynodes[i], 0);
        }
        cout << "clusters: " << test << endl;
        //cout << "currentclusters: " << currentclusters << endl;
        //currentclusters--;
    }
    
    //-------------------END OF LOOP----------------------------
    
    
    vector<int> notin;
    bool isin;
    
    for(int j = 0; j < variables.size();j++){
        if (mintable[j][0] != -1){
            isin = false;
            for(int i = 0; i < mynodes.size();i++){
                if (mynodes[i]->value == j){
                    isin = true;
                }
            }
            if (!isin){
                notin.push_back(j);
            }
        }
    }
    
    for (int i = 0; i < notin.size();i++){
        cout << "NEW" << endl;
        //create a nodes
        
        //root node (uses least variable)
        node* root = new node;
        root->value = notin[i];
        root->left = NULL;
        root->right = NULL;
        mynodes.push_back(root);
        cout << "success" << endl;
    }
    
    //printtree(mynodes[0]);
    
    cout << mynodes.size() << endl;
    for (int i = 0; i < mynodes.size();i++){
        printtree(mynodes[i], 0);
    }
    
    //create similar format to above for connections
    vector<vector<int> > mystack;
    for (int i = 0; i < smallconnections.size();i++){
        if (mystack.empty()){
            vector<int> temp;
            temp.push_back(smallconnections[i]->head);
            temp.push_back(smallconnections[i]->tail);
            mystack.push_back(temp);
        }else{
            int headloc = -1, tailloc = -1;
            for(int k = 0; k < mystack.size();k++){
                for (int j = 0; j < mystack[k].size();j++){
                    if(mystack[k][j] == smallconnections[i]->head){
                        headloc = k;
                    }else if (mystack[k][j] == smallconnections[i]->tail){
                        tailloc = k;
                    }
                }
            }
            //head already inside, tail not
            if ((headloc != -1)&&(tailloc == -1)){
                mystack[headloc].push_back(smallconnections[i]->tail);
            }
            //tail already inside, head not
            else if ((tailloc != -1)&&(headloc == -1)){
                mystack[tailloc].push_back(smallconnections[i]->head);
            }
            //neither inside
            else if ((tailloc == -1)&&(headloc == -1)){
                vector<int> temp;
                temp.push_back(smallconnections[i]->head);
                temp.push_back(smallconnections[i]->tail);
                mystack.push_back(temp);
            }
            //both inside, but different areas
            else if (tailloc != headloc){
                int sizeholder = mystack[tailloc].size();
                for (int i = 0; i < sizeholder;i++){
                    mystack[headloc].push_back(mystack[tailloc].front());
                    mystack[tailloc].erase(mystack[tailloc].begin());
                }
                mystack.erase(mystack.begin()+tailloc);
            }
        }
        
        //cout << smallconnections[i]->head << "," << smallconnections[i]->tail << endl;
    }
    
    for (int i = 0; i < mystack.size();i++){
    for (int j = 0; j < mystack[i].size();j++){
        for (int k = j+1; k < mystack[i].size();k++){
            cout << "j: " << mystack[i][j] << ",k: "<< mystack[i][k] << endl;
            myconnections[mystack[i][j]][mystack[i][k]] = 1;
            myconnections[mystack[i][k]][mystack[i][j]] = 1;
        }
    }
    }
    
    for (int i = 0; i < variables.size();i++){
        cout << "[";
        for (int j = 0; j < variables.size();j++){
            cout << myconnections[i][j] << ",";
        }
        cout << "]" << endl;
    }
    
    //hamming distance
    unsigned long total = (variables.size()*(variables.size()-1))/2;
    unsigned long disagree = 0;
    for (int i = 0; i < variables.size();i++){
        for (int j = i; j < variables.size();j++){
            if (correctconnections[i][j] != myconnections[i][j]){
                disagree++;
            }
        }
    }
    
    cout << "total:" << total << endl;
    cout << "disagree: " << disagree << endl;
    cout << "hamming: " << (double)disagree/(double)total << endl;
    

    
    std::cout << "Hello, World!\n";
    return 0;
}
*/

//=======================================COMPLETE LINKAGE================================================
/*
int main(int argc, const char * argv[]) {
    
    srand(unsigned(time(0)));
    //srand((unsigned)time(NULL));
    
    vector<double> input;
    vector<vector<double> > variables;
    
    
    vector<string> attributes;
    //ifstream ifs ("miniseeds.txt", ifstream::in);
    //ifstream ifs ("seeds_dataset.txt", ifstream::in);
    ifstream ifs ("iris2.data", ifstream::in);
    
    string line;
    long positive;
    
    getline(ifs, line);
    input = getlineandsplit(line);
    cout << "input: " << input.size() << endl;
    positive = input.size()-1;
    variables.push_back(input);
    //amount of successvariables
    vector<double> successvariables;
    
    //building total sets of cleaned data
    while(getline(ifs, line)){
        //cout << "getline: " << line << endl;
        //needed for iris
        //line = line + ",";
        
        input = getlineandsplit(line);
        //cout << input.size() << endl;
        if (input.size() == positive+1){
            variables.push_back(input);
        }
        
    }
    
    
    //checking how many different success variables
    unsigned long clusters;
    bool found = false;
    for (int i = 0; i < variables.size();i++){
        found = false;
        for(int j = 0; j < successvariables.size();j++){
            if (variables[i][positive] == successvariables[j])
                found = true;
        }
        if (!found){
            successvariables.push_back(variables[i][positive]);
        }
    }
    clusters = successvariables.size();
    cout << "success: " << clusters << endl;
    cout << "starting: " << variables.size() << endl;
 
 
    
    //build connections table
    int correctconnections[variables.size()][variables.size()];
    int myconnections[variables.size()][variables.size()];
    vector<vector<int> > correctstack;
    for (int i = 0; i < variables.size();i++){
        for (int j = 0; j < variables.size();j++){
            correctconnections[i][j] = 0;
            myconnections[i][j] = 0;
        }
    }
 
    //create the "correct clustering"
    double correct[successvariables.size()];
    for(int i = 0; i < successvariables.size();i++){
        vector<int> temp;
        correctstack.push_back(temp);
        correct[i] = 0;
    }
 
    for(int i = 0; i < variables.size();i++){
        for(int j = 0; j < successvariables.size();j++){
            if(variables[i][positive] == successvariables[j]){
 
                //create connections on each success variable
                correctstack[j].push_back(i);
 
 
                correct[j]++;
                break;
            }
        }
    }
 
 
    //create connections on each success variable
    for (int i = 0; i < correctstack.size();i++){
 
        //only connection if more than 2
        if (correctstack[i].size() > 1){
 
            //connect all things in the stack
            for (int j = 0; j < correctstack[i].size();j++){
                for (int k = j+1; k < correctstack[i].size();k++){
                    cout << "j: " << correctstack[i][j] << ",k: "<< correctstack[i][k] << endl;
                    correctconnections[correctstack[i][j]][correctstack[i][k]] = 1;
                    correctconnections[correctstack[i][k]][correctstack[i][j]] = 1;
                }
            }
        }
    }

 
    cout << "corrects: ";
    for(int i = 0; i < successvariables.size();i++){
        cout << correct[i] << ",";
    }
    cout << endl;
 
 
 
 
    //keep repeating function until currentclusters <= clusters;
    unsigned long currentclusters = variables.size();
 
    //for testing purposes
    //unsigned long currentclusters = clusters + 2;
 
    unsigned long mysize = variables.size();
    
    //build distance matrix
    double distances[mysize][mysize];
    
    //keep a "closest" table
    double mintable[mysize][2];
    
    for(int i = 0; i < mysize; i++){
        mintable[i][1] = 0;
    }
    
    
    //cout << variables.size() << endl;
    
    //populate tables (first time)
    double amount = 0;
    for (int rows = 0; rows < variables.size(); rows++){
        bool first = true;
        for(int columns = 0; columns < variables.size(); columns++){
            if (columns == rows){
                distances[rows][columns] = 0;
            }
            else{
                amount = calcdist(variables[rows], variables[columns], positive);
                if (first){
                    mintable[rows][0] = columns;
                    mintable[rows][1] = amount;
                    first = false;
                }
                else if (mintable[rows][1] < amount){
                    mintable[rows][1] = amount;
                    mintable[rows][0] = columns;
                }
                distances[rows][columns] = amount;
            }
            //cout << amount << ",";
        }
    }
    
    //vector of nodes to create tree
    
    vector<node*> mynodes;
    
    
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        for(int j = 0; j < variables.size(); j++){
            cout << "[" << distances[i][j] << "],";
        }
        cout << endl;
    }
    cout << endl;
    //begin loop
    
    int test;
    test = 0;
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        cout << mintable[i][0] << "," << mintable[i][1] << endl;
        if (mintable[i][0] != -1){
            test++;
        }
    }
    
    vector<connection *> smallconnections;
    
    //----------------------------------BEGIN LOOP----------------------------------------
    //while(currentclusters > clusters){
    while (test > clusters){
        //starts off at 220,
        //while(mynodes.size() > clusters){
        
        connection* tempconnection = new connection;
        
        //create the closest cluster table
        //starting points
        double smallestdist;
        int a, b;
        smallestdist = mintable[0][1];
        a = 0;
        b = mintable[0][0];
        
        
        //double smallestdist = mintable[0][1];
        //a is the row, b is the column
        //int a = 0, b = mintable[0][0];
        
        
        for(int i = 0; i < variables.size(); i++){
            
            //calculate smallset distance
            if ((smallestdist < mintable[i][1])&&(mintable[i][1] > -1)){
                a = i;
                b = mintable[i][0];
                smallestdist = mintable[i][1];
            }
            //cout << i << ".";
            //cout << mintable[i][0] << "," << mintable[i][1] << endl;
        }
        
        
        
        cout << "smallestdist: " << a << "," << b << "," << smallestdist << endl;
        
        int temp = min(a, b);
        int temp2 = max(a, b);
        int temploc = 0;
        double tempsmallest = distances[temp][0];
        for (int i = 0; i < variables.size(); i++){
            if ((i != temp2)&&(tempsmallest < distances[temp][i])&&(i != temp)&&(distances[temp][i] > -1)){
                tempsmallest = distances[temp][i];
                temploc = i;
            }
        }
        
        //cout << "secondsmallestdist: " << temp << "," << temploc << "," << tempsmallest << endl;
        
        //cout << "secondsmallest: " << seconda << "," << secondb << "," << secondsmallestdist << endl;
        
        
        
        int removed = max(a, b);
        int head = min(a, b);
        
        //mintable[head][0] = 0;
        
        tempconnection->head = head;
        tempconnection->tail = removed;
        smallconnections.push_back(tempconnection);
        
        
        
        //change table to update all with "removed" node
        for (int i = 0; i < variables.size();i++){
            //if ((mintable[i][0] == removed)||(mintable[i][0]==head)){
            if (distances[head][i] < distances[removed][i]){
                distances[head][i] = distances[removed][i];
                
                
            }
            if (distances[i][head] < distances[i][removed]){
                distances[i][head] = distances[i][removed];
            }
            // }
        }
        
        
        //check if closest is removed, change to minimum of removed and head
        for (int i = 0; i < variables.size();i++){
            
            //if the closest WAS the "to be removed"
            if (mintable[i][0] == removed){
                
                //head is closer than removed
                if (distances[head][i] > distances[removed][i]){
                    mintable[i][0] = head;
                    mintable[i][1] = distances[head][i];
                }
                
                //removed was closer than head
                else{
                    mintable[i][0] = head;
                    mintable[i][1] = distances[removed][i];
                    distances[head][i] = distances[removed][i];
                    distances[i][head] = distances[i][removed];
                }
            }
            
            //if the closest was head, but removed is closer
            if (mintable[i][0] == head){
                
                //removed was closer than head
                if (mintable[i][1] < distances[removed][i]){
                    mintable[i][1] = distances[removed][i];
                    distances[head][i] = distances[removed][i];
                    distances[i][head] = distances[i][removed];
                }
            }
        }
        
        //remove b from the mintable
        mintable[removed][0] = -1;
        mintable[removed][1] = -1;
        
        
        //recompute head's closest in mintable
        double min;
        int minloc;
        
        //assigning first check variable
        for (int i = 0; i < variables.size();i++){
            if ((i!= head) && (mintable[i][1] > 0)){
                min = mintable[i][1];
                minloc = i;
                break;
            }
        }
        
        //check the rest
        for(int i = 1; i < variables.size(); i++){
            if((min < distances[i][head])&&(i != head)&&(distances[head][i] > -1)&&(i != removed)){
                min = distances[i][head];
                minloc = i;
            }
        }
        mintable[head][0] = minloc;
        mintable[head][1] = min;
        
        //remove b from the table
        // i = rows j = columns
        for(int i = 0; i < variables.size();i++){
            for(int j = 0; j < variables.size();j++){
                if ((i == removed) || (j == removed)){
                    distances[i][j] = -1;
                }
            }
        }
        
        
        cout << "-----------------------creating node with " << head << " and " << removed << "------------------------" << endl;
        
        int leftnodeloc = -1, rightnodeloc = -1;
        bool leftnode = false, rightnode = false;
        node *l;
        node *r;
        node *root = new node;
        root->value = head;
        
        //check if already in the node list
        for(int i = 0; i < mynodes.size();i++){
            if (mynodes[i]->value == head){
                leftnode = true;
                leftnodeloc = i;
            }
            else if (mynodes[i]->value == removed){
                rightnode = true;
                rightnodeloc = i;
            }
        }
        
        
        //if the left and the right are already in the list
        //remove right and left from list
        if((leftnode)&&(rightnode)){
            cout << "left and right" << endl;
            root->right = mynodes[rightnodeloc];
            root->left = mynodes[leftnodeloc];
            mynodes.erase(mynodes.begin()+rightnodeloc);
            if (rightnodeloc < leftnodeloc){
                mynodes.erase(mynodes.begin()+leftnodeloc-1);
            }
            else{
                mynodes.erase(mynodes.begin()+leftnodeloc);
            }
            currentclusters--;
        }
        //left no right
        else if (rightnode){
            cout << "right" << endl;
            root->right = mynodes[rightnodeloc];
            l = new node;
            l->value = head;
            root->left = l;
            mynodes.erase(mynodes.begin()+rightnodeloc);
            currentclusters--;
        }
        //right no left
        else if (leftnode){
            cout << "left" << endl;
            root->left = mynodes[leftnodeloc];
            r = new node;
            r->value = removed;
            root->right = r;
            mynodes.erase(mynodes.begin()+leftnodeloc);
            currentclusters--;
        }else{
            cout << "NEW" << endl;
            //create 3 nodes
            //left (smaller)
            
            l = new node;
            l->value = head;
            
            //right (larger)
            
            r = new node;
            r->value = removed;
            
            //root node (uses least variable)
            
            
            root->left = l;
            root->right = r;
            currentclusters--;
        }
        
        
        
        
        //add root node to vector list
        
        mynodes.push_back(root);
        
        for(int i = 0; i < variables.size();i++){
            cout << i << ".";
            for(int j = 0; j < variables.size(); j++){
                cout << "[" << distances[i][j] << "],";
            }
            cout << endl;
        }
        cout << endl;
        
        
        cout << "-----------------------------------------------------------------------------------" << endl;
        
        test = 0;
        for(int i = 0; i < variables.size();i++){
            cout << i << ".";
            cout << mintable[i][0] << "," << mintable[i][1] << endl;
            if (mintable[i][1] != -1){
                test++;
            }
        }
        
        
        cout << mynodes.size() << endl;
        for (int i = 0; i < mynodes.size();i++){
            printtree(mynodes[i], 0);
        }
        cout << "clusters: " << test << endl;
        //cout << "currentclusters: " << currentclusters << endl;
        //currentclusters--;
    }
    
    //-------------------END OF LOOP----------------------------
    
    
    vector<int> notin;
    bool isin;
    
    for(int j = 0; j < variables.size();j++){
        if (mintable[j][0] != -1){
            isin = false;
            for(int i = 0; i < mynodes.size();i++){
                if (mynodes[i]->value == j){
                    isin = true;
                }
            }
            if (!isin){
                notin.push_back(j);
            }
        }
    }
    
    for (int i = 0; i < notin.size();i++){
        cout << "NEW" << endl;
        //create a nodes
        
        //root node (uses least variable)
        node* root = new node;
        root->value = notin[i];
        root->left = NULL;
        root->right = NULL;
        mynodes.push_back(root);
        cout << "success" << endl;
    }
    
    //printtree(mynodes[0]);
    
    cout << mynodes.size() << endl;
    for (int i = 0; i < mynodes.size();i++){
        printtree(mynodes[i], 0);
    }
    
    //create similar format to above for connections
    vector<vector<int> > mystack;
    for (int i = 0; i < smallconnections.size();i++){
        if (mystack.empty()){
            vector<int> temp;
            temp.push_back(smallconnections[i]->head);
            temp.push_back(smallconnections[i]->tail);
            mystack.push_back(temp);
        }else{
            int headloc = -1, tailloc = -1;
            for(int k = 0; k < mystack.size();k++){
                for (int j = 0; j < mystack[k].size();j++){
                    if(mystack[k][j] == smallconnections[i]->head){
                        headloc = k;
                    }else if (mystack[k][j] == smallconnections[i]->tail){
                        tailloc = k;
                    }
                }
            }
            //head already inside, tail not
            if ((headloc != -1)&&(tailloc == -1)){
                mystack[headloc].push_back(smallconnections[i]->tail);
            }
            //tail already inside, head not
            else if ((tailloc != -1)&&(headloc == -1)){
                mystack[tailloc].push_back(smallconnections[i]->head);
            }
            //neither inside
            else if ((tailloc == -1)&&(headloc == -1)){
                vector<int> temp;
                temp.push_back(smallconnections[i]->head);
                temp.push_back(smallconnections[i]->tail);
                mystack.push_back(temp);
            }
            //both inside, but different areas
            else if (tailloc != headloc){
                int sizeholder = mystack[tailloc].size();
                for (int i = 0; i < sizeholder;i++){
                    mystack[headloc].push_back(mystack[tailloc].front());
                    mystack[tailloc].erase(mystack[tailloc].begin());
                }
                mystack.erase(mystack.begin()+tailloc);
            }
        }
        
        //cout << smallconnections[i]->head << "," << smallconnections[i]->tail << endl;
    }
    
    for (int i = 0; i < mystack.size();i++){
        for (int j = 0; j < mystack[i].size();j++){
            for (int k = j+1; k < mystack[i].size();k++){
                cout << "j: " << mystack[i][j] << ",k: "<< mystack[i][k] << endl;
                myconnections[mystack[i][j]][mystack[i][k]] = 1;
                myconnections[mystack[i][k]][mystack[i][j]] = 1;
            }
        }
    }
    
    for (int i = 0; i < variables.size();i++){
        cout << "[";
        for (int j = 0; j < variables.size();j++){
            cout << myconnections[i][j] << ",";
        }
        cout << "]" << endl;
    }
    
    //hamming distance
    unsigned long total = (variables.size()*(variables.size()-1))/2;
    unsigned long disagree = 0;
    for (int i = 0; i < variables.size();i++){
        for (int j = i; j < variables.size();j++){
            if (correctconnections[i][j] != myconnections[i][j]){
                disagree++;
            }
        }
    }

    cout << "total:" << total << endl;
    cout << "disagree: " << disagree << endl;
    cout << "hamming: " << (double)disagree/(double)total << endl;
    
    std::cout << "Hello, World!\n";
    return 0;
}
 */


//===============================AVERAGE LINKAGE===================================
/*
int main(int argc, const char * argv[]) {
    
    srand(unsigned(time(0)));
    //srand((unsigned)time(NULL));
    
    vector<double> input;
    vector<vector<double> > variables;
    
    
    vector<string> attributes;
    //ifstream ifs ("miniseeds.txt", ifstream::in);
    ifstream ifs ("seeds_dataset.txt", ifstream::in);
    //ifstream ifs ("iris2.data", ifstream::in);
    
    string line;
    long positive;
    
    getline(ifs, line);
    input = getlineandsplit(line);
    //cout << "input: " << input.size() << endl;
    positive = input.size()-1;
    variables.push_back(input);
    //amount of successvariables
    vector<double> successvariables;
    
    //building total sets of cleaned data
    while(getline(ifs, line)){
        //cout << "getline: " << line << endl;
        //needed for iris
        //line = line + ",";
        
        input = getlineandsplit(line);
        //cout << input.size() << endl;
        if (input.size() == positive+1){
            variables.push_back(input);
        }
        
    }
    
    
    //checking how many different success variables
    unsigned long clusters;
    bool found = false;
    for (int i = 0; i < variables.size();i++){
        found = false;
        for(int j = 0; j < successvariables.size();j++){
            if (variables[i][positive] == successvariables[j])
                found = true;
        }
        if (!found){
            successvariables.push_back(variables[i][positive]);
        }
    }
    clusters = successvariables.size();
    cout << "success: " << clusters << endl;
    cout << "starting: " << variables.size() << endl;
    
    //build connections table
    int correctconnections[variables.size()][variables.size()];
    int myconnections[variables.size()][variables.size()];
    vector<vector<int> > correctstack;
    for (int i = 0; i < variables.size();i++){
        for (int j = 0; j < variables.size();j++){
            correctconnections[i][j] = 0;
            myconnections[i][j] = 0;
        }
    }
 
    //create the "correct clustering"
    double correct[successvariables.size()];
    for(int i = 0; i < successvariables.size();i++){
        vector<int> temp;
        correctstack.push_back(temp);
        correct[i] = 0;
    }
 
    for(int i = 0; i < variables.size();i++){
        for(int j = 0; j < successvariables.size();j++){
            if(variables[i][positive] == successvariables[j]){
 
                //create connections on each success variable
                correctstack[j].push_back(i);
 
 
                correct[j]++;
                break;
            }
        }
    }
 
 
    //create connections on each success variable
    for (int i = 0; i < correctstack.size();i++){
 
        //only connection if more than 2
        if (correctstack[i].size() > 1){
 
            //connect all things in the stack
            for (int j = 0; j < correctstack[i].size();j++){
                for (int k = j+1; k < correctstack[i].size();k++){
                    cout << "j: " << correctstack[i][j] << ",k: "<< correctstack[i][k] << endl;
                    correctconnections[correctstack[i][j]][correctstack[i][k]] = 1;
                    correctconnections[correctstack[i][k]][correctstack[i][j]] = 1;
                }
            }
        }
    }

 
    cout << "corrects: ";
    for(int i = 0; i < successvariables.size();i++){
        cout << correct[i] << ",";
    }
    cout << endl;
 
 
 
 
    //keep repeating function until currentclusters <= clusters;
    unsigned long currentclusters = variables.size();
 
    //for testing purposes
    //unsigned long currentclusters = clusters + 2;
 
    unsigned long mysize = variables.size();
    
    //build distance matrix
    double distances[mysize][mysize];
    
    //keep a "closest" table
    double mintable[mysize][2];
    
    for(int i = 0; i < mysize; i++){
        mintable[i][1] = 0;
    }
    
    
    //cout << variables.size() << endl;
    
    //populate tables (first time)
    double amount = 0;
    for (int rows = 0; rows < variables.size(); rows++){
        bool first = true;
        for(int columns = 0; columns < variables.size(); columns++){
            if (columns == rows){
                distances[rows][columns] = 0;
            }
            else{
                amount = calcdist(variables[rows], variables[columns], positive);
                if (first){
                    mintable[rows][0] = columns;
                    mintable[rows][1] = amount;
                    first = false;
                }
                else if (mintable[rows][1] > amount){
                    mintable[rows][1] = amount;
                    mintable[rows][0] = columns;
                }
                distances[rows][columns] = amount;
            }
            //cout << amount << ",";
        }
    }
    
    //second table to represent amount of distances (for cluster size)
    int distancedepth[mysize];
    for(int i = 0; i < variables.size();i++){
                distancedepth[i] = 1;
    }
    
    
    //vector of nodes to create tree
    
    vector<node*> mynodes;
    
    
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        for(int j = 0; j < variables.size(); j++){
            cout << "[" << distances[i][j] << "],";
        }
        cout << endl;
    }
    cout << endl;
    //begin loop
    
    int test;
    test = 0;
    for(int i = 0; i < variables.size();i++){
        cout << i << ".";
        cout << mintable[i][0] << "," << mintable[i][1] << endl;
        if (mintable[i][0] != -1){
            test++;
        }
    }
    
    vector<connection *> smallconnections;
    
    //----------------------------------BEGIN LOOP----------------------------------------
    //while(currentclusters > clusters){
    while (test > clusters){
        //starts off at 220,
        //while(mynodes.size() > clusters){
        
        connection* tempconnection = new connection;
        
        //create the closest cluster table
        //starting points
        double smallestdist;
        int a, b;
        smallestdist = mintable[0][1];
        a = 0;
        b = mintable[0][0];
        
        
        //double smallestdist = mintable[0][1];
        //a is the row, b is the column
        //int a = 0, b = mintable[0][0];
        
        
        for(int i = 0; i < variables.size(); i++){
            
            //calculate smallest distance
            if ((smallestdist > mintable[i][1])&&(mintable[i][1] > -1)){
                a = i;
                b = mintable[i][0];
                smallestdist = mintable[i][1];
            }
            //cout << i << ".";
            //cout << mintable[i][0] << "," << mintable[i][1] << endl;
        }
        
        
        
        cout << "smallestdist: " << a << "," << b << "," << smallestdist << endl;
        
        //cout << "secondsmallestdist: " << temp << "," << temploc << "," << tempsmallest << endl;
        
        //cout << "secondsmallest: " << seconda << "," << secondb << "," << secondsmallestdist << endl;
        
        
        
        int removed = max(a, b);
        int head = min(a, b);
        //mintable[head][0] = 0;
        
        tempconnection->head = head;
        tempconnection->tail = removed;
        smallconnections.push_back(tempconnection);
        
        //recompute new distance table
        
        
        //double newval = (((mintable[removed][1]*mintable[removed][2])+(mintable[head][1]*mintable[head][2]))/(mintable[removed][2]+mintable[head][2]));
        
        //change table to update all with "removed" node
        for (int i = 0; i < variables.size();i++){
            //if ((mintable[i][0] == removed)||(mintable[i][0]==head)){
            //head*headamount
            double newval = (((distances[head][i]*distancedepth[head])+(distances[removed][i]*distancedepth[removed]))/(distancedepth[head]+distancedepth[removed]));
            //cout << "i: " << i << ", newval: " << newval << endl;
            
            
            distances[head][i] = newval;
            distances[i][head] = newval;
 
        }
        distancedepth[head]+=distancedepth[removed];
        distancedepth[removed] = 0;
        
        
        
        //check if closest is removed, change to head and new avg
        for (int i = 0; i < variables.size();i++){
            
            //if the closest WAS the "to be removed"
            if (mintable[i][0] == removed){
                mintable[i][0] = head;
                mintable[i][1] = distances[head][i];
            }
            
         
            //if the closest was head, but removed is closer
            if (mintable[i][0] == head){
                mintable[i][1] = distances[head][i];
            }
        }
        
        //remove b from the mintable
        mintable[removed][0] = -1;
        mintable[removed][1] = -1;
        
        
        //recompute head's closest in mintable
        double min;
        int minloc;
        
        //assigning first check variable
        for (int i = 0; i < variables.size();i++){
            if ((i!= head) && (mintable[i][1] > 0)){
                min = mintable[i][1];
                minloc = i;
                break;
            }
        }
        
        //check the rest
        for(int i = 1; i < variables.size(); i++){
            if((min > distances[i][head])&&(i != head)&&(distances[head][i] > -1)&&(i != removed)){
                min = distances[i][head];
                minloc = i;
            }
        }
        mintable[head][0] = minloc;
        mintable[head][1] = min;
        
        //remove b from the table
        // i = rows j = columns
        for(int i = 0; i < variables.size();i++){
            for(int j = 0; j < variables.size();j++){
                if ((i == removed) || (j == removed)){
                    distances[i][j] = -1;
                }
            }
        }
        
        
        cout << "-----------------------creating node with " << head << " and " << removed << "------------------------" << endl;
        
        int leftnodeloc = -1, rightnodeloc = -1;
        bool leftnode = false, rightnode = false;
        node *l;
        node *r;
        node *root = new node;
        root->value = head;
        
        //check if already in the node list
        for(int i = 0; i < mynodes.size();i++){
            if (mynodes[i]->value == head){
                leftnode = true;
                leftnodeloc = i;
            }
            else if (mynodes[i]->value == removed){
                rightnode = true;
                rightnodeloc = i;
            }
        }
        
        
        //if the left and the right are already in the list
        //remove right and left from list
        if((leftnode)&&(rightnode)){
            cout << "left and right" << endl;
            root->right = mynodes[rightnodeloc];
            root->left = mynodes[leftnodeloc];
            mynodes.erase(mynodes.begin()+rightnodeloc);
            if (rightnodeloc < leftnodeloc){
                mynodes.erase(mynodes.begin()+leftnodeloc-1);
            }
            else{
                mynodes.erase(mynodes.begin()+leftnodeloc);
            }
            currentclusters--;
        }
        //left no right
        else if (rightnode){
            cout << "right" << endl;
            root->right = mynodes[rightnodeloc];
            l = new node;
            l->value = head;
            root->left = l;
            mynodes.erase(mynodes.begin()+rightnodeloc);
            currentclusters--;
        }
        //right no left
        else if (leftnode){
            cout << "left" << endl;
            root->left = mynodes[leftnodeloc];
            r = new node;
            r->value = removed;
            root->right = r;
            mynodes.erase(mynodes.begin()+leftnodeloc);
            currentclusters--;
        }else{
            cout << "NEW" << endl;
            //create 3 nodes
            //left (smaller)
            
            l = new node;
            l->value = head;
            
            //right (larger)
            
            r = new node;
            r->value = removed;
            
            //root node (uses least variable)
            
            
            root->left = l;
            root->right = r;
            currentclusters--;
        }
        
        
        
        
        //add root node to vector list
        
        mynodes.push_back(root);
        
        for(int i = 0; i < variables.size();i++){
            cout << i << ".";
            for(int j = 0; j < variables.size(); j++){
                cout << "[" << distances[i][j] << "],";
            }
            cout << endl;
        }
        cout << endl;
        for(int i = 0; i < variables.size();i++){
            cout << i << ".";
            cout << distancedepth[i];
            cout << endl;
        }
        cout << endl;
        
        
        cout << "-----------------------------------------------------------------------------------" << endl;
        
        test = 0;
        for(int i = 0; i < variables.size();i++){
            cout << i << ".";
            cout << mintable[i][0] << "," << mintable[i][1] << endl;
            if (mintable[i][1] != -1){
                test++;
            }
        }
        
        
        cout << mynodes.size() << endl;
        for (int i = 0; i < mynodes.size();i++){
            printtree(mynodes[i], 0);
        }
        cout << "clusters: " << test << endl;
        //cout << "currentclusters: " << currentclusters << endl;
        //currentclusters--;
    }
    
    //-------------------END OF LOOP----------------------------
    
    
    vector<int> notin;
    bool isin;
    
    for(int j = 0; j < variables.size();j++){
        if (mintable[j][0] != -1){
            isin = false;
            for(int i = 0; i < mynodes.size();i++){
                if (mynodes[i]->value == j){
                    isin = true;
                }
            }
            if (!isin){
                notin.push_back(j);
            }
        }
    }
    
    for (int i = 0; i < notin.size();i++){
        cout << "NEW" << endl;
        //create a nodes
        
        //root node (uses least variable)
        node* root = new node;
        root->value = notin[i];
        root->left = NULL;
        root->right = NULL;
        mynodes.push_back(root);
        cout << "success" << endl;
    }
    
    //printtree(mynodes[0]);
    
    cout << mynodes.size() << endl;
    for (int i = 0; i < mynodes.size();i++){
        printtree(mynodes[i], 0);
    }
    
    //create similar format to above for connections
    vector<vector<int> > mystack;
    for (int i = 0; i < smallconnections.size();i++){
        if (mystack.empty()){
            vector<int> temp;
            temp.push_back(smallconnections[i]->head);
            temp.push_back(smallconnections[i]->tail);
            mystack.push_back(temp);
        }else{
            int headloc = -1, tailloc = -1;
            for(int k = 0; k < mystack.size();k++){
                for (int j = 0; j < mystack[k].size();j++){
                    if(mystack[k][j] == smallconnections[i]->head){
                        headloc = k;
                    }else if (mystack[k][j] == smallconnections[i]->tail){
                        tailloc = k;
                    }
                }
            }
            //head already inside, tail not
            if ((headloc != -1)&&(tailloc == -1)){
                mystack[headloc].push_back(smallconnections[i]->tail);
            }
            //tail already inside, head not
            else if ((tailloc != -1)&&(headloc == -1)){
                mystack[tailloc].push_back(smallconnections[i]->head);
            }
            //neither inside
            else if ((tailloc == -1)&&(headloc == -1)){
                vector<int> temp;
                temp.push_back(smallconnections[i]->head);
                temp.push_back(smallconnections[i]->tail);
                mystack.push_back(temp);
            }
            //both inside, but different areas
            else if (tailloc != headloc){
                int sizeholder = mystack[tailloc].size();
                for (int i = 0; i < sizeholder;i++){
                    mystack[headloc].push_back(mystack[tailloc].front());
                    mystack[tailloc].erase(mystack[tailloc].begin());
                }
                mystack.erase(mystack.begin()+tailloc);
            }
        }
        
        //cout << smallconnections[i]->head << "," << smallconnections[i]->tail << endl;
    }
    
    for (int i = 0; i < mystack.size();i++){
        for (int j = 0; j < mystack[i].size();j++){
            for (int k = j+1; k < mystack[i].size();k++){
                cout << "j: " << mystack[i][j] << ",k: "<< mystack[i][k] << endl;
                myconnections[mystack[i][j]][mystack[i][k]] = 1;
                myconnections[mystack[i][k]][mystack[i][j]] = 1;
            }
        }
    }
    
    for (int i = 0; i < variables.size();i++){
        cout << "[";
        for (int j = 0; j < variables.size();j++){
            cout << myconnections[i][j] << ",";
        }
        cout << "]" << endl;
    }
    
    //hamming distance
    unsigned long total = (variables.size()*(variables.size()-1))/2;
    unsigned long disagree = 0;
    for (int i = 0; i < variables.size();i++){
        for (int j = i; j < variables.size();j++){
            if (correctconnections[i][j] != myconnections[i][j]){
                disagree++;
            }
        }
    }
    
    cout << "total:" << total << endl;
    cout << "disagree: " << disagree << endl;
    cout << "hamming: " << (double)disagree/(double)total << endl;
    
    
    std::cout << "Hello, World!\n";
    return 0;
}
*/

//===================================LLOYD'S ALGORITHM==============================================

vector<double> calccenter(vector<vector<double> > in, unsigned long size){
    vector<double> res;
    double temp[size];
    
    for(int i = 0; i < size;i++){
        temp[i] = 0;
    }
    
    //for each variable
    for(int i = 0; i < in.size();i++){
        
        //index of j
        for(int j = 0; j < size;j++){
            //cout << "test: " << i << ":" << in[i][j] << ",";
            temp[j] += in[i][j];
        }
        //cout << endl;
    }
    
    
    //cout << in.size() << endl;
    for(int i = 0; i < size;i++){
        //cout << temp[i] << ",";
        res.push_back(temp[i]/(double)in.size());
    }
    //cout << endl;
    return res;
}


int main(int argc, const char * argv[]) {
    
    srand(unsigned(time(0)));
    //srand((unsigned)time(NULL));
    
    vector<double> input;
    vector<vector<double> > variables;
    
    
    vector<string> attributes;
    //ifstream ifs ("miniseeds.txt", ifstream::in);
    //ifstream ifs ("seeds_dataset.txt", ifstream::in);
    ifstream ifs ("iris2.data", ifstream::in);
    
    string line;
    long positive;
    
    getline(ifs, line);
    input = getlineandsplit(line);
    cout << "input: " << input.size() << endl;
    positive = input.size()-1;
    variables.push_back(input);
    //amount of successvariables
    vector<double> successvariables;
    
    //building total sets of cleaned data
    while(getline(ifs, line)){
        //cout << "getline: " << line << endl;
        //needed for iris
        //line = line + ",";
        
        input = getlineandsplit(line);
        //cout << input.size() << endl;
        if (input.size() == positive+1){
            variables.push_back(input);
        }
        
    }
    
    
    //checking how many different success variables
    unsigned long clusters;
    bool found = false;
    for (int i = 0; i < variables.size();i++){
        found = false;
        for(int j = 0; j < successvariables.size();j++){
            if (variables[i][positive] == successvariables[j])
                found = true;
        }
        if (!found){
            successvariables.push_back(variables[i][positive]);
        }
    }
    clusters = successvariables.size();
    cout << "success: " << clusters << endl;
    cout << "starting: " << variables.size() << endl;
    
 //build connections table
 int correctconnections[variables.size()][variables.size()];
 int myconnections[variables.size()][variables.size()];
 vector<vector<int> > correctstack;
 for (int i = 0; i < variables.size();i++){
 for (int j = 0; j < variables.size();j++){
 correctconnections[i][j] = 0;
 myconnections[i][j] = 0;
 }
 }
 
 //create the "correct clustering"
 double correct[successvariables.size()];
 for(int i = 0; i < successvariables.size();i++){
 vector<int> temp;
 correctstack.push_back(temp);
 correct[i] = 0;
 }
 
 for(int i = 0; i < variables.size();i++){
 for(int j = 0; j < successvariables.size();j++){
 if(variables[i][positive] == successvariables[j]){
 
 //create connections on each success variable
 correctstack[j].push_back(i);
 
 
 correct[j]++;
 break;
 }
 }
 }
 
 
 //create connections on each success variable
 for (int i = 0; i < correctstack.size();i++){
 
 //only connection if more than 2
 if (correctstack[i].size() > 1){
 
 //connect all things in the stack
 for (int j = 0; j < correctstack[i].size();j++){
 for (int k = j+1; k < correctstack[i].size();k++){
 cout << "j: " << correctstack[i][j] << ",k: "<< correctstack[i][k] << endl;
 correctconnections[correctstack[i][j]][correctstack[i][k]] = 1;
 correctconnections[correctstack[i][k]][correctstack[i][j]] = 1;
 }
 }
 }
 }

 
    cout << "corrects: ";
    for(int i = 0; i < successvariables.size();i++){
        cout << correct[i] << ",";
    }
    cout << endl;
 
    //the chosen 3
    int firstcenter = rand()%variables.size();
    int secondcenter = -1, thirdcenter = -1;
    do{
        secondcenter = rand()%variables.size();
    }while(secondcenter == firstcenter);
 
    do{
        thirdcenter = rand()%variables.size();
    }while((thirdcenter == secondcenter)&&(thirdcenter == firstcenter));
    
    vector<double> curfirst = variables[firstcenter];
    vector<double> cursecond = variables[secondcenter];
    vector<double> curthird = variables[thirdcenter];
    
    
    vector<vector<double> > firstfollowers;
    vector<vector<double> > secondfollowers;
    vector<vector<double> > thirdfollowers;
    
    vector<int> firststack;
    vector<int> secondstack;
    vector<int> thirdstack;
    
    double tempone, temptwo, tempthree;
    
    bool done = false;
    while(!done){
        
        firstfollowers.clear();
        secondfollowers.clear();
        thirdfollowers.clear();
        
        firststack.clear();
        secondstack.clear();
        thirdstack.clear();
        
    for(int i = 0; i < variables.size();i++){
        tempone = calcdist(variables[i], curfirst, variables[i].size()-1);
        temptwo = calcdist(variables[i], cursecond, variables[i].size()-1);
        tempthree = calcdist(variables[i], curthird, variables[i].size()-1);
        cout << "one: " << tempone << ",two: " << temptwo << ",three: " << tempthree << endl;
        if ((tempone <= temptwo) && (tempone <= tempthree)){
            firstfollowers.push_back(variables[i]);
            firststack.push_back(i);
        }else if ((temptwo <= tempone) && (temptwo <= tempthree)){
            secondfollowers.push_back(variables[i]);
            secondstack.push_back(i);
        }else if ((tempthree <= tempone) && (tempthree <= temptwo)){
            thirdfollowers.push_back(variables[i]);
            thirdstack.push_back(i);
        }
    }
    
        vector<double> newfirst, newsecond, newthird;
        
        newfirst = calccenter(firstfollowers, variables[0].size()-1);
        newsecond = calccenter(secondfollowers, variables[0].size()-1);
        newthird = calccenter(thirdfollowers, variables[0].size()-1);
     
        if ((newfirst == curfirst)&&(newsecond == cursecond)&&(newthird == curthird)){
            done = true;
        }else{
            curfirst = newfirst;
            cursecond = newsecond;
            curthird = newthird;
        }
    }
    
    
    for(int i = 0; i < variables[0].size();i++){
        cout << curfirst[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < variables[0].size();i++){
        cout << cursecond[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < variables[0].size();i++){
        cout << curthird[i] << ",";
    }
    cout << endl;
    
    cout << "distribution: " << firstfollowers.size() << "," << secondfollowers.size() << "," << thirdfollowers.size() << endl;
    
    
    //loop
    
    //int firstcentersaved = firstcenter;
    //int secondcentersaved = secondcenter;
    //int thirdcentersaved = thirdcenter;
    
    //tempone, temptwo, tempthree;
    
    
    
    //hamming distance
    
    //first group
    for (int i = 0; i < firststack.size();i++){
        for (int j = i; j < firststack.size();j++){
            myconnections[firststack[i]][firststack[j]] = 1;
            myconnections[firststack[j]][firststack[i]] = 1;
        }
    }
    
    //second group
    for (int i = 0; i < secondstack.size();i++){
        for (int j = i; j < secondstack.size();j++){
            myconnections[secondstack[i]][secondstack[j]] = 1;
            myconnections[secondstack[j]][secondstack[i]] = 1;
        }
    }
    
    //third group
    for (int i = 0; i < thirdstack.size();i++){
        for (int j = i; j < thirdstack.size();j++){
            myconnections[thirdstack[i]][thirdstack[j]] = 1;
            myconnections[thirdstack[j]][thirdstack[i]] = 1;
        }
    }
    
    //hamming distance
    unsigned long total = (variables.size()*(variables.size()-1))/2;
    unsigned long disagree = 0;
    for (int i = 0; i < variables.size();i++){
        for (int j = i; j < variables.size();j++){
            if (correctconnections[i][j] != myconnections[i][j]){
                disagree++;
            }
        }
    }
    
    cout << "total:" << total << endl;
    cout << "disagree: " << disagree << endl;
    cout << "hamming: " << (double)disagree/(double)total << endl;
    
    
    std::cout << "Hello, World!\n";
    return 0;
}


//====================================K MEANS++===========================================
/*
vector<double> calccenter(vector<vector<double> > in, unsigned long size){
    vector<double> res;
    double temp[size];
    
    for(int i = 0; i < size;i++){
        temp[i] = 0;
    }
    
    //for each variable
    for(int i = 0; i < in.size();i++){
        
        //index of j
        for(int j = 0; j < size;j++){
            //cout << "test: " << i << ":" << in[i][j] << ",";
            temp[j] += in[i][j];
        }
        //cout << endl;
    }
    
    
    //cout << in.size() << endl;
    for(int i = 0; i < size;i++){
        //cout << temp[i] << ",";
        res.push_back(temp[i]/(double)in.size());
    }
    //cout << endl;
    return res;
}

int main(int argc, const char * argv[]) {
    
    srand(unsigned(time(0)));
    //srand((unsigned)time(NULL));
    
    vector<double> input;
    vector<vector<double> > variables;
    
    
    vector<string> attributes;
    //ifstream ifs ("miniseeds.txt", ifstream::in);
    ifstream ifs ("seeds_dataset.txt", ifstream::in);
    //ifstream ifs ("iris2.data", ifstream::in);
    
    string line;
    long positive;
    
    getline(ifs, line);
    input = getlineandsplit(line);
    cout << "input: " << input.size() << endl;
    positive = input.size()-1;
    variables.push_back(input);
    //amount of successvariables
    vector<double> successvariables;
    
    //building total sets of cleaned data
    while(getline(ifs, line)){
        //cout << "getline: " << line << endl;
        //needed for iris
        //line = line + ",";
        
        input = getlineandsplit(line);
        //cout << input.size() << endl;
        if (input.size() == positive+1){
            variables.push_back(input);
        }
        
    }
    
    
    //checking how many different success variables
    unsigned long clusters;
    bool found = false;
    for (int i = 0; i < variables.size();i++){
        found = false;
        for(int j = 0; j < successvariables.size();j++){
            if (variables[i][positive] == successvariables[j])
                found = true;
        }
        if (!found){
            successvariables.push_back(variables[i][positive]);
        }
    }
    clusters = successvariables.size();
    cout << "success: " << clusters << endl;
    cout << "starting: " << variables.size() << endl;
    
 //build connections table
 int correctconnections[variables.size()][variables.size()];
 int myconnections[variables.size()][variables.size()];
 vector<vector<int> > correctstack;
 for (int i = 0; i < variables.size();i++){
 for (int j = 0; j < variables.size();j++){
 correctconnections[i][j] = 0;
 myconnections[i][j] = 0;
 }
 }
 
 //create the "correct clustering"
 double correct[successvariables.size()];
 for(int i = 0; i < successvariables.size();i++){
 vector<int> temp;
 correctstack.push_back(temp);
 correct[i] = 0;
 }
 
 for(int i = 0; i < variables.size();i++){
 for(int j = 0; j < successvariables.size();j++){
 if(variables[i][positive] == successvariables[j]){
 
 //create connections on each success variable
 correctstack[j].push_back(i);
 
 
 correct[j]++;
 break;
 }
 }
 }
 
 
 //create connections on each success variable
 for (int i = 0; i < correctstack.size();i++){
 
 //only connection if more than 2
 if (correctstack[i].size() > 1){
 
 //connect all things in the stack
 for (int j = 0; j < correctstack[i].size();j++){
 for (int k = j+1; k < correctstack[i].size();k++){
 cout << "j: " << correctstack[i][j] << ",k: "<< correctstack[i][k] << endl;
 correctconnections[correctstack[i][j]][correctstack[i][k]] = 1;
 correctconnections[correctstack[i][k]][correctstack[i][j]] = 1;
 }
 }
 }
 }

 
    cout << "corrects: ";
    for(int i = 0; i < successvariables.size();i++){
        cout << correct[i] << ",";
    }
    cout << endl;
 
    double distances[variables.size()][2];
 
 
    //the chosen 3
    //with k means++, give probabilities
    int firstcenter = rand()%variables.size();
    cout << "first: " << firstcenter << endl;
 
 
    //populate tables (first time)
    double amount = 0;
    int mytotal = 0;
    for (int rows = 0; rows < variables.size(); rows++){
            if (rows == firstcenter){
                distances[rows][0] = 0;
            }
            else{
                amount = calcdist(variables[rows], variables[firstcenter], positive);
                distances[rows][0] = amount;
                distances[rows][1] = ceil(amount);
                mytotal += ceil(amount);
            }
            //cout << amount << ",";
    }
    int tickets[mytotal];
    for(int i = 0; i < mytotal;i++){
        for(int j = 0; j < variables.size();j++){
            tickets[i] = j;
            i++;
        }
    }
    int randomgen = rand()%mytotal;
    int secondcenter = tickets[randomgen];
    cout << "second: " << secondcenter << endl;
    
    //populate tables (second time)
    amount = 0;
    mytotal = 0;
    for (int rows = 0; rows < variables.size(); rows++){
        if ((rows == firstcenter)||(rows == secondcenter)){
            distances[rows][0] = 0;
        }
        else{
            amount += calcdist(variables[rows], variables[firstcenter], positive);
            amount += calcdist(variables[rows], variables[secondcenter], positive);
            distances[rows][0] = amount;
            distances[rows][1] = ceil(amount);
            mytotal += ceil(amount);
        }
        //cout << amount << ",";
    }
    int tickets2[mytotal];
    for(int i = 0; i < mytotal;i++){
        for(int j = 0; j < variables.size();j++){
            tickets2[i] = j;
            i++;
        }
    }
    randomgen = rand()%mytotal;
    int thirdcenter = tickets2[randomgen];
    cout << "third: " << thirdcenter << endl;
    
    
    
    vector<double> curfirst = variables[firstcenter];
    vector<double> cursecond = variables[secondcenter];
    vector<double> curthird = variables[thirdcenter];
    
    
    vector<vector<double> > firstfollowers;
    vector<vector<double> > secondfollowers;
    vector<vector<double> > thirdfollowers;
    
    vector<int> firststack;
    vector<int> secondstack;
    vector<int> thirdstack;
    
    double tempone, temptwo, tempthree;
    
    bool done = false;
    while(!done){
        
        firstfollowers.clear();
        secondfollowers.clear();
        thirdfollowers.clear();
        
        firststack.clear();
        secondstack.clear();
        thirdstack.clear();
        
        for(int i = 0; i < variables.size();i++){
            tempone = calcdist(variables[i], curfirst, variables[i].size()-1);
            temptwo = calcdist(variables[i], cursecond, variables[i].size()-1);
            tempthree = calcdist(variables[i], curthird, variables[i].size()-1);
            //cout << "one: " << tempone << ",two: " << temptwo << ",three: " << tempthree << endl;
            if ((tempone <= temptwo) && (tempone <= tempthree)){
                firstfollowers.push_back(variables[i]);
                firststack.push_back(i);
            }else if ((temptwo <= tempone) && (temptwo <= tempthree)){
                secondfollowers.push_back(variables[i]);
                secondstack.push_back(i);
            }else if ((tempthree <= tempone) && (tempthree <= temptwo)){
                thirdfollowers.push_back(variables[i]);
                thirdstack.push_back(i);
            }
        }
        
        vector<double> newfirst, newsecond, newthird;
        
        newfirst = calccenter(firstfollowers, variables[0].size()-1);
        newsecond = calccenter(secondfollowers, variables[0].size()-1);
        newthird = calccenter(thirdfollowers, variables[0].size()-1);
        
        if ((newfirst == curfirst)&&(newsecond == cursecond)&&(newthird == curthird)){
            done = true;
        }else{
            curfirst = newfirst;
            cursecond = newsecond;
            curthird = newthird;
        }
    }
    
    
    for(int i = 0; i < variables[0].size();i++){
        cout << curfirst[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < variables[0].size();i++){
        cout << cursecond[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < variables[0].size();i++){
        cout << curthird[i] << ",";
    }
    cout << endl;
    
    cout << "distribution: " << firstfollowers.size() << "," << secondfollowers.size() << "," << thirdfollowers.size() << endl;
    
    
    //loop
    
    //int firstcentersaved = firstcenter;
    //int secondcentersaved = secondcenter;
    //int thirdcentersaved = thirdcenter;
    
    //tempone, temptwo, tempthree;
    
    
    //hamming distance
    
    //first group
    for (int i = 0; i < firststack.size();i++){
        for (int j = i; j < firststack.size();j++){
            myconnections[firststack[i]][firststack[j]] = 1;
            myconnections[firststack[j]][firststack[i]] = 1;
        }
    }
    
    //second group
    for (int i = 0; i < secondstack.size();i++){
        for (int j = i; j < secondstack.size();j++){
            myconnections[secondstack[i]][secondstack[j]] = 1;
            myconnections[secondstack[j]][secondstack[i]] = 1;
        }
    }
    
    //third group
    for (int i = 0; i < thirdstack.size();i++){
        for (int j = i; j < thirdstack.size();j++){
            myconnections[thirdstack[i]][thirdstack[j]] = 1;
            myconnections[thirdstack[j]][thirdstack[i]] = 1;
        }
    }
    
    //hamming distance
    unsigned long total = (variables.size()*(variables.size()-1))/2;
    unsigned long disagree = 0;
    for (int i = 0; i < variables.size();i++){
        for (int j = i; j < variables.size();j++){
            if (correctconnections[i][j] != myconnections[i][j]){
                disagree++;
            }
        }
    }
    
    cout << "total:" << total << endl;
    cout << "disagree: " << disagree << endl;
    cout << "hamming: " << (double)disagree/(double)total << endl;
    
    
    
    std::cout << "Hello, World!\n";
    return 0;
}
*/


