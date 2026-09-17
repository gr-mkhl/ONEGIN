#ifndef _ONEGIN_H_
#define _ONEGIN_H_

#include "TXLib.h"
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

void MyGetline( char** line, bool* read_status, FILE* stream );
void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) );
//int CompareStringUp( const void* a, const void* b );
int CompareStringEnc( const void* a, const void* b );
int CompareStringRhyme( const void* a, const void* b );
void Swap( void* a, void* b, size_t type_size );
void* Recalloc( void* str, size_t previous_size, size_t new_size );

#define SWAP_BUFFERS_a_AND_b_SIZEOF(SIZE, BUFFER) do                               \
                                                    {                              \
                                                      BUFFER = *((SIZE*)a);        \
                                                      *((SIZE*)a) = *((SIZE*)b);   \
                                                      *((SIZE*)b) = BUFFER;        \
                                                                                   \
                                                      type_size -= sizeof(SIZE);   \
                                                      a = ((SIZE*)a + 1);          \
                                                      b = ((SIZE*)b + 1);          \
                                                    } while(0)

#endif
