/**************************************
 * walla_entry.c
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Entry: Stores date and value 
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#define _GNU_SOURCE
#include <stdio.h>  /* sprintf, asprintf    */
#include <stdlib.h> /* malloc, free         */

#include "json.h"
#include "walla_entry.h"

WallaEntry_t *WallaEntryCreate(long epoch, double value)
{
    WallaEntry_t *walla_entry = NULL;

    if (NULL == (walla_entry = malloc(sizeof(WallaEntry_t))))
    {
        return (NULL);
    }

    walla_entry->epoch = epoch;
    walla_entry->value = value;

    return walla_entry;
}

void WallaEntryDestroy(WallaEntry_t *walla_entry)
{
    free(walla_entry);
    walla_entry = NULL;
}

/* 
 * Turns WallaEntry_t into a json string
 * returns string: { epoch : epoch_val, value: double_value }
 */
char *WallaEntryToJson(WallaEntry_t *walla_entry)
{
    static char *walla_entry_to_json = "{\"epoch\": %lu,"
           " \"value\": " FLOAT_OUTPUT "}";
    char *json_string = NULL;
    int len = -1;

    if (walla_entry == NULL)
    {
        return null_json;
    }

    len = asprintf(
        &json_string, walla_entry_to_json, 
        walla_entry->epoch, walla_entry->value
    );
    if (-1 == len)
    {
        return (NULL);
    }
    
    return json_string;
}