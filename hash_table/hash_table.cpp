#include "hash_table.hpp"

unsigned int hash(const char *str, int tablesize)
{

    return std::hash<std::string>{}(str) % tablesize;
}

 int lookup(node *table, const char *key)
{
    unsigned index = hash(key, TABLESIZE);
    const node *it = &table[index];

    if(it == NULL){
        return 0;
    }
    if(strcmp(it->key, key)){
        return 1;
    }
    // Попытка установить наличие соответствующего ключа в связанном списке
    /*
    while(it != NULL && strcmp(it->key, key) != 0)
    {
        it = it->next;
    }*/
    return 0;
}

int set(node *table, const char *key,const char *data)
{
    if( !lookup(table, key) )
    {
        // Поиск необходимого связанного списка
        unsigned index = hash(key, TABLESIZE);
        //table[index] = (node *)mmap(0,1024,
          //                          prot,flag,-1,0);

        //node *new_node= (node *)mmap(0,1024,prot,flag,-1,0);
        //new_node->key = (char*)key;
       // new_node->data = (char*)data;

        //new_node.next = table[index];
        strcpy(table[index].key, key);
        strcpy(table[index].data, data);

        // Добавление нового ключа и обновление указателя на начало связанного списка
        return 1;
    }
    return 0;
}

std::string get(node *table,const char* key){
    if(lookup(table, key)){
        std::cout <<"EE bla";
        unsigned index = hash(key, TABLESIZE);
        //const node *it = table[index];

        //std::cout << "\nin get func > " << it->data <<'\n';

        // Попытка установить наличие соответствующего ключа в связанном списке
/*
        while(it != NULL && strcmp(it->key, key) != 0)
        {
            it = it->next;
        }*/
        //std::cout << "\nin get func > " << it->data <<'\n';
        return table[index].data;

    }
    return "CANNOT GET VALUE";
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
