#include <iostream>
//For string functions
#include <cstring>
#include <string>
//For files
#include <fstream>
#include <bits/stdc++.h>
#include "quicksort.h"
#include "minheap.h"
#include <thread>
#include <chrono>

using namespace std;

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
//ll MEMORY_SIZE  = 8*GB;
ll MEMORY_SIZE  = 10*MB;

//To implement:
//1. Timing performace
// Lets have timing for two phases separately.
// For merge phase, its simple Just have it there in mergefiles itself
// For sort phase, which happens in parallel, there should be an array
// of num_threads that gets updated at a particular index
// After sort phase, lets add everything and update the global pointer
//for write and read both
//Then measure the time during merge phase and then combine them as well.
//Now, we know how much total time it took.
double total_write_time = 0;
double total_read_time = 0;
double total_sort_time = 0;

class File{
    //This data is an array of some K number of lines
    //Instead of **data lets have string array that reads n lines from the given file
    //Then sort this string array
    //char **data;
    char **data;
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
    //time spent is an array and index is what we are updating
    char** Read(ll numLines,long long startLine,vector<double> &time_spent, int index){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        ifstream infile;
        infile.open(fileName,ios::binary);
        infile.seekg(startLine * LINE_LENGTH,ios_base::beg);
        //string *myArray = new string[numLines];
        
        //string *myArray[numLines];
        //char *arr[numLines];
        char **arr;
        arr = (char**)malloc(numLines * sizeof(char*));
        for (ll i = 0; i < numLines; i++)
        {
            char* line = (char*)malloc(LINE_LENGTH);
            infile.read(line,LINE_LENGTH);
            arr[i] = line;
        }
        infile.close();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        //*time_spent = ;
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        time_spent[index] = time_span.count();
        return arr;
    }
    //Write/append data to files
    //Not the best way
    //Have to open myfile before i write
    void Write(char** writedata,ll numLines,vector<double> &time_spent, int index){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ofstream outfile(fileName,ios::binary);

        for(ll i = 0; i < numLines; ++i)
        {
            outfile.write(writedata[i],LINE_LENGTH);
        }
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        //*time_spent = ;
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        delete[] writedata;
        time_spent[index] = time_span.count();
    }

    ll getTotalFileSize(){
        return totalSize;
    }
    ll getTotalNumLines(){
        return totalNumLines;
    }
    char** getData(){
        return data;
    }
    void setData(char** input_data){
        data = input_data;
    }
    void setNumDataLines(ll num_of_data_lines){
        numDataLines = num_of_data_lines;
    }
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

void sort_operation(char** data,ll num_lines,vector<double>&sort_time_spent,int sort_index){
        //Step 2: Sort the data  

        std::chrono::high_resolution_clock::time_point sort_t1 = std::chrono::high_resolution_clock::now();
        QuickSort(data, 0, num_lines - 1);  
        std::chrono::high_resolution_clock::time_point sort_t2 = std::chrono::high_resolution_clock::now();
        //*time_spent = ;
        std::chrono::duration<double> sort_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(sort_t2 - sort_t1);
        sort_time_spent[sort_index] = sort_time_span.count();  
        
}

void sort_and_write(char** data,ll num_lines,ll file_size,string outfilename,vector<double>&write_time_spent,int write_index,vector<double>&sort_time_spent,int sort_index){

        sort_operation(data,num_lines,sort_time_spent,sort_index);
        //Step 3: Write to a file
        //Create new output file                        
        File *outfile = new File(outfilename,file_size,OUTPUT); //NOTE: Actually file_size will be input file size by number of threads.
        outfile->setNumDataLines((ll)(file_size/LINE_LENGTH));
        //outfile->setData();
        // vector<double> time_spent(1,0);
        //outfile->Write(data,num_lines,time_spent,0);
        outfile->Write(data,num_lines,write_time_spent,write_index);
}



//This needs list of files and output file name
class FileMerger{
    string outfile;
    vector<string> infilenames;
    public:
    FileMerger(vector<string> ifile,string ofile){
        infilenames = ifile;
        outfile = ofile;
    }
    void mergeFiles(){
        ifstream infile[infilenames.size()];
        ofstream ofile;
        int num_input_files = infilenames.size();
        int file_num = 0;
        int line_length = 100;
        //Input files
        for (size_t i = 0; i < num_input_files; i++)
        {
            infile[i].open(infilenames.at(i),ios::binary);
        }
        //Output file
        ofile.open(outfile,ios::binary);
        //MinHeapNode* harr = new MinHeapNode[num_input_files];
        MinHeapNode harr[num_input_files];
        priority_queue<MinHeapNode, vector<MinHeapNode>, comp> pq;

        int i = 0;
        for (i = 0; i < num_input_files; i++)
        {
            
            /* code */
            char* line = (char*)malloc(line_length);
            
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();            
            infile[i].read(line,line_length);
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            //*time_spent = ;
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            total_read_time += time_span.count();
            harr[i].element = line;
            harr[i].i = i;    
            pq.push(harr[i]);
        }
        
        int count = 0;
        
        while (count != num_input_files)
        {
            // Get the minimum element and store it in output file
            MinHeapNode root = pq.top();
            pq.pop();
            //ofile.write(const_cast<char*>(root.element.c_str()),root.element.length());
            std::chrono::high_resolution_clock::time_point w1 = std::chrono::high_resolution_clock::now();            
            ofile.write(root.element,line_length);
            std::chrono::high_resolution_clock::time_point w2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> write_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(w2 - w1);
            total_write_time += write_time_span.count();
            delete root.element;
            //cout<<"file num is "<<i<<endl;
            char* line = (char*)malloc(line_length);
            i = root.i;
            //Read next element from root.i filehandle
            if(infile[i].peek() == EOF){
                count++;
                continue;
            }
            std::chrono::high_resolution_clock::time_point r1 = std::chrono::high_resolution_clock::now();
            infile[i].read(line,line_length);
            std::chrono::high_resolution_clock::time_point r2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> read_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(r2 - r1);
            total_write_time += read_time_span.count();
            
            root.element = line;
            pq.push(root);
        }
        
    }
};

class Controller{
    int type;
    File *inputFile;
    Options* opt;
    public:
    Controller(File* input,Options* options){
        //Type will be set according to this
        inputFile = input;
        if(inputFile->getTotalFileSize() > MEMORY_SIZE){
            type = EXTERNAL_SORTING;
        }
        else{
            type = INTERNAL_SORTING;
        }
        opt = options;
    }
    int execute(){
        inputFile->Open();
        //Step 1: Read the data into memory
        ll num_lines = inputFile->getTotalNumLines(); 
        ll file_size = inputFile->getTotalFileSize();
        switch (type)
        {
            case INTERNAL_SORTING:{
                    vector<double> read_time(1,0);
                    char** data = inputFile->Read(num_lines,0,std::ref(read_time),0);
                    total_read_time += read_time[0];
                    string outfilename = "sorted-data";
                    vector<double> write_time(1,0);
                    vector<double> sort_time(1,0);
                    int index=0;
                    sort_and_write(data,num_lines,file_size,outfilename,std::ref(write_time),index,sort_time,index);
                    total_write_time += write_time[0];
                    total_sort_time += sort_time[0];
                }
                break;
            case EXTERNAL_SORTING:{
                    //STEP1: MULTI THREADING PART
                    //Read input file
                    //Decide the size of each file operated by each thread
                    //Load data for each thread
                    //sort all threads individually and 
                    //for each thread, write output to temp fil
                    int total_num_threads = opt->getNumThreads();
                    //Chunk size in bytes
                    ll chunk_size = (ll)(MEMORY_SIZE/(total_num_threads*LINE_LENGTH))*LINE_LENGTH;
                    ll num_lines_per_chunk = (ll)chunk_size/LINE_LENGTH;
                    ll lines_sorted_per_iteration = num_lines_per_chunk*total_num_threads;
                    //Number of iterations
                    int num_iterations = (int)(file_size/(LINE_LENGTH*lines_sorted_per_iteration));
                    num_iterations += 1;
                    //Add these leftover lines to the last chunk
                    ll leftover_lines = num_lines;
                    //Position
                    ll pos = 0;
                    //Create thread array
                    thread myThreads[total_num_threads];
                    int iteration = 0;
                    vector<string> temp_files;
                    int leftoverrun = false;
                    vector<double> read_time(total_num_threads,0);
                    vector<double> write_time(total_num_threads,0);
                    vector<double> sort_time(total_num_threads,0);
                    //while(iteration < num_iterations ||  leftover_lines > 0){
                    while(iteration < num_iterations){
                        fill(read_time.begin(),read_time.end(),0);    
                        fill(write_time.begin(),write_time.end(),0);    
                        fill(sort_time.begin(),sort_time.end(),0);    
                        int numThreadsRunning = 0;
                        for (int i = 0; i < total_num_threads; i++)
                        {
                            if(leftover_lines > 0){
                                char **data;                                        
                                //Read num lines per chunk from the input file
                                if(leftover_lines>num_lines_per_chunk){
                                    data = inputFile->Read(num_lines_per_chunk,pos,std::ref(read_time),i);
                                }
                                else{
                                    data = inputFile->Read(leftover_lines,pos,std::ref(read_time),i);
                                    leftoverrun = true;
                                }
                                string outfilename = "file_"+to_string(iteration)+"_"+ to_string(i);  
                                temp_files.push_back(outfilename);
                                ll file_size = num_lines_per_chunk*LINE_LENGTH;  
                                if(leftoverrun == false){
                                    myThreads[i] = thread(sort_and_write,data,num_lines_per_chunk,file_size,outfilename,std::ref(write_time),i,std::ref(sort_time),i);                                    
                                    numThreadsRunning++;
                                }
                                else{
                                    myThreads[i] = thread(sort_and_write,data,leftover_lines,file_size,outfilename,std::ref(write_time),i,std::ref(sort_time),i);
                                    numThreadsRunning++;
                                }
                                pos+=num_lines_per_chunk;
                                leftover_lines -=num_lines_per_chunk;
                            }
                            else{
                                break;
                            }
                        }

                        for (size_t i = 0; i < numThreadsRunning; i++)
                        {
                            myThreads[i].join();
                        }

                        //Merge all read times 
                        total_read_time += accumulate(read_time.begin(),read_time.end(),0.0);
                        total_write_time += accumulate(write_time.begin(),write_time.end(),0.0);
                        total_sort_time += accumulate(sort_time.begin(),sort_time.end(),0.0);
                        iteration++;
                        //leftover_lines -= lines_sorted_per_iteration;        
                    }
                        
                    //Now call the operation of min heap
                    string outfile = "sorted-data";
                    FileMerger* fm = new FileMerger(temp_files,outfile);
                    fm->mergeFiles();

                    }

                // STEP2:
                // Build a min heap from first line of all the files
                // Extract the min
                // Repeat until you build the entire file
                // Write/append to sorted-file once everytime buffer is full              
                

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

    double total_time = 0;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    //Just have options and controller objects together
    //here
    Options *opt = new Options(argc,argv);
    //If sanity check fails, then all the expected inputs are not present
    if(!opt->sanityCheck()){
        std::cout<<"Incorrect Usage"<<std::endl;
        std::cout<<"Usage: ./ms.out -F [filename] -t [number of threads]"<<std::endl;
        exit(0);
    };

    long long file_size = fileSize(opt->getFileName());
    File *fh = new File(opt->getFileName(),file_size,INPUT);

    Controller* ctrlr = new Controller(fh,opt);
    ctrlr->execute();

    cout<<"Total Read time : "<<total_read_time<<endl;
    cout<<"Total Write time : "<<total_write_time<<endl;
    cout<<"Total Sort time : "<<total_sort_time<<endl;

    cout<<"file size is "<<file_size<<endl;
    double write_speed = (file_size*2)/(total_write_time*MB);
    double read_speed = (file_size*2)/(total_read_time*MB);
    double sort_speed = (file_size)/(total_sort_time*MB);

    cout<<"Read speed : "<<read_speed<<" MBPS"<<endl;
    cout<<"Write speed : "<<write_speed<<" MBPS"<<endl;
    cout<<"Sort speed : "<<sort_speed<<" MBPS"<<endl;

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> read_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    total_time += read_time_span.count();
    cout<<"Total MySort time : "<<total_time<<endl;
    cout<<"MySort speed : "<<(file_size)/(total_time*MB)<<" MBPS"<<endl;

    return 0;
}
