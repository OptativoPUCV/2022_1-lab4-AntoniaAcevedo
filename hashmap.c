#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
    Pair ** buckets;                                                                                                                                                                                                                                                                       
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  Pair *valor = createPair(key,value);
  map -> current= hash(key,map -> capacity);
  while(map -> buckets[map -> current] != NULL)
  {
    map -> current ++;
    if (map -> current == map -> capacity)
      map -> current = 0;
  }
  map -> buckets[map -> current]= valor;
  map -> size ++;
  
}

void enlarge(HashMap * map) {
enlarge_called = 1; //no borrar (testing purposes)
Pair** buckets = (Pair**) calloc(map->capacity, sizeof(Pair*));
map->buckets = buckets;
map->size=0;
for (size_t i = 0; i < map -> capacity; i++)
    {
      if(map->buckets[i]!=NULL){
        if (map -> buckets[i] -> key!=NULL){
          insertMap(map,map -> buckets[i] -> key, map -> buckets[i] -> value);
        }
      }
    }
}


HashMap * createMap(long capacity) {
  
  HashMap * mapa =(HashMap*) malloc(sizeof(HashMap));
  mapa -> buckets= (Pair**)malloc(capacity* sizeof(Pair*));
  mapa -> current= -1;
  mapa -> capacity= capacity;
  mapa -> size= 0;
  for ( int i = 0; i < capacity ; i++ )
    {
      mapa -> buckets[i] = NULL;
    }
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {  
  
map -> current= hash(key, map -> capacity);
  if (map -> buckets[map -> current] -> key == NULL) return ;
    while(map -> buckets[map -> current] != NULL) {
      if (is_equal(key, map-> buckets[map -> current] -> key) == 1) {
        map -> buckets[map -> current] -> key = NULL;
        map -> size--;
        return ;
      }
      map -> current = (map -> current+1) % map -> capacity;
    }
    return;

}

Pair * searchMap(HashMap * map,  char * key) {   
  map -> current= hash(key,map -> capacity);
  while(map -> buckets[map -> current])
  {
    if (is_equal(map -> buckets[map -> current] -> key,key ) == 1){
            break;
        }
        else map -> current ++;    
    
  }
    return map -> buckets[map -> current];
}

Pair * firstMap(HashMap * map) {
    for (int i = 0; i < map -> capacity; i++){
        if (map -> buckets[i] != NULL ){
          if (map -> buckets[i]->key != NULL){
              map -> current = i;
              return map -> buckets[i];
          }
        }
    }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for (int i = map -> current + 1; i < map->capacity; i++){
    if (map -> buckets[i] != NULL ){
      if (map -> buckets[i] -> key != NULL){
        map -> current = i;
        return map -> buckets[i];
      }
    }
  }
  return NULL;

}
