#include <stdio.h>//For prints
#include <unistd.h>//for lseek
#include <errno.h> //For errno
#include <string.h>//For strerror(errno)
#include <fcntl.h>//For open

int main(int argc, int argv[])
{
    char buf_ref[6] = {0};
    char buf[6] = {0};
    int fd = EINVAL;
    int new_pos = 0;
    int bytes_read = 0;

    fd = open("dummy.txt", O_CREAT|O_RDWR, 0666);
    if (fd == -1){
        printf("Opening the file failed\n");
        return errno;
    }
    //Read and print contents directly without seek.
    while ((bytes_read = read(fd, buf_ref, 6)) > 0) {

       //printf("B:%d\n", bytes_read);
        write(STDOUT_FILENO, buf_ref, bytes_read);
       //for (int i = 0; i<6; i++)
       //printf("%c", buf_ref[i]);
    }

    //write(STDOUT_FILENO, "Actual:\n", 8);
    //Read last 5 bytes of the file
    new_pos = lseek(fd,-5, SEEK_END);

    //memset(buf, 6, 0);
    bytes_read = read(fd, buf, 6);
    //if (bytes_read < 0) {
    //    printf("Reading failed\n");
    //    return errno;
   // }

    //Print the bytes read 
    printf("Bytes from file:\n%s", buf);
    
    //Close file
    close(fd);
    return 0;
}
