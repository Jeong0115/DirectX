#pragma once

#include "zzScript.h"

namespace zz
{   
    class GameObejct;
    class PlayerScript : public Script
    {
    public:
        PlayerScript();
        ~PlayerScript();

        virtual void Initialize() override;
        virtual void Update() override;

        void SetCamera(GameObject* camera) { mCamera = camera; }

    private:
        GameObject* mCamera;
        class Animator* mAnimator;  
        float vel;

        bool jump = false;
        bool fall = false;
    };
}

