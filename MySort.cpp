#include <iostream>
//For string functions
#include <cstring>

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

#define DEFAULT_SORT_THREADS 48

class File{
    //This may not be the entire dataset
    char *data;
    int fd;
    int *fileName;
    public:
    File(char* fileName,int size){
        fd = -1;
        data = (char*)malloc(size*sizeof(char));
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
    char *fileName;
    bool fileNameExists;
    bool threadFlagExists;
    public:
    Options(int argc, char* argv[]){
        threadFlagExists = false;
        //Parse input and set the number of threads
        //Default value is 48 as of now
        for (int i = 0; i < argc; i++)
        {
            if(i+1 != argc){
                if(strcmp(argv[i],"-t") == 0){
                    numThreads = std::stoi(argv[i+1]);
                    threadFlagExists = true;
                }
                else if(strcmp(argv[i],"-F") == 0){
                    fileName = (char*)malloc(sizeof(char)*sizeof(*argv[i+1]));
                    fileName = argv[i+1];
                    fileNameExists = true;
                }
            }
        }
        if(threadFlagExists == false){
            numThreads = DEFAULT_SORT_THREADS;
        }
    }
    int sanityCheck(){
        if(fileNameExists == false){
            return 0;
        }
        return 1;
    }
    int getNumThreads(){
        return numThreads;
    } 
    char* getFileName(){
        return fileName;
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

int main(int argc,char *argv[]){
    //Just have options and controller objects together
    //here
    Options opt = Options(argc,argv);
    //If sanity check fails, then all the expected inputs are not present
    if(!opt.sanityCheck()){
        std::cout<<"Incorrect Usage"<<std::endl;
        std::cout<<"Usage: ./ms.out -F [filename] -t [number of threads]"<<std::endl;
        exit(0);
    };



    //std::cout<<opt.getNumThreads()<<std::endl;
    //std::cout<<opt.getFileName()<<std::endl;

    return 0;
}
