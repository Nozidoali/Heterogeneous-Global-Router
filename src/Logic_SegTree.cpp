#include "Logic_SegTree.h"

#define ROOT 1

SegmentTree :: SegmentTree(int _size) {
    size = _size;
    sum = new int [size<<2];
    lazy = new int [size<<2];
    for(int i=0;i<size<<1;i++) {
        sum[i] = 0;
        lazy[i] = 0;
    }
}


SegmentTree :: SegmentTree(int _size, int * val) {
    size = _size;
    sum = new int [size<<2];
    lazy = new int [size<<2];
    if (val != NULL) {
        build(1, size, ROOT, val);
        return;
    }
    for(int i=0;i<size;i++) {
        sum[i] = 0;
        lazy[i] = 0;
    }
}

SegmentTree :: ~SegmentTree() {
    delete [] sum;  sum = NULL;
    delete [] lazy; lazy = NULL;
}

int SegmentTree :: Sum(int left, int right) {
    if (left < right) {
        return query(left+1, right, 1, size, ROOT);
    }
    if (left > right) {
        return query(right+1, left, 1, size, ROOT);
    }
    else {
        return 0;
    }
}

void SegmentTree :: Add(int left, int right, int val) {
    if (left < right) {
        return update(left+1, right, 1, size, ROOT, val);
    }
    if (left > right) {
        return update(right+1, left, 1, size, ROOT, val);
    }
    else {
        return;
    }
}

int SegmentTree :: query(int left, int right, int l, int r, int index) {
    if (left <= l && r <= right) {
        return sum[index];
    }
    pushDown(index, r-l+1);
    int m = (l+r)>>1;
    int ret = 0;
    if (left<=m)
        ret += query(left, right, l, m, index<<1);
    if (m<right)
        ret += query(left, right, m+1, r, index<<1|1);
    return ret;
}

void SegmentTree :: pushUp(int index) {
    sum[index] = sum[index<<1] + sum[index<<1|1];
}

void SegmentTree :: pushDown(int index, int length) {
    if (lazy[index]!=0) {
        // if the relationship is addition 
        // lazy[index<<1] += lazy[index];
        // lazy[index<<1|1] += lazy[index];

        // if the relationship is modify
        lazy[index<<1] = lazy[index<<1|1] = lazy[index];

        // if the relationship is addition
        // sum[index<<1] += lazy[index]*(length-(length>>1));
        // sum[index<<1|1] += lazy[index]*(length>>1);
        
        // if the relationship is modify
        sum[index<<1] = lazy[index]*(length-(length>>1));
        sum[index<<1|1] = lazy[index]*(length>>1);
        
        lazy[index] = 0;
    }
}

void SegmentTree :: update(int left, int right, int l, int r, int index, int val) {
    if (left <= l && r <= right) {
        // if the relationship is relation
        // lazy[index] += val;
        // sum[index] += val*(r-l+1);

        // if the relationship is modification
        lazy[index] = val;
        sum[index] = val*(r-l+1);
        return;
    }
    pushDown(index, r-l+1);
    int m = (l+r)>>1;
    if (left <= m)
        update(left, right, l, m, index<<1, val);
    if (m < right)
        update(left, right, m+1, r, index<<1|1, val);
    pushUp(index);
}

void SegmentTree :: build(int left, int right, int index, int * val) {
    lazy[index] = 0;
    if (left==right) {
        sum[index] = val[index];
        return;
    }
    int m = (left+right)>>1;
    build(left,m,index<<1,val);
    build(m+1,right,index<<1|1,val);
    pushUp(index);
}