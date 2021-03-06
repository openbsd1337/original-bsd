/*-
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)function.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

/*
 * This file, which never produces a function.o, is used solely to
 * be run through the preprocessor.
 *
 * On a 4.3 system (or even msdos), "cc -E function.h" would produce
 * the correct output.  Unfortunately, 4.2 compilers aren't quite that
 * useful.
 */

#include "function.h"
