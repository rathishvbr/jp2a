/*
 * Copyright (C) 2006 Christian Stigen Larsen, http://csl.sublevel3.org
 * Distributed under the GNU General Public License (GPL) v2 or later.
 *
 * Project homepage on http://jp2a.sf.net
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CURSES_H
#include <curses.h>
#endif

#ifdef HAVE_TERM_H
#include <term.h>
#endif

/*
 * Returns:  1  success
 *           0  terminal type not defined
 *          -1  termcap database inaccessible
 *          -2  environment variable TERM not set
 *
 * Usage:
 *
 *       char* error;
 *       int width, height;
 *
 *       if ( get_termsize(&width, &height, &error) != 1 ) {
 *           fprintf(stderr, "%s\n", error);
 *           exit(1);
 *       }
 *
 *       printf("Your terminal has %d columns and %d lines\n", width, height);
 */
int get_termsize(int* _width, int* _height, char** err) {
	static char errstr[1024];
	errstr[0] = 0;
	*err = errstr;

	char *termtype = getenv("TERM");
	char term_buffer[2048];

	if ( !termtype ) {
		strcpy(errstr, "Environment variable TERM not set.");
		return -2;
	}

	int i = tgetent(term_buffer, termtype);

	if ( !i ) {
		snprintf(errstr, sizeof(errstr)/sizeof(char) - 1,
			"Terminal type '%s' not defined.", termtype);
		return 0;
	}

	if ( i < 0 ) {
		strcpy(errstr, "Could not access the termcap database.");
		return -1;
	}

	*_width = tgetnum("co");
	*_height = tgetnum("li");

	return 1;
}
