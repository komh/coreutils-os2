/* Stub for symlink().
   Copyright (C) 2009-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>

/* Specification.  */
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <sys/stat.h>


#if HAVE_SYMLINK || defined(__OS2__)

# undef symlink

# ifdef __OS2__

#  define INCL_DOS
#  define INCL_DOSERRORS
#  include <os2.h>

/* OS/2 kLIBC supports a symlink, but the others do not understand a symlink.
   So copy instead of creating a symlink. */
static int
os2_symlink (char const *contents, char const *name)
{
  char szOld[CCHMAXPATH];
  char szNew[CCHMAXPATH];
  char *p;
  ULONG rc;

  if (strlen (contents) >= sizeof (szOld) ||
      strlen (name) >= sizeof (szNew))
    {
      errno = ENAMETOOLONG;

      return -1;
    }

  strcpy (szOld, contents);
  strcpy (szNew, name);

  /* Convert slashes to backslashes */
  for (p = szOld; *p; p++)
    if (*p == '/')
      *p = '\\';

  for (p = szNew; *p; p++)
    if (*p == '/')
      *p = '\\';

  rc = DosCopy (szOld, szNew, 0);

  if (rc == 0)
    return 0;

  switch (rc)
    {
    case ERROR_FILE_NOT_FOUND:          errno = ENOENT; break;
    case ERROR_PATH_NOT_FOUND:          errno = ENOENT; break;
    case ERROR_ACCESS_DENIED:           errno = EACCES; break;
    case ERROR_NOT_DOS_DISK:            errno = EIO; break;
    case ERROR_SHARING_VIOLATION:       errno = EACCES; break;
    case ERROR_SHARING_BUFFER_EXCEEDED: errno = EIO; break;
    case ERROR_INVALID_PARAMETER:       errno = EINVAL; break;
    case ERROR_DRIVE_LOCKED:            errno = EACCES; break;
    case ERROR_DISK_FULL:               errno = ENOSPC; break;
    case ERROR_FILENAME_EXCED_RANGE:    errno = ENAMETOOLONG; break;
    case ERROR_DIRECTORY:               errno = EINVAL; break;
    case ERROR_EAS_NOT_SUPPORTED:       errno = ENOTSUP; break;
    case ERROR_NEED_EAS_FOUND:          errno = EINVAL; break;
    default:                            errno = EINVAL; break;
    }

  return -1;
}

#  define symlink(a, b) os2_symlink(a, b)
# endif

/* Create a symlink, but reject trailing slash.  */
int
rpl_symlink (char const *contents, char const *name)
{
  size_t len = strlen (name);
  if (len && name[len - 1] == '/')
    {
      struct stat st;
      if (lstat (name, &st) == 0)
        errno = EEXIST;
      return -1;
    }
  return symlink (contents, name);
}

#else /* !HAVE_SYMLINK */

/* The system does not support symlinks.  */
int
symlink (char const *contents _GL_UNUSED,
         char const *name _GL_UNUSED)
{
  errno = ENOSYS;
  return -1;
}

#endif /* !HAVE_SYMLINK */
