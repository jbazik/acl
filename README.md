# Access control list utilities

This is a fork of

> [http://savannah.nongnu.org/projects/acl](http://savannah.nongnu.org/projects/acl)

The nfs4 branch applies a patch that mimics POSIX acl semantics on NFSv4
filesystems by translating POSIX acls to NFSv4 acls and vice versa.  The
patch comes from UMich:

> [http://www.citi.umich.edu/projects/nfsv4/linux/](http://www.citi.umich.edu/projects/nfsv4/linux/)

The acl translation is described here:

> [https://tools.ietf.org/id/draft-ietf-nfsv4-acl-mapping-05.txt](https://tools.ietf.org/id/draft-ietf-nfsv4-acl-mapping-05.txt)

Note that the translation is not perfect in either direction.

## What is this about?

You have NFSv4 mounts on linux systems, and users accustomed to using
POSIX acls.  The patch allows them to continue using POSIX acl semantics
without regard to the underlying filesystem.

The most recent patch from UMich is for an old version of acl utilities
(2.2.42).  The code here adapts the patch for the most recent version
of acl, rebasing the changes on top of the savannah original.

## No Denies

In addition, a build-time option eliminates deny aces from the
translation, which makes it simpler but less accurate.  Still it's
probably what most POSIX acl users actually expect anyway.

For instance, you have file with a file mode of 0755, and you add
a read acl for Bob.  Did you really mean to prevent Bob from executing
the file?  That's what a POSIX acl would do.  No, I didn't think so.

## Be careful how you use this

Don't just slap this on your system.  It replaces libacl, which a lot of
core utilities (e.g. cp) depend on, and it will alter the way they work,
as well as adding dependencies on id mapping services.

## Build

Build in the usual autoconf way:

    ./autogen.sh
    ./configure --enable-nfs4=nodeny
    make

## TODO

Factor the patch into a separate standalone library and rewrite the
commands (getfacl and setfacl) to do the fallback from POSIX to NFSv4.
That would make it safe to install and more resilient to upstream
changes.

## License

The original code is all GPL2.  The UMich patch has an open-source-looking
UMich license.
