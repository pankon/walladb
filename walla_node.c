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

/* 
 * Turns WallaNodeInfo into a json string
 * {"(x, y, z, t_start, t_end)": {
 *     "avg" : 0.324232
 *     "max" : 324.1232
 *     "min" : 0.010230
 *     "std" : 13.32459
 *  } }
 */
char *WallaNodeInfoToJson(WallaNodeInfo_t *walla_node_info)
{
    static char *walla_node_info_to_json = 
        "{\"(%lu, %lu, %lu, %lu, %lu)\":"
                "{\"avg\":"FLOAT_OUTPUT","
                " \"max\":"FLOAT_OUTPUT","
                " \"min\":"FLOAT_OUTPUT","
                " \"std\":"FLOAT_OUTPUT","
                "}"
        "}";
    char *json_string = NULL;
    int len = -1;

    if (walla_node_info == NULL)
    {
        return null_json;
    }
    
    len = asprintf(
        &json_string, walla_node_info_to_json, 
        walla_node_info->pos.x,
        walla_node_info->pos.x,
        walla_node_info->pos.x,
        walla_node_info->epoch_start,
        walla_node_info->epoch_end,
        walla_node_info->average,
        walla_node_info->max,
        walla_node_info->min,
        walla_node_info->stdev);
    if (-1 == len)
    {
        return (NULL);
    }
    
    return json_string;
}