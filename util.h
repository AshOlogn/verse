#ifndef UTIL_H
#define UTIIL_H

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str);
int* parseRepoSpecifiers(char* str, int maxRepos, int* len, int* statusFlag);

#endif