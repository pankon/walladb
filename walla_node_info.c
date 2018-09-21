/**************************************
 * walla_node_info.c
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Node Info
 * Structure that holds all info on the walla node
 * This struct has no memory of its own, and thus has no constructors/destructors
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#define _GNU_SOURCE
#include <stdio.h>  /* sprintf, asprintf    */

#include "json.h"
#include "walla_pos.h"
#include "walla_node_info.h"

WALLA_STATUS WallaNodeInfoUpdateTime(WallaNodeInfo_t *walla_node_info, unsigned long epoch)
{
    if (NULL == walla_node_info)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (walla_node_info->epoch_start > epoch)
    {
        walla_node_info->epoch_start = epoch;   
    }
    else if (walla_node_info->epoch_end < epoch)
    {
        walla_node_info->epoch_end = epoch;
    }

    return (WALLA_SUCCESS);
}

double WallaNodeInfoGetNewAverage(double average, double value, long current_number_of_entries)
{
    /* https://math.stackexchange.com/questions/22348/how-to-add-and-subtract-values-from-an-average */
    /*(average + ((value - average) / (double)current_number_of_entries));*/

    return (((average * current_number_of_entries) + value) / (current_number_of_entries + 1));  
}

double WallaNodeInfoGetNewStdev(double stddev, double value, long current_number_of_entries)
{
    /* https://math.stackexchange.com/questions/775391/can-i-calculate-the-new-standard-deviation-when-adding-a-value-without-knowing-t */

    /* TODO */

    return -1;
}

WALLA_STATUS WallaNodeInfoUpdateWithValue(WallaNodeInfo_t *walla_node_info, double value, long *current_number_of_entries)
{
    if (NULL == current_number_of_entries)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (walla_node_info->max < value)
    {
        walla_node_info->max = value;
    }
    else if (walla_node_info->min > value)
    {
        walla_node_info->min = value;
    }

    walla_node_info->average = WallaNodeInfoGetNewAverage(walla_node_info->average, value, *current_number_of_entries);
    walla_node_info->stdev = WallaNodeInfoGetNewStdev(walla_node_info->stdev, value, *current_number_of_entries);

    ++(*current_number_of_entries);

    return (WALLA_SUCCESS);
}

WALLA_STATUS WallaNodeInfoUpdateWithEntry(
    WallaNodeInfo_t *walla_node_info, 
    WallaEntry_t *walla_entry,
    long *current_number_of_entries
)
{
    WALLA_STATUS status = WALLA_SUCCESS;

    if (NULL == walla_node_info || NULL == walla_entry)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    status = WallaNodeInfoUpdateTime(walla_node_info, walla_entry->epoch);
    if (status != WALLA_SUCCESS)
    {
        return (status);
    }

    status = WallaNodeInfoUpdateWithValue(walla_node_info, walla_entry->value, current_number_of_entries);

    return (status);
}

WALLA_STATUS WallaNodeInfoInit(WallaNodeInfo_t *walla_node_info, WallaPos_t *pos)
{
    if (NULL == walla_node_info || NULL == pos)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    walla_node_info->pos = *pos;
    walla_node_info->epoch_start = 0;
    walla_node_info->epoch_end = 0;
    walla_node_info->average = 0;
    walla_node_info->max = 0;
    walla_node_info->min = 0;
    walla_node_info->stdev = 0;

    return (WALLA_SUCCESS);
}

void WallaNodeInfoZero(WallaNodeInfo_t *walla_node_info)
{
    if (NULL == walla_node_info)
    {
        return;
    }

    WallaPosZero(&(walla_node_info->pos));
    walla_node_info->epoch_start = 0;
    walla_node_info->epoch_end = 0;
    walla_node_info->average = 0;
    walla_node_info->max = 0;
    walla_node_info->min = 0;
    walla_node_info->stdev = 0;
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
                " \"std\":"FLOAT_OUTPUT
                "}"
        "}";
    char *json_string = NULL;
    int len = -1;

    if (walla_node_info == NULL)
    {
        return (JsonGetNull());
    }

    len = asprintf(
        &json_string, 
        walla_node_info_to_json, 
        walla_node_info->pos.x,
        walla_node_info->pos.y,
        walla_node_info->pos.z,
        walla_node_info->epoch_start,
        walla_node_info->epoch_end,
        walla_node_info->average,
        walla_node_info->max,
        walla_node_info->min,
        walla_node_info->stdev
        );
    if (-1 == len)
    {
        return (JsonGetNull());
    }
    
    return (json_string);
}