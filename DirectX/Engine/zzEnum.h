#pragma once

namespace zz
{
    enum class eComponentType
    {
        None,
        Transform,
        Particle,
        MeshRenderer,
        Script,
        Animator,
        Camera,
        UICollider,
        End,
    };

    enum class eLayerType
    {
        Camera,
        BG,
        Player,
        Monster,
        
        Object,
        UI,
        End,
    };

    enum class eUIType
    {
        NoneCollider,
        ItemSlot,
        SpellSlot,

        Item,
        Spell,

        Highligt,

        Mouse,

        End
    };

    enum class eResourceType
    {
        Texture,
        Shader,
        Material,
        Mesh,
        Animation,
        End,
    };
}