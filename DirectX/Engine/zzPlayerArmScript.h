#pragma once

#include "zzScript.h"

namespace zz
{
    class PlayerArmScript : public Script
    {
    public:
        PlayerArmScript();
        ~PlayerArmScript();

        virtual void Initialize() override;
        virtual void Update() override;

        void SetPlayer(class GameObject* player) { mPlayer = player; }

    private:
        class Animator* mAnimator;
        class GameObject* mPlayer;
    };
}

