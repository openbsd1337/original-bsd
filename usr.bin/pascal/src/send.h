/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)send.h	8.1 (Berkeley) 06/06/93
 */

#define	RINIT	1
#define	RENQ	2
#define	RTREE	3
#define	RTRFREE	4
#define	RTRCHK	5
#define	REVENIT	6
#define	RSTRING	7
#define	REVLAB	8
#define	REVCNST	9
#define	REVTBEG	10
#define	REVTYPE	11
#define	REVTEND	12
#define	REVVBEG	13
#define	REVVAR	14
#define	REVVEND	15
#define	REVFHDR	16
#define	REVFFWD	17
#define	REVFBDY	18
#define	REVFEND	19
#define	ROPUSH	20
#define	ROPOP	21
#define	ROSET	22
#define	RKILL	23
#define	RFINISH	24

#define	RLAST	24

extern	char *trdesc[];
