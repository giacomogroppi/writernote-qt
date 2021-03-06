#ifndef SETTING_DEFINE_H
#define SETTING_DEFINE_H

#define ORGANIZATIONAME "Writernote"
#define APPLICATION_NAME "writernote"

/* group for the style */
#define GROUPNAME_STYLE "style"
#define KEYSTYLE "style_form"
#define KEYDEFAULTSTYLE "style_form_default"

/* group for the size */
#define GROUPNAME_GEOMETRY "geometry"
#define KEY_GEOMETRY "geometry_key"

/* for audio settings */
#define GROUPNAME_AUDIO "audio"
#define KEY_AUDIO_DEVICE_BOX "audioDeviceBox"
#define KEY_SAMPLE_RATE_BOX "sampleRateBox"
#define KEY_AUDIO_CODEC_BOX "audioCodecBox"
#define KEY_CHANNELS_BOX "channelsBox"
#define KEY_QUALITY_SLIDER "qualitySlider"
#define KEY_CONTAINER_BOX "containerBox"

/* autosave */
#define GROUPNAME_AUTOSAVE "autosave_group"
#define KEY_AUTOSAVE "key_autosave"

/* redoundo */
#define GROUPNAME_REDOUNDO "redoundo"
#define KEY_REDOUNDO "redoundo_key"

/* rubber */
#define GROUPNAME_RUBBER "rubber_group"
#define KEY_RUBBER_SIZE "rubber_key_size"
#define KEY_RUBBER_TYPE "rubber_key_type"

/* pen ui */
#define GROUP_PEN "group_pen"
#define KEY_PEN_SIZE "pen_key_size"
#define KEY_PEN_TYPE "type_of_pen_key"
#define KEY_PEN_TIPO_TRATTO "key_tipo_tratto"

/* last file open*/
#define GROUPNAME_LAST_FILE "group_last_file"
#define KEY_LAST_FILE_QUANTI "quanti_last_file"
#define KEY_LAST_BASE_FILE "key_base_"

#define FILE_NAME_USER_CLOUD "writernote-user"

#ifdef CLOUD
/* cloud */
#define GROUPNAME_CLOUD_USER "groupname-cloud-user"
#define KEY_USER_CLOUD_IS_DEFINED "key-user-is-defined" /* bool value. true -> ok false -> return null */
#define KEY_USER_CLOUD_STRUCT "key-user"
#endif

/* list left hidden */
#define GROUPNAME_LIST_HIDDEN "list_left_group"
#define KEY_LIST_HIDDEN "key_list_left"

#define GROUPNAME_TEXT_PEN "groupname-text-pen"
#define KEY_TEXT_PEN_SIZE "groupname-text-pen-size"
#define KEY_TEXT_PEN_COLOR "groupname-text-pen-color"
#define KEY_TEXT_PEN_FONT "groupname-text-pen-font"

#ifdef SNAP
/* permission for audio snapcraft */
#define GROUPNAME_AUDIO_PERMISSION "audio_permission_snapcraft"
#define KEY_AUDIO_PERMISSION "key_audio_permission_snapcraft"
#endif //snap

#define GROUPNAME_AUDIO_POSITION "groupname-audio-position"
#define KEY_AUDIO_POSITION_TYPE "key-audio-position-type" /* int */

/* remove audio from temp folder */
#define GROUPNAME_AUDIO_REMOVE_RECORD "groupname-audio-remove"
#define KEY_AUDIO_REMOVE_RECORD "key-audio-remove"

/* auto create new sheet */
#define GROUPNAME_AUTO_CREATE_SHEET "groupname-auto-sheet"
#define KEY_AUTO_CREATE_SHEET "key-auto-create-sheet" /* bool -> true automatic creation*/
#define KEY_AUTO_CREATE_STYLE_FAST "key-auto-create-fast" /* fast style selectable from mainwindow */

#endif // SETTING_DEFINE_H
