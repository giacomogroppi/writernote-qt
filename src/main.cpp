#include "mainwindow.h"
#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"
#include "string.h"

#include "dataread/xmlstruct.h"

#define COMMAND_EXTRACT "--extract"
#define OK_EXTRACT_AUDIO 1
#define ERROR_NO_COPYBOOK 2
#define ERROR_LOAD_FILE 3
#define ERROR_NOT_RECORD 4
#define ERROR_LOAD_AUDIO 5
#define ERROR_OPEN_TO 6
static int extractAudio(const char *path, const char *namecopybook, const char *path_to);

#define HELP_COMMAND "\nTo extract an audio digit --extract, followed by the location of the file, the name of the copybook, \nand where you would like to save the audio\n\nTo open a file type the path of the file\n"

#ifdef CLOUD
#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"
#endif

int main(int argc, char *argv[])
{
    if(argc == 5 && !strcmp(argv[1], COMMAND_EXTRACT)){
        int res = extractAudio(argv[1], argv[2], argv[3]);
        if(res == OK_EXTRACT_AUDIO){
            printf("File extract correctly into %s", argv[3]);
            return 0;
        }
        if(res == ERROR_NO_COPYBOOK){
            printf("There seems to be no copybook with this name inside the file\n");
            return -1;
        }
        if(res == ERROR_LOAD_FILE){
            printf("We had a problem uploading the file\n");
            return -1;
        }
        if(res == ERROR_NOT_RECORD){
            printf("The copybook you entered has not been registered\n");
            return -1;
        }
        if(res == ERROR_LOAD_AUDIO){
            printf("We had a problem reading the audio\n");
            return -1;
        }
        if(res == ERROR_OPEN_TO){
            printf("Path %s is not valid\n", argv[3]);
            return -1;
        }
        printf("Unknown error\n");
        return -1;
    }


    if(argc != 2 && argc != 1){
        printf("Error");

        printf(HELP_COMMAND);

        return -1;
    }

#ifdef CLOUD
    struct struct_user *user;
    user = load_recent_user();

    cloud_controll m_cloud(user);

#endif

    bool close_all = false;
    TabletApplication *app = new TabletApplication(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app->setCanvas(canvas);

    char * m_last_open = NULL;

    if(argc == 1){

#ifdef CLOUD
        last_open a(nullptr, user, &m_cloud, &close_all);
#else
        last_open a(nullptr, nullptr, nullptr, &close_all);
#endif
        a.setDataReturn(&m_last_open);

        if(a.load_data_() != 0){
            a.exec();
        }
    }


    /*
     * set by load_open to true if the user want to quit all
    */
    if(close_all){
#ifdef CLOUD
        if(user)
            delete user;
#endif
        return 0;
    }

#ifdef CLOUD
    MainWindow w(nullptr, canvas, user, &m_cloud);
#else
    MainWindow w(nullptr, canvas);
#endif

    if(m_last_open != NULL)
        w.setFile(m_last_open);

    if(argc != 1){
        w.setFile(argv[1]);
    }

    w.show();
    int exit_code = app->exec();

#ifdef CLOUD
    if(user)
        delete user;
#endif

    return exit_code;
}


#define DELETE_T delete indice; \
    delete title; \
    delete m_data;

/*
 * the function extract an audio from a copybook, into path_to
*/
static int extractAudio(const char *path, const char *namecopybook, const char *path_to){
    QString m_path = path, m_namecopybook = namecopybook, m_path_to = path_to;

    indice_class * indice = new indice_class;
    currenttitle_class * title = new currenttitle_class;

    xmlstruct * m_data = new xmlstruct(&m_path, indice, title);

    if(m_data->loadindice() != OK){
        DELETE_T;
        return ERROR_LOAD_FILE;
    }

    if(indice->titolo.indexOf(namecopybook) == -1){
        DELETE_T;
        return ERROR_NO_COPYBOOK;
    }

    if(m_data->loadfile(namecopybook) != OK){
        DELETE_T;
        return ERROR_LOAD_FILE;
    }

    if(title->se_registato == audio_record::not_record){
        DELETE_T;
        return ERROR_NOT_RECORD;
    }

    if(load_audio(&title->audio_data, m_namecopybook, m_path) != OK)
    {
        DELETE_T;
        return ERROR_LOAD_AUDIO;
    }

    FILE *fp = fopen(path_to, "w");
    if(!fp){
        DELETE_T;
        return ERROR_OPEN_TO;
    }

    auto size_audio = title->audio_data.size();

    fwrite(title->audio_data.data(), size_audio, 1, fp);

    fclose(fp);

    DELETE_T;

    return OK_EXTRACT_AUDIO;
}
