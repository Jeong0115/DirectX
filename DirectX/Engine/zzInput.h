#pragma once
#include "zzEngine.h"

namespace zz
{
    using namespace zz::math;
    enum class eKeyCode
    {
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,

        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,

        UP, DOWN, LEFT, RIGHT, SPACE,
        LBUTTON, RBUTTON, TAB, CTRL,

        F5, F6, F8,

        END,
    };

    enum class eKeyState
    {
        Down,
        Pressed,
        Up,
        None,
    };

    class Input
    {
    public:
        struct Key
        {
            eKeyCode key;
            eKeyState state;
            bool bPressed;
        };

        static void Initialize();
        static void Update();

        // 인벤토리 장착 아이템 변경 함수, 범용적으로 쓸거면 수정해야됨
        static UINT IsInputNumberKey();

        inline static eKeyState GetKeyState(eKeyCode keyCode)
        {
            return mKeys[(UINT)keyCode].state;
        };

        static __forceinline bool GetKey(eKeyCode keyCode)
        {
            return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Pressed;
        }

        static __forceinline bool GetKeyDown(eKeyCode keyCode)
        {
            return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Down;
        }

        static __forceinline bool GetKeyUp(eKeyCode keyCode)
        {
            return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Up;
        }

        static __forceinline Vector3 GetMouseWorldPos() { return mMouseWorldPos; }
        static __forceinline Vector3 GetMouseUIPos() { return mMouseUIPos; }

        static void SetPlayer(class Transform* player) { mPlayer = player; }
        static float IsFlip();

    private:
        static std::vector<Key> mKeys;
        static Vector3 mMouseWorldPos;
        static Vector3 mMouseUIPos;

        static class Transform* mPlayer;
    };
}
