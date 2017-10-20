#pragma once

void DebugLog(const char* format, ...);

typedef enum {
	Left,
	Right
} GasContainerType;

extern GasContainerType app_type;