#include <inttypes.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
// initialisation du compteur, seconde, minute, heure à 0
int c = 0;
int s = 0;
int m = 0;
int h = 0;

extern void handler_TIME();

// fonction qui gère une horloge normale
void horloge()
{
    c++;
    if ((c % 1000) == 0)
    {
        s++;
    }

    else if ((s % 60) == 0)
    {
        if (s > 0)
            m++;
        s = 0;
    }

    else if ((m % 60) == 0)
    {
        if (m > 0)
            h++;
        m = 0;
    }

    else if ((h % 24) == 0)
    {
        if (h > 24)
            h = 0;
    }
    // je récupère la ligne où se trouve mon curseur
    int ligne = getLigne();
    // je récupère la colonne où se trouve mon curseur
    int colonne = getColonne();
    // pour placer tout à droite de la fenetre
    update_cursor(71, 0);
    printf("%dh:%dm:%ds\r", h, m, s);
    update_cursor(colonne, ligne);
}

void init_time()
{
    // configuration
    outb(0x34, 0x43);
    outb(FREQUENCE & 0xFF, 0x40);
    outb(FREQUENCE >> 8, 0x40);
    init_irq_entry(0x20, (uint32_t)handler_TIME);
    // démasquage
    outb(inb(0x21) & (0xfe), 0x21);
}

void handler_time()
{
    // masquage
    outb(inb(0x21) | 1, 0x21);
    // appel à la fonction horloge plus haut
    horloge();
    // acquittement
    outb(0x20, 0x20);
    // démasquage
    outb(inb(0x21) & (0xfe), 0x21);
}
