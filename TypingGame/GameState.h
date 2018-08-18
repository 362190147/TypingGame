#ifdef __ANDROID__
#include "stdlib.h"
#endif // 

#ifndef GAMESTATE
#define GAMESTATE
#define GAMEEND 0
#define GAMEINIT 1
#define GAMELOGO 2
#define GAMEMENU 3
#define GAMEPLAY 4
#define GAMESCORE 5 
extern int GameState;
extern int GameInit();
extern int GameLogo();
extern int GameMenu();
extern int GamePlay();
extern int GameScore();

#endif