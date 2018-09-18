/**************************************
 * walla_node.c
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Node
 * Main data structure
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#define _GNU_SOURCE
#include <stdio.h>  /* sprintf, asprintf    */
#include <stdlib.h> /* malloc, free         */

#include "json.h"

/* 
 * Turns WallaNode into a json string
 * requires walking over circular buffer and constructing
 * json from the WallaEntry_t's.
 */
char *WallaNodeToJson(WallaNode_t walla_node)
{
    /* TODO: Implement */
    return null_json;
}

