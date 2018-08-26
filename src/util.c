#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str) {
  int len = strlen(str);
  char* ptr = (char*) malloc(sizeof(char)*(len+1));
  strncpy(ptr, str, len+1);
  ptr[len] = '\0';
  return ptr;
}

//checks if fgets received more input than buffer can take
int checkIfBufferOverflow(char* input, int maxLen) {
	return input[maxLen-1] == '\0' && input[maxLen-2] != '\n';
}

//clears stdin input buffer
void clearInputBuffer() {
	int ch;
	while((ch = fgetc(stdin)) != '\n' && ch != EOF);
}

//converts char* input of repos into an integer array
int* parseRepoSpecifiers(char* str, int maxRepos, int* len, PARSE_ERROR_TYPE* statusFlag) {

	//keeps track of current position in string
	char* current = str;

	//pointer that shows where the conversion ended
	char* next;
	int* repoNumbers = (int*) malloc(sizeof(int)*maxRepos);

	int repoCount = 0;
	int repoNumber = 1;

	while(repoCount < maxRepos && repoNumber) {

		repoNumber = (int) strtol(current, &next, 10);

		//throw error if number is out of range
		if(repoNumber < 0 || repoNumber > maxRepos) {

			*statusFlag = BOUNDS_ERROR;
			return repoNumbers;

		} else if(repoNumber == 0) {

			//determine if we're at the end or an actual 0 
			//was input by user (error in latter case)
			int index = (int) (next - str);

			while(str[index] != '\0') {
				//catch error cases
				if(str[index] == '0') {
					*statusFlag = BOUNDS_ERROR;
					return repoNumbers;
				} else if(!isspace(str[index]) && !isdigit(str[index])) {
					*statusFlag = FORMAT_ERROR;
					return repoNumbers;
				}

				index++;
			}

			//ok, we're actually done

		} else {
			repoNumbers[repoCount] = repoNumber;
			repoCount++; 
			current = next;
		}

	}

	*statusFlag = SUCCESS;
	*len = repoCount;
	return repoNumbers;
}

//produces formatted string representations of integer arrays
char* getChosenRepoString(int* repoNumbers, int len) {

	//this is the formatted string that is returned
	char* str = (char*) malloc(sizeof(char) * 5001);
	str[0] = '[';
	str[1] = '\0';

	//this is the temp string that is repeatedly appended to str
	char* temp = (char*) malloc(sizeof(char) * 6);
	int temp_len = 5;

	if(len < 1) {
		return str;
	} else {
		temp[0] = 0;
		snprintf(temp, temp_len, "%d", repoNumbers[0]);
		strncat(str, temp, temp_len);
	}

	for(int i = 1; i < len; i++) {
		temp[0] = 0;
		snprintf(temp, temp_len, ", %d", repoNumbers[i]);
		strncat(str, temp, temp_len);
	}

	//now add a closing bracket
	strncat(str, "]", 1);

	free(temp);
	return str;
}
