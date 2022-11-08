#include "keyrec.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


int main(int argc, const char *argv[]) {

	struct CGEventContext ctx;
	ctx.logfile = NULL;
	ctx.lastFlags = 0;
	for (int i = 0; i < 128; i++) {
		ctx.keyStats[i] = false;
	}
	struct tm utcStart = {0};
	utcStart.tm_hour = 0;
	utcStart.tm_min = 0;
	utcStart.tm_sec = 0;
	utcStart.tm_year = 70;
	utcStart.tm_mon = 0;
	utcStart.tm_mday = 1;
	ctx.utcStart = mktime(&utcStart);

    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask =
		CGEventMaskBit(kCGEventKeyDown)
		| CGEventMaskBit(kCGEventKeyUp)
		| CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
			kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask,
			CGEventCallback, &ctx
    	);

    // Exit the program if unable to create the event tap.
    if (!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        exit(1);
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource =
		CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(
			CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes
		);
    CGEventTapEnable(eventTap, true);


    // Clear the logfile if clear argument used or log to specific file if
	// given.
    if (argc == 2) {
		logfileLocation = argv[1];
    }

    // Get the current time and open the logfile.
    time_t result = time(NULL);
    ctx.logfile = fopen(logfileLocation, "w");

    if (!ctx.logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. "
				"Ensure that you have the proper permissions.\n");
        exit(1);
    }

    // Display the location of the logfile and start the loop.
    printf("Logging to: %s\n", logfileLocation);
    fflush(stdout);
    CFRunLoopRun();

    return 0;
}
