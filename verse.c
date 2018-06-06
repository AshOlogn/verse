#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "queue.h"

//forward declarations
void parseArguments(int argc, char** argv);
int validateArguments();
void printFlagsAndCommands();

void shallowTraverse();
void normalTraverse();
void deepTraverse();

//represents the various commands
int help_command = 0;
int version_command = 0;
int usage_command = 0;
int search_command = 0;


//flags
int deep = 0;
int shallow = 0;
int normal = 0;

int group = 0;

int git = 1;
int hg = 1;
int cvs = 0;


//constants
const char* helpMessage = "usage: verse [--version | -v][--help | -h][--usage | -u]\n"
                          "             [-d | -s | -n][--group | -g] <command>";

const char* version = "verse version 1.0.0";


//flag parsing data for getopt_long function
struct option arguments[] = {
  {"version", no_argument, 0, 'v'},
  {"help", no_argument, 0, 'h'},
  {"usage", no_argument, 0, 'u'},
  {"group", no_argument, 0, 'g'},
  {0, 0, 0, 0}
};

const char* shortFlags = "vVhHuUdDsSnNgG";
int optionIndex = 0;


int main(int argc, char** argv) {

  //process command-line arguments
  parseArguments(argc, argv);

  if(validateArguments()) {

    return 1;

  } else {

    //execute
    if(help_command) {
      //TODO
    } else if(version_command) {

      printf("%s\n", version);   

    } else if(usage_command) {

      printf("%s\n", helpMessage);

    } else if(search_command) {

      if(normal) {
        normalTraverse();
      } else if(shallow) {
        shallowTraverse();
      } else if(deep) {
        deepTraverse();
      }
      
    }

  }
}



//BFS traversal of directories to find all repositories
void shallowTraverse() {

  //create queue to hold subdirectories
  queue* subdirectories = makeQueue();
  queueNode* node = makeNode();
  node->dir = ".";
  enqueue(subdirectories, node);

  //check each subdirectory in the root
  DIR* dir = NULL;
  struct dirent* sub = NULL;

  if((dir = opendir(".")) != NULL) {
  
    while((sub = readdir(dir)) != NULL) {
  
      char* subdirName = sub->d_name;
      if((sub->d_type == DT_DIR) && strcmp(subdirName, ".") && strcmp(subdirName, "..") &&
         strcmp(subdirName, ".git") && strcmp(subdirName, ".hg") && strcmp(subdirName, ".cvs")) {
      
        //add subdirectory to the queue
        int len = strlen(subdirName);
        char* addDir = (char*) malloc(sizeof(char) * (len+3));
        addDir[0] = '.'; addDir[1] = '/';
        for(int i = 0; i < len; i++)
          addDir[i+2] = subdirName[i];
        addDir[len+2] = '\0';

        //enqueue the node
        queueNode* addNode = makeNode();
        addNode->dir = addDir;
        enqueue(subdirectories, addNode);
      }
    } 

  } else {
    return;
  }

  //search for repositories 1 layer in
  while(!isEmpty(subdirectories)) {
  
    char* currDir = dequeue(subdirectories)->dir;
    if((dir = opendir(currDir)) != NULL) {
  
      while((sub = readdir(dir)) != NULL) {
  
        char* subdirName = sub->d_name;

        //check if directory is a repository
        if(git && !strcmp(subdirName, ".git"))
          printf("%s        git\n", currDir);
        
        if(hg && !strcmp(subdirName, ".hg"))
          printf("%s        hg\n", currDir);

        if(cvs && !strcmp(subdirName, ".cvs"))
          printf("%s        cvs\n", currDir);
      }
    }
  }

}

void normalTraverse() {

  //initialize queue with current directory
  queue* files = makeQueue();
  queueNode* start = makeNode();
  start->dir = ".";
  enqueue(files, start);

  DIR* dir = NULL;
  struct dirent* sub = NULL;

  while(!isEmpty(files)) {
  
    queueNode* curr = dequeue(files);

    if((dir = opendir(curr->dir)) != NULL) {
     
      while((sub = readdir(dir)) != NULL) {
        
        char* subdirName = sub->d_name;
        int isVersionControl = 0;
    
        //check if directory is a repository
        if(git && !strcmp(subdirName, ".git")) {
          printf("%s        git\n", curr->dir);
          isVersionControl = 1;
        }
        
        if(hg && !strcmp(subdirName, ".hg")) {
          printf("%s        hg\n", curr->dir);
          isVersionControl = 1;
        }

        if(cvs && !strcmp(subdirName, ".cvs")) {
          printf("%s        cvs\n", curr->dir);
          isVersionControl = 1;
        }
        
        //if this file is a directory, look into it
        if(sub->d_type == DT_DIR && !isVersionControl && strcmp(subdirName, ".") && strcmp(subdirName, "..")) {

          //create string containing subdirectory path
          int currDirLen = strlen(curr->dir);
          int subdirNameLen = strlen(subdirName);
          char* newName = (char*) malloc(sizeof(char) * (currDirLen + subdirNameLen + 2));

          for(int i = 0; i < currDirLen; i++)
            newName[i] = (curr->dir)[i];
          newName[currDirLen] = '/';
          for(int i = currDirLen+1; i <= currDirLen + subdirNameLen; i++)
            newName[i] = subdirName[i-currDirLen-1];
          newName[currDirLen + subdirNameLen + 1] = '\0'; 

          //add new node to queue
          queueNode* subdirectory = makeNode();
          subdirectory->dir = newName;
          enqueue(files, subdirectory);
        }
      }
    }
  }  

}

void deepTraverse() {

  //initialize queue with current directory
  queue* files = makeQueue();
  queueNode* start = makeNode();
  start->dir = ".";
  enqueue(files, start);

  DIR* dir = NULL;
  struct dirent* sub = NULL;

  while(!isEmpty(files)) {
  
    queueNode* curr = dequeue(files);

    if((dir = opendir(curr->dir)) != NULL) {
     
      while((sub = readdir(dir)) != NULL) {
        
        char* subdirName = sub->d_name;
        int isVersionControl = 0;
    
        //check if directory is a repository
        if(git && !strcmp(subdirName, ".git")) {
          printf("%s        git\n", curr->dir);
          isVersionControl = 1;
        }
        
        if(hg && !strcmp(subdirName, ".hg")) {
          printf("%s        hg\n", curr->dir);
          isVersionControl = 1;
        }

        if(cvs && !strcmp(subdirName, ".cvs")) {
          printf("%s        cvs\n", curr->dir);
          isVersionControl = 1;
        }
        
        //if this file is a directory, look into it
        if(sub->d_type == DT_DIR && !isVersionControl && strcmp(subdirName, ".") && strcmp(subdirName, "..")) {

          //create string containing subdirectory path
          int currDirLen = strlen(curr->dir);
          int subdirNameLen = strlen(subdirName);
          char* newName = (char*) malloc(sizeof(char) * (currDirLen + subdirNameLen + 2));

          for(int i = 0; i < currDirLen; i++)
            newName[i] = (curr->dir)[i];
          newName[currDirLen] = '/';
          for(int i = currDirLen+1; i <= currDirLen + subdirNameLen; i++)
            newName[i] = subdirName[i-currDirLen-1];
          newName[currDirLen + subdirNameLen + 1] = '\0'; 

          //add new node to queue
          queueNode* subdirectory = makeNode();
          subdirectory->dir = newName;
          enqueue(files, subdirectory);
        }
      }
    }
  }  

}



//make sure flags and commands don't conflict
int validateArguments() {

  int invalid = 0;

  //only 1 recursive-depth flag can be turned on
  int recOptionFlagCount = 0;
  recOptionFlagCount = (deep) ? 1 : 0;
  recOptionFlagCount = (shallow) ? recOptionFlagCount+1 : recOptionFlagCount;
  recOptionFlagCount = (normal) ? recOptionFlagCount+1 : recOptionFlagCount;

  if(recOptionFlagCount > 1) {
    printf("Only 1 recursive depth flag [-d | -s | -n] can be specified\n");
    invalid = 1;
  }

  //only 1 command can be specified
  int commandCount = 0;
  commandCount = (help_command) ? 1 : 0;
  commandCount = (version_command) ? commandCount+1 : commandCount;
  commandCount = (usage_command) ? commandCount+1 : commandCount;
  commandCount = (search_command) ? commandCount+1 : commandCount;

  if(commandCount > 1) {
    printf("Only 1 command can be specified\n");
    invalid = 1;
  }

  return invalid;
}


//parse command line arguments
void parseArguments(int argc, char** argv) {

  int flag = 0;

  //parse option flags
  while((flag = getopt_long(argc, argv, shortFlags, arguments, &optionIndex)) != -1) {
    switch(flag) {
      case 'h': help_command = 1; break;
      case 'H': help_command = 1; break;
      case 'v': version_command = 1; break;
      case 'V': version_command = 1; break;
      case 'u': usage_command = 1; break;
      case 'U': usage_command = 1; break;
      case 'd': deep = 1; break;
      case 'D': deep = 1; break;
      case 's': shallow = 1; break;
      case 'S': shallow = 1; break;
      case 'n': normal = 1; break;
      case 'N': normal = 1; break;
      case 'g': group = 1; break;
      case 'G': group = 1; break;
    }  
  }

  //if no search options have been set, default is normal 
  if(!(deep | shallow | normal))
    normal = 1;

  //parse command (should only be 1)
  for(int i = optionIndex; i < argc; i++) {

    if(strcmp(argv[i], "version") == 0) {
      version_command = 1;
    } else if(strcmp(argv[i], "help") == 0) {
      help_command = 1;
    } else if(strcmp(argv[i], "usage") == 0) {
      usage_command = 1;
    } else if(strcmp(argv[i], "search") == 0) {
      search_command = 1;
    }
  }
}


//print the commands and flags (for debugging purposes)
void printFlagsAndCommands() {

  printf("help_command = %d\n"
          "version_command = %d\n"
          "search_command = %d\n"
          "usage_command = %d\n\n"
          
          "deep = %d\n"
          "shallow = %d\n"
          "normal = %d\n"
          "group = %d\n\n"

          "git = %d\n"
          "hg = %d\n"
          "cvs = %d\n", 
        
          help_command, version_command, search_command, usage_command, 
          deep, shallow, normal, group, git, hg, cvs);
}




