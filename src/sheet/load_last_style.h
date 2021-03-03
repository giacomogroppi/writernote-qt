#ifndef LOAD_LAST_STYLE_H
#define LOAD_LAST_STYLE_H

#include "style_struct.h"

#define DEFAULTTHICKNESS 1
#define DEFAULTN 20
#define DEFAULTNOME "Undefined"

style_struct * load_last_style();
void save_last_style(style_struct *);

int load_default_drawing_index();

/* the function load the default setting for drawing */
style_struct_S * load_default_drawing();

/* the function save the default setting for drawing */
void save_default_drawing(int *);

void load_default(style_struct *, const char *name, int indice = -1);

/* the function is called by disegnofoglio */
void load_default_onlystyle(style_struct_S *default_setting);

#endif // LOAD_LAST_STYLE_H
