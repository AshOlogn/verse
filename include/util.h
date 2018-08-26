#ifndef UTIL_H
#define UTIIL_H

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str);
int checkIfBufferOverflow(char* input, int maxLen);
void clearInputBuffer();

//for error reporting
typedef enum PARSE_ERROR_TYPE {
	FORMAT_ERROR,
	BOUNDS_ERROR,
	SUCCESS
} PARSE_ERROR_TYPE;

int* parseRepoSpecifiers(char* str, int maxRepos, int* len, PARSE_ERROR_TYPE* statusFlag);
char* getChosenRepoString(int* repoNumbers, int len);

#endif