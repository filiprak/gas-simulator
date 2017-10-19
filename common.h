#pragma once

#define PIXEL_SIZE		1.0
void DebugLog(const char* format, ...);

typedef enum {
	Left,
	Right
} GasContainerType;

extern GasContainerType app_type;