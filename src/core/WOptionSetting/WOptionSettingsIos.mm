#include "WOptionSettings.h"
#include "WFile.h"
#include "VersionFileController.h"

#if is_ios || (is_mac && !USE_QT)

#import <Foundation/Foundation.h>

auto WOptionSettings::getPathWritable() -> WPath
{
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsPath = [documentDirectories firstObject];

    return WPath (documentsPath.UTF8String);
}

#endif
