#include "project-config.h"

#include "arrays.h"
#include "tiny-json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_FIELDS = 256 };
json_t pool[MAX_FIELDS];
json_t const *projectConfigJson;

ARRAY_STRING arguments;

char *configFileContent = NULL;

int openProjectConfigFile();
int loadProjectConfigJson();
void initialize(ProjectConfig *self);
void generateArgs(ProjectConfig *self, int forDebug);

// Getters
void getMainFile(ProjectConfig *self);
void getGccPath(ProjectConfig *self);
void getDebugFileName(ProjectConfig *self);
void getCompileFileName(ProjectConfig *self);
void getDebugFolderName(ProjectConfig *self);
void getCompileFolderName(ProjectConfig *self);

void ConstructProjectConfig(ProjectConfig *instance) {
  instance->initialize = initialize;
  instance->generateArgs = generateArgs;

  instance->args = NULL;
  instance->gccPath = NULL;
  instance->debugFolder = NULL;
  instance->debugFileName = NULL;
  instance->compileFolder = NULL;
  instance->compileFileName = NULL;

  arguments = newArrayString();
}

void initialize(ProjectConfig *self) {
  if (openProjectConfigFile() == -3) {
    return;
  }

  if (loadProjectConfigJson() != 0) {
    return;
  }

  free(configFileContent);

  printf("O arquivo de configuração foi carregado com sucesso\n");

  getMainFile(self);
  getCompileFileName(self);
  getCompileFolderName(self);
  getDebugFileName(self);
  getDebugFolderName(self);
  getGccPath(self);
}

int openProjectConfigFile() {
  FILE *f = fopen("project.config.json", "rb");

  if (f == NULL) {
    printf("The config project file 'project.config.json' not exists\n");
    return -3;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  configFileContent = malloc(fsize + 1);
  fread(configFileContent, fsize, 1, f);
  fclose(f);

  configFileContent[fsize] = 0;

  return 0;
}

int loadProjectConfigJson() {
  projectConfigJson = json_create(configFileContent, pool, MAX_FIELDS);
  if (projectConfigJson == NULL) {
    return -4;
  }
  return 0;
}

void getDebugFileName(ProjectConfig *self) {
  json_t const *property = json_getProperty(projectConfigJson, "debug_name");

  if (!property && self->compileFileName == NULL) {
    printf("Debug name is required.");
    return;
  } else if (!property && self->compileFileName != NULL) {
    self->debugFileName = self->compileFileName;
    return;
  }

  self->debugFileName = (char *)json_getValue(property);
}

void getCompileFileName(ProjectConfig *self) {
  json_t const *property = json_getProperty(projectConfigJson, "compile_name");

  if (!property) {
    printf("Compile name is required.");
    return;
  }

  self->compileFileName = (char *)json_getValue(property);
}

void getDebugFolderName(ProjectConfig *self) {
  json_t const *property = json_getProperty(projectConfigJson, "debug_folder");

  if (!property && self->compileFolder == NULL) {
    printf("Debug folder name is required.");
    return;
  } else if (!property && self->compileFolder != NULL) {
    self->debugFolder = self->compileFolder;
    return;
  }

  self->debugFolder = (char *)json_getValue(property);
}

void getCompileFolderName(ProjectConfig *self) {
  json_t const *property =
      json_getProperty(projectConfigJson, "compile_folder");

  if (!property) {
    printf("Compile folder is required.");
    return;
  }

  self->compileFolder = (char *)json_getValue(property);
}

void getGccPath(ProjectConfig *self) {
  json_t const *property = json_getProperty(projectConfigJson, "gcc_path");

  if (!property) {
    printf("GCC path is required.");
    return;
  }

  self->gccPath = (char *)json_getValue(property);
}

void getMainFile(ProjectConfig *self) {
  json_t const *property = json_getProperty(projectConfigJson, "main_file");

  if (!property) {
    printf("Main file is required.");
    return;
  }

  self->mainFile = (char *)json_getValue(property);
}

int appendArgsList(char *property) {
  json_t const *propertyList = json_getProperty(projectConfigJson, property);

  if (!propertyList) {
    return 4;
  }

  if (JSON_ARRAY != json_getType(propertyList)) {
    return 3;
  }

  json_t const *propertyItem;
  for (propertyItem = json_getChild(propertyList); propertyItem != 0;
       propertyItem = json_getSibling(propertyItem)) {

    char *value = (char *)json_getValue(propertyItem);

    if (JSON_TEXT == json_getType(propertyItem)) {
      arguments.push(&arguments, value);
    }
  }

  return 0;
}

void generateArgs(ProjectConfig *self, int forDebug) {
  arguments.push(&arguments, self->gccPath);
  arguments.push(&arguments, self->mainFile);

  int includesListError = appendArgsList("includes");
  if (includesListError == 4) {
    puts("Error, the includes list property is not found.");
    return;
  }

  if (includesListError == 3) {
    puts("Error, the includes property needs to be an array.");
    return;
  }

  arguments.push(&arguments, "-o");
  ARRAY_CHAR output = newArrayChar();

  if (forDebug) {
    output.concatString(&output, self->debugFolder);
    output.concatString(&output, "/");
    output.concatString(&output, self->debugFileName);
  } else {
    output.concatString(&output, self->compileFolder);
    output.concatString(&output, "/");
    output.concatString(&output, self->compileFileName);
  }

  arguments.push(&arguments, output.obj);

  int argumentsListError = appendArgsList("arguments");

  if (argumentsListError == 3) {
    puts("Error, the arguments property needs to be an array.");
    return;
  }

  int lastIndex = arguments.push(&arguments, "");

  arguments.obj[lastIndex] = NULL;

  self->args = arguments.obj;
}