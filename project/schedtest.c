#include "types.h"
#include "stat.h"
#include "user.h"

#define N 5

void long_running_task() {
  int i, j;
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 50; j++) {
      sleep(1); // Loop to simulate a long-running task
    }
  }
}

int main(void) {
  int pid;
  int initial_priority[] = {0, 1, 2, 2, 1};

  for (int i = 0; i < N; i++) {
    pid = fork();

    if (pid == 0) { // Child process
      renice(initial_priority[i]);

      long_running_task();
      printf(1, "Process %d (priority %d) completed\n", getpid(), initial_priority[i]);
      exit();
    } else if (pid < 0) {
      printf(2, "Error: Fork failed\n");
      exit();
    }
  }

  // Parent process waits for all children to complete
  for (int i = 0; i < N; i++) {
    wait();
  }

  exit();
}
