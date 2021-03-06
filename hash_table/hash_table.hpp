#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>

#include <typeindex>
#include <functional>
#include <iostream>
#include <iomanip>

#define TABLESIZE 30

 struct node
{

    char *key;
    char *data;
    //int ttl;
    //struct node *next;

};

#define  prot   PROT_READ | PROT_WRITE
#define flag   MAP_SHARED|MAP_ANONYMOUS


unsigned int hash(const char *str, int tablesize);

int lookup(node *table, const char *key);


int set(node *table,const char *key, const char *data);

std::string get(node *table, const char*key);

#endif
