#ifndef LOGIC_UNIONFIND_H
#define LOGIC_UNIONFIND_H

class UnionFind {

public:
    int * parent;
    int size;
    UnionFind(int _size);
    ~UnionFind();
    void Merge(int x,int y);
    int Find(int x);
    bool isMerged(int x, int y);
};

#endif