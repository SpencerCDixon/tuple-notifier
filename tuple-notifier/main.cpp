//
//  main.cpp
//  tuple-notifier
//
//  Created by Spencer Dixon on 7/20/19.
//  Copyright © 2019 Spencer Dixon. All rights reserved.
//

#include <iostream>
#include <CoreFoundation/CoreFoundation.h>

std::string startSystemCall = "echo 'Tuple call started'";
std::string endSystemCall = "echo 'Tuple call ended'";

void startCB(CFNotificationCenterRef center, void* observer, CFStringRef notificationName, const void* observedObject, CFDictionaryRef userInfo ) {
    std::cout << "Tuple call started. Executing: " << startSystemCall << std::endl;
    system(startSystemCall.c_str());
}

void endCB(CFNotificationCenterRef center, void* observer, CFStringRef notificationName, const void* observedObject, CFDictionaryRef userInfo ) {
    std::cout << "Tuple call ended. Executing: " << endSystemCall << std::endl;
    system(endSystemCall.c_str());
}

int main(int argc, const char * argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--on-start") {
            if (i + 1 < argc) {
                startSystemCall = argv[++i];
            } else {
                std::cerr << "Uh oh. --on-start requires an argument but none was found.";
            }
        } else if (arg == "--on-end") {
            if (i + 1 < argc) {
                endSystemCall = argv[++i];
            } else {
                std::cerr << "Uh oh. --on-end requires an argument but none was found.";
            }
        }
    }

    CFNotificationCenterRef center = CFNotificationCenterGetDistributedCenter();
    CFStringRef startNotification = CFSTR("app.tuple.IPCCallStarted");
    CFStringRef endNotification = CFSTR("app.tuple.IPCCallEnded");

    void* startObserver;
    void* endObserver;
    std::cout << "Registering observers...\n";
    CFNotificationCenterAddObserver(center, startObserver, &startCB, startNotification, nullptr, CFNotificationSuspensionBehaviorDeliverImmediately);
    CFNotificationCenterAddObserver(center, endObserver, &endCB, endNotification, nullptr, CFNotificationSuspensionBehaviorDeliverImmediately);

    std::cout << "Starting run loop...\n";
    CFRunLoopRun();

    std::cout << "Cleaning up registered observers\n";
    CFNotificationCenterRemoveObserver(center, startObserver, startNotification, nullptr);
    CFNotificationCenterRemoveObserver(center, endObserver, endNotification, nullptr);
    return 0;
}

