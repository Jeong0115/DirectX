//#pragma once
//
//#include "zzSolid.h"
//
//namespace zz
//{
//    class MovableSolid : public Solid
//    {
//    public:
//        MovableSolid();
//        virtual ~MovableSolid();
//       
//        virtual void UpdateUI() override;
//        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth) override;
//
//        virtual Element* Clone() = 0;
//
//    private:
//        void setAroundElementFreeFalling(Position targetPos, int depth);
//        bool setElementFreeFalling(Element* element);
//        int getAdditional(float val);
//        float getAverageVelOrGravity(float vel, float targetVel);
//
//    };
//}