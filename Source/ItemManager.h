#pragma once

#include <vector>
#include"Item.h"
#include <set>

class ItemManager
{
public:
    ItemManager() {};
    ~ItemManager() {};

    static ItemManager& Instance()
    {
        static ItemManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //アイテム登録
    void Register(Item* item);

    //アイテム数取得
    int GetItemCount() const { return static_cast<int>(items.size()); }

    //アイテム取得
    Item* GetItem(int index) { return items.at(index); }

    //アイテム削除
    void Clear();


    void Remove(Item* item);

    bool GetGoalFlag() const { return GoalFlag; }
    void SetGoalFlag(bool goalFlag) { GoalFlag = goalFlag; }



private:

    std::vector<Item*> items;

    std::set<Item*> removes;

    bool GoalFlag;
};
