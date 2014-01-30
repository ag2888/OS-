/*
 * Linker.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Ankit Goel
 */


#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "classes.h"
#include <pthread.h>
using namespace std;

//--------------------------------------------------------------
#define EXIT_FAILURE 1 
#define EXIT_SUCESSES 0
void giveUsageAndExit();
bool skipWhitespaceNow( ifstream& ifs );

//--------------------------------------------------------------
int i = 0;
int main ( int argc, char* argv[] ) {
    
    
    int length;
    SymbolTable symtab;
    
    // Parse the Command line for the -Verbose Flag
    bool isVerboseOn;
	if ( argc < 2 )
		giveUsageAndExit();
    string parseCommandLine = argv[1];
    string flag  = "--verbose";
    if (parseCommandLine.compare(flag) != 0){
        i--;
        isVerboseOn=true; // Use this boolean to determine the Detailed/regular Output 
    }
    
    ifstream inputFile ( argv[2+i] );
    string v1;
    vector<string> v2;
    int getLengthofArguements = 0; 
    vector<string> v3;
    list<unsigned> readyQueue; 
    int n = 0;
    int j = 0; 
    stringstream ss;
    string s = "1234";
    int a = 0;
    
    // PARSE STRINGS AS EACH ONE SEPARATED BY A WHITE SPACE
    while (inputFile.good()) {
          
        inputFile >> skipws;
        inputFile >> v1;
        getLengthofArguements++;
        v2.push_back(v1);
    }
   
    
    int proccesTable [v2.size()];
    // process the input get the integer values get rid of parens, 
  
    
    // Extract the String and Processes it 
    while( n < v2.size() && a <v2.size()) {
        //  GET THREE DIGIT CODE FROM EACH COMMAND 
        if (v2[a].length()==1){
            string myStream = v2[a];
            istringstream buffer(myStream);
            int value;
            buffer >> value; // convert STRING to INT 
            proccesTable[n] = value;
            n++;
    
        }
            a++;
    }
    

    
    //  RESOURCES[0] -- RESOURCES [1] .... RESOURCES[2] = # of Units
    vector<int>RESOURCES;
    
    /* Store each three digit code into a list called ProcessTable */ 
            /* Also store the Units & Resources & TASKS*/
    int max = proccesTable[0];
    
    for(int i = 1; i <= max; i++ ) {
        //cout << "\n";
        if(max>0){
            // Create Entries in the Task Manager 
            symtab.addSymbolOccurrence(i, 0, 0); 
            symtab.NUMOFTASKS++; 
            //cout<<i;
      
        }
        
    }
   
    /* If there is only one Resource than there is an offset of 1 in the processesTable */ 
    if (proccesTable[1]>1) {
        
        int j = 2;// start from the Resource Units
        int k = 2 + proccesTable[1];
        
        // PUSH BACK N RESOURCES PRESENT WITH THEIR CORRESPONDING UNITS 
        for (int i = 1; i<=proccesTable[1]; i++) {
            RESOURCES.push_back(proccesTable[j]);
            j++; 
        }
        
        for(int i = 2 + proccesTable[1]; i < n ; i=i+3) {
            
            // How Many Commands Does Each Task Take & more specifically what are the # of instructions on each stack
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i]);
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i+1]);
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i+2]);
            symtab.symbols[proccesTable[i]].COMMANDS.push_back(v2[k]);
            k= k+4;
 
                
            
            // consectutivey push the next three
            
        }
        
    }
    
    
    
    if(proccesTable[1]==1){
        int j = 2;
        // Only One Resource in Consideration 
        RESOURCES.push_back(proccesTable[j]);
        int commandGetter= 3; 
        
        for(int i = 3; i < n ; i=i+3) {
            // How Many Commands Does Each Task Take & subset of Instructions = 3 * cmd therefore
            // As one Atomic Action let us Processes the three unsigned digits following
            // the command 
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i]);
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i+1]);// consectutivey push the next three
            symtab.symbols[proccesTable[i]].Instructions.push_back(proccesTable[i+2]);
            symtab.symbols[proccesTable[i]].COMMANDS.push_back(v2[commandGetter]);
            if(commandGetter < v2.size())
           
                commandGetter=commandGetter+4;
        }
        
        
     
        
    }
    
    list<unsigned>::iterator cc;
    
    if(isVerboseOn!=true) {
        
        /*  
                                     Both Banker's Algorithm and Optimistic Manager
            Note about the Detailed Runs. Once a task has been blocked it is added to the FIFO blockedQueue and it is inherently checked during every
            cycle. It is not written that the task manager is checking for blocked tasks every cycle in the order they come but it does. If a processes is
            unblocked and taken off the blockedqueue it is indicated by printing its status first & until is it unblocked it will not be printed again until it is unblocked
            stating that its  request has been granted
            Also ( # of times a task blocks) / (Duration of Task) is expressed as a floating point number indicated by the decimal accuracy
        */
        symtab.doUpdatesChanges(symtab, RESOURCES, proccesTable[0]);
        
        cout<<"\n";    cout<<"\n";
        symtab.BANKER(symtab, RESOURCES, proccesTable[0]);
        cout<<"\n";    cout<<"\n"; 
    }
        
        
        
    else  {
        symtab.doUpdatesChangesVERBOSE(symtab,RESOURCES,proccesTable[0]);
        cout<<"\n";    cout<<"\n";
        symtab.BANKERVERBOSE(symtab, RESOURCES, proccesTable[0]);
        cout<<"\n";
    }
    

     
}
bool skipWhitespaceNow( ifstream& ifs ) {
	char c;
	while( ifs.get(c) ) {
		if ( ! isspace(c) ) {
			ifs.unget();
			return true;	// there's more of ifs after any whitespace we just skipped
		}
	}
	return false;			// eof - there's no more of ifs
};

void giveUsageAndExit() {
	std::cerr << "Usage: " << endl;
	std::cerr << "    Linker <filename>" << endl;
	exit(retUsage);
}
