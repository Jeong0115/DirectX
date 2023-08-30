#pragma once

namespace zz
{
    enum class eComponentType
    {
        None,
        Transform,
        RigidBody,
        Particle,
        MeshRenderer,
        Script,
        Animator,
        Camera,
        Collider,
        PixelCollider,
        PixelCollider_Lite,
        Levitation,
        End,
    };

    enum class eLayerType
    {
        Camera,
        BG,
        Player,
        Monster,
        PlayerAttack,

        Object,
        Effect,
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
        WandSlot,

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