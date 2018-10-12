/**************************************
 * memman.h
 * 
 * Memory manager
 * Pankonworks
 * 
 * Memory manager
 *
 * Sun Sep 29 11:46:00 2018
 ***************************************/

#ifndef __MEMMAN_H__
#define __MEMMAN_H__

typedef struct MemMan Memman_t;

MemMan_t *MemManCreate(size_t size);

MemMan_t *MemManInit(void *memory, size_t size);

void *MemManAlloc(MemMan_t *man, size_t size);
void MemManFree(MemMan_t *man, void *data);

void MemManDestroy(MemMan_t *man);

#endif /* __MEMMAN_H__ */