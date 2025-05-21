#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"


void SceneLoading::Initialize()
{
    //BGM�����ݒ�
    Audio& audio = audio.Instance();
    //BGM�ݒ�
    LoadingBGM = audio.LoadAudioSource("Data/Audio/BGM/Loading.wav");
    //BGM���ʐݒ�(100�ȉ�)
    LoadingBGM->sourceVoice->SetVolume(50);
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");
    load = new Sprite("Data/Sprite/Loading.png");
    dot = new Sprite("Data/Sprite/Dot.png");

    thread = new std::thread(LoadingThread, this);
}

void SceneLoading::Finalize()
{
    /*if (thread && thread->joinable())
    {
        thread->join();
    }
    delete thread;
    thread = nullptr;*/

    if (thread != nullptr)
    {
        thread->join();


        delete thread;
        thread = nullptr;
    }

    if (load != nullptr)
    {
        delete load;
        load = nullptr;
    }

    if (dot != nullptr)
    {
        delete dot;
        dot = nullptr;
    }
}

void SceneLoading::Update(float elapsedTime)
{
    //BGM�Đ�
    LoadingBGM->Play(true);
    constexpr float speed = 180;
    angle += speed * elapsedTime;
    timeAccumulator += elapsedTime;

    if (timeAccumulator >= updateInterval) {
        timeAccumulator = 0.0f;
        dotCount = (dotCount % 3) + 1;
    }

    if (nextScene != nullptr && nextScene->isReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(load->GetTextureWidth());
        float textureHeight = static_cast<float>(load->GetTextureHeight());
        //float positionX = screenWidth - textureWidth;
        //float positionY = screenHeight - textureHeight;


        load->Render(dc,
            5, 30, 630.0f, 170.0f,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);

        textureWidth = static_cast<float>(dot->GetTextureWidth());
        textureHeight = static_cast<float>(dot->GetTextureHeight());

        for(int i = 0; i < dotCount; i++)
        {
            dot->Render(dc,
                430.0f + (i * 40), -90.0f, 252.0f, 370.0f,
                0, 0, textureWidth, textureHeight,
                0,
                1, 1, 1, 1);
        }
        
    }
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    //�X���b�h���I���O��COM�֘A�̏�����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}
