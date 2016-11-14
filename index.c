#include <stdio.h>
#include "index.h"

// helper functions
void _add(IndexBSTNode** root, char* word);
void _append_to_list(Index* index, IndexBSTNode** node, char* word);
IndexBSTNode* _searchBST(IndexBSTNode* root, char* target);
int _getLinkedListSize(StringListNode* head);
void _free_linked_list(StringListNode* head);
void _free_tree(IndexBSTNode** root);
bool _notInArray(char* word, char** stringArray, int size);
char* _strdup(char* s); 

Index create_index()
{
	Index newIndex = { .root = NULL, .filesRoot = NULL };
	return newIndex;
}

int _getLinkedListSize(StringListNode* head)
{
	int size = 0;
	StringListNode* currentNode = head;

	while(currentNode != NULL)
	{
		size++;
		currentNode = currentNode -> next;
	}
	
	return size;
}

void _append_to_list(Index* index, IndexBSTNode** node, char* word) 
{
	StringListNode* currentNode =  (*node) -> filenames; // head of filenames list
	StringListNode* newNode = malloc(sizeof(*newNode)); 

	IndexBSTNode* fileNameNode = _searchBST(index -> filesRoot, word); // linking name to name in existing tree of filenames

	newNode -> filename = fileNameNode -> word;
	newNode -> next = NULL;
	
	if(currentNode == NULL) // change head
	{
		(*node) -> filenames = newNode;
	}
	else
	{
		if(strcmp(currentNode -> filename, word) == 0) // stop considering adding it if already in list
		{
			free(newNode);
			return;
		}

		while(currentNode -> next != NULL)
		{
			if(strcmp(currentNode -> filename, word) == 0) // stop considering adding it if already in list
			{
				free(newNode);
				return;
			}
			currentNode = currentNode -> next; // loop to get to the tail of the list
		}

		if(strcmp(currentNode -> filename, word) != 0)
		{
			currentNode -> next = newNode; // append at end
		}
		else
		{
			free(newNode);
			return;
		}
	}
}

IndexBSTNode* _searchBST(IndexBSTNode* root, char* target) // return found node
{
	if(root == NULL)
		return NULL; // not found
	
	if(strcmp(root -> word, target) < 0)
	{
		return _searchBST(root -> right, target);
	}
	else if(strcmp(root -> word, target) > 0)
	{
		return _searchBST(root -> left, target);
	}
	else // found
	{
		return root;
	}
}

void _add(IndexBSTNode** root, char* word) // does not add duplicate words to tree
{
	if(*root == NULL)
	{
		*root = malloc(sizeof(**root));
		(*root) -> word = _strdup(word);
		(*root) -> left = NULL;
		(*root) -> right = NULL;
		(*root) -> filenames = NULL;
	}

	else
	{
		if(strcmp((*root) -> word, word) > 0)
		{
			_add(&((*root) -> left), word);
		}
		else if(strcmp((*root) -> word, word) < 0)
		{
			_add(&((*root) -> right), word);
		}
	}
}

void put(Index* index, char* word, char* filename)
{
	// used alternative helper function because recursion could not be done using the index argument, used the root (IndexBSTNode* type) instead
	
	IndexBSTNode* searchedWord = NULL;

	searchedWord = _searchBST(index -> root, word);

	if(searchedWord == NULL) // if word not already in tree, add it
	{
		_add(&(index -> root), word); // add word to BST of words
		searchedWord = _searchBST(index -> root, word); // repeat search so that you find inserted word
		searchedWord -> filenames = NULL; 
	}

	_add(&(index -> filesRoot), filename); // *BONUS 1* // won't add anything if already in tree
	_append_to_list(index, &searchedWord, filename); // adds filename to word's list of filenames
}

char** get(Index* index, int* count, char* word, ...) //  *BONUS 2* // last argument must be NULL
{
	va_list words, wordsCopy;
	va_start(words, word);
	va_copy(wordsCopy, words); // copy to get the amount of space required for dynamic allocation of array memory first

	char* currentWord = word;
	int arraySize = 0;
	
	while(currentWord != NULL)
	{
		IndexBSTNode* foundWord = _searchBST(index -> root, currentWord);

		if(foundWord != NULL)
		{
			int currentNumOfFiles = _getLinkedListSize(foundWord -> filenames);
			arraySize += currentNumOfFiles;
		}

		currentWord = va_arg(wordsCopy, char*);
	} // when this loop terminates, we have the size of the array and we can allocate space for it

	if(arraySize == 0)
	{
		return NULL; // none of the words given were in any of the files
	}

	char** array_of_filenames = malloc(sizeof(*array_of_filenames) * arraySize);

	currentWord = word;
	int i = 0;

	while(currentWord != NULL)
	{
		IndexBSTNode* foundWord = _searchBST(index -> root, currentWord);

		if(foundWord != NULL) // for each word, iterate through the files it's found in and store their names in the array
		{
			StringListNode* currentNode = foundWord -> filenames;
			while(currentNode != NULL)
			{
				if(_notInArray(currentNode -> filename, array_of_filenames, i))
				{
					array_of_filenames[i] = currentNode -> filename;
					i++;
				}

				currentNode = currentNode -> next;
			}
		}

		currentWord = va_arg(words, char*);
	} 

	va_end(wordsCopy);
	va_end(words);

	*count = i;

	return array_of_filenames;
}

bool _notInArray(char* word, char** stringArray, int size)
{
	for(int i = 0; i < size; i++)
	{
		if(strcmp(stringArray[i], word) == 0)
		{
			return false;
		}
	}
	return true;
}

void free_index(Index* index)
{
	_free_tree(&(index -> root));
	_free_tree(&(index -> filesRoot));
	index = NULL;
}

void _free_tree(IndexBSTNode** root)
{
	if(*root == NULL)
		return;

	_free_tree(&((*root) -> left));
	_free_tree(&((*root) -> right));

	_free_linked_list((*root) -> filenames);

	free((*root) -> word);
	(*root) -> word = NULL;

	free(*root);
	*root = NULL;
}

void _free_linked_list(StringListNode* head) 
{
	if(head == NULL)
		return;

	_free_linked_list(head -> next);

	free(head); 
	head = NULL;
}

char* _strdup(char* s) {
	return strcpy(malloc(sizeof(*s) * (strlen(s) + 1)), s);
}
