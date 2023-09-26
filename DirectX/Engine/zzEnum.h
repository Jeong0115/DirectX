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
        Light,
        Script,
        DetectPlayer,
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
        HUD,
        InfoBox,

        ItemSlot,
        SpellSlot,
        WandSlot,

        Item,
        Spell,

        Highligt,

        TextBox,
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