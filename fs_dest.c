#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define true 1
#define MAX_PATH_LENGTH 50
#define DELAY 1000

char * pathname;
int file_num;
int rows;

void * thread_func(void * arg)
{
    int thread_num = (int)arg;

    char tPath[MAX_PATH_LENGTH];

    snprintf(tPath, MAX_PATH_LENGTH, "%sThread%d", pathname, thread_num);

    char cPath[MAX_PATH_LENGTH];

    strncpy(cPath, tPath, MAX_PATH_LENGTH);

    int counter = 1;

    while(true)
    {
        int fd = open(cPath, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

        if (fd == -1)
        {
            char errMsg[] = "Could not open a file!";

            write(STDERR_FILENO, errMsg, strlen(errMsg));
            _exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rows; i++)
        {
            char buff[50];

            time_t t =  time(NULL);
            char * temp = ctime(&t); //ctime function may add newline at the end of this returned string

            snprintf(buff, sizeof (buff), "The Linux programming interface %s", temp);

            if (write(fd, buff, strlen(buff)) == -1)
            {
                char errMsg[] = "Could not write to a file!";

                write(STDERR_FILENO, errMsg, strlen(errMsg));
                _exit(EXIT_FAILURE);
            }
            usleep(DELAY);
        }

        if (close (fd))
        {
            char errMsg[] = "Could not close a file!";

            write(STDERR_FILENO, errMsg, strlen(errMsg));
            _exit(EXIT_FAILURE);
        }

        snprintf(tPath, MAX_PATH_LENGTH, "%sThread%d.%d", pathname, thread_num, counter);

        if (!access(tPath, F_OK))
        {
           if (unlink(tPath))
           {
               char errMsg[] = "Could not unlink a file!";

               write(STDERR_FILENO, errMsg, strlen(errMsg));
               _exit(EXIT_FAILURE);
           }
        }

        if (rename(cPath, tPath))
        {
            char errMsg[] = "Could not rename the file!";

            write(STDERR_FILENO, errMsg, strlen(errMsg));
            _exit(EXIT_FAILURE);
        }

        counter = (counter + 1) % file_num;
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

    else if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            char helpMsg[] = "[pathname][thread number][file number][row number]";
            write(STDOUT_FILENO, helpMsg, strlen(helpMsg));
            _exit(EXIT_SUCCESS);
        }

        char errMsg[] = "Missing command line arguments!";

        write(STDERR_FILENO, errMsg, strlen(errMsg));
        _exit(EXIT_FAILURE);
    }

    int threadNum = atoi(argv[2]);

    file_num = atoi(argv[3]);
    pathname = argv[1];
    rows = atoi(argv[4]);

//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, 1);

    pthread_t tID;
    for (int i = 0; i < threadNum - 1; i++)
    {
        if (pthread_create((void *)&tID, (void *)NULL, thread_func, (void *)i))
        {
            char errMsg[] = "Could not create a new thread!";

            write(STDERR_FILENO, errMsg, strlen(errMsg));
            _exit(EXIT_FAILURE);
        }
    }

    thread_func((void *)threadNum - 1);

    return 0;
}
