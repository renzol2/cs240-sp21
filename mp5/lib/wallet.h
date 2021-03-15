#pragma once

#include <pthread.h>

/**
 * Resource struct containing a resource name and associated amount
 */
typedef struct resource_t_ {
  char* name;
  int amount;
  int carryover;
} resource_t;

/**
 * Wallet struct containing an array of resources
 */
typedef struct wallet_t_ {
  pthread_mutex_t lookup_lock;
  resource_t* resources;
  int wallet_size;
} wallet_t;

void wallet_init(wallet_t *wallet);
int wallet_get(wallet_t *wallet, const char *resource);
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta);
void wallet_destroy(wallet_t *wallet);
