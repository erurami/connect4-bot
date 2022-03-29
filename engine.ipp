
#pragma once

#include "engine.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


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

#include "minmax.hpp"
#include "minmaxAB.hpp"
#include "minmaxABPlus.hpp"



// ==================== Connect4Ai =====================

DWORD WINAPI _Run_Connect4Ai_ChooseNext(LPVOID lpParam)
{
    ((Connect4Ai*)lpParam)->_ChooseNext();
    return 0;
}


int Connect4Ai::m_ThinkMethod = ENGINE_ALPHABETAPLUS;
int Connect4Ai::m_ThinkDepth  = 10;

Connect4Ai::Connect4Ai()
{
    m_ThinkingStepNow = -3;
    m_pGame = NULL;
    m_ThinkResult = -1;
    m_pDecider = NULL;
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

    DWORD dw_id;
    CreateThread(0, 0, _Run_Connect4Ai_ChooseNext, (LPVOID)this, 0, &dw_id);
}

void Connect4Ai::KillThinkingProcess(void)
{
    if (m_ThinkingStepNow != -1)
    {
        return;
    }

    if (m_pDecider == NULL)
    {
        return;
    }

    switch (m_ThinkMethod)
    {
        case ENGINE_MINMAX:
            ((MinmaxDecider*)m_pDecider)->Kill();
            break;
        case ENGINE_ALPHABETA:
            ((MinmaxABDecider*)m_pDecider)->Kill();
            break;
        case ENGINE_ALPHABETAPLUS:
            ((MinmaxABDecider*)m_pDecider)->Kill();
            break;
    }

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
            {
            m_pDecider = (void*)(new MinmaxDecider);
            m_ThinkResult = ((MinmaxDecider*)m_pDecider)->Decide(m_pGame, m_ThinkDepth);
            delete m_pDecider;
            m_pDecider = NULL;
            break;
            }
        case ENGINE_ALPHABETA:
            {
            m_pDecider = (void*)(new MinmaxABDecider);
            m_ThinkResult = ((MinmaxABDecider*)m_pDecider)->Decide(m_pGame, m_ThinkDepth);
            delete m_pDecider;
            m_pDecider = NULL;
            break;
            }
        case ENGINE_ALPHABETAPLUS:
            {
            m_pDecider = (void*)(new MinmaxABPlusDecider);
            m_ThinkResult = ((MinmaxABPlusDecider*)m_pDecider)->Decide(m_pGame, m_ThinkDepth);
            delete m_pDecider;
            m_pDecider = NULL;
            break;
            }
    }

    m_ThinkingStepNow = 0;
}

