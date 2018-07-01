#ifndef UTIL_H
#define UTIIL_H

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str);
int checkIfBufferOverflow(char* input, int maxLen);
void clearInputBuffer();
void handlePotentialBufferOveflow(char* input, int maxLen);
int* parseRepoSpecifiers(char* str, int maxRepos, int* len, int* statusFlag);


#endif