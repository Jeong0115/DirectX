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
        End,
    };

    enum class eLayerType
    {
        Camera,
        BG,
        Player,
        Monster,
        UI,
        End,
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