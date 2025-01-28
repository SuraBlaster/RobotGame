#include "Stage.h"

static Stage* instance = nullptr;

Stage::Stage()
{
    instance = this;
}

Stage& Stage::Instance()
{
    return *instance;
}
