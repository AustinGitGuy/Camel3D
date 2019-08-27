#pragma once

class Vector3;

class Camera {
	public:
		Camera(Vector3 pos, Vector3 direction);
		~Camera();

		void ResetCamera();
		void Move(int x, int y, int width, int height);
		void SetRotateObject(Vector3 pos);

		Vector3* pos;
		Vector3* rot;
		bool doRotate = false;
		int cameraX = 0;
		int cameraY = 0;

	private:
		Vector3* rotate;

		float angleX = 0.0;
		float angleY = 0.0;

		float rotateRad = 5;

		float phi = .2f;
		float theta = .2f;
};