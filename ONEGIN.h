#ifndef _ONEGIN_H_
#define _ONEGIN_H_

#include "TXLib.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

bool EOMyGetline( char** line, FILE* stream );
void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) );
void Swap( void* a, void* b, size_t type_size );
int CompareStringEnc( const void* a, const void* b );
int CompareStringRhyme( const void* a, const void* b );
void* Recalloc( void* str, size_t previous_size, size_t new_size );

void FPrintArrOfStr( FILE* stream, char** str, size_t num_str, const char* message );
char** ReadArrOfStrFromFile( const char* filename, size_t* num_of_str );
char** CopyArrOfStr( char** str, size_t num_of_str );
void CleanMem( char** str_arr, char** copy_str_arr, size_t num_of_str );

/* в процессе введения в эксплуатацию
const char* MyConstStrAlpha( const char* str );
const char* MyConstStrrAlpha( const char* str, size_t* len );
*/
#define INPUT_FILE_NAME "onegin.txt"
#define OUTPUT_FILE_NAME "out.txt"

#endif
