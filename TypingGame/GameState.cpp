#include"DxLib.h"
#include "GameState.h"
#include"hantei.h"
#include "sql.h"
#ifndef __ANDROID__
#include"stdlib.h"
#endif // 


int GameState = 1;

int GameInit()
{
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(true);
	//创建文件列表
	if (CreateList(TEXT("data"), TEXT("list.txt")) == -1) { 
		GameState = GAMEEND;
		return -1; 
	}
	//读取列表中的文件
	LoadedFileN = LoadList(TEXT("data"), TEXT("list.txt"), song);
	if (LoadedFileN < 0) { GameState = GAMEEND; return -1; }
	//读取背景图片
	BGHandle[0] = LoadGraph(_T("bg/title.jpg"));
	//创建字体
	title1.size = Width / 30;
	title2.size = Width / 50;
	artist1.size = Width / 60;
	artist2.size = Width / 70;
	lyric.size = Width / 40;
	lyricKana.size = Width / 70;
	lyricChar1.size = Width / 30;
	lyricChar2.size = Width / 50;
	mojiTime.size = Width / 60;

	title1.font = CreateFontToHandle(NULL, title1.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	title2.font = CreateFontToHandle(NULL, title2.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	artist1.font = CreateFontToHandle(NULL, artist1.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	artist2.font = CreateFontToHandle(NULL, artist2.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	lyric.font = CreateFontToHandle(NULL, lyric.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	lyricKana.font = CreateFontToHandle(NULL, lyricKana.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	lyricChar1.font = CreateFontToHandle(NULL, lyricChar1.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	lyricChar2.font = CreateFontToHandle(NULL, lyricChar2.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	mojiTime.font = CreateFontToHandle(NULL, mojiTime.size, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	//lyricChar2.font = title2.font;
	//初始化位置
	title1.x = Width / 20;
	title1.y0 = Height / 2;

	title1.y = title1.y0;
	list.x = Width / 10 * 6;
	list.y0 = Height / 10 * 4;
	list.y = list.y0 - FileNum;
	lyric.x = Width / 10;
	lyric.y = Height / 10 * 8;
	lyricKana.x= lyric.x;
	lyricKana.y = lyric.y - lyricKana.size;
	lyricChar1.x = Width / 10;
	lyricChar1.y = Height / 10 * 9;
	TimeX = Width / 3 * 2;
	TimeY = Height / 3 * 2;
	LineStartX = 0;
	LineAX = Width / 2;
	LineBX = list.x;
	//初始化歌词
	memset(Lyric, 0, sizeof(wchar_t) * 100);
	memset(LyricKana, 0, sizeof(wchar_t) * 100);
	memset(LyricChar, 0, sizeof(wchar_t) * 200);

	//初始化声量
	Volume = 50;

	////音效相关
	BurstLinkSE = LoadSoundMem(_T("se\\BurstLink.ogg"));;
	SE[1] = LoadSoundMem(_T("se\\1.ogg"));
	SE[2] = LoadSoundMem(_T("se\\2.ogg"));
	ChangeVolumeSoundMem(100, BurstLinkSE);
	ChangeVolumeSoundMem(100, SE[1]);
	ChangeVolumeSoundMem(100, SE[2]);
	//进入LOGO
	GameState = GAMELOGO;
	return 0;
}

int GameLogo() {
	GameState = GAMEMENU;
	return 0;
}

int GameMenu()
{   //按键选择
	if (key(UP)) { FileNum--; if (FileNum < 0)FileNum = LoadedFileN - 1; PlaySoundMem(SE[1], DX_PLAYTYPE_BACK); }
	if (key(DOWN)) { FileNum++; if (FileNum >= LoadedFileN)FileNum = 0; PlaySoundMem(SE[1], DX_PLAYTYPE_BACK); }
	//计算位置
	//title1.x=Width/10*1;
	if (title1.y < title1.y0)	title1.y += 10;
	if (list.x > Width / 10 * 6)	list.x -= 10;
	if (list.y > list.y0 - FileNum*(title2.size + artist2.size + 10))list.y -= 10;
	if (list.y < list.y0 - FileNum*(title2.size + artist2.size + 10))list.y += 10;
	if (LineStartX > 0)LineStartX -= 20;
	//滚轮控制
	int MouseWheelRotVol = GetMouseWheelRotVol();
	if (MouseWheelRotVol + FileNum < 0)FileNum = 0;
	else if (MouseWheelRotVol + FileNum > LoadedFileN - 1)FileNum = LoadedFileN - 1;
	else FileNum += MouseWheelRotVol;
	//进入游戏页面
	if (key(SPACE) || key(RETURN))
	{
		ScriptNum = LoadScript(song[FileNum].filePath, Script, &ScriptX, SCRIPTMAX);
		GameState = GAMEPLAY;
		PlaySoundMem(SE[2], DX_PLAYTYPE_BACK);
	}
	return 0;
}

int GamePlay()
{	 //计算位置
	//if(title1.x<Width/10*1);
	if (title1.y > Height / 10)title1.y -= 10;//计算选中标题坐标
	if (list.x < Width)list.x += 20;//计算列表坐标
	if (LineStartX < Width)LineStartX += 20;
	if (display() == -1)return -1;
	if (hantei() == -1)return -1;
	if (PassTime > song[FileNum].totalTime || Key[KEY_INPUT_ESCAPE][1] != 0)
	{
		GameState = GAMESCORE;
	}
	return 0;
}

void drawScore(int x, int y,TCHAR *text ) {
	
	DrawStringToHandle(x, y, text, 0xffffff, title2.font, 0x9999bb);
}

int GameScore()
{
	int startX = Width / 2;
	int startX2 = Width / 4;
	int startY = Height / 4;
	if (Score > song[FileNum].highScore) {
		song[FileNum].highScore = Score;
		HighClass[FileNum][0] = Class[0];
		HighClass[FileNum][1] = Class[1];
		HighClass[FileNum][2] = Class[2];
		TCHAR temp[100] = { 0 };
#ifdef X64
		char *title = UnicodeToUtf8(song[FileNum].title);
		setScore(title, "test", Score);
		free(title);
#endif
	}


	TCHAR text[50] = { 0 };
	tsprintf(text, _T("Class%s"), Class);
	DrawStringToHandle(startX - 4 * title1.size, startY, text, 0xffffff, title1.font, 0x9999bb);

	tsprintf(text, _T("Score: %d"), Score);
	drawScore(startX2, startY + title1.size, text);

	tsprintf(text, _T("MaxCombo: %d"), MaxCombo);
	drawScore(startX2, startY + title1.size * 2, text);

	tsprintf(text, _T("TypeSpeed: %d/Min"), TypeSpeed);
	drawScore(startX, startY + title1.size * 2, text);

	tsprintf(text, _T("Corrected: %d"), corrected);
	drawScore(startX2, startY + title1.size * 3 , text);

	int all = 0;
	if ((Missed + corrected + Solve) != 0)
		all=corrected * 100 / (Missed + corrected + Solve);
	tsprintf(text, _T("Corrected Precent: %d%%"), all);
	drawScore(startX, startY + title1.size * 3, text);

	tsprintf(text, _T("Missed: %d"), Missed);
	drawScore(startX2, startY + title1.size * 4, text);

	
	if ((Missed + corrected) != 0)
		all = corrected * 100 / (Missed + corrected);
	tsprintf(text, _T("Corrected Precent(Cut Solve): %d%%"), all);
	drawScore(startX, startY + title1.size * 4, text);

	tsprintf(text, _T("Complete: %d"), Complete);
	drawScore(startX2, startY + title1.size * 5, text);
	
	tsprintf(text, _T("Solve: %d"), Solve);
	drawScore(startX2, startY + title1.size * 6, text);


	if (key(SPACE) || key(RETURN))
	{//初始化评分相关变量
		VideoFlag = 0;
		//评分相关
		Combo = 0;
		MaxCombo = 0;
		Score = 0;
		Complete = 0;
		Solve = 0;
		corrected = 0;
		Missed = 0;
		InputTime = 0;
		TypeSpeed = 0;
		memset(Class, 0, sizeof(wchar_t) * 4);
		//时间相关变量
		StartTime = 0;
		PassTime = 0;
		hanteiTimeOld = 0;
		hanteiTime = 0;
		hanteiTime2 = 0;
		Interval = 0;
		//
		ScriptX = 0;
		ScriptX2 = 0;
		//删除音乐
		StopSoundMem(MuiscHandle);
		DeleteSoundMem(MuiscHandle);
		//停止播放
		if (GetMovieStateToGraph(BGHandle[BGNum]) == 1)PauseMovieToGraph(BGHandle[BGNum]);
		//删除图片

		for (int i = 0; i < BGNUM; i++)
		{
			if (BGHandle[i] != 0 && BGHandle[i] != -1)
			{
				DeleteGraph(BGHandle[i]);
			}
		}

		//背景序数为0。循环图片数量为0，循环时间为0
		BGNum = 0;
		BGloop = 0;
		BGloopTime = 30000;
		BGloopTimeNow = 0;
		//读取菜单背景
		BGHandle[BGNum] = LoadGraph(_T("bg/title.jpg"));
		
		//删除歌词
		memset(Lyric, 0, sizeof(wchar_t) * 100);
		memset(LyricKana, 0, sizeof(wchar_t) * 100);
		memset(LyricChar, 0, sizeof(wchar_t) * 200);

		//准备文字的Alpha的值；
		Alpha = 0;
		GameState = GAMEMENU;
	}
	return 0;
}