#include "arrays.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int push_int(ARRAY_INT *arr, int item) {
  ARRAY_INT new_arr;
  new_arr.length = arr->length + 1;
  new_arr.obj = realloc(arr->obj, new_arr.length * sizeof(int));

  if (!new_arr.obj)
    return -1;

  new_arr.obj[new_arr.length - 1] = item;
  arr->length = new_arr.length;
  arr->obj = new_arr.obj;

  return arr->length - 1;
}

void map_int(ARRAY_INT *arr, int (*cb)(int, int)) {
  for (int i = 0; i < arr->length; i++)
    arr->obj[i] = (*cb)(arr->obj[i], i);
}

void ArrayIntConstruct(ARRAY_INT *arr) {
  arr->length = 0;
  arr->obj = NULL;
  arr->map = map_int;
  arr->push = push_int;
}

int push_char(ARRAY_CHAR *self, char item) {
  ARRAY_CHAR new_arr;
  new_arr.length = self->length + 1;
  new_arr.obj = realloc(self->obj, new_arr.length * sizeof(char));

  if (!new_arr.obj)
    return -1;

  new_arr.obj[new_arr.length - 1] = item;
  self->length = new_arr.length;
  self->obj = new_arr.obj;

  self->obj[self->length] = '\0';

  return self->length - 1;
}

void map_char(ARRAY_CHAR *self, char (*cb)(char, int)) {
  for (int i = 0; i < self->length; i++)
    self->obj[i] = (*cb)(self->obj[i], i);
}

int concat_string(ARRAY_CHAR *self, char *string) {
  for each (element, string, strlen(string)) {
    self->push(self, element);
  }

  self->obj[self->length] = '\0';

  return self->length;
}

void ArrayCharConstruct(ARRAY_CHAR *self) {
  self->length = 0;
  self->obj = NULL;
  self->map = map_char;
  self->push = push_char;
  self->concatString = concat_string;
}

int push_string(ARRAY_STRING *self, char *item) {
  ARRAY_STRING new_arr;
  new_arr.length = self->length + 1;
  new_arr.obj = (char **)realloc(self->obj, new_arr.length * sizeof(char *));

  if (!new_arr.obj) {
    return -1;
  }

  new_arr.obj[new_arr.length - 1] = strdup(item);
  self->length = new_arr.length;
  self->obj = new_arr.obj;

  return self->length - 1;
}

void ArrayStringConstruct(ARRAY_STRING *self) {
  self->length = 0;
  self->obj = NULL;
  self->push = push_string;
}

ARRAY_INT newArrayInt() {
  ARRAY_INT instance;
  ArrayIntConstruct(&instance);
  return instance;
}

ARRAY_CHAR newArrayChar() {
  ARRAY_CHAR instance;
  ArrayCharConstruct(&instance);
  return instance;
}

ARRAY_STRING newArrayString() {
  ARRAY_STRING instance;
  ArrayStringConstruct(&instance);
  return instance;
}