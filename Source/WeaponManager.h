#pragma once

#include <vector>
#include"Weapon.h"
#include <set>

//エネミーマネージャー
class WeaponManager
{
private:
    WeaponManager() {}
    ~WeaponManager() {}

public:
    //唯一のインスタンスを取得
    static WeaponManager& Instance()
    {
        static WeaponManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //エネミー登録
    void Register(Weapon* weapon);

    //エネミー取得
    Weapon* GetEnemy(int index) { return weapons.at(index); }

    //エネミー削除
    void Clear();


    void Remove(Weapon* weapon);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();



private:
    std::vector<Weapon*> weapons;

    std::set<Weapon*> removes;
};