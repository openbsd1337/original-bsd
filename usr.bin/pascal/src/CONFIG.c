/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)CONFIG.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

    /*
     *	make sure you are editing
     *		CONFIG.c
     *	editing config.c won't work
     */

    /*
     *	the version of translator
     */
char	*version = "VERSION (DATE)";

    /*
     *	the location of the error strings
     *	and the length of the path to it
     *	(in case of execution as a.something)
     */
char	*err_file = "LIBDATA/ERRORSTRINGS";
int	err_pathlen = sizeof("LIBDATA/")-1;

    /*
     *	the location of the short explanation
     *	and the length of the path to it
     *	the null at the end is so pix can change it to pi'x' from pi.
     */
char	how_string[] = "LIBDATA/HOWFILE\0";
char	*how_file = how_string;
int	how_pathlen = sizeof("LIBDATA/")-1;
    
    /*
     *	things about the interpreter.
     *	these are not used by the compiler.
     */
#ifndef PC
char	*px_header = "LIBEXEC/px_header";	/* px_header's name */
#endif

#ifndef PXP
char	*pi_comp = "INSTALLDIR/pi";		/* the compiler's name */
char	*px_intrp = "INSTALLDIR/px";		/* the interpreter's name */
char	*px_debug = "INSTALLDIR/pdx";		/* the debugger's name */
#endif
