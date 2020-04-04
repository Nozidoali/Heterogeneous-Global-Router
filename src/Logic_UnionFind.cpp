#include "Logic_UnionFind.h"
#include <cstdlib>

UnionFind :: UnionFind(int _size) {
    size = _size;
    parent = new int [size];
    for(int i=0;i<size;i++) {
        parent[i] = i;
    }
}

UnionFind :: ~UnionFind() {
    delete [] parent;   parent = NULL;
}

void UnionFind :: Merge(int x, int y) {
    parent[this->Find(x)] = parent[this->Find(y)];
}

int UnionFind :: Find(int x) {
    return (parent[x] == x) ? x : parent[x] = Find(parent[x]);
}

bool UnionFind :: isMerged(int x, int y) {
    return Find(x) == Find(y);
}
