#pragma once
#include "CJEngine.h"
#include "DirectXCommon.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "DirectionalLight.h"
#include <wrl.h>
#include <random>
#include <numbers>

class CreateParticle {
public:
	void Initialize(int kNumInstance, Emitter emitter, AccelerationField accelerationField, uint32_t textureIndex);
	void Update();
	void Finalize();
	void Draw(const ViewProjection& viewProjection);

	Particle MakeNewParticle(std::mt19937& randomEngine, const Transform transform);
	std::list<Particle> Emission(const Emitter& emitter, std::mt19937& randomEngine);

	void SetTranslate(Vector3 transform) { emitter_.transform.translate = transform; }
	void SetEmitter(const Emitter& emitter) { emitter_ = emitter; };
	void SetAccelerationField(const AccelerationField& accelerationField) { accelerationField_ = accelerationField; };

	void SetFrequency(float frequency) { emitter_.frequency = frequency; }
	void SetCount(int count) { emitter_.count = count; }
	void SetFired(AccelerationField field) { field = accelerationField_; }
	void SetColor(Vector4 color) {
		color_ = color;
		isColor = true;
	}
private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	uint32_t LoadBuffer(int kNumInstance);
	void LoadBuffer(uint32_t index, int kNumInstance);

private:
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource>instancingResource_ = nullptr;
	VertexData* vertexData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;

	//パーティクルの数
	int kNumMaxInstance_;
	int numInstance_;
	//パーティクルの数と同じ数のTransform
	std::list<Particle> particles_;
	//Δtime 現在は60FPS固定
	const float kDeltaTime = 1.0f / 60.0f;

	DirectionalLights* directionalLights_;
	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;

	uint32_t bufferIndex_;
	ParticleForGPU* instancingData_ = nullptr;

	bool isBillBoard_ = true;

	std::random_device seedGenerator;

	Vector4 color_;
	bool isColor = false;

	Emitter emitter_{};
	AccelerationField accelerationField_;
};
