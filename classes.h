/*
 * Linker.h
 *
 *  Created on: Feb 12, 2013
 *      Author: Ankit Goel
 */

#ifndef CLASSES_H_
#define CLASSES_H_

#include	<list>
#include	<vector>
#include	<map>
#include	<string>
#include	<fstream>
#include	<iostream>
#include	<sstream>
#include	<iomanip>

#include	<assert.h>

using namespace std;

//--------------------------------------------------------------

enum returncode {
	retOK = 0,
	retCantOpenFile,
	retParseError,
	retUsage
};

//---------------------------------------------------------------------------

// forward reference:
struct SymbolTable;
/* STRUCT SymbolData contains the Internal Parameters that are 
   subject to change upon the execution of each of its stacks */

//---------------------------------------------------------------------------
struct SymbolData {
	static	const unsigned kNotDefinedYet;	
    unsigned                ArrivalTime;
    unsigned                ExecutionTime;
    unsigned                SJF; 
    unsigned                     numOfRuns;
    unsigned                     numOfBlocks;
    int                     numOfReady;
    int                     timeWhenTerminated;
    string                  state;
    unsigned                quantumCPUBURST;
    unsigned                IOBURST4QUANTUM;
    unsigned                currentComputation; 
	unsigned				moduleDefinedIn;
	unsigned				absoluteAddress;
    unsigned                CURRENTIOORCPUBURST;
	unsigned                UDRI4CPUBURST;
    bool                    COMPTUM; 
    bool                    MANAGERMADECONTACT;// IF A PROCESSES IS IN THE BLOCKEDQUEUE IT SHOULD BE ITERATED IN THE NORMAL PLACE
    bool                    TURNSTARON; // IF MANAGERMADECONTACT this cycle THE MANAGER SHOULD NOT AGAIN ACESSESS THE SAME TASK THIS CYCLE. 
    list<unsigned>          Instructions;
    list<string>            COMMANDS;
    vector<int>             REQUESTED; 
    vector<int>             RESOURCESNEEDED;
    vector<int>             claims;
    bool                    marked; 
	
	map< unsigned,
		 list<unsigned> >	externalOccurrences;

	SymbolData() :
		moduleDefinedIn(kNotDefinedYet), absoluteAddress(0) {
	}
	SymbolData( unsigned modulenum, unsigned absAddr ) :
		moduleDefinedIn(modulenum), absoluteAddress(absAddr) {
	}

	bool addOccurrence( unsigned modulenum, unsigned instrnum );
    
};

/*Struct contains a map called symbols <pid, internal parameters> Using this 
 mapping scheme allows us acess each task's internal parameters sequentially by simply
 iterating from i to # of TASKS I.E symtab.symbols[i].internalparameter */
//---------------------------------------------------------------------------
struct SymbolTable {
	// map: token (string) -> SymbolData
	map< int, SymbolData >   symbols;
    // module# -> its base address, for printing the symbol table with info requested
    vector<unsigned>            baseAddressOfModule;
    int NUMOFTASKS;
    int totalCycleTime;
    int totalTimeElapsed;
    bool outOFDeadlock;
    int terminatedProc;
    int systemCheck; 
    /*
	bool	symbolIsInTable( string token ) {
		return symbols.count(token) > 0;
	}
	bool	symbolInTableIsDefined( string token ) {
		//assert( symbolIsInTable(token) );
		return ( symbols[token].moduleDefinedIn != SymbolData::kNotDefinedYet );
	}
	bool	symbolIsDefined( string token ) {	// in definition list of a module we've met (pass1)
		return symbolIsInTable(token) && symbolInTableIsDefined(token);
	}
	bool	symbolIsUsed( string token) {		// in table but not "defined"
		return symbolIsInTable(token) && ! symbolIsDefined(token);
	}

	// returns false if already defined
	bool	defineSymbol( string token, unsigned modulenum, unsigned absAddr );

	// returns false if occurrence is already on list (dup listing in module's use list) -
	// warning & ignore duplicate mention
      **/
	bool	addSymbolOccurrence( int token, unsigned modulenum, unsigned instrnum );
    
    void print();
    
    int RandomOs  ( int randomNumber,int Interval);
    
    
	 void doUpdatesChanges( SymbolTable symtab,vector<int>RESOURCES,int ProccessTable);
    
  
    void doUpdatesChangesVERBOSE( SymbolTable symtab,vector<int>RESOURCES,int ProccessTable);
    
    void BANKER(SymbolTable symtab, vector<int>RESOURCES, int ProccessTable);
    
     void BANKERVERBOSE(SymbolTable symtab, vector<int>RESOURCES, int ProccessTable);

};


#endif /* CLASSES_H_ */
