#ifndef _ONEGIN__H_
#define _ONEGIN__H_

//#include "TXLib.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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

struct ComparatorInfo
{
    const char ComparatorName[100];
    int (*Comparator)(const void* a, const void* b);
};

void SortAndWriteToFile( const char* filename, struct FileInfo* file,
                         struct ComparatorInfo comparators[], size_t num_sorts );

void PrepareFileForSorting( const char* filename, struct FileInfo* file );

void SetFileName( struct FileInfo* file, const char* filename );
void PrepareBuffer( struct FileInfo* file );
void ReadFromFile( struct FileInfo* file );
void CountStrings( struct FileInfo* file );
void SplitStrings( struct FileInfo* file );

void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) );
void Swap( void* a, void* b, size_t type_size );
int CompareStringsEnc( const void* a, const void* b );
int CompareStringsRhyme( const void* a, const void* b );
int ComparePointersUp( const void* a, const void* b );
void FPrintArrOfStr( FILE* stream, char** str, size_t num_str, const char* message );

#endif
