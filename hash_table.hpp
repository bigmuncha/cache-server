#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <typeindex>
#include <functional>
#include <iostream>
#include <iomanip>

#define TABLESIZE 100

 struct node
{

    char *key;
    char *data;
    int ttl;
    struct node *next;

};

unsigned int hash(const char *str, int tablesize);

int lookup(node **table, const char *key);


int set(node **table, char *key, char *data);

char* get(node **table, char*key);
