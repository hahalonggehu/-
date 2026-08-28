#ifndef MZ_CONFIG_H
#define MZ_CONFIG_H

#if defined(__SWITCH__)
#  define HAVE_DIRENT_H 1
#  define HAVE_SYS_DIRENT_H 0
#  define HAVE_PDIR 1
#else
#  define HAVE_DIRENT_H 0
#  define HAVE_SYS_DIRENT_H 0
#  define HAVE_PDIR 0
#endif

#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_FSEEKO 1
#define HAVE_SYMLINK 0
#define HAVE_READLINK 0

#endif