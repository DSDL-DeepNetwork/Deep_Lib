#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#include "list.h"

typedef struct{
    char *key;
    char *val;
    int used;
} kvp;

// read 함수
int read_option(char *s, list *options);

// list 추가 함수
void option_insert(list *l, char *key, char *val);
void option_unused(list *l);

// find 함수
char *option_find(list *l, char *key);

int option_find_int(list *l, char *key, int def);
int option_find_int_quiet(list *l, char *key, int def);

float option_find_float(list *l, char *key, float def);
float option_find_float_quiet(list *l, char *key, float def);

#endif
