#include "WOptionSettings.h"
#include "WFile.h"
#include "VersionFileController.h"

#if is_ios || (is_mac && !USE_QT)

#import <Foundation/Foundation.h>

auto WOptionSettings::getPathWritable() -> WString
{
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsPath = [documentDirectories firstObject];

    return WString (documentsPath.UTF8String);
}

#endif
