#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include <functional>
#include "ResourceManager.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::UpdateAnimation(float elapsedTime)
{
	//再生中でないなら処理しない
	if (!IsPlayAnimation())return;

	//ブレンド率の計算
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		blendRate = animationBlendTime / animationBlendSeconds;
		animationBlendTime += elapsedTime;
	}


	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				if (blendRate < 1.0f)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					// スケールの補間
					node.scale = DirectX::XMFLOAT3(
						key0.scale.x + (key1.scale.x - key0.scale.x) * blendRate,
						key0.scale.y + (key1.scale.y - key0.scale.y) * blendRate,
						key0.scale.z + (key1.scale.z - key0.scale.z) * blendRate
					);

					// 回転の補間
					DirectX::XMVECTOR quat0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR quat = DirectX::XMQuaternionSlerp(quat0, quat1, blendRate);
					DirectX::XMStoreFloat4(&node.rotate, quat);

					// 平行移動の補間
					node.translate = DirectX::XMFLOAT3(
						key0.translate.x + (key1.translate.x - key0.translate.x) * blendRate,
						key0.translate.y + (key1.translate.y - key0.translate.y) * blendRate,
						key0.translate.z + (key1.translate.z - key0.translate.z) * blendRate
					);
				}
				else
				{
					// スケールの補間
					node.scale = DirectX::XMFLOAT3(
						key0.scale.x + (key1.scale.x - key0.scale.x) * rate,
						key0.scale.y + (key1.scale.y - key0.scale.y) * rate,
						key0.scale.z + (key1.scale.z - key0.scale.z) * rate
					);

					// 回転の補間
					DirectX::XMVECTOR quat0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR quat = DirectX::XMQuaternionSlerp(quat0, quat1, rate);
					DirectX::XMStoreFloat4(&node.rotate, quat);

					// 平行移動の補間
					node.translate = DirectX::XMFLOAT3(
						key0.translate.x + (key1.translate.x - key0.translate.x) * rate,
						key0.translate.y + (key1.translate.y - key0.translate.y) * rate,
						key0.translate.z + (key1.translate.z - key0.translate.z) * rate
					);
				}
	
			}
			break;
		}
	}
	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationLoopFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//時間経過
	currentAnimationSeconds += elapsedTime;

	//再生時間が経過時間を超えたら
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoopFlag)
		{
			//再生時間を巻き戻す
			currentAnimationSeconds -= animation.secondsLength;
		}
		else
		{
			//再生終了時間にする
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}

}

void Model::PlayAnimation(int index,bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0)return false;
	if (currentAnimationIndex >= resource->GetAnimations().size())return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{

	// すべてのルートノードで検索を開始
	for (Node& node : nodes) {
		if (strcmp(node.name, name) == 0)
		{
			return &node;
		}
		
	}

	//見つからなかった
	return nullptr;
}
