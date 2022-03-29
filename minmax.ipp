
#pragma once

#include "minmax.hpp"

#include <stdio.h>


int MinmaxDecider::Decide(
        FourInARow::Game* pGame,
        int depth)
{
    m_IsForcedToFinish = false;
    m_depthToThink = depth;
    m_ExitCode = -1;

    int decision = -1;

    switch (pGame->GetWhichTurn())
    {
        case 1:
            decision = minmax(pGame, 0, MINMAX_MIN).move;
            break;
        case 2:
            decision = minmax(pGame, 0, MINMAX_MAX).move;
            break;
    }

#ifdef DEBUG
    printf("decided%d\n", decision);
#endif

    return decision;
}

void MinmaxDecider::Kill(void)
{
    m_IsForcedToFinish = true;
}



_MoveAndScore MinmaxDecider::minmax(
        FourInARow::Game* pGame,
        int depth, int minOrMax)
{
#ifdef DEBUG
    printf("%*ccalled\n", depth * 4, ' ');
#endif

    int current_game_state = pGame->GetGameStatus();

    _MoveAndScore best_move;
    best_move.move  = -1;
    best_move.score = SINT32_INF * (-minOrMax);

    if (depth == m_depthToThink)
    {
        best_move.score = 0;
    }
    if (current_game_state == 1)
    {
        best_move.score = -1;
    }
    if (current_game_state == 2)
    {
        best_move.score = 1;
    }
    if (current_game_state == 3)
    {
        best_move.score = 0;
    }

    if (current_game_state != 0 ||
        depth == m_depthToThink)
    {
#ifdef DEBUG
    printf("%*cmove : ---\n", depth * 4, ' ');
    printf("%*cscore : %d\n", depth * 4, ' ', best_move.score);
    printf("%*creturn\n", depth * 4, ' ');
#endif
        return best_move;
    }


    int board_width = pGame->GetBoardWidth();

    int* moves;
    moves = new int [board_width];
    for (int i = 0; i < board_width; i++)
    {
        moves[i] = i;
    }
    _ShuffleArr(moves, board_width);

    for (int i = 0; i < board_width; i++)
    {
        int move   = moves[i];
        int result = pGame->PutCoin(move);

        if (result == 2)
        {
            continue;
        }
        if (result == 1 ||
            result == 3)
        {
            m_ExitCode = 1;
            m_IsForcedToFinish = true;
            return best_move;
        }

#ifdef DEBUG
        printf("%*cif move : %d\n", depth * 4, ' ', move);
#endif
        int next_score = minmax(pGame, depth + 1, -minOrMax).score;

        if (next_score * minOrMax > best_move.score * minOrMax)
        {
            best_move.move  = move;
            best_move.score = next_score;
        }

        pGame->Undo();

        if (m_IsForcedToFinish)
        {
            break;
        }
    }

    delete [] moves;

#ifdef DEBUG
    printf("%*cmove : %d\n", depth * 4, ' ', best_move.move);
    printf("%*cscore : %d\n", depth * 4, ' ', best_move.score);
    printf("%*creturn\n", depth * 4, ' ');
#endif

    return best_move;
}

