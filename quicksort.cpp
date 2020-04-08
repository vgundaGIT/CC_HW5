#include <iostream>
#include <bits/stdc++.h> 
#include <string>
#include "quicksort.h"

using namespace std;  
  

void swap(string &s,string &t){
    string temp = s;
    s = t;
    t = temp;
}

//partition function
long long partition(string arr[],long low, long high){
    string pivot = arr[high];
    long long i = low-1;

    //In a loop, compare every element with pivot 
    //and if the current element is small, swap it with  
    //element at index i

    for (long long j = low; j < high; j++)
    {
        if(arr[j].compare(0,2,pivot) < 0){
            i++;
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[i+1],arr[high]);
    return i+1;
}   

//sorting function
void QuickSortHelper(string arr[],long long low,long long high){
    if(low<high){
        //Find the partitioning
        long long pi = partition(arr,low,high);
        QuickSortHelper(arr,low,pi-1);
        QuickSortHelper(arr,pi+1,high);
    }
}

void QuickSort(string arr[],long long low,long long high){
    QuickSortHelper(arr,low,high);
}


void printArray(string arr[], long long size)  
{  
    int i;  
    for (i = 0; i < size; i++)  
        cout << arr[i] << " ";  
    cout << endl;  
}

//There two are not required



/*
int main()  
{  
    string arr[] = {"dog","cat","beet","snake","insect","ant"};
    int n = 6;
    QuickSort(arr, 0, n - 1);  
    cout << "Sorted array: \n";  
    printArray(arr, n);  
    return 0;  
}  
  
 

 */
