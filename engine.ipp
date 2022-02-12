
#pragma once

#include "engine.hpp"
#include <stdio.h>

#define _MINMAX_MIN 1
#define _MINMAX_MAX 2
#define _MINMAX_DEPTH 9

typedef struct _MoveAndResult
{
    int move;
    int score;
} MoveAndResult;



MoveAndResult _minmax(FourInARow::Game* pGame, int depthToSearch, int minOrMax);
MoveAndResult _minmaxAlphBeta(FourInARow::Game* pGame, int depthToSearch, int minOrMax, int alpha, int beta);

int ChooseNext(FourInARow::Game *pGameBoard/*, int whichIsBot (1 or 2)*/)
{
    FourInARow::Game game(*pGameBoard);

    printf("thinking...\n");

    MoveAndResult move = _minmax(&game, _MINMAX_DEPTH, _MINMAX_MAX);

    return move.move;
}


// return : score of max or min movement and its move.
//          scores
//               0 : minimizing side (player 1) win.
//               1 : no result / draw
//               2 : maximizing side (player 2) win.
MoveAndResult _minmax(FourInARow::Game* pGame, int depthToSearch, int minOrMax)
{
    /*
    static int depth = 0;
    depth++;
    printf("%*cminmax called : \n"
           "%*cdepthToSearch : %d\n"
           "%*cminOrMax min1max2 : %d\n\n",
           depth*4, ' ',
           depth*4, ' ', depthToSearch,
           depth*4, ' ', minOrMax);
    pGame->Print();
    */

    int current_game_status = pGame->GetGameStatus();

    MoveAndResult best_move_and_result;
    best_move_and_result.move = -1;
    best_move_and_result.score = minOrMax == _MINMAX_MIN ? 3 : -1;

    int opposite_of_minOrMax = minOrMax == _MINMAX_MIN ? _MINMAX_MAX : _MINMAX_MIN;

    int board_width = pGame->GetBoardWidth();


    switch (current_game_status)
    {
        case 1:
            best_move_and_result.score = 0;
            goto RETURN;
        case 2:
            best_move_and_result.score = 2;
            goto RETURN;
        case 3:
            best_move_and_result.score = 1;
            goto RETURN;
    }
    if (depthToSearch == 0)
    {
        best_move_and_result.score = 1;
        goto RETURN;
    }


    for (int move = 0; move < board_width; move++)
    {
        int result = pGame->PutCoin(move);
        // check result
        if (result == 2) continue;
        if (result == 1 || result == 3)
        {
            printf("this can't be happen!\n");
            break;
        }

        MoveAndResult best_next_choice = _minmax(pGame, depthToSearch - 1, opposite_of_minOrMax);
        best_next_choice.move = move;

        switch (minOrMax)
        {
            case _MINMAX_MIN:
                if (best_next_choice.score < best_move_and_result.score)
                {
                    best_move_and_result.score = best_next_choice.score;
                    best_move_and_result.move  = best_next_choice.move;
                }
                break;
            case _MINMAX_MAX:
                if (best_next_choice.score > best_move_and_result.score)
                {
                    best_move_and_result.score = best_next_choice.score;
                    best_move_and_result.move  = best_next_choice.move;
                }
                break;
        }
        pGame->Undo();
    }

RETURN:
    /*
    printf("%*cbest move : %d\n"
           "%*cbest score : %d\n\n",
           depth*4, ' ', best_move_and_result.move,
           depth*4, ' ', best_move_and_result.score);
    depth--;
    */

    return best_move_and_result;
}


MoveAndResult _minmaxAlphBeta(FourInARow::Game* pGame, int depthToSearch, int minOrMax, int alpha, int beta)
{
}



