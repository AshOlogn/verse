#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>

//forward declarations
void parseArguments(int argc, char** argv);
void printFlagsAndCommands();

const char* version = "verse version 1.0";

//represents the various commands
int help_command = 0;
int version_command = 0;
int usage_command = 0;
int search_command = 0;


//flags
int recursive = 0;
int shallow = 0;
int normal = 1;

int git = 0;
int hg = 0;
int cvs = 0;

//flag parsing data for getopt_long function
struct option arguments[] = {
  {"version", no_argument, 0, 'v'},
  {"help", no_argument, 0, 'h'},
  {"usage", no_argument, 0, 'u'},
  {0, 0, 0, 0}
};

const char* shortFlags = "vhursn";
int optionIndex = 0;


int main(int argc, char** argv) {

  opterr = 0;

  /*DIR* d;
  struct dirent *ep;  

  d = opendir("./");
  while(ep = readdir(d)) {
    if(ep->d_type == DT_DIR)
      printf("%s\n", ep->d_name);
  }*/

  parseArguments(argc, argv);
  printFlagsAndCommands();

}



void parseArguments(int argc, char** argv) {

  int flag = 0;

  //parse option flags
  while((flag = getopt_long(argc, argv, shortFlags, arguments, &optionIndex)) != -1) {
    switch(flag) {
      case 'h': help_command = 1; break;
      case 'v': version_command = 1; break;
      case 'u': usage_command = 1; break;
      case 'r':
      case 'R': normal = 0; recursive = 1; break;
      case 's':
      case 'S': normal = 0; shallow = 1; break;
      case 'n':
      case 'N': normal = 1; break;
    }  
  }

  //parse command

}


//print the commands and flags
void printFlagsAndCommands() {

  printf("help_command = %d\n"
          "version_command = %d\n"
          "search_command = %d\n"
          "usage_command = %d\n\n"
          
          "recursive = %d\n"
          "shallow = %d\n"
          "normal = %d\n\n"

          "git = %d\n"
          "hg = %d\n"
          "cvs = %d\n", 
        
          help_command, version_command, search_command, usage_command, 
          recursive, shallow, normal, git, hg, cvs);
}









