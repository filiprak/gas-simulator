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

	SendMessageCallback(HWND_BROADCAST, msg_type, dwpos, dwvel, NULL, NULL);
	static int i = 0;
}