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
#include "StageMain.h"
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
	//BGM初期化
	Audio& audio = audio.Instance();
	//BGM設定
	MainBGM = audio.LoadAudioSource("Data/Audio/BGM/Main.wav");
	//BGM音量設定(100以下)
	MainBGM->sourceVoice->SetVolume(50);
	//ステージ初期化
	heremap = SceneSelect::Instance().GetMap();
	StageManager& stageManager = StageManager::Instance();

	if (heremap == 1)
	{
		StageManager& map1Manager = StageManager::Instance();

		map1Manager.Register(map1);

		CrystalPosition = {-20, -28, 0};

	}
	if (heremap == 2)
	{
		StageManager& map2Manager = StageManager::Instance();

		map2Manager.Register(map2);

		CrystalPosition = { 95, -10, 0 };
	}
	Oncrystal = false;
	playernowpos = { 0,0,0 };

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);

	//プレイヤー初期化
	player = new Player;

	WeaponManager& weaponManager = WeaponManager::Instance();

	WeaponGreatSword* greatsword = new WeaponGreatSword;
	weaponManager.Register(greatsword);

	WeaponDagger* dagger = new WeaponDagger;
	weaponManager.Register(dagger);


	//エネミー初期化

	enemyslimeposi = { 0,0,0 };
	enemybomberposi = { 0,0,0 };
	enemyspiderposi = { 0,0,0 };
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 1; ++i)
	{
		EnemyDrone* slime = new EnemyDrone;

		/*EnemyManager& enemyManager = EnemyManager::Instance();
		for (int i = 0; i < 1; ++i)
		{
			EnemyBomber* slime = new EnemyBomber;

			slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
			slime->SetTerritory(slime->GetPosition(), 10.0f);
			enemyManager.Register(slime);
		}


		//for (int i = 0; i < 1; ++i)
		//{
		//	EnemySlime* slime = new EnemySlime;
		//	slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		//	slime->SetTerritory(slime->GetPosition(), 10.0f);
		//	enemyManager.Register(slime);
		//}
		//EnemyManager& enemyspiderManager = EnemyManager::Instance();
		//for (int i = 0; i < 1; ++i)
		//{
		//	EnemySpider* spider = new EnemySpider;
		//	spider->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 10));
		//	spider->SetTerritory(spider->GetPosition(), 10.0f);
		//	enemyspiderManager.Register(spider);
		//}

		ItemManager& itemManager = ItemManager::Instance();
		ItemCrystal* crystal = new ItemCrystal();
		crystal->SetPosition(DirectX::XMFLOAT3(0, 1, 5));
		itemManager.Register(crystal);


		for (int i = 0; i < 1; ++i)
		{
			EnemySlime* slime = new EnemySlime;
			slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
			slime->SetTerritory(slime->GetPosition(), 10.0f);
			enemyManager.Register(slime);
		}
		EnemyManager& enemyspiderManager = EnemyManager::Instance();
		for (int i = 0; i < 1; ++i)
		{
			EnemySpider* spider = new EnemySpider;
			spider->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 10));
			spider->SetTerritory(spider->GetPosition(), 10.0f);
			enemyManager.Register(spider);
		}*/


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

		fadeOut = std::make_unique<FadeOut>();
		fadeOut->Initialize();

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

		
	}
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
	//エネミー終了処理
	/*EnemyBomber::Instance().~EnemyBomber();
	EnemySpider::Instance().~EnemySpider();
	EnemySlime::Instance().~EnemySlime();
	*/
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//BGM再生
	MainBGM->Play(true);
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

		//����X�V����
		WeaponManager::Instance().Update(elapsedTime);

		//�G�l�~�[�X�V����
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

	Mouse& mouse = Input::Instance().GetMouse();
	mouse.updataNormal(isCameraControll);

	//カメラコントローラー更新処理
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	EffectManager::Instance().Update(elapsedTime);

	fadeOut->Update(elapsedTime);
	shieldGauge->Update(elapsedTime);
	if (!isCameraControll)
		cameraController->ZeroClear();
	pauseUpdate();

	//マップ、ラウンドごとの敵の出現位置設定
	if(heremap==1)
	{
		if(raund==0)
		{
			enemyslimeposi = { 2,-8,2 };
			enemybomberposi = { 4,-8,4 };
			enemyspiderposi = { 6,-8,6 };
		}
		if (raund==1)
		{
			enemyslimeposi = { -2,0,-2 };
			enemybomberposi = { -4,0,-4 };
			enemyspiderposi = { 6,0,6 };
		}
		if (raund==2)
		{
			enemyslimeposi = { -18,-28,2 };
			enemybomberposi = { -18,-28,4 };
			enemyspiderposi = { -18,-28,6 };
		}
		if (raund==3)
		{
			enemyslimeposi = { 0,-28,10 };
			enemybomberposi = { 0,-28,-10 };
			enemyspiderposi = { 18,-28,0 };
		}
		if (raund==4)
		{
			enemyslimeposi = { -16,-28,2 };
			enemybomberposi = { -18,-28,4 };
			enemyspiderposi = { -14,-28,6 };
		}
		
	}
	if(heremap==2)
	{
		if (raund == 0)
		{
			enemyslimeposi = { 0,-9,2 };
			enemybomberposi = { 0,-9,4 };
			enemyspiderposi = { 0,-9,6 };
		}
		if (raund == 1)
		{
			enemyslimeposi = { 15,-9,2 };
			enemybomberposi = { 15,-9,4 };
			enemyspiderposi = { 15,-9,6 };
		}
		if (raund == 2)
		{
			enemyslimeposi = { 30,-9,2 };
			enemybomberposi = { 30,-9,4 };
			enemyspiderposi = { 30,-9,6 };
		}
		if (raund == 3)
		{
			enemyslimeposi = { 48,-9,2 };
			enemybomberposi = { 48,-9,4 };
			enemyspiderposi = { 48,-9,6 };
		}
		if (raund == 4)
		{
			enemyslimeposi = { 65,-9,2 };
			enemybomberposi = { 65,-9,4 };
			enemyspiderposi = { 65,-9,6 };
		}
		if (raund == 5)
		{
			enemyslimeposi = { 80,-9,2 };
			enemybomberposi = { 80,-9,4 };
			enemyspiderposi = { 80,-9,6 };
		}
		if (raund == 6)
		{
			enemyslimeposi = { 90,-9,2 };
			enemybomberposi = { 90,-9,4 };
			enemyspiderposi = { 90,-9,6 };
		}

	}

	//ラウンド管理
	RaundManage();
	killspider = EnemySpider::Instance().GetDeadcount();
	killbomber = EnemyBomber::Instance().GetDeadcount();
	killslime  =  EnemySlime::Instance().GetDeadcount();
	killdrone  =  EnemyDrone::Instance().GetDeadcount();
	killcount = killbomber + killspider+killslime+killdrone;
	
	EnemyBomber::Instance().SetTerritory(enemybomberposi,0);
	EnemySpider::Instance().SetTerritory(enemyspiderposi,0);
	EnemySlime::Instance().SetTerritory(enemyslimeposi,0);
	EnemyDrone::Instance().SetTerritory(enemyspiderposi,0);

	playernowpos = Player::Instance().GetPosition();
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

		EnemyManager::Instance().Render(dc, shader);

		ItemManager::Instance().Render(dc,shader);

		UI->Render(dc, shader);

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
		fadeOut->Render();

		shieldGauge->Render();

		shieldIcon->Render();
		//ゲージ表示
		RenderEnemyGauge(dc, rc.view, rc.projection);

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
			ImGui::InputInt("Dronekillcount", &killdrone);
		}
		if (ImGui::CollapsingHeader("RAUND", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("raund", &raund);
			ImGui::Checkbox("crystal",&Oncrystal);
		}
	}


	ImGui::End();
}


void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//すべての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	EnemyManager& enemyspiderManager = EnemyManager::Instance();
	int enemySCount = enemyspiderManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		

		DirectX::XMVECTOR ScreenPosition{};

		DirectX::XMFLOAT3 enemyPosition{};
		

		enemyPosition = enemy->GetPosition();
		
		
		enemyPosition.y += enemy->GetHeight();
		
		

		ScreenPosition = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&enemyPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);

		DirectX::XMFLOAT3 screenPosition{};
		DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);

		float gaugeX = 30.0f;
		float gaugeY = 5.0f;

		float health = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());
		
		gauge->Render(dc,
			screenPosition.x - gaugeX * 0.5f,
			screenPosition.y - gaugeY,
			gaugeX * health,
			gaugeY,
			screenPosition.x,
			screenPosition.y,
			gauge->GetTextureWidth(),
			gauge->GetTextureHeight(),
			0,
			1, 0, 0, 1);
	}
	for (int i = 0; i < enemySCount; ++i)
	{
		
		Enemy* enemyS = enemyspiderManager.GetEnemy(i);

		DirectX::XMVECTOR ScreenPosition{};

		DirectX::XMFLOAT3 enemyPosition{};
		DirectX::XMFLOAT3 enemySPosition{};

		
		enemySPosition = enemyS->GetPosition();

		
		
		enemySPosition.y += enemyS->GetHeight();
		

		ScreenPosition = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&enemyPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);

		DirectX::XMFLOAT3 screenPosition{};
		DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);

		float gaugeX = 30.0f;
		float gaugeY = 5.0f;

		
		float healthS = enemyS->GetHealth() / static_cast<float>(enemyS->GetMaxHealth());

		gauge->Render(dc,
			screenPosition.x - gaugeX * 0.5f,
			screenPosition.y - gaugeY,
			gaugeX * healthS,
			gaugeY,
			screenPosition.x,
			screenPosition.y,
			gauge->GetTextureWidth(),
			gauge->GetTextureHeight(),
			0,
			1, 0, 0, 1);
	}

	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	//if(gamePad.GetButtonUp()& GamePad::BTN_B)
	//ここの敵の増殖
	//if (enemytimer>=3.99f)
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		screenPosition.z = 0.0f;
		DirectX::XMVECTOR WorldPosition{};



		WorldPosition = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 worldPosition{};
		DirectX::XMStoreFloat3(&worldPosition, WorldPosition);
		DirectX::XMFLOAT3 start = { worldPosition };

		screenPosition.z = 1.0f;

		WorldPosition = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);

		DirectX::XMStoreFloat3(&worldPosition, WorldPosition);
		DirectX::XMFLOAT3 end = { worldPosition };

		HitResult hit;
		/*if (StageManager::Instance().RayCast(start, end, hit))
		{
			EnemySlime* enemy = new EnemySlime;
			enemy->SetPosition(hit.position);
			EnemyManager::Instance().Register(enemy);
		}
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			EnemySpider* enemyS = new EnemySpider;
			enemyS->SetPosition(hit.position);
			EnemyManager::Instance().Register(enemyS);
		}*/
		


	}
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

	
		EnemyManager& enemybomberManager = EnemyManager::Instance();
		EnemyBomber* bomber = new EnemyBomber;
		bomber->SetPosition(enemybomberposi);
		bomber->SetTerritory(bomber->GetPosition(), 10.0f);
		enemybomberManager.Register(bomber);
	

	
		EnemyManager& enemydroneManager = EnemyManager::Instance();
		EnemyDrone* drone = new EnemyDrone;
		drone->SetPosition(enemyspiderposi);
		drone->SetTerritory(drone->GetPosition(), 10.0f);
		enemydroneManager.Register(drone);
	

	
		EnemyManager& enemyslimeManager = EnemyManager::Instance();
		EnemySlime* slime = new EnemySlime;
		slime->SetPosition(enemyslimeposi);
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyslimeManager.Register(slime);
	

	
	/*	
	* EnemyManager& enemyspiderManager = EnemyManager::Instance();
		EnemySpider* spider = new EnemySpider;
		spider->SetPosition(enemyspiderposi);
		spider->SetTerritory(spider->GetPosition(), 10.0f);
		enemyspiderManager.Register(spider);
		*/
	
}
void SceneGame::CrystalSet()
{
	Oncrystal = true;

	ItemManager &itemmanger = ItemManager::Instance();
	ItemCrystal* crystal = new ItemCrystal;
	
	crystal->SetPosition(CrystalPosition);
	itemmanger.Register(crystal);

}
void SceneGame::RaundManage()
{
	
		switch(raund)
		{
		case 0:
			EnemySet();
			raund++;
		case 1:
			if(killcount>=2)
			{
			EnemySet();
			raund++;
			}
		case 2:
			if (killcount >= 4)
			{
				
				if(heremap==1)
				{
					map1->OpenDoor();
				}
				if(heremap==2)
				{
					map2->OpenDoor1();
				}
				EnemySet();
				raund++;
			}
		case 3:
			if(killcount>=6)
			{
				EnemySet();
				raund++;
				
			}
		case 4:
			if(killcount>=8)
			{
				if (heremap == 1)
				{
					EnemySet();
					//マップ1クリア
					CrystalSet();
					raund++;
				}
				if (heremap == 2&&playernowpos.x>=canopendoor2)
				{
					map2->OpenDoor2();
					EnemySet();
					raund++;
				}
				
			}
		case 5:
			if(killcount>=10)
			{
				if (heremap == 2)
				{
					EnemySet();
					raund++;
				}

			}
		case 6:
			if(killcount>=14)
			{
				if (heremap == 2)
				{
					//マップ2クリア
					CrystalSet();
					raund++;
				}
			}
		
		}
}