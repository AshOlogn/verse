# verse
Verse is a command line tool that allows you to easily figure out where in the convoluted mess of files and subdirectories on your computer you have created version control repositories (Git, Mercurial, and CVS are supported). Simply run the tool from a root directory to get a neatly-formatted printout of all the repositories you have created in the root or any of its subdirectories.

## Broad Overview
Verse works by traversing the directory tree in a breadth-first search and printing out whichever ones are repositories for the version control systems (VCSs) being tracked. You can use command-line flags to specify how the repos should be output, which version control systems to track, and much more (see below for details).

## Getting Started
TODO

## Commands
#### Version
Providing the flag `--version` or `-v` or running the `version` command specifies the tool version.

#### Help
Providing the flag `--help` or `-h` or running the `help` command prints out the usage guide of the tool as well as a brief explanation of commands and flags (a summary of this README).

#### Usage
Providing the flag `--usage` or `-u` or directly running the `usage` command brings up a concise, POSIX-compliant representation of the command line usage of the tool. For more info on how to interpret the syntax, click [here](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html).

#### Search
The `search` command is used to output all the directories that are repos of whichever VCSs you specify. The output format, VCSs that you want to track, and the root directory from which the search begins can all be specified with flags and arguments.

## Command-Line Options
#### Choosing Version Control Systems
The VCSs you want to look out for can be specified in a couple of ways. If you want to include all 3 supported systems, you can either supply the flag `--all` or `-a` to the command you want to run, or provide `all` as an argument to the command. If you want only some of the VCSs to be looked for, provide 1 or more of the arguments `hg`, `git`, and `cvs` (corresponding to Mercurial, Git, and CVS respectively) to the command you're running.

### Choosing Root of Search
If you do not provide a flag specifying the root, the tool starts searching directories in the current directory (`.`). If you want the tool to start somewhere else, provide the flag `--root` or `-r` followed by a path relative to the current directory. For example, if you want to start searching from the parent directory, you would provide the flag and argument `--root ..`.

### Output Format
When running the `search` command specifically, you can specify whether you want all discovered repositories to be displayed grouped by VCS or not. This is determined by whether you supply the `--group` or `-g` flag. 



