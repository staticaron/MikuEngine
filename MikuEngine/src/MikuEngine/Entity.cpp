#include "Entity.h"

namespace MikuEngine
{
	Entity::Entity( entt::entity entity, Scene* parentScene ) : m_Entity( entity ), m_ParentScene( parentScene ) {}

	Entity::Entity( UUID uuid ) {}
}
