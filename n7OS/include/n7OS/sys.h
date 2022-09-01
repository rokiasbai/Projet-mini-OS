
#ifndef __SYS_H__
#define __SYS_H__
#include <n7OS/syscall_defs.h>
#include <unistd.h>
void init_syscall ();
int write(const char *s, int len);
#endif
