# Answers

## Q1

The difference in timing methods indicates that gethosttime() show
a more tight bound, lower standard deviation in most runs.

The reason being gethosttime() uses cycles to simply calculate the
time passed which is closest to the actual CPU frequency, that's why
the standard deviation is minimal, in contrast to gettimeofday() which
uses a syscall which involves context switching which is slower - although
by loading gettimeofday() to vsyscall area the resulting call does not perform
context switch. Still the resulsts of gethosttime() have lower std deviation.

## Q2

### (a)

```
558e09db9000-558e09dbb000 r--p 00000000 08:01 1566                       /usr/bin/cat
558e09dbb000-558e09dc0000 r-xp 00002000 08:01 1566                       /usr/bin/cat
558e09dc0000-558e09dc3000 r--p 00007000 08:01 1566                       /usr/bin/cat
558e09dc3000-558e09dc4000 r--p 00009000 08:01 1566                       /usr/bin/cat
558e09dc4000-558e09dc5000 rw-p 0000a000 08:01 1566                       /usr/bin/cat
```
These areas are loaded into memory in order to execute commands (spcifically - 'cat');
```
558e0b17e000-558e0b19f000 rw-p 00000000 00:00 0                          [heap]
```
Heap - dynamically allocated memory
```
7f8a5a294000-7f8a5a2c6000 r--p 00000000 08:01 5978                       /usr/lib/locale/C.UTF-8/LC_CTYPE
7f8a5a2c6000-7f8a5a2c7000 r--p 00000000 08:01 5983                       /usr/lib/locale/C.UTF-8/LC_NUMERIC
7f8a5a2c7000-7f8a5a2c8000 r--p 00000000 08:01 5986                       /usr/lib/locale/C.UTF-8/LC_TIME
7f8a5a2c8000-7f8a5a43b000 r--p 00000000 08:01 5977                       /usr/lib/locale/C.UTF-8/LC_COLLATE
7f8a5a43b000-7f8a5a43c000 r--p 00000000 08:01 5981                       /usr/lib/locale/C.UTF-8/LC_MONETARY
7f8a5a43c000-7f8a5a43d000 r--p 00000000 08:01 5975                       /usr/lib/locale/C.UTF-8/LC_MESSAGES/SYS_LC_MESSAGES
7f8a5a43d000-7f8a5a43e000 r--p 00000000 08:01 5984                       /usr/lib/locale/C.UTF-8/LC_PAPER
7f8a5a43e000-7f8a5a43f000 r--p 00000000 08:01 5982                       /usr/lib/locale/C.UTF-8/LC_NAME
7f8a5a43f000-7f8a5a440000 r--p 00000000 08:01 5976                       /usr/lib/locale/C.UTF-8/LC_ADDRESS
7f8a5a440000-7f8a5a441000 r--p 00000000 08:01 5985                       /usr/lib/locale/C.UTF-8/LC_TELEPHONE
```
Language configurations encoding/decoding utf-8
```
7f8a5a441000-7f8a5a727000 r--p 00000000 08:01 5972                       /usr/lib/locale/locale-archive
```
A locale archive is a memory - mapped file which contains all the system-provided locales.
Used for time optimization.
```
7f8a5a727000-7f8a5a74c000 r--p 00000000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f8a5a74c000-7f8a5a8c4000 r-xp 00025000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f8a5a8c4000-7f8a5a90e000 r--p 0019d000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f8a5a90e000-7f8a5a90f000 ---p 001e7000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f8a5a90f000-7f8a5a912000 r--p 001e7000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f8a5a912000-7f8a5a915000 rw-p 001ea000 08:01 3447                       /usr/lib/x86_64-linux-gnu/libc-2.31.so
```
Libc dependencies required by 'cat'.
```
7f8a5a91b000-7f8a5a91c000 r--p 00000000 08:01 5980                       /usr/lib/locale/C.UTF-8/LC_MEASUREMENT
```
Language configurations encoding/decoding utf-8
```
7f8a5a91c000-7f8a5a923000 r--s 00000000 08:01 3756                       /usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache
7f8a5a923000-7f8a5a924000 r--p 00000000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f8a5a924000-7f8a5a947000 r-xp 00001000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f8a5a947000-7f8a5a94f000 r--p 00024000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f8a5a94f000-7f8a5a950000 r--p 00000000 08:01 5979                       /usr/lib/locale/C.UTF-8/LC_IDENTIFICATION
7f8a5a950000-7f8a5a951000 r--p 0002c000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f8a5a951000-7f8a5a952000 rw-p 0002d000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
```
Dynamically loaded shared objects
```
7ffef3985000-7ffef39a6000 rw-p 00000000 00:00 0                          [stack]
```
Stack - local variables
```
7ffef39dc000-7ffef39df000 r--p 00000000 00:00 0                          [vvar]
```
vdso variables, readonly in userspace
```
7ffef39df000-7ffef39e0000 r-xp 00000000 00:00 0                          [vdso]
```
Virtual dynamic shared object is a shared library exported by the kernel to accelerate the execution of certain system calls that do not necessarily have to run in kernel space.
```
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```
This area contains syscalls which are functions loaded to and executed in user space
in order to save time of making a syscall.

### (b)
```
558e78f8a000-558e78f90000 r--p 00000000 08:01 3308                       /usr/lib/systemd/systemd-journald
...
7f9131d09000-7f9132509000 rw-s 00000000 08:01 267152                     /var/log/journal/daa5f54b52c44b9886fdaa70812de1c5/user-1000.journal
```
System log
```
7f9132510000-7f9132512000 r--p 00000000 08:01 3467                       /usr/lib/x86_64-linux-gnu/libcap-ng.so.0.0.0
...
7f913349f000-7f91334a0000 rw-p 0002d000 08:01 3443                       /usr/lib/x86_64-linux-gnu/ld-2.31.so
```
Shared objects

### (c)

We notice that the mapped pages of 'cat' change with each call. 
This is happening because the process's pages are allocated when the process starts and run
and the allocation is freed when it is destroyed. 'cat' is recreated upon each run and the pages are mapped to free memory addresses again.

### (d)

As explained above the vdso and vsyscalls both map objects and functions that normally reside in the kernel to special user-space area which improves the system overhead when
calling these functions. These aim for functions and objects that are called frequently.