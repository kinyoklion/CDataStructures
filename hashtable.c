#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

unsigned long djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

struct HashTable* ht_Create(int number_of_buckets)
{
    size_t table_memory_size = sizeof(struct HashTable) + number_of_buckets * sizeof(struct TableEntry*);
    struct HashTable* new_table = malloc(table_memory_size);
    memset(new_table, 0, table_memory_size);
    new_table->size = number_of_buckets;
    return new_table;
}

struct TableEntry* createEntry(const char* name, void* value)
{
    struct TableEntry* new_entry = malloc(sizeof(struct TableEntry));
    memset(new_entry, 0, sizeof(struct TableEntry));
    new_entry->name = name;
    new_entry->value = value;
    return new_entry;
}

void* ht_Insert(struct HashTable* self, const char* name, void* value)
{
    unsigned long hash = djb2_hash(name);
    size_t bucket_index = hash % self->size;
    
    struct TableEntry* bucket = self->buckets[bucket_index];

    if(bucket == 0) {
        struct TableEntry* new_entry = createEntry(name, value);
        self->buckets[bucket_index] = new_entry;
        return 0;
    }
    else
    {
        struct TableEntry* entry = bucket;
        while(strcmp(entry->name, name) != 0) {
            if(entry->next == 0) {
                struct TableEntry* new_entry = createEntry(name, value);
                entry->next = new_entry;
                return 0;
            }
            entry = entry->next;
            
            void* old_value = value;
            entry->value = value;
            return old_value;
        }
    }
}

struct TableEntry* ht_Get(struct HashTable* self, const char* name)
{
    unsigned long hash = djb2_hash(name);
    size_t bucket_index = hash % self->size;
    
    struct TableEntry* bucket = self->buckets[bucket_index];
    
    if(bucket != 0)
    {
        struct TableEntry* entry = bucket;
        while(strcmp(entry->name, name) != 0) {
            entry = entry->next;
            if(entry == 0) {
                return 0;
            }
        }
        
        return entry;
    }
    return 0;
}

void freeBucket(struct TableEntry* bucket)
{
    while(bucket->next != 0)
    {
        freeBucket(bucket->next);
    }
    free(bucket);
}

void ht_Release(struct HashTable* self)
{
    for(int bucket_index = 0; bucket_index < self->size; bucket_index++)
    {
        struct TableEntry* bucket = self->buckets[bucket_index];
        if(bucket != 0)
        {
            //TODO: Need to implement a stack so we do not need to use recursion.
            freeBucket(bucket);
        }
    }
}