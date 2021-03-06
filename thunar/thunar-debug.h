/* $Id: thunar-debug.h 20445 2006-03-15 17:42:54Z benny $ */
/*-
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>.
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

#ifndef __THUNAR_DEBUG_H__
#define __THUNAR_DEBUG_H__

#include <glib.h>

G_BEGIN_DECLS;

#if defined(G_ENABLE_DEBUG) && defined(G_HAVE_ISO_VARARGS)

#define THUNAR_DEBUG_MARK(...)                          \
G_STMT_START{                                           \
  thunar_debug_mark (__FILE__, __LINE__, __VA_ARGS__);  \
}G_STMT_END

#else

#define THUNAR_DEBUG_MARK(...)  \
G_STMT_START{                   \
  (void)0;                      \
}G_STMT_END

#endif

void thunar_debug_mark (const gchar *file,
                        const gint   line,
                        const gchar *format,
                        ...);

G_END_DECLS;

#endif /* !__THUNAR_DEBUG_H__ */
