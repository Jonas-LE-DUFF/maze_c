#include <stddef.h>
#include <stdlib.h>

struct nodestr {
    int id;
    int rank;
    struct nodestr *parent;
};

typedef struct nodestr node;

node *find(node n);
int merge(node root, node merged);

node *allocate_vector(int dimension);
void free_vector(node *vector);
node **allocate_matrix(int lines, int columns);
void free_matrix(node **matrix, int n);