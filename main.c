/**************************************
 * main.c
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * Usage:
 *
 * Service features:
 * OctTree with circbufs
 * One thread per requester
 * 2+ threads per all sensors but one shmem segment
 * Specs:
 * Magic: WALLA-DB
 * Fri Sep 14 14:51:47 2018
***************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* free */
#include "walla_entry.h"

int main(int argc, char **argv)
{
    char *buffer = NULL;
    WallaEntry_t *walla_entry = NULL;
    
    walla_entry = WallaEntryCreate(1000, 0.2342);
    
    buffer = WallaEntryToJson(walla_entry);
    printf("%s\n", buffer);
    free(buffer);
    
    WallaEntryDestroy(walla_entry);
    walla_entry = NULL;

    return (0);
}
