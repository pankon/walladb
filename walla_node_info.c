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
#include <math.h>   /* sqrt */

#include "logging.h"
#include "json.h"
#include "walla_pos.h"
#include "walla_node_info.h"

WALLA_STATUS WallaNodeInfoUpdateTime(WallaNodeInfo_t *walla_node_info, 
                                     unsigned long epoch)
{
    if (NULL == walla_node_info)
    {
        LogError("[WallaNodeInfoUpdateTime : no walla_node_info passed");
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (0 == walla_node_info->epoch_start && 0 == walla_node_info->epoch_end)
    {
        walla_node_info->epoch_start = epoch;
        walla_node_info->epoch_end = epoch;
    }
    else if (walla_node_info->epoch_start > epoch)
    {
        walla_node_info->epoch_start = epoch;   
    }
    else if (walla_node_info->epoch_end < epoch)
    {
        walla_node_info->epoch_end = epoch;
    }

    return (WALLA_SUCCESS);
}

double WallaNodeInfoGetNewAverage(double average, double value, 
                                  long current_number_of_entries)
{
    return (((average * (current_number_of_entries - 1)) + value) 
            / current_number_of_entries);  
}

double WallaNodeInfoGetNewStdev(double stddev, double old_mean, 
                                double new_mean, double value, 
                                long current_number_of_entries)
{
    /* Sources for modified Welford's method:
     * https://math.stackexchange.com/questions/775391/can-i-calculate-the-new-standard-deviation-when-adding-a-value-without-knowing-t
     * http://jonisalonen.com/2013/deriving-welfords-method-for-computing-variance/
     * this method ends up lagging one behind the actual value 
     */

    double lhs = (value - new_mean) * (value - old_mean);
    double numerator = ((current_number_of_entries - 1) * (stddev * stddev)) + lhs;
    double new_stddev = sqrt(numerator / (current_number_of_entries));

    LogVerbose("{\"lhs\" = %f, \"numerator\": %f, \"new_stddev\" : %f}", 
        lhs, numerator, new_stddev);

    return new_stddev;
}

WALLA_STATUS WallaNodeInfoUpdateWithValue(WallaNodeInfo_t *walla_node_info, 
                                          double value, long *current_number_of_entries)
{
    double old_mean = 0;

    if (NULL == current_number_of_entries || NULL == walla_node_info)
    {
        LogError("[WallaNodeInfoUpdateWithValue : no current_number_of_entries"
                 "or walla_node_info passed");
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (0 == walla_node_info->max && 0 == walla_node_info->min)
    {
        walla_node_info->max = value;
        walla_node_info->min = value;
    }
    else if (walla_node_info->max < value)
    {
        walla_node_info->max = value;
    }
    else if (walla_node_info->min > value)
    {
        walla_node_info->min = value;
    }

    ++(*current_number_of_entries);
    old_mean = walla_node_info->average;

    walla_node_info->average = WallaNodeInfoGetNewAverage(
        walla_node_info->average, 
        value, *current_number_of_entries);

    walla_node_info->stdev = WallaNodeInfoGetNewStdev(
        walla_node_info->stdev, 
        walla_node_info->average, old_mean, 
        value, *current_number_of_entries);

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

    status = WallaNodeInfoUpdateWithValue(walla_node_info, walla_entry->value, 
                                          current_number_of_entries);

    return (status);
}

WALLA_STATUS WallaNodeInfoInit(WallaNodeInfo_t *walla_node_info, WallaPos_t *pos)
{
    if (NULL == walla_node_info || NULL == pos)
    {
        LogError("[WallaNodeInfoInit] : No walla_node_info or walla_pos passed");
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
        LogInfo("[WallaNodeInfoToJson] : walla_node_info is null");
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
        LogInfo("[WallaNodeInfoToJson] : asprintf failed");
        return (JsonGetNull());
    }
    
    return (json_string);
}