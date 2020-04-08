#include <iostream>
//For string functions
#include <cstring>
//For files
#include <fstream>


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
#define MEMORY_SIZE (8*GB)

class File{
    //This data is an array of some K number of lines
    char **data;
    int fd;
    std::string fileName;
    int totalSize;
    public:
    File(std::string fileName,int total_size){
        fd = -1;
        //data = (char*)malloc(data_size*sizeof(char));
        totalSize = total_size;
    };
    int Open(){
        //Set what stream to choose
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
    //char *fileName;
    std::string fileName;
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
                    //fileName = (char*)malloc(sizeof(char)*sizeof(*argv[i+1]));
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
    std::string getFileName(){
        return fileName;
    }
};

class Controller{
    int type;
    File *inputFile;
    public:
    Controller(int file_size,std::string fileName){
        //Type will be set according to this
        if(file_size > MEMORY_SIZE){
            type = EXTERNAL_SORTING;
        }
        else{
            type = INTERNAL_SORTING;
        }
        inputFile = new File(fileName,file_size);
    }
    int execute(){
        switch (type)
        {
            case INTERNAL_SORTING:
                //Read entire data into File
                //Perform quicksort on it
                break;
            case EXTERNAL_SORTING:

                break;
            default:
                break;
        }
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
int fileSize(std::string fileName){
    //std::cout<<fileName<<std::endl;
    FILE *p_file = NULL;
    p_file = fopen(fileName.c_str(),"r");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
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

    int file_size = fileSize(opt.getFileName());
    
    Controller ctrlr = Controller(file_size,opt.getFileName());
    ctrlr.execute();
    
    //std::cout<<file_size<<std::endl;

    //std::cout<<opt.getNumThreads()<<std::endl;
    //std::cout<<opt.getFileName()<<std::endl;

    return 0;
}
