#pragma once
#ifdef X64
extern int sqlconnect();
extern void sqlClose();
extern int setScore(char *title, char*name, int score);
extern int getScore(char *title);
#endif