#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
typedef struct TableEntry* EntryPointer;
#define TYPE EntryPointer
#include "vector.h"
#undef TYPE

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
    new_entry->next = 0;
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

void ht_Release(struct HashTable* self)
{
    struct Vector_EntryPointer* entry_vector = vec_CreateDefault_EntryPointer();
    
    //Put all of the buckets on a stack to be deleted.
    for(int bucket_index = 0; bucket_index < self->size; bucket_index++)
    {
        
        struct TableEntry* bucket = self->buckets[bucket_index];
        if(bucket != 0)
        {
            vec_Push_EntryPointer(entry_vector, bucket);
            
            while(bucket->next != 0)
            {
                bucket = bucket->next;
                vec_Push_EntryPointer(entry_vector, bucket);
            }
        }
    }
    
    //Delete all the buckets from the stack.
    while(vec_GetSize_EntryPointer(entry_vector) != 0)
    {
        free(vec_Pop_EntryPointer(entry_vector));
    }

    vec_Release_EntryPointer(entry_vector);
    free(self);
}