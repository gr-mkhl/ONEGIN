#include "ONEGIN.h"

int main()
{
    size_t num_of_str = 0;

    char** strings = ReadArrOfStrFromFile(INPUT_FILE_NAME, &num_of_str);

    char** original_onegin = CopyArrOfStr(strings, num_of_str);

    FILE* output_file = fopen(OUTPUT_FILE_NAME, "w");
    assert(output_file);

    QuickSort(strings, num_of_str, sizeof(char*), CompareStringRhyme);
    FPrintArrOfStr(output_file, strings, num_of_str, "RHYME SORT");
    QuickSort(strings, num_of_str, sizeof(char*), CompareStringEnc);
    FPrintArrOfStr(output_file, strings, num_of_str, "ENCYCLOPEDIA SORT");
    FPrintArrOfStr(output_file, original_onegin, num_of_str, "ORIGINAL ONEGIN");

    CleanMem(strings, original_onegin, num_of_str);
    fclose(output_file);

    return 0;
}

void CleanMem( char** str_arr, char** copy_str_arr, size_t num_of_str )
{
    for (size_t i = 0; i < num_of_str; i++)
        free(str_arr[i]);
    free(str_arr);
    free(copy_str_arr);
}

char** ReadArrOfStrFromFile( const char* filename, size_t* num_of_str )
{
    FILE* input_file = fopen(filename, "r");
    assert(input_file);

    size_t buffer_size = 128;
    char** str_arr = (char**) calloc(buffer_size, sizeof(char*));
    assert(str_arr);

    bool file_read_flag = true;

    while (file_read_flag != false)
    {

        if (*num_of_str == buffer_size)
        {

            size_t new_buffer_size = buffer_size * 2;
            char** new_str_arr = (char**) Recalloc(str_arr, buffer_size * sizeof(char*), new_buffer_size * sizeof(char*));
            assert(new_str_arr);

            str_arr = new_str_arr;
            buffer_size = new_buffer_size;
        }
        file_read_flag = EOMyGetline(&str_arr[*num_of_str], input_file);
        *num_of_str += file_read_flag;
    }

    fclose(input_file);

    return str_arr;
}

char** CopyArrOfStr( char** str, size_t num_of_str )
{
    assert(str);

    char** copied_arr = (char**) calloc(num_of_str, sizeof(char*));
    assert(copied_arr);

    memcpy(copied_arr, str, num_of_str * sizeof(char*));

    return copied_arr;
}

void FPrintArrOfStr( FILE* stream, char** str, size_t num_str, const char* message )
{
    assert(str);
    assert(stream);

    fprintf(stream, "\nTHE BEGINNING OF %s\n\n", message);
    for (size_t i = 0; i < num_str; i++)
    {
        fprintf(stream, "[%d]:\taddress = [0x%p], len = [%d], string = <%s>\n", i, str[i], strlen(str[i]), str[i]);
    }
    fprintf(stream, "\nTHE END OF %s\n\n", message);
}

void* Recalloc( void* str, size_t previous_size, size_t new_size )
{
    char* new_str = (char*) realloc(str, new_size);

    for (size_t i = previous_size; i < new_size; i++)
        new_str[i] = 0;

    return new_str;
}

bool EOMyGetline( char** line, FILE* stream )
{

    if (line == NULL  || stream == NULL)
        return false;

    size_t buffer_len = 0;

    if (*line == NULL)
    {
        size_t first_size = 64;
        char *new_buf = (char*)calloc(first_size, sizeof(char));
        if (new_buf == NULL)
            return false;

        *line = new_buf;
        buffer_len = first_size;
    }

    size_t len = 0;
    int c = 0;

    while ((c = getc(stream)) != EOF)
    {
        if (len + 1 >= buffer_len)
        {
            size_t new_size = buffer_len * 2;
            char* new_buf = (char*) realloc(*line, new_size);
            if (new_buf == NULL)
                return false;

            *line = new_buf;
            buffer_len = new_size;
        }
        if (c == '\n')
        {
            (*line)[len] = '\0';
            return true;
        }
        (*line)[len++] = (char) c;
    }

    return false;
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

/* в процессе введения в эксплуатацию
const char* MyConstStrAlpha( const char* str )
{
    assert(str);

    while (isalpha(*str) == 0 && *str != '\0')
            str++;

    return str;
}

const char* MyConstStrrAlpha( const char* str, size_t* len )
{
    assert(str);

    str += *len;

    while (isalpha(*str) == 0 && *len > 0)
    {
        str--;
        (*len)--;
    }

    return str;
}
*/
