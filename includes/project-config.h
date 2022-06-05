typedef struct Self {
  char **args;
  char *gccPath;
  char *mainFile;
  char *debugFolder;
  char *compileFolder;
  char *debugFileName;
  char *compileFileName;

  void (*initialize)(struct Self *);
  void (*generateArgs)(struct Self *, int);
} ProjectConfig;

void ConstructProjectConfig(ProjectConfig *instance);
