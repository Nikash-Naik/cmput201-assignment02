// Name: Nikash Naik
// CCID: nsnaik
// Lecture section: A1
// Instructor: Henry Tang
// Lab section: D02


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_LINE  256



typedef struct Node {
    int x;
    int y;
    int g;               // Exact path cost from start to current node
    int h;               // Heuristic estimate cost from current node to end
    int f;               // Total estimated cost (f = g + h)
    struct Node *parent; // Pointer to parent node for reconstructing the path
} Node;

typedef struct {
    Node **data;        // Array of Node pointers
    int size;           // Current number of elements in the heap
    int capacity;       // Maximum capacity of the heap
} MinHeap;

typedef struct {
    Node **data;        // Array of Node pointers in the closed list
    int size;           // Current number of elements in the closed list
    int capacity;       // Maximum capacity of the closed list
} ClosedList;


Node *create_node(int x, int y, int g, int h, Node *parent);

void heap_init(MinHeap *heap);
void heap_insert(MinHeap *heap, Node *node);
Node *heap_extract_min(MinHeap *heap);
void heapify_up(MinHeap *heap, int index);
void heapify_down(MinHeap *heap, int index);
int heap_find(const MinHeap *heap, int x, int y);
void heap_decrease_key(MinHeap *heap, int index, int new_g, int new_h);
bool heap_is_empty(const MinHeap *heap);
void heap_free(MinHeap *heap);

void closed_list_init(ClosedList *list);
void closed_list_add(ClosedList *list, Node *node);
bool closed_list_contains(const ClosedList *list, int x, int y);
void closed_list_free(ClosedList *list);


static Node node_pool[MAX_NODES];
static int node_pool_used = 0;

static Node *heap_storage[MAX_NODES];
static Node *closed_storage[MAX_NODES];


Node *create_node(int x, int y, int g, int h, Node *parent) {
    if (node_pool_used >= MAX_NODES) {
        return NULL;
    }
    Node *node = &node_pool[node_pool_used++];
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;
    return node;
}

void heap_init(MinHeap *heap) {
    heap->data = heap_storage;
    heap->size = 0;
    heap->capacity = MAX_NODES;
}

void closed_list_init(ClosedList *list) {
    list->data = closed_storage;
    list->size = 0;
    list->capacity = MAX_NODES;
}

void heap_free(MinHeap *heap) {
   
    heap->size = 0;
}

void closed_list_free(ClosedList *list) {
    
    list->size = 0;
}



void closed_list_add(ClosedList *list, Node *node) {
    if (node == NULL || list->size >= list->capacity) {
        return;
    }
    list->data[list->size] = node;
    list->size = list->size + 1;
}

bool closed_list_contains(const ClosedList *list, int x, int y) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i]->x == x && list->data[i]->y == y) {
            return true;
        }
    }
    return false;
}

bool node_less(const Node *a, const Node *b) {
    if (a->f != b->f) {
        return a->f < b->f;
    }
    if (a->h != b->h) {
        return a->h < b->h;
    }
    if (a->x != b->x) {
        return a->x < b->x;
    }
    return a->y <  b->y;
}

void heap_swap(MinHeap *heap, int i, int j) {
    Node *temp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = temp;
}

void heapify_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (!node_less(heap->data[index], heap->data[parent])) {
            break;
        }
        heap_swap(heap, index, parent);
        index = parent;
    }
}

void heap_insert(MinHeap *heap, Node *node) {
    if (node == NULL || heap->size >= heap->capacity) {
        return;
    }
    heap->data[heap->size] = node;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

void heapify_down(MinHeap *heap, int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int best = index;

        if (left < heap->size && node_less(heap->data[left], heap->data[best])) {
            best = left;
        }
        if (right < heap->size && node_less(heap->data[right], heap->data[best])) {
            best = right;
        }
        if (best == index) {
            break;
        }
        heap_swap(heap, index, best);
        index = best;
    }
}

Node *heap_extract_min(MinHeap *heap) {
    if (heap->size <= 0) {
        return NULL;
    }
    Node *min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return min;
}

int heap_find(const MinHeap *heap, int x, int y) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->data[i]->x == x && heap->data[i]->y == y) {
            return i;
        }
    }
    return -1;
}

void heap_decrease_key(MinHeap *heap, int index, int new_g, int new_h) {
    if (index < 0 || index >= heap->size) {
        return;
    }
    Node *node = heap->data[index];
    node->g = new_g;
    node->h = new_h;
    node->f = new_g + new_h;
    heapify_up(heap, index);
}

bool heap_is_empty(const MinHeap *heap) {
    return heap->size == 0;
}


static void strip_newline(char *line) {
    size_t length = strlen(line);
    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
        line[length - 1] = '\0';
        length--;
    }
}

static bool is_blank(const char *line) {
    while (*line != '\0') {
        if (*line != ' ' && *line != '\t') {
            return false;
        }
        line++;
    }
    return true;
}


void read_insertions(MinHeap *open_list) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        strip_newline(line);
        if (is_blank(line)) {
            continue;
        }
        if (strncmp(line, "done", 4) == 0) {
            return;
        }

        int x = 0;
        int y = 0;
        int g = 0;
        int h = 0;
        if (sscanf(line, " %d , %d , %d , %d", &x, &y, &g, &h) == 4) {
            heap_insert(open_list, create_node(x, y, g, h, NULL));
        }
    }
}


int main(void) {
    MinHeap open_list;
    ClosedList closed_list;
    char line[MAX_LINE];

    heap_init(&open_list);
    closed_list_init(&closed_list);

    while (fgets(line, sizeof(line), stdin) != NULL) {
        strip_newline(line);

        if (is_blank(line) || strncmp(line, "Commands:", 9) == 0) {
            continue;
        }

        if (strncmp(line, "[O] Insert", 10) == 0) {
            read_insertions(&open_list);

        } else if (strncmp(line, "[O] Extract", 11) == 0) {
            Node *node = heap_extract_min(&open_list);
            if (node != NULL) {
                closed_list_add(&closed_list, node);
                printf("Extracted Node (%d, %d), f = %d, g = %d, h = %d\n",
                       node->x, node->y, node->f, node->g, node->h);
            }

        } else if (strncmp(line, "[O] DecreaseKey", 15) == 0) {
            int x = 0;
            int y = 0;
            int new_g = 0;
            if (sscanf(line, "[O] DecreaseKey ( %d , %d ) , new_g = %d",
                       &x, &y, &new_g) == 3) {
                int index = heap_find(&open_list, x, y);
                if (index >= 0) {
                    Node *node = open_list.data[index];
                    heap_decrease_key(&open_list, index, new_g, node->h);
                    printf("Decreased key for Node (%d, %d): f = %d, g = %d, h = %d\n",
                           node->x, node->y, node->f, node->g, node->h);
                }
            }

        } else if (strncmp(line, "[O] Find", 8) == 0) {
            int x = 0;
            int y = 0;
            if (sscanf(line, "[O] Find ( %d , %d )", &x, &y) == 2) {
                if (heap_find(&open_list, x, y) >= 0) {
                    printf("Node (%d, %d) has been found\n", x, y);
                } else {
                    printf("Node (%d, %d) has not been found\n", x, y);
                }
            }

        } else if (strncmp(line, "[C] Find", 8) == 0) {
            int x = 0;
            int y = 0;
            if (sscanf(line, "[C] Find ( %d , %d )", &x, &y) == 2) {
                if (closed_list_contains(&closed_list, x, y)) {
                    printf("Node (%d, %d) has been found\n", x, y);
                } else {
                    printf("Node (%d, %d) has not been found\n", x, y);
                }
            }

        } else if (strncmp(line, "[O] IsEmpty", 11) == 0) {
            printf("%s\n", heap_is_empty(&open_list) ? "True" : "False");

        } else if (strncmp(line, "[C] IsEmpty", 11) == 0) {
            printf("%s\n", closed_list.size == 0 ? "True" : "False");
        }
    }

    heap_free(&open_list);
    closed_list_free(&closed_list);
    return 0;
}
