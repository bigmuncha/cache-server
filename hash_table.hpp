#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <typeindex>
#include <functional>
#include <iostream>
#include <iomanip>

#define TABLESIZE 5

 struct node
{
    char *data;
    struct node *next;

};

unsigned int hash(const char *str, int tablesize);

int lookup(node **table, const char *key);


int set(node **table, char *key);

char* get(node **table, char*key);
