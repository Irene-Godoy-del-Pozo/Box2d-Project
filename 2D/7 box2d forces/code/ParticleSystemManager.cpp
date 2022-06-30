#include "ParticleSystemManager.h"

ParticleSystemManager::ParticleSystemManager(sf::RenderWindow* window)
{
		
}

ParticleSystemManager::~ParticleSystemManager()
{

}

void ParticleSystemManager::Update(sf::Time deltaTime)
{
	
	int temp = 0;

	//Update de todas las particulas
	for (std::size_t i = 0; i < m_particleSystem.size(); ++i)
	{
		m_particleSystem[i].Update(deltaTime);
		temp += m_particleSystem[i].GetParticleCount();
	}
	particlesCount = temp;

}

void ParticleSystemManager::Draw(sf::RenderWindow* window)
{
	//Se dibujan todas las particulas
	for (std::size_t i = 0; i < m_particleSystem.size(); ++i)
		window->draw(m_particleSystem[i]);

}

void ParticleSystemManager::AddParticleSystem(sf::Vector2f position)
{
	//Se crea y configura el sistema de particulas
	ParticleSystem* particleSystemToAdd = new ParticleSystem();

	particleSystemToAdd->SetEmitter(position);
	particleSystemToAdd->SetEmitAngle(rand() % 360);
	particleSystemToAdd->SetInitialSpeed(rand() % 200);
	m_particleSystem.emplace_back(*particleSystemToAdd);
}


