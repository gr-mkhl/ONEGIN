#include "ONEGIN_.h"

const struct ComparatorInfo Comparators[] = {
                                                {"ENCYCLOPEDIA SORT", CompareStringsEnc},
                                                {"RHYME SORT", CompareStringsRhyme},
                                                {"ORIGINAL TEXT", ComparePointersUp}
                                            };

int main()
{
    struct FileInfo InputFile = {};
    PrepareFileForSorting(INPUT_FILE_NAME, &InputFile);

    SortAndWriteToFile(OUTPUT_FILE_NAME, &InputFile, Comparators, sizeof(Comparators)/sizeof(Comparators[0]));

    freeFileInfo(&InputFile);

    return 0;
}

void PrepareFileForSorting( const char* filename, struct FileInfo* file )
{
    assert(file);
    assert(filename);

    SetFileName(file, filename);
    PrepareBuffer(file);
    ReadFromFile(file);
    CountStrings(file);
    SplitStrings(file);

    return;
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

    return;
}

void ReadFromFile( struct FileInfo* file )
{
    assert(file);

    file->FileStream = open(file->FileName, O_RDONLY);  //TODO Тут должна быть обработка ошибок открытия файлов
    assert(file->FileStream != -1);

    file->TextSize = read(file->FileStream, file->Buffer, file->FileSize);
    assert(file->TextSize > 0);

    file->Buffer[file->TextSize] = '\0';  //NOTE - ЭТО ОЧЕНЬ ВАЖНАЯ СТРОКА!!!!

    int close_status = close(file->FileStream);
    assert(close_status != -1);


    return;
}

void CountStrings( struct FileInfo* file )
{
    assert(file);
    assert(file->Buffer);

    char* buffer_ptr = file->Buffer;

    size_t n_strings = 0;
    size_t i = 0;

    while (buffer_ptr[i] != '\0') //TODO тут должен быть strchr()
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

    String* index_ptr = (String*)calloc(file->NumStrings, sizeof(String));
    assert(index_ptr);

    index_ptr[0].str = file->Buffer;

    char* buffer_ptr = file->Buffer;
    size_t n_strings = file->NumStrings;

    size_t str_index = 1;
    size_t i = 0, str_len = 0;
    while (str_index < n_strings)
    {
        if (buffer_ptr[i] == '\0')
        {
            index_ptr[str_index].str = &buffer_ptr[i + 1];
            index_ptr[str_index++ - 1].len = str_len;
            str_len = 0;
        }
        else
        {
            str_len++; //Не нужно увеличивать str_len на \0 символе
        }
        i++;
    }
    index_ptr[str_index].len = str_len;

    file->Index = index_ptr;

    return;
}

void SortAndWriteToFile( const char* filename, const struct FileInfo* const file,
                         const struct ComparatorInfo* const comparators, size_t num_sorts )
{
    assert(filename);
    assert(file);
    assert(comparators);

    FILE* file_out = fopen(filename, "w");     //TODO Тут должна быть обработка ошибок открытия файлов
    assert(file_out);

    for (size_t i = 0; i < num_sorts; i++)
    {
        QuickSort(file->Index, file->NumStrings, sizeof(String), comparators[i].Comparator);
        FPrintArrOfStr(file_out, file->Index, file->NumStrings, comparators[i].ComparatorName);
    }

    int close_status = fclose(file_out);
    assert(close_status != EOF);

    return;
}

void freeFileInfo( struct FileInfo* file )
{
    free(file->Index);
    free(file->Buffer);

    return;
}


void FPrintArrOfStr( FILE* stream, String* str_arr, size_t num_str, const char* message )
{
    assert(str_arr);
    assert(stream);

    fprintf(stream, "\nTHE BEGINNING OF %s\n\n", message);
    for (size_t i = 0; i < num_str; i++)
    {
        fprintf(stream, "[%d]:\taddress = [0x%p], strlen = [%d], structlen = [%u], string = <%s>\n",
                i, str_arr[i].str, strlen(str_arr[i].str), str_arr[i].len, str_arr[i].str);
    }
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


int CompareStringsEnc( const void* a, const void* b )
{
    assert(a);
    assert(b);

    //const char* first_str = *((const char* const * const)a);
    //const char* second_str = *((const char* const * const)b);

    const char* first_str = ((const String*)a)->str;
    const char* second_str = ((const String*)b)->str;

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


int CompareStringsRhyme( const void* a, const void* b ) //TODO НАДО ПЕРЕДЕЛАТЬ ВСЕ ПОД СТРУКТУРЫ. ВСЁ.
{
    assert(a);
    assert(b);

    const char* first_str = ((const String*)a)->str;
    const char* second_str = ((const String*)b)->str;

    const size_t len_first = ((const String*)a)->len;
    const size_t len_second = ((const String*)b)->len;

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


int ComparePointersUp( const void* a, const void* b )
{
    assert(a);
    assert(b);

    const intptr_t first_ptr = *((const intptr_t*)a);
    const intptr_t second_ptr = *((const intptr_t*)b);

    if (first_ptr > second_ptr)
        return 1;
    else if (first_ptr < second_ptr)
        return -1;
    else
        return 0;
}
