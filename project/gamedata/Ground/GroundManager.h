#pragma once

#include "Ground.h"
#include "ViewProjection.h"
#include "Model.h"
#include "MatrixCalculation.h"

#include <memory>

using namespace std;

class GroundManager {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// OBBの取得
	/// </summary>
	/// <param name="num">取得したいOBBの配列番号</param>
	/// <returns>OBBの情報</returns>
	OBB GetOBB(int num) { return obb_[num]; }

	/// <summary>
	/// Groundの取得
	/// </summary>
	/// <param name="num">取得したいGroundの配列番号</param>
	/// <returns>Groundの情報</returns>
	Ground* GetGround(int num) { return ground_->get(); }
	
private:
	unique_ptr<Ground> ground_[4];
	unique_ptr<Model> groundModel_;
	OBB obb_[4];
};