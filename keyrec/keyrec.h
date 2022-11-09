#ifndef __KEYREC_H__
#define __KEYREC_H__

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
// https://developer.apple.com/library/mac/documentation/Carbon/Reference/QuartzEventServicesRef/Reference/reference.html

static const char *logfileLocation = "./keyrec.csv";

struct CGEventContext {
	FILE         *logfile;
	CGEventFlags lastFlags;
	bool         keyStats[128];
};

CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
const char *convertKeyCode(int);

#endif
