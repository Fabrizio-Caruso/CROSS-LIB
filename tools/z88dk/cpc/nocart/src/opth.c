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
#include "opth.h"
#include <string.h>
#include <ctype.h>

/*-------------------------------*/
/* all characters must be digits */

/* return 0 for not a number, 1 for is a number */
int Option_VerifyDecimalNumber(const char *pOptionName)
{
	unsigned long i;

	if (pOptionName==NULL)
		return 0;

	for (i=0; i<strlen(pOptionName); i++)
	{
		if (!isdigit(pOptionName[i]))
			return 0;
	}
	
	return 1;
}


/*----------------------------*/
/* locate option data in list */
static OPTION *Option_LookUp(OPTION *pOptionTable, const char *pOptionName)
{
	OPTION *pOption;

	pOption = pOptionTable;
	
	while (pOption->option_string!=NULL)
	{
		/* name matches? */
		if (strcmp(pOption->option_string,pOptionName)==0)
		{
			/* return this option */
			return pOption;
		}
		
		pOption++;
	}

	/* option not found! */
	return NULL;
}

/*----------------------------------*/
/* initialise argument list handler */
static void	ArgumentData_Initialise(ARGUMENT_DATA *pHandler,const int argc, const char **argv)
{
	/* setup count - argument 0 is the name of the program run */
	pHandler->Count = 1;
	/* number of arguments specified */
	pHandler->argc = argc;
	/* pointer to each argument */
	pHandler->argv = argv;
}

/*----------------------------------------------*/
/* get next item from list */
/* returns NULL if no more arguments available, otherwise returns the argument */
const char *ArgumentList_GetNext(ARGUMENT_DATA *pHandler)
{
	/* while there are arguments remaining */
	while (pHandler->Count<pHandler->argc)
	{
		pHandler->Count++;
		
		return pHandler->argv[pHandler->Count-1];
	}

	/* no more available */
	return NULL;
}


static void ArgumentList_OutputError(int Code, const char *pOptionText, ERROR_FUNCTION pErrorFunction)
{
	switch (Code)
	{
		case OPTION_NOT_HANDLED:
		{
			pErrorFunction("Option \"%s\" is not handled!\n",pOptionText);
		}
		break;

		case OPTION_NOT_RECOGNISED:
		{
			pErrorFunction("Option \"%s\" is not recognised!\n",pOptionText);
		}
		break;

		case OPTION_MISSING_PARAMETER:
		{
			pErrorFunction("Option \"%s\" is missing parameter(s)!\n",pOptionText);
		}
		break;
		case OPTION_BAD_VALUE:
		{
			pErrorFunction("Option \"%s\" has an invalid value!\n",pOptionText);
		}
		break;
	}
}

/*----------------------------------------------*/
/* execute all options - 0 if no error occured */
int ArgumentList_Execute(const int argc, const char **argv,OPTION *pOptionTable, ERROR_FUNCTION pOutputError, int (*pNonOptionHandler)(const char *pArgument))
{
	const char *pOptionText;
	ARGUMENT_DATA ArgumentData;

	ArgumentData_Initialise(&ArgumentData, argc, argv);
	
	pOptionText = ArgumentList_GetNext(&ArgumentData);

	while (pOptionText)	
	{
		OPTION *pOption;

		/* does this string begin with the option char? */
		if (pOptionText[0]=='-')
		{
			/* check option string is not just "-" */
			if (pOptionText[1]!='\0')
			{
				/* look up */
				pOption  = Option_LookUp(pOptionTable, &pOptionText[1]);
			
				if (pOption!=NULL) 
				{
					/* handler function exists? */
					if (pOption->pOptionFunction!=NULL)
					{
						int Status;

						/* execute handler function */
						Status = pOption->pOptionFunction(&ArgumentData);

						if (Status!=OPTION_OK)
						{
							ArgumentList_OutputError(Status, pOptionText, pOutputError);

							return Status;
						}
					}
					else
					{
						ArgumentList_OutputError(OPTION_NOT_HANDLED, pOptionText, pOutputError);
						
						return OPTION_NOT_HANDLED;
					}
				}
				else
				{
					/* option not found in the list; therefore option is not recognised */
					ArgumentList_OutputError(OPTION_NOT_RECOGNISED, pOptionText, pOutputError);

					return OPTION_NOT_RECOGNISED;
				}
			}
			else
			{
				/* option was not recognised! */
				ArgumentList_OutputError(OPTION_NOT_HANDLED, pOptionText, pOutputError);
				
				return OPTION_NOT_RECOGNISED;
			}
		}
		else
		{
			if (pNonOptionHandler)
				pNonOptionHandler(pOptionText);
		}

		pOptionText = ArgumentList_GetNext(&ArgumentData);
	}

	/* no errors */
	return OPTION_OK;
}

