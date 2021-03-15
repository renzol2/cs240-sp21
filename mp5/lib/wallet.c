#include "wallet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Allocate memory address for resources
  wallet->wallet_size = 0;
  wallet->resources = malloc(sizeof(resource_t) * 0);
  pthread_mutex_init(&wallet->lookup_lock, NULL);
}

/**
 * Gets resource with given name, or NULL if not found
 */
resource_t* get_resource(wallet_t *wallet, const char* resource) {
  for (int i = 0; i < wallet->wallet_size; i++) {
    // Check if there's a resource name equal to parameter
    char *current_resource = wallet->resources[i].name;

    // Return the resource's amount if they're the same
    if (strcmp(current_resource, resource) == 0) {
      return &wallet->resources[i];
    }
  }
  return NULL;
}

/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  resource_t* found_resource = get_resource(wallet, resource);

  if (found_resource != NULL) {
    return found_resource->amount;
  }

  return 0;
}

/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource
 * can be satisfied. (Ths function MUST WAIT until the wallet has enough
 * resources to satisfy the request; there are several ways to accomplish this
 * waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource,
                            const int delta) {
  pthread_mutex_lock(&wallet->lookup_lock);
  resource_t* r = get_resource(wallet, resource);
  
  // If resource is not found, create resource in wallet
  if (r == NULL) {
    // Update size and allocate space
    wallet->wallet_size += 1;
    wallet->resources = realloc(wallet->resources, sizeof(resource_t) * wallet->wallet_size);

    // Initialize new resource
    resource_t* new_resource = &wallet->resources[wallet->wallet_size - 1];
    new_resource->name = malloc(sizeof(char) * strlen(resource));
    strcpy(new_resource->name, resource);
    new_resource->amount = 0;
    new_resource->carryover = 0;
    
    r = new_resource;
  }

  pthread_mutex_unlock(&wallet->lookup_lock);

  pthread_mutex_lock(&wallet->lookup_lock);

  // If taking resource, check that there's enough amount
  if (delta < 0) {
    int difference = r->amount + delta;
    // If there's too much requested, only take possible amount and store carryover
    if (difference < 0) {
      // Difference is negative so we * -1 to get a positive value
      r->carryover += -difference;
      r->amount = 0;
    } else {
      // If difference > 0, it's safe to subtract from amount
      // Delta is negative so we add
      r->amount += delta;
    }
  } else {
    // Always add positive delta amounts
    r->amount += delta;

    // Pay off some carryover if applicable
    int carryover = r->carryover;
    if (carryover == 0) {
      pthread_mutex_unlock(&wallet->lookup_lock);
      return;
    } 

    // If carryover is still more 
    if (carryover > r->amount) {
      int new_carryover = carryover - r->amount;
      r->carryover = new_carryover;
      r->amount = 0;
    } else {
      // If carryover is leq, we can remove the carryover
      r->carryover = 0;
      r->amount -= carryover;
    }
  }

  pthread_mutex_unlock(&wallet->lookup_lock);

}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  for (int i = 0; i < wallet->wallet_size; i++) {
    free(wallet->resources[i].name);
  }
  free(wallet->resources);
  wallet->wallet_size = 0;
}
