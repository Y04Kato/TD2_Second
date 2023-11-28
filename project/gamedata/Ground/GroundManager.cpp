#include "GroundManager.h"

void GroundManager::Initialize() {
	isGroundMove_ = false;

	for (int i = 0; i < 3; i++) {
		ground_[i] = make_unique<Ground>();
	}

	groundModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/ground", "Ground.obj"));
	ground_[0]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { /*40.0f*/4000.0f,3.0f,4.0f });
	ground_[1]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { /*40.0f*/4000.0f,3.0f,4.0f });
	ground_[2]->Initialize(groundModel_.get(), { 0.0f,-6.0f,0.0f }, { /*40.0f*/4000.0f,3.0f,4.0f });

	sideGroundModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/Block", "Block.obj"));

	for (int i = 0; i < 60; i++) {
		leftGround_[i] = make_unique<Ground>();
		leftGround_[i]->Initialize(sideGroundModel_.get(), { 15.0f * i,-6.0f,0.0f }, { /*40.0f*/10.0f,10.0f,10.0f });
	}
}

void GroundManager::Update() {
	OBJtoOBB();

	for (int i = 0; i < 3; i++) {
		ground_[i]->Update();
	}
	for (int i = 0; i < 60; i++) {
		leftGround_[i]->Update();
	}

	MovingGround();
}

void GroundManager::Draw(const ViewProjection& viewProjection) {
	for (int i = 0; i < 3; i++) {
		ground_[i]->Draw(viewProjection);
	}
	for (int i = 0; i < 60; i++) {
		leftGround_[i]->Draw(viewProjection);
	}
}

void GroundManager::OBJtoOBB() {
	for (int i = 0; i < 3; i++) {
		obb_[i].center = ground_[i]->GetWorldTransform().GetWorldPos();
		GetOrientations(MakeRotateXYZMatrix(ground_[i]->GetWorldTransform().rotation_), obb_[i].orientation);
		obb_[i].size = ground_[i]->GetWorldTransform().scale_;
	}
}

void GroundManager::MovingGround() {
	if (isGroundMove_ == true) {
		ground_[0]->SetTranslate(Lerp(ground_[0]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
		ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, 15.0f }, 1.0f));
		ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, { 0.0f, -6.0f, -15.0f }, 1.0f));
	}
	else if (isGroundMove_ == false) {
		//プレイヤーのいるレーンに合わせる
		switch (lane_) {
		case Obstacle::Lane::Left:
			ground_[0]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, 15.0f }, 1.0f));
			ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, { 0.0f, -6.0f, 15.0f }, 1.0f));
			break;
		case Obstacle::Lane::Middle:
			ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
			ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
			break;
		case Obstacle::Right:
			ground_[0]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, -15.0f }, 1.0f));
			ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, -15.0f }, 1.0f));
			break;
		}
	}
}

void GroundManager::Reset() {
	isGroundMove_ = false;
	ground_[0]->SetTranslate(Lerp(ground_[0]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
	ground_[1]->SetTranslate(Lerp(ground_[1]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
	ground_[2]->SetTranslate(Lerp(ground_[2]->GetWorldTransform().translation_, { 0.0f, -6.0f, 0.0f }, 1.0f));
}