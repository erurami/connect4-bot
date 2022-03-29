
#pragma once

#define INCLUDE_4INAROW
#include "game/minigames.hpp"

#define ENGINE_ALPHABETA 1
#define ENGINE_MINMAX    2

int ChooseNext(FourInARow::Game *pGameBoard);



typedef struct __MoveAndResult
{
    int move;
    int score;
} _MoveAndResult;

class Connect4Ai
{
    public:

        static void SetThinkMethod(int method) {m_ThinkMethod = method;};
        static void SetThinkDepth(int depth) {m_ThinkDepth = depth;};

        Connect4Ai();
        Connect4Ai(const FourInARow::Game *pGameBoard);
        ~Connect4Ai();

        void SetGame(const FourInARow::Game *pGameBoard);

        void StartThinkingNextMove(void);
        void KillThinkingProcess(void);
        void WaitForFinish(void);

        // returns : 
        //    >=0 : result
        //    -1  : thinking not finished 
        //          (use KillThinkingProcess() to finish)
        //    -2  : thinking not started
        //          (use StartThinkingNextMove() to start)
        //    -3  : game not set
        //          (use SetGameNow() to set)
        int GetResult(void);

        void _ChooseNext(void);

    private:

        // -3 : game not set
        // -2 : thinking not started
        // -1 : thinking
        //  0 : finished
        int m_ThinkingStepNow;

        FourInARow::Game* m_pGame;

        void* m_pDecider;

        int m_ThinkResult;

        static int m_ThinkMethod;
        static int m_ThinkDepth;

};



#include "engine.ipp"

