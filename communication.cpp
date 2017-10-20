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

void handleMsgFromLeft(WPARAM wParam, LPARAM lParam) {
	float radius = (float)(short)HIWORD(wParam) / 10.0;
	vec2 pos = vec2(radius, (short)LOWORD(wParam));
	vec2 vel = vec2((float)((short)HIWORD(lParam)) / 10.0, (float)((short)LOWORD(lParam)) / 10.0);
	addParticle(pos, vel, radius, Inside);
}

void handleMsgFromRight(WPARAM wParam, LPARAM lParam) {
	float radius = (float)(short)HIWORD(wParam) / 10.0;
	vec2 pos = vec2(gas_container.width - radius, (short)LOWORD(wParam));
	vec2 vel = vec2((float)((short)HIWORD(lParam)) / 10.0, (float)((short)LOWORD(lParam)) / 10.0);
	addParticle(pos, vel, radius, Inside);
}