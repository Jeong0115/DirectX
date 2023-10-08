#pragma once

#include "zzScene.h"

namespace zz
{
    class tScene : public Scene
    {
    public:
        tScene();
        virtual ~tScene();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}