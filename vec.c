#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct {
  void *data;
  size_t dlen;
  size_t cap;
} rawvec_t;

typedef struct {
  rawvec_t raw;
  size_t len;
} vec_t;

#define vec_new(T) vec_from_dlen(sizeof(T))
#define vec_push(vec_ptr, ...) \
  vec_mempush_va(vec_ptr, (sizeof((void*[]){__VA_ARGS__})/sizeof(void*)), __VA_ARGS__)
#define vec_pusht(vec_ptr, T, ...) \
  vec_mempush_va(vec_ptr, (sizeof((T[]){__VA_ARGS__})/sizeof(T)), __VA_ARGS__)
#define vec_getmem_unsafe(vec, idx) ((vec)->raw.data + idx)
#define vec_get_unsafe(vec, idx, T) (T*)vec_getmem_unsafe(vec, idx)
#define vec_get(vec, idx, T) (T*)vec_getmem(vec, idx)

void rawvec_alloc(rawvec_t *vec) {
  if (vec->data) {
    vec->data = realloc(vec->data, vec->cap * vec->dlen);
  } else {
    vec->data = malloc(vec->cap * vec->dlen);
  }
}

vec_t vec_from_dlen(size_t datalen) {
  vec_t vec;

  vec.len = 0;
  vec.raw.data = 0;
  vec.raw.dlen = datalen;
  vec.raw.cap  = 16;

  rawvec_alloc(&vec.raw);

  return vec;
}

void *vec_mempush(vec_t *vec, size_t n, void *data) {
  if (vec->len + n >= vec->raw.cap) {
    vec->raw.cap += n > 8 ? n : n + 8;
    rawvec_alloc(&vec->raw);
  }

  vec->len += n;
  return memcpy(vec->raw.data + vec->raw.dlen * (vec->len - n), data, n * vec->raw.dlen);
}

void *vec_mempush_va(vec_t *vec, size_t n, ...) {
  return vec_mempush(vec, n, (void*)((size_t)&n + (size_t)sizeof n));
}

void *vec_getmem(vec_t *vec, size_t idx) {
  if (idx >= vec->len)
    return 0;

  return vec_getmem_unsafe(vec, idx);
}

int main() {
  /* create vector */
  vec_t v = vec_new(char);

  /* push data via macros */
  vec_mempush(&v, 6, "Hello!");
  //vec_pusht(&v, char, 'H', 'e', 'l', 'l', 'o', '!', 0);

  /* push data via funciton */
  char *copyof = vec_mempush(&v, 6, "Peace");
  printf("Pushed string '%s' to vector. Address: %p\n", copyof, copyof);

  /* safe get data from vector (via macros) */
  char *sym = vec_get(&v, 5, char);
  printf("v[5] (locates at %p) equals '%c'!\n", sym, *sym);

  /* This fill be NULL */
  char *willNull = vec_get(&v, 100, char);
  printf("Safe v[100] equals %p\n", willNull);

  /* Also, you can get values unsafe (without len check) */
  char *secWord = vec_get_unsafe(&v, 7, char);
  printf("v[7..(=\\0)] (first locates at %p) equals '%s'!\n", sym, sym);

  /* vec_get_unsafe is macro */
  char *vec100 = vec_get_unsafe(&v, 100, char);
  printf("v[100] - 100 is %p, also v.raw.data is %p\n", vec100 - 100, v.raw.data);

  return 0;
}

