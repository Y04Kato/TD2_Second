#pragma once

#include "Ground.h"
#include "ViewProjection.h"
#include "Model.h"
#include "MatrixCalculation.h"
#include "Obstacle/Obstacle.h"

#include <memory>

using namespace std;

class GroundManager {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void OBJtoOBB();
	void MovingGround();

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

	/// <summary>
	/// グランドの場所の可変フラグを取得
	/// </summary>
	/// <returns>真:縦レーン 偽:横レーン</returns>
	bool GetFlag() { return isGroundMove_; }

	/// <summary>
	/// レーンを設定
	/// </summary>
	/// <param name="lane"></param>
	void SetLane(int lane) { lane_ = lane; };

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void SetSideGround();
	void SetSideGround2();
	void SetSideGround3();
	void SetSideGround4();
	void SetScroll(bool issideScroll) { isSideScroll_ = issideScroll; };

	float GetSideGround() { return rightGround_[29]->GetWorldTransform().translation_.num[0]; };
	float GetSideGround2() { return rightGround_[59]->GetWorldTransform().translation_.num[0]; };
	float GetSideGround3() { return rightGround2_[29]->GetWorldTransform().translation_.num[0]; };
	float GetSideGround4() { return rightGround2_[59]->GetWorldTransform().translation_.num[0]; };

private:
	static const int sideGroundNum = 60;

	unique_ptr<Ground> ground_[3];
	unique_ptr<Ground> rightGround_[sideGroundNum];
	unique_ptr<Ground> rightGround2_[sideGroundNum];
	unique_ptr<Ground> leftGround_[sideGroundNum];
	unique_ptr<Ground> leftGround2_[sideGroundNum];

	unique_ptr<Model> groundModel_;
	unique_ptr<Model> sideGroundModel_;
	OBB obb_[3];

	bool isGroundMove_;
	int lane_ = Obstacle::Lane::Middle;

	bool isSideScroll_ = true;
};