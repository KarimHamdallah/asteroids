#pragma once
#include <glm/glm.hpp>
#include <engine/defines.h>


enum class particleType
{
	Quad = 0,
	Circle = 1,
	AABB = 2
};

struct particel_properties
{
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec2 startScale, endScale, scaleVariation;
	color startColor, endColor;
	f32 lifeTime = 1.0f;
	particleType type = particleType::AABB;
};

class particle_system
{
public:
	particle_system();

	void update();
	void render();

	void push(const particel_properties& particleProps, u32 particles_count);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		color ColorBegin, ColorEnd;
		f32 Rotation = 0.0f;
		glm::vec2 SizeBegin, SizeEnd;

		f32 LifeTime = 1.0f;
		f32 LifeRemaining = 0.0f;

		particleType ParticleType;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	u32 m_PoolIndex = 999;
};


inline particel_properties blue_vfx(const glm::vec2& position)
{
	particel_properties props;
	props.startColor = BLUE; /*RGB_COLOR(161, 160, 157);*/
	props.endColor = WHITE;
	props.startScale = glm::vec2(10.0f);
	props.endScale = glm::vec2(1.0f);
	props.lifeTime = 0.5f;
	props.position = position;
	props.scaleVariation = glm::vec2(50.0f);
	props.velocity = glm::vec2(0.0f, 0.0f);
	props.velocityVariation = glm::vec2(500.0f);

	return props;
}



inline particel_properties smoke_vfx(const glm::vec2& position)
{
	particel_properties props;
	props.startColor = RGB_COLOR(161, 160, 157);
	props.endColor = WHITE;
	props.startScale = glm::vec2(10.0f);
	props.endScale = glm::vec2(1.0f);
	props.lifeTime = 0.5f;
	props.position = position;
	props.scaleVariation = glm::vec2(50.0f);
	props.velocity = glm::vec2(0.0f, 0.0f);
	props.velocityVariation = glm::vec2(500.0f);

	return props;
}