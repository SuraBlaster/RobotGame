#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "ShieldGauge.h"
#include "ShieldIcon.h"
#include "UI.h"
#include "ClearUI.h"
#include "GameOverUI.h"

#include "FadeOut.h"

#include "Map1.h"
#include "Map2.h"



// ゲームシーン
class SceneGame : public Scene
{
public:
	static SceneGame& Instance()
	{
		static SceneGame instance;
		return instance;
	}

	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;


	CameraController* GetCameraController() { return cameraController; }

	void SetCameraController(CameraController* controller) { cameraController = controller; }


	//敵配置
	void EnemySet();

	//敵位置
	DirectX::XMFLOAT3 enemyslimeposi;
	DirectX::XMFLOAT3 enemybomberposi;
	DirectX::XMFLOAT3 enemyspiderposi;
	//ゴールクリスタル設置
	void CrystalSet();
	//クリスタル位置
	DirectX::XMFLOAT3 CrystalPosition;
	//IMGUIの表示
	void DrawDebugGUI();
	//ラウンド管理（敵を一定数倒したら次のラウンドに移り、扉が開く（マップのアニメーションを再生））
	void RaundManage();

	bool Oncrystal;
	int heremap;

	int raund;
	int raundcase;
	Map1* map1 = new Map1();
	Map2* map2 = new Map2();

	float enemytimer;

	int killcount;
	int killspider;
	int killbomber;
	int killslime;
	int killdrone;
	
	DirectX::XMFLOAT3 playernowpos;
	float canopendoor2 = 40;
private:

	void pauseUpdate();
	void pauseRender(ID3D11DeviceContext* dc);

private:
	bool isCameraControll = true;
	bool isOldCameraControll = true;
	bool isPause = false;
	bool isUIAnimation = false;
	int UITimer = 20;

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	float screenWidth, screenHeight,
		  textureWidth, textureHeight;

	Sprite* gauge = nullptr;

	std::unique_ptr<ShieldGauge> shieldGauge = nullptr;

	std::unique_ptr<ShieldIcon> shieldIcon = nullptr;

	std::unique_ptr<Sprite> toTitleSpr = nullptr;
	std::unique_ptr<Sprite> backSpr = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;

	SpriteData spriteSD;
	SpriteData toTitleSD;
	SpriteData backSD;

	std::unique_ptr<UserInterface> UI = nullptr;
	//BGM再生
	std::unique_ptr<AudioSource> MainBGM;
};
