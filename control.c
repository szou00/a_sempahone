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
#define SEG_SIZE 200

int semd, shmd, file;
char flag[SEG_SIZE];
struct sembuf sb;
int fd;

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *arr;
  struct seminfo *_buf;
}

int create() {
  union semun semaphore;

  semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    semd = semget(KEY, 1, 0);
    semctl(semd, 0, SETVAL, semaphore.val);
    printf("semctl returned: %d\n", v);
    return errno;
  }
  printf("created semaphore\n");
  sempahore.val = 1;
  semctl(semd, 0, SETVAL, semaphore);
  // printf("semctl returned: %d\n", r);

  shmd = shmget(SHKEY, size of(* char), IPC_CREAT | IPC_EXCL | 0644);
  if (shmd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("created shared memory");
  fd = open("story.txt", 0_CREAT | 0_TRUNC, 0644);
  if (fd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  printf("created file");
  close(fd);

  return 0;
}

int remove() {
  view();
  semd = semget(SEMKEY, 1, 0644);
  shmd = semget(SHR_KEY, 0, 0644);
  if (semd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  else {
    printf("tring to get in\n");
    semaphore.sem_num = 0;
    semaphore.sem_num = -1;
    semop(semd, &sempahore,1);
  }

  shmd = smget(SHKEY, sizeof())

    // view();
    // remove("story.txt");
    // int shmid = shmget(KEY2, sizeof(int), 0);
    // int what = shmctl(shmid, IPC_RMID, NULL);
    // int semid = semget(KEY, 1, 0);
    // semctl(semid, 0, IPC_RMID);
    // printf("Removed!\n");
}

int view() {
  int fd = open("story.txt", 0_RDONLY);
  if (fd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return errno;
  }
  char * file = calloc(2056, sizeof(char));
    int last = read(fd, file, 2056);
    if (last == -1){
        printf("error %d: %s\n", errno, strerror(errno));
        return;
    }
  char * lines = strrchr(file, '\n');
    if (lines){
       *lines = 0;
     }
  printf("%s\n", file);
  free(file);
  close(fd);
  return;
}

int main() {
  int sem_id;
  if (argc == 1) {
    printf("Wrong arguments\n");
  }
  if (argc == 2) {
    if (strcmp(argv[1],"-c") == 0) {
      create();
    }
    else if (strcmp(argv[1],"-v") == 0) {
      view();
    }
    else if (strcmp(argv[1],"-r") == 0) {
      delete();
    }
    else
      printf("Wrong arguments\n");
    }
  return 0;
}
