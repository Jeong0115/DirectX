#pragma once

#include "zzScene.h"

namespace zz
{
    class EndScene : public Scene
    {
    public:
        EndScene();
        virtual ~EndScene();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void MovePlayer()   override;
    };
}

