#include "Item.h"
#include "Graphics/Graphics.h"
#include "ItemManager.h"

ItemManager* manager{};



void Item::Destroy()
{
    manager->Instance().Remove(this);
}