#pragma once
//
#include "zzEngine.h"
//#include "zzElement.h"
//#include "zzChunk.h"
//
//namespace zz
//{
//    class PixelGrid
//    {
//    public:
//        PixelGrid();
//        ~PixelGrid();
//
//        enum class eElement
//        {
//            Sand,
//            Water,
//            End
//        };
//
//    public:
//        static void Initialize();
//        static void UpdateUI();
//        static void FixedUpdate();
//        static void Render();
//        static void Release();
//        static void ClearUI();
//
//        static void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
//        static void SetImage(int x, int y, std::shared_ptr<class Texture> texture, std::shared_ptr<class Texture> texture_visual = nullptr);
//        static Element* GetElement(int x, int y);
//        static std::vector<uint8_t>& GetPixelColor() { return mPixelColor; }
//        static void SwapElement(int x1, int y1, int x2, int y2);
//        static void SetActiveChunks(int x, int y);
//        static void SetActiveChunk(int x, int y);
//        static void SetPixelColor(int x, int y, uint32_t color);
//        static void CreateElement(int x, int y, Element* element);
//        static void DeleteElement(int x, int y);
//        static bool IsSurrounded(int x, int y);
//
//        static int GetFrameCount() { return mFrameCount; }
//        static std::bitset<1> Step;
//    private:
//        static void updateChunk(int x, int y);
//        static void increaseFrameCount() { mFrameCount = mFrameCount == 3 ? 0 : mFrameCount + 1; }
//
//        static HWND        mHwnd;
//        static HDC         mHdc;
//        static HBITMAP     mBackBuffer;
//        static HDC         mBackHDC;
//
//        static std::vector<uint8_t> mPixelColor;
//        static std::vector<std::vector<Element*>> mElements;
// 
//        static UINT mWidth;
//        static UINT mHeight;
//  
//        static void* bits;
//        static class PixelGridColor* mImage;
//  
//        static double mFixedTime;
//        static Chunk mChunks[32][32];
// 
//        static std::map<char, Element*> mElementMap;
//        static Element* mSelectElement;
//
//        static float x;
//        static float y;
//
//        static int mFrameCount;
//
//        static std::vector<Element*> mDeadElement;
//    };
//
//
//
//
namespace zz
{
    class PixelGridColor
    {
    public:
        PixelGridColor();
        ~PixelGridColor();

        void Update(std::vector<uint32_t>& pixelColor, HDC BackDC, float x, float y);

    private:
        HBITMAP mBitmap;
        HDC mHdc;
        void* bits;
    };
    
    
}

