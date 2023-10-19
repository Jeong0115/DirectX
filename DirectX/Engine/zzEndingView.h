#pragma once

#include "zzGameObject.h"

namespace zz
{
    class EndingView : public GameObject
    {
    public:
        EndingView();
        ~EndingView();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;


    private:
        class MeshRenderer* renderer;
    };
}

