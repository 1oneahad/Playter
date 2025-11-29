#pragma once
#include "LinkedList.h"


struct StackNode {
    Node* playlistNode; // Pointer to the song in the real playlist
    StackNode* next; // Pointer to the next item in the history pile
};

class HistoryStack {
public:
    StackNode* topNode;

    HistoryStack() { topNode = nullptr; }

    void push(Node* songNode) {
        StackNode* newStackNode = new StackNode;
        
        newStackNode->playlistNode = songNode;
        newStackNode->next = topNode;
        topNode = newStackNode;
    }


    Node* pop() {
        if (topNode == nullptr) return nullptr;

        Node* songNode = topNode->playlistNode;
        StackNode* temp = topNode;
        
        topNode = topNode->next;
        
        delete temp;
        return songNode;
    }

    bool isEmpty() { return topNode == nullptr; }
};