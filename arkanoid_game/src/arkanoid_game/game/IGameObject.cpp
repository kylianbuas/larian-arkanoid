#include "./IGameObject.h"

#include "arkanoid_game/game/Game.h"

namespace arkanoid::game
{
	IGameObject::IGameObject(float a_fPosX, float a_fPosY) :
		m_fPosX(a_fPosX), m_fPosY(a_fPosY)
	{}

	void IGameObject::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{

	}

	void IGameObject::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
	{

	}

	void IGameObject::Destroy(bool a_bForce)
	{
		m_bDestroyed = true;

		if (!a_bForce)
		{
			OnDestroyed();
		}
	}

	bool IGameObject::IsDestroyed() const
	{
		return m_bDestroyed;
	}

	int IGameObject::GetPosX() const
	{
		return m_fPosX;
	}

	int IGameObject::GetPosY() const
	{
		return m_fPosY;
	}
}