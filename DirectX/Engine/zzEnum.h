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
        AudioSource,
        AudioListener,
        Light,
        Script,
        DetectPlayer,
        Animator,
        Camera,
        Collider,
        PixelCollider,
        PixelCollider_Lite,
        HealPoint,
        Levitation,
        Box2dCollider,
        End,
    };

    enum class eLayerType
    {
        Camera,
        BG,
        Vegetation,
        Player,
        Monster,
        PlayerAttack,
        MonsterAttack,
        PixelWorld,

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
        
        WandSlot,
        SpellSlot,
        ItemSlot,

        Item,
        Spell,

        Highligt,

        TextObj,
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
        AudioClip,
        ComputeShader,
        End,
    };

    enum class eEventType
    {
        Create_Object,
        Delete_Object,
        End,
    };

    enum class eBodyShape
    {
        Box,
        Circle,
        End,
    };
}