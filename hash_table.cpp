#include "hash_table.hpp"

unsigned int hash(const char *str, int tablesize)
{

    return std::hash<std::string>{}(str) % tablesize;
}

 int lookup(node **table, const char *key)
{
    unsigned index = hash(key, TABLESIZE);
    const node *it = table[index];

    // Попытка установить наличие соответствующего ключа в связанном списке
    while(it != NULL && strcmp(it->key, key) != 0)
    {
        it = it->next;
    }
    return it != NULL;
}

int set(node **table, const char *key,const char *data)
{
    if( !lookup(table, key) )
    {
        // Поиск необходимого связанного списка
        unsigned index = hash(key, TABLESIZE);
        node *new_node = (node *)malloc(sizeof *new_node);

        if(new_node == NULL)
            return 0;
        new_node->key = (char *) malloc(strlen(key)+1);
        new_node->data = (char *)malloc(strlen(data)+1);

        if(new_node->data == NULL)
            return 0;

        // Добавление нового ключа и обновление указателя на начало связанного списка
        strcpy(new_node->key, key);
        strcpy(new_node->data, data);
        new_node->next = table[index];
        table[index] = new_node;
        return 1;
    }
    return 0;
}

char* get(node **table,const char* key){
    if(lookup(table, key)){
        unsigned index = hash(key, TABLESIZE);
        const node *it = table[index];

        // Попытка установить наличие соответствующего ключа в связанном списке
        while(it != NULL && strcmp(it->key, key) != 0)
        {
            it = it->next;
        }
        return it->data;

    }
    return (char *)"CANNOT GET VALUE";
}


/*
int main(int argc, char *argv[]) {


    try {
        std::cout <<"OMAR";
        //node *arr[5]= {0};
        node **arr = (node **) malloc(1024*1024);
        node marr[20];
        //arr = marr;
        insert(arr, (char *)"omar");

        std::cout << lookup(arr, (char *)"omar");
        std::cout <<get(arr,(char *)"omar");

    } catch (std::exception& e) {
        std::cerr << "Exception " << e.what() <<std::endl;
    }


    return 0;
}
*/
