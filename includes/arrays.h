#define each(item, array, length)                                              \
  (typeof(*(array)) *p = (array), (item) = *p; p < &((array)[length]);         \
   p++, (item) = *p)

typedef struct _ARRAY_INT {
  int length;
  int *obj;
  int (*push)(struct _ARRAY_INT *, int);
  void (*map)(struct _ARRAY_INT *, int (*map)(int, int));
} ARRAY_INT;

typedef struct _ARRAY_CHAR {
  int length;
  char *obj;
  int (*push)(struct _ARRAY_CHAR *, char);
  int (*concatString)(struct _ARRAY_CHAR *, char *);
  void (*map)(struct _ARRAY_CHAR *, char (*map)(char, int));
} ARRAY_CHAR;

typedef struct _ARRAY_STRING {
  int length;
  char **obj;
  int (*push)(struct _ARRAY_STRING *, char *);
} ARRAY_STRING;

ARRAY_INT newArrayInt();
ARRAY_CHAR newArrayChar();
ARRAY_STRING newArrayString();
