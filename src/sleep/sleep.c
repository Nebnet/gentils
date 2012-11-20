/*************************************************************************\
	Author: Anonymous
	Date: November 19, 2012
	Description: sleep.c, a contribution to the Gentils project
	Comments: The next version will probably use getopt and have
	support for other OSes. Feel free to change the bracket
	style. For questions, comments, or concerns contact me at
	nebnet@lavabit.com
  =======================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    For more information, see <https://www.gnu.org/licenses/>.
\*************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#if defined(_POSIX_SOURCE)
#include <unistd.h>
#elif defined(_WIN32)
#include <Windows.h>
#else
#include <time.h>
#endif

#define DAY 86400
#define HOUR 3600
#define MINUTE 60
#define SECOND  1
#define VERSION "Gentils sleep v0.1"
#define USAGE   "Usage: sleep `number'[suffix] \
where suffix can be `d' for days, `h' for hours, \
`m' for minutes, or `s' (the default) for seconds, \
or `sleep option'."

int getmultiplier(char*, int);

int
main(int argc, char** argv)
{
	if (argc == 1)
	{
		fputs("sleep: Missing operand.\nSee `sleep --help' for more info.", stdout);
		return 1;
	}

	char* arg;
	uint32_t i, m, s;
	uint64_t sleeptime = 0;

	for (i = 1; i < argc; ++i)
	{
		if (strcmp(argv[1], "--help")==0)
		{
			fputs(USAGE, stdout);
			return 0;
		}
		else if (strcmp(argv[1], "--version")==0)
		{
			fputs(VERSION, stdout);
			return 0;
		}

		if ((m = getmultiplier(argv[i], strlen(argv[i]))) != 0)
		{
			s = m * (int)ceil(atof(argv[i]));
			sleeptime += s;
		}
	}
#if defined(_POSIX_SOURCE)
	return sleep(sleeptime);
#elif defined(_WIN32)
	Sleep(sleeptime * 1000);
#else
	/* Temporary hack in case of any other OSes */
	clock_t ct = clock();
	while ((clock() - ct) * CLOCKS_PER_SEC < sleeptime);
#endif
	return 0;
}

int
getmultiplier(char* s, int l)
{
	char c;
	int i;
	for (i = 0; i < l-1; ++i)
	{
		if (i != l-1 && s[i] != '.' && !isdigit(s[i]))
		{
			printf("sleep: Invalid operand `%s'.\nSee `sleep --help' for more info.\n", s);
			exit(0);
		}
	}

	c = s[l-1];
	switch (c)
	{
		case 'd':
			return DAY;
			break;
		case 'h':
			return HOUR;
			break;
		case 'm':
			return MINUTE;
			break;
	}
	if ((c >= '0' && c <= '9') || c=='s')
	{
		return SECOND;
	}
	return 0;
}
