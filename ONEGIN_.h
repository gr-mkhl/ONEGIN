#ifndef _ONEGIN__H_
#define _ONEGIN__H_

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
#define MAX_NAME_LEN 200
#define INPUT_FILE_NAME "onegin.txt"
#define OUTPUT_FILE_NAME "out_.txt"


struct String
{
    char* str;
    size_t len;
};

struct FileInfo
{
    int FileStream;
    size_t FileSize;
    ssize_t TextSize;
    size_t NumStrings;
    char* Buffer;
    String* Index;
    char FileName[MAX_NAME_LEN];
};

struct ComparatorInfo
{
    const char ComparatorName[MAX_NAME_LEN];
    int (*Comparator)(const void* a, const void* b);
};


void SetFileName( struct FileInfo* file, int argc, char** argv );

void PrepareFileForSorting( const char* filename, struct FileInfo* file );

void SortAndWriteToFile( const char* filename, const struct FileInfo* const file,
                         const struct ComparatorInfo* const comparators, size_t num_sorts );

void FreeFileInfo( struct FileInfo* file );

void PrepareBuffer( struct FileInfo* file );
void ReadFromFile( struct FileInfo* file );
void CountStrings( struct FileInfo* file );
void SplitStrings( struct FileInfo* file );

void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) );
void Swap( void* a, void* b, size_t type_size );
int CompareStringsEnc( const void* a, const void* b );
int CompareStringsRhyme( const void* a, const void* b );
int ComparePointersUp( const void* a, const void* b );

void FPrintArrOfStr( FILE* stream, String* str_arr, size_t num_str, const char* message );

#endif
