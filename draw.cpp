#include "stdafx.h"

void drawParticle(HDC hdc, Particle p) {
	int left = (int)floor((p.position.x - p.radius));
	int right = (int)floor((p.position.x + p.radius));
	int top = (int)floor((p.position.y - p.radius));
	int bottom = (int)floor((p.position.y + p.radius));

	Ellipse(hdc, left, top, right, bottom);
}

void drawGasContainer(HDC hdc) {
	for (int i = 0; i < gas_container.particles.size(); ++i)
		drawParticle(hdc, gas_container.particles[i]);
}