 /*
 * classes.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Ankit Goel
 */


#include "classes.h"

#include	<limits>
// This is better than UINT_MAX?
#define CPP_UINT_MAX     ( numeric_limits<unsigned int>::max() )

/*static*/ const unsigned SymbolData::kNotDefinedYet  = CPP_UINT_MAX;


//---------------------------------------------------------------------------
// SymbolData methods
//---------------------------------------------------------------------------
bool SymbolData::addOccurrence( unsigned modulenum, unsigned instrnum ) {
	// create empty list of ints, set externalOccurrences[modulenum] = that list
	// add this instrnum to that list (at front, via push_front ?
	// instrnums tend to arrive backwards)
	if ( externalOccurrences.count(modulenum) == 0 ) {
		// can't be a dup: no list for modulenum
		externalOccurrences[modulenum] = *(new list<unsigned>);	// TODO this DOES call copy ctor, yes? no?
		externalOccurrences[modulenum].push_front(instrnum);
		return true;
	}
	// Get corresponding list of ints = instruction#s = externalOccurrences[modulenum]
	// else, there is a list for modulenum;
	// if instrnum isn't already there, add it and return true,
	// else return false
	list<unsigned> & moduleOccs = externalOccurrences[modulenum];
	list<unsigned>::iterator ii;
	for ( ii = moduleOccs.begin(); ii != moduleOccs.end(); ++ii ) {
		if ( *ii == instrnum )				// it's a dup
			return false;					// do nothing and return false
	}
	moduleOccs.push_front(instrnum);
	return true;
}

int SymbolTable:: RandomOs( int randomNumber,  int Interval) {
    // Find the CPU/IOBURST
    int x = (randomNumber % Interval) + 1;
    return x;
}


// Add Each Task to the Asssociative Container use break statements to avoid RACE CONDITIONS 
bool SymbolTable::addSymbolOccurrence( int token, unsigned modulenum, unsigned instrnum ) {
	                                                               
    symbols[token] = *(new SymbolData());	// use default ctor (and then copy)
	
	SymbolData& symdata = symbols[token];
	return true;// true if this isn't a duplicate occurrence
}
/* This Method Essentially Updates and Changes the Struct symtab ( which contains an Associative Memory thats map from (i to the # of taks)
   each task's internal parameters such as its status, its individual locks, its INSTRUCTIONS,  its COMMANDS, time when terminated, numOfTimesBlocked*/

void SymbolTable::doUpdatesChanges(SymbolTable symtab,vector<int>RESOURCES,int Proccesstable)  {
    
    symtab.systemCheck=0; 
    symtab.terminatedProc=0; 
    list<string>::iterator gg;
    list<unsigned>::iterator cc;
    list<unsigned>::iterator BB;
    vector <int> BLOCKEDQUEUE;    // FIFO QUEUE OF PROCESSES
    vector<int>::iterator dd;
    vector<int>::iterator POLAR;
    vector<int>AVAIL;
    list<unsigned> TMPSTACK; // Get the Entire set of Instructions sequentially for each task
    list<unsigned> TMPLIST;  // Get the First Three Free Digits of the Instructions 
    string         TMPCMD;    // Get Temporary Command
    int            DigitForPrintout; // PID
    int            CurrentResource;  // RESOURCE ID 
    symtab.totalCycleTime=0; // START THE CLOCK AT ZERO 
    int             REQUEST;
    int             Count = 0;
    symtab.outOFDeadlock=true; 
    int             max = Proccesstable;
    
    
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        
        
        for(int i = 1; i <=Proccesstable; i++) {
            
            // Currently Each Task has Zero Resources during Initiation 
            symtab.symbols[i].REQUESTED.push_back(0);
        }
        
    }
    
    
    // Initially All Resources are Available before program counter starts Decoding Instructions 
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        AVAIL.push_back(*dd);
    }
    
    // Set TRUE to LOCKS on Critical Regions
    for(int i = 1; i <=Proccesstable; i++) {
        symtab.symbols[i].TURNSTARON=true;
        symtab.symbols[i].MANAGERMADECONTACT=true;
        symtab.symbols[i].COMPTUM=false;
        symtab.symbols[i].numOfBlocks=0;
    }
    
    
    while ( symtab.systemCheck < Proccesstable  ){
            cout<<"\n"; 
            cout<< "During Cycle "; cout<<symtab.totalCycleTime; cout<<"-"; cout<<symtab.totalCycleTime + 1; cout<<":"; cout<<"\n"; ;
            symtab.totalCycleTime++;
        
           //DEADLOCK If you detect the deadlock at cycle k, you abort the task(s) at cycle k and hence its/their resources
           //  become available at cycle k+1.
        
            if(BLOCKEDQUEUE.size()==Proccesstable-symtab.terminatedProc){
               
                // System Is in Deadlock and will Continue to Be Until someone's request has been granted 
                symtab.outOFDeadlock=false;
                list<unsigned>getTask;
                int minimum = 0; 
                
                // FIND LOWERED NUMBERED TASK
               
                    for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
            
                        getTask.push_back(*dd);
                    }
                getTask.sort();
                minimum=getTask.front();
                getTask.clear(); // free up memory 
               
                // DELETE POSITION FROM BLOCKEDQUEUE
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                    if (*dd==minimum) {
                        BLOCKEDQUEUE.erase(dd); // The container is modified.
                        //The elements removed are modified. Concurrently accessing or modifying other elements is safe, although iterating ranges that include the removed elements is not.
                    }
                }
               
                // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
                for(int i = 0; i < RESOURCES.size(); i++){
                    RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                    AVAIL[i]= AVAIL[i] + symtab.symbols[minimum].REQUESTED[i];
                }
            
                symtab.symbols[minimum].REQUESTED.clear();
                
                // UPDATE THE STATE
                symtab.symbols[minimum].state="ABORTED";
                symtab.systemCheck++; 
                
                //PRINT THE STATE
                cout <<"\t";cout<<"Task "; cout<< minimum; cout<<" was "; cout<< symtab.symbols[minimum].state; cout<<"\n"; 
            
                // Now that the Resources are available and the lowest task has been aborted
                // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS 
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                    
                    for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                        
                        // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                        TMPSTACK.push_back(*cc);
                    }
                    // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPSTACK.clear(); 
                    // GET FIRST COMMAND ON THE LIST
                    TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                    
                    // Now Clear TMPLIST 
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // Check for Blocked Tasks and see if The Requests can be granted
                    if (REQUEST <= RESOURCES[CurrentResource-1]) {
                        symtab.outOFDeadlock=true; 
                       
                        
                    }
                    
                    
                }
                // STILL IN DEADLOCK 
                if(symtab.outOFDeadlock==false){
                    list<unsigned>getPosition; 
                    // FIND LOWERED NUMBERED TASK
                    
                    for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                        
                        getTask.push_back(*dd);
                        
                    }
                    getTask.sort();
                    minimum=getTask.front();
                    getTask.clear();
                  
                    
                   
                    
                    // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
                    for(int i = 0; i < RESOURCES.size(); i++){
                        RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                        AVAIL[i]= AVAIL[i] + symtab.symbols[minimum].REQUESTED[i];
                    }
                    
                    symtab.symbols[minimum].REQUESTED.clear();
                    symtab.symbols[minimum].state="ABORTED";
                    symtab.systemCheck++; 
                    cout <<"\t";cout<<"Task "; cout<< minimum; cout<<" was "; cout<< symtab.symbols[minimum].state; cout<<"\n";
                    
                    // Now that the Resources are available and the lowest task has been aborted
                    // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS
                    for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                        
                        for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                            
                            // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                            TMPSTACK.push_back(*cc);
                        }
                        
                        // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPSTACK.clear(); 
                        // GET FIRST COMMAND ON THE LIST
                        TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                        
                        
                        DigitForPrintout=TMPLIST.front();
                        TMPLIST.pop_front();
                        CurrentResource=TMPLIST.front();
                        TMPLIST.pop_front();
                        REQUEST=TMPLIST.front();
                        TMPLIST.pop_front();
                        
                        // Check for Blocked Tasks and see if The Requests can be granted
                        if (REQUEST <= RESOURCES[CurrentResource-1]) {
                            symtab.outOFDeadlock=true; // Update Lock Variables 
                            
                            
                        }
                        
                        
                  
                    }
                }
                // AGAIN STILL IN DEADLOCK 
                if (symtab.outOFDeadlock==false) {
                    
                    // FIND LOWERED NUMBERED TASK
                    
                    for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                        
                        getTask.push_back(*dd);
                    }
                    getTask.sort();// SORT AND TAKE FRONT ELEMENT 
                    minimum=getTask.front();
                    getTask.clear(); 
                    // DELETE POSITION FROM BLOCKEDQUEUE
                 
                    
                    // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
                    for(int i = 0; i < RESOURCES.size(); i++){
                        RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                        
                    }
                    
                    symtab.symbols[minimum].REQUESTED.clear();
                    symtab.symbols[minimum].state="ABORTED";
                    symtab.systemCheck++;
                    cout<<"Task "; cout<< minimum; cout<<" has been "; cout<<symtab.symbols[minimum].state; cout<<"\n";
                    
                    // Now that the Resources are available and the lowest task has been aborted
                    // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS
                    for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                        
                        // Make sure to not iterate through a task that is aborted
                        if(symtab.symbols[*dd].state!="ABORTED"){
                        
                        for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                            
                            // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                            TMPSTACK.push_back(*cc);
                        }
                        // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPSTACK.clear(); 
                        // GET FIRST COMMAND ON THE LIST
                        TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                        
                        
                        DigitForPrintout=TMPLIST.front(); // Get the TASK ID 
                        TMPLIST.pop_front();
                        CurrentResource=TMPLIST.front();  // Current Resource ID 
                        TMPLIST.pop_front();
                        REQUEST=TMPLIST.front();          // Amount of Resource
                        TMPLIST.pop_front();
                        
                        // Check for Blocked Tasks and see if The Requests can be granted
                        if (REQUEST <= RESOURCES[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                            // System is no longer in a deadlock 
                            symtab.outOFDeadlock=true;
                           
                            
                        }
                        
                        
                    }
                    }
                }
            
            
        }
        
        if(BLOCKEDQUEUE.size()>1){
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                // Aborted Tasks stay on the blockedQueue so Check that during iteration that an Aborted Task is not updated and looked at
                if(symtab.symbols[*dd].state!="ABORTED"){
                for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); 
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                
                //Extract Task Info and pass it to MANAGER
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                
                // Check for Blocked Tasks and see if The Requests can be granted
                if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                    symtab.symbols[*dd].COMMANDS.pop_front();
                    int counter = 0;
                    while ( counter < REQUEST){
                        RESOURCES[CurrentResource-1]--;
                        AVAIL[CurrentResource-1]--; 
                        counter++;
                        // KEEP TRACK SEQUENTIALLY OF REQUESTED[0] .... THE PROCESSES TAKES 
                        symtab.symbols[*dd].REQUESTED[CurrentResource-1]++;
                    }
                    symtab.symbols[*dd].state="Requested and was Granted ";
                    symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front();
                    cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[*dd].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                    symtab.symbols[*dd].MANAGERMADECONTACT=true; symtab.symbols[*dd].TURNSTARON=false; 
                    BLOCKEDQUEUE.erase(dd);
                    break; // Break From Iterating through the Container to AVOID RACE CONDITION 
                }
                
                
            }
            }
           
        }
        
        if(BLOCKEDQUEUE.size()==1){
            
            int i = BLOCKEDQUEUE[0];
            // Again skip over Aborted Tasks 
            if (symtab.symbols[i].state!="ABORTED") {
                
            
            for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                
                // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                TMPSTACK.push_back(*cc);
            }
            // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPSTACK.clear(); 
            // GET FIRST COMMAND ON THE LIST
            TMPCMD = symtab.symbols[i].COMMANDS.front();
            
            
            DigitForPrintout=TMPLIST.front();
            TMPLIST.pop_front();
            CurrentResource=TMPLIST.front();
            TMPLIST.pop_front();
            REQUEST=TMPLIST.front();
            TMPLIST.pop_front();
            
            // Check for Blocked Tasks and see if The Requests can be granted
            if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                symtab.symbols[i].COMMANDS.pop_front();
                int counter = 0;
                while ( counter < REQUEST){
                    RESOURCES[CurrentResource-1]--;
                    AVAIL[CurrentResource-1]--;
                    counter++;
                    symtab.symbols[i].REQUESTED[CurrentResource-1]++; // 
                }
                
                // UPDATE STATE
                symtab.symbols[i].state="Requested and was Granted ";
                
                // POP INSTRUCTIONS
                symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                
                // Print the State
                cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                symtab.symbols[i].MANAGERMADECONTACT=true; symtab.symbols[i].TURNSTARON=false;
                BLOCKEDQUEUE.clear();
                
            }
            
            
            }
            
            
        }
        
        
   
    
    
        // Iterate Through ALL Task READING & DECODING THEIR INSTRUCTIONS 
    for(int i = 1; i <=Proccesstable; i++) {
        
        //Increment Blocked Processes numOfBlocks
        if(symtab.symbols[i].state=="Blocked"){
            symtab.symbols[i].numOfBlocks++; 
        }
        
        
        
        
        // Make Sure that the Task in Question has not just been checked, is blocked, or is terminated
        
       // MANGERMADECONTACT - TRUE IF IT WAS JUST GRANTED A RESOURCE WHILE BEING BLOCKED || IT IS NOT IN A BLOCKEDQUEUE 
       // TURNSTARON- FALSE IF JUST GRANTED A RESOURCE WHILE BEING BLOCKED
        
       
        
       // Make Sure that the Task in Question has not just been checked, is blocked, or is terminated
        
        if((symtab.symbols[i].MANAGERMADECONTACT) && (symtab.symbols[i].TURNSTARON) && (symtab.symbols[i].state !=
           "Terminated" ) && (symtab.symbols[i].state!="ABORTED")){
        
            for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
         
                // FETCH INSTRUCTIONS 
                TMPSTACK.push_back(*cc);
            }
            
            // CONSIDER ONLY THE FIRST THREE INSTRUCTIONS ORDERED SEQUENTIALLY AS THEY COME IN THE PROGRAM AND THEN CLEAR TMPSTACK
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPLIST.push_back(TMPSTACK.front());
            TMPSTACK.pop_front();
            TMPSTACK.clear(); 
            
            // GET FIRST COMMAND ON THE LIST
            TMPCMD = symtab.symbols[i].COMMANDS.front();
        
        
        
            //ANALYZE AND DECODE INSTRUCTIONS
        
                if ( TMPCMD == "compute") {
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                
                //Initially the COMPTUM for every Task is FALSE
               
                if (symtab.symbols[i].COMPTUM==false) {
                    symtab.symbols[i].currentComputation=CurrentResource;
                    symtab.symbols[i].COMPTUM=true;
                }
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
              
                symtab.symbols[i].state="Computing "; cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" FOR "; cout<<symtab.symbols[i].currentComputation;
                cout<<"\n";
                
                // TASK IS ALREADY IN COMPUTATION STAGE
                if (symtab.symbols[i].currentComputation>0) {
                    symtab.symbols[i].currentComputation--;
                }
                
                // Task finishes its Computation
                if (symtab.symbols[i].currentComputation==0) {
                    symtab.symbols[i].COMPTUM=false;
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    symtab.symbols[i].COMMANDS.pop_front();
                }
                
            }
                        //ANALYZE THE COMMAND
            
                if ( TMPCMD == "initiate") {
                    // Basically This Data is Refuse or Trash Just Get It Out of the Optimistic Manager's  Way 
                    symtab.symbols[i].COMMANDS.pop_front();
                    // GET PID OF THE TASK
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    symtab.symbols[i].state="Initialilized"; cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" Resource "; cout<<CurrentResource;
                    cout<<"\n";
                }
        
                        //ANALYZE THE COMMAND
                if( TMPCMD == "request") {
            
                    DigitForPrintout=TMPLIST.front(); // PID
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front(); // RESOURCE ID
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front(); // AMOUNT OF RESOURCE
                    TMPLIST.pop_front();
                    // AVAILABLE RESOURCES ARE CHECKED
                    if (REQUEST <= AVAIL[CurrentResource-1]) {
                        symtab.symbols[i].COMMANDS.pop_front();
                        int counter = 0;
                        while ( counter < REQUEST){
                            RESOURCES[CurrentResource-1]--; // DECREMENT THE RESOURCES
                            AVAIL[CurrentResource-1]--; // DECREMENT THE AVAILABLE RESOURCES
                            counter++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]++; 
                        }
                        symtab.symbols[i].state="Requested and was Granted "; // GET STATE OF THE TASK
                        symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                        cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                    }
           
                        //Update BLOCKEDQUEUE & state & numOfBlock 
                    else if (REQUEST > AVAIL[CurrentResource-1]) {
                        symtab.symbols[i].state="Blocked";
                        symtab.symbols[i].numOfBlocks++; 
                        BLOCKEDQUEUE.push_back(i);
                        symtab.symbols[i].MANAGERMADECONTACT=false;
                        cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                    }
            
        }
            
        
                //ANALYZE THE COMMAND
                if(TMPCMD == "release"){
                        DigitForPrintout=TMPLIST.front();
                        TMPLIST.pop_front();
                        CurrentResource=TMPLIST.front();
                        TMPLIST.pop_front();
                        REQUEST=TMPLIST.front();
                        TMPLIST.pop_front();
            
                    // POP THE COMMAND- THERE ARE EXACTLY 1/3 OF COMMANDS AS INSTRUCTIONS
                
                        symtab.symbols[i].COMMANDS.pop_front();
                        int RESTOCK = 0;
            
                    // UPDATE RESOURCES AND REQUESTED
                
                while (RESTOCK < REQUEST) {
                    RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                    RESTOCK++;
                    symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                }
            
            // UPDATE THE STATE
                
                symtab.symbols[i].state="released";
            
            // POP THE NEXT THREE DIGITS PERMANENTLY
            
                symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
            
            // PRINT OUT STATE
                
                cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" ";  cout<<REQUEST; cout<<" UNITS OF "; cout<<" RESOURCE "; cout<<CurrentResource; cout<<"\n";
       
        }
            //ANALYZE THE COMMAND
            if(TMPCMD == "terminate"){
               
                // GET FIRST THREE DIGITS 
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                //DigitForPrintout ~ PID , CurrentResource ~ RESOURCE PID
                
                //Delete Next COMMAND
                symtab.symbols[i].COMMANDS.pop_front();
               
                
                //Update the State 
                symtab.symbols[i].state="Terminated";
                symtab.systemCheck++; 
                symtab.terminatedProc++;               // DELETE NEXT THREE DIGITS FROM INSTRUCTIONS
                symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                
                // note time when the Task Terminates
                symtab.symbols[i].timeWhenTerminated=symtab.totalCycleTime-1;
                
                // Print the State
                cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" at "; cout<<symtab.totalCycleTime-1;  cout<<" "; cout<<"\n";
            }
            
            
            
            }
                // Force Iteration of Task that was Just knocked off the BlockedQueue to be read come around next time 
                    
                if(symtab.symbols[i].TURNSTARON==false){
                        
                    symtab.symbols[i].TURNSTARON=true;
                }
    
    
    
    }
        
        
        
        
        
        AVAIL.clear(); // Clear This memory has it NEEDS TO UPDATED AFTER EVERY CLOCK CYCLE
        
        //SINCE A RESOURCE IS ONLY AVAILABLE AFTER A CYCLE UPDATE AVAIL HERE
        for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
            AVAIL.push_back(*dd);
        }
      
    }
    // PRINT THE STATISTICS 
    cout<<"\n";
    cout<<"\t"; cout<<"\t"; cout<<"FIFO"; cout<<"\n";
    int total = 0; 
    int totBlocks = 0;
    bool keepTrackOfSpaces=false;
    for(int n = 1; n <=Proccesstable; n++) {
        
        if(symtab.symbols[n].timeWhenTerminated>10){
            keepTrackOfSpaces=true; 
        }
        
        if(symtab.symbols[n].state!="ABORTED"){
        total+=symtab.symbols[n].timeWhenTerminated;
        totBlocks+=symtab.symbols[n].numOfBlocks;
        }
        
        if(symtab.symbols[n].state=="ABORTED"){
        cout<<"\t";    cout<< " Task "; cout<< n; cout<<"     "; cout<<symtab.symbols[n].state; cout<<"\n";
            
        }
        else {
            float x = 0;
            x = (float)(((float)symtab.symbols[n].numOfBlocks/symtab.symbols[n].timeWhenTerminated) * 100);
        cout<<"\t";    cout<<" Task "; cout<< n;  cout<<"   "; cout<<symtab.symbols[n].timeWhenTerminated; cout<<"   ";cout<<symtab.symbols[n].numOfBlocks; cout<<"    "; cout<< x; cout<<"%"; cout<< "\n";
        }
        
    }
    
    float y = 0;
    y = (float)(((float)totBlocks/total) * 100);
  //Keep Track of Spaces 
   cout<<"\t ";
    cout<<"Total";
    
    if(total<10)
        cout<<"    ";
    else
        cout<<"    ";
   
    cout<<total;
    if(keepTrackOfSpaces)
        cout<<" ";
    if(total>=10)
    cout<<"  ";
    else
    cout<<"   "; // current change 
    cout<<totBlocks;
    
    if(totBlocks<10)
        cout<<"    ";
    else
        cout<<"   ";
    
    cout<< y;
    cout<<"%";
    
    
}


void SymbolTable:: doUpdatesChangesVERBOSE(SymbolTable symtab,vector<int>RESOURCES,int Proccesstable)  {
    
    symtab.systemCheck=0;
    symtab.terminatedProc=0;
    list<string>::iterator gg;
    list<unsigned>::iterator cc;
    list<unsigned>::iterator BB;
    vector <int> BLOCKEDQUEUE;    // FIFO QUEUE OF PROCESSES
    vector<int>::iterator dd;
    vector<int>::iterator POLAR;
    vector<int>AVAIL;
    list<unsigned> TMPSTACK; // Get the Entire set of Instructions sequentially for each task
    list<unsigned> TMPLIST;  // Get the First Three Free Digits of the Instructions
    string         TMPCMD;    // Get Temporary Command
    int            DigitForPrintout; // PID
    int            CurrentResource;  // RESOURCE ID
    symtab.totalCycleTime=0; // START THE CLOCK AT ZERO
    int             REQUEST;
    int             Count = 0;
    symtab.outOFDeadlock=true;
    int             max = Proccesstable;
    
    
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        
        
        for(int i = 1; i <=Proccesstable; i++) {
            
            // Currently Each Task has Zero Resources during Initiation
            symtab.symbols[i].REQUESTED.push_back(0);
        }
        
    }
    
    
    // Initially All Resources are Available before program counter starts Decoding Instructions
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        AVAIL.push_back(*dd);
    }
    
    // Set TRUE to LOCKS on Critical Regions
    for(int i = 1; i <=Proccesstable; i++) {
        symtab.symbols[i].TURNSTARON=true;
        symtab.symbols[i].MANAGERMADECONTACT=true;
        symtab.symbols[i].COMPTUM=false;
        symtab.symbols[i].numOfBlocks=0;
    }
    
    
    while ( symtab.systemCheck < Proccesstable  ){
       // cout<<"\n";
       // cout<< "During Cycle "; cout<<symtab.totalCycleTime; cout<<"-"; cout<<symtab.totalCycleTime + 1; cout<<":"; cout<<"\n"; ;
        symtab.totalCycleTime++;
        
        /* DEADLOCK If you detect the deadlock at cycle k, you abort the task(s) at cycle k and hence its/their resources
         become available at cycle k+1.**/
        
        if(BLOCKEDQUEUE.size()==Proccesstable-symtab.terminatedProc){
            
            /* System Is in Deadlock and will Continue to Be Until someone's request has been granted */
            symtab.outOFDeadlock=false;
            list<unsigned>getTask;
            int minimum = 0;
            
            // FIND LOWERED NUMBERED TASK
            
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                getTask.push_back(*dd);
            }
            getTask.sort();
            minimum=getTask.front();
            getTask.clear(); // free up memory
            
            // DELETE POSITION FROM BLOCKEDQUEUE
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                if (*dd==minimum) {
                    BLOCKEDQUEUE.erase(dd); // The container is modified.
                    //The elements removed are modified. Concurrently accessing or modifying other elements is safe, although iterating ranges that include the removed elements is not.
                }
            }
            
            // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
            for(int i = 0; i < RESOURCES.size(); i++){
                RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                AVAIL[i]= AVAIL[i] + symtab.symbols[minimum].REQUESTED[i];
            }
            
            symtab.symbols[minimum].REQUESTED.clear();
            
            // UPDATE THE STATE
            symtab.symbols[minimum].state="ABORTED";
            symtab.systemCheck++;
            
            //PRINT THE STATE
           // cout <<"\t";cout<<"Task "; cout<< minimum; cout<<" was "; cout<< symtab.symbols[minimum].state; cout<<"\n";
            
            // Now that the Resources are available and the lowest task has been aborted
            // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                
                // Now Clear TMPLIST
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                
                // Check for Blocked Tasks and see if The Requests can be granted
                if (REQUEST <= RESOURCES[CurrentResource-1]) {
                    symtab.outOFDeadlock=true;
                    
                    
                }
                
                
            }
            /* STILL IN DEADLOCK */
            if(symtab.outOFDeadlock==false){
                list<unsigned>getPosition;
                // FIND LOWERED NUMBERED TASK
                
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                    
                    getTask.push_back(*dd);
                    
                }
                getTask.sort();
                minimum=getTask.front();
                getTask.clear();
                
                
                
                
                // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
                for(int i = 0; i < RESOURCES.size(); i++){
                    RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                    AVAIL[i]= AVAIL[i] + symtab.symbols[minimum].REQUESTED[i];
                }
                
                symtab.symbols[minimum].REQUESTED.clear();
                symtab.symbols[minimum].state="ABORTED";
                symtab.systemCheck++;
              //  cout <<"\t";cout<<"Task "; cout<< minimum; cout<<" was "; cout<< symtab.symbols[minimum].state; cout<<"\n";
                
                // Now that the Resources are available and the lowest task has been aborted
                // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                    
                    for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                        
                        // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                        TMPSTACK.push_back(*cc);
                    }
                    
                    // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPSTACK.clear(); /*CLEAR*/
                    // GET FIRST COMMAND ON THE LIST
                    TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                    
                    
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // Check for Blocked Tasks and see if The Requests can be granted
                    if (REQUEST <= RESOURCES[CurrentResource-1]) {
                        symtab.outOFDeadlock=true; // Update Lock Variables
                        
                        
                    }
                    
                    
                    
                }
            }
            /* AGAIN STILL IN DEADLOCK */
            if (symtab.outOFDeadlock==false) {
                
                // FIND LOWERED NUMBERED TASK
                
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                    
                    getTask.push_back(*dd);
                }
                getTask.sort();// SORT AND TAKE FRONT ELEMENT
                minimum=getTask.front();
                getTask.clear();
                // DELETE POSITION FROM BLOCKEDQUEUE
                
                
                // REPLENISH THE RESOURCES THAT LOWEST NUMBERED TASK JUST LET GO OFF
                for(int i = 0; i < RESOURCES.size(); i++){
                    RESOURCES[i]= RESOURCES[i] + symtab.symbols[minimum].REQUESTED[i];
                    
                }
                
                symtab.symbols[minimum].REQUESTED.clear();
                symtab.symbols[minimum].state="ABORTED";
                symtab.systemCheck++;
              //  cout<<"Task "; cout<< minimum; cout<<" has been "; cout<<symtab.symbols[minimum].state; cout<<"\n";
                
                // Now that the Resources are available and the lowest task has been aborted
                // Repeat up TO THREE MORE TIMES ASSUMING A MAX LIMIT OF 4 TASKS
                for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                    
                    // Make sure to not iterate through a task that is aborted
                    if(symtab.symbols[*dd].state!="ABORTED"){
                        
                        for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                            
                            // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                            TMPSTACK.push_back(*cc);
                        }
                        // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPLIST.push_back(TMPSTACK.front());
                        TMPSTACK.pop_front();
                        TMPSTACK.clear(); /*CLEAR*/
                        // GET FIRST COMMAND ON THE LIST
                        TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                        
                        
                        DigitForPrintout=TMPLIST.front(); /* Get the TASK ID */
                        TMPLIST.pop_front();
                        CurrentResource=TMPLIST.front();  /* Current Resource ID */
                        TMPLIST.pop_front();
                        REQUEST=TMPLIST.front();          /* Amount of Resource */
                        TMPLIST.pop_front();
                        
                        // Check for Blocked Tasks and see if The Requests can be granted
                        if (REQUEST <= RESOURCES[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                            // System is no longer in a deadlock
                            symtab.outOFDeadlock=true;
                            
                            
                        }
                        
                        
                    }
                }
            }
            
            
        }
        
        if(BLOCKEDQUEUE.size()>1){
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                // Aborted Tasks stay on the blockedQueue so Check that during iteration that an Aborted Task is not updated and looked at
                if(symtab.symbols[*dd].state!="ABORTED"){
                    for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                        
                        // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                        TMPSTACK.push_back(*cc);
                    }
                    // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPSTACK.clear(); /*CLEAR*/
                    // GET FIRST COMMAND ON THE LIST
                    TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                    
                    /*Extract Task Info and pass it to MANAGER*/
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // Check for Blocked Tasks and see if The Requests can be granted
                    if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                        symtab.symbols[*dd].COMMANDS.pop_front();
                        int counter = 0;
                        while ( counter < REQUEST){
                            RESOURCES[CurrentResource-1]--;
                            AVAIL[CurrentResource-1]--;
                            counter++;
                            // KEEP TRACK SEQUENTIALLY OF REQUESTED[0] .... THE PROCESSES TAKES
                            symtab.symbols[*dd].REQUESTED[CurrentResource-1]++;
                        }
                        symtab.symbols[*dd].state="Requested and was Granted ";
                        symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front();
                     //   cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[*dd].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                        symtab.symbols[*dd].MANAGERMADECONTACT=true; symtab.symbols[*dd].TURNSTARON=false;
                        BLOCKEDQUEUE.erase(dd);
                        break; // Break From Iterating through the Container to AVOID RACE CONDITION
                    }
                    
                    
                }
            }
            
        }
        
        if(BLOCKEDQUEUE.size()==1){
            
            int i = BLOCKEDQUEUE[0];
            // Again skip over Aborted Tasks
            if (symtab.symbols[i].state!="ABORTED") {
                
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                
                // Check for Blocked Tasks and see if The Requests can be granted
                if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                    symtab.symbols[i].COMMANDS.pop_front();
                    int counter = 0;
                    while ( counter < REQUEST){
                        RESOURCES[CurrentResource-1]--;
                        AVAIL[CurrentResource-1]--;
                        counter++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]++; //
                    }
                    
                    // UPDATE STATE
                    symtab.symbols[i].state="Requested and was Granted ";
                    
                    // POP INSTRUCTIONS
                    symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                    
                    // Print the State
                   // cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                    symtab.symbols[i].MANAGERMADECONTACT=true; symtab.symbols[i].TURNSTARON=false;
                    BLOCKEDQUEUE.clear();
                    
                }
                
                
            }
            
            
        }
        
        
        
        
        
        /* Iterate Through ALL Task READING & DECODING THEIR INSTRUCTIONS */
        for(int i = 1; i <=Proccesstable; i++) {
            
            /*Increment Blocked Processes numOfBlocks*/
            if(symtab.symbols[i].state=="Blocked"){
                symtab.symbols[i].numOfBlocks++;
            }
            
            
            
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            /* MANGERMADECONTACT - TRUE IF IT WAS JUST GRANTED A RESOURCE WHILE BEING BLOCKED || IT IS NOT IN A BLOCKEDQUEUE */
            /* TURNSTARON- FALSE IF JUST GRANTED A RESOURCE WHILE BEING BLOCKED */
            
            
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            if((symtab.symbols[i].MANAGERMADECONTACT) && (symtab.symbols[i].TURNSTARON) && (symtab.symbols[i].state !=
                                                                                            "Terminated" ) && (symtab.symbols[i].state!="ABORTED")){
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                
                /*ANALYZE AND DECODE INSTRUCTIONS*/
                
                if ( TMPCMD == "compute") {
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    /*Initially the COMPTUM for every Task is FALSE*/
                    
                    if (symtab.symbols[i].COMPTUM==false) {
                        symtab.symbols[i].currentComputation=CurrentResource;
                        symtab.symbols[i].COMPTUM=true;
                    }
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    
                    symtab.symbols[i].state="Computing "; //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" FOR "; cout<<symtab.symbols[i].currentComputation;
                    //cout<<"\n";
                    
                    /* TASK IS ALREADY IN COMPUTATION STAGE*/
                    if (symtab.symbols[i].currentComputation>0) {
                        symtab.symbols[i].currentComputation--;
                    }
                    
                    /* Task finishes its Computation*/
                    if (symtab.symbols[i].currentComputation==0) {
                        symtab.symbols[i].COMPTUM=false;
                        symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                        symtab.symbols[i].COMMANDS.pop_front();
                    }
                    
                }
                /*ANALYZE THE COMMAND*/
                
                if ( TMPCMD == "initiate") {
                    // Basically This Data is Refuse or Trash Just Get It Out of the Optimistic Manager's  Way
                    symtab.symbols[i].COMMANDS.pop_front();
                    // GET PID OF THE TASK
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    symtab.symbols[i].state="Initialilized"; //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" Resource "; cout<<CurrentResource;
                    //cout<<"\n";
                }
                
                /*ANALYZE THE COMMAND*/
                if( TMPCMD == "request") {
                    
                    DigitForPrintout=TMPLIST.front(); // PID
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front(); // RESOURCE ID
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front(); // AMOUNT OF RESOURCE
                    TMPLIST.pop_front();
                    // AVAILABLE RESOURCES ARE CHECKED
                    if (REQUEST <= AVAIL[CurrentResource-1]) {
                        symtab.symbols[i].COMMANDS.pop_front();
                        int counter = 0;
                        while ( counter < REQUEST){
                            RESOURCES[CurrentResource-1]--; // DECREMENT THE RESOURCES
                            AVAIL[CurrentResource-1]--; // DECREMENT THE AVAILABLE RESOURCES
                            counter++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]++;
                        }
                        symtab.symbols[i].state="Requested and was Granted "; // GET STATE OF THE TASK
                        symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                       // cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                    }
                    
                    /*Update BLOCKEDQUEUE & state & numOfBlock */
                    else if (REQUEST > AVAIL[CurrentResource-1]) {
                        symtab.symbols[i].state="Blocked";
                        symtab.symbols[i].numOfBlocks++;
                        BLOCKEDQUEUE.push_back(i);
                        symtab.symbols[i].MANAGERMADECONTACT=false;
                       // cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                    }
                    
                }
                
                
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "release"){
                    
                    /* GET FIRST THREE DIGITS OF THE PROGRAM SEQUENCE*/
                    
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // POP THE COMMAND- THERE ARE EXACTLY 1/3 OF COMMANDS AS INSTRUCTIONS
                    
                    symtab.symbols[i].COMMANDS.pop_front();
                    int RESTOCK = 0;
                    
                    // UPDATE RESOURCES AND REQUESTED
                    
                    while (RESTOCK < REQUEST) {
                        RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                        RESTOCK++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                    }
                    
                    // UPDATE THE STATE
                    
                    symtab.symbols[i].state="released";
                    
                    // POP THE NEXT THREE DIGITS PERMANENTLY
                    
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // PRINT OUT STATE
                    
                    //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" ";  cout<<REQUEST; cout<<" UNITS OF "; cout<<" RESOURCE "; cout<<CurrentResource; cout<<"\n";
                    
                }
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "terminate"){
                    
                    /* GET FIRST THREE DIGITS */
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    /*DigitForPrintout ~ PID , CurrentResource ~ RESOURCE PID*/
                    
                    /*Delete Next COMMAND*/
                    symtab.symbols[i].COMMANDS.pop_front();
                    
                    
                    /*Update the State */
                    symtab.symbols[i].state="Terminated";
                    symtab.systemCheck++;
                    symtab.terminatedProc++;               /* DELETE NEXT THREE DIGITS FROM INSTRUCTIONS*/
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // note time when the Task Terminates
                    symtab.symbols[i].timeWhenTerminated=symtab.totalCycleTime-1;
                    
                    // Print the State
                    //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" at "; cout<<symtab.totalCycleTime-1;  cout<<" "; cout<<"\n";
                    
                    
                    
                    
                    
                    
                    
                    
                    
                }
                
                
                
            }
            // Force Iteration of Task that was Just knocked off the BlockedQueue to be read come around next time
            
            if(symtab.symbols[i].TURNSTARON==false){
                
                symtab.symbols[i].TURNSTARON=true;
            }
            
            
            
        }
        
        
        
        
        
        AVAIL.clear(); // Clear This memory has it NEEDS TO UPDATED AFTER EVERY CLOCK CYCLE
        
        /*SINCE A RESOURCE IS ONLY AVAILABLE AFTER A CYCLE UPDATE AVAIL HERE*/
        for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
            AVAIL.push_back(*dd);
        }
        
    }
    
    cout<<"\n";
    cout<<"\t"; cout<<"\t"; cout<<"FIFO"; cout<<"\n";
    int total = 0;
    int totBlocks = 0;
    bool keepTrackOfSpaces=false;
    for(int n = 1; n <=Proccesstable; n++) {
        
        if(symtab.symbols[n].timeWhenTerminated>10){
            keepTrackOfSpaces=true;
        }
        
        if(symtab.symbols[n].state!="ABORTED"){
            total+=symtab.symbols[n].timeWhenTerminated;
            totBlocks+=symtab.symbols[n].numOfBlocks;
        }
        
        if(symtab.symbols[n].state=="ABORTED"){
            cout<<"\t";    cout<< " Task "; cout<< n; cout<<"     "; cout<<symtab.symbols[n].state; cout<<"\n";
            
        }
        else {
            float x = 0;
            x = (float)(((float)symtab.symbols[n].numOfBlocks/symtab.symbols[n].timeWhenTerminated) * 100);
            cout<<"\t";    cout<<" Task "; cout<< n;  cout<<"   "; cout<<symtab.symbols[n].timeWhenTerminated; cout<<"   ";cout<<symtab.symbols[n].numOfBlocks; cout<<"    "; cout<< x; cout<<"%"; cout<< "\n";
        }
        
    }
    
    float y = 0;
    y = (float)(((float)totBlocks/total) * 100);
    /*Keep Track of Spaces */
    cout<<"\t ";
    cout<<"Total";
    
    if(total<10)
        cout<<"    ";
    else
        cout<<"    ";
    
    cout<<total;
    if(keepTrackOfSpaces)
        cout<<" ";
    if(total>=10)
        cout<<"  ";
    else
        cout<<"   "; // current change 
    cout<<totBlocks;
    
    if(totBlocks<10)
        cout<<"    ";
    else
        cout<<"   ";
    
    cout<< y;
    cout<<"%";
    
    
} 

void SymbolTable:: BANKER(SymbolTable symtab, vector<int> RESOURCES, int Proccesstable){
    
    symtab.systemCheck=0;
    symtab.terminatedProc=0;
    list<string>::iterator gg;
    list<unsigned>::iterator cc;
    list<unsigned>::iterator BB;
    vector <int> BLOCKEDQUEUE;    // FIFO QUEUE OF PROCESSES
    vector<int>::iterator dd;
    vector<int>::iterator POLAR;
    vector<int>AVAIL;
    vector<int>AVAILCOPY; 
    list<unsigned> TMPSTACK; // Get the Entire set of Instructions sequentially for each task
    list<unsigned> TMPLIST;  // Get the First Three Free Digits of the Instructions
    string         TMPCMD;    // Get Temporary Command
    int            DigitForPrintout; // PID
    int            CurrentResource;  // RESOURCE ID
    int            claims; // Banker's Claims 
    symtab.totalCycleTime=0; // START THE CLOCK AT ZERO
    int             REQUEST;
    int             Count = 0;
    symtab.outOFDeadlock=true;
    int             max = Proccesstable;
    
    
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        
        
        for(int i = 1; i <=Proccesstable; i++) {
            
            // Currently Each Task has Zero Resources during Initiation
            symtab.symbols[i].REQUESTED.push_back(0);
        }
        
    }
    
    
    // Initially All Resources are Available before program counter starts Decoding Instructions
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        AVAIL.push_back(*dd);
    }
    
    // Set TRUE to LOCKS on Critical Regions
    for(int i = 1; i <=Proccesstable; i++) {
        symtab.symbols[i].TURNSTARON=true;
        symtab.symbols[i].MANAGERMADECONTACT=true;
        symtab.symbols[i].COMPTUM=false;
        symtab.symbols[i].numOfBlocks=0;
    }
    
    
    while ( symtab.systemCheck < Proccesstable  ){
        cout<<"\n";
        cout<< "During Cycle "; cout<<symtab.totalCycleTime; cout<<"-"; cout<<symtab.totalCycleTime + 1; cout<<":"; cout<<"\n"; ;
        symtab.totalCycleTime++;
        
        /* DEADLOCK If you detect the deadlock at cycle k, you abort the task(s) at cycle k and hence its/their resources
         become available at cycle k+1.**/
        
    
        
        if(BLOCKEDQUEUE.size()>1){
            
            /* Reset the Marked Status of Every Task*/ 
            for(int i = 1; i <=Proccesstable; i++) {
                symtab.symbols[i].marked=false;
               
            }
            
            
            
            
            
            
            
            
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                // Aborted Tasks stay on the blockedQueue so Check that during iteration that an Aborted Task is not updated and looked at
                if(symtab.symbols[*dd].state!="ABORTED"){
                    for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                        
                        // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                        TMPSTACK.push_back(*cc);
                    }
                    // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPSTACK.clear(); /*CLEAR*/
                    // GET FIRST COMMAND ON THE LIST
                    TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                    
                    /*Extract Task Info and pass it to MANAGER*/
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // Check for Blocked Tasks and see if The Requests can be granted
                    if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                       // symtab.symbols[*dd].COMMANDS.pop_front();
                        
                        AVAILCOPY.clear(); 
                        // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                        for (int j = 0; j < AVAIL.size(); j++) {
                            AVAILCOPY.push_back(AVAIL[j]);
                        }
                        int counter = 0;
                        while ( counter < REQUEST){
                            //RESOURCES[CurrentResource-1]--;
                            AVAILCOPY[CurrentResource-1]--;
                            counter++;
                            // KEEP TRACK SEQUENTIALLY OF REQUESTED[0] .... THE PROCESSES TAKES
                            symtab.symbols[*dd].REQUESTED[CurrentResource-1]++;
                            //cout<<symtab.symbols[*dd].REQUESTED[CurrentResource-1];
                            //symtab.symbols[*dd].RESOURCESNEEDED[CurrentResource-1]--;
                        }
                        int markedProc2 = 0;
                        int iter2 = Proccesstable;
                        // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                        while (iter2 > 0) {
                            bool requestIsSafe2 = true;
                      
                        for (int k = 1; k<=Proccesstable; k++) {
                            if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED"){
                            /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to A*/
                           
                                
                                if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                    requestIsSafe2=true;
                                }
                                else
                                    requestIsSafe2=false;
                         
                            // Add the Resources that the Task has had assuming it will be able to terminate
                            if (requestIsSafe2) {
                                markedProc2++;
                                symtab.symbols[k].marked=true; 
                                
                                // Replenish the Resources
                                for( int d = 0; d < AVAILCOPY.size(); d++){
                                    AVAILCOPY[d]+= symtab.symbols[k].REQUESTED[d];
                                }
                            }
                            }
                        }
                            iter2--; 
                        }
                        
                        if (markedProc2==Proccesstable-symtab.systemCheck) {
                            symtab.symbols[*dd].COMMANDS.pop_front();
                            symtab.symbols[*dd].state="Requested and was Granted ";
                            symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front();
                            cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[*dd].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                            symtab.symbols[*dd].MANAGERMADECONTACT=true; symtab.symbols[*dd].TURNSTARON=false;
                            BLOCKEDQUEUE.erase(dd);
                            
                            // Clear Available to make it available copy
                            RESOURCES[CurrentResource-1]=AVAIL[CurrentResource-1]-REQUEST;
                            AVAIL[CurrentResource-1]= AVAIL[CurrentResource-1]-REQUEST;
                            
                            break; // Break From Iterating through the Container to AVOID RACE CONDITION
                            
                            
                            
                            
                        }
                        
                        if(markedProc2!=Proccesstable-symtab.systemCheck){
                            int b = 0;
                            while ( b < REQUEST){
                                //RESOURCES[CurrentResource-1]--;
                                
                                b++;
                                symtab.symbols[*dd].REQUESTED[CurrentResource-1]--; //
                                //symtab.symbols[*dd].RESOURCESNEEDED[CurrentResource-1]++;
                            }
                        }
                        
                        
                        
                        
                        
                        
                        
                        
                    }
                    
                    
                }
            }
            
        }
        
        if(BLOCKEDQUEUE.size()==1){
            for(int i = 1; i <=Proccesstable; i++) {
                symtab.symbols[i].marked=false;
              
            }
            int i = BLOCKEDQUEUE[0];
            // Again skip over Aborted Tasks
            if (symtab.symbols[i].state!="ABORTED") {
                
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                
                // Check for Blocked Tasks and see if The Requests can be granted
                if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                    int counter = 0;
                    AVAILCOPY.clear(); 
                    // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                    for (int j = 0; j < AVAIL.size(); j++) {
                        AVAILCOPY.push_back(AVAIL[j]);
                    }
                    
                    
                    while ( counter < REQUEST){
                        //RESOURCES[CurrentResource-1]--;
                        AVAILCOPY[CurrentResource-1]--;
                        counter++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]++; //
                        //symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--;
                    }
                 
                    int markedProc3 = 0;
                    int iter =  Proccesstable;
                    
                    /* ESSENTIALLY THIS MY RUDIMENTARY IMPLEMENTATION OF The Banker's Algorithm for Multiple Resources For Granting A REQUEST 
                    
                    
                                    1.) Grant the Request and Temporarily flush the memory in AVALCOPY, AVAIL STORES ALL THE MODIFIED STATES
                        
                                    2.) Copy Memory from AVAIL INTO AVAILCOPY
                     
                                    3.) DEDUCT FROM symtab.symbols[i].RESOURCESNEEDED[CurrentResouce-1] by the REQUESTED AMOUNT
                    
                                    4.) Also Deduct from AVAILCOPY THE Amount Requested.
                    
                                    5.) Now see if there are any Tasks including the one just making the REQUEST that all the elements of the vector RESOURCEDNEEDED ARE LESS than AVAILCOPY
                     
                                    6.) This is done (# of tasks) times. It goes through the tasks sequentially but it does not matter because each run through can only add to the Resources
                                    
                                    7.) A task is marked through if it is available to be satisfied otherwise if it is denied acesses and marked as FALSE. If the number of marked Processees is equal to the (# of Tasks) - ( symtab.systemCheck) 
                                        then the request is granted
                    
                    */
                    // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                    while (iter > 0) {
                        bool requestIsSafe3 = true;
                    for (int k = 1; k<=Proccesstable; k++) {
                        if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED" && symtab.symbols[k].state!="Terminated" ){
                        /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to A*/
                       
                            if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                requestIsSafe3=true;
                            }
                            else
                                requestIsSafe3=false;
                   
                        // Add the Resources that the Task has had assuming it will be able to terminate
                        if (requestIsSafe3) {
                            markedProc3++;
                            symtab.symbols[k].marked=true; 
                            // Replenish the Resources
                            for( int d = 0; d < AVAILCOPY.size(); d++){
                                AVAILCOPY[d]+=  symtab.symbols[k].REQUESTED[d];
                            }
                        }
                        
                    }
                    
                    }
                        iter--; 
                    }
                    // State is Safe so Grant the Requests because all Tasks have been marked meaning that a Deadlock can not happen 
                    if (markedProc3==Proccesstable-symtab.systemCheck) {
                        AVAILCOPY.clear();
                        
                        // UPDATE STATE
                        symtab.symbols[i].state="Requested and was Granted ";
                        symtab.symbols[i].COMMANDS.pop_front();
                       
                        // POP INSTRUCTIONS
                        symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                        AVAIL[CurrentResource-1]= AVAIL[CurrentResource-1]-REQUEST;
                        RESOURCES[CurrentResource-1]=RESOURCES[CurrentResource-1]-REQUEST; 
                        
                        // Print the State
                        cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                        symtab.symbols[i].MANAGERMADECONTACT=true; symtab.symbols[i].TURNSTARON=false;
                        BLOCKEDQUEUE.clear();
                        
                    }
                    //State is still Unsafe 
                    if(markedProc3!=Proccesstable-symtab.systemCheck){
                        int b = 0;  
                        while ( b < REQUEST){
                            //RESOURCES[CurrentResource-1]--;
                            
                            b++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]--; //
                            //symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]++;
                        }
                    }
                    
                    
                }
                    
               
                
            }
            
            
        }
        // Initially all Tasks are Marked False before Undergoing the Banker Algorithm 
      
        
        
        /* Iterate Through ALL Task READING & DECODING THEIR INSTRUCTIONS */
        for(int i = 1; i <=Proccesstable; i++) {
            
            /*Increment Blocked Processes numOfBlocks*/
            if(symtab.symbols[i].state=="Blocked"){
                symtab.symbols[i].numOfBlocks++;
            }
            
           
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            /* MANGERMADECONTACT - TRUE IF IT WAS JUST GRANTED A RESOURCE WHILE BEING BLOCKED || IT IS NOT IN A BLOCKEDQUEUE */
            /* TURNSTARON- FALSE IF JUST GRANTED A RESOURCE WHILE BEING BLOCKED */
            
            
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            if((symtab.symbols[i].MANAGERMADECONTACT) && (symtab.symbols[i].TURNSTARON) && (symtab.symbols[i].state !=
                                                                                            "Terminated" ) && (symtab.symbols[i].state!="ABORTED")){
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // FETCH INSTRUCTIONS
                    TMPSTACK.push_back(*cc);
                }
                
                // CONSIDER ONLY THE FIRST THREE INSTRUCTIONS ORDERED SEQUENTIALLY AS THEY COME IN THE PROGRAM AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                
                /*ANALYZE AND DECODE INSTRUCTIONS*/
                
                if ( TMPCMD == "compute") {
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    /*Initially the COMPTUM for every Task is FALSE*/
                    
                    if (symtab.symbols[i].COMPTUM==false) {
                        symtab.symbols[i].currentComputation=CurrentResource;
                        symtab.symbols[i].COMPTUM=true;
                    }
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    
                    symtab.symbols[i].state="Computing "; cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" FOR "; cout<<symtab.symbols[i].currentComputation;
                    cout<<"\n";
                    
                    /* TASK IS ALREADY IN COMPUTATION STAGE*/
                    if (symtab.symbols[i].currentComputation>0) {
                        symtab.symbols[i].currentComputation--;
                    }
                    
                    /* Task finishes its Computation*/
                    if (symtab.symbols[i].currentComputation==0) {
                        symtab.symbols[i].COMPTUM=false;
                        symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                        symtab.symbols[i].COMMANDS.pop_front();
                    }
                    
                }
                /*ANALYZE THE COMMAND*/
                
                if ( TMPCMD == "initiate") {
                    // Unlike the Optimistic Manager this data is more important See that the Initial Claims do not exceed the TOTAL RESOURCES 
                    symtab.symbols[i].COMMANDS.pop_front();
                    // GET PID OF THE TASK
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    claims=TMPLIST.front();
                    
                    symtab.symbols[i].claims.push_back(claims);
                    symtab.symbols[i].RESOURCESNEEDED.push_back(claims);
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    /*Check that the Initial Claims of a particular Resource do not exceed the total Resource Amount*/
                    if (claims > RESOURCES[CurrentResource-1]) {
                        symtab.symbols[i].state="ABORTED"; cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state;cout<<"\n";
                        symtab.systemCheck++;
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]=0; 
                    }
                    /* If not Initialize the Resource Count */ 
                    else{
                    symtab.symbols[i].state="Initialilized"; cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" Resource "; cout<<CurrentResource;
                    cout<<"\n";
                    }
                }
                
                /*ANALYZE THE COMMAND*/
                if( TMPCMD == "request") {
                    
                    DigitForPrintout=TMPLIST.front(); // PID
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front(); // RESOURCE ID
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front(); // AMOUNT OF RESOURCE
                    TMPLIST.pop_front();
                    
                    int totalAmount = 0;
                    totalAmount = symtab.symbols[i].REQUESTED[CurrentResource-1] + REQUEST;
                    // RESET THIS VALUE BECAUSE IT HAS ALREADY ACQUIRED THE CLAIMS AMOUNT BUT NOW AFTER RELEASING IT STILL WANTS MORE 
                    if(symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]<= 0){
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]= symtab.symbols[i].claims[CurrentResource-1];
                    }
                    
                    // See if the Request is more than the initial Claims if it is abort 
                    if(totalAmount > symtab.symbols[i].claims[CurrentResource-1]) {
                        symtab.symbols[i].state="ABORTED";
                        cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                        symtab.systemCheck++;
                        
                        int bmon = 0;
                        while (bmon <= symtab.symbols[i].REQUESTED[CurrentResource-1]) {
                            RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                            
                            bmon++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                            //cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                        }
                        
                        
                        
                    }
                    
                    for(int b = 1; b <=Proccesstable; b++) {
                        symtab.symbols[b].marked=false;
                        
                    }
                    
                    
                    // AVAILABLE RESOURCES ARE CHECKED
                    if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                        //symtab.symbols[i].COMMANDS.pop_front();
                        AVAILCOPY.clear(); 
                     
                        // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                        for (int j = 0; j < AVAIL.size(); j++) {
                            AVAILCOPY.push_back(AVAIL[j]);
                        }
                        int counter = 0;
                        while ( counter < REQUEST){
                          
                            AVAILCOPY[CurrentResource-1]--;
                            counter++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]++; // Update The Potential # of Resources that a task might have
                            symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--; // Subtract this from the RESOURCESNEEDED
                           // cout<<symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1];
                           // cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                        }
                       
                        int markedProc = 0; 
                        int iter4 = Proccesstable;
                        // Basically For How Many Tasks there are Iterate through each task (# of tasks ) times. So if one task later in the sequence finishes it adds its resources to the collection and repeat 
                        // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                        while (iter4 > 0) {
                            bool requestIsSafe = true;
                      
                        for (int k = 1; k<=Proccesstable; k++) {
                            if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED"){
                            /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to Available resources*/ 
                           
                                
                                if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                    requestIsSafe=true;
                                }
                                else
                                    requestIsSafe=false;
                            
                            // Add the Resources that the Task has had assuming it will be able to terminate 
                            if (requestIsSafe) {
                                markedProc++;
                                symtab.symbols[k].marked=true; 
                                // Replenish the Resources 
                                for( int d = 0; d < AVAILCOPY.size(); d++){
                                    AVAILCOPY[d]+= symtab.symbols[k].REQUESTED[d];
                                }
                            }
                            
                        }
                        }
                              iter4--;
                        }
                        // Lets make an Exception Here to make this code more flexable lets say that if this is the only task not blocked then we can safely satisfy this request
                        // This is safe because the request is less than the amount available therefore there should be no reason why we cant grant the request 
                        bool only1TaskNotBlocked = false;
                        
                        if(Proccesstable-(BLOCKEDQUEUE.size() + symtab.systemCheck)==1)
                            only1TaskNotBlocked=true; 
                        if (markedProc==Proccesstable-symtab.systemCheck || only1TaskNotBlocked) {
                            
                                // Clear Available to make it available copy
                            AVAIL[CurrentResource-1] = AVAIL[CurrentResource-1]-REQUEST;
                            
                                // Update the Status
                                symtab.symbols[i].state="Requested and was Granted "; // GET STATE OF THE TASK
                                symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                                cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                           symtab.symbols[i].COMMANDS.pop_front();
                                // Update the Resources as well
                                int fer = 0;
                                while ( fer < REQUEST){
                                
                                    RESOURCES[CurrentResource-1]--; 
                                    fer++;
                               
                              
                                }
                        }
                        // State is Unsafe 
                        if (markedProc!=Proccesstable-symtab.systemCheck && only1TaskNotBlocked!=true) {
                            
                            
                            // Block the Request
                            symtab.symbols[i].state="Blocked";
                            symtab.symbols[i].numOfBlocks++;
                            BLOCKEDQUEUE.push_back(i);
                            symtab.symbols[i].MANAGERMADECONTACT=false;
                            cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                            // FIX UP THE REQUESTED VECTOR
                            int z = 0;
                            while ( z < REQUEST){
                                z++;
                                symtab.symbols[i].REQUESTED[CurrentResource-1]--; // Basically Undo the Action done to internal Paramters because request was blocked
                               // symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]++; // Same idea
                            }
                            // clear the AVAILCOPY VECTOR 
                            AVAILCOPY.clear(); 
                            
                            
                            
                        }
                        
                        
                    }
                        
                     else if (REQUEST > AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                         int totalAmount = 0;
                         totalAmount = symtab.symbols[i].REQUESTED[CurrentResource-1] + REQUEST;                         // Block the Request
                         
                         // See if the Request is more than the initial Claims if it is abort
                         if(totalAmount > symtab.symbols[i].claims[CurrentResource-1]) {
                             symtab.symbols[i].state="ABORTED";
                             cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                             symtab.systemCheck++;
                             int bmon = 0; 
                             while (bmon < symtab.symbols[i].REQUESTED[CurrentResource-1]) {
                                 RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                                 
                                 bmon++;
                                 symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                                 //cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                             }
                         
                         
                         
                         
                         
                         }
                         
                       
                         
                         
                         
                         else {
                         
                         
                         
                         symtab.symbols[i].state="Blocked";
                         symtab.symbols[i].numOfBlocks++;
                         BLOCKEDQUEUE.push_back(i);
                         symtab.symbols[i].MANAGERMADECONTACT=false;
                         cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                         // FIX UP THE REQUESTED VECTOR
                         int z = 0;
                         while ( z < REQUEST){
                             z++;
                           // Basically Undo the Action done to internal Paramters because request was blocked
                             symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--; // Same idea chan
                         }
                         // clear the AVAILCOPY VECTOR
                         AVAILCOPY.clear();
                         
                         }
                         
                         
                      
                     }
                    
                }
                
                
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "release"){
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // POP THE COMMAND- THERE ARE EXACTLY 1/3 OF COMMANDS AS INSTRUCTIONS
                    
                    symtab.symbols[i].COMMANDS.pop_front();
                    int RESTOCK = 0;
                    
                    // UPDATE RESOURCES AND REQUESTED
                    
                    
                    
                    // IF THIS IS NOT THE LAST COMMAND BEFORE TERMINATE THEN LETS UPDATE THE RESOURCEDNEEDED VECTOR
                    if (symtab.symbols[i].COMMANDS.size()>2 ) {
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]+=REQUEST;
                        //cout<<" After Re:"; cout<< symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1];
                    }
                    
                    while (RESTOCK < REQUEST) {
                        RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                        
                        RESTOCK++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                    }
                    
                    // UPDATE THE STATE
                    
                    symtab.symbols[i].state="released";
                    
                    // POP THE NEXT THREE DIGITS PERMANENTLY
                    
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // PRINT OUT STATE
                    
                    cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" ";  cout<<REQUEST; cout<<" UNITS OF "; cout<<" RESOURCE "; cout<<CurrentResource; cout<<"\n";
                    
                }
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "terminate"){
                    
                    /* GET FIRST THREE DIGITS */
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    /*DigitForPrintout ~ PID , CurrentResource ~ RESOURCE PID*/
                    
                    /*Delete Next COMMAND*/
                    symtab.symbols[i].COMMANDS.pop_front();
                    
                    
                    /*Update the State */
                    symtab.symbols[i].state="Terminated";
                    symtab.systemCheck++;
                    symtab.terminatedProc++;               /* DELETE NEXT THREE DIGITS FROM INSTRUCTIONS*/
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // note time when the Task Terminates
                    symtab.symbols[i].timeWhenTerminated=symtab.totalCycleTime-1;
                    
                    // Print the State
                    cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" at "; cout<<symtab.totalCycleTime-1;  cout<<" "; cout<<"\n";
                }
                
                
                
            }
            // Force Iteration of Task that was Just knocked off the BlockedQueue to be read come around next time
            
            if(symtab.symbols[i].TURNSTARON==false){
                
                symtab.symbols[i].TURNSTARON=true;
            }
            
            
            
        }
        
        
        
        
        
        AVAIL.clear(); // Clear This memory as it NEEDS TO UPDATED AFTER EVERY CLOCK CYCLE
        
        /*SINCE A RESOURCE IS ONLY AVAILABLE AFTER A CYCLE UPDATE AVAIL HERE*/
        for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
            AVAIL.push_back(*dd);
        }
        
    }
    /* PRINT THE STATISTICS */
    cout<<"\n";
    cout<<"\t"; cout<<"\t"; cout<<"BANKER"; cout<<"\n";
    int total = 0;
    int totBlocks = 0;
    bool keepTrackOfSpaces=false;
    for(int n = 1; n <=Proccesstable; n++) {
        
        if(symtab.symbols[n].timeWhenTerminated>10){
            keepTrackOfSpaces=true;
        }
        
        if(symtab.symbols[n].state!="ABORTED"){
            total+=symtab.symbols[n].timeWhenTerminated;
            totBlocks+=symtab.symbols[n].numOfBlocks;
        }
        
        if(symtab.symbols[n].state=="ABORTED"){
            cout<<"\t";    cout<< " Task "; cout<< n; cout<<"     "; cout<<symtab.symbols[n].state; cout<<"\n";
            
        }
        else {
            float x = 0;
            x = (float)(((float)symtab.symbols[n].numOfBlocks/symtab.symbols[n].timeWhenTerminated) * 100);
            cout<<"\t";    cout<<" Task "; cout<< n;  cout<<"   "; cout<<symtab.symbols[n].timeWhenTerminated;
            if(symtab.symbols[n].timeWhenTerminated >=10)
                cout<<"  ";
            else
                cout<<"   "; 
            cout<<symtab.symbols[n].numOfBlocks;
            cout<<"    ";
            cout<< x;
            cout<<"%";
            cout<< "\n";
        }
        
    }
    
    float y = 0;
    y = (float)(((float)totBlocks/total) * 100);
    /*Keep Track of Spaces */
    cout<<"\t ";
    cout<<"Total";
    
    if(total<10)
        cout<<"    ";
    else
        cout<<"    ";
    
    cout<<total;

        cout<<" ";
    if(total>=10)
        cout<<" ";
    else
        cout<<"   "; // current change 
    cout<<totBlocks;
    
    if(totBlocks<10)
        cout<<"    ";
    else
        cout<<"   ";
    
    cout<< y;
    cout<<"%";
    
    
}


void SymbolTable:: BANKERVERBOSE(SymbolTable symtab, vector<int> RESOURCES, int Proccesstable){
    
    symtab.systemCheck=0;
    symtab.terminatedProc=0;
    list<string>::iterator gg;
    list<unsigned>::iterator cc;
    list<unsigned>::iterator BB;
    vector <int> BLOCKEDQUEUE;    // FIFO QUEUE OF PROCESSES
    vector<int>::iterator dd;
    vector<int>::iterator POLAR;
    vector<int>AVAIL;
    vector<int>AVAILCOPY;
    list<unsigned> TMPSTACK; // Get the Entire set of Instructions sequentially for each task
    list<unsigned> TMPLIST;  // Get the First Three Free Digits of the Instructions
    string         TMPCMD;    // Get Temporary Command
    int            DigitForPrintout; // PID
    int            CurrentResource;  // RESOURCE ID
    int            claims; // Banker's Claims
    symtab.totalCycleTime=0; // START THE CLOCK AT ZERO
    int             REQUEST;
    int             Count = 0;
    symtab.outOFDeadlock=true;
    int             max = Proccesstable;
    
    
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        
        
        for(int i = 1; i <=Proccesstable; i++) {
            
            // Currently Each Task has Zero Resources during Initiation
            symtab.symbols[i].REQUESTED.push_back(0);
        }
        
    }
    
    
    // Initially All Resources are Available before program counter starts Decoding Instructions
    for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
        AVAIL.push_back(*dd);
    }
    
    // Set TRUE to LOCKS on Critical Regions
    for(int i = 1; i <=Proccesstable; i++) {
        symtab.symbols[i].TURNSTARON=true;
        symtab.symbols[i].MANAGERMADECONTACT=true;
        symtab.symbols[i].COMPTUM=false;
        symtab.symbols[i].numOfBlocks=0;
    }
    
    
    while ( symtab.systemCheck < Proccesstable  ){
       // cout<<"\n";
       // cout<< "During Cycle "; cout<<symtab.totalCycleTime; cout<<"-"; cout<<symtab.totalCycleTime + 1; cout<<":"; cout<<"\n"; ;
        symtab.totalCycleTime++;
        
        /* DEADLOCK If you detect the deadlock at cycle k, you abort the task(s) at cycle k and hence its/their resources
         become available at cycle k+1.**/
        
        
        
        if(BLOCKEDQUEUE.size()>1){
            
            /* Reset the Marked Status of Every Task*/
            for(int i = 1; i <=Proccesstable; i++) {
                symtab.symbols[i].marked=false;
                
            }
            
            
            
            
            
            
            
            
            for(dd = BLOCKEDQUEUE.begin(); dd!=BLOCKEDQUEUE.end(); ++dd){
                
                // Aborted Tasks stay on the blockedQueue so Check that during iteration that an Aborted Task is not updated and looked at
                if(symtab.symbols[*dd].state!="ABORTED"){
                    for (cc = symtab.symbols[*dd].Instructions.begin(); cc != symtab.symbols[*dd].Instructions.end(); ++cc) {
                        
                        // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                        TMPSTACK.push_back(*cc);
                    }
                    // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPLIST.push_back(TMPSTACK.front());
                    TMPSTACK.pop_front();
                    TMPSTACK.clear(); /*CLEAR*/
                    // GET FIRST COMMAND ON THE LIST
                    TMPCMD = symtab.symbols[*dd].COMMANDS.front();
                    
                    /*Extract Task Info and pass it to MANAGER*/
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // Check for Blocked Tasks and see if The Requests can be granted
                    if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[*dd].state!="ABORTED") {
                        // symtab.symbols[*dd].COMMANDS.pop_front();
                        
                        AVAILCOPY.clear();
                        // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                        for (int j = 0; j < AVAIL.size(); j++) {
                            AVAILCOPY.push_back(AVAIL[j]);
                        }
                        int counter = 0;
                        while ( counter < REQUEST){
                            //RESOURCES[CurrentResource-1]--;
                            AVAILCOPY[CurrentResource-1]--;
                            counter++;
                            // KEEP TRACK SEQUENTIALLY OF REQUESTED[0] .... THE PROCESSES TAKES
                            symtab.symbols[*dd].REQUESTED[CurrentResource-1]++;
                            //symtab.symbols[*dd].RESOURCESNEEDED[CurrentResource-1]--;
                        }
                        int markedProc2 = 0;
                        int iter2 = Proccesstable;
                        // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                        while (iter2 > 0) {
                            bool requestIsSafe2 = true;
                            
                            for (int k = 1; k<=Proccesstable; k++) {
                                if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED"){
                                    /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to A*/
                                    
                                    
                                    if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                        requestIsSafe2=true;
                                    }
                                    else
                                        requestIsSafe2=false;
                                    
                                    // Add the Resources that the Task has had assuming it will be able to terminate
                                    if (requestIsSafe2) {
                                        markedProc2++;
                                        symtab.symbols[k].marked=true;
                                        
                                        // Replenish the Resources
                                        for( int d = 0; d < AVAILCOPY.size(); d++){
                                            AVAILCOPY[d]+= symtab.symbols[k].REQUESTED[d];
                                        }
                                    }
                                }
                            }
                            iter2--;
                        }
                        
                        if (markedProc2==Proccesstable-symtab.systemCheck) {
                            symtab.symbols[*dd].COMMANDS.pop_front();
                            symtab.symbols[*dd].state="Requested and was Granted ";
                            symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front(); symtab.symbols[*dd].Instructions.pop_front();
                         //   cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[*dd].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                            symtab.symbols[*dd].MANAGERMADECONTACT=true; symtab.symbols[*dd].TURNSTARON=false;
                            BLOCKEDQUEUE.erase(dd);
                            
                            // Clear Available to make it available copy
                            RESOURCES[CurrentResource-1]=AVAIL[CurrentResource-1]-REQUEST;
                            AVAIL[CurrentResource-1]= AVAIL[CurrentResource-1]-REQUEST;
                            
                            break; // Break From Iterating through the Container to AVOID RACE CONDITION
                            
                            
                            
                            
                        }
                        
                        if(markedProc2!=Proccesstable-symtab.systemCheck){
                            int b = 0;
                            while ( b < REQUEST){
                                //RESOURCES[CurrentResource-1]--;
                                
                                b++;
                                symtab.symbols[*dd].REQUESTED[CurrentResource-1]--; //
                                //symtab.symbols[*dd].RESOURCESNEEDED[CurrentResource-1]++;
                            }
                        }
                        
                        
                        
                        
                        
                        
                        
                        
                    }
                    
                    
                }
            }
            
        }
        
        if(BLOCKEDQUEUE.size()==1){
            for(int i = 1; i <=Proccesstable; i++) {
                symtab.symbols[i].marked=false;
                
            }
            int i = BLOCKEDQUEUE[0];
            // Again skip over Aborted Tasks
            if (symtab.symbols[i].state!="ABORTED") {
                
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // TAKE IN ALL THE REMAINING INSTRUCTIONS FOR EACH TASK
                    TMPSTACK.push_back(*cc);
                }
                // CONSIDER ONLY THE FIRST THREE AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                DigitForPrintout=TMPLIST.front();
                TMPLIST.pop_front();
                CurrentResource=TMPLIST.front();
                TMPLIST.pop_front();
                REQUEST=TMPLIST.front();
                TMPLIST.pop_front();
                
                // Check for Blocked Tasks and see if The Requests can be granted
                if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                    int counter = 0;
                    AVAILCOPY.clear();
                    // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                    for (int j = 0; j < AVAIL.size(); j++) {
                        AVAILCOPY.push_back(AVAIL[j]);
                    }
                    
                    
                    while ( counter < REQUEST){
                        //RESOURCES[CurrentResource-1]--;
                        AVAILCOPY[CurrentResource-1]--;
                        counter++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]++; //
                        //symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--;
                    }
                    
                    int markedProc3 = 0;
                    int iter =  Proccesstable;
                    
                    /* ESSENTIALLY THIS MY RUDIMENTARY IMPLEMENTATION OF The Banker's Algorithm for Multiple Resources For Granting A REQUEST
                     
                     
                     1.) Grant the Request and Temporarily flush the memory in AVALCOPY, AVAIL STORES ALL THE MODIFIED STATES
                     
                     2.) Copy Memory from AVAIL INTO AVAILCOPY
                     
                     3.) DEDUCT FROM symtab.symbols[i].RESOURCESNEEDED[CurrentResouce-1] by the REQUESTED AMOUNT
                     
                     4.) Also Deduct from AVAILCOPY THE Amount Requested.
                     
                     5.) Now see if there are any Tasks including the one just making the REQUEST that all the elements of the vector RESOURCEDNEEDED ARE LESS than AVAILCOPY
                     
                     6.) This is done (# of tasks) times. It goes through the tasks sequentially but it does not matter because each run through can only add to the Resources
                     
                     7.) A task is marked through if it is available to be satisfied otherwise if it is denied acesses and marked as FALSE. If the number of marked Processees is equal to the (# of Tasks) - ( symtab.systemCheck)
                     then the request is granted
                     
                     */
                    // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                    while (iter > 0) {
                        bool requestIsSafe3 = true;
                        for (int k = 1; k<=Proccesstable; k++) {
                            if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED" && symtab.symbols[k].state!="Terminated" ){
                                /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to A*/
                                
                                if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                    requestIsSafe3=true;
                                }
                                else
                                    requestIsSafe3=false;
                                
                                // Add the Resources that the Task has had assuming it will be able to terminate
                                if (requestIsSafe3) {
                                    markedProc3++;
                                    symtab.symbols[k].marked=true;
                                    // Replenish the Resources
                                    for( int d = 0; d < AVAILCOPY.size(); d++){
                                        AVAILCOPY[d]+=  symtab.symbols[k].REQUESTED[d];
                                    }
                                }
                                
                            }
                            
                        }
                        iter--;
                    }
                    // State is Safe so Grant the Requests because all Tasks have been marked meaning that a Deadlock can not happen
                    if (markedProc3==Proccesstable-symtab.systemCheck) {
                        AVAILCOPY.clear();
                        
                        // UPDATE STATE
                        symtab.symbols[i].state="Requested and was Granted ";
                        symtab.symbols[i].COMMANDS.pop_front();
                        
                        // POP INSTRUCTIONS
                        symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                        AVAIL[CurrentResource-1]= AVAIL[CurrentResource-1]-REQUEST;
                        RESOURCES[CurrentResource-1]=RESOURCES[CurrentResource-1]-REQUEST;
                        
                        // Print the State
                      //  cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                        symtab.symbols[i].MANAGERMADECONTACT=true; symtab.symbols[i].TURNSTARON=false;
                        BLOCKEDQUEUE.clear();
                        
                    }
                    //State is still Unsafe
                    if(markedProc3!=Proccesstable-symtab.systemCheck){
                        int b = 0;
                        while ( b < REQUEST){
                            //RESOURCES[CurrentResource-1]--;
                            
                            b++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]--; //
                            //symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]++;
                        }
                    }
                    
                    
                }
                
                
                
            }
            
            
        }
        // Initially all Tasks are Marked False before Undergoing the Banker Algorithm
        
        
        
        /* Iterate Through ALL Task READING & DECODING THEIR INSTRUCTIONS */
        for(int i = 1; i <=Proccesstable; i++) {
            
            /*Increment Blocked Processes numOfBlocks*/
            if(symtab.symbols[i].state=="Blocked"){
                symtab.symbols[i].numOfBlocks++;
            }
            
            
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            /* MANGERMADECONTACT - TRUE IF IT WAS JUST GRANTED A RESOURCE WHILE BEING BLOCKED || IT IS NOT IN A BLOCKEDQUEUE */
            /* TURNSTARON- FALSE IF JUST GRANTED A RESOURCE WHILE BEING BLOCKED */
            
            
            
            /* Make Sure that the Task in Question has not just been checked, is blocked, or is terminated*/
            
            if((symtab.symbols[i].MANAGERMADECONTACT) && (symtab.symbols[i].TURNSTARON) && (symtab.symbols[i].state !=
                                                                                            "Terminated" ) && (symtab.symbols[i].state!="ABORTED")){
                
                for (cc = symtab.symbols[i].Instructions.begin(); cc != symtab.symbols[i].Instructions.end(); ++cc) {
                    
                    // FETCH INSTRUCTIONS
                    TMPSTACK.push_back(*cc);
                }
                
                // CONSIDER ONLY THE FIRST THREE INSTRUCTIONS ORDERED SEQUENTIALLY AS THEY COME IN THE PROGRAM AND THEN CLEAR TMPSTACK
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPLIST.push_back(TMPSTACK.front());
                TMPSTACK.pop_front();
                TMPSTACK.clear(); /*CLEAR*/
                
                // GET FIRST COMMAND ON THE LIST
                TMPCMD = symtab.symbols[i].COMMANDS.front();
                
                
                
                /*ANALYZE AND DECODE INSTRUCTIONS*/
                
                if ( TMPCMD == "compute") {
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    /*Initially the COMPTUM for every Task is FALSE*/
                    
                    if (symtab.symbols[i].COMPTUM==false) {
                        symtab.symbols[i].currentComputation=CurrentResource;
                        symtab.symbols[i].COMPTUM=true;
                    }
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    
                    symtab.symbols[i].state="Computing "; //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" FOR "; cout<<symtab.symbols[i].currentComputation;
                    //cout<<"\n";
                    
                    /* TASK IS ALREADY IN COMPUTATION STAGE*/
                    if (symtab.symbols[i].currentComputation>0) {
                        symtab.symbols[i].currentComputation--;
                    }
                    
                    /* Task finishes its Computation*/
                    if (symtab.symbols[i].currentComputation==0) {
                        symtab.symbols[i].COMPTUM=false;
                        symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                        symtab.symbols[i].COMMANDS.pop_front();
                    }
                    
                }
                /*ANALYZE THE COMMAND*/
                
                if ( TMPCMD == "initiate") {
                    // Unlike the Optimistic Manager this data is more important See that the Initial Claims do not exceed the TOTAL RESOURCES
                    symtab.symbols[i].COMMANDS.pop_front();
                    // GET PID OF THE TASK
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    claims=TMPLIST.front();
                    
                    symtab.symbols[i].claims.push_back(claims);
                    symtab.symbols[i].RESOURCESNEEDED.push_back(claims);
                    TMPLIST.clear(); // Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    /*Check that the Initial Claims of a particular Resource do not exceed the total Resource Amount*/
                    if (claims > RESOURCES[CurrentResource-1]) {
                        symtab.symbols[i].state="ABORTED"; //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state;cout<<"\n";
                        symtab.systemCheck++;
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]=0;
                    }
                    /* If not Initialize the Resource Count */
                    else{
                        symtab.symbols[i].state="Initialilized"; //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<" Resource "; cout<<CurrentResource;
                        //cout<<"\n";
                    }
                }
                
                /*ANALYZE THE COMMAND*/
                if( TMPCMD == "request") {
                    
                    DigitForPrintout=TMPLIST.front(); // PID
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front(); // RESOURCE ID
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front(); // AMOUNT OF RESOURCE
                    TMPLIST.pop_front();
                    
                    int totalAmount = 0;
                    totalAmount = symtab.symbols[i].REQUESTED[CurrentResource-1] + REQUEST;
                    // RESET THIS VALUE BECAUSE IT HAS ALREADY ACQUIRED THE CLAIMS AMOUNT BUT NOW AFTER RELEASING IT STILL WANTS MORE
                    if(symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]<= 0){
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]= symtab.symbols[i].claims[CurrentResource-1];
                    }
                    
                    // See if the Request is more than the initial Claims if it is abort
                    if(totalAmount > symtab.symbols[i].claims[CurrentResource-1]) {
                        symtab.symbols[i].state="ABORTED";
                       // cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                        symtab.systemCheck++;
                        
                        int bmon = 0;
                        while (bmon <= symtab.symbols[i].REQUESTED[CurrentResource-1]) {
                            RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                            
                            bmon++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                            //cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                        }
                        
                        
                        
                    }
                    
                    for(int b = 1; b <=Proccesstable; b++) {
                        symtab.symbols[b].marked=false;
                        
                    }
                    
                    
                    // AVAILABLE RESOURCES ARE CHECKED
                    if (REQUEST <= AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                        //symtab.symbols[i].COMMANDS.pop_front();
                        AVAILCOPY.clear();
                        
                        // Create AVAILCOPY FROM AVAIL BY SETTING ALL THE ELEMENTS OF AVAIL TO AVAILCOPY
                        for (int j = 0; j < AVAIL.size(); j++) {
                            AVAILCOPY.push_back(AVAIL[j]);
                        }
                        int counter = 0;
                        while ( counter < REQUEST){
                            
                            AVAILCOPY[CurrentResource-1]--;
                            counter++;
                            symtab.symbols[i].REQUESTED[CurrentResource-1]++; // Update The Potential # of Resources that a task might have
                            symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--; // Subtract this from the RESOURCESNEEDED
                            //cout<<symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1];
                            //cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                        }
                        
                        int markedProc = 0;
                        int iter4 = Proccesstable;
                        // Basically For How Many Tasks there are Iterate through each task (# of tasks ) times. So if one task later in the sequence finishes it adds its resources to the collection and repeat
                        // Iterate through each List of each task  and see if it is able to satisfy its needs if so mark it and add it and see if all the tasks are marked only then is the Request Granted
                        while (iter4 > 0) {
                            bool requestIsSafe = true;
                            
                            for (int k = 1; k<=Proccesstable; k++) {
                                if(symtab.symbols[k].marked!=true && symtab.symbols[k].state!="ABORTED"){
                                    /* Go through Tasks Order Doesnt Matter and see whose unmet resource needs are all smaller than or equal to Available resources*/
                                    
                                    
                                    if (symtab.symbols[k].RESOURCESNEEDED[CurrentResource-1] <= AVAILCOPY[CurrentResource-1]) {
                                        requestIsSafe=true;
                                    }
                                    else
                                        requestIsSafe=false;
                                    
                                    // Add the Resources that the Task has had assuming it will be able to terminate
                                    if (requestIsSafe) {
                                        markedProc++;
                                        symtab.symbols[k].marked=true;
                                        // Replenish the Resources
                                        for( int d = 0; d < AVAILCOPY.size(); d++){
                                            AVAILCOPY[d]+= symtab.symbols[k].REQUESTED[d];
                                        }
                                    }
                                    
                                }
                            }
                            iter4--;
                        }
                        // Lets make an Exception Here to make this code more flexable lets say that if this is the only task not blocked then we can safely satisfy this request
                        // This is safe because the request is less than the amount available therefore there should be no reason why we cant grant the request
                        bool only1TaskNotBlocked = false;
                        
                        if(Proccesstable-(BLOCKEDQUEUE.size() + symtab.systemCheck)==1)
                            only1TaskNotBlocked=true;
                        if (markedProc==Proccesstable-symtab.systemCheck || only1TaskNotBlocked) {
                            
                            // Clear Available to make it available copy
                            AVAIL[CurrentResource-1] = AVAIL[CurrentResource-1]-REQUEST;
                            
                            // Update the Status
                            symtab.symbols[i].state="Requested and was Granted "; // GET STATE OF THE TASK
                            symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front(); symtab.symbols[i].Instructions.pop_front();
                            //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<REQUEST; cout<<" Unit "; cout<<" of "; cout<<" Resource "; cout<<CurrentResource; cout<<"\n";
                            symtab.symbols[i].COMMANDS.pop_front();
                            // Update the Resources as well
                            int fer = 0;
                            while ( fer < REQUEST){
                                
                                RESOURCES[CurrentResource-1]--;
                                fer++;
                                
                                
                            }
                        }
                        // State is Unsafe
                        if (markedProc!=Proccesstable-symtab.systemCheck && only1TaskNotBlocked!=true) {
                            
                            
                            // Block the Request
                            symtab.symbols[i].state="Blocked";
                            symtab.symbols[i].numOfBlocks++;
                            BLOCKEDQUEUE.push_back(i);
                            symtab.symbols[i].MANAGERMADECONTACT=false;
                            //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                            // FIX UP THE REQUESTED VECTOR
                            int z = 0;
                            while ( z < REQUEST){
                                z++;
                                symtab.symbols[i].REQUESTED[CurrentResource-1]--; // Basically Undo the Action done to internal Paramters because request was blocked
                                // symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]++; // Same idea
                            }
                            // clear the AVAILCOPY VECTOR
                            AVAILCOPY.clear();
                            
                            
                            
                        }
                        
                        
                    }
                    
                    else if (REQUEST > AVAIL[CurrentResource-1] && symtab.symbols[i].state!="ABORTED") {
                        int totalAmount = 0;
                        totalAmount = symtab.symbols[i].REQUESTED[CurrentResource-1] + REQUEST;                         // Block the Request
                        
                        // See if the Request is more than the initial Claims if it is abort
                        if(totalAmount > symtab.symbols[i].claims[CurrentResource-1]) {
                            symtab.symbols[i].state="ABORTED";
                            //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                            symtab.systemCheck++;
                            int bmon = 0;
                            while (bmon < symtab.symbols[i].REQUESTED[CurrentResource-1]) {
                                RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                                
                                bmon++;
                                symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                               // cout<<symtab.symbols[i].REQUESTED[CurrentResource-1];
                            }
                            
                            
                            
                            
                            
                        }
                        
                        
                        
                        
                        
                        else {
                            
                            
                            
                            symtab.symbols[i].state="Blocked";
                            symtab.symbols[i].numOfBlocks++;
                            BLOCKEDQUEUE.push_back(i);
                            symtab.symbols[i].MANAGERMADECONTACT=false;
                           // cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout; cout<<" "; cout<<symtab.symbols[i].state; cout<<"\n";
                            // FIX UP THE REQUESTED VECTOR
                            int z = 0;
                            while ( z < REQUEST){
                                z++;
                                // Basically Undo the Action done to internal Paramters because request was blocked
                                symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]--; // Same idea chan
                            }
                            // clear the AVAILCOPY VECTOR
                            AVAILCOPY.clear();
                            
                        }
                        
                        
                        
                    }
                    
                }
                
                
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "release"){
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    
                    // POP THE COMMAND- THERE ARE EXACTLY 1/3 OF COMMANDS AS INSTRUCTIONS
                    
                    symtab.symbols[i].COMMANDS.pop_front();
                    int RESTOCK = 0;
                    
                    // UPDATE RESOURCES AND REQUESTED
                    
                    
                    
                    // IF THIS IS NOT THE LAST COMMAND BEFORE TERMINATE THEN LETS UPDATE THE RESOURCEDNEEDED VECTOR
                    if (symtab.symbols[i].COMMANDS.size()>2 ) {
                        symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1]+=REQUEST;
                        //cout<<" After Re:"; cout<< symtab.symbols[i].RESOURCESNEEDED[CurrentResource-1];
                    }
                    
                    while (RESTOCK < REQUEST) {
                        RESOURCES[CurrentResource-1]++; // REPLENISH THE RESOURCES SO THAT WHEN MAIN ITERATION IS OVER YOU CAN UPDATE THE AVAIL VECTOR
                        
                        RESTOCK++;
                        symtab.symbols[i].REQUESTED[CurrentResource-1]--;
                    }
                    
                    // UPDATE THE STATE
                    
                    symtab.symbols[i].state="released";
                    
                    // POP THE NEXT THREE DIGITS PERMANENTLY
                    
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // PRINT OUT STATE
                    
                    //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" ";  cout<<REQUEST; cout<<" UNITS OF "; cout<<" RESOURCE "; cout<<CurrentResource; cout<<"\n";
                    
                }
                /*ANALYZE THE COMMAND*/
                if(TMPCMD == "terminate"){
                    
                    /* GET FIRST THREE DIGITS */
                    DigitForPrintout=TMPLIST.front();
                    TMPLIST.pop_front();
                    CurrentResource=TMPLIST.front();
                    TMPLIST.pop_front();
                    REQUEST=TMPLIST.front();
                    TMPLIST.pop_front();
                    /*DigitForPrintout ~ PID , CurrentResource ~ RESOURCE PID*/
                    
                    /*Delete Next COMMAND*/
                    symtab.symbols[i].COMMANDS.pop_front();
                    
                    
                    /*Update the State */
                    symtab.symbols[i].state="Terminated";
                    symtab.systemCheck++;
                    symtab.terminatedProc++;               /* DELETE NEXT THREE DIGITS FROM INSTRUCTIONS*/
                    symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();symtab.symbols[i].Instructions.pop_front();
                    
                    // note time when the Task Terminates
                    symtab.symbols[i].timeWhenTerminated=symtab.totalCycleTime-1;
                    
                    // Print the State
                    //cout<<"\t"; cout<<"Task "; cout<<DigitForPrintout;cout<<" "; cout<<symtab.symbols[i].state; cout<<" at "; cout<<symtab.totalCycleTime-1;  cout<<" "; cout<<"\n";
                }
                
                
                
            }
            // Force Iteration of Task that was Just knocked off the BlockedQueue to be read come around next time
            
            if(symtab.symbols[i].TURNSTARON==false){
                
                symtab.symbols[i].TURNSTARON=true;
            }
            
            
            
        }
        
        
        
        
        
        AVAIL.clear(); // Clear This memory as it NEEDS TO UPDATED AFTER EVERY CLOCK CYCLE
        
        /*SINCE A RESOURCE IS ONLY AVAILABLE AFTER A CYCLE UPDATE AVAIL HERE*/
        for (dd = RESOURCES.begin(); dd != RESOURCES.end(); ++dd) {
            AVAIL.push_back(*dd);
        }
        
        }
        /* PRINT THE STATISTICS */
        cout<<"\n";
        cout<<"\t"; cout<<"\t"; cout<<"BANKER"; cout<<"\n";
        int total = 0;
        int totBlocks = 0;
        bool keepTrackOfSpaces=false;
        for(int n = 1; n <=Proccesstable; n++) {
            
            if(symtab.symbols[n].timeWhenTerminated>10){
                keepTrackOfSpaces=true;
            }
            
            if(symtab.symbols[n].state!="ABORTED"){
                total+=symtab.symbols[n].timeWhenTerminated;
                totBlocks+=symtab.symbols[n].numOfBlocks;
            }
            
            if(symtab.symbols[n].state=="ABORTED"){
                cout<<"\t";    cout<< " Task "; cout<< n; cout<<"     "; cout<<symtab.symbols[n].state; cout<<"\n";
                
            }
            else {
                float x = 0;
                x = (float)(((float)symtab.symbols[n].numOfBlocks/symtab.symbols[n].timeWhenTerminated) * 100);
                cout<<"\t";    cout<<" Task "; cout<< n;  cout<<"   "; cout<<symtab.symbols[n].timeWhenTerminated;
                if(symtab.symbols[n].timeWhenTerminated >=10)
                    cout<<"  ";
                else
                    cout<<"   ";
                cout<<symtab.symbols[n].numOfBlocks;
                cout<<"    ";
                cout<< x;
                cout<<"%";
                cout<< "\n";
            }
            
        }
        
        float y = 0;
        y = (float)(((float)totBlocks/total) * 100);
        /*Keep Track of Spaces */
        cout<<"\t ";
        cout<<"Total";
        
        if(total<10)
        cout<<"    ";
        else
        cout<<"    ";
        
        cout<<total;
        
        cout<<" ";
        if(total>=10)
        cout<<" ";
        else
        cout<<"   "; // current change 
        cout<<totBlocks;
        
        if(totBlocks<10)
        cout<<"    ";
        else
        cout<<"   ";
        
        cout<< y;
        cout<<"%";
        
        
        }



