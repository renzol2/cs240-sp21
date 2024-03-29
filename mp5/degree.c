#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "lib/wallet.h"

wallet_t wallet;


void *job_clover_patch() {
  int i = 0;
  for (i = 0; i < 100; i++) {
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "four-leaf-clover", 1); fprintf(stderr, "🍀");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    if (rand() % 10 == 0) {
      wallet_change_resource(&wallet, "four-leaf-clover", 1); fprintf(stderr, "🍀");
    }
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
    wallet_change_resource(&wallet, "clover", 1); fprintf(stderr, "☘️");
  }

  return NULL;
}

void *job_orchard() {
  int i = 0;
  for (i = 0; i < 110; i++) {
    wallet_change_resource(&wallet, "green-apple", 1);
    fprintf(stderr, "🍏");
  }

  return NULL;
}

void *job_workshop() {
  int i = 0;
  for (i = 0; i < 520; i++) {
    wallet_change_resource(&wallet, "tools", 1);
    fprintf(stderr, "🧰");

    if (i % 5 == 0) {
      wallet_change_resource(&wallet, "gem", 1);
      fprintf(stderr, "💎");
    }
  }

  return NULL;
}

void *job_dna() {
  int i = 0;
  for (i = 0; i < 1750; i++) {
    wallet_change_resource(&wallet, "dna", 1);
    fprintf(stderr, "🧬");
  }

  return NULL;
}

void *job_research_green() {
  // 📗 requires 1x🍏 1x🍀 10x☘️ 5x🧬
  int i = 0;
  for (i = 0; i < 100; i++) {
    wallet_change_resource(&wallet, "green-apple", -1);
    wallet_change_resource(&wallet, "four-leaf-clover", -1);
    wallet_change_resource(&wallet, "clover", -10);
    wallet_change_resource(&wallet, "dna", -5);
    wallet_change_resource(&wallet, "green-book", 1);
    fprintf(stderr, "📗");
  }
  return NULL;  
}

void *job_research_blue() {
  // 📘 requires 10x🧬 1x💎
  int i = 0;
  for (i = 0; i < 100; i++) {
    wallet_change_resource(&wallet, "dna", -10);
    wallet_change_resource(&wallet, "gem", -1);
    wallet_change_resource(&wallet, "blue-book", 1);
    fprintf(stderr, "📘");
  }
  return NULL;  
}

void *job_research_orange() {
  // 📙 requires 5x🧰 2x🧬
  int i = 0;
  for (i = 0; i < 100; i++) {
    wallet_change_resource(&wallet, "tools", -5);
    wallet_change_resource(&wallet, "dna", -2);
    wallet_change_resource(&wallet, "orange-book", 1);
    fprintf(stderr, "📙");
  }
  return NULL;  
}

void *job_combine_research() {
  // 📚 requires 1x📗, 1x📘, 1x📙
  int i = 0;
  for (i = 0; i < 100; i++) {
    wallet_change_resource(&wallet, "orange-book", -1);
    wallet_change_resource(&wallet, "blue-book", -1);
    wallet_change_resource(&wallet, "green-book", -1);
    wallet_change_resource(&wallet, "books", 1);
    fprintf(stderr, "📚");
  }
  return NULL;    
}

void *job_graduation() {
  // 🎓 requires 100x 📚 
  wallet_change_resource(&wallet, "books", -100);
  wallet_change_resource(&wallet, "degree!", 1);
  fprintf(stderr, "🎓");
  return NULL;
}



int main() {
  const int NUM_ROUNDS = 1000;
  int total_degrees = 0;
  for (int i = 0; i < NUM_ROUNDS; i++) {
    wallet_init(&wallet);
    srand (time(NULL));

    pthread_t tids[100];
    fprintf(stderr, "Resources Generated: ");
    pthread_create(&tids[0], NULL, job_graduation, NULL);
    pthread_create(&tids[1], NULL, job_combine_research, NULL);
    pthread_create(&tids[2], NULL, job_research_blue, NULL);
    pthread_create(&tids[3], NULL, job_research_orange, NULL);
    pthread_create(&tids[4], NULL, job_research_green, NULL);
    pthread_create(&tids[5], NULL, job_dna, NULL);
    pthread_create(&tids[6], NULL, job_workshop, NULL);
    pthread_create(&tids[7], NULL, job_clover_patch, NULL);
    pthread_create(&tids[8], NULL, job_orchard, NULL);
    const int totalJobs = 9;

    int i;
    for (i = 0; i < totalJobs; i++) {
      pthread_join(tids[i], NULL);
    }
    fprintf(stderr, "\n");

    if (wallet_get(&wallet, "degree!") == 1) {
      printf("Your wallet contains a degree! 🎉\n");
      printf("- Extra 🧬: %d\n", wallet_get(&wallet, "dna"));
      printf("- Extra 🍀: %d\n", wallet_get(&wallet, "four-leaf-clover"));
      total_degrees++;
    } else {
      printf("Yikes -- your wallet may not have that degree yet... :(\n");
    }

    wallet_destroy(&wallet);
  }
  
  printf("Total degrees: %d\n", total_degrees);

  return 0;
}
