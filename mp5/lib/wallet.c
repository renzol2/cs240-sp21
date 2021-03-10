#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wallet.h"

/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  wallet->head = NULL;
  pthread_mutex_init(&wallet->lock, NULL);
}

/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  wallet_entry_t *thru = wallet->head;

  while (thru != NULL) {
    if (strcmp(thru->key, resource) == 0) {
      return thru->amount;
    }

    thru = thru->next;
  }

  return 0;
}

int _try_wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  wallet_entry_t *thru = wallet->head;

  while (thru != NULL) {
    if (strcmp(thru->key, resource) == 0) {
      if (thru->amount + delta >= 0) {
        thru->amount = thru->amount + delta;
        return 1;
      }

      // Would go negative, can't allocate
      return 0;
    }

    thru = thru->next;
  }

  // Not found
  if (delta >= 0) {
    // If positive, add to beginning:
    wallet_entry_t *entry = malloc(sizeof(wallet_entry_t));
    entry->key = resource;
    entry->amount = delta;
    entry->next = wallet->head;

    wallet->head = entry;
    return 1;
  }

  // If negative, we can't do anything:
  return 0; 
}


/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  int success = 0;

  while (!success) {
    pthread_mutex_lock(&wallet->lock);
    success = _try_wallet_change_resource(wallet, resource, delta);
    pthread_mutex_unlock(&wallet->lock);
  }

}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {

}
