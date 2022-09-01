#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <stdio.h>
#include <n7OS/irq.h>
#include <inttypes.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>
#include <unistd.h>
#include <n7OS/sys.h>

void kernel_start(void)
{
    // extern void handler_TIME();
    init_syscall();
    // initialiser le timer
    init_time();

    // effacer tout ce que contient l'écran au début
    printf("\f");
    // activer le handler
    sti();
    // instancier handler
    // handler_IT appelle Handler_c dans test_irq.c
    extern void handler_IT();
    // initialiser la ligne 50
    init_irq_entry(50, (uint32_t)handler_IT);
    // exécuter int dans du code C
    __asm__("int $50"
            :
            :);

    // tester \n
    printf("C'est moi la console :) \n");

    // test de l'appel système example
    //activer l'interruption
    sti();
    if (example() == 1)
    {
        printf("Appel systeme example ok\n");
    }

    // test de l'appel système shutdown
    if (shutdown(4) == 4)
    {
        printf("Appel systeme shutdown ok\n");
    }

    // test de l'appel système write
    write("c'est moi write :)", 18);

/*
    //test processus
    extern void processus1();
    int pid = creer("idle", (fnptr) idle);
    idle();
    pid = fork("processus1.c", (fnptr) processus1);
    processus1();
    pid = fork("processus1.c", (fnptr) processus1);

*/

    //arreter
    while (1)
    {
        hlt();
    }
}
