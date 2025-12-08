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


LOCK_EX is exclusive lock. Generally used for writing data. 
LOCK_SH can be used if user wants to read data. 
Reading is allowed simultaneously. But writing is done exclusively.


Lock held by      Lock requested    Result for    Why? 
Process A	        by Process B		  Process B

LOCK_SH (Read)	  LOCK_SH (Read)	  Acquired	    It's safe for two readers.

LOCK_EX (Write)	  LOCK_SH (Read)	  Blocked	      Cannot read while a write is happening.

LOCK_SH (Read)	  LOCK_EX (Write)	  Blocked	      Cannot write while others are reading incomplete data.

