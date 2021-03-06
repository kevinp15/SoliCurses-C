#include <curses.h>
#include <time.h>
#include "../include/consts.h"
#include "../include/gb.h"

static const char val_ch[4] = "JQK";
static const int card_width = 4;
static const int card_height = 3;

static bool canmove(GB * gb, pile p1, pile p2, int ind1, int ind2);

static card * lastRevealed(GB * gb, pile p);

static int totalRevealed(GB * gb, pile p);

static void printBackground(void);

static void printCard(int y, int x, card * c);

static void printDeck(int y, int x, GB * gb);

static void printEmptyCard(int y, int x);

static void printHiddenCard(int y, int x);

void doinput(GB * gb, Cursor * c, int input)
{
    switch(input)
    {
        default:
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "Time elapsed: %i", time(NULL) - gb->starttime);
            break;

        // Exit prompt
        case 'e':
        case 'E':
            break;

        // Draw card
        case 'd':
        case 'D':
            break;

        // Select card for movement then select location for movement
        case ' ':
            break;

        case KEY_UP:
            break;

        case KEY_DOWN:
            break;

        case KEY_RIGHT:
            break;

        case KEY_LEFT:
            break;
    }
}

void initGB(GB * gb, card * deck)
{
    int count = 0;

    for(int i = DS; i <= T7; i++)
    {
        Array_init(&gb->gb[i], 1);
    }

    for(int i = T1; i <= T7; i++)
    {
        for(int j = 0; j <= i - T1; j++)
        {
            Array_append(&gb->gb[i], &deck[count++]);
        }
        Card_reveal(Array_last(&gb->gb[i]));
    }

    while(count < 52)
    {
        Array_append(&gb->gb[DS], &deck[count++]);
    }

    gb->starttime = time(NULL);
    gb->gamestart = true;
    gb->gamestop = false;
    gb->gamewin = false;
}

void printGB(GB * gb, Cursor * c)
{
    printBackground();
    printDeck(2, 4, gb);

    // Print Foundation
    for(int i = F1; i <= F4; i++)
    {
        printCard(2, 24 + 10 * i, lastRevealed(gb, i));
    }

    // Print Tableau
    for(int i = T1; i <= T7; i++)
    {
        for(int j = 0; j < gb->gb[i].used; j++)
        {
            printCard(8 + 2 * j, 10 * i - 46, gb->gb[i].array[j]);
        }
    }
}

void refreshGB(GB * gb)
{
    // Flip last cards in tableau face up
    for(int y = T1; y <= T7; y++)
    {
        Card_reveal(Array_last(&gb->gb[y]));
    }

    // Leaves all cards in foundation except the last flipped face up
    for(int y = F1; y <= F4; y++)
    {
        if(gb->gb[y].used > 0)
        {    
            for(int x = 0; x < gb->gb[y].used; x++)
            {
                Card_hide(gb->gb[y].array[x]);
            }
            Card_reveal(Array_last(&gb->gb[y]));
        }
    }
}

static card * lastRevealed(GB * gb, pile p)
{
    for(int i = gb->gb[p].used - 1; i >= 0; i--)
    {
        if(gb->gb[p].array[i]->r)
            return gb->gb[p].array[i];
    }
    return NULL;
}

static int totalRevealed(GB * gb, pile p)
{
    int total = 0;
    for(int i = 0; i < gb->gb[p].used; i++)
    {
        if(gb->gb[p].array[i]->r)
            total++;
    }
    return total;
}

static void printBackground(void)
{
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    for(int y = 0; y < 100; y++)
    {
        for(int x = 0; x < 1000; x++)
        {
            mvprintw(y, x, " ");
        }
    }
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
}

static void printCard(int y, int x, card * c)
{
    if(c == NULL)
    {
        printEmptyCard(y, x);
        return;
    }

#ifndef RANDOMSTRINGOFLETTERS
    if(!c->r) // If card is not revealed
    {
        printHiddenCard(y, x);
        return;
    }
#endif
    int color;
    if(Card_color(c) == black)
        color = 3;
    else /* if color is red */
        color = 2;

    // Apply color attributes
    attron(COLOR_PAIR(color));

    // Print white card background
    for(int yc = y; yc < y + card_height; yc++)
    {
        for(int xc = x; xc < x + card_width; xc++)
        {
            mvprintw(yc, xc, " ");
        }
    }

    attron(COLOR_PAIR(color + 5));

    // Print card suit symbol
#ifdef _MSVC_TRADITIONAL
    mvaddwstr(y, x, suit_ch[c->s]);
    mvaddwstr(y + card_height - 1, x + card_width - 1, suit_ch[c->s]);
#else
    mvprintw(y, x, "%s", suit_ch[c->s]);
    mvprintw(y + card_height - 1, x + card_width - 1, "%s", suit_ch[c->s]);
#endif
    // Print card value
    if(c->v == Ace)
    {
        mvprintw(y, x + card_width - 1, "A");
        mvprintw(y + card_height - 1, x, "A");
    }
    else if(c->v >= Two && c->v < Ten)
    {
        mvprintw(y, x + card_width - 1, "%i", c->v + 1);
        mvprintw(y + card_height - 1, x, "%i", c->v + 1);
    }
    else if(c->v == Ten)
    {
        mvprintw(y, x + card_width - 2, "10");
        mvprintw(y + card_height - 1, x, "10");
    }
    else if(c->v >= Jack)
    {
        mvprintw(y, x + card_width - 1, "%c", val_ch[c->v - Jack]);
        mvprintw(y + card_height - 1, x, "%c", val_ch[c->v - Jack]);
    }

    // Remove color attributes
    attron(COLOR_PAIR(1));
}

static void printDeck(int y, int x, GB * gb)
{
    if(totalRevealed(gb, DS) == 0)
        printHiddenCard(y, x);
    else
        printEmptyCard(y, x);
}

static void printEmptyCard(int y, int x)
{
    attron(COLOR_PAIR(7));
    for(int yc = y; yc < y + card_height; yc++)
    {
        for(int xc = x; xc < x + card_width; xc++)
        {
            mvprintw(yc, xc, " ");
        }
    }
    attron(COLOR_PAIR(1));
    for(int yc = y + 1; yc < y + card_height - 1; yc++)
    {
        for(int xc = x + 1; xc < x + card_width - 1; xc++)
        {
            mvprintw(yc, xc, " ");
        }
    }
}

static void printHiddenCard(int y, int x)
{
    attron(COLOR_PAIR(3));
    for(int yc = y; yc < y + card_height; yc++)
    {
        for(int xc = x; xc < x + card_width; xc++)
        {
            mvprintw(yc, xc, " ");
        }
    }
    attron(COLOR_PAIR(4));
    for(int yc = y + 1; yc < y + card_height - 1; yc++)
    {
        for(int xc = x + 1; xc < x + card_width - 1; xc++)
        {
            mvprintw(yc, xc, " ");
        }
    }
    attron(COLOR_PAIR(1));
}
