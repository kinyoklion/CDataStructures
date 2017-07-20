//Struct used to store a hash table.
struct HashTable {
    int size;
    struct TableEntry* buckets[];
};

//Struct used to store individual entries in a hash table.
struct TableEntry {
    struct TableEntry* next;
    const char* name;
    void* value;
};

//Create a hash table. Will have the specified number of buckets.
struct HashTable* ht_Create(int number_of_buckets);

//Add something to a hash table. If a value was previously set, then it will be
//returned.
void* ht_Insert(struct HashTable* self, const char* name, void* value);

//Get something from a hash table. Returns 0 if the item is not found.
struct TableEntry* ht_Get(struct HashTable* self, const char* name);

//Delete the memory associated with the hash table.
void ht_Release(struct HashTable* self);

//TODO: Need to implement remove.
