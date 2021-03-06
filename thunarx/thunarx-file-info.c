/* $Id: thunarx-file-info.c 20531 2006-03-24 14:46:35Z benny $ */
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <thunarx/thunarx-file-info.h>
#include <thunarx/thunarx-private.h>
#include <thunarx/thunarx-alias.h>



/* Signal identifiers */
enum
{
  CHANGED,
  RENAMED,
  LAST_SIGNAL,
};



static guint file_info_signals[LAST_SIGNAL];



GType
thunarx_file_info_get_type (void)
{
  static GType type = G_TYPE_INVALID;

  if (G_UNLIKELY (type == G_TYPE_INVALID))
    {
      static const GTypeInfo info =
      {
        sizeof (ThunarxFileInfoIface),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        NULL,
        NULL,
      };

      /* register the interface type */
      type = g_type_register_static (G_TYPE_INTERFACE, I_("ThunarxFileInfo"), &info, 0);

      /* implementations must inherit GObject */
      g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);

      /**
       * ThunarxFileInfo::changed:
       * @file_info : a #ThunarxFileInfo.
       *
       * Emitted whenever the system notices a change to @file_info.
       *
       * Thunar plugins should use this signal to stay informed about
       * changes to a @file_info for which they currently display
       * information (i.e. in a #ThunarxPropertyPage), and update
       * it's user interface whenever a change is noticed on @file_info.
       **/
      file_info_signals[CHANGED] =
        g_signal_new (I_("changed"),
                      type,
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (ThunarxFileInfoIface, changed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

      /**
       * ThunarxFileInfo::renamed:
       * @file_info : a #ThunarxFileInfo
       *
       * Emitted when the @file_info is renamed to another
       * name.
       *
       * For example, within Thunar, #ThunarFolder uses this
       * signal to reregister it's VFS directory monitor, after
       * the corresponding file was renamed.
       **/
      file_info_signals[RENAMED] =
        g_signal_new (I_("renamed"),
                      type,
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (ThunarxFileInfoIface, renamed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);
    }

  return type;
}



/**
 * thunarx_file_info_get_name:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the real name of the file represented
 * by @file_info in the local file system encoding.
 * You can use g_filename_display_name() or similar
 * functions to generate an UTF-8 version of the
 * name, which is suitable for use in the user
 * interface.
 *
 * The caller is responsible to free the returned
 * string using g_free() when no longer needed.
 *
 * Return value: the real name of the file represented
 *               by @file_info.
 **/
gchar*
thunarx_file_info_get_name (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_name) (file_info);
}



/**
 * thunarx_file_info_get_uri:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the escaped, fully qualified URI
 * of the file object represented by @file_info.
 * You may use g_filename_from_uri() and similar
 * functions to work with the returned URI.
 *
 * The caller is responsible to free the returned
 * string using g_free() when no longer needed.
 *
 * Return value: the fully qualified URI of @file_info.
 **/
gchar*
thunarx_file_info_get_uri (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_uri) (file_info);
}



/**
 * thunarx_file_info_get_parent_uri:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the URI to the parent file of
 * @file_info or %NULL if @file_info has
 * no parent. Note that the parent URI
 * may be of a different type than the
 * URI of @file_info. For example, the
 * parent of "file:///" is "computer:///".
 *
 * The caller is responsible to free the
 * returned string using g_free() when no
 * longer needed.
 *
 * Return value: the parent URI for @file_info
 *               or %NULL.
 **/
gchar*
thunarx_file_info_get_parent_uri (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_parent_uri) (file_info);
}



/**
 * thunarx_file_info_get_uri_scheme:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the URI scheme of the file represented
 * by @file_info. E.g. if @file_info refers to the
 * file "file:///usr/home", the return value will
 * be "file".
 *
 * The caller is responsible to free the returned
 * string using g_free() when no longer needed.
 *
 * Return value: the URI scheme for @file_info.
 **/
gchar*
thunarx_file_info_get_uri_scheme (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_uri_scheme) (file_info);
}



/**
 * thunarx_file_info_get_mime_type:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the MIME-type of the file represented by
 * @file_info or %NULL if no MIME-type is known for
 * @file_info.
 *
 * The caller is responsible to free the returned
 * string using g_free() when no longer needed.
 *
 * Return value: the MIME-type for @file_info or
 *               %NULL.
 **/
gchar*
thunarx_file_info_get_mime_type (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_mime_type) (file_info);
}



/**
 * thunarx_file_info_has_mime_type:
 * @file_info : a #ThunarxFileInfo.
 * @mime_type : a MIME-type (e.g. "text/plain").
 *
 * Checks whether @file_info is of the given @mime_type
 * or whether the MIME-type of @file_info is a subclass
 * of @mime_type.
 *
 * This is the preferred way for most extensions to check
 * whether they support a given file or not, and you should
 * consider using this method rather than
 * thunarx_file_info_get_mime_type(). A simple example would
 * be a menu extension that performs a certain action on
 * text files. In this case you want to check whether a given
 * #ThunarxFileInfo refers to any kind of text file, not only
 * to "text/plain" (e.g. this also includes "text/xml" and
 * "application/x-desktop").
 *
 * But you should be aware that this method may take some
 * time to test whether @mime_type is valid for @file_info,
 * so don't call it too often.
 *
 * Return value: %TRUE if @mime_type is valid for @file_info,
 *               else %FALSE.
 **/
gboolean
thunarx_file_info_has_mime_type (ThunarxFileInfo *file_info,
                                 const gchar     *mime_type)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), FALSE);
  g_return_val_if_fail (mime_type != NULL, FALSE);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->has_mime_type) (file_info, mime_type);
}



/**
 * thunarx_file_info_is_directory:
 * @file_info : a #ThunarxFileInfo.
 *
 * Checks whether @file_info refers to a directory.
 *
 * Return value: %TRUE if @file_info is a directory.
 **/
gboolean
thunarx_file_info_is_directory (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), FALSE);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->is_directory) (file_info);
}



/**
 * thunarx_file_info_get_vfs_info:
 * @file_info : a #ThunarxFileInfo.
 *
 * Returns the #ThunarVfsInfo associated with @file_info,
 * which includes additional information about the @file_info
 * as queried from the VFS library earlier. The caller is
 * responsible to free the returned #ThunarVfsInfo object
 * using thunar_vfs_info_unref() when no longer needed.
 *
 * Note that the <application>thunarx</application> library itself
 * is not linked to the <application>thunar-vfs</application> library,
 * and so, if you need to use this method, you'll need to include
 * <code>&lt;thunar-vfs/thunar-vfs.h&gt;</code> in your code and
 * add <code>`pkg-config --cflags thunar-vfs-1`</code> to your
 * <envar>CFLAGS</envar>.
 *
 * Return value: the #ThunarVfsInfo object associated with @file_info,
 *               which MUST be freed using thunar_vfs_info_unref().
 **/
ThunarVfsInfo*
thunarx_file_info_get_vfs_info (ThunarxFileInfo *file_info)
{
  g_return_val_if_fail (THUNARX_IS_FILE_INFO (file_info), NULL);
  return (*THUNARX_FILE_INFO_GET_IFACE (file_info)->get_vfs_info) (file_info);
}



/**
 * thunarx_file_info_changed:
 * @file_info : a #ThunarxFileInfo.
 *
 * Emits the ::changed signal on @file_info. This method should not
 * be invoked by Thunar plugins, instead the file manager itself
 * will use this method to emit ::changed whenever it notices a
 * change on @file_info.
 **/
void
thunarx_file_info_changed (ThunarxFileInfo *file_info)
{
  g_return_if_fail (THUNARX_IS_FILE_INFO (file_info));
  g_signal_emit (G_OBJECT (file_info), file_info_signals[CHANGED], 0);
}



/**
 * thunarx_file_info_renamed:
 * @file_info : a #ThunarxFileInfo.
 *
 * Emits the ::renamed signal on @file_info. This method should
 * not be invoked by Thunar plugins, instead the file manager
 * will emit this signal whenever the user renamed the @file_info.
 *
 * The plugins should instead connect to the ::renamed signal
 * and update it's internal state and it's user interface
 * after the file manager renamed a file.
 **/
void
thunarx_file_info_renamed (ThunarxFileInfo *file_info)
{
  g_return_if_fail (THUNARX_IS_FILE_INFO (file_info));
  g_signal_emit (G_OBJECT (file_info), file_info_signals[RENAMED], 0);
}



GType
thunarx_file_info_list_get_type (void)
{
  static GType type = G_TYPE_INVALID;

  if (G_UNLIKELY (type == G_TYPE_INVALID))
    {
      type = g_boxed_type_register_static (I_("ThunarxFileInfoList"),
                                           (GBoxedCopyFunc) thunarx_file_info_list_copy,
                                           (GBoxedFreeFunc) thunarx_file_info_list_free);
    }

  return type;
}



/**
 * thunarx_file_info_list_copy:
 * @file_infos : a #GList of #ThunarxFileInfo<!---->s.
 *
 * Does a deep copy of @file_infos and returns the
 * new list.
 *
 * Return value: a copy of @file_infos.
 **/
GList*
thunarx_file_info_list_copy (GList *file_infos)
{
  GList *list = NULL;
  GList *lp;

  for (lp = g_list_last (file_infos); lp != NULL; lp = lp->prev)
    list = g_list_prepend (list, g_object_ref (G_OBJECT (lp->data)));

  return list;
}



/**
 * thunarx_file_info_list_free:
 * @file_infos : a #GList of #ThunarxFileInfo<!---->s.
 *
 * Frees the resources allocated for the @file_infos
 * list and decreases the reference count on the
 * #ThunarxFileInfo<!---->s contained within.
 **/
void
thunarx_file_info_list_free (GList *file_infos)
{
  g_list_foreach (file_infos, (GFunc) g_object_unref, NULL);
  g_list_free (file_infos);
}



#define __THUNARX_FILE_INFO_C__
#include <thunarx/thunarx-aliasdef.c>
