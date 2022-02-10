
#include "engine.hpp"

#define INCLUDE_4INAROW
#include "game/minigames.hpp"

#include <stdlib.h>

int main()
{
    FourInARow::Game game(7, 6);

    while (1)
    {
        // player turn
        game.Print(true, 2, 2);

        for (bool was_valid_input = false; was_valid_input == false; )
        {
            was_valid_input = false;

            int input;
            if (scanf("%d", &input) != 1)
            {
                printf("please input valid value\n");
                continue;
            }

            int result = game.PutCoin(input);
            switch(result)
            {
                case 0:
                    was_valid_input = true;
                    break;
                case 1:
                    printf("error : this can't be occur!\n");
                    break;
                case 2:
                    printf("error : the column is full.\n");
                    break;
                case 3:
                    printf("error : wrong column.\n");
                    break;
            }
        }

        if (game.GetGameStatus() != 0)
        {
            break;
        }

        // bot turn
        int bot_result = game.PutCoin(ChooseNext(&game));
        switch (bot_result)
        {
            case 0:
                break;
            case 1:
                printf("bot error : this can't be occur!\n");
                exit(1);
            case 2:
                printf("bot error : the column is full.\n");
                exit(1);
            case 3:
                printf("bot error : wrong column.\n");
                exit(1);
        }

        if (game.GetGameStatus() != 0)
        {
            break;
        }
    }
    game.Print(true, 2, 2);

    int game_status = game.GetGameStatus();
    switch (game_status)
    {
        case 1:
            printf("player won\n");
            break;
        case 2:
            printf("bot won\n");
            break;
        case 3:
            printf("draw\n");
            break;
    }
}

