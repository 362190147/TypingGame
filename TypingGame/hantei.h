#ifndef HANTEI
#define HANTEI
#define SCRIPTMAX 5000
#define MAXNUM 100
#define MAXLENGTH 100
#define BGNUM 100
#define key(A) Key[KEY_INPUT_##A][1] 

#ifdef UNICODE
#define tsprintf wsprintf
#else
#define tsprintf sprintf
#endif // UNICODE

///窗口大小
extern int Width;
extern int Height;
struct SongDate {
	int highScore=0;
	int totalTime = 0;
	int totalMin = 0;
	int totalSec = 0;
	int totalSec2 = 0;
	TCHAR fileName[MAXNUM];
	TCHAR filePath[MAXNUM];
	TCHAR title[MAXNUM];
	TCHAR artist[MAXNUM];
};
extern SongDate song[MAXNUM];

//字体相关
struct MojiSruct{
	int font = 0;
	int size = 0;
	int x = 0;
	int y = 0;
	int y0 = 0;
};
extern MojiSruct title1;
extern MojiSruct title2;
extern MojiSruct artist1;
extern MojiSruct artist2;
extern MojiSruct lyric;
extern MojiSruct lyricKana;
extern MojiSruct lyricChar1;
extern MojiSruct lyricChar2;
extern MojiSruct mojiTime;
extern MojiSruct list;

//线位置
extern int TimeX;
extern int TimeY;
extern int LineStartX;
extern int LineAX;
extern int LineBX;

//错误信息存储变量
extern TCHAR ErrorMessage[100];
//评分相关
extern int Combo;
extern int MaxCombo;
extern int Score;
extern int Complete;
extern int Solve;
extern int corrected;
extern int Missed;
extern int InputTime;
extern int TypeSpeed;
extern int ClassScore[MAXNUM];//评分标准
extern wchar_t HighClass[MAXNUM][4];
extern wchar_t Class[4];

//音乐相关
extern int MuiscHandle;
extern int Volume;
//音效相关
extern int BurstLinkSE;
extern int SE[50];
//视频相关
extern int VideoFlag;
//時間
extern int Ready;
extern int StartTime;
extern int PassTime;
extern int hanteiTimeOld;
extern int hanteiTime;
extern int hanteiTime2;
extern int Interval;
//脚本
extern int ScriptX;
extern int ScriptX2;
extern int ScriptNum;
extern wchar_t Script[SCRIPTMAX];
//歌詞
extern wchar_t Lyric[MAXLENGTH];
extern wchar_t LyricKana[MAXLENGTH];
extern wchar_t LyricChar[MAXLENGTH*2];
extern int LyricKanaX;
extern int LyricCharX;
extern int Kana2;
extern wchar_t Lyric2[MAXLENGTH];
extern wchar_t LyricKana2[MAXLENGTH];
extern int LyricClear;
extern int LyricClear2;
//背景图片相关
extern int TitleBG;
extern int BGHandle[100];
extern int BGNum;
extern int BGloop;
extern int BGloopTime;
extern int BGloopTimeNow;
//命令
extern wchar_t command[100];
//文件信息相关变量
extern int LoadedFileN;//读取文件的数量
extern int FileNum;//当前读取的文件
//按键相关
extern char KeyBuf[256];
extern int Key[256][4];
extern int keyhantei;
extern int MouseX;
extern int MouseY;
extern int MousePL;
extern int MousePR;
extern int MouseFL;
extern int MouseFR;
extern int MouseCL;
extern int MouseCR;
//透明度READY
extern int Alpha;

extern char* UnicodeToUtf8(const wchar_t* unicode);
//游戏模式
extern int mode;//0为打字模式，1为方向键模式

extern void MouseLoop();
extern void KeyBoard(char KeyBuf[], int Keys[][4]);
extern  int CreateList(TCHAR *DirName,TCHAR *ListName);
extern  int LoadScript(TCHAR *FileName,wchar_t ScriptBuf[],int *ScriptBufX,int MaxNum);
extern  int LoadList(TCHAR* DirName, TCHAR* ListPath,SongDate* songs);

extern  int display();
extern  int hantei();

#endif