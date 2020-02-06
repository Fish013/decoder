/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * vim: set tabstop=4 syntax=c :
 *
 * Copyright (C) 2014-2020, Peter Haemmerlein (peterpawn@yourfritz.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program, please look for the file LICENSE.
 */

#define OPTIONS_C

#include "common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wformat-security"

EXPORTED	bool	setAlternativeEnvironment(char * newEnvironment)
{
	struct stat		st;

	if (stat(newEnvironment, &st))
	{
		int			error = errno;

		errorMessage(errorAccessingEnvironment, errno, strerror(error), newEnvironment);
		return false;
	}
	setEnvironmentPath(newEnvironment);

	return true;
}

EXPORTED	bool	checkLastArgumentIsInputFile(char * name)
{
	struct stat		st;

	if (stat(name, &st))
		return false;

	verboseMessage(verboseRedirectStdin, name);
	/*
		It's possible to re-assign 'stdin' with glibc and others, but with
		C89/C99 'stdin' is a macro and for better portability, we use an own
		variable here.
		The old fd may be closed first (according to POSIX), as a result the
		stream behind the pointer may be invalid, if an error occurs.
	*/
	FILE * stdin_reloc = freopen(name, "r", stdin);
	if (!stdin_reloc)
	{
		errorMessage(errorUnexpectedIOError, errno, "freopen", "STDIN");
		setError(IO_ERROR);
	}

	return (stdin_reloc ? true : false);
}

EXPORTED	void	warnAboutExtraArguments(char ** argv, int i)
{
	int		index = i;

	while (argv[index])
	{
		warningMessage(verboseTooMuchArguments, argv[index]);
		if (isStrict())
			setError(WARNING_ISSUED);
		index++;
	}
}

static		char	optionString[16];

EXPORTED	char *	optionsString(int option, const char * longOption)
{
	if (longOption)
		snprintf(optionString, sizeof(optionString), "--%s", longOption);
	else
		snprintf(optionString, sizeof(optionString), "-%c", option);
	return optionString;
}

EXPORTED	int		setLineWidth(char * value, char * option, char * next)
{
	char *			val = value;
	int				offset = 0;

	setLineWrap();
	if (!value)
	{
		if (next && *next && *next != '-' && *next >= '0' && *next <= '9')
		{
			val = next;
			offset = 1;
		}
	}
	
	if (val && *val)
	{
		char *		endString = NULL;
		char *		startString = val;
		unsigned long	lineSize = strtoul(startString, &endString, 10);

		if (*startString && strlen(endString))
		{
			errorMessage(errorInvalidWidth, startString, option);
			return -1;
		}
		setOutputLineWidth(lineSize);
	}

	return offset;
}

EXPORTED	bool	setInputBufferSize(char * value, UNUSED char * option)
{
	char *			val = value;

	if (val && *val)
	{
		char *		endString = NULL;
		char *		startString = val;
		size_t		size = strtoul(startString, &endString, 10);

		if (*startString && strlen(endString))
		{
			if ((strcmp(endString, "k") == 0) || (strcmp(endString, "K") == 0))
			{
				size *= 1024;
			}
			else if ((strcmp(endString, "m") == 0) || (strcmp(endString, "M") == 0))
			{
				size *= 1024 * 1024;
			}
			else
			{
				errorMessage(errorInvalidBufferSize, startString);
				return false;
			}
		}
		if ((size < 32) || (size > (16 * 1024 * 1024)))
		{
			errorMessage(errorInvalidBufferSize, startString);
			return false;
		}
		memoryBufferSetSize(size);
		verboseMessage(verboseBufferSize, size);
	}

	return true;
}

#pragma GCC diagnostic pop
