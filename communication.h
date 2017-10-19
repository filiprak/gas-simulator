#pragma once

extern UINT MSG_FROM_LEFT;
extern UINT MSG_FROM_RIGHT;

bool registerMessages();
void notifyParticleEnter(UINT msg_type, vec2 pos, vec2 vel, float radius);