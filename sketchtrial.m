#import <objc/runtime.h>
#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>


//
// BCRegularLicense
//

@interface BCRegularLicensePatched : NSObject
- (bool)isExpired;
- (bool)isValid;
@end

@interface BCLicenseManagerPatched : NSObject
- (long long)numberOfDaysLeftInTrialMode;
@end

@implementation BCRegularLicensePatched

+ (void)load {
    Class origClass = NSClassFromString(@"BCRegularLicense");
    
    method_exchangeImplementations(
        class_getInstanceMethod(origClass, @selector(isExpired)),
        class_getInstanceMethod(NSClassFromString(@"BCRegularLicensePatched"), @selector(isExpired))
    );

    method_exchangeImplementations(
        class_getInstanceMethod(origClass, @selector(isValid)),
        class_getInstanceMethod(NSClassFromString(@"BCRegularLicensePatched"), @selector(isValid))
    );
}

- (bool)isExpired {
    return false;
}

- (bool)isValid {
    return true;
}

@end


//
// BCLicenseManager
//

@implementation BCLicenseManagerPatched

+ (void)load {
    Class origClass = NSClassFromString(@"BCLicenseManager");

    method_exchangeImplementations(
        class_getInstanceMethod(origClass, @selector(numberOfDaysLeftInTrialMode)),
        class_getInstanceMethod(NSClassFromString(@"BCLicenseManagerPatched"), @selector(numberOfDaysLeftInTrialMode))
    );
}

- (long long)numberOfDaysLeftInTrialMode {
    return 9000;
}

@end
