
#pragma once

#include <stddef.h>

#include "minmaxdef.hpp"

#define DEBUG
#define DEBUG2

class MinmaxABPlusDecider
{

    public:

        int Decide(
                FourInARow::Game* pGame,
                int depth);

        // When this function called, 
        // the decide function will return value.
        void Kill(void);

    private:

        _MoveAndScore minmaxABPlus(
                FourInARow::Game* pGame,
                int depth, int minOrMax,
                int alpha, int beta
                );

        int m_depthToThink;

        int m_IsForcedToFinish;

        int m_ExitCode;
};


#include "minmaxABPlus.ipp"

#undef DEBUG
#undef DEBUG2

