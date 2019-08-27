#pragma once
#include "Collider.h"

class BoxCollider : public Collider {

	public:
		BoxCollider(Vector3* pos, Vector3* size);
		~BoxCollider();
		void DrawCollider();
		void RegenerateCollider(Vector3* pos, Vector3* size);
		bool CheckCollision(GameObject* obj);
		bool CheckCollision(Collider* col);

};