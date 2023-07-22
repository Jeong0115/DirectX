#pragma once

#include "..\\Engine\\zzGameObject.h"

namespace zz
{
    class DebugObject : public GameObject
    {
    public:
        DebugObject();
        virtual ~DebugObject();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;
    };
}
