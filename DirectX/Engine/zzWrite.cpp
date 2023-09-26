#include "zzWrite.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzShader.h"
#include "zzMaterial.h"


namespace zz
{
    std::unordered_map<int, FontData> WriteManager::mFontPixelData = {};
    WriteManager::WriteManager()
    {
    }
    WriteManager::~WriteManager()
    {
    }

    void WriteManager::Initialize()
    {
        insertData();
    }

    Texture* WriteManager::Wrtie(const std::wstring& writing, math::Vector3 scale)
    {
        std::shared_ptr<Texture> font_pixel = ResourceManager::Load<Texture>(L"font_pixel", L"..\\Resources\\Font\\font_pixel.png");
        Texture* writingTexture = new Texture();

        ScratchImage& srcImage = font_pixel->GetScratchImage();
        ScratchImage& dstImage = writingTexture->GetScratchImage();

        //TexMetadata metadata;
        //metadata.width = 100;
        //metadata.height = 100;
        //metadata.depth = 1;
        //metadata.arraySize = 1;
        //metadata.mipLevels = 1;
        //metadata.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        //metadata.dimension = TEX_DIMENSION_TEXTURE2D;

        dstImage.Initialize2D(DXGI_FORMAT_B8G8R8A8_UNORM, scale.x, scale.y, 1, 1);

        int line = 0;
        int word = 0;

        for (int i = 0; i < writing.size(); i++)
        {
            if (writing[i] == 10)
            {
                word = 0;
                line++;

                continue;
            }

            FontData data = mFontPixelData.find(writing[i])->second;
            Rect rect(data.rect_x, data.rect_y, data.rect_w, data.rect_h);

            HRESULT hr = CopyRectangle(*srcImage.GetImage(0, 0, 0), rect
                , *dstImage.GetImage(0, 0, 0), TEX_FILTER_DEFAULT, word++ * 6, line * 8);

            if (FAILED(hr))
            {
                int a = 0;
            }
        }

        writingTexture->UpdateImage();
        return writingTexture;
    }

    void WriteManager::insertData()
    {
        FontData data;

        data.rect_h = 11; data.rect_w = 3; data.rect_x = 0; data.rect_y = 0;
        mFontPixelData.insert({ 32, data });

        data.rect_h = 11; data.rect_w = 3; data.rect_x = 4; data.rect_y = 0;
        mFontPixelData.insert({ 33, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 8; data.rect_y = 0;
        mFontPixelData.insert({ 34, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 13; data.rect_y = 0;
        mFontPixelData.insert({ 35, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 20; data.rect_y = 0;
        mFontPixelData.insert({ 36, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 27; data.rect_y = 0;
        mFontPixelData.insert({ 37, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 34; data.rect_y = 0;
        mFontPixelData.insert({ 38, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 41; data.rect_y = 0;
        mFontPixelData.insert({ 39, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 44; data.rect_y = 0;
        mFontPixelData.insert({ 40, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 51; data.rect_y = 0;
        mFontPixelData.insert({ 41, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 58; data.rect_y = 0;
        mFontPixelData.insert({ 42, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 65; data.rect_y = 0;
        mFontPixelData.insert({ 43, data });

        data.rect_h = 11; data.rect_w = 3; data.rect_x = 72; data.rect_y = 0;
        mFontPixelData.insert({ 44, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 76; data.rect_y = 0;
        mFontPixelData.insert({ 45, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 83; data.rect_y = 0;
        mFontPixelData.insert({ 46, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 86; data.rect_y = 0;
        mFontPixelData.insert({ 47, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 93; data.rect_y = 0;
        mFontPixelData.insert({ 48, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 100; data.rect_y = 0;
        mFontPixelData.insert({ 49, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 105; data.rect_y = 0;
        mFontPixelData.insert({ 50, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 112; data.rect_y = 0;
        mFontPixelData.insert({ 51, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 119; data.rect_y = 0;
        mFontPixelData.insert({ 52, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 126; data.rect_y = 0;
        mFontPixelData.insert({ 53, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 133; data.rect_y = 0;
        mFontPixelData.insert({ 54, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 140; data.rect_y = 0;
        mFontPixelData.insert({ 55, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 147; data.rect_y = 0;
        mFontPixelData.insert({ 56, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 154; data.rect_y = 0;
        mFontPixelData.insert({ 57, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 161; data.rect_y = 0;
        mFontPixelData.insert({ 58, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 164; data.rect_y = 0;
        mFontPixelData.insert({ 59, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 167; data.rect_y = 0;
        mFontPixelData.insert({ 60, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 174; data.rect_y = 0;
        mFontPixelData.insert({ 61, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 181; data.rect_y = 0;
        mFontPixelData.insert({ 62, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 188; data.rect_y = 0;
        mFontPixelData.insert({ 63, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 195; data.rect_y = 0;
        mFontPixelData.insert({ 64, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 202; data.rect_y = 0;
        mFontPixelData.insert({ 65, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 209; data.rect_y = 0;
        mFontPixelData.insert({ 66, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 216; data.rect_y = 0;
        mFontPixelData.insert({ 67, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 223; data.rect_y = 0;
        mFontPixelData.insert({ 68, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 230; data.rect_y = 0;
        mFontPixelData.insert({ 69, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 237; data.rect_y = 0;
        mFontPixelData.insert({ 70, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 244; data.rect_y = 0;
        mFontPixelData.insert({ 71, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 251; data.rect_y = 0;
        mFontPixelData.insert({ 72, data });

        data.rect_h = 11; data.rect_w = 5; data.rect_x = 258; data.rect_y = 0;
        mFontPixelData.insert({ 73, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 264; data.rect_y = 0;
        mFontPixelData.insert({ 74, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 271; data.rect_y = 0;
        mFontPixelData.insert({ 75, data });

        data.rect_h = 11; data.rect_w = 5; data.rect_x = 278; data.rect_y = 0;
        mFontPixelData.insert({ 76, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 284; data.rect_y = 0;
        mFontPixelData.insert({ 77, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 291; data.rect_y = 0;
        mFontPixelData.insert({ 78, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 298; data.rect_y = 0;
        mFontPixelData.insert({ 79, data });

        data.rect_h = 11; data.rect_w = 5; data.rect_x = 305; data.rect_y = 0;
        mFontPixelData.insert({ 80, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 311; data.rect_y = 0;
        mFontPixelData.insert({ 81, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 318; data.rect_y = 0;
        mFontPixelData.insert({ 82, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 325; data.rect_y = 0;
        mFontPixelData.insert({ 83, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 332; data.rect_y = 0;
        mFontPixelData.insert({ 84, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 339; data.rect_y = 0;
        mFontPixelData.insert({ 85, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 346; data.rect_y = 0;
        mFontPixelData.insert({ 86, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 353; data.rect_y = 0;
        mFontPixelData.insert({ 87, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 360; data.rect_y = 0;
        mFontPixelData.insert({ 88, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 367; data.rect_y = 0;
        mFontPixelData.insert({ 89, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 374; data.rect_y = 0;
        mFontPixelData.insert({ 90, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 381; data.rect_y = 0;
        mFontPixelData.insert({ 91, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 388; data.rect_y = 0;
        mFontPixelData.insert({ 92, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 395; data.rect_y = 0;
        mFontPixelData.insert({ 93, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 402; data.rect_y = 0;
        mFontPixelData.insert({ 94, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 409; data.rect_y = 0;
        mFontPixelData.insert({ 95, data });

        data.rect_h = 11; data.rect_w = 3; data.rect_x = 416; data.rect_y = 0;
        mFontPixelData.insert({ 96, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 420; data.rect_y = 0;
        mFontPixelData.insert({ 97, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 425; data.rect_y = 0;
        mFontPixelData.insert({ 98, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 430; data.rect_y = 0;
        mFontPixelData.insert({ 99, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 435; data.rect_y = 0;
        mFontPixelData.insert({ 100, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 440; data.rect_y = 0;
        mFontPixelData.insert({ 101, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 445; data.rect_y = 0;
        mFontPixelData.insert({ 102, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 450; data.rect_y = 0;
        mFontPixelData.insert({ 103, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 455; data.rect_y = 0;
        mFontPixelData.insert({ 104, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 460; data.rect_y = 0;
        mFontPixelData.insert({ 105, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 463; data.rect_y = 0;
        mFontPixelData.insert({ 106, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 468; data.rect_y = 0;
        mFontPixelData.insert({ 107, data });

        data.rect_h = 11; data.rect_w = 2; data.rect_x = 473; data.rect_y = 0;
        mFontPixelData.insert({ 108, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 476; data.rect_y = 0;
        mFontPixelData.insert({ 109, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 483; data.rect_y = 0;
        mFontPixelData.insert({ 110, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 488; data.rect_y = 0;
        mFontPixelData.insert({ 111, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 493; data.rect_y = 0;
        mFontPixelData.insert({ 112, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 498; data.rect_y = 0;
        mFontPixelData.insert({ 113, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 503; data.rect_y = 0;
        mFontPixelData.insert({ 114, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 508; data.rect_y = 0;
        mFontPixelData.insert({ 115, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 513; data.rect_y = 0;
        mFontPixelData.insert({ 116, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 518; data.rect_y = 0;
        mFontPixelData.insert({ 117, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 523; data.rect_y = 0;
        mFontPixelData.insert({ 118, data });

        data.rect_h = 11; data.rect_w = 6; data.rect_x = 528; data.rect_y = 0;
        mFontPixelData.insert({ 119, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 535; data.rect_y = 0;
        mFontPixelData.insert({ 120, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 540; data.rect_y = 0;
        mFontPixelData.insert({ 121, data });

        data.rect_h = 11; data.rect_w = 4; data.rect_x = 545; data.rect_y = 0;
        mFontPixelData.insert({ 122, data });
    }

    
}