#include "keyrec.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, const char *argv[]) {

	struct CGEventContext ctx;
	ctx.logfile = NULL;
	ctx.lastFlags = 0;
	for (int i = 0; i < 128; i++) {
		ctx.keyStats[i] = false;
	}

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

	// Open the logfile.
	ctx.logfile = fopen(logfileLocation, "w");

	if (!ctx.logfile) {
		fprintf(stderr, "ERROR: Unable to open log file. "
				"Ensure that you have the proper permissions.\n");
		exit(1);
	}
	fprintf(ctx.logfile, "sec\tmsec\tup/down\tkey\n");

	// Display the location of the logfile and start the loop.
	printf("Logging to: %s\n", logfileLocation);
	fflush(stdout);
	CFRunLoopRun();

	return 0;
}
