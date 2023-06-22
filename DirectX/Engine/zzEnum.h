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
        End,
    };

    enum class eLayerType
    {
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