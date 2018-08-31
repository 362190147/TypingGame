#include"DxLib.h"
#include"hantei.h"
#include"stdlib.h"
#include"math.h"
#include"malloc.h"
#include"sql.h";
int Width = 1208;
int Height = 690;
SongDate song[MAXNUM];
//字体相关
MojiSruct title1;
MojiSruct title2;
MojiSruct artist1;
MojiSruct artist2;
MojiSruct lyric;
MojiSruct lyricKana;
MojiSruct lyricChar1;
MojiSruct lyricChar2;
MojiSruct mojiTime;
MojiSruct list;

//文字位置
int TimeX = 0;
int TimeY = 0;

int LineStartX = 0;
int LineAX = 0;
int LineBX = 0;
//线坐标
int TotalLineX = 0;
int TotalLineY = 0;
int IntervalX = 0;
int IntervalY = 0;
//文字颜色
//int KanaColor = 0;
const int white = 0xffffff;
//错误信息存储变量
TCHAR ErrorMessage[100];
//评分相关
int Combo = 0;
int MaxCombo = 0;
int Score = 0;
//int HighScore[MAXNUM] = { 0 };
int Complete = 0;
int Solve = 0;
int corrected = 0;
int Missed = 0;
int InputTime = 0;
int TypeSpeed = 0;
int ClassScore[MAXNUM] = { 0 };//评分标准
wchar_t HighClass[MAXNUM][4] = { 0 };
wchar_t Class[4] = { 0 };
//音乐相关
int MuiscHandle = 0;
int Volume = 0;
//音效相关
int BurstLinkSE;
int SE[50];
//视频相关
int VideoFlag = 0;
//時間
int Ready = 0;
int StartTime = 0;
int PassTime = 0;
int hanteiTimeOld = 0;
int hanteiTime = 0;
int hanteiTime2 = 0;
int Interval = 0;
int Total = 0;
wchar_t Time[50] = { 0 };
int TimeLine=0;//时间线长度
//脚本
int ScriptX=0;
int ScriptX2=0;
int ScriptNum=0;
wchar_t Script[SCRIPTMAX]={0};
//歌詞
wchar_t Lyric[MAXLENGTH]={0};
wchar_t LyricKana[MAXLENGTH]={0};
wchar_t LyricChar[MAXLENGTH*2]={0};
int LyricKanaX=0;
int LyricCharX=0;
int Kana2=0;
wchar_t Lyric2[MAXLENGTH]={0};
wchar_t LyricKana2[MAXLENGTH]={0};
int LyricClear=0;
int LyricClear2=0;
//背景图片相关
int TitleBG=0;
int BGHandle[BGNUM]={0};
int BGNum=0;
int BGloop=0;
int BGloopTime=30000;
int BGloopTimeNow=0;
//命令
wchar_t command[100];
//文件信息相关变量
int LoadedFileN=0;//读取文件的数量
int FileNum=0;//当前读取的文件
//按键相关
char KeyBuf[256];
int Key[256][4];
int keyhantei = 0;
int MouseX =    0;
int MouseY =    0;
int MousePL =   0;
int MousePR =   0;
int MouseFL =   0;
int MouseFR =   0;
int MouseCL =   0;
int MouseCR =   0;
//透明度
int Alpha=0;
//游戏模式
int mode=0;//0为打字模式，1为方向键模式

char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

int GetTchar(TCHAR *tchar, int max)
{
	int i;
	for (i = 0; i <= max; i++)
	{
		if (tchar[i] == 0)
		{
			break;
		}
	}
	return i;
}

int GetFile(TCHAR *Path)
{	
	WIN32_FIND_DATA FindData;
	TCHAR			FullName[256] = { 0 };
	_tcscpy ( FullName, Path);
	_tcscpy ( FullName, TEXT("\\*.*"));
	
	HANDLE h = FindFirstFile( FullName, & FindData);
	
	if(h == INVALID_HANDLE_VALUE )
	{
		_tprintf(TEXT("%s"));
		return -1;
	}
	FILE *fp = NULL;
	fp = _tfopen(TEXT("list.txt"),TEXT("wb"));
#ifdef UNICODE
	TCHAR temp[] = {0xfeff};
	fwrite(temp,2,1,fp);
#endif
	while( FindNextFile ( h , & FindData))
	{	
		fwrite(FindData.cFileName,GetTchar(FindData.cFileName,260)*2,1,fp);
		fwrite(TEXT("\r\n"),4,1,fp);
	}
	fclose(fp);
	return 0;
}

void MouseLoop()
{
	GetMousePoint(&MouseX,&MouseY);//マウスの座標を取得
	MousePL=GetMouseInput() & MOUSE_INPUT_LEFT;//マウスの左
	MousePR=GetMouseInput() & MOUSE_INPUT_RIGHT;//マウスの右
	if(MouseFL!=0 && MousePL==0){MouseCL=MouseFL;}else{MouseCL=0;}
	if(MouseFR!=0 && MousePR==0){MouseCR=MouseFR;}else{MouseCR=0;}
	MouseFL=MousePL;
	MouseFR=MousePR;

}

//键盘按键检测
void KeyBoard(char KeyBuf[],int Keys[][4])
{	GetHitKeyStateAll(KeyBuf);
	for(int i=0;i<256;i++)
	{
		if(Keys[i][0]==0&&KeyBuf[i]!=0)
			{Keys[i][1]=1;}
		else
			{Keys[i][1]=0;}
		Keys[i][0]=KeyBuf[i];
		Keys[i][2]=0;
		if (Keys[i][1]!=0)
		{
			if(Keys[i][3]+500>GetNowCount() )
			{Keys[i][2]=Keys[i][1];Keys[i][3]=0;} 
			else 
			Keys[i][3]=GetNowCount();
		}
		
	}
}

//script的第scriptX个开始是否与cmd相等
int tCharCmp(TCHAR* script, TCHAR* cmd, int scriptX) {
	int result = 0;
	int length = 0;
	while (cmd[result]!=_T('\0')){
		if (script[scriptX + result] != cmd[result]) {
			return 0;
		}
		result++;
	}
	return result;
}

int wChartoChar(wchar_t *wchar,char *charbuf,int Num)
{
	for(int i=0;i<Num;i++)
	{
		charbuf[i]=(char)wchar[i];
	}
	return 0;
}

//将脚本的数字读出
int ScriptCheck(wchar_t ScriptBuf[],int *ScriptBufX)///今のScriptXから数値と認識し値を返す。（ScriptXはCHAR Aの後に移動する。）
{	int i=0,keta=0,kazu=0;
	while(ScriptBuf[*ScriptBufX+i]>=L'0'&&ScriptBuf[*ScriptBufX+i]<=L'9'&&i<100){i++;keta++;}
	
	for(i=0;i<keta;i++)
	{
		kazu+=(ScriptBuf[*ScriptBufX+i]-L'0')*pow((double)10,keta-i-1);
	}
	*ScriptBufX+=keta+1;
	return kazu;
}

//读取信息
void LoadMassage(wchar_t *ScriptBuf,int *ScriptX,wchar_t *LoadBuf,int MaxNum)
{	memset(LoadBuf,0,sizeof(wchar_t)*MaxNum);
	int n=0;
	for(int i=0;i<MaxNum;i++)
	{	
		if (ScriptBuf[*ScriptX + i] != L'['&&ScriptBuf[*ScriptX + i] != L']'&&ScriptBuf[*ScriptX + i] != L';')
		{LoadBuf[i]=ScriptBuf[*ScriptX+i];}
		else
			{n=i;i=MaxNum;}
	}
	if(ScriptBuf[*ScriptX+n]==L'[')
	*ScriptX+=n;
	else
	*ScriptX+=++n;
}

void LoadLyric(wchar_t *ScriptBuf, int *ScriptX, wchar_t *lyric,wchar_t *lyricKana, int MaxNum)
{
	memset(lyric, 0, sizeof(wchar_t)*MaxNum);
	memset(lyricKana, 0, sizeof(wchar_t)*MaxNum);
	int n = 0;
	int l = 0;
	int k = 0;
	for (int i = 0; i<MaxNum; i++)
	{
		if (ScriptBuf[*ScriptX + i] != L'['&&ScriptBuf[*ScriptX + i] != L']'&&ScriptBuf[*ScriptX + i] != L';')
		{
			//
			if (ScriptBuf[*ScriptX + i] == L'(') {
				lyric[l++] = ScriptBuf[*ScriptX + i++];
				while (ScriptBuf[*ScriptX + i] != L')') {
					lyricKana[k++] = ScriptBuf[*ScriptX + i];
					lyric[l++] = ScriptBuf[*ScriptX + i++];
				}
				lyric[l++] = ScriptBuf[*ScriptX + i];
				i++;
				if (ScriptBuf[*ScriptX + i] == L'[') { n = i; break; }
			}

			if ((ScriptBuf[*ScriptX + i] >= L'ぁ'&&ScriptBuf[*ScriptX + i] <= L'ヺ')
				|| (ScriptBuf[*ScriptX + i] >= L'A'&&ScriptBuf[*ScriptX + i] <= L'Z')
				|| (ScriptBuf[*ScriptX + i] >= L'a'&&ScriptBuf[*ScriptX + i] <= L'z')
				|| (ScriptBuf[*ScriptX + i] >= L'Ａ'&&ScriptBuf[*ScriptX + i] <= L'Ｚ')
				|| (ScriptBuf[*ScriptX + i] >= L'ａ'&&ScriptBuf[*ScriptX + i] <= L'ｚ')
				|| (ScriptBuf[*ScriptX + i] >= L'0'&&ScriptBuf[*ScriptX + i] <= L'9')
				|| (ScriptBuf[*ScriptX + i] >= L'０'&&ScriptBuf[*ScriptX + i] <= L'９')
				) {
				lyricKana[k++] = ScriptBuf[*ScriptX + i];
				lyric[l++] = ScriptBuf[*ScriptX + i];
			}
			else
			{
				lyric[l++] = ScriptBuf[*ScriptX + i];
			}
			
		}
		else
		{
			n = i; i = MaxNum;
		}
	}
	if (ScriptBuf[*ScriptX + n] == L'[')
		*ScriptX += n;
	else
		*ScriptX += ++n;
}

//读取脚本
int LoadScript(TCHAR *FileName,wchar_t ScriptBuf[],int *ScriptBufX,int MaxNum)
{	memset( ScriptBuf,0,sizeof(wchar_t)*MaxNum);
	//FILE *fp=NULL;
	int fp = 0;
	wchar_t wcTemp=0;
	int LoadedNum=0;
	
	if(FileName!=NULL)
	{
		//fp=_tfopen(FileName,TEXT("rb"));
		fp = FileRead_open(FileName);
		if(fp!=NULL)
		{	
			//wcTemp=fgetwc(fp);
			FileRead_read(&wcTemp, 2, fp);
			
			//unicode判定
			if(wcTemp!=0xfeff) {	
				tsprintf(ErrorMessage,_T("%sはUnicodeファイルではありません"), FileName);
#ifdef WIN32
				MessageBox(NULL, ErrorMessage, _T("error"), MB_ICONERROR);
#endif // WIN32	
				return -1;
			}
			else {
			
			
			}

			//loadtitle
			int i,a=0;
			for( i=0;i<MaxNum;i++)
			{
				//ScriptBuf[i]=fgetwc(fp);
				FileRead_read(&(ScriptBuf[i]), 2, fp);
				//LoadTextNum++;
				if(ScriptBuf[i]==L'\n')//忽略换行。
				{
					ScriptBuf[i]=0;
					a=0;
					i-=2;
				}
				else if(ScriptBuf[i-1]==L'/'&&Script[i]==L'/')
				{
					ScriptBuf[i]=0;
					a=1;
					i--;
				}
				else if (ScriptBuf[i] == WEOF)
				{
					ScriptBuf[i] = 0; break;
				}
				else if (a == 1)
				{
					ScriptBuf[i] = 0; i--;
				}
			}
			LoadedNum=i;
			FileRead_close(fp);
			ScriptBufX=0;
		}
		else{	
			swprintf(ErrorMessage,_T("%sファイルがオーブンできません"),FileName);
			MessageBox(NULL,ErrorMessage,_T("error"),MB_ICONERROR);
			return -1;
		}
	}
	else{
		swprintf(ErrorMessage,_T("%sファイルがありません"),FileName);
		MessageBox(NULL,ErrorMessage,_T("error"),MB_ICONERROR);
		return -1;
	}
	return LoadedNum;
}

void FindInAll(TCHAR *Path,FILE *fp) {
	WIN32_FIND_DATA FindData;
	TCHAR find[MAX_PATH] = {0};
	_tcscpy(find, Path);
	_tcscat(find, TEXT("\\*"));
	HANDLE h = FindFirstFile(find, &FindData);
	if (h == INVALID_HANDLE_VALUE) {
		swprintf(ErrorMessage, TEXT("%s没有文件"), Path);
		MessageBox(NULL, ErrorMessage, TEXT("error"), MB_ICONERROR);
		return;
	}

	do
	{
		if (FindData.cFileName[0] == '.') //过滤本级目录和父目录
			continue;

		TCHAR szFile[MAX_PATH] = { 0 };
		_tcscpy(szFile, Path);
		_tcscat(szFile, TEXT("\\"));
		_tcscat(szFile, FindData.cFileName);
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果找到的是目录，则进入此目录进行递归
		{
			FindInAll(szFile, fp);
		}
		else //找到的是文件
		{
			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
			_tsplitpath(szFile, drive, dir, fname, ext);
			if (lstrcmp(ext, _T(".txt")) == 0|| lstrcmp(ext, _T(".lrc")) == 0) {
				fwrite(szFile, GetTchar(szFile, MAX_PATH) * 2, 1, fp);
				fwrite(TEXT("\r\n"), 4, 1, fp);
			}
		}
	} while (FindNextFile(h, &FindData));
	FindClose(h);

}

//创建文件列表
int CreateList(TCHAR *DirName,TCHAR *ListName){
	FILE *fp = NULL;
	fp = _tfopen( ListName,TEXT("wb") );
	TCHAR temp = 0xfeff;
	fwrite(&temp, 2, 1, fp);
	FindInAll(DirName, fp);
	fclose(fp);
	return 0;
}

//读取最后时间
void LoadTotalTime(wchar_t  script[],int scriptn,int *Time,int *Min,int *Sec,int *Sec2)
{	
	wchar_t Temp[50]={0};
	int X=0;
	int i=0;
	while(script[scriptn-i]!=L'[')
	{
		i++;
	}
	X=scriptn-i;
	i=0;
	for(i=0;script[X+i]!=L']';i++)
	{
		Temp[i]=script[X+i];
	}
	X=1;
	*Min=ScriptCheck(Temp,&X);
	*Sec=ScriptCheck(Temp,&X);
	*Sec2=ScriptCheck(Temp,&X);
	*Time=*Min*60000+*Sec*1000+*Sec*10;
}

//拗音
void youon(wchar_t *a,wchar_t *b,int *i,int *n)
{	
	switch(a[*i+1])
	{
		case L'ゃ':{b[(*n)++]=L'Y';b[(*n)++]=L'A';(*i)++;}
			break;
		case L'ゅ':{b[(*n)++]=L'Y';b[(*n)++]=L'U';(*i)++;}
			break;
		case L'ょ':{b[(*n)++]=L'Y';b[(*n)++]=L'O';(*i)++;}
			break;
		default:{b[(*n)++]=L'I';}
			break;
	}
}

//将假名转为字母
int KanaToChar(wchar_t a[],wchar_t b[],int Num)
{	int n=0;
	memset(b,0,sizeof(wchar_t)*Num*2);
	for(int i=0;i<Num;i++)
	{
		switch(a[i])
		{
			case L'あ':case L'ア':b[n++]=L'A';
				break;
			case L'い':case L'イ':b[n++]=L'I';
				break;
			case L'う':case L'ウ':b[n++]=L'U';
				break;
			case L'え':case L'エ':b[n++]=L'E';
				break;
			case L'お':case L'オ':b[n++]=L'O';
				break;
			case L'か':case L'カ': {b[n++]=L'K';b[n++]=L'A';}
				break;
			case L'き':case L'キ': {b[n++]=L'K';youon(a,b,&i,&n);}
				break;
			case L'く':case L'ク': {b[n++]=L'K';b[n++]=L'U';}
				break;
			case L'け':case L'ケ': {b[n++]=L'K';b[n++]=L'E';}
				break;
			case L'こ':case L'コ': {b[n++]=L'K';b[n++]=L'O';}
				break;
			case L'が':case L'ガ': {b[n++]=L'G';b[n++]=L'A';}
				break;
			case L'ぎ':case L'ギ': {b[n++]=L'G';youon(a,b,&i,&n);}
				break;
			case L'ぐ':case L'グ': {b[n++]=L'G';b[n++]=L'U';}
				break;
			case L'げ':case L'ゲ': {b[n++]=L'G';b[n++]=L'E';}
				break;
			case L'ご':case L'ゴ': {b[n++]=L'G';b[n++]=L'O';}
				break;
			case L'さ':case L'サ': {b[n++]=L'S';b[n++]=L'A';}
				break;
			case L'し':case L'シ': {b[n++]=L'S';youon(a,b,&i,&n);}
				break;
			case L'す':case L'ス': {b[n++]=L'S';b[n++]=L'U';}
				break;
			case L'せ':case L'セ': {b[n++]=L'S';b[n++]=L'E';}
				break;
			case L'そ':case L'ソ': {b[n++]=L'S';b[n++]=L'O';}
				break;
			case L'ざ':case L'ザ': {b[n++]=L'Z';b[n++]=L'A';}
				break;
			case L'じ':case L'ジ': {b[n++]=L'J';youon(a,b,&i,&n);}
				break;
			case L'ず':case L'ズ': {b[n++]=L'Z';b[n++]=L'U';}
				break;
			case L'ぜ':case L'ゼ': {b[n++]=L'Z';b[n++]=L'E';}
				break;
			case L'ぞ':case L'ゾ': {b[n++]=L'Z';b[n++]=L'O';}
				break;
			case L'た':case L'タ': {b[n++]=L'T';b[n++]=L'A';}
				break;
			case L'ち':case L'チ': {b[n++]=L'C';if(a[i+1]!=L'ゃ'&&a[i+1]!=L'ゅ'&&a[i+1]!=L'ょ'){b[n++]=L'H';}youon(a,b,&i,&n);}
				break;
			case L'つ':case L'ツ': {b[n++]=L'T';b[n++]=L'U';}
				break;
			case L'て':case L'テ': {b[n++]=L'T';b[n++]=L'E';}
				break;
			case L'と':case L'ト': {b[n++]=L'T';b[n++]=L'O';}
				break;
			case L'だ':case L'ダ': {b[n++]=L'D';b[n++]=L'A';}
				break;
			case L'ぢ':case L'ヂ': {b[n++]=L'D';b[n++]=L'I';}
				break;
			case L'づ':case L'ヅ': {b[n++]=L'D';b[n++]=L'U';}
				break;
			case L'で':case L'デ': {b[n++]=L'D';b[n++]=L'E';}
				break;
			case L'ど':case L'ド': {b[n++]=L'D';b[n++]=L'O';}
				break;
			case L'な':case L'ナ': {b[n++]=L'N';b[n++]=L'A';}
				break;
			case L'に':case L'ニ': {b[n++]=L'N';youon(a,b,&i,&n);}
				break;
			case L'ぬ':case L'ヌ': {b[n++]=L'N';b[n++]=L'U';}
				break;
			case L'ね':case L'ネ': {b[n++]=L'N';b[n++]=L'E';}
				break;
			case L'の':case L'ノ': {b[n++]=L'N';b[n++]=L'O';}
				break;
			case L'ん':case L'ン': {b[n++]=L'N';b[n++]=L'N';}
				break;
			case L'は':case L'ハ': {b[n++]=L'H';b[n++]=L'A';}
				break;
			case L'ひ':case L'ヒ': {b[n++]=L'H';youon(a,b,&i,&n);}
				break;
			case L'ふ':case L'フ': {b[n++]=L'F';b[n++]=L'U';}
				break;
			case L'へ':case L'ヘ': {b[n++]=L'H';b[n++]=L'E';}
				break;
			case L'ほ':case L'ホ': {b[n++]=L'H';b[n++]=L'O';}
				break;
			case L'ば':case L'バ': {b[n++]=L'B';b[n++]=L'A';}
				break;
			case L'び':case L'ビ': {b[n++]=L'B';youon(a,b,&i,&n);}
				break;
			case L'ぶ':case L'ブ': {b[n++]=L'B';b[n++]=L'U';}
				break;
			case L'べ':case L'ベ': {b[n++]=L'B';b[n++]=L'E';}
				break;
			case L'ぼ':case L'ボ': {b[n++]=L'B';b[n++]=L'O';}
				break;
			case L'ぱ':case L'パ': {b[n++]=L'P';b[n++]=L'A';}
				break;
			case L'ぴ':case L'ピ': {b[n++]=L'P';youon(a,b,&i,&n);}
				break;
			case L'ぷ':case L'プ': {b[n++]=L'P';b[n++]=L'U';}
				break;
			case L'ぺ':case L'ペ': {b[n++]=L'P';b[n++]=L'E';}
				break;
			case L'ぽ':case L'ポ': {b[n++]=L'P';b[n++]=L'O';}
				break;
			case L'ま':case L'マ': {b[n++]=L'M';b[n++]=L'A';}
				break;
			case L'み':case L'ミ': {b[n++]=L'M';youon(a,b,&i,&n);}
				break;
			case L'む':case L'ム': {b[n++]=L'M';b[n++]=L'U';}
				break;
			case L'め':case L'メ': {b[n++]=L'M';b[n++]=L'E';}
				break;
			case L'も':case L'モ': {b[n++]=L'M';b[n++]=L'O';}
				break;
			case L'や':case L'ヤ': {b[n++]=L'Y';b[n++]=L'A';}
				break;
			case L'ゆ':case L'ユ': {b[n++]=L'Y';b[n++]=L'U';}
				break;
			case L'よ':case L'ヨ': {b[n++]=L'Y';b[n++]=L'O';}
				break;
			case L'ら':case L'ラ': {b[n++]=L'R';b[n++]=L'A';}
				break;
			case L'り':case L'リ': {b[n++]=L'R';youon(a,b,&i,&n);}
				break;
			case L'る':case L'ル': {b[n++]=L'R';b[n++]=L'U';}
				break;
			case L'れ':case L'レ': {b[n++]=L'R';b[n++]=L'E';}
				break;
			case L'ろ':case L'ロ': {b[n++]=L'R';b[n++]=L'O';}
				break;
			case L'わ':case L'ワ': {b[n++]=L'W';b[n++]=L'A';}
				break;
			case L'を':case L'ヲ': {b[n++]=L'W';b[n++]=L'O';}
				break;
			case L'っ':case L'ッ':{
				switch(a[i+1]){
				case L'か':
				case L'き':
				case L'く':
				case L'け':
				case L'こ':
				case L'カ':
				case L'キ':
				case L'ク':
				case L'ケ':
				case L'コ':{b[n++]=L'K';}
					break;
				case L'が':
				case L'ぎ':
				case L'ぐ':
				case L'げ':
				case L'ご':
				case L'ガ':
				case L'ギ':
				case L'グ':
				case L'ゲ':
				case L'ゴ': {b[n++]=L'G';}
					break;
				case L'さ':
				case L'し':
				case L'す':
				case L'せ':
				case L'そ':
				case L'サ':
				case L'シ':
				case L'ス':
				case L'セ':
				case L'ソ': {b[n++]=L'S';}
						break;
				case L'ざ':
				case L'ず':
				case L'ぜ':
				case L'ぞ':{b[n++]=L'Z';}
					break;
				case L'じ':{b[n++]=L'J';}
					break;
				case L'た':
				case L'つ':
				case L'て':
				case L'と':
				case L'タ':
				case L'ツ':
				case L'テ':
				case L'ト':{b[n++]=L'T';}
					break;
				case L'ち':
				case L'チ': {b[n++]=L'C';}
					break;
				case L'だ':
				case L'ぢ':
				case L'づ':
				case L'で':
				case L'ど':{b[n++]=L'D';}
					break;
				case L'ら':
				case L'り':
				case L'る':
				case L'れ':
				case L'ろ':{b[n++]=L'R';}
					break;
				case L'は':
				case L'ひ':
				case L'ふ':
				case L'へ':
				case L'ほ':{b[n++]=L'H';}
					break;
				case L'ば':
				case L'び':
				case L'ぶ':
				case L'べ':
				case L'ぼ':{b[n++]=L'B';}
					break;
				case L'ぱ':
				case L'ぴ':
				case L'ぷ':
				case L'ぺ':
				case L'ぽ':
				case L'パ':
				case L'ピ':
				case L'プ':
				case L'ペ':
				case L'ポ': {b[n++]=L'P';}
					break;
				case L'ま':
				case L'み':
				case L'む':
				case L'め':
				case L'も':{b[n++]=L'M';}
					break;
				case L'や':
				case L'ゆ':
				case L'よ':{b[n++]=L'Y';}
					break;
				case L'わ':
				case L'を':{b[n++]=L'W';}
					break;
				default:
					break;
				}
			}
				break;
			case L'ー':{b[n++]=L'ー';}
				break;
			case L'　':
			case L' ':{b[n++]=L'_';}
				break;
			case L'A':
			case L'a':
			case L'Ａ':
			case L'ａ':{b[n++]=L'A';}
				break;
			case L'B':
			case L'b':
			case L'Ｂ':
			case L'ｂ':{b[n++]=L'B';}
				break;
			case L'C':
			case L'c':
			case L'Ｃ':
			case L'ｃ':{b[n++]=L'C';}
				break;
			case L'D':
			case L'd':
			case L'Ｄ':
			case L'ｄ':{b[n++]=L'D';}
				break;
			case L'E':
			case L'e':
			case L'Ｅ':
			case L'ｅ':{b[n++]=L'E';}
				break;
			case L'f':
			case L'F':
			case L'ｆ':
			case L'Ｆ':{b[n++]=L'F';}
				break;
			case L'G':
			case L'g':
			case L'Ｇ':
			case L'ｇ':{b[n++]=L'G';}
				break;
			case L'H':
			case L'h':
			case L'Ｈ':
			case L'ｈ':{b[n++]=L'H';}
				break;
			case L'I':
			case L'i':
			case L'Ｉ':
			case L'ｉ':{b[n++]=L'I';}
				break;
			case L'j':
			case L'J':
			case L'ｊ':
			case L'Ｊ':{b[n++]=L'J';}
				break;
			case L'K':
			case L'k':
			case L'Ｋ':
			case L'ｋ':{b[n++]=L'K';}
				break;
			case L'L':
			case L'l':
			case L'Ｌ':
			case L'ｌ':{b[n++]=L'L';}
				break;
			case L'Ｎ':
			case L'ｎ':
			case L'N':
			case L'n':{b[n++]=L'N';}
				break;
			case L'M':
			case L'm':
			case L'Ｍ':
			case L'ｍ':{b[n++]=L'M';}
				break;
			case L'O':
			case L'o':
			case L'ｏ':
			case L'Ｏ':{b[n++]=L'O';}
				break;
			case L'P':
			case L'p':
			case L'ｐ':
			case L'Ｐ':{b[n++]=L'P';}
				break;
			case L'q':
			case L'Q':
			case L'Ｑ':
			case L'ｑ':{b[n++]=L'Q';}
				break;
			case L'R':
			case L'r':
			case L'ｒ':
			case L'Ｒ':{b[n++]=L'R';}
				break;
			case L'S':
			case L's':
			case L'Ｓ':
			case L'ｓ':{b[n++]=L'S';}
				break;
			case L'T':
			case L't':
			case L'Ｔ':
			case L'ｔ':{b[n++]=L'T';}
				break;
			case L'U':
			case L'u':
			case L'ｕ':
			case L'Ｕ':{b[n++]=L'U';}
				break;
			case L'V':
			case L'v':
			case L'ｖ':
			case L'Ｖ':{b[n++]=L'V';}
				break;
			case L'W':
			case L'w':
			case L'ｗ':
			case L'Ｗ':{b[n++]=L'W';}
				break;
			case L'X':
			case L'x':
			case L'Ｘ':
			case L'ｘ':{b[n++]=L'X';}
				break;
			case L'Y':
			case L'y':
			case L'ｙ':
			case L'Ｙ':{b[n++]=L'Y';}
				break;
			case L'Z':
			case L'z':
			case L'ｚ':
			case L'Ｚ':{b[n++]=L'Z';}
					  break;
			case L'\'':{b[n++]=L'\'';}
				break;
			case L'1':
			case L'１':{b[n++]=L'1';}
				break;
			case L'2':
			case L'２':{b[n++]=L'2';}
				break;
			case L'3':
			case L'３':{b[n++]=L'3';}
				break;
			case L'4':
			case L'４':{b[n++]=L'4';}
				break;
			case L'5':
			case L'５':{b[n++]=L'5';}
				break;
			case L'6':
			case L'６':{b[n++]=L'6';}
				break;
			case L'7':
			case L'７':{b[n++]=L'7';}
				break;
			case L'8':
			case L'８':{b[n++]=L'8';}
				break;
			case L'9':
			case L'９':{b[n++]=L'9';}
				break;
			case L'0':
			case L'０':{b[n++]=L'0';}
				break;
			case L'↑':{b[n++]=L'↑';}
				break;
			case L'↓':{b[n++]=L'↓';}
				break;
			case L'←':{b[n++]=L'←';}
				break;
			case L'→':{b[n++]=L'→';}
				break;
			default:
				break;
		}
	}
	return n;
}

//读取列表
int  LoadList(TCHAR* DirName, TCHAR *ListPath, SongDate *songs)
{
	FILE *fp = NULL;
	int i, n, FileN = 0;
	TCHAR temp;
	//读取列表文件名
	fp = _tfopen(ListPath, TEXT("rb"));
	if (fp != NULL){
		temp = getwc(fp);
		//判断unicode
		if (temp != 0xfeff) { MessageBox(NULL, TEXT("list不是UNICODE 文件"), TEXT("error"), MB_ICONERROR); return -1; }
		for (n = 0; n < MAXNUM; n++)
		{
			for (i = 0; i < 100; i++)
			{
				temp = fgetwc(fp);
				if (temp == WEOF) { n = MAXNUM; break; }
				if (temp == 13) { getwc(fp); break; }
				//FileName[n][i] = temp;
				songs[n].fileName[i] = temp;
			}
			//{swprintf(ErrorMessage,TEXT("文件名%s"),FileName[n]);MessageBox(NULL,ErrorMessage,TEXT("error"),MB_ICONERROR);}
		}
	}
	else {
		MessageBox(NULL, TEXT("找不到list文件"), TEXT("error"), MB_ICONERROR);
		return -1;
	}

	if (songs[0].fileName[0] == NULL)
	{
		swprintf(ErrorMessage, TEXT("%s找不到文件"), songs[n].fileName);
		MessageBox(NULL, ErrorMessage, TEXT("error"), MB_ICONERROR);
		return -1;
	}

	//将列表文件名转换为完整地址
	for (n = 0; n < MAXNUM; n++)
	{
		if (songs[n].fileName[0] != 0)
		{
			FileN++;
			int ScriptN = 0;

			//利用变量数组将列表文件名转换为完整地址
			swprintf(songs[n].filePath, TEXT("%s"), songs[n].fileName);
			
			//打开文件读取信息
			ScriptN = LoadScript(songs[n].filePath, Script, &ScriptX, SCRIPTMAX);
			ScriptX = 0;
			if (ScriptN == -1)
			{
				swprintf(ErrorMessage, TEXT("打开文件错误")); MessageBox(NULL, ErrorMessage, TEXT("error"), MB_ICONERROR); return -1;
			}
			ScriptX += 4;
			LoadMassage(Script, &ScriptX, songs[n].title, 100);
			ScriptX += 4;
			LoadMassage(Script, &ScriptX, songs[n].artist, 100);
			LoadTotalTime(Script, ScriptN, &(songs[n].totalTime), &songs[n].totalMin, &songs[n].totalSec, &songs[n].totalSec2);

			//读取最高成绩 
			{
#ifdef X64
				char *title = UnicodeToUtf8(song[n].title);
				song[n].highScore=getScore(title);
				free(title);
#endif
			}

			// 读取最后时间
			ScriptX = 0;
			while (ScriptX < ScriptN)
			{
				if (Script[ScriptX] == L'[')
				{
					ScriptX++;
					//如果是字母判断为命令
					if (Script[ScriptX] >= L'a'&&Script[ScriptX] <= L'z')
					{
						LoadMassage(Script, &ScriptX, command, 100);
					}
					//如果是数字判断为歌词时间
					else if (Script[ScriptX] >= L'0'&&Script[ScriptX] <= L'9')
					{
						int min = 0;
						int sec = 0;
						int sec2 = 0;
						min = ScriptCheck(Script, &ScriptX);
						sec = ScriptCheck(Script, &ScriptX);
						sec2 = ScriptCheck(Script, &ScriptX);
					}
				}
				else
				{	//读取歌詞
					LoadLyric(Script, &ScriptX, Lyric, LyricKana, MAXLENGTH);
					if (Script[ScriptX] != L'[') {
						LoadMassage(Script, &ScriptX, LyricKana, MAXLENGTH);
					}
					int a = KanaToChar(LyricKana, LyricChar, MAXLENGTH);
					ClassScore[n] += 100 * a;
				}
			}
		}
	}
	ScriptX = 0;
	return FileN;
}
//成功按键处理
void seikou()
{	Combo++;
	corrected++;
	Score+=100+Combo;
	if(Combo>MaxCombo)MaxCombo=Combo;
	//if(Score>HighScore[FileNum])HighScore[FileNum]=Score;
}
//失败按键处理
void shippai()
{
	Combo=0;
	Missed++;
	Score-=50;
}
//拗音按键判断
void YouonKey(wchar_t kana[],int *KanaX,int *CharX,int *kana2)
{
	switch(kana[*KanaX+1])
	{
		case L'ゃ':
		case L'ャ':if(*kana2==1){if(key(Y)||key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2==2){if(key(A)){(*CharX)++;*KanaX+=2;*kana2=0;seikou();}else shippai();break;}
			break;
		case L'ゅ':
		case L'ュ':if(*kana2==1){if(key(Y)||key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2==2){if(key(U)){(*CharX)++;*KanaX+=2;*kana2=0;seikou();}else shippai();break;}
			break;
		case L'ょ':
		case L'ョ':if(*kana2==1){if(key(Y)||key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2==2){if(key(O)){(*CharX)++;*KanaX+=2;*kana2=0;seikou();}else shippai();break;}
			break;
		default:if(key(I)){(*CharX)++;(*KanaX)++;*kana2=0;seikou();}else shippai();
			break;
	}
}
//按键判断
void KeyCheck(wchar_t kana[],int *KanaX,int *CharX,int *kana2)
{	int n=0;
	keyhantei=0;
	if(StartTime!=0)for(int i=0;i<255;i++){keyhantei|=Key[i][1];}
	if(keyhantei==1)
	{
	switch(kana[*KanaX]){
		case L'あ':case L'ア':if(key(A)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'い':case L'イ':if(key(I)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'う':case L'ウ':if(key(U)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'え':case L'エ':if(key(E)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'お':case L'オ':if(key(O)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'か':case L'カ': {if(*kana2==0){if(key(K)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
					if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'き':case L'キ':if(*kana2==0){if(key(K)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'く':case L'ク': {if(*kana2==0){if(key(K)||key(Q)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'け':case L'ケ': {if(*kana2==0){if(key(K)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'こ':case L'コ': {if(*kana2==0){if(key(K)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'が':case L'ガ': {if(*kana2==0){if(key(G)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぎ':case L'ギ':if(*kana2==0){if(key(G)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ぐ':case L'グ': {if(*kana2==0){if(key(G)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'げ':case L'ゲ': {if(*kana2==0){if(key(G)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ご':case L'ゴ': {if(*kana2==0){if(key(G)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
				break;
		case L'さ':case L'サ': {if(*kana2==0){if(key(S)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'し':case L'シ': {
			if(*kana2==0){
				if(key(S)||(key(C)&&kana[*KanaX+1]!=L'ゃ'&&kana[*KanaX+1]!=L'ゅ'&&kana[*KanaX+1]!=L'ょ'
					&&kana[*KanaX + 1] != L'ャ'&&kana[*KanaX + 1] != L'ュ'&&kana[*KanaX + 1] != L'ョ')){
					(*CharX)++;if(key(S))(*kana2)++;else *kana2=2;seikou();}else shippai();break;
			}
			if(*kana2==1&&kana[*KanaX+1]!=L'ゃ'&&kana[*KanaX+1]!=L'ゅ'&&kana[*KanaX+1]!=L'ょ'
				&&kana[*KanaX + 1] != L'ャ'&&kana[*KanaX + 1] != L'ュ'&&kana[*KanaX + 1] != L'ョ'){
				if(key(H)||key(I)){
					if(key(H))(*kana2)++;
					else {*kana2=0;(*KanaX)++;(*CharX)++;}seikou();}
				else shippai();break;}
			if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}}
			break;
		case L'す':case L'ス': {if(*kana2==0){if(key(S)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'せ':case L'セ': {if(*kana2==0){if(key(S)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'そ':case L'ソ': {if(*kana2==0){if(key(S)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ざ':case L'ザ': {if(*kana2==0){if(key(Z)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'じ':case L'ジ':if(*kana2==0){if(key(J)||(key(Z)&&kana[*KanaX+1]!=L'ゃ'&&kana[*KanaX+1]!=L'ゅ'&&kana[*KanaX+1]!=L'ょ')){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ず':case L'ズ': {if(*kana2==0){if(key(Z)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぜ':case L'ゼ': {if(*kana2==0){if(key(Z)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぞ':case L'ゾ': {if(*kana2==0){if(key(Z)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'た':case L'タ': {if(*kana2==0){if(key(T)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ち':case L'チ': {
			if(*kana2==0){
				if(key(C) || (key(T)&&kana[*KanaX+1]!=L'ゃ'&&kana[*KanaX+1]!=L'ゅ'&&kana[*KanaX+1]!=L'ょ'
					&& kana[*KanaX + 1] != L'ャ'&&kana[*KanaX + 1] != L'ュ'&&kana[*KanaX + 1] != L'ョ')){
					if(key(T)){*CharX+=2;(*kana2)=2;}
					else{(*CharX)++;*kana2=1;}seikou();}
				else shippai();break;}
					if(*kana2==1){
						if(key(H)||(key(Y)&&(kana[*KanaX+1]==L'ゃ'||kana[*KanaX+1]==L'ゅ'||kana[*KanaX+1]==L'ょ'||
							kana[*KanaX + 1] == L'ャ' || kana[*KanaX + 1] == L'ュ' || kana[*KanaX + 1] == L'ョ')))
						{ (*CharX)++;*kana2=2;seikou();}
						else shippai();break;}
					if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}}
			break;
		case L'つ':case L'ツ': {if(*kana2==0){if(key(T)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
					if(*kana2>=1){if(key(U)||(*kana2==1&&key(S))){(*kana2)++;if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;}seikou();}else shippai();break;}
					}
			break;
		case L'て':case L'テ': {if(*kana2==0){if(key(T)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
					if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'と':case L'ト': {if(*kana2==0){if(key(T)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'だ':case L'ダ': {if(*kana2==0){if(key(D)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぢ':case L'ヂ': {if(*kana2==0){if(key(D)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(I)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'づ':case L'ヅ': {if(*kana2==0){if(key(D)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){if(key(U)||(*kana2==1&&key(S))){(*kana2)++;if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;}seikou();}else shippai();break;}}
			break;
		case L'で':case L'デ': {if(*kana2==0){if(key(D)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ど':case L'ド': {if(*kana2==0){if(key(D)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'な':case L'ナ': {if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'に':case L'ニ':if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ぬ':case L'ヌ': {if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ね':case L'ネ': {if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'の':case L'ノ': {if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ん':case L'ン': {if(*kana2==0){if(key(N)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(N)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'は':case L'ハ': {if(*kana2==0){if(key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ひ':case L'ヒ':if(*kana2==0){if(key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ふ':case L'フ': {if(*kana2==0){if(key(H)||key(F)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'へ':case L'ヘ': {{if(*kana2==0){if(key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ほ':case L'ホ': {if(*kana2==0){if(key(H)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ば':case L'バ': {if(*kana2==0){if(key(B)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'び':case L'ビ':if(*kana2==0){if(key(B)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ぶ':case L'ブ': {if(*kana2==0){if(key(B)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'べ':case L'ベ': {if(*kana2==0){if(key(B)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぼ':case L'ボ': {if(*kana2==0){if(key(B)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぱ':case L'パ': {if(*kana2==0){if(key(P)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぴ':case L'ピ':if(*kana2==0){if(key(P)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'ぷ':case L'プ': {if(*kana2==0){if(key(P)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぺ':case L'ペ': {if(*kana2==0){if(key(P)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ぽ':case L'ポ': {if(*kana2==0){if(key(P)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ま':case L'マ': {if(*kana2==0){if(key(M)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'み':case L'ミ':if(*kana2==0){if(key(M)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'む':case L'ム': {if(*kana2==0){if(key(M)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'め':case L'メ': {if(*kana2==0){if(key(M)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'も':case L'モ': {if(*kana2==0){if(key(M)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'や':case L'ヤ': {if(*kana2==0){if(key(Y)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ゆ':case L'ユ': {if(*kana2==0){if(key(Y)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'よ':case L'ヨ': {if(*kana2==0){if(key(Y)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ら':case L'ラ': {if(*kana2==0){if(key(R)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'り':case L'リ':if(*kana2==0){if(key(R)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				  if(*kana2>=1){YouonKey(kana,KanaX,CharX,kana2);}
			break;
		case L'る':case L'ル': {if(*kana2==0){if(key(R)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(U)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'れ':case L'レ': {if(*kana2==0){if(key(R)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(E)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'ろ':case L'ロ': {if(*kana2==0){if(key(R)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'わ':case L'ワ': {if(*kana2==0){if(key(W)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(A)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'を':case L'ヲ': {if(*kana2==0){if(key(W)){(*CharX)++;(*kana2)++;seikou();}else shippai();break;}
				 if(*kana2==1){if(key(O)){(*KanaX)++;(*CharX)++;*kana2=0;seikou();}else shippai();break;}}
			break;
		case L'っ':case L'ッ':
		{
			switch(kana[*KanaX+1])
			{
				case L'か':
				case L'き':
				case L'く':
				case L'け':
				case L'こ':
				case L'カ':
				case L'キ':
				case L'ク':
				case L'ケ':
				case L'コ':if(key(K)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'が':
				case L'ぎ':
				case L'ぐ':
				case L'げ':
				case L'ご':if(key(G)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'さ':
				case L'し':
				case L'す':
				case L'せ':
				case L'そ':
				case L'サ':
				case L'シ':
				case L'ス':
				case L'セ':
				case L'ソ':if(key(S)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ざ':
				case L'ず':
				case L'ぜ':
				case L'ぞ':if(key(Z)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'じ':if(key(J)||key(Z)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'た':
				case L'つ':
				case L'て':
				case L'と':
				case L'タ':
				case L'ツ':
				case L'テ':
				case L'ト':if(key(T)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ち':
				case L'チ':if(key(C)||key(T)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'だ':
				case L'ぢ':
				case L'づ':
				case L'で':
				case L'ど':if(key(D)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ら':
				case L'り':
				case L'る':
				case L'れ':
				case L'ろ':if(key(R)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'は':
				case L'ひ':
				case L'へ':
				case L'ほ':if(key(H)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ふ':if(key(F)||key(H)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ば':
				case L'び':
				case L'ぶ':
				case L'べ':
				case L'ぼ':if(key(B)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ぱ':
				case L'ぴ':
				case L'ぷ':
				case L'ぺ':
				case L'ぽ':
				case L'パ':
				case L'ピ':
				case L'プ':
				case L'ペ':
				case L'ポ':
					if(key(P)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'ま':
				case L'み':
				case L'む':
				case L'め':
				case L'も':if(key(M)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'や':
				case L'ゆ':
				case L'よ':if(key(Y)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				case L'わ':
				case L'を':if(key(W)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
					break;
				default:
					break;
				}
			}
		break;
		case L'ー':if(key(MINUS)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'　':
		case L' ':if(key(SPACE)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'A':
		case L'a':
		case L'Ａ':
		case L'ａ':if(key(A)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'B':
		case L'b':
		case L'Ｂ':
		case L'ｂ':if(key(B)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'C':
		case L'c':
		case L'Ｃ':
		case L'ｃ':if(key(C)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'D':
		case L'd':
		case L'Ｄ':
		case L'ｄ':if(key(D)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'E':
		case L'e':
		case L'Ｅ':
		case L'ｅ':if(key(E)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'f':
		case L'F':
		case L'ｆ':
		case L'Ｆ':if(key(F)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'G':
		case L'g':
		case L'Ｇ':
		case L'ｇ':if(key(G)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'H':
		case L'h':
		case L'Ｈ':
		case L'ｈ':if(key(H)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'I':
		case L'i':
		case L'Ｉ':
		case L'ｉ':if(key(I)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'j':
		case L'J':
		case L'ｊ':
		case L'Ｊ':if(key(J)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'K':
		case L'k':
		case L'Ｋ':
		case L'ｋ':if(key(K)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'L':
		case L'l':
		case L'Ｌ':
		case L'ｌ':if(key(L)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'Ｎ':
		case L'ｎ':
		case L'N':
		case L'n':if(key(N)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'M':
		case L'm':
		case L'Ｍ':
		case L'ｍ':if(key(M)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'O':
		case L'o':
		case L'ｏ':
		case L'Ｏ':if(key(O)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
		break;
		case L'P':
		case L'p':
		case L'ｐ':
		case L'Ｐ':if(key(P)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'q':
		case L'Q':
		case L'Ｑ':
		case L'ｑ':if(key(Q)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'R':
		case L'r':
		case L'ｒ':
		case L'Ｒ':if(key(R)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'S':
		case L's':
		case L'Ｓ':
		case L'ｓ':if(key(S)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'T':
		case L't':
		case L'Ｔ':
		case L'ｔ':if(key(T)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'U':
		case L'u':
		case L'ｕ':
		case L'Ｕ':if(key(U)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'V':
		case L'v':
		case L'ｖ':
		case L'Ｖ':if(key(V)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'W':
		case L'w':
		case L'ｗ':
		case L'Ｗ':if(key(W)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'X':
		case L'x':
		case L'Ｘ':
		case L'ｘ':if(key(X)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'Y':
		case L'y':
		case L'ｙ':
		case L'Ｙ':if(key(Y)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'Z':
		case L'z':
		case L'ｚ':
		case L'Ｚ':if(key(Z)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'\'':if(Key[144][1]){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'1':
		case L'１':if(key(1)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'2':
		case L'２':if(key(2)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'3':
		case L'３':if(key(3)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'4':
		case L'４':if(key(4)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'5':
		case L'５':if(key(5)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'6':
		case L'６':if(key(6)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'7':
		case L'７':if(key(7)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'8':
		case L'８':if(key(8)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'9':
		case L'９':if(key(9)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'0':
		case L'０':if(key(0)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'↑':if(key(UP)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'↓':if(key(DOWN)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'←':if(key(LEFT)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		case L'→':if(key(RIGHT)){(*KanaX)++;(*CharX)++;seikou();}else shippai();
			break;
		default:
			break;
			}
		}
	}
}
//读取音乐
int LoadMusic(wchar_t *ScriptBuf,int *ScriptBufX,wchar_t *LoadBuf,int MaxNum)
{
	LoadMassage( ScriptBuf, ScriptBufX,LoadBuf,MaxNum);

	MuiscHandle=LoadSoundMem(LoadBuf);
	return 0;
}
//读取背景图片
int LoadBG(wchar_t *ScriptBuf,int *ScriptBufX,wchar_t *LoadBuf,int MaxNum,int *handle)
{	
	LoadMassage( ScriptBuf, ScriptBufX,LoadBuf,MaxNum);

	*handle=LoadGraph(LoadBuf);
	return 0;
}
//显示
int display()
{	
	//画总时间线
	TotalLineX=Width/6;
	TotalLineY=Height/5*3+mojiTime.size;
	IntervalX=TotalLineX;
	IntervalY=TotalLineY+mojiTime.size;
	TimeLine=Width/5*4;//时间线长度

	DrawStringToHandle(TotalLineX-mojiTime.size*4,TotalLineY-mojiTime.size/2,_T("Total"), white, mojiTime.font,0x7777ff);
	
	DrawLine(TotalLineX,TotalLineY,TotalLineX+TimeLine,TotalLineY,0x888888,5);
	Total=PassTime*TimeLine/song[FileNum].totalTime;
	if(Total>0)DrawLine(TotalLineX,TotalLineY,TotalLineX+Total,TotalLineY, white,5);
	
	//间隔时间线
	DrawStringToHandle(IntervalX-mojiTime.size*5,IntervalY-mojiTime.size/2,_T("Interval"), white, mojiTime.font,0x7777ff);
	DrawLine(IntervalX,IntervalY,TotalLineX+TimeLine,IntervalY,0x888888,5);
	if(hanteiTime-hanteiTimeOld!=0)Interval=(PassTime-hanteiTimeOld)*TimeLine/(hanteiTime-hanteiTimeOld);
	if(Interval>0)DrawLine(IntervalX,IntervalY,IntervalX+Interval,IntervalY, white,5);
	
	//显示时间
	TimeX=TotalLineX+TimeLine-mojiTime.size*10;
	TimeY=TotalLineY-mojiTime.size-5;
	wsprintf(Time,_T("%02d:%02d.%02d/%02d:%02d.%02d"),PassTime/60000,PassTime%60000/1000,PassTime%1000/10,song[FileNum].totalMin, song[FileNum].totalSec, song[FileNum].totalSec2);
	DrawStringToHandle(TimeX,TimeY,Time, white, mojiTime.font,0xff0000);
	
	//画歌词
	if(LyricKana[LyricKanaX]!=L'\0')
	{
		int lrcX = 0;
		int kanaX = 0;
		int KanaColor = 0;
		int *x = NULL;
		int y = 0;
		int x1 = 0;
		int x2 = 0;
		wchar_t temp[2] = { 0 };
		/*
		//完整歌词
		DrawStringToHandle(lyric.x, lyric.y, Lyric, 0xeeeeff, lyric.font, 0xaaaaff);
		//GetDrawStringSizeToHandle()
		//假名
		for (int i = 0; i<100; i++)
		{
			if (i < LyricKanaX) {
				KanaColor = 0xff55ff;
			}
			else {
				KanaColor = white;
			}

			temp[0] = LyricKana[i];
			DrawStringToHandle(lyricKana.x + i * lyricKana.size, lyricKana.y, temp, KanaColor, lyricKana.font, 0xaaaaff);

		}*/

		
		/*
		x = lyric.x;
		while (lrcX < 100 && kanaX < 100) {
			if (kanaX < LyricKanaX) {
				KanaColor = 0xff55ff;
			}
			else {
				KanaColor = white;
			}

			if (Lyric[lrcX] != LyricKana[kanaX]) {
				
				if((Lyric[lrcX] >= L'ぁ'&&Lyric[lrcX] <= L'ヺ')
					|| (Lyric[lrcX] >= L'A'&&Lyric[lrcX] <= L'Z')
					|| (Lyric[lrcX] >= L'a'&&Lyric[lrcX] <= L'z')
					|| (Lyric[lrcX] >= L'Ａ'&&Lyric[lrcX] <= L'Ｚ')
					|| (Lyric[lrcX] >= L'ａ'&&Lyric[lrcX] <= L'ｚ')
					|| (Lyric[lrcX] >= L'0'&&Lyric[lrcX] <= L'9')
					|| (Lyric[lrcX] >= L'０'&&Lyric[lrcX] <= L'９')) 
				{
					temp[0] = LyricKana[kanaX++];
					DrawStringToHandle(x1+x2, lyricKana.y, temp, KanaColor, lyricKana.font, 0xaaaaff);
					x2+=GetDrawStringWidthToHandle(temp,2, lyricKana.font);
						
				}
				else
				{
					//如果是汉字把定位到第一个汉字上面
					if (Lyric[lrcX]>=0x4E00 ) {
						if(x2 != -5)
						{
							x1 = x;
							x2 = -5;
						}
					}
					temp[0] = Lyric[lrcX++];
					DrawStringToHandle(x, lyric.y, temp, KanaColor, lyric.font, 0xaaaaff);
					x+=GetDrawStringWidthToHandle(temp, 2, lyric.font);
					

				}
					
			}else{
				temp[0] = Lyric[lrcX++];
				DrawStringToHandle(x, lyric.y, temp, KanaColor, lyric.font, 0xaaaaff);
				x += GetDrawStringWidthToHandle(temp, 2, lyric.font);
				kanaX++;
			}
			
		}
		//*/

		///*
		int isFirst = true;
		int font = lyric.font;
		x1 = lyric.x;
		x2 = lyricKana.x;
		x = &x1;
		y = lyric.y;
		while (lrcX < 100 && kanaX < 100) {
			if (kanaX < LyricKanaX) {
				KanaColor = 0xaaaaff;
			}
			else {
				KanaColor = white;
			}

			if (Lyric[lrcX] == L'(') {
				y = lyricKana.y;
				font = lyricKana.font;
				lrcX++;
				x = &x2;
				//continue;
			}

			if (Lyric[lrcX] == L')') {

				y = lyric.y;
				font = lyric.font;
				lrcX++;
				x = &x1;
				//continue;
			}

			//如果是汉字把定位到第一个汉字上面
			if (Lyric[lrcX]>= 0x4E00)
			{
				if (isFirst) {
					x2 = x1;
					isFirst = false;
				}
				
			}
			else
			{
				isFirst = true;
			}

			if (Lyric[lrcX] == LyricKana[kanaX]) {
				kanaX++;
			}

			temp[0] = Lyric[lrcX++];
			
			DrawStringToHandle(*x, y, temp, KanaColor, font, 0xaaaaff);
			*x += GetDrawStringWidthToHandle(temp, 2, font);
		}
		//*/
	

		temp[0]=LyricChar[LyricCharX];
		DrawStringToHandle(lyricChar1.x-(lyricChar1.size-lyricChar2.size)/4, lyricChar1.y-(lyricChar1.size-lyricChar2.size)+5,temp,white,lyricChar1.font,0x5555ff);

		DrawStringToHandle(lyricChar1.x+lyricChar1.size/2,lyricChar1.y,&LyricChar[LyricCharX+1],white,lyricChar2.font,0x5555ff);
	}else{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawStringToHandle(lyric.x,lyric.y,Lyric2,white,lyric.font,0x555555);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL,255);
	}

	//得分
	TCHAR Temp[50]={0};
	tsprintf(Temp,_T("%d COMBO"),Combo);
	DrawStringToHandle(Width/25,Height*7/10,Temp,white,lyric.font,0x9999bb);

	tsprintf(Temp,_T("Score:%d"),Score);
	DrawStringToHandle(IntervalX,IntervalY+lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	tsprintf(Temp,_T("MaxCombo:%03d"),MaxCombo);
	DrawStringToHandle(IntervalX+TimeLine/4,IntervalY+lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	tsprintf(Temp,_T("Complete:%03d"),Complete);
	DrawStringToHandle(IntervalX+TimeLine/4*2,IntervalY+lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	tsprintf(Temp,_T("Solve:%03d"),Solve);
	DrawStringToHandle(IntervalX+TimeLine/4*3,IntervalY+lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);
	if(InputTime!=0)
	TypeSpeed=corrected*60000/InputTime;

	tsprintf(Temp,_T("TypeSpeed:%03d/min"),TypeSpeed);
	DrawStringToHandle(IntervalX-lyricChar2.size,Height-lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	tsprintf(Temp,_T("Corrected:%03d"),corrected);
	DrawStringToHandle(IntervalX+TimeLine/4,Height-lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	tsprintf(Temp,_T("Missed:%03d"),Missed);
	DrawStringToHandle(IntervalX+TimeLine/4*2,Height-lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	static int ClassHantei;
	if(mode==0)ClassHantei=ClassScore[FileNum];else ClassHantei=ClassScore[FileNum]/2;

	if(Score>ClassHantei){
		Class[0]=L'Ｓ';Class[1]=L'Ｓ'; Class[2]=L'Ｓ';
	}
	else if(Score>ClassHantei*95/100){Class[0]=L'　';Class[1]=L'Ｓ'; Class[2]=L'Ｓ';}
	else if(Score>ClassHantei*90/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｓ';}
	else if(Score>ClassHantei*80/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ａ';}
	else if(Score>ClassHantei*70/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｂ';}
	else if(Score>ClassHantei*60/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｃ';}
	else if(Score>ClassHantei*50/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｄ';}
	else if(Score>ClassHantei*40/100){Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｅ';}
	else{Class[0]=L'　';Class[1]=L'　'; Class[2]=L'Ｆ';}

	tsprintf(Temp,_T("Class: %s"),Class);
	DrawStringToHandle(IntervalX+TimeLine/4*3,Height-lyricChar2.size,Temp,white,lyricChar2.font,0x9999bb);

	return 0;
}

//判定
int hantei()
{	
	int offset = 0;
	KeyCheck(LyricKana,&LyricKanaX,&LyricCharX,&Kana2);
	//开始
	if(Ready==1)
	{	
		if(CheckHandleASyncLoad(MuiscHandle)==0||VideoFlag==1)
		{
			if(Alpha==0) Alpha=1;
			Alpha+=30;
			if(Alpha>255)Alpha=255;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,Alpha);
			DrawStringToHandle(Width/2-3*title1.size,Height/2-2*title1.size,_T("ＲＥＡＤＹ"),white,title1.font,0x56faff);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		}
		if(key(SPACE)||key(RETURN)){
			PlaySoundMem(BurstLinkSE,DX_PLAYTYPE_BACK);
			if(CheckHandleASyncLoad(MuiscHandle)==0){
				Ready=0;StartTime=GetNowCount();
				ChangeVolumeSoundMem((int)Volume / 100.0 * 255,MuiscHandle);
				PlaySoundMem(MuiscHandle,DX_PLAYTYPE_BACK);
			}
			if( VideoFlag==1){
				Ready=0;StartTime=GetNowCount();
				ChangeMovieVolumeToGraph((int)Volume / 100.0 * 255,BGHandle[BGNum]);
				PlayMovieToGraph( BGHandle[BGNum]);
			}
		}
	}
	if(StartTime!=0)
		PassTime=GetNowCount()-StartTime;
	
	while(PassTime>=hanteiTime&&ScriptX<=ScriptNum)
	{	
		if(LyricClear==1){
			LyricClear=0;
			memset(Lyric,0,sizeof(wchar_t)*MAXNUM);
			memset(LyricKana,0,sizeof(wchar_t)*MAXNUM);
			memset(LyricChar,0,sizeof(wchar_t)*MAXNUM);
		}

		if( Script[ScriptX]==L'['){	
			ScriptX++;
			//命令
			if(Script[ScriptX]>=L'a'&&Script[ScriptX]<=L'z')
			{	
				//读取歌名
				if(offset=tCharCmp(Script,_T("ti:"),ScriptX)){
					ScriptX+= offset;
					LoadMassage(Script, &ScriptX, song[FileNum].title, 100);

				}//读取歌手
				else if (offset = tCharCmp(Script, _T("ar:"), ScriptX)) {
					
					ScriptX+= offset;
					LoadMassage(Script,&ScriptX, song[FileNum].artist,100);
				}//开始计时
				else if (offset = tCharCmp(Script, _T("start"), ScriptX)) {
					LoadMassage(Script,&ScriptX,command,100);
					Ready=1;
					BGNum=0;
				}
				//读取音乐
				else if (offset = tCharCmp(Script, _T("lm:"), ScriptX)) {
					ScriptX+= offset;
					SetUseASyncLoadFlag(TRUE);
					LoadMusic(Script,&ScriptX,command,100);
					SetUseASyncLoadFlag(FALSE);
				}
				//读取背景
				else if (offset = tCharCmp(Script, _T("bg:"), ScriptX)) {
					ScriptX += offset;
					LoadBG(Script,&ScriptX,command,100,&BGHandle[BGNum]);
				}
				//读取背景方式2，读取多个背景时循环播放图片
				else if (offset = tCharCmp(Script, _T("bg2:"), ScriptX)) {
					ScriptX += offset;
					LoadBG(Script,&ScriptX,command,100,&BGHandle[BGNum]);
					BGNum++;
					BGloop++;
				}//设置循环时间
				else if(offset = tCharCmp(Script, _T("slt:"), ScriptX)){
					ScriptX += offset;
					BGloopTime=ScriptCheck(Script,&ScriptX);
				}//设置图片编号
				//else if(Script[ScriptX]==L's'&&Script[ScriptX+1]==L'g'&&Script[ScriptX+2]==L'n'&Script[ScriptX+3]==L':')
				else if (offset = tCharCmp(Script, _T("sgn:"), ScriptX)) {
					ScriptX += offset;
					BGNum = ScriptCheck(Script,&ScriptX);
				}//读取视频
				else if (offset = tCharCmp(Script, _T("lv:"), ScriptX)) {	
					ScriptX+=offset;
					LoadBG(Script,&ScriptX,command,100,&BGHandle[BGNum]);
					Ready=1;
					VideoFlag=1;
				}
				else{
					LoadMassage(Script,&ScriptX,command,100);
				}
			}
			//时间轴
			else if(Script[ScriptX]>=L'0'&&Script[ScriptX]<=L'9')
			{	
				int min=0;
				int sec=0;
				int sec2=0;
				hanteiTimeOld=hanteiTime;
				min=ScriptCheck(Script,&ScriptX);
				sec=ScriptCheck(Script,&ScriptX);
				sec2=ScriptCheck(Script,&ScriptX);
				hanteiTime=min*60000+sec*1000+sec2*10;
				if(Script[ScriptX+2]>=L'0'&&Script[ScriptX+2]<=L'9')
				{
					LyricClear=1;
				}
			}
		}
		else {
			//计算完成和漏过数
			if (LyricKana[0] != L'\0'&&LyricKana[LyricKanaX] != L'\0')
			{
				Combo = 0;
				for (int i = 0; LyricKana[LyricKanaX + i] != L'\0'; i++)Solve++;
			}
			else
			{
				if (LyricKana[0] != L'\0')Complete++;
			}


			//读取歌詞
			LyricKanaX = 0;
			LyricCharX = 0;
			Kana2 = 0;
			LoadLyric(Script, &ScriptX, Lyric, LyricKana, 100);
			if (Script[ScriptX] != L'[') {
				LoadMassage(Script, &ScriptX, LyricKana, MAXLENGTH);
			}
			

			if (mode == 1)
			{
				for (int i = 0; i<100 && LyricKana[i] != L'\0'; i++)
				{
					switch (LyricKana[i] % 4)
					{
					case 0:LyricKana[i] = L'↑';
						break;
					case 1:LyricKana[i] = L'↓';
						break;
					case 2:LyricKana[i] = L'←';
						break;
					case 3:LyricKana[i] = L'→';
						break;
					default:
						break;
					}
				}
			}
			KanaToChar(LyricKana, LyricChar, MAXLENGTH);
		}
	}
	while(hanteiTime>=hanteiTime2&&ScriptX2<=ScriptNum)
	{	//if(LyricClear2==1){LyricClear=0;memset(Lyric2,0,sizeof(wchar_t)*MAXNUM);memset(LyricKana2,0,sizeof(wchar_t)*MAXNUM);}
		if( Script[ScriptX2]==L'[')
		{	
			ScriptX2++;
			//字母开头为命令
			if(Script[ScriptX2]>=L'a'&&Script[ScriptX2]<=L'z')
			{	
				LoadMassage(Script,&ScriptX2,command,100);
			}
			//数字开头为时间
			else if(Script[ScriptX2]>=L'0'&&Script[ScriptX2]<=L'9')
			{	int min=0;
				int sec=0;
				int sec2=0;
				min=ScriptCheck(Script,&ScriptX2);
				sec=ScriptCheck(Script,&ScriptX2);
				sec2=ScriptCheck(Script,&ScriptX2);
				hanteiTime2=min*60000+sec*1000+sec2*10;
				if(Script[ScriptX2+2]>=L'0'&&Script[ScriptX2+2]<=L'9'){LyricClear2=1;}
				
			}
		}
		else 
		{	//读取歌詞
			LoadMassage(Script,&ScriptX2,Lyric2,MAXLENGTH);
			
			if(Script[ScriptX2]!=L'[')
			{
				LoadMassage(Script,&ScriptX2,LyricKana2,MAXLENGTH);
			}
		}
	}
	
	return 0;
}


