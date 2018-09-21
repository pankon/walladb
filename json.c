/**************************************
 * simple_json.c
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * Usage:
 *
 * Service features:
 * Makes a json
 * Tue Sep 17 09:21:00 2018
***************************************/

#include <stdio.h>
#include <stdlib.h> /* malloc, free 	*/

#include "json.h"

char *JsonGetNull()
{
	static char *null_json = "{}";
	char *json = NULL;

	if (NULL == (json = malloc(3)))
	{
		return (NULL);
	}

	*json = *null_json;
	*(json + 1) = *(null_json + 1);
	*(json + 2) = 0;

	return (json);
}