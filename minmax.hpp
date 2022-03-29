
#pragma once

#include <stddef.h>

#include "minmaxdef.hpp"

class MinmaxDecider
{

    public:

        int Decide(
                FourInARow::Game* pGame,
                int depth);

        // When this function called, 
        // the decide function will return value.
        void Kill(void);

    private:

        _MoveAndScore minmax(
                FourInARow::Game* pGame,
                int depth, int minOrMax
                );

        int m_depthToThink;

        int m_IsForcedToFinish;

        int m_ExitCode;
};


#include "minmax.ipp"

#undef DEBUG

