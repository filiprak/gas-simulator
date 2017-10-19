#include "stdafx.h"

UINT	MSG_FROM_LEFT,
		MSG_FROM_RIGHT;

bool registerMessages() {
	MSG_FROM_LEFT = RegisterWindowMessage(L"gas-simulation-left");
	MSG_FROM_RIGHT = RegisterWindowMessage(L"gas-simulation-right");

	if (MSG_FROM_LEFT == 0 || MSG_FROM_RIGHT == 0)
		return false;
	return true;
}

void notifyParticleEnter(UINT msg_type, vec2 pos, vec2 vel, float radius) {
	DWORD dwpos = (0xffff0000 & ((short)(radius * 10.0) << 16)) | (0x0000ffff & (short)pos.y);
	DWORD dwvel = (0xffff0000 & ((short)(vel.x * 10.0) << 16)) | (0x0000ffff & (short)(vel.y * 10.0));

	SendMessage(HWND_BROADCAST, msg_type, dwpos, dwvel);
	static int i = 0;
	DebugLog("Sended message %d\n", i++);
}