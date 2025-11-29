#pragma once
#include <string>
#include <cstring>
#include <iostream>

struct Song{
    char* title;
    char* artist;
    char* filePath;
};

struct Node{
    Song data;
    Node* next;

    Node(const char* t, const char* a, const char* p){
        
        size_t lenTitle = strlen(t) + 1;
        data.title = new char[lenTitle];
        strcpy_s(data.title, lenTitle, t);

        size_t lenArtist = strlen(a) + 1;
        data.artist = new char[lenArtist];
        strcpy_s(data.artist, lenArtist, a);

        size_t lenPath = strlen(p) + 1;
        data.filePath = new char[lenPath];
        strcpy_s(data.filePath, lenPath, p);

        next = nullptr;
    }

    ~Node(){
        if (data.title) delete[] data.title;
        if (data.artist) delete[] data.artist;
        if (data.filePath) delete[] data.filePath;
    }
};

class Playlist{
public:
    Node* head;
    
    Playlist(){ head = nullptr; }

    void addSong(const char* title, const char* artist, const char* path){
        Node* newNode = new Node(title, artist, path);
        
        if (head == nullptr){
            head = newNode;
        }
        else{
            Node* temp = head;
            while (temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    
    void clearList(){
        Node* current = head;
        while (current != nullptr){
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    bool isEmpty(){ return head == nullptr; }
};