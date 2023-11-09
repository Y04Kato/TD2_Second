#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}

void DebugCamera::initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,2.7f,-50 };
	viewProjection_.rotation_ = { 0,0,0 };

	input_ = Input::GetInstance();
}

void DebugCamera::Update() {
#ifdef _DEBUG
	if (input_->PressKey(DIK_UPARROW)) {
		viewProjection_.rotation_.num[0] -= 0.05f;
	}
	if (input_->PressKey(DIK_DOWNARROW)) {
		viewProjection_.rotation_.num[0] += 0.05f;
	}
	if (input_->PressKey(DIK_RIGHTARROW)) {
		viewProjection_.rotation_.num[1] += 0.05f;
	}
	if (input_->PressKey(DIK_LEFTARROW)) {
		viewProjection_.rotation_.num[1] -= 0.05f;
	}

	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat3("rotation", viewProjection_.rotation_.num, -20, 20);
	ImGui::SliderFloat3("translation", viewProjection_.translation_.num, -50, 50);
	ImGui::End();
#endif

	if (isMovingCamera == true) {
		timer_ += 0.01f;
		if (timer_ >= endTimer_) {
			isMovingCamera = false;
			timer_ = endTimer_;
		}
		viewProjection_.translation_.num[0] = (1.0f - timer_) * movingStartTranslate_.num[0] + timer_ * movingEndTranslate_.num[0];
		viewProjection_.translation_.num[1] = (1.0f - timer_) * movingStartTranslate_.num[1] + timer_ * movingEndTranslate_.num[1];
		viewProjection_.translation_.num[2] = (1.0f - timer_) * movingStartTranslate_.num[2] + timer_ * movingEndTranslate_.num[2];
		viewProjection_.rotation_.num[0] = (1.0f - timer_) * movingStartRotate_.num[0] + timer_ * movingEndRotate_.num[0];
		viewProjection_.rotation_.num[1] = (1.0f - timer_) * movingStartRotate_.num[1] + timer_ * movingEndRotate_.num[1];
		viewProjection_.rotation_.num[2] = (1.0f - timer_) * movingStartRotate_.num[2] + timer_ * movingEndRotate_.num[2];
	}

	viewProjection_.UpdateMatrix();
}

void DebugCamera::ShakeCamera(int shakePower, int dividePower) {
	viewProjection_.translation_.num[0] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
	viewProjection_.translation_.num[1] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
	viewProjection_.translation_.num[2] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
}

void DebugCamera::SetCamera(Vector3 translation, Vector3 rotation) {
	viewProjection_.translation_ = translation;
	viewProjection_.rotation_ = rotation;
}

void DebugCamera::MovingCamera(Vector3 translation, Vector3 rotation, float timer) {
	timer_ = 0.0f;
	endTimer_ = timer;
	movingStartTranslate_ = viewProjection_.translation_;
	movingStartRotate_ = viewProjection_.rotation_;
	movingEndTranslate_ = translation;
	movingEndRotate_ = rotation;
	isMovingCamera = true;
}