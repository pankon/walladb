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

typedef struct simple_json 
{
} json_t;

/*
 * Builds Json to string
 */
char *GetJsonString(json_t *json)
{
	int total_length = 0;
	int i = 0;
	for (i = 0; i < json->length; ++i)
	{
		PrepareJsonSegment(json->segment[i], &total_length);
	}

	return (NULL);
}
