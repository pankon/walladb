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

struct MemMan
{

};

MemMan_t *MemManCreate(size_t size)
{
	return (NULL);
}

MemMan_t *MemManInit(void *memory, size_t size)
{
	return (NULL);
}

void *MemManAlloc(MemMan_t *man, size_t size)
{
	return (NULL);
}

void MemManFree(MemMan_t *man, void *data)
{
}

void MemManDestroy(MemMan_t *man)
{
}
