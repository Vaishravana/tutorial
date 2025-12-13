/* fcntl works on fd, you can read, write too.
 * It uses fcntl(fd, <cmd>, <cmd args>) approach
 * 
 * Use fcntl to coordinate.
 * In this example, fork and spawn a child.
 * From parent,lock 100 bytes, write into file(write system time), and wait for 5s,
 * just to ensure lock is still held for 5s. Then release lock.
 * In child, lock next 100bytes, write the data(write system time)
 *
 * Observation:
 * Child was able to write to the file, without waiting for whole 5s.
 * */
#include <stdio.h> //For prints
#include <fcntl.h> //For fcntl
#include <unistd.h> //For sleep,fork
#include <sys/types.h>//for pid
#include <sys/wait.h>//for wait

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if(pid<0) {
        printf("fork failed\n");
        return -1;
    } else if (pid==0) {
        int flags;
        struct flock fl;
        printf("Child process pid=%d\n", getpid());
        //algo
        //open the file, create if not present, append if already present
        int fd;
        fd = open("locked_file.txt", O_CREAT|O_RDWR|O_APPEND, 0666);
        if (fd == -1)
        {
            printf("file open failed\n");
            //error handling;
            return -1;
        }
        //lock the file granularly
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 100;
        fl.l_len = 100;
        fl.l_pid = getpid();

        if (fcntl(fd, F_SETLK, &fl) == -1)
            printf("Locking failed from child pid=%d\n", getpid());
        sleep(2);
        //write to the file
        dprintf(fd, "Locked section=%d\n", getpid());
        //sleep(5)
        sleep(5);
        //release lock
        fl.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &fl);
        //Handling maybe
        //close file
        close(fd);
    }
    else{
        printf("Parent process pid=%d", getpid());
        int flags;
        int fd;
        struct flock fl;
        printf("Parent process pid=%d\n", getpid());
        //algo
        //open the file, create if not present, append if already present
        fd = open("locked_file.txt", O_CREAT|O_RDWR|O_APPEND, 0666);
        if (fd == -1)
        {
            printf("file open failed\n");
            //error handling;
            return -1;
        }
        //lock the file granularly
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 0;
        fl.l_len = 100;
        fl.l_pid = getpid();

        if (fcntl(fd, F_SETLK, &fl) == -1)
            printf("Locking failed from parent pid=%d\n", getpid());
        //write to the file
        dprintf(fd, "Writing to file from parent pid=%d\n", getpid());
        //Do not sleep(5)
        //release lock
        fl.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &fl);
        //Handling maybe
        //close file
        close(fd);

        //Wait till child runs to completion
        wait(NULL);
    }

    return 0;
}

