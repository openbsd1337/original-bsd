/*
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Harris Corp.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)hd.c	7.8 (Berkeley) 06/28/90
 */

#include "sys/param.h"
#include "sys/time.h"
#include "sys/vnode.h"
#include "ufs/inode.h"
#include "ufs/fs.h"
#include "sys/buf.h"
#include "sys/ioctl.h"
#include "sys/disklabel.h"
#include "saio.h"
#include "tahoe/mtpr.h"
#include "tahoevba/hdreg.h"

static struct registers *hdc_regs[HDC_MAXCTLR][HDC_MAXBUS];
static struct disklabel dklabel[HDC_MAXDRIVE][HDC_MAXCTLR][HDC_MAXBUS];

hdopen(io)
	register struct iob *io;
{
	register struct disklabel *dlp;
	struct status status;
	struct module_id id;
	struct registers *hr;
	struct mcb mcb;
	long junk, dlbuf[DEV_BSIZE/sizeof(long)];

	/* validate the device specification */
	if ((u_int)io->i_bus >= HDC_MAXBUS)
		return(EADAPT);
	if ((u_int)io->i_ctlr >= HDC_MAXCTLR)
		return(ECTLR);
	if ((u_int)io->i_unit >= HDC_MAXDRIVE)
		return(EUNIT);
	if ((u_int)io->i_part > 7)
		return(EPART);

	/* init drive structure. */
	hdc_regs[io->i_ctlr][io->i_bus] = hr = (struct registers *)(io->i_bus ?
	    0x80000000 | io->i_ctlr << 24 | HDC_MID << 16 :
	    0xC0000000 | io->i_ctlr << 24 | HDC_MID << 16);

	/* insure that this is an hdc, then reset the hdc. */
	if (wbadaddr(&hr->module_id, 4, &junk)) {
		printf("hd%d: %x: invalid csr\n", io->i_ctlr, (u_int)hr);
		return(ENXIO);
	}
	hr->soft_reset = 0;
	DELAY(1000000);

	/*
	 * read in the hdc module id word.  The controller is bad if the
	 * hdc's writeable control store is not loaded or if the hdc failed
	 * the functional integrity test for any reason.
	 */
	hr->module_id = (u_long)&id;
	DELAY(10000);
	mtpr(PADC, 0);
	if (id.module_id != (u_char)HDC_MID) {
		printf("hdc: controller bad module id: id = %x\n",
		    id.module_id);
		return(ENXIO);
	}
	if (id.code_rev == (u_char)0xff) {
		printf("hdc: controller micro-code is not loaded.\n");
		return(ENXIO);
	}
	if (id.fit != (u_char)0xff) {
		printf("hdc: controller FIT test failed: error= %x\n",
		    id.fit);
		return(ENXIO);
	}

	/* read the drive status */
	mcb.command = HCMD_STATUS;
	mcb.drive = io->i_unit;
	mcb.cyl = 0;
	mcb.head = 0;
	mcb.sector = 0;
	mcb.chain[0].wcount = (long)(sizeof(struct status) / sizeof(long));
	mcb.chain[0].memadr = (long)&status;
	if (hdimcb(&mcb, io))
		return(EIO);

	/*
	 * Report drive down if anything in the drive status is bad.
	 * If fault condition, reading will try to clear the fault.
	 */
	if (status.drs&DRS_FAULT)
		printf("hdc: clearing drive fault.\n");
	if (!(status.drs&DRS_ONLINE)) {
		printf("hdc: drive is not online.\n");
		return(EIO);
	}

	/* read in the pack label */
	mcb.command = HCMD_READ;
	mcb.drive = io->i_unit;
	mcb.cyl = 0;
	mcb.head = 0;
	mcb.sector = LABELSECTOR;
	mcb.chain[0].wcount = (long)(DEV_BSIZE / sizeof(long));
	mcb.chain[0].memadr = (long)dlbuf;
	if (hdimcb(&mcb, io))
		return(ERDLAB);
	dlp = (struct disklabel *)(dlbuf + (LABELOFFSET / sizeof(long)));
	if (dlp->d_magic != DISKMAGIC || dlp->d_magic2 != DISKMAGIC)
#ifdef COMPAT_42
	{
		int error;

		if (error = hdmaptype(io, dlp, &status, io->i_unit))
			return(error);
	}
#else
		return(EUNLAB);
#endif
	dklabel[io->i_unit][io->i_ctlr][io->i_bus] = *dlp;
	if (io->i_part >= dlp->d_npartitions ||
	    dlp->d_partitions[io->i_part].p_size == 0)
		return(EPART);
	io->i_boff = (dlp->d_partitions[io->i_part].p_offset *
	    dlp->d_secsize) / DEV_BSIZE;
	return(0);
}

hdstrategy(io, cmd)
	register struct iob *io;
	int cmd;
{
	register struct disklabel *dlp;
	struct mcb mcb;
	long sector;

	if (io->i_cc&3) {
		printf("hd%d: i/o not a longword multiple.\n", io->i_unit);
		return(0);
	}
	dlp = &dklabel[io->i_unit][io->i_ctlr][io->i_bus];
	sector = io->i_bn * HDC_SPB;
	mcb.command = (cmd == READ) ? HCMD_READ : HCMD_WRITE;
	mcb.drive = io->i_unit;
	mcb.cyl = sector / dlp->d_secpercyl;
	mcb.head = (sector / dlp->d_nsectors) % dlp->d_ntracks;
	mcb.sector = sector % dlp->d_nsectors;
	mcb.chain[0].wcount = io->i_cc / sizeof(long);
	mcb.chain[0].memadr  = (u_long)io->i_ma;
	return(hdimcb(&mcb, io) ? -1 : io->i_cc);
}

hdimcb(mcb, io)
	register struct mcb *mcb;
	register struct iob *io;
{
	struct master_mcb master;
	int timeout;

	/* fill in mcb */
	mcb->interrupt = 0;
	mcb->forw_phaddr = 0;

	/* fill in master mcb */
	master.mcw = MCL_IMMEDIATE;
	master.forw_phaddr = (u_long)mcb;
	master.mcs = 0;

	hdc_regs[io->i_ctlr][io->i_bus]->master_mcb = (u_long)&master;
	for (timeout = 15000; timeout; --timeout) {
		DELAY(1000);
		mtpr(PADC, 0);
		if (master.mcs&MCS_FATALERROR) {
			printf("hdc%d: fatal error.\n", io->i_ctlr);
			return(1);
		}
		if (master.mcs&MCS_DONE)
			return(0);
	}
	printf("hdc%d: timed out.\n", io->i_ctlr);
	return(1);
}

#ifdef COMPAT_42
hdmaptype(io, dlp, status, unit)
	register struct iob *io;
	register struct disklabel *dlp;
	struct status *status;
	int unit;
{
	geometry_sector geometry;
	geometry_block *geo;
	struct mcb mcb;
	int cnt;
	char *strcpy();

	printf("hd%d: unlabeled\n", unit);
	/*
	 * Read the geometry block (at head = 0 sector = 0 of the drive
	 * definition cylinder), validate it (must have the correct version
	 * number, header, and checksum).
	 */
	mcb.command = HCMD_READ;
	mcb.drive = unit;
	mcb.cyl = status->def_cyl;
	mcb.head = 0;
	mcb.sector = 0;
	mcb.chain[0].wcount = (long)(sizeof(geometry_sector) / sizeof(long));
	mcb.chain[0].memadr = (long)&geometry;
	if (hdimcb(&mcb, io)) {
 		printf("hd%d: can't read default geometry.\n", io->i_unit);
		return(ERDLAB);
	}
	geo = &geometry.geometry_block;
 	if (geo->version > 64000  ||  geo->version < 0) {
 		printf("hd%d: bad default geometry version#.\n", io->i_unit);
		return(ENXIO);
	}
 	if (strcmp(&geo->id[0], GB_ID)) {
 		printf("hd%d: bad default geometry header.\n", io->i_unit);
		return(ENXIO);
	}
	GB_CHECKSUM(geo, cnt);
	if (geometry.checksum != cnt) {
		printf("hd%d: bad default geometry checksum.\n", io->i_unit);
		return(ENXIO);
	}
	for (cnt = 0; cnt < GB_MAXPART; cnt++) {
		dlp->d_partitions[cnt].p_offset = geo->partition[cnt].start;
		dlp->d_partitions[cnt].p_size = geo->partition[cnt].length;
	}
#ifdef RAW_SIZE
	dlp->d_secsize = status->bytes_per_sec;
#else
	dlp->d_secsize = 512;
#endif
	dlp->d_nsectors = status->max_sector + 1;
	dlp->d_ncylinders = status->max_cyl + 1;
	dlp->d_ntracks = status->max_head + 1;
	dlp->d_secpercyl = dlp->d_ntracks * dlp->d_nsectors;
	dlp->d_npartitions = GB_MAXPART;
	dlp->d_rpm = status->rpm;
	(void)strcpy(dlp->d_typename, "hdc (prom)");
	return(0);
}
#endif /* COMPAT_42 */
