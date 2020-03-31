
#include "SharedMemory.h"
#include <string>

int SharedCreate(key_t key){

    if(key<0){
      std::cout<<"Error in key!"<<std::endl;
      return -1;
    }

    return shmget(key, sizeof(Shared), IPC_CREAT | 0666);
}

Shared* SharedAttach(int SharedID){
    return (Shared*)shmat(SharedID, 0, 0);
}

int SharedDetach(Shared* ShmPtr){
    return shmdt(ShmPtr);
}

int SharedDelete(int SharedID){
    return shmctl(SharedID, IPC_RMID, 0);
}
