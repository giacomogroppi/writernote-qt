#include "frompdf.h"
#include <QPdfium>
#include "../utils/dialog_critic/dialog_critic.h"

frompdf::frompdf()
{
    QPdfium pdf("/home/");
    if(!pdf.isValid()){
        dialog_critic("Pdf not valid position");
    }
}
