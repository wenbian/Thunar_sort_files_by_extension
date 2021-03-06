/* $Id: thunar-renamer-dialog.c 28975 2008-12-03 18:11:17Z nick $ */
/*-
 * Copyright (c) 2005-2007 Benedikt Meurer <benny@xfce.org>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <thunar/thunar-abstract-dialog.h>
#include <thunar/thunar-application.h>
#include <thunar/thunar-dialogs.h>
#include <thunar/thunar-gtk-extensions.h>
#include <thunar/thunar-icon-factory.h>
#include <thunar/thunar-icon-renderer.h>
#include <thunar/thunar-launcher.h>
#include <thunar/thunar-private.h>
#include <thunar/thunar-properties-dialog.h>
#include <thunar/thunar-renamer-dialog.h>
#include <thunar/thunar-renamer-dialog-ui.h>
#include <thunar/thunar-renamer-model.h>
#include <thunar/thunar-renamer-progress.h>



/* Property identifiers */
enum
{
  PROP_0,
  PROP_CURRENT_DIRECTORY,
  PROP_SELECTED_FILES,
  PROP_STANDALONE,
};

/* Identifiers for DnD target types */
enum
{
  TARGET_GTK_TREE_MODEL_ROW,
  TARGET_TEXT_URI_LIST,
};



static void     thunar_renamer_dialog_class_init          (ThunarRenamerDialogClass *klass);
static void     thunar_renamer_dialog_init                (ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_dispose             (GObject                  *object);
static void     thunar_renamer_dialog_finalize            (GObject                  *object);
static void     thunar_renamer_dialog_get_property        (GObject                  *object,
                                                           guint                     prop_id,
                                                           GValue                   *value,
                                                           GParamSpec               *pspec);
static void     thunar_renamer_dialog_set_property        (GObject                  *object,
                                                           guint                     prop_id,
                                                           const GValue             *value,
                                                           GParamSpec               *pspec);
static void     thunar_renamer_dialog_realize             (GtkWidget                *widget);
static void     thunar_renamer_dialog_unrealize           (GtkWidget                *widget);
static void     thunar_renamer_dialog_response            (GtkDialog                *dialog,
                                                           gint                      response);
static void     thunar_renamer_dialog_context_menu        (ThunarRenamerDialog      *renamer_dialog,
                                                           guint                     button,
                                                           guint32                   time);
static void     thunar_renamer_dialog_help                (ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_save                (ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_action_add_files    (GtkAction                *action,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_action_remove_files (GtkAction                *action,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_action_clear        (GtkAction                *action,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_action_about        (GtkAction                *action,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_action_properties   (GtkAction                *action,
                                                           ThunarRenamerDialog      *renamer_dialog);
static gboolean thunar_renamer_dialog_button_press_event  (GtkWidget                *tree_view,
                                                           GdkEventButton           *event,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_drag_data_received  (GtkWidget                *tree_view,
                                                           GdkDragContext           *context,
                                                           gint                      x,
                                                           gint                      y,
                                                           GtkSelectionData         *selection_data,
                                                           guint                     info,
                                                           guint                     time,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_drag_leave          (GtkWidget                *tree_view,
                                                           GdkDragContext           *context,
                                                           guint                     time,
                                                           ThunarRenamerDialog      *renamer_dialog);
static gboolean thunar_renamer_dialog_drag_motion         (GtkWidget                *tree_view,
                                                           GdkDragContext           *context,
                                                           gint                      x,
                                                           gint                      y,
                                                           guint                     time,
                                                           ThunarRenamerDialog      *renamer_dialog);
static gboolean thunar_renamer_dialog_drag_drop           (GtkWidget                *tree_view,
                                                           GdkDragContext           *context,
                                                           gint                      x,
                                                           gint                      y,
                                                           guint                     time,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_notify_page         (GtkNotebook              *notebook,
                                                           GParamSpec               *pspec,
                                                           ThunarRenamerDialog      *renamer_dialog);
static gboolean thunar_renamer_dialog_popup_menu          (GtkWidget                *tree_view,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_row_activated       (GtkTreeView              *tree_view,
                                                           GtkTreePath              *path,
                                                           GtkTreeViewColumn        *column,
                                                           ThunarRenamerDialog      *renamer_dialog);
static void     thunar_renamer_dialog_selection_changed   (GtkTreeSelection         *selection,
                                                           ThunarRenamerDialog      *renamer_dialog);



struct _ThunarRenamerDialogClass
{
  ThunarAbstractDialogClass __parent__;
};

struct _ThunarRenamerDialog
{
  ThunarAbstractDialog __parent__;

  /* need to grab a reference on the icon factory for the standalone version */
  ThunarIconFactory   *icon_factory;

  ThunarRenamerModel  *model;

  ThunarLauncher      *launcher;

  GtkActionGroup      *action_group;
  GtkUIManager        *ui_manager;

  GtkWidget           *cancel_button;
  GtkWidget           *close_button;

  GtkWidget           *tree_view;
  GtkWidget           *progress;

  /* the current directory used for the "Add Files" dialog */
  ThunarFile          *current_directory;

  /* the list of currently selected files */
  GList               *selected_files;

  /* whether the dialog should act like a standalone application */
  gboolean             standalone;

  /* TRUE while drop highlighting */
  gboolean             drag_highlighted;
};



static const GtkActionEntry action_entries[] =
{
  { "file-menu", NULL, N_ ("_File"), NULL, },
  { "sendto-menu", NULL, N_ ("_Send To"), NULL, },
  { "file-context-menu", NULL, N_ ("File Context Menu"), NULL, },
  { "add-files", GTK_STOCK_ADD, N_ ("_Add Files..."), NULL, N_ ("Include additional files in the list of files to be renamed"), G_CALLBACK (thunar_renamer_dialog_action_add_files), },
  { "remove-files", GTK_STOCK_REMOVE, NULL, NULL, NULL, G_CALLBACK (thunar_renamer_dialog_action_remove_files), },
  { "clear", GTK_STOCK_CLEAR, N_ ("Clear"), NULL, N_ ("Clear the file list below"), G_CALLBACK (thunar_renamer_dialog_action_clear), },
  { "about", GTK_STOCK_ABOUT, N_ ("_About"), NULL, N_ ("Display information about Thunar Bulk Rename"), G_CALLBACK (thunar_renamer_dialog_action_about), },
  { "properties", GTK_STOCK_PROPERTIES, N_ ("_Properties..."), "<alt>Return", N_ ("View the properties of the selected file"), G_CALLBACK (thunar_renamer_dialog_action_properties), },
};

/* Target types for dropping to the tree view */
static const GtkTargetEntry drop_targets[] =
{
  { "GTK_TREE_MODEL_ROW", GTK_TARGET_SAME_WIDGET, TARGET_GTK_TREE_MODEL_ROW },
  { "text/uri-list", 0, TARGET_TEXT_URI_LIST }
};

/* Target types for dragging files in the tree view */
static const GtkTargetEntry drag_targets[] = {
  { "GTK_TREE_MODEL_ROW", GTK_TARGET_SAME_WIDGET, TARGET_GTK_TREE_MODEL_ROW }
};



static GObjectClass *thunar_renamer_dialog_parent_class;



GType
thunar_renamer_dialog_get_type (void)
{
  static GType type = G_TYPE_INVALID;

  if (G_UNLIKELY (type == G_TYPE_INVALID))
    {
      static const GTypeInfo info =
      {
        sizeof (ThunarRenamerDialogClass),
        NULL,
        NULL,
        (GClassInitFunc) thunar_renamer_dialog_class_init,
        NULL,
        NULL,
        sizeof (ThunarRenamerDialog),
        0,
        (GInstanceInitFunc) thunar_renamer_dialog_init,
        NULL,
      };

      type = g_type_register_static (THUNAR_TYPE_ABSTRACT_DIALOG, I_("ThunarRenamerDialog"), &info, 0);
    }

  return type;
}



static void
thunar_renamer_dialog_class_init (ThunarRenamerDialogClass *klass)
{
  GtkDialogClass *gtkdialog_class;
  GtkWidgetClass *gtkwidget_class;
  GObjectClass   *gobject_class;

  /* determine the parent type class */
  thunar_renamer_dialog_parent_class = g_type_class_peek_parent (klass);

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->dispose = thunar_renamer_dialog_dispose;
  gobject_class->finalize = thunar_renamer_dialog_finalize;
  gobject_class->get_property = thunar_renamer_dialog_get_property;
  gobject_class->set_property = thunar_renamer_dialog_set_property;

  gtkwidget_class = GTK_WIDGET_CLASS (klass);
  gtkwidget_class->realize = thunar_renamer_dialog_realize;
  gtkwidget_class->unrealize = thunar_renamer_dialog_unrealize;

  gtkdialog_class = GTK_DIALOG_CLASS (klass);
  gtkdialog_class->response = thunar_renamer_dialog_response;

  /**
   * ThunarRenamerDialog:current-directory:
   *
   * The current directory of the #ThunarRenamerDialog,
   * which is the default directory for the "Add Files"
   * dialog. May also be %NULL in which case the default
   * current directory of the file manager process will
   * be used.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_CURRENT_DIRECTORY,
                                   g_param_spec_object ("current-directory",
                                                        "current-directory",
                                                        "current-directory",
                                                        THUNAR_TYPE_FILE,
                                                        EXO_PARAM_READWRITE));

  /**
   * ThunarRenamerDialog:selected-files:
   *
   * The list of currently selected #ThunarFile<!---->s
   * in this #ThunarRenamerDialog.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_SELECTED_FILES,
                                   g_param_spec_boxed ("selected-files",
                                                       "selected-files",
                                                       "selected-files",
                                                       THUNARX_TYPE_FILE_INFO_LIST,
                                                       EXO_PARAM_READABLE));

  /**
   * ThunarRenamerDialog:standalone:
   *
   * %TRUE if the #ThunarRenamerDialog should display a
   * toolbar and add several actions which are only
   * useful in a standalone fashion.
   *
   * If the #ThunarRenamerDialog is opened from within
   * Thunar this property will always be %FALSE.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_STANDALONE,
                                   g_param_spec_boolean ("standalone",
                                                         "standalone",
                                                         "standalone",
                                                         FALSE,
                                                         EXO_PARAM_READWRITE));
}



static gint
trd_renamer_compare (gconstpointer a,
                     gconstpointer b)
{
  return g_utf8_collate (thunarx_renamer_get_name (THUNARX_RENAMER (a)), thunarx_renamer_get_name (THUNARX_RENAMER (b)));
}



static void
thunar_renamer_dialog_init (ThunarRenamerDialog *renamer_dialog)
{
  ThunarxProviderFactory *provider_factory;
  GtkTreeViewColumn      *column;
  GtkTreeSelection       *selection;
  GtkCellRenderer        *renderer;
  GtkAccelGroup          *accel_group;
  GtkSizeGroup           *size_group;
  const gchar            *active_str;
  GHashTable             *settings;
  GEnumClass             *klass;
  GtkWidget              *notebook;
  GtkWidget              *toolbar;
  GtkWidget              *button;
  GtkWidget              *mcombo;
  GtkWidget              *rcombo;
  GtkWidget              *frame;
  GtkWidget              *image;
  GtkWidget              *label;
  GtkWidget              *hbox;
  GtkWidget              *mbox;
  GtkWidget              *rbox;
  GtkWidget              *vbox;
  GtkWidget              *swin;
  XfceRc                 *rc;
  gchar                 **entries;
  GList                  *providers;
  GList                  *renamers = NULL;
  GList                  *lp;
  gint                    active = 0;
  guint                   n;

  /* allocate a new bulk rename model */
  renamer_dialog->model = thunar_renamer_model_new ();

  /* determine the list of available renamers (using the renamer providers) */
  provider_factory = thunarx_provider_factory_get_default ();
  providers = thunarx_provider_factory_list_providers (provider_factory, THUNARX_TYPE_RENAMER_PROVIDER);
  for (lp = providers; lp != NULL; lp = lp->next)
    {
      /* determine the renamers for this provider */
      renamers = g_list_concat (renamers, thunarx_renamer_provider_get_renamers (lp->data));

      /* release this provider */
      g_object_unref (G_OBJECT (lp->data));
    }
  g_object_unref (G_OBJECT (provider_factory));
  g_list_free (providers);

  /* initialize the dialog */
  gtk_dialog_set_has_separator (GTK_DIALOG (renamer_dialog), FALSE);
  gtk_window_set_default_size (GTK_WINDOW (renamer_dialog), 510, 490);
  gtk_window_set_title (GTK_WINDOW (renamer_dialog), _("Rename Multiple Files"));

  /* add the Cancel/Close buttons */
  renamer_dialog->cancel_button = gtk_dialog_add_button (GTK_DIALOG (renamer_dialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
  renamer_dialog->close_button = gtk_dialog_add_button (GTK_DIALOG (renamer_dialog), GTK_STOCK_CLOSE, GTK_RESPONSE_DELETE_EVENT);
  gtk_widget_hide (renamer_dialog->close_button);

  /* add the "Rename Files" button */
  button = gtk_dialog_add_button (GTK_DIALOG (renamer_dialog), _("_Rename Files"), GTK_RESPONSE_ACCEPT);
  exo_binding_new (G_OBJECT (renamer_dialog->model), "can-rename", G_OBJECT (button), "sensitive");
  gtk_dialog_set_default_response (GTK_DIALOG (renamer_dialog), GTK_RESPONSE_ACCEPT);
  thunar_gtk_widget_set_tooltip (button, _("Click here to actually rename the files listed above to their new names."));

  /* setup the action group for this dialog */
  renamer_dialog->action_group = gtk_action_group_new ("ThunarRenamerDialog");
  gtk_action_group_set_translation_domain (renamer_dialog->action_group, GETTEXT_PACKAGE);
  gtk_action_group_add_actions (renamer_dialog->action_group, action_entries, G_N_ELEMENTS (action_entries), GTK_WIDGET (renamer_dialog));

  /* setup the UI manager for this dialog */
  renamer_dialog->ui_manager = gtk_ui_manager_new ();
  gtk_ui_manager_insert_action_group (renamer_dialog->ui_manager, renamer_dialog->action_group, 0);
  gtk_ui_manager_add_ui_from_string (renamer_dialog->ui_manager, thunar_renamer_dialog_ui, thunar_renamer_dialog_ui_length, NULL);

  /* connect the accelerators */
  accel_group = gtk_ui_manager_get_accel_group (renamer_dialog->ui_manager);
  gtk_window_add_accel_group (GTK_WINDOW (renamer_dialog), accel_group);

  /* setup the launcher support for this dialog */
  renamer_dialog->launcher = thunar_launcher_new ();
  thunar_launcher_set_widget (renamer_dialog->launcher, GTK_WIDGET (renamer_dialog));
  thunar_component_set_ui_manager (THUNAR_COMPONENT (renamer_dialog->launcher), renamer_dialog->ui_manager);
  exo_binding_new (G_OBJECT (renamer_dialog), "selected-files", G_OBJECT (renamer_dialog->launcher), "selected-files");

  /* add the toolbar to the dialog */
  toolbar = gtk_ui_manager_get_widget (renamer_dialog->ui_manager, "/toolbar");
  exo_binding_new (G_OBJECT (renamer_dialog), "standalone", G_OBJECT (toolbar), "visible");
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (renamer_dialog)->vbox), toolbar, FALSE, FALSE, 0);

  /* create the main vbox */
  mbox = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (mbox), 6);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (renamer_dialog)->vbox), mbox, TRUE, TRUE, 0);
  gtk_widget_show (mbox);

  /* create the scrolled window for the tree view */
  swin = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (swin), GTK_SHADOW_ETCHED_IN);
  gtk_box_pack_start (GTK_BOX (mbox), swin, TRUE, TRUE, 0);
  gtk_widget_show (swin);

  /* create the tree view */
  renamer_dialog->tree_view = exo_tree_view_new ();
  gtk_tree_view_set_model (GTK_TREE_VIEW (renamer_dialog->tree_view), GTK_TREE_MODEL (renamer_dialog->model));
  gtk_tree_view_set_search_column (GTK_TREE_VIEW (renamer_dialog->tree_view), THUNAR_RENAMER_MODEL_COLUMN_OLDNAME);
  gtk_tree_view_set_fixed_height_mode (GTK_TREE_VIEW (renamer_dialog->tree_view), TRUE);
  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (renamer_dialog->tree_view), TRUE);
  gtk_tree_view_set_rubber_banding (GTK_TREE_VIEW (renamer_dialog->tree_view), TRUE);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (renamer_dialog->tree_view), TRUE);
  g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "button-press-event", G_CALLBACK (thunar_renamer_dialog_button_press_event), renamer_dialog);
  g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "popup-menu", G_CALLBACK (thunar_renamer_dialog_popup_menu), renamer_dialog);
  g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "row-activated", G_CALLBACK (thunar_renamer_dialog_row_activated), renamer_dialog);
  gtk_container_add (GTK_CONTAINER (swin), renamer_dialog->tree_view);
  gtk_widget_show (renamer_dialog->tree_view);

  /* create the tree view column for the old file name */
  column = gtk_tree_view_column_new ();
  gtk_tree_view_column_set_spacing (column, 2);
  gtk_tree_view_column_set_min_width (column, 100);
  gtk_tree_view_column_set_title (column, _("Name"));
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_column_set_fixed_width (column, 250);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  renderer = g_object_new (THUNAR_TYPE_ICON_RENDERER, "size", 16, NULL);
  gtk_tree_view_column_pack_start (column, renderer, FALSE);
  gtk_tree_view_column_set_attributes (column, renderer, "file", THUNAR_RENAMER_MODEL_COLUMN_FILE, NULL);
  renderer = g_object_new (GTK_TYPE_CELL_RENDERER_TEXT, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, NULL);
  gtk_tree_view_column_pack_start (column, renderer, TRUE);
  gtk_tree_view_column_set_attributes (column, renderer, "text", THUNAR_RENAMER_MODEL_COLUMN_OLDNAME, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (renamer_dialog->tree_view), column);

  /* create the tree view column for the new file name */
  column = gtk_tree_view_column_new ();
  gtk_tree_view_column_set_min_width (column, 75);
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_column_set_title (column, _("New Name"));
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  renderer = g_object_new (GTK_TYPE_CELL_RENDERER_TEXT,
                           "ellipsize", PANGO_ELLIPSIZE_END,
                           "ellipsize-set", TRUE,
                           "foreground", "Red",
                           "weight", PANGO_WEIGHT_SEMIBOLD,
                           NULL);
  gtk_tree_view_column_pack_start (column, renderer, TRUE);
  gtk_tree_view_column_set_attributes (column, renderer,
                                       "foreground-set", THUNAR_RENAMER_MODEL_COLUMN_CONFLICT,
                                       "text", THUNAR_RENAMER_MODEL_COLUMN_NEWNAME,
                                       "weight-set", THUNAR_RENAMER_MODEL_COLUMN_CONFLICT,
                                       NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (renamer_dialog->tree_view), column);

  /* create the vbox for the renamer parameters */
  vbox = gtk_vbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (mbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show (vbox);

  /* create the hbox for the renamer selection */
  rbox = gtk_hbox_new (FALSE, 3);
  gtk_box_pack_start (GTK_BOX (vbox), rbox, FALSE, FALSE, 0);
  gtk_widget_show (rbox);

  /* create the rename progress bar */
  renamer_dialog->progress = thunar_renamer_progress_new ();
  gtk_box_pack_start (GTK_BOX (vbox), renamer_dialog->progress, FALSE, FALSE, 0);
  exo_binding_new_with_negation (G_OBJECT (renamer_dialog->progress), "visible", G_OBJECT (rbox), "visible");
  exo_binding_new_with_negation (G_OBJECT (renamer_dialog->progress), "visible", G_OBJECT (swin), "sensitive");
  exo_binding_new_with_negation (G_OBJECT (renamer_dialog->progress), "visible", G_OBJECT (toolbar), "sensitive");
  exo_binding_new (G_OBJECT (renamer_dialog->progress), "visible", G_OBJECT (renamer_dialog->model), "frozen");

  /* synchronize the height of the progress bar and the rbox with the combos */
  size_group = gtk_size_group_new (GTK_SIZE_GROUP_VERTICAL);
  gtk_size_group_add_widget (size_group, renamer_dialog->progress);
  gtk_size_group_add_widget (size_group, rbox);
  g_object_unref (G_OBJECT (size_group));

  /* create the frame for the renamer widgets */
  frame = g_object_new (GTK_TYPE_FRAME, "shadow-type", GTK_SHADOW_ETCHED_IN, NULL);
  exo_binding_new_with_negation (G_OBJECT (renamer_dialog->progress), "visible", G_OBJECT (frame), "sensitive");
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  /* check if we have any renamers */
  if (G_LIKELY (renamers != NULL))
    {
      /* sort the renamers by name */
      renamers = g_list_sort (renamers, trd_renamer_compare);

      /* open the config file, xfce_rc_config_open() should never return NULL */
      rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "Thunar/renamerrc", TRUE);
      xfce_rc_set_group (rc, "Configuration");

      /* create the renamer combo box for the renamer selection */
      rcombo = gtk_combo_box_new_text ();
      for (lp = renamers; lp != NULL; lp = lp->next)
        gtk_combo_box_append_text (GTK_COMBO_BOX (rcombo), thunarx_renamer_get_name (lp->data));
      gtk_box_pack_start (GTK_BOX (rbox), rcombo, FALSE, FALSE, 0);
      gtk_widget_show (rcombo);

      /* add a "Help" button */
      button = gtk_button_new ();
      thunar_gtk_widget_set_tooltip (button, _("Click here to view the documentation for the selected rename operation."));
      g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK (thunar_renamer_dialog_help), renamer_dialog);
      gtk_box_pack_start (GTK_BOX (rbox), button, FALSE, FALSE, 0);
      gtk_widget_show (button);

      image = gtk_image_new_from_stock (GTK_STOCK_HELP, GTK_ICON_SIZE_BUTTON);
      gtk_container_add (GTK_CONTAINER (button), image);
      gtk_widget_show (image);

      /* create the name/suffix/both combo box */
      mcombo = gtk_combo_box_new_text ();
      klass = g_type_class_ref (THUNAR_TYPE_RENAMER_MODE);
      active_str = xfce_rc_read_entry_untranslated (rc, "LastActiveMode", "");
      for (active = 0, n = 0; n < klass->n_values; ++n)
        {
          if (exo_str_is_equal (active_str, klass->values[n].value_name))
            active = n;
          gtk_combo_box_append_text (GTK_COMBO_BOX (mcombo), _(klass->values[n].value_nick));
        }
      exo_mutual_binding_new (G_OBJECT (renamer_dialog->model), "mode", G_OBJECT (mcombo), "active");
      gtk_box_pack_end (GTK_BOX (rbox), mcombo, FALSE, FALSE, 0);
      gtk_combo_box_set_active (GTK_COMBO_BOX (mcombo), active);
      g_type_class_unref (klass);
      gtk_widget_show (mcombo);

      /* allocate the notebook with the renamers */
      notebook = gtk_notebook_new ();
      gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), FALSE);
      gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook), FALSE);
      g_signal_connect (G_OBJECT (notebook), "notify::page", G_CALLBACK (thunar_renamer_dialog_notify_page), renamer_dialog);
      gtk_container_add (GTK_CONTAINER (frame), notebook);
      gtk_widget_show (notebook);

      /* determine the last active renamer (as string) */
      active_str = xfce_rc_read_entry_untranslated (rc, "LastActiveRenamer", "");

      /* add the renamers to the notebook */
      for (active = 0, lp = renamers; lp != NULL; lp = lp->next)
        {
          /* append the renamer to the notebook... */
          gtk_notebook_append_page (GTK_NOTEBOOK (notebook), lp->data, NULL);

          /* check if this page should be active by default */
          if (exo_str_is_equal (G_OBJECT_TYPE_NAME (lp->data), active_str))
            active = g_list_position (renamers, lp);

          /* try to load the settings for the renamer */
          entries = xfce_rc_get_entries (rc, G_OBJECT_TYPE_NAME (lp->data));
          if (G_LIKELY (entries != NULL))
            {
              /* add the settings to a hash table */
              xfce_rc_set_group (rc, G_OBJECT_TYPE_NAME (lp->data));
              settings = g_hash_table_new (g_str_hash, g_str_equal);
              for (n = 0; entries[n] != NULL; ++n)
                g_hash_table_insert (settings, entries[n], (gchar *) xfce_rc_read_entry_untranslated (rc, entries[n], NULL));

              /* let the renamer load the settings */
              thunarx_renamer_load (THUNARX_RENAMER (lp->data), settings);

              /* cleanup */
              g_hash_table_destroy (settings);
              g_strfreev (entries);
            }

          /* ...and make sure its visible */
          gtk_widget_show (lp->data);
        }

      /* default to the first renamer */
      gtk_combo_box_set_active (GTK_COMBO_BOX (rcombo), active);

      /* connect the combo box to the notebook */
      exo_binding_new (G_OBJECT (rcombo), "active", G_OBJECT (notebook), "page");

      /* close the config handle */
      xfce_rc_close (rc);

      /* setup drop support for the tree view */
      gtk_drag_dest_set (renamer_dialog->tree_view, 0, drop_targets, G_N_ELEMENTS (drop_targets), GDK_ACTION_COPY | GDK_ACTION_MOVE);
      gtk_drag_source_set (renamer_dialog->tree_view, GDK_BUTTON1_MASK, drag_targets, G_N_ELEMENTS (drag_targets), GDK_ACTION_MOVE);
      g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "drag-data-received", G_CALLBACK (thunar_renamer_dialog_drag_data_received), renamer_dialog);
      g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "drag-leave", G_CALLBACK (thunar_renamer_dialog_drag_leave), renamer_dialog);
      g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "drag-motion", G_CALLBACK (thunar_renamer_dialog_drag_motion), renamer_dialog);
      g_signal_connect (G_OBJECT (renamer_dialog->tree_view), "drag-drop", G_CALLBACK (thunar_renamer_dialog_drag_drop), renamer_dialog);

      /* setup the tree selection */
      selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (renamer_dialog->tree_view));
      gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);
      g_signal_connect (G_OBJECT (selection), "changed", G_CALLBACK (thunar_renamer_dialog_selection_changed), renamer_dialog);
      thunar_renamer_dialog_selection_changed (selection, renamer_dialog);
    }
  else
    {
      /* make the other widgets insensitive */
      gtk_widget_set_sensitive (toolbar, FALSE);
      gtk_widget_set_sensitive (swin, FALSE);

      /* display an error to the user */
      hbox = gtk_hbox_new (FALSE, 12);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 12);
      gtk_container_add (GTK_CONTAINER (frame), hbox);
      gtk_widget_show (hbox);

      image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_ERROR, GTK_ICON_SIZE_DIALOG);
      gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
      gtk_widget_show (image);

      /* TRANSLATORS: You can test this string by temporarily removing thunar-sbr.* from $libdir/thunarx-1/,
       *              and opening the multi rename dialog by selecting multiple files and pressing F2.
       */
      label = gtk_label_new (_("No renamer modules were found on your system. Please check your\n"
                               "installation or contact your system administrator. If you install Thunar\n"
                               "from source, be sure to enable the \"Simple Builtin Renamers\" plugin."));
      gtk_misc_set_alignment (GTK_MISC (label), 0.0f, 0.5f);
      gtk_label_set_selectable (GTK_LABEL (label), TRUE);
      gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
      gtk_widget_show (label);
    }

  /* release the renamers list */
  g_list_foreach (renamers, (GFunc) g_object_unref, NULL);
  g_list_free (renamers);
}



static void
thunar_renamer_dialog_dispose (GObject *object)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (object);

  /* reset the "current-directory" property */
  thunar_renamer_dialog_set_current_directory (renamer_dialog, NULL);

  /* reset the widget for the launcher support */
  thunar_launcher_set_widget (renamer_dialog->launcher, NULL);

  (*G_OBJECT_CLASS (thunar_renamer_dialog_parent_class)->dispose) (object);
}



static void
thunar_renamer_dialog_finalize (GObject *object)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (object);

  /* release the launcher support */
  g_object_unref (G_OBJECT (renamer_dialog->launcher));

  /* release the action group and the ui manager */
  g_object_unref (G_OBJECT (renamer_dialog->action_group));
  g_object_unref (G_OBJECT (renamer_dialog->ui_manager));

  /* release our bulk rename model */
  g_object_unref (G_OBJECT (renamer_dialog->model));

  /* release the list of selected files */
  thunarx_file_info_list_free (renamer_dialog->selected_files);

  (*G_OBJECT_CLASS (thunar_renamer_dialog_parent_class)->finalize) (object);
}



static void
thunar_renamer_dialog_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (object);

  switch (prop_id)
    {
    case PROP_CURRENT_DIRECTORY:
      g_value_set_object (value, thunar_renamer_dialog_get_current_directory (renamer_dialog));
      break;

    case PROP_SELECTED_FILES:
      g_value_set_boxed (value, thunar_renamer_dialog_get_selected_files (renamer_dialog));
      break;

    case PROP_STANDALONE:
      g_value_set_boolean (value, thunar_renamer_dialog_get_standalone (renamer_dialog));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
thunar_renamer_dialog_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (object);

  switch (prop_id)
    {
    case PROP_CURRENT_DIRECTORY:
      thunar_renamer_dialog_set_current_directory (renamer_dialog, g_value_get_object (value));
      break;

    case PROP_STANDALONE:
      thunar_renamer_dialog_set_standalone (renamer_dialog, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
thunar_renamer_dialog_realize (GtkWidget *widget)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (widget);
  GtkIconTheme        *icon_theme;

  /* realize the widget */
  (*GTK_WIDGET_CLASS (thunar_renamer_dialog_parent_class)->realize) (widget);

  /* grab a reference on the icon factory for the screen */
  icon_theme = gtk_icon_theme_get_for_screen (gtk_widget_get_screen (widget));
  renamer_dialog->icon_factory = thunar_icon_factory_get_for_icon_theme (icon_theme);
}



static void
thunar_renamer_dialog_unrealize (GtkWidget *widget)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (widget);

  /* release the reference on the icon factory */
  g_object_unref (G_OBJECT (renamer_dialog->icon_factory));
  renamer_dialog->icon_factory = NULL;

  /* unrealize the dialog */
  (*GTK_WIDGET_CLASS (thunar_renamer_dialog_parent_class)->unrealize) (widget);
}



static void
thunar_renamer_dialog_response (GtkDialog *dialog,
                                gint       response)
{
  ThunarRenamerDialog *renamer_dialog = THUNAR_RENAMER_DIALOG (dialog);
  GtkTreeIter          iter;
  ThunarFile          *file;
  GList               *pair_list = NULL;
  gchar               *name;

  /* grab an additional reference on the dialog */
  g_object_ref (G_OBJECT (dialog));

  /* check if we should rename */
  if (G_LIKELY (response == GTK_RESPONSE_ACCEPT))
    {
      /* hide the close button and show the cancel button */
      gtk_widget_show (renamer_dialog->cancel_button);
      gtk_widget_hide (renamer_dialog->close_button);

      /* save the current settings */
      thunar_renamer_dialog_save (renamer_dialog);

      /* display the rename progress bar */
      gtk_widget_show (renamer_dialog->progress);

      /* determine the iterator for the first item */
      if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (renamer_dialog->model), &iter))
        {
          /* determine the tree row references for the rows that should be renamed */
          do
            {
              /* determine the file and newname for the row */
              gtk_tree_model_get (GTK_TREE_MODEL (renamer_dialog->model), &iter,
                                  THUNAR_RENAMER_MODEL_COLUMN_NEWNAME, &name,
                                  THUNAR_RENAMER_MODEL_COLUMN_FILE, &file,
                                  -1);

              /* check if this row should be renamed */
              if (G_LIKELY (name != NULL && *name != '\0'))
                pair_list = g_list_append (pair_list, thunar_renamer_pair_new (file, name));

              /* cleanup */
              g_object_unref (file);
              g_free (name);
            }
          while (gtk_tree_model_iter_next (GTK_TREE_MODEL (renamer_dialog->model), &iter));
        }

      /* perform the rename (returns when done) */
      thunar_renamer_progress_run (THUNAR_RENAMER_PROGRESS (renamer_dialog->progress), pair_list);

      /* hide the rename progress bar */
      gtk_widget_hide (renamer_dialog->progress);

      /* release the pairs */
      thunar_renamer_pair_list_free (pair_list);

      /* check if we're in standalone mode */
      if (thunar_renamer_dialog_get_standalone (renamer_dialog))
        {
          /* hide the cancel button again and display the close button */
          gtk_widget_hide (renamer_dialog->cancel_button);
          gtk_widget_show (renamer_dialog->close_button);
        }
      else
        {
          /* hide and destroy the dialog window */
          gtk_widget_hide (GTK_WIDGET (dialog));
          gtk_widget_destroy (GTK_WIDGET (dialog));
        }
    }
  else if (thunar_renamer_progress_running (THUNAR_RENAMER_PROGRESS (renamer_dialog->progress)))
    {
      /* cancel the rename operation */
      thunar_renamer_progress_cancel (THUNAR_RENAMER_PROGRESS (renamer_dialog->progress));
    }
  else
    {
      /* hide and destroy the dialog window */
      gtk_widget_hide (GTK_WIDGET (dialog));
      gtk_widget_destroy (GTK_WIDGET (dialog));
    }

  /* release the additional reference on the dialog */
  g_object_unref (G_OBJECT (dialog));
}



static void
thunar_renamer_dialog_context_menu (ThunarRenamerDialog *renamer_dialog,
                                    guint                button,
                                    guint32              time)
{
  GtkActionGroup *renamer_actions = NULL;
  ThunarxRenamer *renamer;
  GtkWidget      *menu;
  GList          *actions = NULL;
  GList          *lp;
  gint            renamer_merge_id = 0;

  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* grab an additional reference on the dialog */
  g_object_ref (G_OBJECT (renamer_dialog));

  /* determine the active renamer */
  renamer = thunar_renamer_model_get_renamer (renamer_dialog->model);
  if (G_LIKELY (renamer != NULL))
    {
      /* determine the actions provided by the active renamer */
      actions = thunarx_renamer_get_actions (renamer, GTK_WINDOW (renamer_dialog), renamer_dialog->selected_files);
    }

  /* check if we have any renamer actions */
  if (G_UNLIKELY (actions != NULL))
    {
      /* allocate a new action group and the merge id for the custom actions */
      renamer_actions = gtk_action_group_new ("thunar-renamer-dialog-renamer-actions");
      renamer_merge_id = gtk_ui_manager_new_merge_id (renamer_dialog->ui_manager);
      gtk_ui_manager_insert_action_group (renamer_dialog->ui_manager, renamer_actions, -1);

      /* add the actions to the UI manager */
      for (lp = actions; lp != NULL; lp = lp->next)
        {
          /* add the action to the action group */
          gtk_action_group_add_action (renamer_actions, GTK_ACTION (lp->data));

          /* add the action to the UI manager */
          gtk_ui_manager_add_ui (renamer_dialog->ui_manager, renamer_merge_id,
                                 "/file-context-menu/placeholder-renamer-actions",
                                 gtk_action_get_name (GTK_ACTION (lp->data)),
                                 gtk_action_get_name (GTK_ACTION (lp->data)),
                                 GTK_UI_MANAGER_MENUITEM, FALSE);

          /* release the reference on the action */
          g_object_unref (G_OBJECT (lp->data));
        }

      /* be sure to update the UI manager to avoid flickering */
      gtk_ui_manager_ensure_update (renamer_dialog->ui_manager);

      /* cleanup */
      g_list_free (actions);
    }

  /* run the menu on the dialog's screen */
  menu = gtk_ui_manager_get_widget (renamer_dialog->ui_manager, "/file-context-menu");
  thunar_gtk_menu_run (GTK_MENU (menu), GTK_WIDGET (renamer_dialog), NULL, NULL, button, time);

  /* remove the previously merge items from the UI manager */
  if (G_UNLIKELY (renamer_merge_id != 0))
    {
      gtk_ui_manager_remove_ui (renamer_dialog->ui_manager, renamer_merge_id);
      gtk_ui_manager_ensure_update (renamer_dialog->ui_manager);
    }

  /* disconnect the previously created action group */
  if (G_UNLIKELY (renamer_actions != NULL))
    {
      gtk_ui_manager_remove_action_group (renamer_dialog->ui_manager, renamer_actions);
      g_object_unref (G_OBJECT (renamer_actions));
    }

  /* release the additional reference on the dialog */
  g_object_unref (G_OBJECT (renamer_dialog));
}



static void
thunar_renamer_dialog_help (ThunarRenamerDialog *renamer_dialog)
{
  ThunarxRenamer *renamer;
  const gchar    *help_url = NULL;
  GdkScreen      *screen;
  GError         *error = NULL;

  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* check if the active renamer provides a help URL */
  renamer = thunar_renamer_model_get_renamer (renamer_dialog->model);
  if (G_LIKELY (renamer != NULL))
    help_url = thunarx_renamer_get_help_url (renamer);

  /* check if we have a specific URL */
  if (G_LIKELY (help_url == NULL))
    {
      /* open the general documentation if no specific URL */
      thunar_dialogs_show_help (GTK_WIDGET (renamer_dialog), "advanced-topics", "to-bulk-rename-files");
    }
  else
    {
      /* determine the dialog's screen */
      screen = gtk_widget_get_screen (GTK_WIDGET (renamer_dialog));

      /* try to launch the specific URL */
      if (!exo_url_show_on_screen (help_url, NULL, screen, &error))
        {
          /* tell the user that we failed */
          thunar_dialogs_show_error (GTK_WIDGET (renamer_dialog), error, _("Failed to open the documentation browser"));

          /* release the error */
          g_error_free (error);
        }
    }
}



static void
trd_save_foreach (gpointer key,
                  gpointer value,
                  gpointer user_data)
{
  xfce_rc_write_entry (user_data, key, value);
}



static void
thunar_renamer_dialog_save (ThunarRenamerDialog *renamer_dialog)
{
  ThunarxRenamer *renamer;
  GHashTable     *settings;
  GEnumClass     *klass;
  GEnumValue     *value;
  XfceRc         *rc;

  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* try to open the config handle for writing */
  rc = xfce_rc_config_open (XFCE_RESOURCE_CONFIG, "Thunar/renamerrc", FALSE);
  if (G_UNLIKELY (rc == NULL))
    return;

  /* determine the currently active renamer */
  renamer = thunar_renamer_model_get_renamer (renamer_dialog->model);
  if (G_LIKELY (renamer != NULL))
    {
      /* save general settings first */
      xfce_rc_set_group (rc, "Configuration");

      /* remember current mode as last active */
      klass = g_type_class_ref (THUNAR_TYPE_RENAMER_MODE);
      value = g_enum_get_value (klass, thunar_renamer_model_get_mode (renamer_dialog->model));
      if (G_LIKELY (value != NULL))
        xfce_rc_write_entry (rc, "LastActiveMode", value->value_name);
      g_type_class_unref (klass);

      /* remember renamer as last active */
      xfce_rc_write_entry (rc, "LastActiveRenamer", G_OBJECT_TYPE_NAME (renamer));

      /* save the renamer's settings */
      settings = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
      xfce_rc_delete_group (rc, G_OBJECT_TYPE_NAME (renamer), TRUE);
      xfce_rc_set_group (rc, G_OBJECT_TYPE_NAME (renamer));
      thunarx_renamer_save (renamer, settings);
      g_hash_table_foreach (settings, trd_save_foreach, rc);
      g_hash_table_destroy (settings);
    }

  /* close the config handle */
  xfce_rc_close (rc);
}



static gboolean
trd_audio_filter_func (const GtkFileFilterInfo *info,
                       gpointer                 data)
{
  /* check if the MIME type is an audio MIME type */
  return (strncmp (info->mime_type, "audio/", 6) == 0);
}



static gboolean
trd_video_filter_func (const GtkFileFilterInfo *info,
                       gpointer                 data)
{
  /* check if the MIME type is a video MIME type */
  return (strncmp (info->mime_type, "video/", 6) == 0);
}



static void
thunar_renamer_dialog_action_add_files (GtkAction           *action,
                                        ThunarRenamerDialog *renamer_dialog)
{
  GtkFileFilter *filter;
  ThunarFile    *file;
  GtkWidget     *chooser;
  GSList        *uris;
  GSList        *lp;
  gchar         *uri;

  _thunar_return_if_fail (GTK_IS_ACTION (action));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* allocate the file chooser */
  chooser = gtk_file_chooser_dialog_new (_("Select files to rename"),
                                         GTK_WINDOW (renamer_dialog),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                         NULL);
  gtk_file_chooser_set_local_only (GTK_FILE_CHOOSER (chooser), TRUE);
  gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (chooser), TRUE);

  /* add file chooser filers */
  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter, _("All Files"));
  gtk_file_filter_add_pattern (filter, "*");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (chooser), filter);

  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter, _("Audio Files"));
  gtk_file_filter_add_custom (filter, GTK_FILE_FILTER_MIME_TYPE, trd_audio_filter_func, NULL, NULL);
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (chooser), filter);

  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter, _("Image Files"));
  gtk_file_filter_add_pixbuf_formats (filter);
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (chooser), filter);

  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter, _("Video Files"));
  gtk_file_filter_add_custom (filter, GTK_FILE_FILTER_MIME_TYPE, trd_video_filter_func, NULL, NULL);
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (chooser), filter);

  /* check if "current-directory" is set */
  if (G_LIKELY (renamer_dialog->current_directory != NULL))
    {
      /* use the "current-directory" as default for the chooser */
      uri = thunarx_file_info_get_uri (THUNARX_FILE_INFO (renamer_dialog->current_directory));
      gtk_file_chooser_set_current_folder_uri (GTK_FILE_CHOOSER (chooser), uri);
      g_free (uri);
    }

  /* run the chooser dialog */
  if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_ACCEPT)
    {
      /* determine the URIs of the selected files */
      uris = gtk_file_chooser_get_uris (GTK_FILE_CHOOSER (chooser));
      for (lp = uris; lp != NULL; lp = lp->next)
        {
          /* determine the file for the URI */
          file = thunar_file_get_for_uri (lp->data, NULL);
          if (G_LIKELY (file != NULL))
            {
              /* append the file to the renamer model */
              thunar_renamer_model_append (renamer_dialog->model, file);

              /* release the file */
              g_object_unref (G_OBJECT (file));
            }

          /* release the URI */
          g_free (lp->data);
        }
      g_slist_free (uris);

      /* determine the current folder of the chooser */
      uri = gtk_file_chooser_get_current_folder_uri (GTK_FILE_CHOOSER (chooser));
      if (G_LIKELY (uri != NULL))
        {
          /* determine the file for that URI */
          file = thunar_file_get_for_uri (uri, NULL);
          if (G_LIKELY (file != NULL))
            {
              /* remember the current folder of the chooser as default */
              thunar_renamer_dialog_set_current_directory (renamer_dialog, file);

              /* release the file */
              g_object_unref (G_OBJECT (file));
            }

          /* release the URI */
          g_free (uri);
        }
    }

  /* cleanup */
  gtk_widget_destroy (chooser);
}



static void
thunar_renamer_dialog_action_remove_files (GtkAction           *action,
                                           ThunarRenamerDialog *renamer_dialog)
{
  GtkTreeRowReference *row;
  GtkTreeSelection    *selection;
  GtkTreeModel        *model;
  GtkTreePath         *path;
  GList               *rows;
  GList               *lp;

  _thunar_return_if_fail (GTK_IS_ACTION (action));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* determine the selected rows in the view */
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (renamer_dialog->tree_view));
  rows = gtk_tree_selection_get_selected_rows (selection, &model);
  for (lp = rows; lp != NULL; lp = lp->next)
    {
      /* turn the path into a tree row reference */
      row = gtk_tree_row_reference_new (model, lp->data);
      gtk_tree_path_free (lp->data);
      lp->data = row;
    }

  /* actually remove the rows */
  for (lp = rows; lp != NULL; lp = lp->next)
    {
      /* determine the path for the row */
      path = gtk_tree_row_reference_get_path (lp->data);
      if (G_LIKELY (path != NULL))
        {
          /* remove the row from the model */
          thunar_renamer_model_remove (THUNAR_RENAMER_MODEL (model), path);
          gtk_tree_path_free (path);
        }

      /* release the row reference */
      gtk_tree_row_reference_free (lp->data);
    }

  /* cleanup */
  g_list_free (rows);
}



static void
thunar_renamer_dialog_action_clear (GtkAction           *action,
                                    ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_if_fail (GTK_IS_ACTION (action));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* just clear the list of files in the model */
  thunar_renamer_model_clear (renamer_dialog->model);
}



static void
thunar_renamer_dialog_action_about (GtkAction           *action,
                                    ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_if_fail (GTK_IS_ACTION (action));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* just popup the about dialog */
  thunar_dialogs_show_about (GTK_WINDOW (renamer_dialog), _("Bulk Rename"),
                             _("Thunar Bulk Rename is a powerful and extensible\n"
                               "tool to rename multiple files at once."));
}



static void
thunar_renamer_dialog_action_properties (GtkAction           *action,
                                         ThunarRenamerDialog *renamer_dialog)
{
  GtkWidget *dialog;

  _thunar_return_if_fail (GTK_IS_ACTION (action));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* verify that we have exactly one file selected */
  if (G_LIKELY (g_list_length (renamer_dialog->selected_files) == 1))
    {
      /* popup the properties dialog */
      dialog = thunar_properties_dialog_new ();
      gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
      gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
      gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (renamer_dialog));
      thunar_properties_dialog_set_file (THUNAR_PROPERTIES_DIALOG (dialog), renamer_dialog->selected_files->data);
      gtk_widget_show (dialog);
    }
}



static gboolean
thunar_renamer_dialog_button_press_event (GtkWidget           *tree_view,
                                          GdkEventButton      *event,
                                          ThunarRenamerDialog *renamer_dialog)
{
  GtkTreeSelection *selection;
  GtkTreePath      *path;

  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), FALSE);
  _thunar_return_val_if_fail (GTK_IS_TREE_VIEW (tree_view), FALSE);

  /* determine the tree selection */
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));

  /* we unselect all selected items if the user clicks on an empty
   * area of the treeview and no modifier key is active.
   */
  if ((event->state & gtk_accelerator_get_default_mod_mask ()) == 0
      && !gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (tree_view), event->x, event->y, NULL, NULL, NULL, NULL))
    {
      /* just unselect all rows */
      gtk_tree_selection_unselect_all (selection);
    }

  /* check if we should popup the context menu */
  if (event->window == gtk_tree_view_get_bin_window (GTK_TREE_VIEW (tree_view)) && event->type == GDK_BUTTON_PRESS && event->button == 3)
    {
      /* determine the path at the event coordinates */
      if (gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (tree_view), event->x, event->y, &path, NULL, NULL, NULL))
        {
          /* select the path on which the user clicked if not already selected */
          if (!gtk_tree_selection_path_is_selected (selection, path))
            {
              /* we don't unselect all other items if Control is active */
              if ((event->state & GDK_CONTROL_MASK) == 0)
                gtk_tree_selection_unselect_all (selection);
              gtk_tree_selection_select_path (selection, path);
            }

          /* release the tree path */
          gtk_tree_path_free (path);
        }

      /* popup the context menu */
      thunar_renamer_dialog_context_menu (renamer_dialog, event->button, event->time);

      /* we handled the event */
      return TRUE;
    }

  return FALSE;
}



static void
thunar_renamer_dialog_drag_data_received (GtkWidget           *tree_view,
                                          GdkDragContext      *context,
                                          gint                 x,
                                          gint                 y,
                                          GtkSelectionData    *selection_data,
                                          guint                info,
                                          guint                time,
                                          ThunarRenamerDialog *renamer_dialog)
{
  ThunarFile              *file;
  GList                   *path_list;
  GList                   *lp;
  GtkTreeModel            *model;
  GtkTreePath             *path;
  GtkTreeViewDropPosition  drop_pos;
  gint                     position = -1;

  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));
  _thunar_return_if_fail (GTK_IS_TREE_VIEW (tree_view));

  /* we only accept text/uri-list drops with format 8 and atleast one byte of data */
  if (info == TARGET_TEXT_URI_LIST && selection_data->format == 8 && selection_data->length > 0)
    {
      /* determine the renamer model */
      model = gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));

      /* get the drop position, if no path is foud we append the files */
      if (gtk_tree_view_get_dest_row_at_pos (GTK_TREE_VIEW (tree_view), x, y, &path, &drop_pos))
        {
          /* get the position of the path */
          position = gtk_tree_path_get_indices (path)[0];
          gtk_tree_path_free (path);

          /* advance the offset if we drop after the item */
          if (drop_pos == GTK_TREE_VIEW_DROP_AFTER || drop_pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER)
            position++;

          /* append if we've reached the end of the model */
          if (position >= gtk_tree_model_iter_n_children (model, NULL))
            position = -1;
        }

      /* determine the path list from the selection_data */
      path_list = thunar_vfs_path_list_from_string ((const gchar *) selection_data->data, NULL);

      /* add all paths to the model */
      for (lp = path_list; lp != NULL; lp = lp->next)
        {
          /* determine the file for the path */
          file = thunar_file_get_for_path (lp->data, NULL);
          if (G_LIKELY (file != NULL))
            {
              /* insert the file in the model */
              thunar_renamer_model_insert (renamer_dialog->model, file, position);

              /* advance the offset if we do not append, so the drop order is consistent */
              if (position != -1)
                position++;

              /* release the file */
              g_object_unref (G_OBJECT (file));
            }

          /* release the path */
          thunar_vfs_path_unref (lp->data);
        }

      /* finish the drag */
      gtk_drag_finish (context, (path_list != NULL), FALSE, time);

      /* release the list */
      g_list_free (path_list);
    }

  /* stop the emission of the "drag-data-received" signal */
  g_signal_stop_emission_by_name (G_OBJECT (tree_view), "drag-data-received");
}



static void
thunar_renamer_dialog_drag_leave (GtkWidget           *tree_view,
                                  GdkDragContext      *context,
                                  guint                time,
                                  ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));
  _thunar_return_if_fail (GTK_IS_TREE_VIEW (tree_view));

  /* disable the highlighting */
  if (renamer_dialog->drag_highlighted)
    {
      /* we use the tree view parent (the scrolled window),
       * as the drag_highlight doesn't work for tree views.
       */
      gtk_drag_unhighlight (tree_view->parent);
      renamer_dialog->drag_highlighted = FALSE;
    }
}



static gboolean
thunar_renamer_dialog_drag_motion (GtkWidget           *tree_view,
                                   GdkDragContext      *context,
                                   gint                 x,
                                   gint                 y,
                                   guint                time,
                                   ThunarRenamerDialog *renamer_dialog)
{
  GdkAtom                  target;
  GtkTreeViewDropPosition  position;
  GtkTreePath             *path;
  GdkDragAction            action;
  GtkTreeModel            *model;
  gint                     n_rows;

  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), FALSE);
  _thunar_return_val_if_fail (GTK_IS_TREE_VIEW (tree_view), FALSE);

  /* determine the drop target */
  target = gtk_drag_dest_find_target (tree_view, context, NULL);
  if (target == gdk_atom_intern_static_string ("text/uri-list"))
    {
      /* set the drag action for uris */
      action = GDK_ACTION_COPY;
    }
  else if (target == gdk_atom_intern_static_string ("GTK_TREE_MODEL_ROW"))
    {
      /* set the drag action for tree rows */
      action = GDK_ACTION_MOVE;
    }
  else
    {
      if (renamer_dialog->drag_highlighted)
        {
          /* we use the tree view parent (the scrolled window),
           * as the drag_highlight doesn't work for tree views.
           */
          gtk_drag_unhighlight (tree_view->parent);
          renamer_dialog->drag_highlighted = FALSE;
        }

      /* we cannot handle the drop */
      return FALSE;
    }
      
  /* compute the drop position */
  if (gtk_tree_view_get_dest_row_at_pos (GTK_TREE_VIEW (tree_view), x, y, &path, &position))
    {
      /* we can only drop before or after an item */
      if (position == GTK_TREE_VIEW_DROP_BEFORE || position == GTK_TREE_VIEW_DROP_INTO_OR_BEFORE)
        position = GTK_TREE_VIEW_DROP_BEFORE;
      else
        position = GTK_TREE_VIEW_DROP_AFTER;
    }
  else
    {
      /* append the item if there are rows in the model */
      model = gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));
      n_rows = gtk_tree_model_iter_n_children (model, NULL);

      if (G_LIKELY (n_rows > 0))
        {
          /* get the last tree path in the model */
          path = gtk_tree_path_new_from_indices (n_rows - 1, -1);
          position = GTK_TREE_VIEW_DROP_AFTER;
        }
    }

  if (G_LIKELY (path != NULL))
    {
      /* highlight the appropriate row */
      gtk_tree_view_set_drag_dest_row (GTK_TREE_VIEW (tree_view), path, position);
      gtk_tree_path_free (path);
    }
  else if (!renamer_dialog->drag_highlighted)
    {
      /* highlight the parent */
      gtk_drag_highlight (tree_view->parent);
      renamer_dialog->drag_highlighted = TRUE;
    }

  /* we can handle the drop */
  gdk_drag_status (context, action, time);
  return TRUE;
}



static gboolean
thunar_renamer_dialog_drag_drop (GtkWidget           *tree_view,
                                 GdkDragContext      *context,
                                 gint                 x,
                                 gint                 y,
                                 guint                time,
                                 ThunarRenamerDialog *renamer_dialog)
{
  GdkAtom                  target;
  GtkTreeSelection        *selection;
  GtkTreePath             *dst_path;
  GtkTreeModel            *model;
  GtkTreeViewDropPosition  drop_pos;
  gint                     position = -1;
  GList                   *rows;
  
  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), FALSE);
  _thunar_return_val_if_fail (GTK_IS_TREE_VIEW (tree_view), FALSE);
  
  /* determine the drop target */
  target = gtk_drag_dest_find_target (tree_view, context, NULL);
  if (G_LIKELY (target == gdk_atom_intern_static_string ("text/uri-list")))
    {
      /* request the data, we call gtk_drag_finish() later */
      gtk_drag_get_data (tree_view, context, target, time);
    }
  else if (target == gdk_atom_intern_static_string ("GTK_TREE_MODEL_ROW"))
    {
      selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));
      rows = gtk_tree_selection_get_selected_rows (selection, &model);
      if (G_LIKELY (rows != NULL))
        {
          /* get the drop position, if no path is found we append the file */
          if (gtk_tree_view_get_dest_row_at_pos (GTK_TREE_VIEW (tree_view), x, y, &dst_path, &drop_pos))
            {
              /* get the position of the path */
              position = gtk_tree_path_get_indices (dst_path)[0];
              gtk_tree_path_free (dst_path);

              /* advance the offset if we drop after the item */
              if (drop_pos == GTK_TREE_VIEW_DROP_AFTER || drop_pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER)
                position++;
            }

          /* perform the move */
          thunar_renamer_model_reorder (renamer_dialog->model, rows, position);
          g_list_foreach (rows, (GFunc) gtk_tree_path_free, NULL);
          g_list_free (rows);
        }

      /* finish the dnd operation */
      gtk_drag_finish (context, TRUE, FALSE, time);
    }
  else
    {
      /* we cannot handle the drop */
      return FALSE;
    }

  return TRUE;
}
                                  


static void
thunar_renamer_dialog_notify_page (GtkNotebook         *notebook,
                                   GParamSpec          *pspec,
                                   ThunarRenamerDialog *renamer_dialog)
{
  GtkWidget *renamer;
  gint       page;

  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));
  _thunar_return_if_fail (GTK_IS_NOTEBOOK (notebook));

  /* determine the currently active renamer page */
  page = gtk_notebook_get_current_page (notebook);
  if (G_LIKELY (page >= 0))
    {
      /* determine the renamer for that page... */
      renamer = gtk_notebook_get_nth_page (notebook, page);

      /* ...and set that renamer for the model */
      thunar_renamer_model_set_renamer (renamer_dialog->model, THUNARX_RENAMER (renamer));
    }
}



static gboolean
thunar_renamer_dialog_popup_menu (GtkWidget           *tree_view,
                                  ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_val_if_fail (GTK_IS_TREE_VIEW (tree_view), FALSE);
  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), FALSE);

  /* popup the context menu */
  thunar_renamer_dialog_context_menu (renamer_dialog, 0, gtk_get_current_event_time ());

  /* we handled the event */
  return TRUE;
}



static void
thunar_renamer_dialog_row_activated (GtkTreeView         *tree_view,
                                     GtkTreePath         *path,
                                     GtkTreeViewColumn   *column,
                                     ThunarRenamerDialog *renamer_dialog)
{
  GtkAction *action;

  _thunar_return_if_fail (GTK_IS_TREE_VIEW (tree_view));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* just activate the "open" action */
  action = thunar_gtk_ui_manager_get_action_by_name (renamer_dialog->ui_manager, "open");
  if (G_LIKELY (action != NULL))
    gtk_action_activate (action);
}



static void
thunar_renamer_dialog_selection_changed (GtkTreeSelection    *selection,
                                         ThunarRenamerDialog *renamer_dialog)
{
  GtkTreeModel *model;
  GtkTreeIter   iter;
  ThunarFile   *file;
  GtkAction    *action;
  GList        *rows;
  GList        *lp;
  gint          n_selected_files = 0;

  _thunar_return_if_fail (GTK_IS_TREE_SELECTION (selection));
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* release the list of previously selected files */
  thunarx_file_info_list_free (renamer_dialog->selected_files);
  renamer_dialog->selected_files = NULL;

  /* determine the new list of selected files */
  rows = gtk_tree_selection_get_selected_rows (selection, &model);
  for (lp = g_list_last (rows); lp != NULL; lp = lp->prev, ++n_selected_files)
    {
      /* determine the iter for this path */
      if (gtk_tree_model_get_iter (model, &iter, lp->data))
        {
          /* determine the file for this row and prepend it to our list */
          gtk_tree_model_get (model, &iter, THUNAR_RENAMER_MODEL_COLUMN_FILE, &file, -1);
          if (G_LIKELY (file != NULL))
            renamer_dialog->selected_files = g_list_prepend (renamer_dialog->selected_files, file);
        }

      /* release this path */
      gtk_tree_path_free (lp->data);
    }
  g_list_free (rows);

  /* the "Remove Files" action is only sensitive if we have one or more files in the selection */
  action = gtk_action_group_get_action (renamer_dialog->action_group, "remove-files");
  g_object_set (G_OBJECT (action),
                "label", ngettext ("Remove File", "Remove Files", n_selected_files),
                "sensitive", (n_selected_files > 0),
                "tooltip", ngettext ("Remove the selected file from the list of files to be renamed",
                                     "Remove the selected files from the list of files to be renamed",
                                     n_selected_files),
                NULL);

  /* the "Properties" action is only sensitive if we have exactly one selected file */
  action = gtk_action_group_get_action (renamer_dialog->action_group, "properties");
  gtk_action_set_sensitive (action, (n_selected_files == 1));

  /* notify listeners */
  g_object_notify (G_OBJECT (renamer_dialog), "selected-files");
}



/**
 * thunar_renamer_dialog_new:
 *
 * Allocates a new #ThunarRenamerDialog instance.
 *
 * Return value: the newly allocated #ThunarRenamerDialog
 *               instance.
 **/
GtkWidget*
thunar_renamer_dialog_new (void)
{
  return g_object_new (THUNAR_TYPE_RENAMER_DIALOG, NULL);
}



/**
 * thunar_renamer_dialog_get_current_directory:
 * @renamer_dialog : a #ThunarRenamerDialog.
 *
 * Returns the current directory of the @renamer_dialog, which
 * is used as default directory for the "Add Files" dialog.
 * May also be %NULL in which case the current directory of the
 * file manager process will be used.
 *
 * Return value: the current directory for @renamer_dialog.
 **/
ThunarFile*
thunar_renamer_dialog_get_current_directory (ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), NULL);
  return renamer_dialog->current_directory;
}



/**
 * thunar_renamer_dialog_set_current_directory:
 * @renamer_dialog    : a #ThunarRenamerDialog.
 * @current_directory : a #ThunarFile or %NULL.
 *
 * The @renamer_dialog will use @current_directory as default
 * directory for the "Add Files" dialog. If @current_directory
 * is %NULL, the current directory of the file manager process
 * will be used.
 **/
void
thunar_renamer_dialog_set_current_directory (ThunarRenamerDialog *renamer_dialog,
                                             ThunarFile          *current_directory)
{
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));
  _thunar_return_if_fail (current_directory == NULL || THUNAR_IS_FILE (current_directory));

  /* check if we already use that directory */
  if (G_UNLIKELY (renamer_dialog->current_directory == current_directory))
    return;

  /* disconnect from the previous directory */
  if (renamer_dialog->current_directory != NULL)
    g_object_unref (G_OBJECT (renamer_dialog->current_directory));

  /* activate the new directory */
  renamer_dialog->current_directory = current_directory;

  /* connect to the new directory */
  if (current_directory != NULL)
    g_object_ref (G_OBJECT (current_directory));

  /* notify listeners */
  g_object_notify (G_OBJECT (renamer_dialog), "current-directory");
}



/**
 * thunar_renamer_dialog_get_selected_files:
 * @renamer_dialog : a #ThunarRenamerDialog.
 *
 * Returns the #GList of the currently selected #ThunarFile<!---->s
 * in @renamer_dialog.
 *
 * Return value: the list of currently selected files.
 **/
GList*
thunar_renamer_dialog_get_selected_files (ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), NULL);
  return renamer_dialog->selected_files;
}



/**
 * thunar_renamer_dialog_get_standalone:
 * @renamer_dialog : a #ThunarRenamerDialog.
 *
 * Returns %TRUE if the @renamer_dialog is standalone, i.e.
 * if it was not opened from within the file manager with
 * a preset file list.
 *
 * Return value: %TRUE if @renamer_dialog is standalone.
 **/
gboolean
thunar_renamer_dialog_get_standalone (ThunarRenamerDialog *renamer_dialog)
{
  _thunar_return_val_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog), FALSE);
  return renamer_dialog->standalone;
}



/**
 * thunar_renamer_dialog_set_standalone:
 * @renamer_dialog : a #ThunarRenamerDialog.
 * @standalone     : the new standalone state for @renamer_dialog.
 *
 * If @standalone is %TRUE the @renamer_dialog will display a
 * toolbar and several other actions that do not make sense
 * when opened from within the file manager.
 **/
void
thunar_renamer_dialog_set_standalone (ThunarRenamerDialog *renamer_dialog,
                                      gboolean             standalone)
{
  _thunar_return_if_fail (THUNAR_IS_RENAMER_DIALOG (renamer_dialog));

  /* normalize the value */
  standalone = !!standalone;

  /* check if we have a new setting */
  if (G_LIKELY (renamer_dialog->standalone != standalone))
    {
      /* apply the new setting */
      renamer_dialog->standalone = standalone;

      /* change title to reflect the standalone status */
      gtk_window_set_title (GTK_WINDOW (renamer_dialog), standalone ? _("Bulk Rename - Rename Multiple Files") : _("Rename Multiple Files"));

      /* display the appropriate Cancel/Close button */
      if (G_UNLIKELY (standalone))
        {
          /* standalone has Close button by default */
          gtk_widget_hide (renamer_dialog->cancel_button);
          gtk_widget_show (renamer_dialog->close_button);
        }
      else
        {
          /* else, Cancel button is default */
          gtk_widget_show (renamer_dialog->cancel_button);
          gtk_widget_hide (renamer_dialog->close_button);
        }

      /* notify listeners */
      g_object_notify (G_OBJECT (renamer_dialog), "standalone");
    }
}



/**
 * thunar_show_renamer_dialog:
 * @parent            : the #GtkWidget or the #GdkScreen on which to open the
 *                      dialog. May also be %NULL in which case the default
 *                      #GdkScreen will be used.
 * @current_directory : the current directory for the #ThunarRenamerDialog,
 *                      which is used as default folder for the "Add Files"
 *                      dialog. May also be %NULL in which case the current
 *                      directory of the file manager process will be used.
 * @files             : the list of #ThunarFile<!---->s to rename.
 * @standalone        : whether the dialog should appear like a standalone
 *                      application instead of an integrated renamer dialog.
 *
 * Convenience function to display a #ThunarRenamerDialog with
 * the given parameters.
 **/
void
thunar_show_renamer_dialog (gpointer    parent,
                            ThunarFile *current_directory,
                            GList      *files,
                            gboolean    standalone)
{
  ThunarApplication *application;
  GdkScreen         *screen;
  GtkWidget         *dialog;
  GtkWidget         *window = NULL;
  GList             *lp;

  _thunar_return_if_fail (parent == NULL || GDK_IS_SCREEN (parent) || GTK_IS_WIDGET (parent));

  /* determine the screen for the dialog */
  if (G_UNLIKELY (parent == NULL))
    {
      /* just use the default screen, no toplevel window */
      screen = gdk_screen_get_default ();
    }
  else if (GTK_IS_WIDGET (parent))
    {
      /* use the screen for the widget and the toplevel window */
      screen = gtk_widget_get_screen (parent);
      window = gtk_widget_get_toplevel (parent);
    }
  else
    {
      /* parent is a screen, no toplevel window */
      screen = GDK_SCREEN (parent);
    }

  /* display the bulk rename dialog */
  dialog = g_object_new (THUNAR_TYPE_RENAMER_DIALOG,
                         "current-directory", current_directory,
                         "screen", screen,
                         "standalone", standalone,
                         NULL);

  /* check if we have a toplevel window */
  if (G_LIKELY (window != NULL && GTK_WIDGET_TOPLEVEL (window)))
    {
      /* dialog is transient for toplevel window, but not modal */
      gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
      gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (window));
    }

  /* append all specified files to the dialog's model */
  for (lp = files; lp != NULL; lp = lp->next)
    thunar_renamer_model_append (THUNAR_RENAMER_DIALOG (dialog)->model, lp->data);

  /* let the application handle the dialog */
  application = thunar_application_get ();
  thunar_application_take_window (application, GTK_WINDOW (dialog));
  g_object_unref (G_OBJECT (application));

  /* display the dialog */
  gtk_widget_show (dialog);
}


