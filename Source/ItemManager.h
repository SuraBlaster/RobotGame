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

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�A�C�e���o�^
    void Register(Item* item);

    //�A�C�e�����擾
    int GetItemCount() const { return static_cast<int>(items.size()); }

    //�A�C�e���擾
    Item* GetItem(int index) { return items.at(index); }

    //�A�C�e���폜
    void Clear();


    void Remove(Item* item);

    bool GetGoalFlag() const { return GoalFlag; }
    void SetGoalFlag(bool goalFlag) { GoalFlag = goalFlag; }



private:

    std::vector<Item*> items;

    std::set<Item*> removes;

    bool GoalFlag;
};
