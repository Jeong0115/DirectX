#pragma once

#include "zzGameObject.h"

namespace zz
{
    class LimbA : public GameObject
    {
    public:
        enum class eMoveState
        {
            Bend,
            Search,
            Move,
            Stay,
        };

        LimbA(GameObject* body);
        virtual ~LimbA();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetMoveState(eMoveState state);
        void TransPosition();

    private:
        void bend();
        void search();
        void move();
        void stay();

        GameObject* mBody;

        class LimbB* limbB;

        bool mbBend;
        float mTime;
        float mRotationAngle;
        float mCurAngle;
        int dir;

        float mDuration;

        Vector2 landPos;
        eMoveState mMoveState;

        
    };
}

