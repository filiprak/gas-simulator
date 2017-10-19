#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "common.h"

using namespace glm;


typedef enum {
	Inside,
	Outside,
	JustLeft,
	Shared
} PState;

typedef struct {
	vec2 position;
	vec2 velocity;

	float radius;
	int id;

	PState state;

} Particle;


typedef struct {
	float width, height;

	int num_particles;
	std::vector<Particle> particles;

} GasContainer;

// particle container box
extern GasContainer gas_container;

void initGasContainer(GasContainerType type, int p_num, float p_radius, float width, float height);
void updateGasContainerSize(float width, float height);
bool setParticleState(Particle& p);
void addParticle(vec2 init_pos, vec2 init_vel, float radius, PState state);
void runGasSimulation();

Particle createParticle(float r, vec2 init_pos, vec2 vel, int id);