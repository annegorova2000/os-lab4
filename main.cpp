#include "lin-2-list-barrier.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#define LISTS_NUM 10

int list_no(int *st);

typedef struct
{
    int stor[1024];
    pthread_mutex_t mutex;
} shared_data;

static shared_data* data = NULL;

void initialise_shared()
{
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;
    data = (shared_data*)mmap(NULL, sizeof(shared_data), prot, flags, -1, 0);
    if (data == MAP_FAILED) {
        exit(-3);
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&data->mutex, &attr);
}

int run_parent()
{
    char c;
    char fb;
    int st;
    int no;
    int val;
    List *A[LISTS_NUM];
    for (int i = 0; i < LISTS_NUM; i++) {
        A[i] = NULL;
    }
    while (1){
        sleep(1);
        pthread_mutex_lock(&data->mutex);
        scanf("%c", &c);
        switch (c) {
            case 'c':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    A[st] = list_create();
                    data->stor[0] = 0;
                } else {
                    data->stor[0] = -1;
                }
                break;
            case 'd':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] != NULL) {
                    list_destroy(&A[st]);
                    data->stor[0] = 0;
                } else {
                    data->stor[0] = -1;
                }
                break;
            case 'a':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                if (scanf(" %c", &fb) != 1) {
                    data->stor[0] = -1;
                    break;
                }
                switch (fb) {
                    case 'b':
                        while (scanf("%d", &val) == 1) {
                            list_push_front(A[st], val);
                            c = getchar();
                            if (getchar() == '\n') {break;}
                        }
                        data->stor[0] = 0;
                        break;
             
                    case 'f':
                        while (scanf("%d", &val) == 1) {
                            list_push_back(A[st], val);
                            c = getchar();
                            if (getchar() == '\n') {break;}
                        }
                        data->stor[0] = 0;
                        break;
                    case 'i':
                        if (scanf("%d %d",&no , &val) != 2) {
                            data->stor[0] = -1;
                            break;
                        }
                        if (list_insert(A[st], no, val)) {
                            data->stor[0] = -1;
                        } else {
                            data->stor[0] = 0;
                        }
                        break;
                    default:
                        data->stor[0] = -1;
                        break;
                }
                break;
            case 'o':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                if (scanf(" %c", &fb) != 1) {
                    data->stor[0] = -1;
                    break;
                }
                if (list_size(A[st]) == 0) {
                    data->stor[0] = -1;
                    break;
                }
                switch (fb) {
                    case 'b':
                        data->stor[0] = 1;
                        data->stor[1] = list_pop_front(A[st]);
                        break;
                    case 'f':
                        data->stor[0] = 1;
                        data->stor[1] = list_pop_back(A[st]);
                        break;
                    case 'i':
                        if (scanf("%d",&no) != 1) {
                            data->stor[0] = -1;
                            break;
                        }
                        if (abs(no) > list_size(A[st]) || no == 0) {
                            data->stor[0] = -1;
                        } else {
                            data->stor[0] = 1;
                            data->stor[0] = list_get_out(A[st], no);
                        }
                        break;
                    case 'p':
                        if (scanf("%d",&no) != 1) {
                            data->stor[0] = -1;
                            break;
                        }
                        if (abs(no) > list_size(A[st]) || no == 0) {
                            data->stor[0] = -1;
                        } else {
                            data->stor[0] = 1;
                            data->stor[0] = list_peak(A[st], no);
                        }
                        break;
                    default:
                        data->stor[0] = -1;
                        break;
                }
                break;
            case 'r':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                if (scanf("%d",&no) != 1) {
                    data->stor[0] = -1;
                    break;
                }
                if (list_remove(A[st], no)) {
                    data->stor[0] = -1;
                } else {
                    data->stor[0] = 0;
                }
                break;
            case 'p':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                data->stor[0] = list_size(A[st]);
                list_print(A[st], data->stor);
                break;
            case 'q':
                for (int i = 0; i < 10; i++) {
                    if (A[i] != NULL) {
                        list_destroy(&A[i]);
                    }
                }
                data->stor[0] = -2;
                return 0;
                break;
            case 's':
                if (list_no(&st)) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                data->stor[0] = 1;
                data->stor[1] = list_size(A[st]);
                break;
            case ' ':
                break;
            case '\n':
                break;
            default:
                data->stor[0] = -1;
        }
        while(c != '\n') {
            c = getchar();
        }
        pthread_mutex_unlock(&data->mutex);
    }
}

int list_no(int *st)
{
    if (scanf("%d", st) != 1) {return 1;}
    return *st > LISTS_NUM ? 1 : 0;
}


void run_child()
{
    while (true) {
        sleep(1);
        pthread_mutex_lock(&data->mutex);
        std::cout << "Mesage no: " << data->stor[0] << '\n';
        if (data->stor[0] == -2) {
            break;
        }
        for (int i = 1; i <= data->stor[0]; i++) {
            //read(pipeFdFromServ[0], &inMsg, sizeof(int));
            std::cout << data->stor[i] << '\n';
        }
        pthread_mutex_unlock(&data->mutex);
    }
}

int main(int argc, char** argv)
{
    initialise_shared();

    pid_t serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        exit(-1);
    } else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        run_child();
    } else {
        run_parent();
    }

    munmap(data, sizeof(data));
    return 0;
}
