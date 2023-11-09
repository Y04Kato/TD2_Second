#pragma once
#include "ViewProjection.h"
#include "Input.h"

class DebugCamera{
public:
	static DebugCamera* GetInstance();

	void initialize();

	void Update();

	void ShakeCamera(int shakePower,int dividePower);

	void SetCamera(Vector3 translation, Vector3 rotation);

	void MovingCamera(Vector3 translation, Vector3 rotation,float timer);

	ViewProjection* GetViewProjection() { return &viewProjection_; }

private:
	ViewProjection viewProjection_;

	Input* input_;

	bool isMovingCamera = false;
	Vector3 movingStartTranslate_;
	Vector3 movingStartRotate_;
	Vector3 movingEndTranslate_;
	Vector3 movingEndRotate_;
	float timer_ = 0.0f;
	float endTimer_ = 0.0f;
};
