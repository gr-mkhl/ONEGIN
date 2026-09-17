#include "ONEGIN.h"

int main()
{
    size_t buffer_size = 128;

    char** strings = (char**) calloc(buffer_size, sizeof(char*));
    FILE* READ = fopen("onegin.txt", "r");
    assert(READ);
    FILE* WRITE = fopen("out.txt", "w");
    assert(WRITE);

    size_t Strings_Read = 0;
    bool file_read_flag = 1;

    while (file_read_flag != 0)
    {

        if (Strings_Read == buffer_size)
        {

            size_t new_buffer_size = buffer_size * 2;
            char** new_strings = (char**) Recalloc(strings, buffer_size  * sizeof(char*), new_buffer_size * sizeof(char*));
            strings = new_strings;
            buffer_size = new_buffer_size;
        }
        MyGetline(&strings[Strings_Read], &file_read_flag, READ);

        Strings_Read += file_read_flag;
    }

    char** original_Onegin = (char**) calloc(Strings_Read, sizeof(char*));
    memcpy(original_Onegin, strings, Strings_Read * sizeof(char*));

    QuickSort(strings, Strings_Read, sizeof(char*), CompareStringEnc);

    for (size_t i = 0; i < Strings_Read; i++)
    {
        fprintf(WRITE, "id = [%d] address =[%p] len = [%d] string = <%s>\n", i, strings[i], strlen(strings[i]), strings[i]);
    }

    QuickSort(strings, Strings_Read, sizeof(char*), CompareStringRhyme);

    for (size_t i = 0; i < Strings_Read; i++)
    {
        fprintf(WRITE, "id = [%d] address =[%p] len = [%d] string = <%s>\n", i, strings[i], strlen(strings[i]), strings[i]);
    }

    for (size_t i = 0; i < Strings_Read; i++)
    {
        fprintf(WRITE, "id = [%d] address =[%p] len = [%d] string = <%s>\n", i, original_Onegin[i], strlen(original_Onegin[i]), original_Onegin[i]);
    }

    fclose(READ);
    fclose(WRITE);

    return 0;
}

void* Recalloc( void* str, size_t previous_size, size_t new_size )
{
    char* new_str = (char*) realloc(str, new_size);

    for (size_t i = previous_size; i < new_size; i++)
        new_str[i] = 0;

    return new_str;
}
void MyGetline( char** line, bool* read_status, FILE* stream )
{

    if (line == NULL  || stream == NULL)
    {
        *read_status = 0;
        return;
    }

    size_t buffer_len = 0;

    if (*line == NULL)
    {
        size_t first_size = 64;
        char *new_buf = (char*)calloc(first_size, sizeof(char));
        if (new_buf == NULL)
        {
            *read_status = 0;
            return;
        }
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
            {
                *read_status = 0;
                return;
            }
            *line = new_buf;
            buffer_len = new_size;
        }
        if (c == '\n')
        {
            (*line)[len] = '\0';
            return;
        }
        (*line)[len++] = (char) c;
    }

    *read_status = 0;
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

void Swap( void* a, void* b, size_t type_size )
{
    assert(a);
    assert(b);

    uint64_t temp64 = 0;
    uint32_t temp32 = 0;
    uint16_t temp16 = 0;
    uint8_t  temp8  = 0;

    while (type_size >= sizeof(uint64_t))
    {
        SWAP_BUFFERS_a_AND_b_SIZEOF(uint64_t, temp64);
        /*
                                                //если макрос                                        \
                                                  SWAP_BUFFERS_a_AND_b_SIZEOF(SIZE)                  \
                                                  полетит, или я случайно забуду                     \
                                                  его перекопировать в нужный мне проект,            \
                                                  то использовать эту часть кода,                    \
                                                  как руководство по экстренному восстановлению кода \
                                                  удачи)))
        temp =  *((uint64_t*)a);
        *((uint64_t*)a) = *((uint64_t*)b);
        *((uint64_t*)b) = temp;

        type_size -= sizeof(uint64_t);
        a = ((uint64_t*)a + 1);
        b = ((uint64_t*)b + 1);
        */
    }
    if (type_size >= sizeof(uint32_t))
    {
        SWAP_BUFFERS_a_AND_b_SIZEOF(uint32_t, temp32);
    }
    if (type_size >= sizeof(uint16_t))
    {
        SWAP_BUFFERS_a_AND_b_SIZEOF(uint16_t, temp16);
    }
    if (type_size >= sizeof(uint8_t))
    {
        SWAP_BUFFERS_a_AND_b_SIZEOF(uint8_t, temp8);
    }
}


int CompareStringEnc( const void* a, const void* b )
{
    assert(a);
    assert(b);

    const char* first_str = *((const char**)a);
    const char* second_str = *((const char**)b);

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

    const char* first_str = *((const char**)a);
    const char* second_str = *((const char**)b);

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
