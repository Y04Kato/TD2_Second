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
	
	/// <summary>
	/// グランドの場所の可変フラグ
	/// </summary>
	/// <param name="flag">移動フラグ</param>
	/// <returns>真:縦レーン 偽:横レーン</returns>
	void SetFlag(bool flag) { isGroundMove_ = flag; }

private:
	unique_ptr<Ground> ground_[3];
	unique_ptr<Model> groundModel_;
	OBB obb_[3];

	bool isGroundMove_;
};