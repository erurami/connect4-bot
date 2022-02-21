
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

        void SetThinkMethod(int method) {m_ThinkMethod = method;};
        void SetThinkDepth(int depth) {m_ThinkDepth = depth;};

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

        _MoveAndResult minmax(
                FourInARow::Game* pGame,
                int depth, int minOrMax);

        _MoveAndResult minmaxAlphaBeta(
                FourInARow::Game* pGame,
                int depth, int minOrMax,
                int alpha = -1,
                int beta  = 3);


        // -3 : game not set
        // -2 : thinking not started
        // -1 : thinking
        //  0 : finished
        int m_ThinkingStepNow;

        bool m_IsForcedToStop;

        FourInARow::Game* m_pGame;

        int m_ThinkResult;

        static int m_ThinkMethod;
        static int m_ThinkDepth;

};


#include "engine.ipp"

