#pragma once

#include "zzEngine.h"

namespace zz
{
    struct FontData
    {
        int rect_h;
        int rect_w;
        int rect_x;
        int rect_y;
    };

    class WriteManager
    {
    public:
        WriteManager();
        ~WriteManager();

        static void Initialize();
        static class Texture* Wrtie(const std::wstring& writing, math::Vector3 scale);
        static class Texture* WrtieItemInformation(const std::wstring& writing, math::Vector3 scale);
        static class Texture* WrtieDamage(const std::wstring& writing, math::Vector3 scale);

    private:
        static void insertData();
        static void insertFontData();
        static void insertDamageData();

        static std::unordered_map<int, FontData> mFontPixelData;
        static std::unordered_map<int, FontData> mFontDamageData;

        static std::shared_ptr<Texture> mFont_pixel;
        static std::shared_ptr<Texture> mFont_damage;
    };
}
