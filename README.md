algo:
when example program is run, it
acquires lock
writes data to a file
waits for 5s
releases lock

procedure:
run program in background
run program a second time

observe: second process is blocked until
lock is released by first process.
