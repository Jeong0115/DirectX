#include "zzImmovableSolid.h"

namespace zz
{
    ImmovableSolid::ImmovableSolid()
    {
        isFreeFalling = false;
    }

    ImmovableSolid::~ImmovableSolid()
    {
    }

    void ImmovableSolid::Update()
    {
        //applyHeatToNeighborsIfIgnited(matrix);
        //takeEffectsDamage(matrix);
        //spawnSparkIfIgnited(matrix);
        //modifyColor();
        //customElementFunctions(matrix);
    }
    bool ImmovableSolid::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
    {
        return true;
    }
}