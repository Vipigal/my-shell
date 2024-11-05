#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PROC 20

struct Process {
  pid_t pid;
  char *user;
  char *procname;
  char state;
};

void *print_top(void *arg) {
  struct Process *top = (struct Process *)arg;
  int counter = 0;
  while (1) {
    if (counter % 300 == 3) {
      top[1].pid = 40;
      top[1].state = 'W';
    }
    system("clear");
    printf("PID     | User           | PROCNAME       | Estado |\n");
    printf("--------|----------------|----------------|--------|\n");
    for (int i = 0; i < MAX_PROC; i++) {
      if (top[i].pid != 0) {
        printf("%-8d| %-15s| %-15s| %-6c |\n", top[i].pid, top[i].user,
               top[i].procname, top[i].state);
        fflush(stdout);
      }
    }
    counter++;
    sleep(1);
  }
  return NULL;
}

void populate_top(void *arg) {
  struct Process *top = (struct Process *)arg;
  for (int i = 0; i < 5; i++) {
    top[i].pid = i;
    top[i].user = "vinic";
    top[i].procname = "proc";
    top[i].state = 'S';
  }
}

pthread_t initialize_thread(struct Process *top) {
  pthread_t tid;
  pthread_create(&tid, NULL, print_top, top);
  return tid;
}

int main(int argc, char *argv[]) {
  struct Process top[MAX_PROC] = {};
  populate_top(top);
  initialize_thread(top);

  char buffer[256];
  while (1) {
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      buffer[strcspn(buffer, "\n")] = 0;
      printf("Você digitou: %s\n", buffer);
    }
  }

  return EXIT_SUCCESS;
}
