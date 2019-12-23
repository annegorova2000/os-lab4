#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <pthread.h>
#include "list.h"

// actions
#define EXIT        0
#define APPEND      1
#define TOP         2
#define POP         3
#define PRINT       4

#define STACK_SIZE  100

typedef int struct_type;
typedef struct {
  pthread_mutex_t toserv;
  pthread_mutex_t fromserv;

  int type;           // action type
  size_t obQty;
  struct_type objects[STACK_SIZE];
} message;

void errorDie(const char *msg) {
  perror(msg);
  exit(3);
}

void newLine(FILE* fl) {
  fprintf(fl, "\n");
}

void initMutexes(message* msg) {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&msg->toserv, &attr);
  pthread_mutex_init(&msg->fromserv, &attr);
}

int main(int argc, char *argv[]) {
  int err;

  const char *memname = "sample";
  const size_t region_size = sysconf(_SC_PAGE_SIZE);

  // create firtual file
  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1)
    errorDie("shm_open");

  err = ftruncate(fd, region_size);
  if (err != 0)
    errorDie("ftruncate");

  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_SHARED | MAP_ANONYMOUS;
  message* msg = (message*)mmap(NULL, region_size, prot, flags, fd, 0);
  if (msg == MAP_FAILED)
    errorDie("mmap");
  close(fd);

  int num = 2;
  pid_t pid = fork();
  
  msg->obQty = 0;

  initMutexes(msg);
  pthread_mutex_lock(&msg->fromserv);
  pthread_mutex_lock(&msg->toserv);

  if (pid == 0) {
    fprintf(stdout, "child enter\n");
    int init = 5;
    int obQty;
    int listSize;
    List* list = list_create();
    for (;;) {
      pthread_mutex_lock(&msg->toserv);
      listSize = list->top + 1;
      obQty = 0;

      // PROCESSING
      /* fprintf(stdout, "(child)msg->type = %d\n", msg->type); */
      switch (msg->type) {
        case APPEND:
          printf("append\n");
          list_push(list, msg->objects[0]);
          break;
        case POP:
          printf("pop\n");
          list_pop(list);
          break;
        case TOP:
          printf("top\n");
          obQty = 1;
          msg->objects[0] = list_peek(list);
          break;
        case PRINT:
          printf("print\n");
          obQty = listSize;
          for (int i = listSize; i >= 0; --i) {
            msg->objects[i] = list->data[i];
          }
          break;
        case EXIT:
          printf("exit\n");
          exit(EXIT);
          break;
        default:
          printf("no such type\n");
          break;
      }
      msg->obQty = obQty;
      // END OF PROCESSING
      
      pthread_mutex_unlock(&msg->fromserv);
    }
  }
  else if (pid > 0) {
    fprintf(stdout, "parent enter\n");
    int action;
    int toAppend;
    for (;;) {
      // IO
      printf("enter num: 1. append, 2. top, 3. pop, 4. print, 0. exit\n");
      if (scanf("%d", &action) != 1) {
        errorDie("invalid read into action\n");
      }
      msg->type = action;
      if (action == APPEND) {
        fprintf(stdout, "enter num to append:\n");
        if (fscanf(stdin, "%d", &toAppend) != 1)
          errorDie("invalid read into toAppend\n");
        msg->obQty = 1;
        msg->objects[0] = toAppend;
      } else if (action == EXIT) {
        pthread_mutex_unlock(&msg->toserv);
        wait(EXIT); 
        exit(EXIT);
      }
      // END OF IO
      
      pthread_mutex_unlock(&msg->toserv);
      pthread_mutex_lock(&msg->fromserv);
      // OUTPUTTING RESULTS
      for (int i = 0; i < msg->obQty; ++i) {
        printf("%d ", msg->objects[i]);
      } newLine(stdout);
    }
  } else {
    exit(1);
  }

  err = munmap(msg, region_size);
  if (err != 0)
    errorDie("munmap");

  err = shm_unlink(memname);
  if (err != 0)
    errorDie("shm_unlink");

  return 0;
}
