/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char copyright[] =
"@(#) Copyright (c) 1980, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)version.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

#include <sys/time.h>
#include <stdio.h>

extern char	version[];

main()
{
    long	time();
    long	clock;
    struct tm	*localtime();
    struct tm	*tmp;
    int		major;
    int		minor;

    time(&clock);
    tmp = localtime(&clock);
    sscanf(version, "%d.%d", &major, &minor);
    minor += 1;
    printf("char	version[] = \"%d.%d (%d/%d/%d)\";\n",
	    major, minor, tmp->tm_mon+1, tmp->tm_mday, tmp->tm_year);
    exit(0);
}
