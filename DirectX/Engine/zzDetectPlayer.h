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

        void SetDetectRange(float rangeX, float rangeYUp, float rangeYDown) 
        { mDetectRangeX = rangeX; mDetectRangeYUp = rangeYUp; mDetectRangeYDown = rangeYDown; }
        Vector3 GetPlayerPos() { return PlayerPos; }

        bool IsPlayerInRange(); 

        int GetDirection() { return mDirection; }
        //юс╫ц
        static Vector3 PlayerPos;
    private:
        void rayCast(Vector3 pos);   

        float mDetectRangeYUp;
        float mDetectRangeYDown;
        float mDetectRangeX;

        bool mbPlayerInRange;
        bool mbBlock;

        int mDirection;
    };
}