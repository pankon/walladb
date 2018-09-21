/**************************************
 * walla_status.c
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * 
 * Just holds the enum
 * 
 * Fri Sep 14 14:51:47 2018
***************************************/

#include <stdio.h> /* NULL */

#include "walla_status.h"

char *WallaStatusToString(WALLA_STATUS status)
{
	static char *success = "Success";
	static char *oops = "Ooops.. something went wrong";
	static char *bye = "Bye.";
	static char *bad_format = "Bad format!";
	static char *not_implemented = "Not implemented yet!";
	static char *you_forgot_to_pass_anything = "You forgot to pass anything!";
	
	switch (status)
	{
		case WALLA_SUCCESS:
			return (success);
		case WALLA_OOPS:
			return (oops);
		case WALLA_BYE:
			return (bye);
		case WALLA_BAD_FORMAT:
			return (bad_format);
		case WALLA_NOT_IMPLEMENTED:
			return (not_implemented);
		case WALLA_YOU_FORGOT_TO_PASS_ANYTHING:
			return (you_forgot_to_pass_anything);
	}

	return (NULL);
}