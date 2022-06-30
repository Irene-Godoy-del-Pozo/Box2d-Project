#include "ParticleSystem.h"
#include <math.h>
#define PI 3.1415926

ParticleSystem::ParticleSystem()
{
	//Se configuran los valores iniciales del sistema
	m_particles.resize(count);
	m_shapes.resize(count);
	m_emiterLifetime = sf::seconds(6);
	m_emitter.x = 0;
	m_emitter.y = 0;
	
	//Se configura la forma de los circulos
	for (int i = 0; i < m_shapes.size(); i++)
	{
		m_shapes[i].setRadius(rand() % 10);
		m_shapes[i].setTextureRect(sf::IntRect(0, 0, 56, 56));
	}
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::SetEmitter(sf::Vector2f position)
{
	m_emitter = position;
	
}

void ParticleSystem::Update(sf::Time deltaTime)
{
	if (timer < .5f)
		timer += deltaTime.asSeconds();


	for (std::size_t i = 0; i < m_particles.size(); ++i)
	{
		// Se actualiza el tiempo de vida de la particula
		Particle& p = m_particles[i];
		p.lifetime -= deltaTime;	
		p.timePassed += deltaTime;

		// Resetea la particula si se ha acabado su tiempo de vida
		if (p.lifetime <= sf::Time::Zero)
			ResetParticle(i);

		// Setea aceleracion

		p.velAcc = 0;
		p.growsSpeed = 0;
		p.rotAcc = 0;

 
		// Posicion
		p.velocity -= sf::Vector2f(p.velAcc * deltaTime.asMilliseconds() * std::cos(p.angle), p.velAcc * deltaTime.asMilliseconds() * std::sin(p.angle));
		sf::Vector2f lastFramePos = m_shapes[i].getPosition();
		sf::Vector2f newPos = lastFramePos - p.velocity * deltaTime.asSeconds();
		m_shapes[i].setPosition(newPos);

		//Tamaño
		p.size -= sf::Vector2f(p.growsSpeed * deltaTime.asSeconds(), p.growsSpeed * deltaTime.asSeconds());
		m_shapes[i].setScale(p.size);

		// Angulo
		p.rotation += p.rotAcc * deltaTime.asSeconds();
		m_shapes[i].rotate(p.rotation);

		// Alfa y color
		float ratio = p.lifetime.asSeconds() / m_emiterLifetime.asSeconds()+10;
		m_shapes[i].setFillColor(sf::Color(252, 132 +(1000* p.timePassed.asSeconds()),5, static_cast<sf::Uint8>(ratio * 255)));
	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	states.transform *= getTransform();

	for(int i =0; i < m_shapes.size(); i++)
		target.draw(m_shapes[i], states);
}

void ParticleSystem::ResetParticle(std::size_t index)
{
	// Angulos y velocidades aleatorias
	float angle = std::rand() % emiterAngle * 3.14f / 180.f;
	float speed = (std::rand() % 50) + initialSpeed;

	// Contador a cero
	m_particles[index].timePassed = sf::Time::Zero;
	// Velocidad inicial
	m_particles[index].velocity = sf::Vector2f(cos(angle) * speed , std::sin(angle)  * speed);
	// Angulo inicial
	m_particles[index].angle = 0.5*PI/180;

	// Rotacion inicial
	m_particles[index].rotation = 0.5 * PI / 180;

	// Aceleracion inicial
	m_particles[index].velAcc = 0;

	// Rotacion inicial
	m_particles[index].rotAcc = 0;

	// Velocidad de crecimiento inicial
	m_particles[index].growsSpeed = -1;

	// Resetea la escala
	m_particles[index].size = sf::Vector2f(1.0f, 1.0f);
	m_shapes[index].setScale(m_particles[index].size);

	// Tiempo de vida
	m_particles[index].lifetime = sf::milliseconds((std::rand() % 350) );

	// Resetea posicion
	m_shapes[index].setPosition(m_emitter);


}



int ParticleSystem::GetParticleCount()
{
	return count;
}

void ParticleSystem::SetEmitAngle(int emiterAngle)
{
	this->emiterAngle = emiterAngle;
}

void ParticleSystem::SetInitialSpeed(float initialSpeed)
{
	this->initialSpeed = initialSpeed;
}





