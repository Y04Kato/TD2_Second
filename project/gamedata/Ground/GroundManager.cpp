#include "GroundManager.h"

void GroundManager::Initialize() {
	isGroundMove_ = false;

	for (int i = 0; i < 3; i++) {
		ground_[i] = make_unique<Ground>();
	}

	groundModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/ground", "CUBE.obj"));
	ground_[0]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { 20.0f,3.0f,4.0f });
	ground_[1]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { 20.0f,3.0f,4.0f });
	ground_[2]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { 20.0f,3.0f,4.0f });
}

void GroundManager::Update() {
	for (int i = 0; i < 3; i++) {
		obb_[i].center = ground_[i]->GetWorldTransform().GetWorldPos();
		GetOrientations(MakeRotateXYZMatrix(ground_[i]->GetWorldTransform().rotation_), obb_[i].orientation);
		obb_[i].size = ground_[i]->GetWorldTransform().scale_;
	}

	for (int i = 0; i < 3; i++) {
		ground_[i]->Update();
	}

	if (isGroundMove_ == true) {
		ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, {0.0f, -6.0f, 15.0f}, 1.0f));
		ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, {0.0f, -6.0f, -15.0f}, 1.0f));
	}
	else if (isGroundMove_ == false) {
		ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
		ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
	}
}

void GroundManager::Draw(const ViewProjection& viewProjection) {
	for (int i = 0; i < 3; i++) {
		ground_[i]->Draw(viewProjection);
	}
}