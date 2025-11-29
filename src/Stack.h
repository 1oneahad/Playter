#pragma once
#include "LinkedList.h" // We need access to the Node struct

struct StackNode {
    Node* playlistNode; // Holds a pointer to the song in the actual playlist
    StackNode* next;
};

class HistoryStack {
public:
    StackNode* topNode;

    HistoryStack() { topNode = nullptr; }

    // Push a song to history
    void push(Node* songNode) {
        StackNode* newStackNode = new StackNode;
        newStackNode->playlistNode = songNode;
        newStackNode->next = topNode;
        topNode = newStackNode;
    }

    // Get the last song and remove it from history
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