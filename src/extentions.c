#include "extentions.h"

int strlen(const char *str)
{
    int length = 0;
    while (*str != '\0')
    {
        length++;
        str++;
    }
    return length;
}

int isEqual(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return 0; // Not equal
        }
        str1++;
        str2++;
    }
    // Check if both strings have reached the null-terminator at the same time
    return (*str1 == '\0' && *str2 == '\0') ? 1 : 0;
}

int strncmp(const char *s1, const char *s2, int n)
{
    while (n > 0 && (*s1 != '\0' || *s2 != '\0'))
    {
        if (*s1 != *s2)
        {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
        n--;
    }
    return 0;
}

void strncpy(char *dest, const char *src, int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
        if (src[i] == '\0')
        {
            break;
        }
    }
}

void toLowerCase(char *c)
{
    while (*c != '\0')
    {
        if (*c >= 'A' && *c <= 'Z')
        {
            *c = *c - 'A' + 'a';
        }
        c++;
    }
}

void getFirstWord(const char *input, char *firstWord)
{
    // Adjust the size based on your needs
    int i = 0, j = 0;
    // Skip leading spaces
    while (input[i] == ' ')
    {
        i++;
    }
    // Copy characters until a space or null-terminator is encountered
    while (input[i] != ' ' && input[i] != '\0')
    {
        firstWord[j++] = input[i++];
    }
    firstWord[j] = '\0';
}
