#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>

//forward declarations
void parseArguments(int argc, char** argv);
int validateArguments();
void printFlagsAndCommands();

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

int git = 0;
int hg = 0;
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

 //TODO
 /* DIR* d;
  struct dirent *ep;  

  d = opendir("./");
  while(ep = readdir(d)) {
    if(ep->d_type == DT_DIR)
      printf("%s\n", ep->d_name);
  }
  */

}

void normalTraverse() {
  //TODO

}

void deepTraverse() {
  //TODO

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




