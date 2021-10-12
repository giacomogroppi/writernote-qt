#ifndef STRUCT_LAST_FILE_H
#define STRUCT_LAST_FILE_H

#include <QSettings>

#include <QMetaType>
#define MAXSTR 200
#define MAXMOD 12

/* the file is in your computer or in cloud */
#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

#define MAXSTR__FILE MAXSTR+1
#define MAXMOD__FILE MAXMOD+1

#define TYPE_OWNER_YOU 0
#define TYPE_OWNER_EXTERNAL 1

struct owner_s{
    unsigned short int type_user;
    char name[MAXSTR__FILE];
};

struct last_file_s{
    char posizione[MAXSTR__FILE];
    unsigned short int type;
    char last_modification_o[MAXMOD__FILE]; /* time */
    char last_modification_g[MAXMOD__FILE]; /* day */

    struct owner_s owner;
};

class last_file{
private:

public:
    QList<last_file_s> m_data;

    inline uint length() const
    { return m_data.length(); }
    inline const last_file_s & at(int i) const
    { return m_data.at(i); }
    inline last_file_s & at_mod(int i)
    { return m_data.operator[](i); }
    inline void removeAt(int i)
    { m_data.removeAt(i); }

    void save_quanti(int quanti);
    static void save_data_setting(const QByteArray &arr);
    void save_data_setting();

    static void save_data(const QString &path, int type = TYPE_COMPUTER, int owner_TYPE = TYPE_OWNER_YOU, const char *owner = nullptr);

    bool load_data();
    bool load_folder(const QString &path);

    void removeDouble();
    void tidyup();
};

#endif // STRUCT_LAST_FILE_H
