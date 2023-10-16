#pragma once

#include "zzGameObject.h"

namespace zz
{
    class PlayerView : public GameObject
    {
    public:
        PlayerView();
        ~PlayerView();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetPlayer(GameObject* player) { mPlayer = player; }

    private:
        class MeshRenderer* renderer;

        GameObject* mPlayer;
    };
}

