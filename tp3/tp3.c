#include "tp3.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FNV_OFFSET_BASIS 0x811C9DC5
#define FNV_PRIME 0x01000193

typedef struct node node_t;

struct node {
  char* key;
  void* value;
  bool is_deleted;
};

struct dictionary {
  node_t* nodes;
  size_t m; 
  size_t size;
  destroy_f destroy;
};



uint32_t fnv_1a (char* key, size_t len){
  uint32_t hash = FNV_OFFSET_BASIS;
  const unsigned char* ptr = (const unsigned char*)key; 
  for (size_t i = 0; i < len; i++){
    hash = hash ^ ptr[i];
    hash = hash * FNV_PRIME;
  }
  return hash;
}


size_t hashing (const char* key, size_t m, size_t i){ 
  char* copy = (char*)key;
  size_t len = strlen((char*)copy);
  uint32_t fnv_hash = fnv_1a(copy, len);
  size_t pos = (fnv_hash+i)%m;

  return pos;
}


dictionary_t *dictionary_create(destroy_f destroy) {
  dictionary_t* dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (dict == NULL) return NULL;
  dict->destroy = destroy;
  dict->size = 0;
  dict->m = 7;
  dict->nodes = (node_t*) calloc (dict->m, sizeof(node_t));
  if (dict->nodes == NULL){
    free(dict);
    return NULL;
  }
  return dict;
  };

bool rehash(dictionary_t* dictionary){
  size_t m = dictionary->m;
  node_t* nodes = dictionary->nodes;
  dictionary->m = m*7;
  dictionary->nodes = (node_t*) calloc (dictionary->m, sizeof(node_t));
  if (dictionary->nodes == NULL){
    dictionary->m = m;
    dictionary->nodes = nodes;
    return false;
  }
  dictionary->size = 0;
  for (size_t i = 0; i < m; i++){
    if (nodes[i].key != NULL){
      dictionary_put(dictionary, nodes[i].key, nodes[i].value);
      free(nodes[i].key);
    }
  }
  free(nodes);
  return true;
}


bool dictionary_put(dictionary_t *dictionary, const char *key, void *value){
  float fn = (float)((float)dictionary->size/(float)dictionary->m);
  if (fn >= 0.7){
    bool validate = rehash(dictionary);
    if (!validate) return false;
  }

  size_t len = strlen(key);
  for (size_t i = 0; i < dictionary->m; i++){
    size_t pos = hashing(key, dictionary->m, i);

    if (dictionary->nodes[pos].key == NULL){
      char* copy = (char*) malloc(len+1);
      if (copy == NULL) return false;
      strcpy(copy, key);
      copy[len] = '\0';
      dictionary->nodes[pos].key = copy;
      dictionary->nodes[pos].value = value;
      dictionary->nodes[pos].is_deleted = false;
      dictionary->size++;
      return true;
    } else if (strcmp(dictionary->nodes[pos].key, key) == 0){
      if (dictionary->destroy != NULL) dictionary->destroy(dictionary->nodes[pos].value);
      dictionary->nodes[pos].value = value;
      return true;
    }
  }

  return false;
};


void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  for (size_t i = 0; i < dictionary->m; i++){
    size_t pos = hashing(key, dictionary->m, i);

    if (dictionary->nodes[pos].key != NULL && strcmp(dictionary->nodes[pos].key, key) == 0){
      *err = false;
      return dictionary->nodes[pos].value;
    } else if (dictionary->nodes[pos].key == NULL && dictionary->nodes[pos].is_deleted == false){
      *err = true;
      return NULL;
    }
  }
  *err = true;
  return NULL;
};


bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  for (size_t i = 0; i < dictionary->m; i++){
    size_t pos = hashing(key, dictionary->m, i);

    if (dictionary->nodes[pos].key != NULL && strcmp(dictionary->nodes[pos].key, key) == 0) {
      dictionary->nodes[pos].is_deleted = true;
      dictionary->size--;
      if (dictionary->destroy != NULL) dictionary->destroy(dictionary->nodes[pos].value);
      free(dictionary->nodes[pos].key);
      dictionary->nodes[pos].key = NULL;
      return true;
    } else if (dictionary->nodes[pos].key == NULL && dictionary->nodes[pos].is_deleted == false){
      return false;
    }
  }
  return false;
};


void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  for (size_t i = 0; i < dictionary->m; i++){
    size_t pos = hashing(key, dictionary->m, i);

    if (dictionary->nodes[pos].key != NULL && strcmp(dictionary->nodes[pos].key, key) == 0) {
      void* value = dictionary->nodes[pos].value;
      dictionary->nodes[pos].is_deleted = true;
      dictionary->size--;
      free(dictionary->nodes[pos].key);
      dictionary->nodes[pos].key = NULL;
      dictionary->nodes[pos].value = NULL;
      *err = false;
      return value;
    } else if (dictionary->nodes[pos].key == NULL && dictionary->nodes[pos].is_deleted == false){
      *err = true;
      return NULL;
    }
  }
  *err = true;
  return NULL;
};


bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  for (size_t i = 0; i < dictionary->m; i++){
    size_t pos = hashing(key, dictionary->m, i);
  
    if (dictionary->nodes[pos].key == NULL && dictionary->nodes[pos].is_deleted == false){
      return false;
    } else if (strcmp(dictionary->nodes[pos].key, key) == 0){
      return true;
    }
  }
  return false;
};


size_t dictionary_size(dictionary_t *dictionary) { return dictionary->size; };


void dictionary_destroy(dictionary_t *dictionary){
  for (size_t i = 0; i < dictionary->m; i++){
    if (dictionary->nodes[i].key != NULL) {
      free(dictionary->nodes[i].key);
      if (dictionary->destroy != NULL) dictionary->destroy(dictionary->nodes[i].value);
    };
  }
  free(dictionary->nodes);
  free(dictionary);
};
