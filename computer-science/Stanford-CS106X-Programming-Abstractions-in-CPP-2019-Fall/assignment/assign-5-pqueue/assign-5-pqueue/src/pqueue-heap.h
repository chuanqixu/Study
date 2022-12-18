#pragma once
#include "pqueue.h"
#include <string>
#include "vector.h"
#include <cstddef>

class HeapPQueue : public PQueue {
public:
	HeapPQueue();
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    
private:
    // provide data methods and helper methods to
    // help realize the binary heap-backed PQueue
    std::string *collection_ptr;
    size_t capacity; // how large the array is. Notice that this is not the same as logSize,
                    // which specifies how many items are in the array now

    void heapify(size_t idx); // heapify the element at idx
};
