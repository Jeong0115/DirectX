#pragma once

#include "zzComputeShader.h"
#include "zzTexture2D.h"

namespace zz
{
    class BloomManager
    {
    public:
        BloomManager();
        ~BloomManager();

        static void Initialize();
        static void Update();
        static void Render();
        static void Release();

        static void SetComputeShader(std::shared_ptr<ComputeShader> shader, int type);
    private:

        static std::shared_ptr<ComputeShader> mExtracionCS;
        static std::shared_ptr<ComputeShader> mHorizonCS;
        static std::shared_ptr<ComputeShader> mVerticalCS;
        
        static Texture2D* mBloomPingTex;
        static Texture2D* mBloomPongTex;
    };
}

