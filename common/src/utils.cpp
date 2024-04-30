#include "utils.h"

const int ARGS_NUM = 3;

Translation HandleCommandLineArgs (int args_num, const char** arguments)
{
    assert (arguments);

    if (args_num != ARGS_NUM)
        return Translation::ERR;

    if (strcmp (arguments[1], "-S") == 0)
        return Translation::ASM;

    if (strcmp (arguments[1], "-E") == 0)
        return Translation::ELF;

    return Translation::ERR;
}

char* GetFileContent(const char* filename)
{
    assert(filename);

    FILE* file = fopen(filename, "rb");

    int file_size = GetFileSize(file);

    printf (file_size);

    char* buffer = (char*) calloc(file_size + 1, sizeof(char));
    assert(buffer);

    buffer[file_size] = '\0';
    fread(buffer, sizeof(char), file_size, file);

    fclose(file);

    return buffer;
}

int GetFileSize(FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    int position = ftell(file);
    fseek(file, 0, SEEK_SET);

    return position;
}

int CalcNlines(char* buffer)
{
    assert(buffer);

    int nlines = 0;
    char* temp = buffer;

    while ((temp = strchr(temp, '\n')) != nullptr)
    {
        nlines++;
        temp++;
    }

    return nlines;
}

bool IsEqual(double a, double b)
{
    const double EPS = 1e-10;
    return fabs(a - b) < EPS;
}

int SkipSpaces (char* buffer, int i)
{
    assert (buffer);

    while (isspace (buffer[i])) i++;

    return i;
}

int Factorial (int n)
{
    if (n == 1 || n == 0) return 1;
    return n * Factorial (n - 1);
}

bool IsInt (double n)
{
    return IsEqual (n, round (n));
}

int PrintCommand (FILE* file, const char* command, ...)
{
    assert (file);

    fprintf (file, "\t\t");

    va_list args = {};
    va_start (args, command);

    int n_symbols = vfprintf (file, command, args);

    va_end (args);

    return n_symbols;
}
