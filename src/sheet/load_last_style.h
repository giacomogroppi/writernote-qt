#ifndef LOAD_LAST_STYLE_H
#define LOAD_LAST_STYLE_H

#include "style_struct.h"

style_struct * load_last_style();
int save_last_style(style_struct *);

void load_default(struct style_struct *);

#endif // LOAD_LAST_STYLE_H
