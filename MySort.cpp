#include <iostream>

//Do the phase 2
//We need only one thread anyway
//You may have multiple threads for reading 
//But you need only one for merging

//Merging k lists. Each list is a file
//Take min heap approach

//Constants
#define GB (1024*1024*1024)
#define MB (1024*1024)
#define KB (1024)

#define INTERNAL_SORTING 0
#define EXTERNAL_SORTING 1


class File{
    //This may not be the entire dataset
    char *data;
    int fd;
    public:
    File(int file_descriptor,int size){
        data = (char*)malloc(size*sizeof(char));
        fd = file_descriptor;  
    };
    int Open(){

    };
    int Close(){

    }
    int Read(){
        //May be use linux head command for this

    }
    int Write(){

    }
};

//Node that has to be used for MinHeap
class Node{

};

//Has function like getmin, add an element etc.,
class MinHeap{

};

// Perform quick sort algorithm here
// You can do merge sort as well because merge sort requires atmost 8gb space for 4gb file
// which fits in the memory
void QuickSort(char *arr, int size){
    //Perform quick sort on this arr

}

//See what all options you can have
class Options{
    int numThreads;
    public:
    Options(char* input){
        //Parse input and set the number of threads
        //Default value is 48 as of now
    } 
};

class Controller{
    int type;
    public:
    Controller(int size){
        //Type will be set according to this
    }
    int execute(){
        //Majority of the code goes here
        //has a switch case and performs the 
        //task based on the type
        //In case of inplace:
            // Load entire file into memory
            // Perform operation on that memory
        //In case of out of place:
            //Perform external sorting
    }
};

//Returns file size of a given file. This is a helper function
int fileSize(char *fileName){

}

int main(int argv,char *argc[]){
    //Just have options and controller objects together
    //here
}
