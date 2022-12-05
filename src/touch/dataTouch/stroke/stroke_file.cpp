#include "stroke_file.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "currenttitle/document.h"
#include "core/WReadZip.h"
#include "core/WZipWriterSingle.h"
#include "datawrite/source_read_ext.h"

#define stroke_file_size_len 4

#define MANAGE_ERR() return ERROR

