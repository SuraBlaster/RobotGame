#pragma once

#include "Scene.h"

//シーンマネージャー
class SceneManager
{
private:
    SceneManager(){}
    ~SceneManager(){}

public:
    //唯一のインスタンス取得
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    void Update(float elapsedTime);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

    bool GetIsGameEnd() { return isGameEnd; }
    void GameClose() { isGameEnd = true; }
    
private:
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
    bool isGameEnd = false;
};
