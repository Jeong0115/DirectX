#pragma once

#include "zzEngine.h"
#include "zzElement.h"
#include "zzChunk.h"

namespace zz
{
    class PixelGrid
    {
    public:
        PixelGrid();
        ~PixelGrid();

        enum class eElement
        {
            Sand,
            Water,
            End
        };

    private:
        

    public:
        static void Initialize();
        static void Update();
        static void FixedUpdate();
        static void Render();
        static void Release();
        static void Clear();
        static void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
        static void SetImage(int x, int y, std::shared_ptr<class Texture> texture, std::shared_ptr<class Texture> texture_visual = nullptr);
        static Element* GetElement(int x, int y) { 
            if (x < 0 || y < 0) return nullptr;
            return mElements[y][x]; } // ¼öÁ¤
        static std::vector<uint8_t>& GetPixelColor() { return mPixelColor; }
        static void SwapElement(int x1, int y1, int x2, int y2);
       // static Element* GetElement(int y, int x) { return mElements[y][x]; }
        static void SetActiveChunks(int x, int y);
        static void SetActiveChunk(int x, int y);

    private:
        static void updateChunk(int x, int y);
        static void registerElements(std::vector<Element*> elements);

        static HWND        mHwnd;
        static HDC         mHdc;
        static HBITMAP     mBackBuffer;
        static HDC         mBackHDC;

        static std::vector<uint8_t> mPixelColor;
        static std::vector<std::vector<Element*>> mElements;
 
        static UINT mWidth;
        static UINT mHeight;
  
        static void* bits;
        static class PixelGridColor* mImage;
  
        static double mFixedTime;
        static Chunk mChunks[32][32];
 
        static std::map<char, Element*> mElementMap;
        static Element* mSelectElement;

        static float x;
        static float y;
    };




    class PixelGridColor
    {
    public:
        PixelGridColor();
        ~PixelGridColor();

        void Update(std::vector<uint8_t>& pixelColor, HDC BackDC, float x, float y);

    private:
        HBITMAP mBitmap;
        HDC mHdc;
        void* bits;
    };
    
    
}

