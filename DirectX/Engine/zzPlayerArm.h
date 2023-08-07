#pragma once

#include "zzGameObject.h"

namespace zz
{
    class PlayerArm : public GameObject
    {
    public:
        PlayerArm();
        virtual ~PlayerArm();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetPlayer(GameObject* player);

    private:
        GameObject* mPlayer;
    };
}
