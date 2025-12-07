#include<stdio.h>
#include<sys/file.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    int fd;
    //create open a file
    fd = open("locked_file.txt",O_RDWR|O_APPEND|O_CREAT,0666);
    if (fd == -1)
    {   
        printf("File open failed\n");
        return -1;
    }
    //Use the file descriptor and lock the file
    if(flock(fd, LOCK_EX)==-1)
    {
        printf("Failed to obtain lockf\n");
        close(fd);
        return -1;
    }
    sleep(5);
    //Write into the file..and wait for 5s(just for experimentation)
    dprintf(fd, "wrting to the file from %d", getpid());
    //Release lock
    if(flock(fd, LOCK_UN)==-1)
    {
        printf("Unlocking failed\n");
    }
    //close the file desc.
    close(fd);
    return 0;

}
