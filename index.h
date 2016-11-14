#define HW11_BONUS_1
#define HW11_BONUS_2

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct _StringListNode {
	struct _StringListNode* next;
	char*                   filename;
} StringListNode;

typedef struct _IndexBSTNode { 
	char*                   word;
	StringListNode*         filenames;
	struct _IndexBSTNode*   left;
	struct _IndexBSTNode*   right;
} IndexBSTNode;

typedef struct {
	IndexBSTNode*           root; // index of words
	IndexBSTNode* 			filesRoot; // index of filenames
} Index;

Index create_index();
void put(Index* index, char* word, char* filename);
char** get(Index* index, int* count, char* word, ...);
void free_index(Index* index);
