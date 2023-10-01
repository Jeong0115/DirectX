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

    private:
        static void insertData();

        static std::unordered_map<int, FontData> mFontPixelData;

    };
}
