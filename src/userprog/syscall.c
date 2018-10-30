#include "userprog/syscall.h"
#include <hash.h>
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "console.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}

int
write (int fd, const void *buffer, unsigned length){
  if(fd == 1) {
    // writes to console
    putbuf((const char *)buffer,length);
    return length;
  }
}
