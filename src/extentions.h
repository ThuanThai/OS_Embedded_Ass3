#include "mbox.h"
#include "uart.h"

#define MAX_WORDS 10
#define MAX_WORD_LEN 20

#define array_length(arr) (sizeof(arr) / sizeof(arr[0]))

int strlen(const char *str);
int strncmp(const char *s1, const char *s2, int length);
void strncpy(char *dest, const char *src, int n);
int isEqual(const char *str1, const char *str2);
void getFirstWord(const char *input, char *firstWord);
void toLowerCase(char *c);
