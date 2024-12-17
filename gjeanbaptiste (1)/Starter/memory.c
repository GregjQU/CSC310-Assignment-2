#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


// Function implementations
struct memNode *makeNode(int isFree, int start, int size, struct memNode *prev, struct memNode *next) {
    struct memNode *node = (struct memNode *)malloc(sizeof(struct memNode));
    node->isFree = isFree;
    node->start = start;
    node->size = size;
    node->prev = prev;
    node->next = next;
    return node;
}

void printList(struct memNode *p) {
    // Function to print the memory list
    while (p != NULL) {
        printf("(%d,%d,%d) ", p->isFree, p->start, p->size);
        p = p->next;
    }
    printf("\n");
}

void printVars(struct memNode *p[]) {
    // Function to print the variable list
    for (int i = 0; i < NUM_VARS; i++) {
        if (p[i] != NULL) {
            printf("(%d,%d,%d) ", i, p[i]->start, p[i]->size);
        }
    }
    printf("\n");
}

void split(struct memNode *p, int size) {
    // Function to split a node
    if (p->size > size) {
        struct memNode *newNode = makeNode(1, p->start + size, p->size - size, p, p->next);
        p->size = size;
        p->next = newNode;
        if (newNode->next) {
            newNode->next->prev = newNode;
        }
    }
}

void coalesce(struct memNode *p) {
    // Function to coalesce nodes
    if (p->prev && p->prev->isFree) {
        p->prev->size += p->size;
        p->prev->next = p->next;
        if (p->next) {
            p->next->prev = p->prev;
        }
        free(p);
        p = p->prev;
    }
    if (p->next && p->next->isFree) {
        p->size += p->next->size;
        struct memNode *temp = p->next;
        p->next = temp->next;
        if (temp->next) {
            temp->next->prev = p;
        }
        free(temp);
    }
}

struct memNode *findFree(struct memNode *h, int size, int algo) {
    // Function to find free space to allocate
    struct memNode *bestNode = NULL;
    struct memNode *worstNode = NULL;

    while (h != NULL) {
        if (h->isFree && h->size >= size) {
            if (algo == FIRST_FIT) {
                return h;
            } else if (algo == BEST_FIT) {
                if (bestNode == NULL || h->size < bestNode->size) {
                    bestNode = h;
                }
            } else if (algo == WORST_FIT) {
                if (worstNode == NULL || h->size > worstNode->size) {
                    worstNode = h;
                }
            }
        }
        h = h->next;
    }

    return (algo == BEST_FIT) ? bestNode : worstNode;
}

// Malloc simulation
void simulateMalloc(struct memNode **head, struct memNode *varList[], int var, int size, int algo) {
    struct memNode *freeNode = findFree(*head, size, algo);
    
    if (!freeNode) {
        printf("ERROR: Out of Space\n");
        return;
    }

    // Allocate memory
    split(freeNode, size);
    freeNode->isFree = 0;
    varList[var] = freeNode;
}

// Free simulation
void simulateFree(struct memNode *varList[], int var) {
    struct memNode *node = varList[var];
    
    if (!node) {
        fprintf(stderr, "ERROR: Variable %d is not allocated\n", var);
        return;
    }

    node->isFree = 1;
    varList[var] = NULL;
    coalesce(node);
}

// Realloc simulation
void simulateRealloc(struct memNode **head, struct memNode *varList[], int var, int newSize, int algo) {
    struct memNode *node = varList[var];
    
    if (!node) {
        fprintf(stderr, "ERROR: Variable %d is not allocated\n", var);
        return;
    }

    // Free the old space
    simulateFree(varList, var);

    if (newSize > 0) {
        // Allocate new space
        simulateMalloc(head, varList, var, newSize, algo);
    }
}

// Main function
int main(int argc, char *argv[]) {
    // Initialize variables
    int memorySize, algorithm, requestType, var, size;
    struct memNode *head = NULL;
    struct memNode *varList[NUM_VARS] = {NULL};

    // Open input file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Parse memory size and algorithm
    fscanf(inputFile, "%d %d", &memorySize, &algorithm);

    // Initialize memory list with a single free block
    head = makeNode(1, 0, memorySize, NULL, NULL);

    // Simulate memory requests
    while (fscanf(inputFile, "%d", &requestType) != EOF && requestType != -1) {
        switch (requestType) {
            case MALLOC:
                fscanf(inputFile, "%d %d", &var, &size);
                simulateMalloc(&head, varList, var, size, algorithm);
                break;
            case FREE:
                fscanf(inputFile, "%d", &var);
                simulateFree(varList, var);
                break;
            case REALLOC:
                fscanf(inputFile, "%d %d", &var, &size);
                simulateRealloc(&head, varList, var, size, algorithm);
                break;
            default:
                fprintf(stderr, "Unknown request type: %d\n", requestType);
                break;
        }
    }

    // Print results
    printList(head);
    printVars(varList);

    // Clean up
    fclose(inputFile);
    return 0;
}
