#include "ONEGIN_.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int main()
{

    struct FileInfo Input_File = {};
    clock_t Start = clock();
    SetFileName(&Input_File, INPUT_FILE_NAME);
    PrepareBuffer(&Input_File);
    ReadFromFile(&Input_File);
    CountStrings(&Input_File);
    SplitStrings(&Input_File);

    clock_t End = clock();
    printf("prepare time: %g\n", (double)(End - Start) / CLOCKS_PER_SEC);

    FILE* File_out = fopen(OUTPUT_FILE_NAME, "w");

    Start = clock();
    FPrintArrOfStr(File_out, Input_File.Index, Input_File.NumStrings, "ORIGINAL ONEGIN");
    End = clock();
    printf("print original time: %g\n", (double)(End - Start) / CLOCKS_PER_SEC);


    Start = clock();
    QuickSort(Input_File.Index, Input_File.NumStrings, sizeof(char*), CompareStringEnc);
    FPrintArrOfStr(File_out, Input_File.Index, Input_File.NumStrings, "ENCYCLOPEDIA SORT");
    End = clock();
    printf("encyclopedia sort time: %g\n", (double)(End - Start) / CLOCKS_PER_SEC);

    Start = clock();
    QuickSort(Input_File.Index, Input_File.NumStrings, sizeof(char*), CompareStringRhyme);
    FPrintArrOfStr(File_out, Input_File.Index, Input_File.NumStrings, "RHYME SORT");
    End = clock();
    printf("rhyme sort time: %g\n", (double)(End - Start) / CLOCKS_PER_SEC);

    fclose(File_out);

    return 0;
}

void SetFileName( struct FileInfo* file, const char* filename )
{
    assert(file);
    assert(filename);

    char* set_file_status = strcpy(file->FileName, filename);
    assert(set_file_status);

    return;
}

void PrepareBuffer( struct FileInfo* file )
{
    assert(file);

    struct stat my_file = {};

    int stat_status = stat(file->FileName, &my_file);
    assert(stat_status != -1);

    file->FileSize = my_file.st_size;

    file->Buffer = (char*)calloc(file->FileSize + 1, sizeof(char));
    assert(file->Buffer != NULL);

    file->Buffer[file->FileSize] = '\0';

    return;
}

void ReadFromFile( struct FileInfo* file )
{
    assert(file);

    file->FileStream = open(file->FileName, O_RDONLY);
    assert(file->FileStream != -1);

    file->TextSize = read(file->FileStream, file->Buffer, file->FileSize);
    assert(file->TextSize > 0);

    int close_status = close(file->FileStream);
    assert(close_status != -1);

    file->Buffer[file->TextSize] = '\0';

    return;
}

void CountStrings( struct FileInfo* file )
{
    assert(file);
    assert(file->Buffer);

    char* buffer_ptr = file->Buffer;

    size_t i = 0, n_strings = 0;

    while (buffer_ptr[i] != '\0')
    {
        if (buffer_ptr[i] == '\n')
        {
            buffer_ptr[i] = '\0';
            n_strings++;
        }
        i++;
    }
    file->NumStrings = n_strings;

    return;
}

void SplitStrings( struct FileInfo* file )
{
    assert(file);

    char** index_ptr = (char**)calloc(file->NumStrings, sizeof(char*));
    assert(index_ptr);

    index_ptr[0] = file->Buffer;
    char* buffer_ptr = file->Buffer;
    size_t n_strings = file->NumStrings;

    size_t str_index = 1;
    size_t i = 0;
    while (str_index < n_strings)
    {
        if (buffer_ptr[i] == '\0')
            index_ptr[str_index++] = &buffer_ptr[i + 1];
        i++;
    }

    file->Index = index_ptr;

    return;
}

void FPrintArrOfStr( FILE* stream, char** str, size_t num_str, const char* message )
{
    assert(str);
    assert(stream);

    fprintf(stream, "\nTHE BEGINNING OF %s\n\n", message);
    for (size_t i = 0; i < num_str; i++)
        fprintf(stream, "[%d]:\taddress = [0x%p], len = [%d], string = <%s>\n", i, str[i], strlen(str[i]), str[i]);
    fprintf(stream, "\nTHE END OF %s\n\n", message);
}

void QuickSort( void* mas, size_t len, size_t typesize, int (*Compare)(const void* a, const void* b) )
{
    assert(mas);

    if (len == 1 || len == 0)
        return;

    size_t i = 0, num = 0;

    for (i = 0; i < len - 1; i++)
        if (Compare((uint8_t*)mas + i * typesize, (uint8_t*)mas + (len - 1) * typesize) <= 0)
            Swap((uint8_t*)mas + i * typesize, (uint8_t*)mas + num++ * typesize, typesize);
    Swap((uint8_t*)mas + (len - 1) * typesize, (uint8_t*)mas + num * typesize, typesize);

    QuickSort(mas, num, typesize, Compare);
    QuickSort((uint8_t*)mas + (num + 1) * typesize, len - 1 - num, typesize, Compare);
}

#define SWAP_BUFFERS_a_AND_b_SIZEOF(SIZE)  do                                      \
                                            {                                      \
                                                while (type_size >= sizeof(SIZE))  \
                                                {                                  \
                                                    SIZE temp = 0;                 \
                                                    temp = *((SIZE*)a);            \
                                                    *((SIZE*)a) = *((SIZE*)b);     \
                                                    *((SIZE*)b) = temp;            \
                                                                                   \
                                                    type_size -= sizeof(SIZE);     \
                                                    a = ((SIZE*)a + 1);            \
                                                    b = ((SIZE*)b + 1);            \
                                                }                                  \
                                            } while(0)

void Swap( void* a, void* b, size_t type_size )
{
    assert(a);
    assert(b);

    SWAP_BUFFERS_a_AND_b_SIZEOF(uint64_t);
    SWAP_BUFFERS_a_AND_b_SIZEOF(uint32_t);
    SWAP_BUFFERS_a_AND_b_SIZEOF(uint16_t);
    SWAP_BUFFERS_a_AND_b_SIZEOF(uint8_t);


}

#undef SWAP_BUFFERS_a_AND_b_SIZEOF

int CompareStringEnc( const void* a, const void* b )
{
    assert(a);
    assert(b);

    const char* first_str = *((const char* const*)a);
    const char* second_str = *((const char* const*)b);

    while (*first_str != '\0' && *second_str != '\0')
    {
        while (isalpha(*first_str) == 0 && *first_str != '\0')
            first_str++;
        while (isalpha(*second_str) == 0 && *second_str != '\0')
            second_str++;

        if (tolower(*first_str) != tolower(*second_str))
            return tolower(*first_str) - tolower(*second_str);
        first_str++;
        second_str++;
    }

    return tolower(*first_str) - tolower(*second_str);
}

int CompareStringRhyme( const void* a, const void* b )
{
    assert(a);
    assert(b);

    const char* first_str = *((const char* const*)a);
    const char* second_str = *((const char* const*)b);

    size_t len_first = strlen(first_str);
    size_t len_second = strlen(second_str);

    first_str += len_first;
    second_str += len_second;

    while (len_first >= 1 && len_second >= 1)
    {
        while (isalpha(*first_str) == 0 && len_first > 0)
        {
            first_str--;
            len_first--;
        }
        while (isalpha(*second_str) == 0 && len_second > 0)
        {
            second_str--;
            len_second--;
        }
        if (len_first == 0 || len_second == 0 || tolower(*first_str) != tolower(*second_str))
            return tolower(*first_str) - tolower(*second_str);
        first_str--;
        second_str--;
    }

    if (len_first != 0)
        first_str--;
    if (len_second != 0)
        second_str--;

    return tolower(*first_str) - tolower(*second_str);
}


