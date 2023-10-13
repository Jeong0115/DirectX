#pragma once

#include "zzGameObject.h"

namespace zz
{
    class LimbB : public GameObject
    {
    public:
        enum class eMoveState
        {
            Bend,
            Search,
            Move,
            Land,
            Stay,
        };

        LimbB();
        virtual ~LimbB();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetDir(int i) { dir = i; }
        void SetAngle(float angle) { mCurAngle = angle; }
        void SetBend();
        void SetMove(float revolutionAngle);
    private:
        void bend();
        void move();
        void land();

        bool mbBend;
        float mDuration;
        float mTime;
        int dir;

        float mCurAngle;
        float mNextAngle;

        eMoveState mMoveState;
    };
}

