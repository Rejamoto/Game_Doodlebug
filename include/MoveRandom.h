#ifndef MOVERANDOM_H
#define MOVERANDOM_H

#include "MoveBehavior.h"

class MoveRandom : public MoveBehavior
{
    public:
        MoveRandom() {}
        virtual ~MoveRandom() {}
        void moving();
};

#endif // MOVERANDOM_H
