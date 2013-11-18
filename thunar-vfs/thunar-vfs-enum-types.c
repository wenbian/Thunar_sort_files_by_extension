
/* Generated data (by glib-mkenums) */

#undef GTK_DISABLE_DEPRECATED
#define GTK_ENABLE_BROKEN
#include <thunar-vfs/thunar-vfs.h>
#include <thunar-vfs/thunar-vfs-alias.h>

/* enumerations from "thunar-vfs-job.h" */
GType
thunar_vfs_job_response_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_JOB_RESPONSE_YES, "THUNAR_VFS_JOB_RESPONSE_YES", "yes" },
	{ THUNAR_VFS_JOB_RESPONSE_YES_ALL, "THUNAR_VFS_JOB_RESPONSE_YES_ALL", "yes-all" },
	{ THUNAR_VFS_JOB_RESPONSE_NO, "THUNAR_VFS_JOB_RESPONSE_NO", "no" },
	{ THUNAR_VFS_JOB_RESPONSE_CANCEL, "THUNAR_VFS_JOB_RESPONSE_CANCEL", "cancel" },
	{ THUNAR_VFS_JOB_RESPONSE_NO_ALL, "THUNAR_VFS_JOB_RESPONSE_NO_ALL", "no-all" },
	{ THUNAR_VFS_JOB_RESPONSE_RETRY, "THUNAR_VFS_JOB_RESPONSE_RETRY", "retry" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsJobResponse", values);
  }
	return type;
}

/* enumerations from "thunar-vfs-mime-handler.h" */
GType
thunar_vfs_mime_handler_flags_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_MIME_HANDLER_HIDDEN, "THUNAR_VFS_MIME_HANDLER_HIDDEN", "hidden" },
	{ THUNAR_VFS_MIME_HANDLER_REQUIRES_TERMINAL, "THUNAR_VFS_MIME_HANDLER_REQUIRES_TERMINAL", "requires-terminal" },
	{ THUNAR_VFS_MIME_HANDLER_SUPPORTS_STARTUP_NOTIFY, "THUNAR_VFS_MIME_HANDLER_SUPPORTS_STARTUP_NOTIFY", "supports-startup-notify" },
	{ THUNAR_VFS_MIME_HANDLER_SUPPORTS_MULTI, "THUNAR_VFS_MIME_HANDLER_SUPPORTS_MULTI", "supports-multi" },
	{ THUNAR_VFS_MIME_HANDLER_SUPPORTS_URIS, "THUNAR_VFS_MIME_HANDLER_SUPPORTS_URIS", "supports-uris" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsMimeHandlerFlags", values);
  }
	return type;
}

/* enumerations from "thunar-vfs-monitor.h" */
GType
thunar_vfs_monitor_event_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ THUNAR_VFS_MONITOR_EVENT_CHANGED, "THUNAR_VFS_MONITOR_EVENT_CHANGED", "changed" },
	{ THUNAR_VFS_MONITOR_EVENT_CREATED, "THUNAR_VFS_MONITOR_EVENT_CREATED", "created" },
	{ THUNAR_VFS_MONITOR_EVENT_DELETED, "THUNAR_VFS_MONITOR_EVENT_DELETED", "deleted" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ThunarVfsMonitorEvent", values);
  }
	return type;
}

/* enumerations from "thunar-vfs-thumb.h" */
GType
thunar_vfs_thumb_size_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ THUNAR_VFS_THUMB_SIZE_NORMAL, "THUNAR_VFS_THUMB_SIZE_NORMAL", "normal" },
	{ THUNAR_VFS_THUMB_SIZE_LARGE, "THUNAR_VFS_THUMB_SIZE_LARGE", "large" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ThunarVfsThumbSize", values);
  }
	return type;
}

/* enumerations from "thunar-vfs-types.h" */
GType
thunar_vfs_deep_count_flags_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_DEEP_COUNT_FLAGS_NONE, "THUNAR_VFS_DEEP_COUNT_FLAGS_NONE", "none" },
	{ THUNAR_VFS_DEEP_COUNT_FLAGS_FOLLOW_SYMLINKS, "THUNAR_VFS_DEEP_COUNT_FLAGS_FOLLOW_SYMLINKS", "follow-symlinks" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsDeepCountFlags", values);
  }
	return type;
}
GType
thunar_vfs_file_type_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ THUNAR_VFS_FILE_TYPE_PORT, "THUNAR_VFS_FILE_TYPE_PORT", "port" },
	{ THUNAR_VFS_FILE_TYPE_DOOR, "THUNAR_VFS_FILE_TYPE_DOOR", "door" },
	{ THUNAR_VFS_FILE_TYPE_SOCKET, "THUNAR_VFS_FILE_TYPE_SOCKET", "socket" },
	{ THUNAR_VFS_FILE_TYPE_SYMLINK, "THUNAR_VFS_FILE_TYPE_SYMLINK", "symlink" },
	{ THUNAR_VFS_FILE_TYPE_REGULAR, "THUNAR_VFS_FILE_TYPE_REGULAR", "regular" },
	{ THUNAR_VFS_FILE_TYPE_BLOCKDEV, "THUNAR_VFS_FILE_TYPE_BLOCKDEV", "blockdev" },
	{ THUNAR_VFS_FILE_TYPE_DIRECTORY, "THUNAR_VFS_FILE_TYPE_DIRECTORY", "directory" },
	{ THUNAR_VFS_FILE_TYPE_CHARDEV, "THUNAR_VFS_FILE_TYPE_CHARDEV", "chardev" },
	{ THUNAR_VFS_FILE_TYPE_FIFO, "THUNAR_VFS_FILE_TYPE_FIFO", "fifo" },
	{ THUNAR_VFS_FILE_TYPE_UNKNOWN, "THUNAR_VFS_FILE_TYPE_UNKNOWN", "unknown" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ThunarVfsFileType", values);
  }
	return type;
}
GType
thunar_vfs_file_mode_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_FILE_MODE_SUID, "THUNAR_VFS_FILE_MODE_SUID", "suid" },
	{ THUNAR_VFS_FILE_MODE_SGID, "THUNAR_VFS_FILE_MODE_SGID", "sgid" },
	{ THUNAR_VFS_FILE_MODE_STICKY, "THUNAR_VFS_FILE_MODE_STICKY", "sticky" },
	{ THUNAR_VFS_FILE_MODE_USR_ALL, "THUNAR_VFS_FILE_MODE_USR_ALL", "usr-all" },
	{ THUNAR_VFS_FILE_MODE_USR_READ, "THUNAR_VFS_FILE_MODE_USR_READ", "usr-read" },
	{ THUNAR_VFS_FILE_MODE_USR_WRITE, "THUNAR_VFS_FILE_MODE_USR_WRITE", "usr-write" },
	{ THUNAR_VFS_FILE_MODE_USR_EXEC, "THUNAR_VFS_FILE_MODE_USR_EXEC", "usr-exec" },
	{ THUNAR_VFS_FILE_MODE_GRP_ALL, "THUNAR_VFS_FILE_MODE_GRP_ALL", "grp-all" },
	{ THUNAR_VFS_FILE_MODE_GRP_READ, "THUNAR_VFS_FILE_MODE_GRP_READ", "grp-read" },
	{ THUNAR_VFS_FILE_MODE_GRP_WRITE, "THUNAR_VFS_FILE_MODE_GRP_WRITE", "grp-write" },
	{ THUNAR_VFS_FILE_MODE_GRP_EXEC, "THUNAR_VFS_FILE_MODE_GRP_EXEC", "grp-exec" },
	{ THUNAR_VFS_FILE_MODE_OTH_ALL, "THUNAR_VFS_FILE_MODE_OTH_ALL", "oth-all" },
	{ THUNAR_VFS_FILE_MODE_OTH_READ, "THUNAR_VFS_FILE_MODE_OTH_READ", "oth-read" },
	{ THUNAR_VFS_FILE_MODE_OTH_WRITE, "THUNAR_VFS_FILE_MODE_OTH_WRITE", "oth-write" },
	{ THUNAR_VFS_FILE_MODE_OTH_EXEC, "THUNAR_VFS_FILE_MODE_OTH_EXEC", "oth-exec" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsFileMode", values);
  }
	return type;
}
GType
thunar_vfs_file_flags_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_FILE_FLAGS_NONE, "THUNAR_VFS_FILE_FLAGS_NONE", "none" },
	{ THUNAR_VFS_FILE_FLAGS_SYMLINK, "THUNAR_VFS_FILE_FLAGS_SYMLINK", "symlink" },
	{ THUNAR_VFS_FILE_FLAGS_EXECUTABLE, "THUNAR_VFS_FILE_FLAGS_EXECUTABLE", "executable" },
	{ THUNAR_VFS_FILE_FLAGS_HIDDEN, "THUNAR_VFS_FILE_FLAGS_HIDDEN", "hidden" },
	{ THUNAR_VFS_FILE_FLAGS_READABLE, "THUNAR_VFS_FILE_FLAGS_READABLE", "readable" },
	{ THUNAR_VFS_FILE_FLAGS_WRITABLE, "THUNAR_VFS_FILE_FLAGS_WRITABLE", "writable" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsFileFlags", values);
  }
	return type;
}

/* enumerations from "thunar-vfs-volume.h" */
GType
thunar_vfs_volume_kind_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GEnumValue values[] = {
	{ THUNAR_VFS_VOLUME_KIND_UNKNOWN, "THUNAR_VFS_VOLUME_KIND_UNKNOWN", "unknown" },
	{ THUNAR_VFS_VOLUME_KIND_CDROM, "THUNAR_VFS_VOLUME_KIND_CDROM", "cdrom" },
	{ THUNAR_VFS_VOLUME_KIND_CDR, "THUNAR_VFS_VOLUME_KIND_CDR", "cdr" },
	{ THUNAR_VFS_VOLUME_KIND_CDRW, "THUNAR_VFS_VOLUME_KIND_CDRW", "cdrw" },
	{ THUNAR_VFS_VOLUME_KIND_DVDROM, "THUNAR_VFS_VOLUME_KIND_DVDROM", "dvdrom" },
	{ THUNAR_VFS_VOLUME_KIND_DVDRAM, "THUNAR_VFS_VOLUME_KIND_DVDRAM", "dvdram" },
	{ THUNAR_VFS_VOLUME_KIND_DVDR, "THUNAR_VFS_VOLUME_KIND_DVDR", "dvdr" },
	{ THUNAR_VFS_VOLUME_KIND_DVDRW, "THUNAR_VFS_VOLUME_KIND_DVDRW", "dvdrw" },
	{ THUNAR_VFS_VOLUME_KIND_DVDPLUSR, "THUNAR_VFS_VOLUME_KIND_DVDPLUSR", "dvdplusr" },
	{ THUNAR_VFS_VOLUME_KIND_DVDPLUSRW, "THUNAR_VFS_VOLUME_KIND_DVDPLUSRW", "dvdplusrw" },
	{ THUNAR_VFS_VOLUME_KIND_FLOPPY, "THUNAR_VFS_VOLUME_KIND_FLOPPY", "floppy" },
	{ THUNAR_VFS_VOLUME_KIND_HARDDISK, "THUNAR_VFS_VOLUME_KIND_HARDDISK", "harddisk" },
	{ THUNAR_VFS_VOLUME_KIND_USBSTICK, "THUNAR_VFS_VOLUME_KIND_USBSTICK", "usbstick" },
	{ THUNAR_VFS_VOLUME_KIND_AUDIO_PLAYER, "THUNAR_VFS_VOLUME_KIND_AUDIO_PLAYER", "audio-player" },
	{ THUNAR_VFS_VOLUME_KIND_AUDIO_CD, "THUNAR_VFS_VOLUME_KIND_AUDIO_CD", "audio-cd" },
	{ THUNAR_VFS_VOLUME_KIND_MEMORY_CARD, "THUNAR_VFS_VOLUME_KIND_MEMORY_CARD", "memory-card" },
	{ THUNAR_VFS_VOLUME_KIND_REMOVABLE_DISK, "THUNAR_VFS_VOLUME_KIND_REMOVABLE_DISK", "removable-disk" },
	{ 0, NULL, NULL }
	};
	type = g_enum_register_static ("ThunarVfsVolumeKind", values);
  }
	return type;
}
GType
thunar_vfs_volume_status_get_type (void)
{
	static GType type = 0;
	if (type == 0) {
	static const GFlagsValue values[] = {
	{ THUNAR_VFS_VOLUME_STATUS_MOUNTED, "THUNAR_VFS_VOLUME_STATUS_MOUNTED", "mounted" },
	{ THUNAR_VFS_VOLUME_STATUS_PRESENT, "THUNAR_VFS_VOLUME_STATUS_PRESENT", "present" },
	{ THUNAR_VFS_VOLUME_STATUS_MOUNTABLE, "THUNAR_VFS_VOLUME_STATUS_MOUNTABLE", "mountable" },
	{ 0, NULL, NULL }
	};
	type = g_flags_register_static ("ThunarVfsVolumeStatus", values);
  }
	return type;
}

#define __THUNAR_VFS_ENUM_TYPES_C__
#include "thunar-vfs-aliasdef.c"

/* Generated data ends here */

