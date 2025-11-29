#pragma once
#include "LinkedList.h"
#include <string.h>


const int TABLE_SIZE = 100; 

struct HashNode {
    char* key; // the song title
    Node* value; // pointer to the song node in the playlist
    HashNode* next; // pointer to the next node in case of collision
};

class SongMap {
    HashNode* table[TABLE_SIZE]; // buckets

    // hashing song title to an index on the bucket table
    unsigned int hashFunction(const char* key) {
        unsigned int hash = 0;
        
        for (int i = 0; key[i] != '\0'; i++)
            hash = 31 * hash + key[i];
        
        return hash % TABLE_SIZE; // modulus to not go outside table size
    }

public:

    SongMap() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }


    void insert(const char* title, Node* songNode) {
        int index = hashFunction(title);
        
        HashNode* newNode = new HashNode;
        
        long long int len = strlen(title) + 1;
        newNode->key = new char[len];
        strcpy_s(newNode->key, len, title); // copy title into key
        
        newNode->value = songNode;
        
        newNode->next = table[index]; // incase of collision, point to existing node
        table[index] = newNode;
    }


    Node* search(const char* title) {
        
        int index = hashFunction(title);
        
        HashNode* entry = table[index];

        while (entry != nullptr) {
            
            if (strcmp(entry->key, title) == 0) {
                return entry->value;
            }
            entry = entry->next; // traverse the chain until we find the song
        }
        
        return nullptr; // if not found
    }
};