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
#define KEY_PEN_SAME_DATA "key-same-data-pen"

/* last file open*/
#define GROUPNAME_LAST_FILE "group_last_file"
//#define KEY_LAST_FILE_QUANTI "quanti_last_file"
#define KEY_LAST_BASE_FILE "key_base_"

/* method: uchar */
#define KEY_LAST_FILE_METHOD "key-last-file-method"

/* number: int */
#define KEY_LAST_FILE_NUMBER "key-last-file-numbero"
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
#define KEY_LAST_FILE_SHOW_ONLY_NAME "key-last-file-show-only-name"
#endif // ANDROID

#define FILE_NAME_USER_CLOUD "writernote-user"

#ifdef CLOUD
/* cloud */
#define GROUPNAME_CLOUD_USER "groupname-cloud-user"
#define KEY_USER_CLOUD_IS_DEFINED "key-user-is-defined" /* bool value. true -> ok false -> return null */
#define KEY_USER_CLOUD_STRUCT "key-user"
#endif

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

/* last method used */
#define GROUPNAME_METHOD_TOUCH "groupname-method-touch"
#define KEY_METHOD_TOUCH "key-method-touch" /* int */

/* log position */
#define GROUPNAME_LOG_POSITION "groupname-log-position"
#define KEY_LOG_POSITION "key-log-position" /* QString */
#define KEY_LOG_POSITION_DEFINE "key-log-position-define" /* int */

/* default type of inpunt */
#define GROURPNAME_DEFAULT_INSERT "groupname-default-insert"
#define KEY_DEFAULT_INSERT "key-default-insert-method"

#define GROUPNAME_LAST_LANGUAGE "groupname-last-language"
#define KEY_LAST_LANGUAGE "key-last-language"

/* tablet scrolling speed */
#define GROUPNAME_SPEED_SCROLLING "groupname-speed-scrolling"
#define KEY_SPEED_SCROLLING "key-speed-scrolling" /* int */
#define KEY_SPEED_SCROLLING_ENABLE "key-speed-scrolling-enable" /* bool */

/* integrity of data and autosave */
#define GROUPNAME_INT "groupname-int"
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
#define KEY_INT_AUTOSAVE_ENABLE "key-int-autosave-enable"
#endif
#define KEY_INT_AUTOSAVE_TIME "key-int-autosave-time" /* uint -> second */
#define KEY_INT_REMOVE_FILE_ENABLE "key-int-remove-file-enable" /* bool */
#define KEY_INT_TMP_ENABLE "key-int-tmp-enable"
#define KEY_INT_TMP_TIME "key-int-tmp-time" /* uint -> second */

#define GROUPNAME_INSERT_METHOD_PEN_MOUSE "groupname-insert-method-pen-mouse"
#define KEY_INSERT_METHOD_PEN_MOUSE "key-insert-method-pen-mouse" /* bool */

/* highlighter */
#define GROUPNAME_HIGHLIGHTER "groupname-highlighter"
#define KEY_HIGHLIGHTER_ALFA "key-highlighter-alfa"
#define KEY_HIGHLIGHTER_SIZE "key-highlighter-size" /* int */
#define KEY_HIGHLIGHTER_SPESS "key-highlighter-spess" /* bool */
#define KEY_HIGHLIGHTER_SOTTO "key-highlighter-sotto" /* bool */

/* size button */
#define GROUPNAME_SIZE_BUTTON "groupname-size-button"
#define KEY_SIZE_BUTTON "key-size-button"

#define GROUPNAME_MODALITY "groupname-modality"
#define KEY_MODALITY "key-modality" /* uchar */

#endif // SETTING_DEFINE_H
