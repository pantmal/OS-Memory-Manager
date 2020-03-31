#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/timeb.h>


#include "Hash.h"
#include "Semaphores.h"
#include "SharedMemory.h"


using namespace std;

void rmbzip(){ //rmbizp and rmgcc are funtcions that remove the offsets from each file
	
    string hexValue; 
    unsigned int i;
    string bit;
    cout << "Removing offsets from bzip file... " << endl;
    ofstream file1("visual_addresses1.txt");  //these two functions will create new files without the offsets(visual_addresses1 and 2)
											  //and the main function will read from these two

    ifstream File1("bzip.trace");                 
    if(!File1){
        cout << "Cannot open file here\n" << endl;
        return ;
    }
    
    do{       //this loop removes the offset. specifically, it reads 5 chars as well as the r/w bit from the original file and writes them on to the new one 
        File1 >> hexValue >> bit;

        for(i=0; i < 5; i++){           
            if (file1.is_open()){
                file1 << hexValue[i];
            }
            else{
                cout << "Couldn't open" << endl;
            }
        }
        if (file1.is_open()){
            file1 << " " << bit << endl ;
        }
    }
    while(!File1.eof());

    file1.close();
    File1.close();
}

void rmgcc(){ //same thing for the gcc file
	
    string hexValue;
    unsigned int i;
    string bit;
    cout << "Removing offsets from gcc file..." << endl;
    ofstream file2("visual_addresses2.txt");

    ifstream File2("gcc.trace");                    
    if(!File2){
        cout << "Cannot open file here\n" << endl;
        return ;
    }
    do{
        File2 >> hexValue >> bit;

        for(i=0; i < 5; i++){           
            if (file2.is_open()){
                file2 << hexValue[i];
            }
            else{
                cout << "Couldn't open" << endl;
            }
        }
        if (file2.is_open()){
            file2 << " " << bit << endl ;
        }
    }
    while(!File2.eof());

    file2.close();
    File2.close();
}



int main(int argc, char* argv[]){
	
	string address, bit;  //declaring variables
	string address2, bit2;
    int q, max, frames, k;
    int frames1,frames2;
    int max1,max2;

	if(argc < 5){
		printf("Error of argc!");
	}
	else{
    	k = atoi(argv[1]);
    	frames = atoi(argv[2]);
    	q = atoi(argv[3]);
    	max = atoi(argv[4]);
    }
       
    frames1=frames/2; //since we have two hash tables (one for each process) frames will be divided by 2 so each hash table will fave the same amount
    frames2=frames/2; 
    max1=max/2; //max has to be split in two in order to have max amount of traces examined by both files
    max2=max/2;
    
    int n=3;   //3 processes
    int temp1=0; //temp variables used in loops
    int temp2=0;
    key_t key;	 //the key used in shared memory creation
    
	int ShmID;		//arguments for shared memory creation
    Shared *ShmPtr;
    
    int Full_ID, Empty_ID;	//the semaphores that will be used
    int Full1_ID, Empty1_ID;
    int Proc2_ID;
    int Print1_ID, Print2_ID;
    int Wait1_ID, Wait_ID;
    int Q1_ID, Q2_ID;
    
    key=1000+(rand()%9000);		//creating the shared memory
   	if (key == -1) {                
        printf(" ftok error! \n");
            exit(1);
    }

    ShmID = SharedCreate(key);                                 
    if (ShmID < 0) {
        printf("shmget error! \n");
        exit(1);
    }
    ShmPtr = SharedAttach(ShmID);
    if (ShmPtr == (Shared *)(-1)){
        printf("shmat error! \n");
        exit(1);
    }
    
    
	Empty_ID = SemCreate((key_t)123456,1,1);     //creating and initializing the semaphores                   
    if (Empty_ID < 0){							 
        printf("semget error! \n");
        exit(0);
    }
    
    Full_ID = SemCreate((key_t)45656,1,0);                        
    if (Full_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Empty1_ID = SemCreate((key_t)12734576,1,1);                     
    if (Empty1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Full1_ID = SemCreate((key_t)42596586,1,0);                        
    if (Full1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    
    Proc2_ID = SemCreate((key_t)789579,1,0);                        
    if (Proc2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Print1_ID = SemCreate((key_t)69956989,1,0);                        
    if (Print1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Print2_ID = SemCreate((key_t)55956999,1,0);                        
    if (Print2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Wait_ID = SemCreate((key_t)69956799,1,1);                        
    if (Wait_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Wait1_ID = SemCreate((key_t)956799,1,1);                        
    if (Wait1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Q1_ID = SemCreate((key_t)33956799,1,0);                        
    if (Q1_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    Q2_ID = SemCreate((key_t)446799,1,0);                        
    if (Q2_ID < 0){
        printf("semget error! \n");
        exit(0);
    }
    
    rmbzip(); //removing the offsets
    rmgcc();
    
   	ifstream File1("visual_addresses1.txt");                    
    if(!File1){
        cout << "Cannot open file1\n" << endl;;
        return 1;
    }

    ifstream File2("visual_addresses2.txt");                  
    if(!File2){
        cout << "Cannot open file2\n" << endl;;
        return 1;
    }
    
    HashTable Table1(frames1, k); //Creating the hash tables. The class is defined in the Hash.cpp and Hash.h files
    HashTable Table2(frames2, k);

	int first=1; //the use of the "first" variable will be explained later on
    
    int i;
    pid_t pid;
    for (i=0; i<n; i++){				//we will use a loop that will create 3 processes. each time it calls fork to create a process. 
       pid=fork();						
       if(pid == -1){
           printf("Error in fork() \n");
           return 1;
       }
       if(pid == 0){					//if we are on a child, then we will check the value of the counter. 
       	    if(i == 0){				//if it's 0 then we will do stuff for the first process. if it's 1 it will do stuff for the second process, and so on.
       	    	//PM1 Process
       	    	
				int procid=1; //process id will be passed in the Shared Memory so we can know if we're on the right process
				
				while(temp1<max1){ //if we have 50000 traces, each process will send 25000 
					
					int qc=0;
        			while(qc<q){
        				
            			File1 >> address >> bit;
            			if(File1.eof()){
                			break;
                		}
                	
            			SemDown(Empty_ID,0); //critical section begins here
            		
						int tmpbit=0;  //i had to convert the address and r/w bit from string to int types, otherwise they wouldn't pass on to the shared memory 
						int tmpaddress=0;
						if(bit=="R"){
					 		tmpbit=82;
						}
						if(bit=="W"){
					 		tmpbit=87;	
						}
						for(i=0; i<address.length(); i++){                
							tmpaddress = tmpaddress + address[i];
						}
					
            			ShmPtr->procid=procid;
            			ShmPtr->address=tmpaddress;
            			ShmPtr->bit=tmpbit;
					
						SemUp(Full_ID,0);
            			temp1++;
            			if(temp1 >= max1){  //if pm1 has passed its traces
                			break;  
                		}
                	
                		qc++;
        			}
        			SemDown(Q1_ID,0); //if we have sent q amount of traces, block yourself so the pm2 process will send its q traces. 
        			if(temp1>=max1){ //pm2 process will have to be unblocked by this process in its last loop
        				SemUp(Q2_ID,0);
					}
					
        		}
        		
    			SemUp(Print1_ID,0); //we may now print what we have to from hash table 1
    	
			
			}
    		else if(i==1){
    			//PM2 Process
    			//similar work here
    		
    			int procid=2;
    			
    			SemDown(Proc2_ID,0); //initially this process is blocked because in my simulation the pm1 has to begin first
				while(temp2<max2){
	
        			int qc=0;
        			while(qc<q){
        				
            			File2 >> address2 >> bit2;
            			if(File2.eof()){
                			break;
                		}
                	
            			SemDown(Empty1_ID,0);
            		
            			int tmpbit=0;
						int tmpaddress=0;
						if(bit2=="R"){
					 		tmpbit=82;
					 	}
						if(bit2=="W"){
					 		tmpbit=87;	
						}
						for(i=0; i<address2.length(); i++){                //calculate the int value of string
							tmpaddress = tmpaddress + address2[i];
						}
					            		
            			ShmPtr->procid=procid;
            			ShmPtr->address=tmpaddress;
            			ShmPtr->bit=tmpbit;
            		
            			SemUp(Full1_ID,0);
            			temp2++;
            			if(temp2 >= max2){
                			break;
                		}
                	
        			
        				qc++;
        			}
        			
        			SemDown(Q2_ID,0);		
        			
    			}
    			
    			SemUp(Print2_ID,0);
    		
			}
    		else if(i==2){
    			//Memory Manager(MM) Process 
    			
    			int temp31=0;
    			int temp32=0;
    			int qc1=0;
    			int qc=0;
    			
    			while(1){ //forcing the process to enter a loop
    				
    				if(temp31>=max1 || temp32>=max2){ //if either process finished its work we may leave the loop
    					break;
    				}
    			
    				if(temp31<max1){ //if we have more traces left
		
    					while(qc1<q){ //for q traces 
		
    						SemDown(Full_ID,0);
    						SemUp(Wait_ID,0);
			
							if(ShmPtr->procid==1){ //if this is the pm1 process
								int temproc=ShmPtr->procid; //read from the shared memory and pass the data on to the hash table
								int tempaddress=ShmPtr->address; 
								int tempbit=ShmPtr->bit;
					
    							Table1.Insertion(tempaddress,temproc,tempbit);
    				
    						}
							temp31++;
			
							SemUp(Empty_ID,0); 
        					SemDown(Wait_ID,0);
        		
        					qc1++;
        	
        				}
        				if(first==1){ //if this is the first q we sent unblock the second process.
        					SemUp(Proc2_ID,0); //this is done only once, because the pm1 process has to begin first
        					first=0;
        					qc1=0;
						}
						else{ //if it's not the first q, you may unblock the pm2 process so it can send again
							SemUp(Q2_ID,0);
							qc1=0;
						}
        		
					}
				
					if(temp32<max2){ //similar work for pm2 process
						
						while(qc<q){
					
    						SemDown(Full1_ID,0);
    						SemUp(Wait1_ID,0);
			
							if(ShmPtr->procid==2){
								int temproc=ShmPtr->procid;
								int tempaddress =ShmPtr->address; 
								int tempbit =ShmPtr->bit;
								
    							Table2.Insertion(tempaddress,temproc,tempbit);
    			
							}
							temp32++;
		
        					SemDown(Wait1_ID,0);
							SemUp(Empty1_ID,0);
        		
        					qc++;
        		
        				}
        				SemUp(Q1_ID,0); //pm2 has sent q traces, this means pm1 can be ublocked in order to send again
        				qc=0;
        			}
    
        		}
        	
        	
        		SemDown(Print1_ID,0); //making sure we"ll print the output at the right time
        		SemDown(Print2_ID,0);
        		
        		int totalDiskReads=Table1.getDiskReads() + Table2.getDiskReads(); //getting the information we need
        		int totalDiskWrites=Table1.getDiskWrites() + Table2.getDiskWrites();
        		int totalPageFaults=Table1.getpfaults() + Table2.getpfaults();
        		int totalCoveredFrames=Table1.getcoveredframes() + Table2.getcoveredframes();
        		
        		cout<<"Total Disk Reads are: "<<totalDiskReads<<endl; //printing what we need
				cout<<"Total Disk Writes are: "<<totalDiskWrites<<endl;
				cout<<"Total Page Faults are: "<<totalPageFaults<<endl;
				cout<<"Max number of traces examined by each file: "<<max<<endl;
				cout<<"Total Covered Frames are: "<<totalCoveredFrames<<endl;
				
				if(k>=1 && k<=9){ //creating 3 different output files for different values of k
					ofstream out("Output1.txt");
					out <<"Output for k between 1 and 9 (The other arguments are: frames=2048, q=10 and max=50000) is : "<< endl;
					out <<endl;
					out <<"Total Disk Reads are: "<<totalDiskReads<<endl;
					out <<"Total Disk Writes are: "<<totalDiskWrites<<endl;
					out <<"Total Page Faults are: "<<totalPageFaults<<endl;
					out <<"Max number of traces examined by each file: "<<max<<endl;
					out <<"Total Covered Frames are: "<<totalCoveredFrames<<endl;
				}
				if(k>=10 && k<=99){
					ofstream out("Output2.txt");
					out <<"Output for k between 10 and 99 (The other arguments are: frames=2048, q=10 and max=50000) is : "<< endl;
					out <<endl;
					out <<"Total Disk Reads are: "<<totalDiskReads<<endl;
					out <<"Total Disk Writes are: "<<totalDiskWrites<<endl;
					out <<"Total Page Faults are: "<<totalPageFaults<<endl;
					out <<"Max number of traces examined by each file: "<<max<<endl;
					out <<"Total Covered Frames are: "<<totalCoveredFrames<<endl;
				}
				if(k>=100){
					ofstream out("Output3.txt");
					out <<"Output for k over 100 (The other arguments are: frames=2048, q=10 and max=50000) is : "<< endl;
					out <<endl;
					out <<"Total Disk Reads are: "<<totalDiskReads<<endl;
					out <<"Total Disk Writes are: "<<totalDiskWrites<<endl;
					out <<"Total Page Faults are: "<<totalPageFaults<<endl;
					out <<"Max number of traces examined by each file: "<<max<<endl;
					out <<"Total Covered Frames are: "<<totalCoveredFrames<<endl;
				}
				
				
			}
			
			else{
        		printf("Something went REALLY wrong... \n"); //If for some reason a child was not created...
    		}
    	
    		exit(0);	//Child process exits
    	
        }
	}
           
    if (pid != 0) {	//All a father has to do is wait for his children to finish.
          
        for(i=0; i<3; i++){
          
	  		int status;
	  		pid_t child_pid;	
        	child_pid = wait(&status);
          
       }

    }
	
	
	SemDel(Full_ID);	//Deleting the semaphores
	SemDel(Empty_ID);
	SemDel(Full1_ID);	
	SemDel(Empty1_ID);
	SemDel(Proc2_ID);
	SemDel(Print1_ID);
	SemDel(Print2_ID);
	SemDel(Wait_ID);
	SemDel(Wait1_ID);
	SemDel(Q1_ID);
	SemDel(Q2_ID);
	
	
	if (SharedDetach(ShmPtr) == -1) {       //Detaching and deleting Shared Memories  	         
        cout<<" shmdt failed! " <<endl;
        exit(1);
    }
    if (SharedDelete(ShmID) == -1) {
        cout<<" shmctl(IPC_RMID) failed!" <<endl;
        exit(1);
    }
	
	
	
}
