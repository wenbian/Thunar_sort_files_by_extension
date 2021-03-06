/* $Id: thunar-path-entry.h 18843 2005-11-14 14:25:58Z benny $ */
/*-
 * Copyright (c) 2005 Benedikt Meurer <benny@xfce.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __THUNAR_PATH_ENTRY_H__
#define __THUNAR_PATH_ENTRY_H__

#include <thunar/thunar-file.h>

G_BEGIN_DECLS;

typedef struct _ThunarPathEntryClass ThunarPathEntryClass;
typedef struct _ThunarPathEntry      ThunarPathEntry;

#define THUNAR_TYPE_PATH_ENTRY            (thunar_path_entry_get_type ())
#define THUNAR_PATH_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), THUNAR_TYPE_PATH_ENTRY, ThunarPathEntry))
#define THUNAR_PATH_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), THUNAR_TYPE_PATH_ENTRY, ThunarPathEntryClass))
#define THUNAR_IS_PATH_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THUNAR_TYPE_PATH_ENTRY))
#define THUNAR_IS_PATH_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), THUNAR_TYPE_PATH_ENTRY))
#define THUNAR_PATH_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), THUNAR_TYPE_PATH_ENTRY, ThunarPathEntryClass))

GType       thunar_path_entry_get_type         (void) G_GNUC_CONST;

GtkWidget  *thunar_path_entry_new              (void);

ThunarFile *thunar_path_entry_get_current_file (ThunarPathEntry *path_entry);
void        thunar_path_entry_set_current_file (ThunarPathEntry *path_entry,
                                                ThunarFile      *current_file);

G_END_DECLS;

#endif /* !__THUNAR_PATH_ENTRY_H__ */
