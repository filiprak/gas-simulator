#include "stdafx.h"


GasContainer gas_container;


float rand_float(float low, float high) {
	if (high - low <= 0.0)
		return 0.0;
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

void initGasContainer(GasContainerType type, int p_num = 0, float p_radius = 0.0, float width = 0.0, float height = 0.0) {
	gas_container.width = width;
	gas_container.height = height;
	gas_container.num_particles = p_num;

	if (type == Right)
		return;

	srand(static_cast <unsigned> (time(0)));
	
	int part_per_width = (int)floor(width / (3 * p_radius));

	vec2 last_pos = vec2(p_radius, p_radius);
	// init particles
	int column = 0, row = 0;
	for (int i = 0; i < p_num; ++i) {
		if (i - row * part_per_width >= part_per_width) {
			row++;
			column = 0;
		}

		float vx = rand_float(-INITIAL_VELOCITY_MAX, INITIAL_VELOCITY_MAX);
		float vy = rand_float(-INITIAL_VELOCITY_MAX, INITIAL_VELOCITY_MAX);

		vec2 vel = vec2(vx, vy);
		vec2 pos = vec2(column * 3 * p_radius + 2 * p_radius, row * 3 * p_radius +  2 * p_radius);
		gas_container.particles.push_back(createParticle(p_radius, pos, vel, i));
		column++;
	}

}

void updateGasContainerSize(float width, float height) {
	gas_container.width = width;
	gas_container.height = height;
}

void addParticle(vec2 init_pos, vec2 init_vel, float radius, PState state) {
	static int id = 10000;
	Particle p;
	p.id = id++;
	p.position = init_pos;
	p.velocity = init_vel;
	p.radius = radius;
	gas_container.particles.push_back(p);
}

void removeParticle(int id) {
	std::vector<Particle>::iterator it;
	for (it = gas_container.particles.begin(); it != gas_container.particles.end(); ++it) {
		if (it->id == id) {
			gas_container.particles.erase(it);
			return;
		}
	}
}

Particle createParticle(float r, vec2 init_pos, vec2 vel, int id) {
	Particle p;
	p.radius = r;
	p.position = init_pos;
	p.velocity = vel;
	p.id = id;
	p.state = Inside;
	return p;
}

void slowParticles() {
	for (int i = 0; i < gas_container.particles.size(); ++i) {
		gas_container.particles[i].velocity *= 0.999;

	}
}

void runGasSimulation() {
	std::vector<Particle> next_state;

	for (int i = 0; i < gas_container.particles.size(); ++i) {
		Particle particle_copy = gas_container.particles[i];
		bounceParticle(particle_copy);
		particle_copy.position += particle_copy.velocity;
		if (particle_copy.state != Outside)
			next_state.push_back(particle_copy);
	}
	gas_container.particles = next_state;
	slowParticles();
}

void bounceParticle(Particle& p) {
	//DebugLog("p: [%f, %f] v[%f, %f]\n", p.position.x, p.position.y, p.velocity.x, p.velocity.y);
	for (int i = 0; i < gas_container.particles.size(); ++i) {
		Particle& collis_particle = gas_container.particles[i];
		if (p.id == collis_particle.id)
			continue;
		
		vec2 collis_vec = p.position - collis_particle.position;
		//DebugLog("length: %f, radius_sum: %f\n", length(collis_vec), p.radius + collis_particle.radius);
		if (length(collis_vec) <= p.radius + collis_particle.radius) {
			p.velocity += /*abs(diff) * */normalize(collis_vec);

			//DebugLog("collision dot1=%f, dot2=%f, diff: %f * [%f, %f]\n", dot1, dot2, diff, collis_vec.x, collis_vec.y);
		}
	}

	bool sharedStateChanged = setParticleState(p);
	
	if (p.position.x > gas_container.width - p.radius) {
		if (app_type == Right) {
			p.velocity.x = -p.velocity.x;
			p.position.x = gas_container.width - p.radius;
		}
		if (sharedStateChanged && p.state == JustLeft && app_type == Left) {
			notifyParticleEnter(MSG_FROM_LEFT, p.position, p.velocity, p.radius);
		}
	}

	if (p.position.x < p.radius) {
		if (app_type == Left) {
			p.velocity.x = -p.velocity.x;
			p.position.x = p.radius;
		}
		if (sharedStateChanged && p.state == JustLeft && app_type == Right){
			notifyParticleEnter(MSG_FROM_RIGHT, p.position, p.velocity, p.radius);
		}
	}

	if (p.position.y > gas_container.height - p.radius) {
		p.velocity.y = -p.velocity.y;
		p.position.y = gas_container.height - p.radius;
	}
	if (p.position.y < p.radius) {
		p.velocity.y = -p.velocity.y;
		p.position.y = p.radius;
	}
}

bool setParticleState(Particle& p) {
	bool res = false;
	PState prev_state = p.state;

	if (app_type == Left) {
		if (p.position.x <= gas_container.width - p.radius)
			p.state = Inside;
		else
			p.state = Outside;
	}
	else if (app_type == Right) {
		if (p.position.x >= p.radius)
			p.state = Inside;
		else
			p.state = Outside;
	}

	if (prev_state == Inside && p.state == Outside)
		p.state = JustLeft;

	return (prev_state != p.state);
}