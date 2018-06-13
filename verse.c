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
char* getFreeableString(char* str);
void printFlagsAndCommands();

void shallowTraverse();
void normalTraverse();
void deepTraverse();

//represents the various commands
static int help_command = 0;
static int version_command = 0;
static int usage_command = 0;
static int search_command = 0;

//points to location of root from which repos must be searched (or NULL if root is . )
static char* root = "."; 


//flags
static int shallow = 0;
static int normal = 0;

static int git = 0;
static int hg = 0;
static int cvs = 0;

static int group = 0;
static queue* gitRepos = NULL;
static queue* hgRepos = NULL;
static queue* cvsRepos = NULL;


//constants
static const char* version = "verse version 1.0.0";

static const char* usage = "usage: verse [--version | -v][--help | -h][--usage | -u][--root | -r]<path>\n"
                           "             [-s | -n][--group | -g][--all | -a]<command>";

static const char* help = "usage: verse [--version | -v][--help | -h][--usage | -u][--root | -r]<path>\n"
                          "             [-s | -n][--group | -g][--all | -a]<command>\n\n"

                          "Commands:\n"
                          "    version                 Get the version of the program that's running\n"
                          "    usage                   Get a compact representation of the command syntax\n"
                          "    help                    Get usage information plus explanation of each command and flag option\n"
                          "    search                  Find version control repositories in all subdirectories, including this one\n\n"

                          "Flags:\n"
                          "    --version, -v           Get the version of the program that is running (equivalent to version command)\n"
                          "    --usage, -u             Get a compact representation of the command syntax (equivalent to usage command)\n"
                          "    --help, -h              Get usage information plus explanation of each command and flag option (equivalent to help command)\n" 
                          "    --group, -g             When searching, get the repository printout grouped by version control type\n"
                          "    --all, -a               When searching, look for repos of all 3 supported version control systems (Git, Mercurial, CVS)\n"
                          "    --root, -r              Begin searching for repos from a root defined by a relative path argument following the flag";


//flag parsing data for getopt_long function
struct option arguments[] = {
  {"version", no_argument, 0, 'v'},
  {"help", no_argument, 0, 'h'},
  {"usage", no_argument, 0, 'u'},
  {"group", no_argument, 0, 'g'},
  {"all", no_argument, 0, 'a'},
  {"root", required_argument, 0, 'r'},
  {0, 0, 0, 0}
};

const char* shortFlags = "vVhHuUsSnNgGaAr:R:";
int argumentsIndex = 0;


int main(int argc, char** argv) {

  //process command-line arguments
  parseArguments(argc, argv);

  if(validateArguments()) {

    return 1;

  } else {

    //execute
    if(help_command) {
      
      printf("%s\n", help);

    } else if(version_command) {

      printf("%s\n", version);   

    } else if(usage_command) {

      printf("%s\n", usage);

    } else if(search_command) {

      if(normal) {
        normalTraverse();
      } else {
        shallowTraverse();
      }

    }

  }

}



//BFS traversal of directories to find all repositories
void shallowTraverse() {

  //create queue to hold subdirectories
  queue* subdirectories = makeQueue();
  queueNode* node = makeNode();
  node->dir = root;
  enqueue(subdirectories, node);

  //if output should be grouped, malloc queues
  if(group) {
    gitRepos = makeQueue();
    hgRepos = makeQueue();
    cvsRepos = makeQueue();
  }

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
        if(git && !strcmp(subdirName, ".git")) {
          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = currDir;
            enqueue(gitRepos, qnode);
          } else {
            printf("%s        git\n", currDir);
          }
        }
        
        if(hg && !strcmp(subdirName, ".hg")) {
          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = currDir;
            enqueue(hgRepos, qnode);
          } else {
            printf("%s        hg\n", currDir);
          }
        }

        if(cvs && !strcmp(subdirName, ".cvs")) {
          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = currDir;
            enqueue(cvsRepos, qnode);
          } else {
            printf("%s        cvs\n", currDir);
          }
        }
      }

    }
  }

  //if output should be grouped, print the repos in the queues
  if(group) {
  
    if(git) {
      printf("Git---------------------------\n");
      while(!isEmpty(gitRepos)) {
        printf("%s\n", dequeue(gitRepos)->dir);
      }
    }

    if(hg) {

      if(git)
        printf("\n");

      printf("Mercurial---------------------------\n");
      while(!isEmpty(hgRepos)) {
        printf("%s\n", dequeue(hgRepos)->dir);
      }
    }

    if(cvs) {

      if(git || hg)
        printf("\n");

      printf("CVS---------------------------\n");
      while(!isEmpty(cvsRepos)) {
        printf("%s\n", dequeue(cvsRepos)->dir);
      }
    }

  }

}

void normalTraverse() {

  //initialize queue with current directory
  queue* files = makeQueue();
  queueNode* start = makeNode();
  start->dir = getFreeableString(root);
  enqueue(files, start);

  DIR* dir = NULL;
  struct dirent* sub = NULL;

  //if output should be grouped, malloc queues
  if(group) {
    gitRepos = makeQueue();
    hgRepos = makeQueue();
    cvsRepos = makeQueue();
  }

  while(!isEmpty(files)) {
 
    queueNode* curr = dequeue(files);

    if((dir = opendir(curr->dir)) != NULL) {
 
      while((sub = readdir(dir)) != NULL) {
  
        char* subdirName = sub->d_name;
        int isVersionControl = 0;
  
        //check if directory is a repository
        if(git && !strcmp(subdirName, ".git")) {

          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = getFreeableString(curr->dir);
            enqueue(gitRepos, qnode);
          } else {
            printf("%s        git\n", curr->dir);
          }

          isVersionControl = 1;
        }
        
        if(hg && !strcmp(subdirName, ".hg")) {

          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = getFreeableString(curr->dir);
            enqueue(hgRepos, qnode);
          } else {
            printf("%s        hg\n", curr->dir);
          }

          isVersionControl = 1;
        }

        if(cvs && !strcmp(subdirName, ".cvs")) {
          
          if(group) {
            queueNode* qnode = makeNode();
            qnode->dir = getFreeableString(curr->dir);
            enqueue(cvsRepos, qnode);
          } else {
            printf("%s        cvs\n", curr->dir);
          }

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

      free(dir);
    }

    //free data associated with current node
    freeQueueNode(curr);
	
  }

  //free the BFS queue
  freeQueue(files);

  //if output should be grouped, print the repos in the queues
  if(group) {
  
    if(git) {
      printf("Git---------------------------\n");
      queueNode* qn = NULL;
      while(!isEmpty(gitRepos)) {
        qn = dequeue(gitRepos);
        printf("%s\n", qn->dir);
        freeQueueNode(qn);
      }

      freeQueue(gitRepos);
    }

    if(hg) {

      if(git)
        printf("\n");

      printf("Mercurial---------------------------\n");
      queueNode* qn = NULL;
      while(!isEmpty(hgRepos)) {
        qn = dequeue(hgRepos);
        printf("%s\n", qn->dir);
        freeQueueNode(qn);
      }

      freeQueue(hgRepos);
    }

    if(cvs) {

      if(git || hg)
        printf("\n");

      printf("CVS---------------------------\n");
      queueNode* qn = NULL;
      while(!isEmpty(cvsRepos)) {
        qn = dequeue(cvsRepos);
        printf("%s\n", qn->dir);
        freeQueueNode(qn);
      }

      freeQueue(cvsRepos);
    }

  }

}


//make sure flags and commands don't conflict
int validateArguments() {

  int invalid = 0;

  //only 1 recursive-depth flag can be turned on
  int recOptionFlagCount = (normal) ? 1 : 0;
  recOptionFlagCount = (shallow) ? recOptionFlagCount+1 : recOptionFlagCount;

  if(recOptionFlagCount > 1) {
    printf("Only 1 recursive depth flag [-s | -n] can be specified\n");
    invalid = 1;
  }

  //only 1 command can be specified
  int commandCount = 0;
  commandCount = (help_command) ? 1 : 0;
  commandCount = (version_command) ? commandCount+1 : commandCount;
  commandCount = (usage_command) ? commandCount+1 : commandCount;
  commandCount = (search_command) ? commandCount+1 : commandCount;

  if(commandCount == 0) {
    printf("No commands were specified. Please specify exactly 1 command.\n");
    invalid = 1;
  }

  if(commandCount > 1) {
    printf("Only 1 command can be specified\n");
    invalid = 1;
  }

  return invalid;
}


//parse command line arguments
void parseArguments(int argc, char** argv) {

  optind = 0;
  int flag = 0;

  //parse option flags
  while((flag = getopt_long(argc, argv, shortFlags, arguments, &argumentsIndex)) != -1) {
    switch(flag) {
      case 'h': help_command = 1; break;
      case 'H': help_command = 1; break;
      case 'v': version_command = 1; break;
      case 'V': version_command = 1; break;
      case 'u': usage_command = 1; break;
      case 'U': usage_command = 1; break;
      case 's': shallow = 1; break;
      case 'S': shallow = 1; break;
      case 'n': normal = 1; break;
      case 'N': normal = 1; break;
      case 'g': group = 1; break;
      case 'G': group = 1; break;
      case 'a': git = hg = cvs = 1; break;
      case 'A': git = hg = cvs = 1; break;
      case 'r': root = optarg; break;
      case 'R': root = optarg; break;
    }  
  }

  //if no search options have been set, default is normal 
  if(!shallow && !normal) {
    shallow = 0; 
    normal = 1;
  }

  //parse command (should only be 1)
  for(int i = optind; i < argc; i++) {

    if(strcmp(argv[i], "version") == 0) {
      version_command = 1;
    } else if(strcmp(argv[i], "help") == 0) {
      help_command = 1;
    } else if(strcmp(argv[i], "usage") == 0) {
      usage_command = 1;
    } else if(strcmp(argv[i], "search") == 0) {

      search_command = 1;

      for(int j = i+1; j < argc; j++) {       
        if(!strcmp(argv[j], "git")) {
          git = 1;
        } else if(!strcmp(argv[j], "hg")) {
          hg = 1;
        } else if(!strcmp(argv[j], "cvs")) {
          cvs = 1;
        } else if(!strcmp(argv[j], "all")) {
          git = hg = cvs = 1;
        }
      }
      
      //if no version control option flags are set, default is only git
      if(!git && !hg && !cvs) { 
        git = 1;
        hg = cvs = 0;
      }
      
      return;
    }
  }

}

//returns equivalent of string literal allocated on heap (so it can be freed)
char* getFreeableString(char* str) {
  int len = strlen(str);
  char* ptr = (char*) malloc(sizeof(char)*(len+1));
  strncpy(ptr, str, len+1);
  ptr[len] = '\0';
  return ptr;
}


//print the commands and flags (for debugging purposes)
void printFlagsAndCommands() {

  printf("help_command = %d\n"
          "version_command = %d\n"
          "search_command = %d\n"
          "usage_command = %d\n\n"
          
          "shallow = %d\n"
          "normal = %d\n"
          "group = %d\n\n"

          "git = %d\n"
          "hg = %d\n"
          "cvs = %d\n", 
        
          help_command, version_command, search_command, usage_command, 
          shallow, normal, group, git, hg, cvs);
}




