#include "union_find.h"

// union-find data structure will allow the maze generation algorithm to be optimized in execution
// time I used this tutorial to understand this data structure :
// https://www.youtube.com/watch?v=K1pAB9_l2bM and got the idea from wikipedia

node *find(node n) {
    if (n.parent->id != n.id) {
        n.parent = find(*n.parent);
    }

    return n.parent;
}

int merge(node n1, node n2) {
    node *root1 = find(n1);
    node *root2 = find(n2);
    if (root1->id == root2->id) {
        // nodes already in the same tree
        return 0;
    }

    // merging trees
    if (root1->rank > root2->rank) {
        root2->parent = root1;
    } else if (root2->rank > root1->rank) {
        root1->parent = root2;
    } else {
        root1->parent = root2;
        root2->rank++;
    }

    return 1;
}

node *allocate_vector(int dimension) {
    node *ret = malloc(sizeof(node) * dimension);
    return ret;
}

void free_vector(node *vector) {
    free(vector);
}

node **allocate_matrix(int lines, int columns) {
    node **ret = (node **)malloc(lines * sizeof(node *));
    for (int i = 0; i < lines; i++) {
        ret[i] = allocate_vector(columns);
    }
    return ret;
}

void free_matrix(node **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free_vector(matrix[i]);
    }
    free(matrix);
}