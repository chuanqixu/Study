# Introduction

[Assignment description](./25-Assignment-5-PQueue.pdf).

This assignment uses 4 data structures to implement the priority queue:

1. Unsorted vector
2. Binary heap
3. Sorted doubly linked list
4. [Binomial heap](https://en.wikipedia.org/wiki/Binomial_heap)



# Note


1. Remember to change `logSize` when implementing `enqueue()`, `extractMin()`, and `merge()`. Also, remember to check size when implementing `peek()` and `extractMin()`.
2. The definition for `peek()` is `virtual const std::string& peek() const = 0;`. Notice that it is defined to return a reference, which means in its implementation, it may be better to get the index of the smallest element instead of getting the smallest element itself. This is because the smallest element itself is a local variable and will be destroyed after the function ends, while with the index, we can return the smallest element with the member access operator because this operator returns an lvalue reference.
3. Can specify which part to be tested at lines 203 - 206 in `pqueue-test.cpp`. After finishing and testing each part, comment out that part so that do not need to wait every time run the program.


# Implementation 1: Unsorted Vector

This is a simple implementation. The implementations of member functions are:

1. `peek()`: simply transverse every item and find the smallest one. Remember to return a reference.
2. `extractMin()`: call `peek()` to get the smallest element, and delete it from the `Vector`. Also, don't forget to decrease `logSize`.
3. `enqueue()`: directly add the element to the `Vector`. Also, don't forget to increase `logSize`.
4. `merge()`: directly add two `Vector`s and assign the result to the `Vector` of the new `VectorPQueue`. Also, don't forget to specify `logSize`.

The `Vector` can be both dynamically allocated, or just allocate it locally. Because in the following implementations, they use dynamically allocated collections, I also use dynamically allocated `Vector` in this implementation. Therefore, the constructor and destructor need some code for memory allocation and deallocation. In `merge()`, also need to delete the previous two `VectorPQueue`.


# Implementation 2: Binary Heap

Notice that this assignment requires implementing the heap with the array. Do not use `Vector<string>` to store the items.

Because `merge()` needs to heapify all layers except the last layer, it is better to first implement `heapify(int idx)` that heapifies starting from `idx`, so that both `merge()` and `extractMin()` can use.

In each iteration in `heapify()`, get the smallest child between the left and right child (or directly the left child if there is no right child). If this child is smaller than the current node, then swap them. Otherwise, the heap has been heapified already. Note that this assumption needs the requirement that both the left and right child (if they exist) should be already a heap. Therefore, in `merge()`, we need to do `heapify()` bottom up instead of top down.

The code for `heapify()` is below:

```cpp
void HeapPQueue::heapify(size_t idx) {
    size_t curr = idx, min_child_idx = 2 * curr + 1;

    do {
        // get the smallest chile index
        if (min_child_idx + 1 < logSize) { // has both children
            if (collection_ptr[min_child_idx] > collection_ptr[min_child_idx + 1]) {
                ++min_child_idx;
            }
        } else if (min_child_idx >= logSize) { // no child
            return;
        }

        // if element at curr is larger than any child, the process has finished
        if (collection_ptr[curr] <= collection_ptr[min_child_idx]) {
            return;
        }
        // otherwise, swap them
        swap(collection_ptr[curr], collection_ptr[min_child_idx]);

        // update to the next level
        curr = min_child_idx; // curr goes to the smaller child
        min_child_idx = 2 * min_child_idx + 1; // min_child_idx goes to its left child
    } while (min_child_idx < logSize);
}
```

The implementations of other member functions are:

1. As mentioned earlier, it requires to use the array to implement the heap. So we need one additional data member `capacity` to keep track of the length of the array. Also, need to pay attention to the allocation and deallocation of the array.
2. `peek()`: directly return the reference to the first item in the array.
3. `extractMin()`: call `peek()` to get the smallest element, and call `heapify(0)`. Also, don't forget to decrease `logSize`.
4. `enqueue()`: directly add the element to the array. Need to resize the array if there is not enough space. Also, don't forget to increase `logSize`.
5. `merge()`: follow the instructions in the handout. First, combine two arrays, and then heapify every node layer by layer from the last second layer to the top of the heap. Also, don't forget to specify `logSize` and `capacity`.




# Implementation 3: Sorted doubly linked list

This is a simple implementation. Need to pay attention to two pointers in the doubly linked list. I also implemented [sentinel](https://en.wikipedia.org/wiki/Sentinel_value) for the doubly linked list.


The implementations of member functions are:

1. `peek()`: directly return the reference to the first item in the list.
2. `extractMin()`: call `peek()` to get the smallest element, and delete the first item in the list. Also, don't forget to decrease `logSize`.
3. `enqueue()`: directly add the element to the list. Also, don't forget to increase `logSize`.
4. `merge()`: directly apply the merge step in [merge sort](https://en.wikipedia.org/wiki/Merge_sort). Also, don't forget to specify `logSize` and `capacity`.

# Implementation 4: Binomial Heaps

There are several concepts:

1. Node: a class storing the string.

2. Binomial heap: a binomial heap of order $k$ stores $2^k$ nodes. In this assignment, using the root node to store the heap. The `Node` below is different from the node above in that it stores its children. To make the addition more efficient, I directly used the same memory by changing the pointer. So the destructor of `Node` is the default destructor, which does not free the `Vector` pointed by `children`. The code for `Node` is:

    ```cpp
    // Node is the class storing each binomial heap
    struct Node {
        std::string elem;
        Vector<Node *> children;

        Node(std::string elem): elem(elem) {}
        size_t bit_idx() {return children.size();}

    };
    ```

3. Binomial Heap Priority Queue: priority queue backed by the binomial heap. The class is what we need to implement. A series of binomial heaps need to be used to represent each "bit" according to the handout. I used a linked list to store it:

    ```cpp
    // HeapBit is the class storing each bit (binomial heap) in the heap
    struct HeapBit {
        Node *head;
        HeapBit *next;
        HeapBit(Node *head, HeapBit *next = nullptr): head(head), next(next) {}
    };
    ```

Because `enqueue()` and `extractMin()` need the operation like `merge()`, it is better to first implement `merge()`. With  `merge()` implemented, `enqueue()` can be implemented as merging with a `BinomialHeapPQueue` with only one node at the index 0. `extractMin()` can be implemented to delete the `BinomialHeap` whose root is the min string from the `BinomialHeapPQueue`, and then merging with a `BonomialHeapPQueue` which consists of all the previous children. This can greatly simplify the implementation. However, because `merge()` is a general operation, while `enqueue()` and `extractMin()` have their own special cases, and they can be done in-place while `merge()` is not implemented in an in-place manner. Rather than directly calling `merge()`, I implemented them considering the special cases.

Another thing is that although it is much simpler to store all `BinomialHeap` of `BinomialHeapPQueue` in one `Vector<Node *>`, the `BinomialHeapPQueue` objects may be very sparse, where a `Vector<Node *>` will waster a lot of space. In addition, there is no need to do random accessing in these methods. Therefore, I implemented a linked list to store `BinomialHeap` and also used the sentinel.

Before starting, I implemented a static helper member function to add two `Node`s:

```cpp
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
```

Consider the addition between two `BinomialHeapPQueue`. The process is to add three `BinomialHeap` at each "bit", the left operand `one`, the right operand `two`, and the carry. There are 3 conditions:

1. No item at the bit: nothing happens.
2. One item at the bit: this item can be directly added to this bit.
3. Two items at the bit: the result has no item at the bit, but one carry with the higher bit needs to be considered at the next bit.
4. Three items at the bit: the result has one item at the bit, and this item can be __any__ one of these three items. The other two will be added to form the carry with the higher bit that needs to be considered at the next bit.

Consider the simplest case, I implemented below helper function to add one `BinomialHeap` to a specific bit:

```cpp
void BinomialHeapPQueue::add_binomial_heap(HeapBit *curr, Node *carry) {
    if (!curr || !carry || (curr->head && curr->head->bit_idx() > carry->bit_idx())) {
        throw "Cannot add the binomial heap after curr!";
    }
    while (carry) {
        if (!curr->next || curr->next->head->bit_idx() > carry->bit_idx()) { // only one item at this bit: directly add it
            curr->next = new HeapBit(carry, curr->next);
            break;
        } else { // only two items: no item for this location, but one carry for the next bit
            carry = add_nodes(curr->next->head, carry);
            auto trash = curr->next;
            curr->next = curr->next->next;
            delete trash;
        }
    }
}
```

Because I used a linked list to store the `BinomialHeap`, when adding a `BinomialHeap` into the linked list, we need to keep track of its previous node so that we can add the pointer to point to the new node of the result, and we also need to check if there is any item at that bit.


1. `peek()`: return the smallest string in all `BinomialHeap`. The code is:

    ```cpp
    const string& BinomialHeapPQueue::peek() const {
        // placeholder so method compiles..
        // replace with your own implementation
    //    return kEmptyString;

        if (logSize == 0) {
            throw "No item in the BinomialHeapPQueue!";
        }

        auto min_heapbit = sentinel->next;
        auto curr = sentinel->next->next;
        while (curr) {
            if (curr->head->elem < min_heapbit->head->elem) {
                min_heapbit = curr;
            }
            curr = curr->next;
        }
        return min_heapbit->head->elem;
    }
    ```

2. `extractMin()`: the special case is that there are items in all bits before the bit of the minimum string. Suppose there are $n$ bits to be added, i.e., the `BinomialHeap` whose root is the minimum string has $n$ children. These $n$ children form a `BinomialHeapPQueue` whose bits from 0 to $n-1$ are all 1. And suppose the non-zero bit of the `BinomialHeapPQueue` is $k$. According to all 4 cases listed above, at bits from 0 to $k-1$, because there are only items to be added, we directly add them. At bit $k$, because there are two items to be added, it means this location will be no item after addition, and one carry at the higher bit needs to be considered. At bits from $k+1$ to $n$, the carry will be continuously moved to higher bits because there are at least two items at each bit. We can only compute the carry and keep the original `BinomialHeapPQueue` the same. The code is below:

    ```cpp
    string BinomialHeapPQueue::extractMin() {
        // placeholder so method compiles..
        // replace with your own implementation
        //  return peek();

        string min_str = peek();
        --logSize;

        // end_ptr points to the previous element of the min string
        auto end_ptr = sentinel;
        while (end_ptr->next) {
            if (end_ptr->next->head->elem == min_str) {
                break;
            }
            end_ptr = end_ptr->next;
        }

        auto trash = end_ptr->next;
        // if the min string is at bit 0, directly remove it
        if (end_ptr->next->head->bit_idx() == 0) {
            end_ptr->next = end_ptr->next->next;
            delete trash;
            return min_str;
        }

        // for bits from 0 to max_bit, compute the carry
        // 1. from 0 to the bit before the non-zero bit of the original
        //  heap: because there is only the binomial heap to be added,
        //  add all them to the original heap
        // 2. at the non-zero bit of the original heap: compute the
        //  carry and remove the original binomial heap
        // 3. starting from the non-zero bit of the original heap:
        //  there is carry, because there is also a binomial heap to
        //  be added, compute the carry and keep the original heap
        //  the same
        size_t max_bit = end_ptr->next->head->bit_idx() - 1;
        Vector<Node *> *heads = &(end_ptr->next->head->children);
        end_ptr->next = end_ptr->next->next;
        delete trash;

        auto curr = sentinel;
        Node *carry = nullptr;

        for (size_t idx = 0; idx <= max_bit; ++idx) {
            if (carry) { // from the non-zero bit of the original heap to to end of the binomial heap to add
                carry = add_nodes(heads->get(idx), carry);
            } else {
                if (!curr->next || curr->next->head->bit_idx() > idx) { // from 0 to the bit before the non-zero bit of the original heap
                    curr->next = new HeapBit(heads->get(idx), curr->next);
                    curr = curr->next;
                } else { // at the non-zero bit of the original heap
                    carry = add_nodes(curr->next->head, heads->get(idx));
                    trash = curr->next;
                    curr->next = curr->next->next;
                    delete trash;
                }
            }
        }

        // if there is carry
        if (carry) {
            // get the location after which to add the carry
            curr = sentinel;
            while (curr->next && curr->next->head->bit_idx() < carry->bit_idx()) {
                curr = curr->next;
            }

            // add the carry
            add_binomial_heap(curr, carry);
        }

        return min_str;
    }
    ```


3. `enqueue()`: the special case is that it only needs to add one node, so it can save a little to write a code for this case compared with creating a `BinomialHeapPQueue` and doing `merge()`. Because the carry can be continuously added up, need a loop to consider its case. Difference from `merge()`, because `enqueue()` only adds one node, it means if one `BinomialHeap` is added to the `BinomialHeapQueue`, it can stop. According to all 4 cases listed above, this case happens when there is only the carry (if we view the enqueued node as the carry at bit 0 at the beginning). Otherwise, if there is one item at the bit of the carry, the new carry at the higher bit needed to be computed and this item needs to be removed. Also, don't forget to increase `logSize`. The code is:

    ```cpp
    void BinomialHeapPQueue::enqueue(const string &elem /* elem */) {
        // placeholder so method compiles..
        // replace with your own implementation

        add_binomial_heap(sentinel, new Node(elem));
        ++logSize;
    }
    ```

4. `merge()`: follow the above 4 cases when adding two `BinomialHeapPQueue`.

    ```cpp
    BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one /* one */, BinomialHeapPQueue *two /* two */) {
        // placeholder so method compiles..
        // replace with your own implementation
    //  return new BinomialHeapPQueue();

        auto merged_BinomialHeapPQueue_ptr = new BinomialHeapPQueue;
        merged_BinomialHeapPQueue_ptr->logSize = one->logSize + two->logSize;

        // add two to one
        // similar to the merge step in merge sort
        HeapBit *curr = merged_BinomialHeapPQueue_ptr->sentinel,
                *curr_one = one->sentinel->next,
                *curr_two = two->sentinel->next;
        Node *carry = nullptr;

        while (curr_one && curr_two) {
            if (carry) {
                if (curr_one->head->bit_idx() > carry->bit_idx() && curr_two->head->bit_idx() > carry->bit_idx()) { // only one item at this bit: directly add it
                    curr->next = new HeapBit(carry);
                    carry = nullptr;
                    curr = curr->next;
                } else if (curr_one->head->bit_idx() == carry->bit_idx() && curr_one->head->bit_idx() == curr_two->head->bit_idx()) { // three items at this bit: keep the carry and compute the next carry equal to one + two
                    auto next_carry = add_nodes(curr_one->head, curr_two->head);
                    curr->next = new HeapBit(carry);
                    carry = next_carry;

                    curr = curr->next;
                    curr_one->head = nullptr;
                    curr_two->head = nullptr;
                    curr_one = curr_one->next;
                    curr_two = curr_two->next;
                } else if (curr_one->head->bit_idx() == carry->bit_idx()) { // only two items: no item for this location, but one carry for the next bit
                    carry = add_nodes(curr_one->head, carry);
                    curr_one->head = nullptr;
                    curr_one = curr_one->next;
                } else if (curr_two->head->bit_idx() == carry->bit_idx()){ // only two items: no item for this location, but one carry for the next bit
                    carry = add_nodes(curr_two->head, carry);
                    curr_two->head = nullptr;
                    curr_two = curr_two->next;
                }
            } else {
                if (curr_one->head->bit_idx() == curr_two->head->bit_idx()) { // only two items: no item for this location, but one carry for the next bit
                    carry = add_nodes(curr_one->head, curr_two->head);
                    curr_one->head = nullptr;
                    curr_two->head = nullptr;
                    curr_one = curr_one->next;
                    curr_two = curr_two->next;
                } else if (curr_one->head->bit_idx() < curr_two->head->bit_idx()) { // only one item at this bit: directly add it
                    curr->next = new HeapBit(curr_one->head);
                    curr_one->head = nullptr;
                    curr_one = curr_one->next;
                } else { // only one item at this bit: directly add it
                    curr->next = new HeapBit(curr_two->head);
                    curr_two->head = nullptr;
                    curr_two = curr_two->next;
                }
            }
        }

        // consider the remaining parts in two operands
        while(curr_one) {
            if (carry) {
                if (curr_one->head->bit_idx() > carry->bit_idx()) {
                    curr->next = new HeapBit(carry);
                    carry = nullptr;
                    curr = curr->next;
                } else {
                    carry = add_nodes(curr_one->head, carry);
                    curr_one->head = nullptr;
                    curr_one = curr_one->next;
                }
            } else {
                curr->next = new HeapBit(curr_one->head);
                curr_one->head = nullptr;
                curr_one = curr_one->next;
            }
        }

        while(curr_two) {
            if (carry) {
                if (curr_two->head->bit_idx() > carry->bit_idx()) {
                    curr->next = new HeapBit(carry);
                    carry = nullptr;
                    curr = curr->next;
                } else {
                    carry = add_nodes(curr_two->head, carry);
                    curr_two->head = nullptr;
                    curr_two = curr_two->next;
                }
            } else {
                curr->next = new HeapBit(curr_two->head);
                curr_two->head = nullptr;
                curr_two = curr_two->next;
            }
        }

        // consider the last carry
        if (carry) {
            curr->next = new HeapBit(carry);
        }

        delete one;
        delete two;

        return merged_BinomialHeapPQueue_ptr;
    }

    ```