/* 
 * (c) Kevin Thacker 2002
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef __OPTION_HANDLER_HEADER_INCLUDED__
#define __OPTION_HANDLER_HEADER_INCLUDED__

#ifndef NULL
#define NULL 0
#endif

typedef enum
{
	/* no error */
	OPTION_OK,
	/* option specified, but not recognised */
	OPTION_NOT_RECOGNISED,
	/* option exists, but is not handled */
	OPTION_NOT_HANDLED,
	/* option is missing a parameter */
	OPTION_MISSING_PARAMETER,
	/* option is the wrong type */
	OPTION_WRONG_TYPE,
	/* bad value */
	OPTION_BAD_VALUE,
} OPTION_ENUM;

typedef struct
{
	/* count of current argument */
	int Count;
	/* number of args */
	int argc;
	/* argument array */
	const char **argv;
} ARGUMENT_DATA;

typedef int (*ERROR_FUNCTION)(const char *pErrorText, ...);

typedef struct
{
	/* name of option */
	const char *option_string;
	/* function to handle option */
	int (*pOptionFunction)(ARGUMENT_DATA *pHandler);
} OPTION;

/* execute the argument list */
int ArgumentList_Execute(const int argc, const char **argv,OPTION *pOptionTable, ERROR_FUNCTION pOutputError, int (*pNonOptionHandler)(const char *pArgument));

/* get next string in argument list */
const char *ArgumentList_GetNext(ARGUMENT_DATA *pHandler);

/* very option is a decimal number */
int Option_VerifyDecimalNumber(const char *pOptionName);

#endif

