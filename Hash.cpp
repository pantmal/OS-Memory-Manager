#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <climits>
#include <cstddef>

#include "Hash.h"

const int TABLE_SIZE = 7; //wasn't sure what capacity the Hash Array needs so i put a random one :P

using namespace std;

//**********************Page Table Entry Class*************************//

PTEntry::PTEntry(int k, int p, int bit):key(k), pid(p), RWbit(bit){ //page table entry class is where the information from the memory manager is stored 
    																//it is a list which contains all the pages that have the same key
	this->next = NULL;
}

void PTEntry::setKey(int key){
	this->key = key;
}

int PTEntry::getKey(){
    return key;
}

void PTEntry::setPID(int pid){
    this->pid = pid;
}

int PTEntry::getPID(){
    return pid;
}

void PTEntry::set_RWbit(int bit){
    this->RWbit = bit;
}

int PTEntry::get_RWbit(){
    return RWbit;
}

void PTEntry::setNext(PTEntry* next){
    this->next = next;
}

PTEntry* PTEntry::getNext(){
    return next;
}

//**********************Hash Table Class*************************//

//**********************Constructor Function*************************//

HashTable::HashTable(int f, int k): totalFrames(f), hk(k) {
    HashArray = new PTEntry*[TABLE_SIZE]; //Hash Table class consists of an array that stores pointers at the head of each page table entry 

    int i;
    for (i = 0; i < TABLE_SIZE; i++){    //initializing the HashTable
        HashArray[i] = NULL;
	}

    frames = new int[totalFrames];   //initializing an array of frames
    for(i=0;i<totalFrames;i++){
      frames[i] = 0;
  	}


    this->DiskWrites = 0; //initializing the counters
    this->DiskReads = 0;
    this->fc=0;
    this->kcounter=0;
    this->pfaults=0;
}

//**********************Search Function*************************//

int HashTable::Search(int key, int pid) {  
    int hash = HashFunction(key);                               //finding the position of the key we need
    
    if(HashArray[hash] == NULL){                                //if the key does not exist
        return -1;												
    }
    else {														//if the key does exist, we have to check if the page is in the list  
        PTEntry *entry = HashArray[hash]; 
        while(entry!=NULL){                                     
            if((entry->getKey() == key) && (entry->getPID() == pid)){
                return entry->getPID(); //we found it!
            }
            else{
                entry = entry->getNext();
            }
        }

        if(entry == NULL){
            return -1; //we didn't find it...
        }
    }
    return -1;
}


//********************************Insert Functions********************************//

void HashTable::Insertion(int address, int pid, int RWbit) {   
	
	if(Search(address, pid) == -1){       //if the page is not inside the hash table

        this->pfaults++; //we have a page fault
		this->kcounter++;
		if(kcounter==hk+1){ //if k exceeded its threshold
			FlushAll();  //remove every page
		}                            
        Insert(address, pid, RWbit);  //insert this page
        
    }

}


void HashTable::Insert(int key, int pid, int RWbit) {    //inserting the page in the hash table

        int hash = HashFunction(key);                           //find the key's position inside the  Hash Table
    
        if(HashArray[hash] == NULL)                            //if this position is empty
            HashArray[hash] = new PTEntry(key,pid,RWbit);      //insert the first item
        else{												   //otherwise insert the page at the end of the list	
            PTEntry *entry = HashArray[hash];                  
            while(entry->getNext() != NULL){
                entry = entry->getNext();
            }
            entry->setNext(new PTEntry(key,pid,RWbit));
        }
        
        fc++;  //how many covered frames we have
        
        for(int i;i<fc;i++){ //if a frame is covered then the frames array simple recieves the 1 value in the right cell
        	frames[i]=1;
		}
        
}


int HashTable::HashFunction(int key) { //hash function used in order to find the key in the hash table 
	int hashVal = 0; 

    hashVal = key % TABLE_SIZE;     //mod with the size of the Hash Table

    return hashVal;
}


//******************** Flush Function ***********************//


void HashTable::FlushAll(){  //removing every page
	
	for(int i=0;i<TABLE_SIZE;i++){
		if(HashArray[i]==NULL){ //if this array cell has nothing check the next one
			continue;
		}
		else{                 //if there is at least one page here
			PTEntry *entry = HashArray[i]; 
			while(entry!=NULL){ //before we delete the page we have to store the r/w bit somewhere
				if(entry->get_RWbit() == 87){  //increment the disk write counter
					this->DiskWrites++;                            
				}
				if(entry->get_RWbit() == 82){ //increment the disk read counter
					this->DiskReads++;
				}
				PTEntry* temp=entry; //deleting the page
            	entry = entry->getNext();
            	delete temp;
            }
		}
	}
	
	for(int i=0;i<TABLE_SIZE;i++){ //set everything to null so the hash table will start anew
		HashArray[i]=NULL;
	}
	
	for(int i=0; i<totalFrames; i++){ //samething for the frames array
		frames[i]=0;
	}
	
	fc=0;
	
}

//******************** Getters ***********************//
int HashTable::getDiskWrites(){ //functions used in order to get the output 
    return this->DiskWrites;
}

int HashTable::getDiskReads(){
    return this->DiskReads;
}

int HashTable::getpfaults(){
    return this->pfaults;
}

int HashTable::getcoveredframes(){
	return this->fc;
}

//**********************Destructor Function*************************//

HashTable::~HashTable() { //deleting what isn't needed anymore                  
    int i;
    PTEntry *prev = NULL;
    for (i = 0; i < TABLE_SIZE; i++){
        if(HashArray[i]!=NULL){
            PTEntry *entry = HashArray[i];
            while(entry!=NULL){
                prev = entry;
                entry = entry->getNext();
                delete prev;
            }
        }
    }

    delete[] HashArray;

}
