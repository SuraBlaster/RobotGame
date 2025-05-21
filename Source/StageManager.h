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

    bool GetCursorFlag1() { return CursorFlag1; }

    void SetCursorFlag1(bool cursorFlag) { CursorFlag1 = cursorFlag; }

    bool GetCursorFlag2() { return CursorFlag2; }

    void SetCursorFlag2(bool cursorFlag) { CursorFlag2 = cursorFlag; }


private:
    std::vector<Stage*> stages;

    std::set<Stage*> removes;

    SceneSelect::Stage stage;

    bool CursorFlag1;

    bool CursorFlag2;

};