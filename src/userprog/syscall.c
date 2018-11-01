#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/syscall-nr.h"
#include "console.h"

static void syscall_handler (struct intr_frame *);


/* A system call. */
struct system_call {
  int argc;                        /* # of args */
  void (*function)(void **argv);   /* Function to handle the system call. */
};

static void
write_helper(int fd, const void *buffer, unsigned size) {
  printf("in the helper.\n");
  printf("fd is %d\n", fd);
}

static void
halt_handler(void **argv) {
}

static void
exit_handler(void **argv) {
}

static void
exec_handler(void **argv) {
}
static void
wait_handler(void **argv) {
}
static void
create_handler(void **argv) {
}
static void
remove_handler(void **argv) {
}
static void
open_handler(void **argv) {
}
static void
file_size_handler(void **argv) {
}
static void
read_handler(void **argv) {
}

// argv should be void pointers
// this function will dereference the pointer, at this stage all pointer should
// be valid.
static void
write_handler(void **argv) {
  int fd = *((int *) argv[0]);
  const void *buffer = argv[1];
  unsigned size = *((unsigned *) argv[2]);
  write_helper(fd, buffer, size);
}

static void
seek_handler(void **argv) {
}

static void
tell_handler(void **argv) {
}

static void
close_handler(void **argv) {
}

/* The idea here is to create an array of `struct system_call`,
 * and the first element in the array corresponding to the SYS_HALT,
 * second to SYS_EXIT and so on. Basically one can expect to use
 * system_calls[SYS_HALT] to get the `struct system_call`
 * corresponding to the SYS_HALT. */
static const struct system_call system_calls[] =
    {
        {0, halt_handler},       // halt
        {1, exit_handler},       // exit
        {1, exec_handler},       // exec
        {1, wait_handler},       // wait
        {2, create_handler},     // create
        {1, remove_handler},     // remove
        {1, open_handler},       // open
        {1, file_size_handler},  // file_size
        {3, read_handler},       // read
        {3, write_handler},      // write
        {2, seek_handler},       // seek
        {1, tell_handler},       // tell
        {1, close_handler},      // close
        {0, NULL},
    };

static void **get_args(int num, void *esp) {
  void **args = malloc(sizeof(void *) * num);

  for (int i = 0; i < num; i++) {
    // TODO: pointer validation code goes into here.
    args[i] = (esp - (1 + i) * sizeof(void *));
    printf("pointers:%p\n", args[i]);
  }
  return args;
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  printf ("system call!\n");
  printf ("num: %li\n", *((long *) f->esp));
  printf ("num: %d\n", *((int *) f->esp) - 1);
  printf ("original esp pointers:%p\n", f->esp);

  struct system_call system_call = system_calls[*((long *) f->esp)];
  void **args = get_args(system_call.argc, f->esp);
  system_call.function(args);
  thread_exit ();
}

NO_RETURN void
exit (int status)
{
  thread_current()->exit_status = status;
  thread_exit();
}

int
write (int fd, const void *buffer, unsigned length) 
{
  if (fd == 1) {
    // writes to console
    putbuf((const char *)buffer, length);
    return length;
  }
}
