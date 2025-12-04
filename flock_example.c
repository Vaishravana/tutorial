#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

int main() {
    int fd;
    const char* file_path = "lock_example.txt";

    // 1. Open the file
    // O_RDWR: Open for reading and writing
    // O_CREAT: Create the file if it doesn't exist
    fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    printf("Attempting to acquire an exclusive lock on %s...mypid=%d \n", file_path, getpid());

    // 2. Acquire an exclusive lock (blocking operation by default)
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Lock acquired. Writing to file...mypid=%d\n", getpid());

    // 3. Perform file I/O while holding the lock
    dprintf(fd, "Data protected by exclusive lock from PID %d.\n", getpid());

    printf("Finished writing. Holding the lock for 5 seconds (check with another process)..mypidk=%d\n",getpid());
    sleep(5); // Hold lock for 5 seconds so you can test concurrency

    // 4. Release the lock
    if (flock(fd, LOCK_UN) == -1) {
        perror("flock unlock failed");
    }

    printf("Lock released. Closing file. mypid=%d\n", getpid());

    // 5. Close the file descriptor (locks are also automatically released on close/exit)
    close(fd);

    return EXIT_SUCCESS;
}

