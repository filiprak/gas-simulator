#include "stdafx.h"

void drawParticle(HDC hdc, Particle p) {
	int left = (int)floor((p.position.x - p.radius) / PIXEL_SIZE);
	int right = (int)floor((p.position.x + p.radius) / PIXEL_SIZE);
	int top = (int)floor((p.position.y - p.radius) / PIXEL_SIZE);
	int bottom = (int)floor((p.position.y + p.radius) / PIXEL_SIZE);

	Ellipse(hdc, left, top, right, bottom);
}

void drawGasContainer(HDC hdc) {
	std::vector<Particle>::iterator it;
	for (int i = 0; i < gas_container.particles.size(); ++i)
		drawParticle(hdc, gas_container.particles[i]);
}