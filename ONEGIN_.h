#ifndef _ONEGIN__H_
#define _ONEGIN__H_

//#include "TXLib.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define INPUT_FILE_NAME "onegin.txt"
#define OUTPUT_FILE_NAME "out_.txt"

struct FileInfo
{
    int FileStream;
    size_t FileSize;
    ssize_t TextSize;
    size_t NumStrings;
    char* Buffer;
    char** Index;
    char FileName[100];

};
void SetFileName( struct FileInfo* file, const char* filename );
void PrepareBuffer( struct FileInfo* file );
void ReadFromFile( struct FileInfo* file );
void CountStrings( struct FileInfo* file );
void SplitStrings( struct FileInfo* file );

void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) );
void Swap( void* a, void* b, size_t type_size );
int CompareStringEnc( const void* a, const void* b );
int CompareStringRhyme( const void* a, const void* b );
void FPrintArrOfStr( FILE* stream, char** str, size_t num_str, const char* message );

/*
bool EOMyGetline( char** line, FILE* stream );
char** ReadArrOfStrFromFile( const char* filename, size_t* num_of_str );
char** CopyArrOfStr( char** str, size_t num_of_str );
void CleanMem( char** str_arr, char** copy_str_arr, size_t num_of_str );
void* Recalloc( void* str, size_t previous_size, size_t new_size );
*/

#endif
