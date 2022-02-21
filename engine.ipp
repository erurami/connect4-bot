
#pragma once

#include "engine.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define _MINMAX_MIN 1
#define _MINMAX_MAX 2


int ChooseNext(FourInARow::Game *pGameBoard/*, int whichIsBot (1 or 2)*/)
{
    Connect4Ai connect4_ai(pGameBoard);

    connect4_ai.StartThinkingNextMove();

    // connect4_ai.WaitForFinish();
    Sleep(1000);
    connect4_ai.KillThinkingProcess();

    return connect4_ai.GetResult();

}



void _ShuffleArr(int* arr, int length)
{
    srand(clock());
    for (int i = length - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp;
        tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}



// ==================== Connect4Ai =====================

DWORD WINAPI _Run_Connect4Ai_ChooseNext(LPVOID lpParam)
{
    ((Connect4Ai*)lpParam)->_ChooseNext();
    return 0;
}


int Connect4Ai::m_ThinkMethod = ENGINE_ALPHABETA;
int Connect4Ai::m_ThinkDepth  = 10;

Connect4Ai::Connect4Ai()
{
    m_ThinkingStepNow = -3;
    m_pGame = NULL;
    m_ThinkResult = -1;
}

Connect4Ai::Connect4Ai(const FourInARow::Game *pGameBoard)
    :Connect4Ai()
{
    SetGame(pGameBoard);
}

Connect4Ai::~Connect4Ai()
{
    if (m_pGame != NULL)
    {
        delete m_pGame;
    }
}

void Connect4Ai::SetGame(const FourInARow::Game* pGameBoard)
{
    if (m_pGame != NULL)
    {
        delete m_pGame;
    }

    m_pGame = new FourInARow::Game(*pGameBoard);

    m_ThinkingStepNow = -2;
}

void Connect4Ai::StartThinkingNextMove(void)
{
    m_ThinkingStepNow = -1;
    m_IsForcedToStop = false;

    DWORD dw_id;
    CreateThread(0, 0, _Run_Connect4Ai_ChooseNext, (LPVOID)this, 0, &dw_id);
}

void Connect4Ai::KillThinkingProcess(void)
{
    if (m_ThinkingStepNow != -1)
    {
        return;
    }

    m_IsForcedToStop = true;
    WaitForFinish();
}

void Connect4Ai::WaitForFinish(void)
{
    if (m_ThinkingStepNow < -1)
    {
        return;
    }
    while (m_ThinkingStepNow == -1)
    {
        Sleep(50);
    }
}

int Connect4Ai::GetResult(void)
{
    if (m_ThinkingStepNow < 0)
    {
        return m_ThinkingStepNow;
    }
    return m_ThinkResult;
}

void Connect4Ai::_ChooseNext(void)
{
    switch (m_ThinkMethod)
    {
        case ENGINE_MINMAX:
            m_ThinkResult = minmax(m_pGame, m_ThinkDepth, _MINMAX_MAX).move;
            break;
        case ENGINE_ALPHABETA:
            m_ThinkResult = minmaxAlphaBeta(m_pGame, m_ThinkDepth, _MINMAX_MAX).move;
            break;
    }

    m_ThinkingStepNow = 0;
}

// ------------------- engines -------------------


// return : score of max or min movement and its move.
//          scores
//               0 : minimizing side (player 1) win.
//               1 : no result / draw
//               2 : maximizing side (player 2) win.

_MoveAndResult Connect4Ai::minmax(FourInARow::Game* pGame, int depth, int minOrMax)
{
    int current_game_status = pGame->GetGameStatus();

    _MoveAndResult best_move_and_result;
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
    if (depth == 0)
    {
        best_move_and_result.score = 1;
        goto RETURN;
    }


    // ganarate shuffled moves array
    int* moves;
    moves = new int [board_width];
    for (int i = 0; i < board_width; i++)
    {
        moves[i] = i;
    }
    _ShuffleArr(moves, board_width);


    for (int i = 0; i < board_width; i++)
    {
        int move = moves[i];
        int result = pGame->PutCoin(move);

        if (result == 2) // if row was full
        {
            continue;
        }
        if (result == 1 ||
            result == 3)
        {
            printf("this can't be happen!\n");
            break;
        }


        int next_score = minmax(pGame, depth - 1, opposite_of_minOrMax).score;

        switch (minOrMax)
        {
            case _MINMAX_MIN:
                if (next_score < best_move_and_result.score)
                {
                    best_move_and_result.score = next_score;
                    best_move_and_result.move  = move;
                }
                break;
            case _MINMAX_MAX:
                if (next_score > best_move_and_result.score)
                {
                    best_move_and_result.score = next_score;
                    best_move_and_result.move  = move;
                }
                break;
        }
        if (i == 0)
        {
            best_move_and_result.score = next_score;
            best_move_and_result.move  = move;
        }

        pGame->Undo();

        if (m_IsForcedToStop)
        {
            break;
        }
    }

    delete [] moves;

RETURN:
    return best_move_and_result;
}


_MoveAndResult Connect4Ai::minmaxAlphaBeta(FourInARow::Game* pGame, int depth, int minOrMax, int alpha, int beta)
{
    int current_game_status = pGame->GetGameStatus();

    _MoveAndResult best_move_and_result;
    best_move_and_result.move  = -1;
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
    if (depth == 0)
    {
        best_move_and_result.score = 1;
        goto RETURN;
    }


    // generate shuffled moves array
    int* moves;
    moves = new int [board_width];
    for (int i = 0; i < board_width; i++)
    {
        moves[i] = i;
    }
    _ShuffleArr(moves, board_width);

    for (int i = 0; i < board_width; i++)
    {
        int move = moves[i];
        int result = pGame->PutCoin(move);

        if (result == 2)
        {
            continue;
        }
        if (result == 1 ||
            result == 3)
        {
            printf("this can't be heppen!\n");
            break;
        }


        int next_score = minmaxAlphaBeta(pGame, depth - 1, opposite_of_minOrMax, alpha, beta).score;

        switch (minOrMax)
        {
            case _MINMAX_MIN:
                if (next_score < best_move_and_result.score)
                {
                    best_move_and_result.score = next_score;
                    best_move_and_result.move  = move;
                }
                if (best_move_and_result.score < beta)
                {
                    beta = next_score;
                }
                break;
            case _MINMAX_MAX:
                if (next_score > best_move_and_result.score)
                {
                    best_move_and_result.score = next_score;
                    best_move_and_result.move  = move;
                }
                if (best_move_and_result.score > alpha)
                {
                    alpha = next_score;
                }
                break;
        }
        if (i == 0)
        {
            best_move_and_result.score = next_score;
            best_move_and_result.move  = move;
        }

        pGame->Undo();

        if (alpha >= beta)
        {
            break;
        }
        if (m_IsForcedToStop)
        {
            break;
        }
    }

    delete [] moves;

RETURN:
    return best_move_and_result;
}
