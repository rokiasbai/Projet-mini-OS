#include <n7OS/processus.h>
#include <stdio.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <malloc.h>
#include <stdbool.h>

pid_t pid_courant = 0;
process_t *process_courant;

pid_t allouer_pid()
{
    for (int i = 0; i < NB_PROC; i++)
    {
        if (process_table[i].pid == -1)
        {
            return i;
        }
    }
    return -1;
}

void init_tableau_processus()
{
    for (int i = 0; i < NB_PROC; i++)
    {
        // process_table[i].state=PRET;
        process_table[i].pid = -1;
    }
}



pid_t getpid()
{
    return pid_courant;
}
int exit()
{
    pid_t pid = getpid();
    process_table[pid].pid = -1;
    removeProcess(pid);
    schedule();
    return 0;
}

extern void ctx_sw(void *ctx_old, void *ctx_new);

void schedule()
{
    pid_t pid = pid_courant + 1;
    bool suivant = false;
    while (!suivant)
    {
        if (process_table[pid].state == PRET)
        {
            suivant = true;
            process_table[pid].state = ELU;
        }
        pid = (pid + 1) % NB_PROC;
    }
    ctx_sw(process_table[pid_courant].regs, process_table[pid].regs);
    pid_courant = pid;
}

pid_t fork(char *nom, fnptr process)
{
    pid_t pid;
    if ((pid = allouer_pid()) == -1)
    {
        printf("creation impossible");
        exit();
    }
    process_table[pid].pid = pid;
    process_table[pid].stack[STACK_SIZE - 1] = (uint32_t)process;
    process_table[pid].nom = nom;
    process_table[pid].state = PRET;
    process_table[pid].regs[EBX] = 0;
    process_table[pid].regs[EBP] = 0;
    process_table[pid].regs[ESI] = 0;
    process_table[pid].regs[EDI] = 0;
    process_table[pid].regs[ESP] = (process_table[pid].stack[STACK_SIZE - 1]);
    return process_table[pid].pid;
}

void addProcess(pid_t pid)
{
    fifo_t *cellule = malloc(sizeof(fifo_t));
    cellule->pid = pid;
    if (process_liste == NULL)
    {
        process_liste = cellule;
    }
    fifo_t *tmp = process_liste;
    while (tmp->suivant == NULL)
    {
        tmp = tmp->suivant;
    }
    tmp->suivant = cellule;
}
pid_t removeProcess()
{
    if (process_liste != NULL)
    {
        fifo_t *cellule = process_liste->suivant;
        pid_t pid = cellule->pid;
        free(process_liste);
        process_liste = NULL;
        process_liste = cellule;
        return pid;
    }
    return -1;
}
void arreter()
{
    pid_t pid = getpid();
    process_table[pid].state = PRET;
    addProcess(pid);
    schedule();
}
void activer(pid_t pid)
{
    if (process_table[pid].state == PRET)
    {
        process_table[pid].state = ELU;
    }
    else if (process_table[pid].state == ELU)
    {
        printf("processus déjà en cours");
    }
    else
    {
        printf("processus bloqué donc on s'en fou");
    }
    addProcess(pid);
    schedule();
}

void code_process()
{
    schedule();
    while (1);
}


