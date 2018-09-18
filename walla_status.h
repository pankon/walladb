/**************************************
 * walla_status.h
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * 
 * Just holds the enum
 * 
 * Fri Sep 14 14:51:47 2018
***************************************/

#ifndef __WALLA_STATUS_H__
#define __WALLA_STATUS_H__

typedef enum
{
    WALLA_SUCCESS,
    WALLA_OOPS,
    WALLA_BYE,
    WALLA_BAD_FORMAT,
    WALLA_NOT_IMPLEMENTED,
    WALLA_YOU_FORGOT_TO_PASS_ANYTHING,
} WALLA_STATUS;

#endif /* __WALLA_STATUS_H__ */