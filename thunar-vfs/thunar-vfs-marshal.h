
#ifndef ___thunar_vfs_marshal_MARSHAL_H__
#define ___thunar_vfs_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* BOOLEAN:POINTER (thunar-vfs-marshal.list:1) */
extern void _thunar_vfs_marshal_BOOLEAN__POINTER (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data) G_GNUC_INTERNAL;

/* FLAGS:BOXED,BOXED (thunar-vfs-marshal.list:2) */
extern void _thunar_vfs_marshal_FLAGS__BOXED_BOXED (GClosure     *closure,
                                                    GValue       *return_value,
                                                    guint         n_param_values,
                                                    const GValue *param_values,
                                                    gpointer      invocation_hint,
                                                    gpointer      marshal_data) G_GNUC_INTERNAL;

/* FLAGS:STRING,FLAGS (thunar-vfs-marshal.list:3) */
extern void _thunar_vfs_marshal_FLAGS__STRING_FLAGS (GClosure     *closure,
                                                     GValue       *return_value,
                                                     guint         n_param_values,
                                                     const GValue *param_values,
                                                     gpointer      invocation_hint,
                                                     gpointer      marshal_data) G_GNUC_INTERNAL;

/* VOID:UINT64,UINT,UINT,UINT (thunar-vfs-marshal.list:4) */
extern void _thunar_vfs_marshal_VOID__UINT64_UINT_UINT_UINT (GClosure     *closure,
                                                             GValue       *return_value,
                                                             guint         n_param_values,
                                                             const GValue *param_values,
                                                             gpointer      invocation_hint,
                                                             gpointer      marshal_data) G_GNUC_INTERNAL;

G_END_DECLS

#endif /* ___thunar_vfs_marshal_MARSHAL_H__ */

