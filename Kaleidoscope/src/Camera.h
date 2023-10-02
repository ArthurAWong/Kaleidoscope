#pragma once

class Camera {

public:
	Camera(Eigen::Vector3f cameraPos);
	~Camera();

private:
	Eigen::Vector3f cameraPos;
	Eigen::Vector3f cameraFront = { 0.0f, 0.0f, -1.0f };
	Eigen::Vector3f cameraUp = { 0.0f, 1.0f, 0.0f };
};