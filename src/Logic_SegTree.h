#ifndef LOGIC_SEGTREE_H
#define LOGIC_SEGTREE_H

#include <cstdlib>
using namespace std;

class SegmentTree {
private:
    int size;
    void update(int left, int right, int l, int r, int index, int val);
    void build(int left, int right, int index, int * val);
    void pushUp(int index);
    void pushDown(int index, int length);
    int query(int left, int right, int l, int r, int index);
    int * sum;
    int * lazy;
public:
    SegmentTree(int _size, int * val);
    SegmentTree(int _size);
    ~SegmentTree();
    void Add(int left, int right, int val);
    int Sum(int left, int right);
};

#endif