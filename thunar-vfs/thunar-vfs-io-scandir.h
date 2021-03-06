/* $Id: thunar-vfs-io-scandir.h 22534 2006-07-27 16:16:51Z benny $ */
/*-
 * Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if !defined(THUNAR_VFS_COMPILATION)
#error "Only <thunar-vfs/thunar-vfs.h> can be included directly, this file is not part of the public API."
#endif

#ifndef __THUNAR_VFS_IO_SCANDIR_H__
#define __THUNAR_VFS_IO_SCANDIR_H__

#include <thunar-vfs/thunar-vfs-path.h>
#include <thunar-vfs/thunar-vfs-types.h>

G_BEGIN_DECLS;

/**
 * ThunarVfsIOScandirFlags:
 * @THUNAR_VFS_IO_SCANDIR_RECURSIVE    : scan directories recursively.
 * @THUNAR_VFS_IO_SCANDIR_FOLLOW_LINKS : follow symbolic links to directories.
 *
 * Flags for _thnar_vfs_io_scandir().
 **/
typedef enum /*< flags, skip >*/
{
  THUNAR_VFS_IO_SCANDIR_RECURSIVE    = (1L << 0),
  THUNAR_VFS_IO_SCANDIR_FOLLOW_LINKS = (1L << 1),
} ThunarVfsIOScandirFlags;

GList *_thunar_vfs_io_scandir (ThunarVfsPath          *path,
                               volatile gboolean      *cancelled,
                               ThunarVfsIOScandirFlags flags,
                               GError                **error) G_GNUC_INTERNAL G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS;

#endif /* !__THUNAR_VFS_IO_SCANDIR_H__ */
