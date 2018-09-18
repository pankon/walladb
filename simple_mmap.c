/**************************************
 * simple_mmap.c
 * 
 * SimpleMmap
 * Pankonworks
 * Nathan Pankowsky
 * Usage:
 *
 * nu, just use it
 * Fri Sep 14 15:32:50 2018
***************************************/
#include <stdio.h>  /* fprintf, stderr, fopen, FILE  */
#include <stdlib.h> /* malloc, free     */
#include <fcntl.h>  /* open             */
#include <unistd.h> /* close            */
#include <sys/mman.h>   /* mmap, munmap */
#include <sys/stat.h>   /* fstat, stat  */

#include "simple_mmap.h"


/* 
 * Get a char array, mmapped
 */ 
mmap_t *SimpleMmapGetMeACharArray(char *filename)
{
    struct stat sb = {0};
    mmap_t *mmapped_data = NULL;
    
    if (NULL == (mmapped_data = malloc(sizeof(mmap_t))))
    {
        fputs("Malloc error", stderr);
        return (NULL);
    }
    
    mmapped_data->handle = open(filename, O_RDONLY);
    if (-1 == mmapped_data->handle)
    {
        fputs("Error opening file", stderr);
        return (NULL);
    }
    
    if (-1 == fstat(mmapped_data->handle, &sb))
    {
        fputs("fstat error", stderr);
        return (NULL);
    }
    
    mmapped_data->data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, mmapped_data->handle, 0);
    if ((void *)-1 == mmapped_data->data)
    {
        fputs("Error mapping file", stderr);
        return (NULL);
    }
    
    mmapped_data->size = sb.st_size;
    
    return (mmapped_data);
}

/* 
 * Get a rid of a mmapped char array
 */ 
void SimpleMmapPleaseDisposeOfThisCharArray(
    mmap_t *mmapped_data
)
{
    if (NULL == mmapped_data)
    {
        return;
    }

    if (0 != munmap(mmapped_data->data, mmapped_data->size))
    {
        fputs("munmap error", stderr);
    }
    close(mmapped_data->handle);
}
