divert(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
#
# %sccs.include.redist.sh%
#

#
#  This is a Berkeley-specific configuration file for a specific
#  machine in the Computer Science Division at Berkeley, and should
#  not be used elsewhere.   It is provided on the sendmail distribution
#  as a sample only.
#
#  This file is for the BSD development machine; it has some parameters
#  set up (to stress sendmail) and accepts mail for some other machines.
#

include(`../m4/cf.m4')
VERSIONID(`@(#)vangogh.cs.mc	8.3 (Berkeley) 04/21/95')
DOMAIN(CS.Berkeley.EDU)dnl
OSTYPE(bsd4.4)dnl
MAILER(local)dnl
MAILER(smtp)dnl
define(`MCI_CACHE_SIZE', 5)
Cw okeeffe.CS.Berkeley.EDU
Cw python.CS.Berkeley.EDU
