#pragma once

#define ERROR 0
#define OK 1
#define ERROR_VERSION 2
#define ERROR_VERSION_NEW 3
#define ERROR_CONTROLL 4            /* return when the data is corrupted */
#define ERROR_MULTIPLE_COPYBOOK 5
#define ERROR_VERSION_KEY 6         /* return when the file is written with keayboard */

/* controll keyboard */
#define CONTROLL_KEY(x) if(!(x)) \
    return ERROR_VERSION_KEY;

#define message_key_error dialog_critic(QApplication::tr("It is not possible to load this file, from now on the files written with the keyboard will no longer be readable"));

