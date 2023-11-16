#pragma once

#include "Enemy.h"
#include "ViewProjection.h"
#include "Model.h"
#include "MatrixCalculation.h"

#include <memory>

using namespace std;

class EnemyManager {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	
	void SetPlayerPosition(const Vector3& position);

	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };
private:
	unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> model_;
	OBB obb_;
	bool isSideScroll_ = true;
};