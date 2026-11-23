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
    // TODO 2
    // Linear scan of indices 0 .. list->size - 1. Return true as soon as
    // an entry matches BOTH x and y; false if you run out.
    //
    // The parameter is `const ClosedList *`: read through it, never assign.
    return false;
}

// ------------------------------------------------------- min-heap (open)

bool node_less(const Node *a, const Node *b) {
    // TODO 3  --  the most important function in the file
    //
    // Return true when node `a` has strictly higher priority than `b`.
    // Priority order from the spec, highest precedence first:
    //     1. smaller f    2. then smaller h    3. then smaller x    4. then smaller y
    //
    // Write it as a cascade: if the f values differ, decide on f; else if
    // the h values differ, decide on h; and so on, with y last.
    //
    // All five nodes in test case 1-1 have f = 6, so a version comparing
    // only f passes zero tests. Once this is right, nothing else in the
    // file should ever compare two nodes directly.
    return false;
}

void heap_swap(MinHeap *heap, int i, int j) {
    // TODO 4
    // Exchange the two POINTERS at indices i and j via a temp Node *.
    // Never copy struct contents: other code holds pointers to these
    // nodes and they must keep pointing at the same structs.
}

void heapify_up(MinHeap *heap, int index) {
    // TODO 5
    // Repairs a node of higher priority than its position allows, by
    // walking it toward the root.
    //
    // Loop while index > 0:
    //   - parent is at (index - 1) / 2
    //   - if the node at index does NOT outrank its parent, stop
    //   - otherwise swap them and continue from the parent's index
    //
    // The index > 0 guard is load-bearing: in C, (0 - 1) / 2 is 0, since
    // integer division truncates toward zero. Without it the root becomes
    // its own parent and the loop never ends.
}

void heap_insert(MinHeap *heap, Node *node) {
    // TODO 6
    // Guard against a NULL node and a full heap. Then put the pointer at
    // index heap->size, increase size, and heapify_up from the index you
    // just filled. A new leaf can only be misplaced upward, so one upward
    // repair is enough. This function does no comparing of its own.
}

void heapify_down(MinHeap *heap, int index) {
    // TODO 7
    // The mirror of heapify_up: repairs a node of lower priority than its
    // position allows, by walking it toward the leaves.
    //
    // Loop:
    //   - children of index are at 2*index + 1 and 2*index + 2
    //   - track a `best` index, starting at index itself
    //   - for each child, FIRST check it exists (child < heap->size), then
    //     if it outranks data[best], best = that child
    //   - if best is still index, stop
    //   - otherwise swap index with best and continue from best
    //
    // The `child < heap->size` test is what keeps you inside the array.
}

Node *heap_extract_min(MinHeap *heap) {
    // TODO 8
    // Return NULL if the heap is empty. Otherwise save data[0] (your
    // answer), move the LAST pointer into slot 0, decrease size,
    // heapify_down from 0, and return the saved pointer.
    //
    // Do NOT free the node: the caller is about to put that same pointer
    // into the closed list, and the struct never leaves node_pool.
    // Adding to the closed list and printing is main's job, not this
    // function's.
    return NULL;
}

int heap_find(const MinHeap *heap, int x, int y) {
    // TODO 9
    // Return the INDEX of the node matching (x, y), or -1 if absent.
    // A linear scan is correct here: a heap is ordered by priority, not by
    // coordinates, so there is nothing to binary search on.
    return -1;
}

void heap_decrease_key(MinHeap *heap, int index, int new_g, int new_h) {
    // TODO 10
    // Check index is in range. Set the node's g and h to the new values,
    // recompute f = g + h, then heapify_up from index.
    //
    // Only an upward repair is needed: lowering g lowers f, which can only
    // make the node outrank its parent. It can never make it worse than
    // its own children.
}

bool heap_is_empty(const MinHeap *heap) {
    // TODO 11
    // One line.
    return true;
}

// ---------------------------------------------- GIVEN: small line helpers

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

// ----------------------------------------------------------- input phases

void read_insertions(MinHeap *open_list) {
    // TODO 12
    // The insertion phase runs its own inner fgets loop. That is how the
    // two phases stay separate without a mode flag.
    //
    // Loop over lines from stdin:
    //   - strip the newline, skip blank lines
    //   - a line starting with "done" ends the phase -> return
    //   - otherwise parse four ints with sscanf, format " %d , %d , %d , %d",
    //     and check it returned 4. Each literal space in a scanf format
    //     matches ANY run of whitespace, so the spacing in the test files
    //     does not matter.
    //   - create_node(x, y, g, h, NULL) and heap_insert it
    //
    // This phase prints nothing at all.
}

// ------------------------------------------------------------------ main

int main(void) {
    MinHeap open_list;
    ClosedList closed_list;
    char line[MAX_LINE];

    heap_init(&open_list);
    closed_list_init(&closed_list);

    while (fgets(line, sizeof(line), stdin) != NULL) {
        strip_newline(line);

        // Blank lines and the "Commands:" marker produce no output.
        if (is_blank(line) || strncmp(line, "Commands:", 9) == 0) {
            continue;
        }

        if (strncmp(line, "[O] Insert", 10) == 0) {
            // TODO 13: hand off to read_insertions

        } else if (strncmp(line, "[O] Extract", 11) == 0) {
            // TODO 14
            // Extract the min. If not NULL, add it to the closed list and
            // print exactly:
            //   Extracted Node (x, y), f = F, g = G, h = H
            // If the heap was empty, print nothing.

        } else if (strncmp(line, "[O] DecreaseKey", 15) == 0) {
            // TODO 15
            // Parse with "[O] DecreaseKey ( %d , %d ) , new_g = %d" and
            // check it returned 3. heap_find the node; if index >= 0, take
            // the Node * from open_list.data[index] BEFORE the call, pass
            // the node's EXISTING h as new_h (only g changes), then print:
            //   Decreased key for Node (x, y): f = F, g = G, h = H
            // Read the fields after the call: the pointer stays valid even
            // though heapify_up may have moved its slot.

        } else if (strncmp(line, "[O] Find", 8) == 0) {
            // TODO 16
            // Parse "[O] Find ( %d , %d )". heap_find >= 0 means present.
            // Print exactly one of:
            //   Node (x, y) has been found
            //   Node (x, y) has not been found

        } else if (strncmp(line, "[C] Find", 8) == 0) {
            // TODO 17: same, but via closed_list_contains

        } else if (strncmp(line, "[O] IsEmpty", 11) == 0) {
            // TODO 18: print True or False, capitalised exactly so

        } else if (strncmp(line, "[C] IsEmpty", 11) == 0) {
            // TODO 19
            // There is no closed_list_is_empty in the provided header, so
            // test the size field directly.
        }
    }

    heap_free(&open_list);
    closed_list_free(&closed_list);
    return 0;
}
