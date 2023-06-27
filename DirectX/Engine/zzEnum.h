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
        End,
    };
}