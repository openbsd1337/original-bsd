/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Edward Wang at The University of California, Berkeley.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)tth29.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

#include "ww.h"
#include "tt.h"
#include "char.h"

/*
 * H29 Driver
 *
 * WWM_USR mode is alternate character set.
 *
kC|h29|heath-29|z29|zenith-29:\
	:am:bc=\ED:bt=\E-:cr=^M:do=^J:nl=^J:bl=^G:\
	:al=\EL:le=^H:bs:cd=\EJ:ce=\EK:cl=\EE:cm=\EY%+ %+ :co#80:dc=\EN:\
	:dl=1*\EM:do=\EB:ei=\EO:ho=\EH:im=\E@:li#24:mi:nd=\EC:as=\EF:ae=\EG:\
	:ms:ta=^I:pt:sr=\EI:se=\Eq:so=\Ep:up=\EA:vs=\Ex4:ve=\Ey4:\
	:kb=^H:ku=\EA:kd=\EB:kl=\ED:kr=\EC:kh=\EH:kn#1:k0=\E~:l0=HOME:\
	:k1=\ES:k2=\ET:k3=\EU:k4=\EV:k5=\EW:k6=\EP:k7=\EQ:k8=\ER:k9=\E01:\
	:es:hs:ts=\Ej\Ex5\Ex1\EY8%+ \Eo:fs=\Ek\Ey5:ds=\Ey1:us=\Es8:ue=\Es0:
 *
 */

h29_setmodes(new)
register new;
{
	register modes = '0';

	if (new & WWM_REV)
		modes += 0x01;
	if (new & WWM_BLK)
		modes += 0x02;
	if (new & WWM_DIM)
		modes += 0x04;
	if (new & WWM_UL)
		modes += 0x08;
	if (new & WWM_USR)
		modes += 0x10;
	ttesc('s');
	ttputc(modes);
	if (new & WWM_GRP) {
		if ((tt.tt_modes & WWM_GRP) == 0)
			ttesc('F');
	} else
		if (tt.tt_modes & WWM_GRP)
			ttesc('G');
	tt.tt_modes = new;
}

tt_h29()
{
	if (tt_h19() < 0)
		return -1;
	tt.tt_setmodes = h29_setmodes;
	tt.tt_availmodes |= WWM_BLK|WWM_UL|WWM_DIM|WWM_USR;
	return 0;
}
