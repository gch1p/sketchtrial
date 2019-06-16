#import <objc/runtime.h>
#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>


//
// BCRegularLicense
//

@interface BCRegularLicensePatched : NSObject
- (bool)isExpired;
@end

@implementation BCRegularLicensePatched

+(void)load {
Class origClass = NSClassFromString(@"BCRegularLicense");
    Method origMethod = class_getInstanceMethod(origClass, @selector(isExpired));
    Method replMethod = class_getInstanceMethod(NSClassFromString(@"BCRegularLicensePatched"), @selector(isExpired));
    
    method_exchangeImplementations(origMethod, replMethod);
}

-(bool)isExpired {
    return false;
}

@end
