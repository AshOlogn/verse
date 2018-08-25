#ifndef MAIN_H
#define MAIN_H

void parseArguments(int argc, char** argv);
void parseCommandParameters(int position, int argc, char** argv);
int validateArguments();
void printFlagsAndCommands();

void shallowTraverse();
void normalTraverse();
void deepTraverse();

#endif