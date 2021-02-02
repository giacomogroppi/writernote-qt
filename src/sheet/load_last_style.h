#ifndef LOAD_LAST_STYLE_H
#define LOAD_LAST_STYLE_H

#include "style_struct.h"

style_struct * load_last_style();
int save_last_style(style_struct *);

/* the function load the default setting for drawing */
style_struct_S * load_default_drawing();

/* the function save the default setting for drawing */
void save_default_drawing(struct style_struct_S *);

void load_default(struct style_struct *);

#endif // LOAD_LAST_STYLE_H
