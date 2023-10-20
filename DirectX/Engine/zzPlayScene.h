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

        virtual void Exit()         override;
    private:
        void loadBGM();
        void playNextBGM();

        std::vector<std::shared_ptr<class AudioClip>> mBGM;

        int mBGM_Index;
    };
}
