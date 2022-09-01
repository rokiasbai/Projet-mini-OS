#include <n7OS/console.h>
#include <stdio.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
// au début, on initialise ligne à 0 et colonne à 0
int ligne = 0;
int colonne = 0;
uint16_t *scr_tab = (uint16_t *)0xB8000; //@ mémoire : 753664
#define BS '\b'
#define BS_CODE 0x08 // 8
#define HT '\t'
#define HT_CODE 0x09 // 9
#define LF '\n'
#define LF_CODE 0xA // 10
#define FF '\f'
#define FF_CODE 0xC // 12
#define CR '\r'
#define CR_CODE 0xD // 13

// fonction pour avoir la position actuelle du curseur
uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x0F, 0x3D4);
    pos |= inb(0x3D5);
    outb(0x0E, 0x3D4);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

// fonction pour mettre à jour le curseur
void update_cursor(int colonneloc, int ligneloc)
{
    uint16_t posloc = ligneloc * 80 + colonneloc;
    // position = 16 bits, 8 poids forts  et 8 poids faibles
    outb(0x0F, 0x3D4);
    // récupere les 8 bits de poids faibles
    outb((uint8_t)(posloc & 0xFF), 0x3D5);
    outb(0x0E, 0x3D4);
    // récupère les 8 bits de poids forts
    outb((uint8_t)((posloc >> 8) & 0xFF), 0x3D5);
}

// fonction pour vérifier
void check()
{
    if (colonne >= 80)
    {
        ligne = ligne + 1;
        colonne = 0;
    }
    if (ligne >= 25)
    {
        for (int i = 0; i < ((80 * 25) - 1) - 80; i++)
        {
            scr_tab[i] = scr_tab[i + 80];
        }
        for (int i = ((80 * 25) - 1) - 80; i < ((80 * 25) - 1); i++)
        {
            scr_tab[i] = ' ';
        }
        ligne = 24;
    }
    update_cursor(colonne, ligne);
}

// fonction pour mettre à jour la variable ligne
void majLigne(int l)
{
    ligne = l;
    check();
}

// fonction pour mettre à jour la variable colonne
void majColonne(int c)
{
    colonne = c;
    check();
}

// fonction qui retourne la variable ligne
int getLigne()
{
    return ligne;
}

// fonction qui retourne la variable colonne
int getColonne()
{
    return colonne;
}

// fonction console_putchar qui affiche le caratère
void console_putchar(const char c)
{
    if (c > 31 && c < 127)
    {
        // récupérer la pos du curseur
        uint16_t pos = get_cursor_position();
        // ascii value du caractère c
        scr_tab[pos] = (0xF << 8) | c;
        // passer à la colonne suivante (caractère suivant)
        colonne = colonne + 1;
        // mettre à jour le curseur avec la bonne colonne
        update_cursor(colonne, ligne);
    }
    else if (c == BS)
    {
        update_cursor(colonne - 2, ligne);
    }
    else if (c == HT)
    {
        update_cursor(colonne + 7, ligne);
    }
    else if (c == LF)
    { // ou c==LF_CODE
        majLigne(ligne + 1);
        majColonne(0);
    }
    else if (c == FF)
    {
        for (int i = get_cursor_position(); i >= 0; i--)
        {
            scr_tab[i] = ' ';
        }
        update_cursor(0, 0);
    }
    else if (c == CR)
    {
        majLigne(ligne);
        update_cursor(0, (ligne));
    }
    else
    {
        // on fait rien
    }
}

// fonction console_putbytes qui manipule le curseur
void console_putbytes(const char *s, int len)
{
    uint16_t posloc = get_cursor_position();
    // refixe ligne et colonne pour le update du 1er if de console_putchar
    ligne = (posloc - colonne) / 80;
    colonne = posloc - (80 * ligne);
    for (int c = 0; c < len; c++)
    {
        // affiche le caractère un à un
        console_putchar(s[c]);
    }
}
