#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/syscall-nr.h"

static void syscall_handler (struct intr_frame *);


/* A system call. */
struct system_call {
  int argc;                        /* # of args, including action name. */
  void (*function)(void **argv);   /* Function to execute action. */
};

/* The idea here is to create an array of this, and the first element
 * in the array corresponding to the SYS_HALT, second to SYS_EXIT and so on.
 * Basically one can expect to use */
static void
write_helper(int fd, const void *buffer, unsigned size) {
  printf("in the helper.\n");
  printf("fd is %d\n", fd);
}

static void
halt(void **argv) {
}

static void
exit(void **argv) {
}

static void
exec(void **argv) {
}
static void
wait(void **argv) {
}
static void
create(void **argv) {
}
static void
remove(void **argv) {
}
static void
open(void **argv) {
}
static void
file_size(void **argv) {
}
static void
read(void **argv) {
}

// argv should be void pointers
// this function will dereference the pointer, at this stage all pointer should
// be valid.
static void
write(void **argv) {
  int fd = *((int *) argv[0]);
  const void *buffer = argv[1];
  unsigned size = *((unsigned *) argv[2]);
  write_helper(fd, buffer, size);
}

static void
seek(void **argv) {
}

static void
tell(void **argv) {
}

static void
close(void **argv) {
}

static const struct system_call system_calls[] =
    {
        {1, halt},       // halt
        {1, exit},       // exit
        {1, exec},       // exec
        {1, wait},       // wait
        {2, create},     // create
        {1, remove},     // remove
        {1, open},       // open
        {1, file_size},  // file_size
        {3, read},       // read
        {3, write},      // write
        {2, seek},       // seek
        {1, tell},       // tell
        {1, close},      // close
        {0, NULL},
    };

static void **getArgs(int num, void *esp) {
  void **args = malloc(sizeof(void *) * num);

  for (int i = 0; i < num; i++) {
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
  void **args = getArgs(system_call.argc, f->esp);
  system_call.function(args);
  thread_exit ();
}
