/*
** Memory header
*/
#include <stdio.h>
#include <stdlib.h>


// Memory API calls
#define MALLOC  1
#define FREE    2
#define REALLOC 3

// Algorithms
#define BEST_FIT  1
#define WORST_FIT 2
#define FIRST_FIT 3

#define NUM_VARS 25

// Doubly-linked list node
struct memNode {
  int isFree;
  int start;
  int size;
  struct memNode *prev;
  struct memNode *next;
};

// Function to create a node
struct memNode *makeNode(int isfree, int start, int size, struct memNode *prev, struct memNode *next);

// Function to print the memory list
void printList(struct memNode *p);

// Function to print the variable list
void printVars(struct memNode *p[]);

// Function to split a node
void split(struct memNode *p, int size);

// Function to coalesce nodes
void coalesce(struct memNode *p);

// Function to find free space to allocate
struct memNode *findFree(struct memNode *h, int size, int algo);
