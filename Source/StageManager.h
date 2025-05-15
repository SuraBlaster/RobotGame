#pragma once

#include <vector>
#include "Stage.h"
#include <set>
#include <SceneSelect.h>

class StageManager
{
private:
    StageManager(){}
    ~StageManager(){}

public:
    //唯一のインスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //更新処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //ステージ登録
    void Register(Stage* stage);

    //ステージ全削除
    void Clear();

    //指定のステージ消去
    void Remove(Stage* stage);

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        HitResult& hit);

    SceneSelect::Stage GetStage() { return stage; }

    void SetStage(SceneSelect::Stage stage) { this->stage = stage; }

    bool GetButtonFlag() { return ButtonFlag; }

    void SetButtonFlag(bool buttonFlag) { ButtonFlag = buttonFlag; }


private:
    std::vector<Stage*> stages;

    std::set<Stage*> removes;

    SceneSelect::Stage stage;

    bool ButtonFlag;

};