#include <iostream>
//For string functions
#include <cstring>
#include <string>
//For files
#include <fstream>
#include "quicksort.h"

using namespace std;

//Do the phase 2
//We need only one thread anyway
//You may have multiple threads for reading 
//But you need only one for merging

//Merging k lists. Each list is a file
//Take min heap approach

//Constants
//#define 
#define MB (1024*1024)
#define KB (1024)
#define LINE_LENGTH 100

#define ll long long
#define INTERNAL_SORTING 0
#define EXTERNAL_SORTING 1
#define DEFAULT_SORT_THREADS 48

#define INPUT 0 //Reading from file
#define OUTPUT 1 //Writing to file

ll GB  = 1024*1024*1024;
ll MEMORY_SIZE  = 8*GB;


class File{
    //This data is an array of some K number of lines
    //Instead of **data lets have string array that reads n lines from the given file
    //Then sort this string array
    //char **data;
    string *data;
    ll numDataLines; //Number of lines that data has
    fstream fileHandle;    
    fstream myfile;
    std::string fileName;
    int openType;
    long long totalSize;
    ll totalNumLines;
    /*
        MAY REQUIRE CURRENT POSITION ll to append
     */
    public:
    File(std::string file_name,long long total_size,int open_type){
        //fd = -1;
        //data = (char*)malloc(data_size*sizeof(char));
        totalSize = total_size;
        fileName = file_name;
        totalNumLines = (ll)total_size/LINE_LENGTH;
        openType = open_type;
    }
    int Open(){
        if(openType == INPUT){
            fileHandle.open(fileName,ios::out | ios::in);
        }
        else{
            fileHandle.open(fileName,ios::out);
        }        
    }
    int Close(){
        fileHandle.close();
    }
    //Reads lines. Start index is 0
    //Controller will set how many lines to read
    int Read(ll numLines,long long startLine){
        numDataLines = numLines;
        fileHandle.seekg(startLine * LINE_LENGTH,ios_base::beg);
        ll count = 0;
        string line;
        data = (string *)malloc(numLines*LINE_LENGTH*sizeof(char));
        while(count<numLines){
            getline(fileHandle,line);
            data[count] = line;
            count++;    
        }
    }
    //Write/append data to files
    //Not the best way
    //Have to open myfile before i write
    void Write(){
        myfile.open(fileName,ios::out | ios::app);
        //myfile.seekg(startLine * LINE_LENGTH,ios_base::beg);
        int count = 0;
        while(count < numDataLines){
            myfile << data[count];
            count++;
        }
        myfile.close();
    }
    ll getTotalFileSize(){
        return totalSize;
    }
    ll getTotalNumLines(){
        return totalNumLines;
    }
    string* getData(){
        return data;
    }
    void setData(string *input_data){
        data = input_data;
    }
    void setNumDataLines(ll num_of_data_lines){
        numDataLines = num_of_data_lines;
    }
};

//Node that has to be used for MinHeap
class Node{

};

//Has function like getmin, add an element etc.,
class MinHeap{

};


//See what all options you can have
class Options{
    int numThreads;
    //char *fileName;
    string fileName;
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
    string getFileName(){
        return fileName;
    }
};

class Controller{
    int type;
    File *inputFile;
    public:
    Controller(File* input){
        //Type will be set according to this
        inputFile = input;
        if(inputFile->getTotalFileSize() > MEMORY_SIZE){
            type = EXTERNAL_SORTING;
        }
        else{
            type = INTERNAL_SORTING;
        }
    }
    int execute(){
        switch (type)
        {
            case INTERNAL_SORTING:{
                    inputFile->Open();
                    //Step 1: Read the data into memory
                    ll num_lines = inputFile->getTotalNumLines(); 
                    ll file_size = inputFile->getTotalFileSize();
                    inputFile->Read(num_lines,0);
                    string* data = inputFile->getData();                
                    //Step 2: Sort the data  
                    QuickSort(data, 0, num_lines - 1);  
                    //Step 3: Write to a file
                    //Create new output file                        
                    File *outfile = new File("sorted-data",file_size,OUTPUT); //NOTE: Actually file_size will be input file size by number of threads.
                    outfile->setNumDataLines((ll)(file_size/LINE_LENGTH));
                    outfile->setData(data);
                    outfile->Write();
                }
                break;
            case EXTERNAL_SORTING:
                //STEP1:
                //Read input file
                //Decide the size of each file operated by each thread
                //Load data for each thread
                //sort all threads individually and 
                //for each thread, write output to temp file


                //STEP2:
                //Build a min heap from first line of all the files
                //Extract the min
                //Repeat until you build the entir file
                //Write/append to sorted-file once everytime buffer is full

                break;
            default:
                break;
        }
    }

};


//Returns file size of a given file. This is a helper function
long long fileSize(std::string fileName){
    //std::cout<<fileName<<std::endl;
    FILE *p_file = NULL;
    p_file = fopen(fileName.c_str(),"r");
    fseek(p_file,0,SEEK_END);
    long long size = ftell(p_file);
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

    long long file_size = fileSize(opt.getFileName());
    File *fh = new File(opt.getFileName(),file_size,INPUT);

    Controller* ctrlr = new Controller(fh);
    ctrlr->execute();

    return 0;
}
