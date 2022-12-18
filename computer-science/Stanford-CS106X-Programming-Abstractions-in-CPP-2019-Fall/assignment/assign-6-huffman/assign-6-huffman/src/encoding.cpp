// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
using namespace std;
// TODO: include any other headers you need

#include "priorityqueue.h" // for PriorityQueue
#include "filelib.h" // for rewindStream

Map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    Map<int, int> freqTable;   // this is just a placeholder so it will compile

    // read the input stream until EOF
    rewindStream(input);
    int c = input.get();
    while (!input.eof()) {
        if (freqTable.containsKey(c)) {
            ++freqTable[c];
        } else {
            freqTable[c] = 1;
        }
        c = input.get();
    }

    // add the PSEUDO_EOF
    freqTable[PSEUDO_EOF] = 1;

    return freqTable;          // this is just a placeholder so it will compile
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // TODO: implement this function
//    return NULL;   // this is just a placeholder so it will compile

    // add all items into the priority queue
    PriorityQueue<HuffmanNode *> pq;
    for (const auto &key : freqTable.keys()) {
        auto node = new HuffmanNode(key, freqTable[key], nullptr, nullptr);
        pq.enqueue(node, freqTable[key]);
    }

    // build the encoding tree
    while (pq.size() > 1) {
        auto left = pq.dequeue(), right = pq.dequeue();
        int count  = left->count + right->count;
        auto head = new HuffmanNode(NOT_A_CHAR, count, left, right);
        pq.enqueue(head, count);
    }

    return pq.dequeue();
}

void buildEncodingMapHelper(HuffmanNode *encodingTree, Map<int, string> &encodingMap, string prefix) {
    if (!encodingTree) {
        return;
    }
    if (encodingTree->character != NOT_A_CHAR) {
        encodingMap[encodingTree->character] = prefix;
    } else {
        buildEncodingMapHelper(encodingTree->zero, encodingMap, prefix + "0");
        buildEncodingMapHelper(encodingTree->one, encodingMap, prefix + "1");
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    Map<int, string> encodingMap;   // this is just a placeholder so it will compile

    buildEncodingMapHelper(encodingTree, encodingMap, "");

    return encodingMap;             // this is just a placeholder so it will compile
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    // TODO: implement this function

    rewindStream(input);

    // read and write
    int c = input.get();
    while (!input.eof()) {
        for (const auto &bit : encodingMap[c]) {
            output.writeBit(bit - '0');
        }
        c = input.get();
    }
    for (const auto &bit : encodingMap[PSEUDO_EOF]) {
        output.writeBit(bit - '0');
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function

    auto curr = encodingTree;
    int c = input.readBit();
    while (!input.fail()) {
        if (c == 0) {
            curr = curr->zero;
        } else {
            curr = curr->one;
        }
        if (curr->character != NOT_A_CHAR) {
            if (curr->character == PSEUDO_EOF) {
                return;
            }
            output.put(curr->character);
            curr = encodingTree;
        }
        c = input.readBit();
    }
}

void compress(istream& input, obitstream& output) {
    // TODO: implement this function

    auto frequency_table = buildFrequencyTable(input);
    auto encoding_tree = buildEncodingTree(frequency_table);
    auto encoding_map = buildEncodingMap(encoding_tree);

    output << frequency_table;
    encodeData(input, encoding_map, output);
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function

    Map<int, int> frequency_table;
    input >> frequency_table;

    auto encoding_tree = buildEncodingTree(frequency_table);

    decodeData(input, encoding_tree, output);
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function

    if (!node) {
        return;
    }

    freeTree(node->zero);
    freeTree(node->one);

    delete node;
}
