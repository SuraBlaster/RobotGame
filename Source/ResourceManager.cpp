#include "Graphics/Graphics.h"
#include "ResourceManager.h"

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
    // モデルを検索
    auto it = models.find(filename);
    if (it != models.end())
    {
        // 弱い参照から共有参照を取得
        std::shared_ptr<ModelResource> modelResource = it->second.lock();
        if (modelResource)
        {
            return modelResource;
        }
    }

    // 新規モデルリソース作成＆読み込み
    std::shared_ptr<ModelResource> newModelResource = std::make_shared<ModelResource>();
    newModelResource->Load(Graphics::Instance().GetDevice(), filename);

    // マップに登録
    models[filename] = newModelResource;

    return newModelResource;
}
