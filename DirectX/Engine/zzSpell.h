#pragma once

#include "zzGameObject.h"

namespace zz
{
    enum class eSpellType
    {
        Projectile,
        Utility,
        End
    };

    class Spell : public GameObject
    {
    public:
        Spell();
        ~Spell();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        eSpellType GetSpellType() { return mType; }
        float GetManaDrain() { return mManaDrain; }
        float GetCastDelay() { return mCastDelay; }
            
    protected:
        eSpellType  mType;

        float       mManaDrain;
        float       mCastDelay;
    };
}
