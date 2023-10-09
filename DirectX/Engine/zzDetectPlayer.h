#pragma once

#include "zzComponent.h"

namespace zz
{
    class DetectPlayer : public Component
    {
    public:
        DetectPlayer();
        virtual ~DetectPlayer();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetDetectRange(float range) { mDetectRange = range; }
        Vector3 GetPlayerPos() { return PlayerPos; }
        //юс╫ц
        static Vector3 PlayerPos;
    private:
        void rayCast();   

        float mDetectRange;
    };
}