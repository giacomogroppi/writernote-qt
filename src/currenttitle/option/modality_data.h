#pragma once

#include "restore_file/ui/setting_restore_ui.h"

#include "QtGlobal"

#define LEN_MODAL 20
#define MOD_SAVE_ALL 1
#define MOD_ASK_SAVE 2
#define MOD_NOT_SAVE 3

struct modality{
    char name[LEN_MODAL];
    struct __data m_data;

    uchar save_when_change = MOD_ASK_SAVE;
};

