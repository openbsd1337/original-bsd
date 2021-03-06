/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)source.h	8.1 (Berkeley) 06/06/93
 */

/*
 * Definitions for interfacing with source management routines.
 */

char *cursource;		/* current source file name */
LINENO lastlinenum;		/* last source line number */

int skimsource();		/* get seek pointers to source lines */
int chkline();			/* checks to see that a line number is valid */
int printlines();		/* print out from first line to second */
