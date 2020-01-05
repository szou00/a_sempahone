#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>

#define KEY_1 24601
#define KEY_2 24602

int shmid;
int shmd;
// int fd;
// union semun sm;
struct sembuf semaphore;

int main() {
  printf("trying to get in\n");
  //checking semaphore
  shmid = semget(KEY_2, 1, 0);
  if (shmid < 0) {
    printf("semaphore error %d: %s\n", errno, strerror(errno));
    return errno;
  }

  semop(shmid, &semaphore, 1);

  //checking memory
  shmd = shmget(KEY_1, sizeof(char *), 0);
  if (shmd < 0) {
    printf("memory error %d: %s\n", errno, strerror(errno));
    return errno;
  }

  //opening file
  FILE *fd = fopen("file.txt", "a");
  //printing last addition
  char *last_line = shmat(shmd, 0, 0);
  printf("This was your last addition: %s\n", last_line);
  char next_line[1000];
  //getting next line from user
  printf("Type in your next addition: ");
  fgets(next_line, 1000, stdin);
  fprintf(fd, "%s", next_line);

  fclose(fd);
  //release memory
  strcpy(last_line, next_line);
  shmdt(last_line);
  semaphore.sem_op = 1;
  //release semaphore
  semop(shmd, &semaphore, 1);
  return 0;

}
