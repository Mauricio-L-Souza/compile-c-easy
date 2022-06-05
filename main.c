#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "includes/project-config.h"

int run();
int compile();

ProjectConfig project;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("One of these arguments is required [compile] or [run]\n");
    printf("The [compile] argument will only compile your c code.\n");
    printf("The [run] argument will compile and run your c code.\n");
    return -1;
  }

  ConstructProjectConfig(&project);
  project.initialize(&project);

  if (strcmp(argv[1], "compile") == 0) {
    return compile();
  }

  if (strcmp(argv[1], "run") == 0) {
    return run();
  }

  printf("One of these arguments is required [compile] or [run]\n");
  printf("The [compile] argument will only compile your c code.\n");
  printf("The [run] argument will compile and run your c code.\n");

  return 0;
}

int makeCompileDir() {
  struct stat st = {0};

  if (stat(project.compileFolder, &st) == -1) {
    mkdir(project.compileFolder, 0700);
  }
  return 0;
}

int makeDebugDir() {
  struct stat st = {0};

  if (stat(project.debugFolder, &st) == -1) {
    mkdir(project.debugFolder, 0700);
  }
  return 0;
}

int run() {
  makeDebugDir();
  project.generateArgs(&project, 1);
  execv(project.gccPath, project.args);
  return 0;
}

int compile() {
  makeCompileDir();
  project.generateArgs(&project, 0);
  execv(project.gccPath, project.args);
  return 0;
}