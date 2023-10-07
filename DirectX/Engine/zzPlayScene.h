#pragma once

#include "zzScene.h"

namespace zz
{
    class PlayScene : public Scene
    {
    public:
        PlayScene();
        virtual ~PlayScene();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void MakeBG(std::wstring material, Vector3 scale, Vector3 pos, float moveSpeed, float parallaxScale);

    private:

    };
}
