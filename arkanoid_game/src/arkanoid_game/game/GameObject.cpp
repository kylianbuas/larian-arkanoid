#include "./GameObject.h"

#include "arkanoid_game/game/Game.h"

namespace arkanoid::game
{
	GameObject::GameObject(float a_fPosX, float a_fPosY) :
		m_fPosX(a_fPosX), m_fPosY(a_fPosY)
	{}

	void GameObject::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{

	}

	void GameObject::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
	{

	}

	void GameObject::Destroy()
	{
		m_bDestroyed = true;

		OnDestroyed();
	}

	bool GameObject::IsDestroyed() const
	{
		return m_bDestroyed;
	}

	int GameObject::GetPosX() const
	{
		return m_fPosX;
	}

	int GameObject::GetPosY() const
	{
		return m_fPosY;
	}
}