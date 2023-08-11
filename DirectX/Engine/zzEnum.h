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
        Particle,
        UI,
        End,
    };

    enum class eUIType
    {
        BG,
        InfoBox,

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
        ComputeShader,
        End,
    };

    enum class eEventType
    {
        Create_Object,
        Delete_Object,
        End,
    };
}