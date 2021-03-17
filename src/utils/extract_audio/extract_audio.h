#ifndef EXTRACT_AUDIO_H
#define EXTRACT_AUDIO_H

/*
#define OK_EXTRACT_AUDIO 1
#define ERROR_NO_COPYBOOK 2
#define ERROR_LOAD_FILE 3
#define ERROR_NOT_RECORD 4
#define ERROR_LOAD_AUDIO 5
#define ERROR_OPEN_TO 6
*/
namespace extract {
    enum n_extract{
        ok,
        no_copybook,
        load_file,
        not_record,
        load_audio,
        open_to
    };
};

extract::n_extract extract_audio(const char *path, const char *namecopybook, const char *path_to);

#endif // EXTRACT_AUDIO_H
