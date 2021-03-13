#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define true 1
#define MAX_PATH_LENGTH 50
#define MAX_ROWS 1000000

typedef struct
{
    int file_num;
    int thread_num;
    char * pathname;
}thread_data;

void * thread_func(void * arg)
{
    const thread_data * const data = (thread_data *)arg;

    char tPath[MAX_PATH_LENGTH];

    snprintf(tPath, MAX_PATH_LENGTH, "%sThread%d", data->pathname, data->thread_num);

    int fd = open(tPath, O_WRONLY | O_CREAT);

    if (fd == -1)
    {
        char errMsg[] = "Could not open a file!";

        write(STDERR_FILENO, errMsg, strlen(errMsg));
        _exit(EXIT_FAILURE);
    }

    int counter = 0;

    while(true)
    {
        for (int i = 0; i < MAX_ROWS; i++)
        {
            //TODO
        }
    }

    return (void *)NULL;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        char errMsg[] = "Missing command line arguments!";

        write(STDERR_FILENO, errMsg, strlen(errMsg));
        _exit(EXIT_FAILURE);
    }

    int threadNum = atoi(argv[1]);

    thread_data tData;

    tData.file_num = atoi(argv[2]);
    tData.pathname = (char *)malloc(strlen(argv[0]));

    strncpy(tData.pathname, argv[0], strlen(argv[0]));

    pthread_t tID[threadNum];

    for (int i = 0; i < threadNum; i++)
    {
        tData.thread_num = i;

        if (pthread_create(&tID[i], NULL, thread_func, (void *)&tData))
        {
            char errMsg[] = "Could not create a new thread!";

            write(STDERR_FILENO, errMsg, strlen(errMsg));
            _exit(EXIT_FAILURE);
        }
    }


    return 0;
}
