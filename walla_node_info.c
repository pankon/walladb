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

#include "walla_node_info.h"

void WallaNodeInfoUpdateTime(WallaNodeInfo_t *walla_node_info, unsigned long epoch)
{
    if (NULL == walla_node_info)
    {
        return;
    }

    if (walla_node_info->epoch_start > epoch)
    {
        walla_node_info->epoch_start = epoch;   
    }
    else if (walla_node_info->epoch_end < epoch)
    {
        walla_node_info->epoch_end = epoch;
    }
}

double WallaNodeInfoGetNewAverage(double average, double value, long current_number_of_entries)
{
    /* https://math.stackexchange.com/questions/22348/how-to-add-and-subtract-values-from-an-average */

    return (average + ((value - average) / (double)current_number_of_entries));
}

double WallaNodeInfoGetNewStdev(double stddev, double value, long current_number_of_entries)
{
    /* https://math.stackexchange.com/questions/775391/can-i-calculate-the-new-standard-deviation-when-adding-a-value-without-knowing-t */

    /* TODO */

    return -1;
}

void WallaNodeInfoUpdateWithValue(WallaNodeInfo_t *walla_node_info, double value, long current_number_of_entries)
{
   
    if (walla_node_info->max < value)
    {
        walla_node_info->max = value;
    }
    else if (walla_node_info->min > value)
    {
        walla_node_info->min = value;
    }

    walla_node_info->average = WallaNodeInfoGetNewAverage(walla_node_info->average, value);
    walla_node_info->stdev = WallaNodeInfoGetNewStdev(walla_node_info->stdev, value);
}

WALLA_STATUS WallaNodeInfoUpdateWithEntry(
    WallaNodeInfo_t *walla_node_info, 
    WallaEntry_t *walla_entry,
    long current_number_of_entries
)
{
    if (NULL == walla_node_info || NULL == walla_entry)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    WallaNodeInfoUpdateTime(walla_node_info, walla_entry->epoch);
    WallaNodeInfoUpdateWithValue(walla_node_info, walla_entry->value);

    return (WALLA_SUCCESS);
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
    
    return (json_string);
}