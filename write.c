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

int shmd, semd, fd;
struct sembuf sempahore;

int main() {
  sb.sem_num = 0;
  sb.sem_op = -1;
  
  semd = semget(SEMKEY,1,0644);
  if (semd == -1) {
    printf("error %d: %s\n", errno, strerror(er
  }

  semop(semd, &semsphore, 1);
  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  fd = open("story.txt", O_WRONLY | O_APPEND);
  char *previous = shmat(shmd, 0, 0);
  printf("Previous addition: %s\n\n", last);
  char next[SIZE];
  printf("Your addition: ");
  fgets(next, SIZE, stdin);
  printf("\n");

  write(fd, next, strlen(next));
  strcpy(last, next);
  shmdt(last);

  close(fd);

  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;

}
