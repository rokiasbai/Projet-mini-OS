#include <stdio.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <stddef.h>

#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__
#define STACK_SIZE 1042
#define BASE_TSS	0x08

#define NB_PROC 100

typedef uint32_t pid_t;
typedef enum PROCESS_STATE {ELU, PRET, BLOQUE} PROCESS_STATE;
typedef enum ctx {EBX, ESP, EBP, ESI, EDI} ctx;


//typedef enum {ELU, PRET, BLOQUE} PROCESS_STATE;

typedef struct process_t{
    uint32_t stack[STACK_SIZE];
    PROCESS_STATE state;
    pid_t pid;
    uint32_t regs[5];
    char * nom;
    } process_t;

typedef struct fifo{
    pid_t pid;
    struct fifo* suivant;
    } fifo_t;

fifo_t *process_liste;

typedef void* (*fnptr)();
int exit();
pid_t getpid();
//on ne fait pas sleep pour l'instant
int sleep(int seconds);

process_t process_table[NB_PROC];

void schedule();
pid_t allouer_pid();
pid_t fork(char *nom, fnptr process);
void addProcess (pid_t pid );
pid_t removeProcess();
void arreter();
void activer(pid_t pid);
//void suspendre(pid_t pid);
void *stop_process_uc();
pid_t getpid();
void code_process ();

#endif