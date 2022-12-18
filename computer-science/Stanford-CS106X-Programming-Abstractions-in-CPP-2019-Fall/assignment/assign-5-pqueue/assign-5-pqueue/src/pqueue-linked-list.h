#pragma once
#include "pqueue.h"
#include <string>

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
	// provide data methods and helper methods to
    // help realize the linked list-backed PQueue

    struct Node {
        Node(std::string str, Node *prev, Node *next): str(str), prev(prev), next(next) {}

        std::string str;
        Node *prev;
        Node *next;
    };

    Node *sentinel;
};
