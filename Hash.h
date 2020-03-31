//Definitions used by Hash.cpp

#include <string>
#include <ctime>

class PTEntry { 
    private:
      int key;
      int pid;                  
      int RWbit;        
      PTEntry* next;

  public:
      PTEntry(int, int, int);
      void setKey(int);
      int getKey();             
      void setPID(int);                 
      int getPID();
      void set_RWbit(int);
	  int get_RWbit();
      void setNext(PTEntry*);
	  PTEntry* getNext();
      
};

class HashTable {
    private:
      PTEntry **HashArray;                
      int DiskWrites;
      int DiskReads;
      int pfaults;
      int totalFrames;                  
      int *frames;                      
      int fc;
      int kcounter;
      int hk;

    public:
      HashTable(int, int);                                                 
      int Search(int, int);                    
      void Insertion(int , int, int );
      void Insert(int, int, int);       
      int HashFunction(int );                                    
      int getDiskWrites();                                              
      int getDiskReads();                                             
      int getpfaults();
      int getcoveredframes();
      void FlushAll(); 
      ~HashTable();                                                       
};
