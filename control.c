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
int fd;
union semun sm;
struct sembuf semaphore;
// union semun {
//   int val;
//   struct semid_ds *buf;
//   unsigned short *arr;
//   struct seminfo *_buf;
// }

int create() {

  //semaphore
  shmid = semget(KEY_2, 1, IPC_CREAT | 0644);
  if (shmid < 0) {
    printf("SEMAPHORE error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("semaphore created\n");

  semctl(shmid, 0, SETVAL, sm);
  // sm.val = 1;
  // shmid = semget(KEY, 1, 0);

  //memory
  shmd = shmget(KEY_1, sizeof(char *), IPC_CREAT | 0644);
  if (shmd < 0) {
    printf("MEMORY error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("shared memory created\n");

  //file
  fd = open("file.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd < 0) {
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
  shmid = semget(KEY_2, 1, 0); //checks if semaphore is open
  if (shmid < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  // printf("available!\n");
  // printf("tring to get in\n");

  // semaphore.sem_num = 0;
  // semaphore.sem_num = -1;
  semop(shmid, &semaphore,1);

  //printing contents
  view();

  shmd = shmget(KEY_1, 1, 0);
  if (shmd < 0) {
    printf("Trying to viewing the file you got shared memory error %d: %s\n", errno, strerror(errno));
    return errno;
  }

  //removing memory
  if (shmctl(shmd, IPC_RMID, 0) == -1) {
    printf("in removing u have shared memory error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("\nshared memory removed\n");

  //removing semaphore
  if (semctl(shmid, IPC_RMID, 0) == -1) {
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
    printf("Please type in two arguments\n");
  }
  if (argc == 2) {
    if (strcmp(argv[1],"-c") == 0) {
      create();
    }
    else if (strcmp(argv[1],"-v") == 0) {
      view();
    }
    else if (strcmp(argv[1],"-r") == 0) {
      // printf("removing\n");
      removing();
    }
    else
      printf("Please make sure to type in -c, -v, or -r only\n");
    }
  return 0;
}
