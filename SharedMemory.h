//Header file for Shared Memory

#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <string>

struct Shared{ //The definition of the Part that will be used in the main program
    int procid;
    int address;
    int bit;
    
};


int SharedCreate(key_t); //Creating shared memory 

Shared *SharedAttach(int); //Getting a pointer to the shared memory segment

int SharedDetach(Shared*); //Detaching the shared segment

int SharedDelete(int); //Deleting the shared memory segment
