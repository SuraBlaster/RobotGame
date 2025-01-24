#pragma once

#include <vector>
#include"Weapon.h"
#include <set>

//�G�l�~�[�}�l�[�W���[
class WeaponManager
{
private:
    WeaponManager() {}
    ~WeaponManager() {}

public:
    //�B��̃C���X�^���X���擾
    static WeaponManager& Instance()
    {
        static WeaponManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�G�l�~�[�o�^
    void Register(Weapon* weapon);

    //�G�l�~�[�擾
    Weapon* GetEnemy(int index) { return weapons.at(index); }

    //�G�l�~�[�폜
    void Clear();


    void Remove(Weapon* weapon);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();



private:
    std::vector<Weapon*> weapons;

    std::set<Weapon*> removes;
};