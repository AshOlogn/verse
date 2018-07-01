#include <stdlib.h>
#include <string.h>

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str) {
  int len = strlen(str);
  char* ptr = (char*) malloc(sizeof(char)*(len+1));
  strncpy(ptr, str, len+1);
  ptr[len] = '\0';
  return ptr;
}

int* parseRepoSpecifiers(char* str, int maxRepos, int* len, int* statusFlag) {
	
	char* current = str;
	char* next;
	int* repoNumbers = (int*) malloc(sizeof(int)*maxRepos);

	int repoCount = 0;
	int repoNumber = 1;

	while(repoCount < maxRepos && repoNumber) {

		repoNumber = (int) strtol(current, &next, 10);

		if(repoNumber < 0 || repoNumber > maxRepos) {
			*statusFlag = 1;
			return repoNumbers;
		}

		if(repoNumber != 0) {
			repoNumbers[repoCount] = repoNumber;
			repoCount++; 
			current = next;
		}
	}

	*statusFlag = 0;
	*len = repoCount;
	return repoNumbers;
}
