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

#define KEY 24601
#define KEY_2 24602
#define SHSIZE 200

int shmid;
char *shm;
int fd;
union semun sm;
// union semun {
//   int val;
//   struct semid_ds *buf;
//   unsigned short *arr;
//   struct seminfo *_buf;
// }

int create() {
  int shmid;
  char *shm;
  int fd;
  union semun sm;

  //semaphore
  shmid = shmget(KEY, SHSIZE, IPC_CREAT | 0644);
  if (shmid < 0) {
    printf("SEMAPHORE error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("semaphore created\n");

  semctl(shmid, 0, SETVAL, sm);
  sm.val = 1;

  //memory
  shm = shmat(shmid, NULL, 0);
  if (shm == (char *) -1) {
    printf("MEMORY error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("shared memory created\n");

  //file
  fd = open("file.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd == -1) {
    printf("FILE error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  close(fd);
  printf("file created\n");
  return 0;
}

int view() {
  FILE *fd = fopen("file.txt", "r");
  char lines;
  if (fd == NULL) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  lines = fgetc(fd);
  printf("the story so far:\n");
  while (lines != EOF) {
    printf("%c", lines);
    lines = fgetc(fd);
  }
  fclose(fd);
  return 0;
}

int removing() {
  printf("checking if semaphore is available\n");
  shmid = semget(KEY, 1, 0); //checks if semaphore is open
  if (shmid < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("available!\n");
  printf("tring to get in\n");

  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_num = -1;
  semop(shmid, &semaphore,1);

  view(); //display contents

  //removing memory
  int shmd;
  shmd = shmget(KEY, SHSIZE, 0);
  if (semctl(shmd, IPC_RMID, 0)) {
    printf("SHARED MEMORY error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("shared memory removed\n");

  //removing semaphore
  if (semctl(shmd, IPC_RMID, 0)) {
    printf("SEMAPHORE error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("semaphore removed\n");

  //removing file
  if (remove("file.txt")) {
    printf("FILE error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("file removed\n");

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("please type in two arguments\n");
  }
  if (argc == 2) {
    if (strcmp(argv[1],"-c") == 0) {
      create();
    }
    else if (strcmp(argv[1],"-v") == 0) {
      view();
    }
    else if (strcmp(argv[1],"-r") == 0) {
      printf("removing\n");
      removing();
    }
    else
      printf("please make sure to type in -c, -v, or -r only\n");
    }
  return 0;
}
