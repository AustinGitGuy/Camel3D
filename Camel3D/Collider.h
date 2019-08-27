#pragma once

class Vector3;
class GameObject;


class Collider {

	public:
		Collider(){};
		Collider(Vector3* pos, Vector3* size){};
		virtual ~Collider(){};
		virtual void DrawCollider(){};
		virtual void RegenerateCollider(Vector3* pos, Vector3* size){};
		virtual bool CheckCollision(GameObject* obj){return false;}
		virtual bool CheckCollision(Collider* col){return false;}

	protected:
		Vector3* min;
		Vector3* max;

		friend class BoxCollider;

};