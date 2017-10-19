#pragma once
#include "gas_model.h"
#include "windows.h"

void drawParticle(HDC hdc, Particle p);
void drawGasContainer(HDC hdc);
void bounceParticle(Particle& p);