/**************************************
 * walla_entry.h
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Entry: Stores date and value 
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#ifndef __WALLA_ENTRY_H__
#define __WALLA_ENTRY_H__

typedef struct WallaEntry {
    unsigned long epoch;
    double value;
} WallaEntry_t;

/* 
 * Create a walla entry
 */
WallaEntry_t *WallaEntryCreate(long epoch, double value);

/* 
 * Instantiate a walla entry on preallocated memory
 */
WallaEntry_t *WallaEntryInit(void *memory, long epoch, double value);

/*
 * Destroy a walla entry
 */
void WallaEntryDestroy(WallaEntry_t *walla_entry);

/* 
 * Zero a walla entry on preallocated memory
 */
void WallaEntryZero(WallaEntry *walla_entry);

/* 
 * Turns WallaEntry_t into a json string
 */
char *WallaEntryToJson(WallaEntry_t *walla_entry);

#endif /* __WALLA_ENTRY_H__ */