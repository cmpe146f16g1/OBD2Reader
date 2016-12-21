//
//  main.cpp
//  146_project
//
//  Created by Samira C. Oliva on 26/11/16.
//  Copyright (c) 2016 Samira C. Oliva. All rights reserved.
/*
 
    build byte offset array of file to use with
    read functions from storage objects.
 */

#include <iostream>
#include <string.h>
using namespace std;

#define PRINT_OUT_LEN 1000
#define CODES 3395
#define INPUT_FILE "/Users/samiracoliva/Documents/F16/CMPE146/project/146_project_code/146_project_code/input.txt"
string obd2_codes[CODES][2];


#define MAX_LINE 1024
#define ELEMENTS 50   //to realloc for idxAry

long   *sweepFileAndBuildByteOffsetIndexArray(FILE *spIn);
void    printarraycode(long* ary);
void    freeMem(long *idxAry);

int main(int argc, const char * argv[])
{
    FILE *spIn;
    spIn = fopen(INPUT_FILE, "r");
    //byte offsets array
    long *idxAry = sweepFileAndBuildByteOffsetIndexArray(spIn);
    printarraycode(idxAry);
    freeMem(idxAry);

    fclose(spIn);

    return 0;
}

void printarraycode(long* ary)
{
    long *ptr = ary;
    int count = 0;
    while (ptr  && count < PRINT_OUT_LEN)
    {
        if(count+1 % 10 == 0)
          cout << endl;
        cout << *ptr << ",";
        count++;
        ptr++;
    }
    
}

//-------------------------------------------------------------------
// Builds and index array to store the byte offsets of the beginning
// of each line in a file, with the each index of the array
// corresponding to each line in a file
//
// pre:  spIn
//
// post: int* and index array containing the byte offsets of the
//       beginning of each line in a given file
//
long *sweepFileAndBuildByteOffsetIndexArray(FILE *spIn)
{
    char buffer[MAX_LINE];
    long *idxAry, *mover;
    int  arySize = 0;
    long byteOffset;
    
    fseek(spIn, 0, SEEK_SET);
    idxAry     = (long*)malloc(ELEMENTS * sizeof(long));
    mover      = (long*)malloc(sizeof(long));
    
    mover = idxAry;
    
    *mover = 0; //for first line
    mover++;
    while(fgets(buffer, MAX_LINE - 1, spIn)) //change to fread
    {
        byteOffset =  ftell(spIn);
        *mover     =  byteOffset;
        
        if(++arySize % ELEMENTS == 0)
        {
            idxAry = (long*)realloc(idxAry, (arySize + ELEMENTS + 1) * sizeof(long));
            mover  = idxAry + arySize - 1;
            
        }
        mover++;
        
    }//while
    
    //mover = NULL;
    //free(mover);
    return idxAry;
}

//-------------------------------------------------------------------
// Function frees any memory that was dynamically allocated
//
// pre:   idxAray
//
// post:  idxAray is freed
//
void  freeMem(long *idxAry)
{
    free(idxAry);
    idxAry = NULL;
}





