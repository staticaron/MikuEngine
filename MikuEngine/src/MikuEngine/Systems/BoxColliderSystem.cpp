#include "Systems/BoxColliderSystem.h"

#include "Components.h"
#include "Entity.h"

namespace MikuEngine
{
	bool BoxColliderSystem::IsColliding( Entity entity1, Entity entity2 )
	{
		auto boxCollider1 = entity1.GetComponent<BoxColliderComponent>();
		auto boxCollider2 = entity2.GetComponent<BoxColliderComponent>();

		auto transform1 = entity1.GetComponent<TransformComponent>();
		auto transform2 = entity2.GetComponent<TransformComponent>();

		bool horizontalOverlap = transform1.Position.x + boxCollider1.m_Bounds.x > transform2.Position.x - boxCollider2.m_Bounds.x && transform1.Position.x - boxCollider1.m_Bounds.x < transform2.Position.x + boxCollider2.m_Bounds.x;
		bool verticalOverlap = transform1.Position.y + boxCollider1.m_Bounds.y > transform2.Position.y - boxCollider2.m_Bounds.y && transform1.Position.y - boxCollider1.m_Bounds.y < transform2.Position.y + boxCollider2.m_Bounds.y;

		return horizontalOverlap && verticalOverlap;
	}
}
