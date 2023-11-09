#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"

#include <memory>

class Ground {
public:
	void Initialize(Model* model, Vector3 translation, Vector3 scale);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetTranslate(const Vector3& translate) { worldTransform_.translation_ = translate; }

private:
	TextureManager* textureManager_;
	WorldTransform worldTransform_;
	Model* groundModel_;
	uint32_t textureHandle_;
};