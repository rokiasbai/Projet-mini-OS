#include <inttypes.h>
#include <n7OS/segment.h>
#include <n7OS/processor_structs.h>
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */

// redéclarer les fonctions utilisées
void console_putbytes(const char *s, int len);
void update_cursor(int colonneloc, int ligneloc);
uint16_t get_cursor_position(void);
int getLigne();
int getColonne();

#endif
