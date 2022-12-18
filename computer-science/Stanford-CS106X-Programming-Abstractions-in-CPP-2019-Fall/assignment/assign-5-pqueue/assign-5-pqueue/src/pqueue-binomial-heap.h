#pragma once
#include "pqueue.h"
#include <string>
#include "vector.h"

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;

private:
    // provide data methods and helper methods to
    // help realize the binomial heap-backed PQueue

    // Node is the class storing each binomial heap
    struct Node {
        std::string elem;
        Vector<Node *> children;

        Node(std::string elem): elem(elem) {}
        size_t bit_idx() {return children.size();}

    };

    static Node *add_nodes(Node *a, Node *b) {
        if (a->bit_idx() != b->bit_idx()) {
            throw "To add two binomial heaps, they must have the same size!";
        }

        if (a->elem < b->elem) {
            a->children.add(b);
            return a;
        } else {
            b->children.add(a);
            return b;
        }
    }

    // HeapBit is the class storing each bit (binomial heap) in the heap
    struct HeapBit {
        Node *head;
        HeapBit *next;
        HeapBit(Node *head, HeapBit *next = nullptr): head(head), next(next) {}
    };

    // add binomial heap to the location of the heap
    void add_binomial_heap(HeapBit *curr, Node *carry);

    HeapBit *sentinel;
};

