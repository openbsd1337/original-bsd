divert(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988 The Regents of the University of California.
# All rights reserved.
#
# %sccs.include.redist.sh%
#
divert(0)
VERSIONID(@(#)CS.Berkeley.EDU.m4	2.12 (Berkeley) 12/20/91)
DOMAIN(Berkeley)dnl
ifdef(`_OLD_SENDMAIL_', `', `FEATURE(no_wildcard_MX)')dnl
HACK(cssubdomain)dnl
define(`USERDB_SPEC', `/usr/sww/share/lib/users.cs.db')dnl
