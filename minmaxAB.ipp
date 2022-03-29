
#pragma once

#include "minmaxAB.hpp"

#include <stdio.h>


int MinmaxABDecider::Decide(
        FourInARow::Game* pGame,
        int depth)
{
    m_IsForcedToFinish = false;
    m_depthToThink = depth;
    m_ExitCode = -1;

    _MoveAndScore decision = {-1, 0};

    switch (pGame->GetWhichTurn())
    {
        case 1:
            decision = minmaxAB(pGame, 0, MINMAX_MIN, -SINT32_INF, SINT32_INF);
            break;
        case 2:
            decision = minmaxAB(pGame, 0, MINMAX_MAX, -SINT32_INF, SINT32_INF);
            break;
    }

#ifdef DEBUG
    printf("estimated score : %d\n", decision.score);
    printf("decided : %d\n", decision.move);
#endif

    return decision.move;
}

void MinmaxABDecider::Kill(void)
{
#ifdef DEBUG
    printf("Kill called\n");
#endif
    m_IsForcedToFinish = true;
}



_MoveAndScore MinmaxABDecider::minmaxAB(
        FourInARow::Game* pGame,
        int depth, int minOrMax,
        int alpha, int beta)
{
#ifdef DEBUG2
    printf("%*ccalled\n", depth * 4, ' ');
    printf("%*cminOrMax : %d\n", depth * 4, ' ', minOrMax);
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
#ifdef DEBUG2
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

#ifdef DEBUG2
        printf("%*cif move : %d\n", depth * 4, ' ', move);
#endif
        int next_score = minmaxAB(pGame, depth + 1, -minOrMax, alpha, beta).score;

        if (next_score * minOrMax > best_move.score * minOrMax)
        {
            best_move.move  = move;
            best_move.score = next_score;
            if (minOrMax == 1 && next_score > alpha)
            {
                alpha = next_score;
            }
            if (minOrMax == -1 && next_score < beta)
            {
                beta = next_score;
            }
        }

        pGame->Undo();

        if (alpha >= beta)
        {
#ifdef DEBUG2
            printf("%*calpha : %d\n", depth * 4, ' ', alpha);
            printf("%*cbeta  : %d\n", depth * 4, ' ', beta);
#endif
            break;
        }
        if (m_IsForcedToFinish)
        {
#ifdef DEBUG2
            printf("killed\n");
#endif
            break;
        }
    }

    delete [] moves;

#ifdef DEBUG2
    printf("%*cmove : %d\n", depth * 4, ' ', best_move.move);
    printf("%*cscore : %d\n", depth * 4, ' ', best_move.score);
    printf("%*creturn\n", depth * 4, ' ');
#endif

    return best_move;
}

