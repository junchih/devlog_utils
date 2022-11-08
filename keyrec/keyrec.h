#ifndef __KEYREC_H__
#define __KEYREC_H__

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
// https://developer.apple.com/library/mac/documentation/Carbon/Reference/QuartzEventServicesRef/Reference/reference.html
#include <time.h>

static const char *logfileLocation = "./keyrec.log";

struct CGEventContext {
	FILE         *logfile;
	CGEventFlags lastFlags;
	bool         keyStats[128];
	time_t       utcStart;
};

CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
const char *convertKeyCode(int);

#endif
