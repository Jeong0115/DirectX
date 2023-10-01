#pragma once

#include "zzDebugObject.h"
#include "..\\Engine\\zzGraphics.h"



namespace zz
{
    class Editor
    {
    public:
        static void Initialize();
        static void Run();

        static void Update();
        static void LateUpdate();
        static void Render();
        static void Release();

        static void DebugRender(const DebugMesh& mesh);

    private:
        //static std::vector<Widget*> mWidgets;
        //static std::vector<EditorObject*> mEditorObjects;
        static std::vector<DebugObject*> mDebugObjects;
    };
}
