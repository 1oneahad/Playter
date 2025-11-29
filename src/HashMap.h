#pragma once
#include "LinkedList.h"
#include <string.h>

// Simple hash table size
const int TABLE_SIZE = 100; 

struct HashNode {
    char* key; // Song Title
    Node* value; // Pointer to Playlist Node
    HashNode* next; // For collisions
};

class SongMap {
    HashNode* table[TABLE_SIZE];

    // Simple Hash Function
    unsigned int hashFunction(const char* key) {
        unsigned int hash = 0;
        for (int i = 0; key[i] != '\0'; i++)
            hash = 31 * hash + key[i];
        return hash % TABLE_SIZE;
    }

public:
    SongMap() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    void insert(const char* title, Node* songNode) {
        int index = hashFunction(title);
        HashNode* newNode = new HashNode;
        
        // Allocate string key
        size_t len = strlen(title) + 1;
        newNode->key = new char[len];
        strcpy_s(newNode->key, len, title);
        
        newNode->value = songNode;
        newNode->next = table[index]; // Insert at front of bucket
        table[index] = newNode;
    }

    Node* search(const char* title) {
        int index = hashFunction(title);
        HashNode* entry = table[index];

        while (entry != nullptr) {
            if (strcmp(entry->key, title) == 0) {
                return entry->value;
            }
            entry = entry->next;
        }
        return nullptr; // Not found
    }
};