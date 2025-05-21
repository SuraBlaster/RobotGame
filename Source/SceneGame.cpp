#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include "SceneSelect.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyBomber.h"
#include "EnemyDrone.h"
#include "EnemySpider.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StageMoveFloor.h"
#include "SceneManager.h"
#include <Input/Input.h>
#include <WeaponManager.h>
#include <WeaponGreatSword.h>
#include <WeaponDagger.h>
#include "ItemCrystal.h"
#include "ItemManager.h"


void SceneGame::Initialize()
{
	//ステージ初期化
	heremap = SceneSelect::Instance().GetMap();
	StageManager& stageManager = StageManager::Instance();

	if(heremap==1)
	{
	StageManager& map1Manager = StageManager::Instance();
	
	map1Manager.Register(map1);

	}
	if(heremap==2)
	{
	StageManager& map2Manager = StageManager::Instance();
	
	map2Manager.Register(map2);
	}

	//プレイヤー初期化
	player = new Player;

	WeaponManager& weaponManager = WeaponManager::Instance();

	WeaponGreatSword* greatsword = new WeaponGreatSword;
	weaponManager.Register(greatsword);

	WeaponDagger* dagger = new WeaponDagger;
	weaponManager.Register(dagger);

	ItemManager& itemManager = ItemManager::Instance();
	ItemCrystal* crystal = new ItemCrystal();
	crystal->SetPosition(DirectX::XMFLOAT3(0, 1, 5));
	itemManager.Register(crystal);

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetParspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	//カメラコントローラー初期化
	cameraController = new CameraController;
	SceneGame::Instance().SetCameraController(cameraController);

	Mouse& mouse = Input::Instance().GetMouse();
	mouse.setCenter();
	cameraController->ZeroClear();

	//ゲージスプライト
	gauge = new Sprite();

	gameOverUI = std::make_unique<GameOverUI>();
	gameOverUI->Initialize();

	clearUI = std::make_unique<ClearUI>();
	clearUI->Initialize();

	shieldGauge = std::make_unique<ShieldGauge>();
	shieldGauge->Initialize();

	shieldIcon = std::make_unique<ShieldIcon>();
	shieldIcon->Initialize();

	UI = std::make_unique<UserInterface>();
	UI->Initialize();
	//2Dスプライト
	{
		screenWidth = static_cast<float>(graphics.GetScreenWidth());
		screenHeight = static_cast<float>(graphics.GetScreenHeight());

		sprite = std::make_unique<Sprite>();
		spriteSD = {
				0, 0, screenWidth, screenHeight,
				0, 0, 1, 1,
				0,
				0, 0, 0, 0.6f
		};

		toTitleSpr = std::make_unique<Sprite>("Data/Sprite/GoTitle.png");
		toTitleSD = {
			800,500, 250, 50,
			0, 0,
			static_cast<float>(toTitleSpr->GetTextureWidth()),
			static_cast<float>(toTitleSpr->GetTextureHeight()),
			0,
			1, 1, 1, 1.0f
		};

		backSpr = std::make_unique<Sprite>("Data/Sprite/Close.png");
		backSD = {
			300, 500, 150, 50,
			0, 0,
			static_cast<float>(backSpr->GetTextureWidth()),
			static_cast<float>(backSpr->GetTextureHeight()),
			0,
			1, 1, 1, 1.0f
		};
	}
	raund = 0;
	raundcase = 0;
	EnemySpider::Instance().SetDeadcount(0);
	EnemySlime::Instance().SetDeadcount(0);
	EnemyBomber::Instance().SetDeadcount(0);
	
	Rcase = 0;
}

// 終了化
void SceneGame::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	ItemManager::Instance().Clear();

	EnemyManager::Instance().Clear();

	WeaponManager::Instance().Clear();
  
	//カメラコントローラー終了処理
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//プレイヤー終了処理
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//ステージ終了処理
	StageManager::Instance().Clear();
	
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	if (!isPause)
	{
		//ステージ更新処理
		StageManager::Instance().Update(elapsedTime);

		//プレイヤー更新処理
		player->Update(elapsedTime);

		//エネミー更新処理
		EnemyManager::Instance().Update(elapsedTime);

		//アイテム更新処理
		ItemManager::Instance().Update(elapsedTime);

		//エフェクト更新処理
		EffectManager::Instance().Update(elapsedTime);

		//
		WeaponManager::Instance().Update(elapsedTime);

		//
		EnemyManager::Instance().Update(elapsedTime);
			isCameraControll = true;
			isOldCameraControll = true;
			UI->Update(elapsedTime);
	}
	else
	{
		if (isOldCameraControll)
		{
			isCameraControll = false;
			isOldCameraControll = false;
		}
	}

	if (Camera::Instance().GetClearFlag() ||
		Camera::Instance().GetOverFlag())
	{
		isCameraControll = false;
		isOldCameraControll = false;
	}

	Mouse& mouse = Input::Instance().GetMouse();
	mouse.updataNormal(isCameraControll);

	//カメラコントローラー更新処理
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	EffectManager::Instance().Update(elapsedTime);

	gameOverUI->Update(elapsedTime);
	clearUI->Update(elapsedTime);
	shieldGauge->Update(elapsedTime);
	if (!isCameraControll)
		cameraController->ZeroClear();
	pauseUpdate();

	//ラウンド管理
	RaundManage();

	killspider = EnemySpider::Instance().GetDeadcount();
	killbomber = EnemyBomber::Instance().GetDeadcount();
	killslime  =  EnemySlime::Instance().GetDeadcount();
	killcount = killbomber + killspider + killslime;

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラ初期設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//プレイヤー描画
		player->Render(dc, shader);

		WeaponManager::Instance().Render(dc, shader);

		if (!Player::Instance().GetHiddenFlag())
		{
			EnemyManager::Instance().Render(dc, shader);
		}
		
		ItemManager::Instance().Render(dc,shader);

		

		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	

	// 3Dデバッグ描画
	{
		//EnemyManager::Instance().DrawDebugPrimitive();

		//player->DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		gameOverUI->Render();

		clearUI->Render();

		if (!Player::Instance().GetHiddenFlag())
		{
			UI->Render(dc);

			shieldGauge->Render(dc);

			shieldIcon->Render();
		}

		pauseRender(dc);
	}

	// 2DデバッグGUI描画
	{
		player->DrawDebugGUI();

		EnemyManager::Instance().DrawDebugGUI();

		DrawDebugGUI();

	}
}

void SceneGame::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("SceneGame", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("KILL", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("killcount", &killcount);
			ImGui::InputInt("Bomberkillcount", &killbomber);
			ImGui::InputInt("Spiderkillcount", &killspider);
			ImGui::InputInt("Slimekillcount", &killslime);
		}
	}


	ImGui::End();
}

void SceneGame::pauseUpdate()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse mouse = Input::Instance().GetMouse();
	if (gamePad.GetButtonDown() & GamePad::BTN_BACK)
	{
		isPause = !isPause;
	}
	//タイトルに戻る
	if (mouse.mouseVsRect(toTitleSD.dx, toTitleSD.dy, toTitleSD.dw, toTitleSD.dh))
	{
		toTitleSD.r = toTitleSD.g = toTitleSD.b = 0.7f;
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}
	}
	else
	{
		toTitleSD.r = toTitleSD.g = toTitleSD.b = 1.0f;
	}
	//ゲームに戻る
	if (mouse.mouseVsRect(backSD.dx, backSD.dy, backSD.dw, backSD.dh))
	{
		backSD.r = backSD.g = backSD.b = 0.7f;
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			isPause = !isPause;
		}
	}
	else
	{
		backSD.r = backSD.g = backSD.b = 1.0f;
	}
}

void SceneGame::pauseRender(ID3D11DeviceContext* dc)
{
	if (isPause)
	{
		//タイトルスプライト描画
		sprite->Render(dc, spriteSD);

		toTitleSpr->Render(dc, toTitleSD);
		backSpr->Render(dc, backSD);
	}
}

void SceneGame::EnemySet()
{
	DirectX::XMFLOAT3 start = { 0,0,0 };
	DirectX::XMFLOAT3 end = { 0,0,0 };
	HitResult hit;
	EnemyManager& enemyManager = EnemyManager::Instance();
	
	
	if(heremap==1)
	{
		switch(raund)
		{
		case 0:
			for (int i = 0; i < 1; ++i)
			{
				EnemyDrone* drone = new EnemyDrone;
				drone->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 5));
				drone->SetTerritory(drone->GetPosition(), 10.0f);
				enemyManager.Register(drone);
			}
			

			/*for (int i = 0; i < 2; ++i)
			{
				EnemyBomber* bomber = new EnemyBomber;
				bomber->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySpider* spider = new EnemySpider;
				spider->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
				spider->SetTerritory(spider->GetPosition(), 10.0f);
				enemyManager.Register(spider);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySlime* slime = new EnemySlime;
				slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
				slime->SetTerritory(slime->GetPosition(), 10.0f);
				enemyManager.Register(slime);
			}*/
			break;
		case 1:
			for (int i = 0; i < 1; ++i)
			{
				EnemyBomber* bomber = new EnemyBomber;
				bomber->SetPosition(DirectX::XMFLOAT3(i * -10.0f, -28, 20));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySpider* bomber = new EnemySpider;
				bomber->SetPosition(DirectX::XMFLOAT3(i * 10.0f, -28, -20));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			break;
		}
	}
	else if(heremap==2)
	{
		switch (raund)
		{
		case 0:
			for (int i = 0; i < 1; ++i)
			{
				EnemyBomber* bomber = new EnemyBomber;
				bomber->SetPosition(DirectX::XMFLOAT3(0.0f, 0, i*2.0f));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySpider* spider = new EnemySpider;
				spider->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
				spider->SetTerritory(spider->GetPosition(), 10.0f);
				enemyManager.Register(spider);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySlime* slime = new EnemySlime;
				slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
				slime->SetTerritory(slime->GetPosition(), 10.0f);
				enemyManager.Register(slime);
			}
			break;
		case 1:
			for (int i = 0; i < 2; ++i)
			{
				EnemyBomber* bomber = new EnemyBomber;
				bomber->SetPosition(DirectX::XMFLOAT3(0.0f, 0, i*2.0f));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySpider* spider = new EnemySpider;
				spider->SetPosition(DirectX::XMFLOAT3(0.0f, 0, i*1.0f));
				spider->SetTerritory(spider->GetPosition(), 10.0f);
				enemyManager.Register(spider);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySlime* slime = new EnemySlime;
				slime->SetPosition(DirectX::XMFLOAT3(0.0f, 0, i*3.0f));
				slime->SetTerritory(slime->GetPosition(), 10.0f);
				enemyManager.Register(slime);
			}
			break;
		case 2:
			for (int i = 0; i < 3; ++i)
			{
				EnemyBomber* bomber = new EnemyBomber;
				bomber->SetPosition(DirectX::XMFLOAT3(95.0f, 0, i*2.0f));
				bomber->SetTerritory(bomber->GetPosition(), 10.0f);
				enemyManager.Register(bomber);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySpider* spider = new EnemySpider;
				spider->SetPosition(DirectX::XMFLOAT3(95.0f, 0, i*1.0f));
				spider->SetTerritory(spider->GetPosition(), 10.0f);
				enemyManager.Register(spider);
			}
			for (int i = 0; i < 1; ++i)
			{
				EnemySlime* slime = new EnemySlime;
				slime->SetPosition(DirectX::XMFLOAT3(95.0f, 0, i*3.0f));
				slime->SetTerritory(slime->GetPosition(), 10.0f);
				enemyManager.Register(slime);
			}
			break;
		}
	}

}

void SceneGame::RaundManage()
{
	switch(raundcase)
	{
	case 0:
		EnemySet();
		raundcase++;
	case 1:
		
		switch(Rcase)
		{
		case 0:
			if (killcount>=5)
			{
				if (heremap == 1)
				{
					map1->OpenDoor();
					EnemySet();
					
				}
				if (heremap == 2)
				{
					map2->OpenDoor1();
					EnemySet();
					
				}
				raund++;
				Rcase++;
			}
			break;
		case 1:
			if(killcount>=10)
			{
			if(heremap==1)
			{
			//マップ２クリア

			}
			if(heremap==2)
			{
				map2->OpenDoor2();
				EnemySet();
				raund++;
			}
			Rcase++;
			}
			break;
		case 2:
			if(killcount==144)
			{
			if (heremap == 2)
			{
				//マップ２クリア

			}
			}
			
		}
		
	}

	
	

}