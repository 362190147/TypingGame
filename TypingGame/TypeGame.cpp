#include"DxLib.h"
#include"resource.h"
#include "GameState.h"
#include"hantei.h"
#ifdef WIN32
#define main() WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif //
#ifdef __ANDROID__
#define main() android_main(void)
#endif //

int Time;
int FPS;
int Count;
int FPSFlag;

//画选中歌曲的线
void selectLine(int   x1, int   y1, int   x2, int   y2){
		DrawLine(x1, y1, x2, y2, 0x5555ff, 5);
}

//画选中的线
void drawSelectLine() {
	int x = 0;
	int y = 0;
	int y1 = title1.y0 + title1.size + 5;
	int y2 = list.y0 + title2.size + 5;
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 128);
	if (LineStartX<LineAX)
	{
		selectLine(LineStartX, y1, LineAX, y1);
	}
	if (LineStartX<LineBX)
	{
		if (LineStartX < LineAX) {
			x = LineAX;
			y = y1;
		}
		else {
			x = LineStartX;
			y = (LineStartX - LineAX)*(list.y0 + title2.size - title1.y0 - title1.size) / (LineBX - LineAX) + y1;
		}
		selectLine(x, y, LineBX, y2);
	}

	if (LineStartX<Width)
	{
		if (LineStartX < LineBX)
			x = LineBX;
		else
			x = LineStartX;
		selectLine(x, y2, Width, y2);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

//画背景
void drawbg() {
	static int alpha = 255, b = 0;
	if (BGloop>0) {
		BGloopTimeNow += 17;
		if (BGloopTimeNow >= BGloopTime) {
			if (b == 0) {
				alpha -= 5;
				if (alpha<20)b = 1;
			}
			else {
				alpha += 5;
				if (alpha <= 20) BGNum++;
				if (BGNum >= BGloop)BGNum = 0;
				if (alpha >= 255) { alpha = 255; b = 0; BGloopTimeNow = 0; }
			}
		}
	}
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_WIPE, alpha);
	if (BGHandle[BGNum]>0) {
		DrawExtendGraph(0, 0, Width, Height, BGHandle[BGNum], false);
	}
}

int main()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(Width, Height, 32);
#ifdef WIN32
	ChangeWindowMode( TRUE );
	SetWindowIconID(IDI_ICON1);//ウインドウのアイコンを変更する
	SetWindowText(_T("日本語タイピングゲーム Ver1.0 BY362190147"));
#endif

	if ( DxLib_Init()==-1  ) return -1;
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	while(!ProcessMessage()&&GameState){	
		if( GetNowCount() - Time >= 1000 ){FPS=Count;Count=0;Time=GetNowCount();}//FPS表示
		else{Count++;}
		ClearDrawScreen();
		//检测所有按键
		MouseLoop();
		KeyBoard(KeyBuf,Key);

		if(key(F2)){mode =! mode;}
		
		drawbg();
		
		//控制和显示声量
		if(key(ADD)){
			Volume+=5;
			if(Volume>=100)
				Volume=100;
			ChangeVolumeSoundMem((int)Volume/100.0*255,MuiscHandle);
			ChangeMovieVolumeToGraph((int)Volume/100.0*255,BGHandle[BGNum]);
		}

		if(key(MINUS)||key(SUBTRACT)){
			Volume-=5;
			if(Volume<0)
				Volume=0;
			ChangeVolumeSoundMem((int)Volume / 100.0 * 255,MuiscHandle);
			ChangeMovieVolumeToGraph((int)Volume / 100.0 * 255,BGHandle[BGNum]);
		}
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_WIPE,150);
		DrawLine(15,15,600+15,15,0xaaaaaa,10);
		DrawLine(15,15,Volume*6+15,15,0x5555ff,10);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL,10);

		//画选中的标题和歌手		
		DrawStringToHandle(title1.x, title1.y, song[FileNum].title, 0xffffff, title1.font, 0xff9999ff);
		DrawStringToHandle(title1.x, title1.y - artist1.size, song[FileNum].artist, 0xffffff, artist1.font, 0xaaaaff);

		//画最高分
		{
			TCHAR temp[100]={0};
			tsprintf(temp,_T("HIGH SCORE:%d"), song[FileNum].highScore);
			DrawStringToHandle(title1.x,title1.y+title1.size+5,temp,0xffffff, title2.font,0xff9999ff);
			tsprintf(temp, _T("HIGH CLASS:%s"),HighClass[FileNum]);
			DrawStringToHandle(title1.x,title1.y+title1.size+title2.size+5,temp,0xffffff, title2.font,0xff9999ff);
			if(mode == 0)
				tsprintf(temp, _T("MODE:仮名")); 
			else 
				tsprintf(temp,_T("MODE:↑↓←→")); 
			DrawStringToHandle(title1.x,title1.y+title1.size+title2.size*2+5,temp,0xffffff, title2.font,0xff9999ff);
		}
		
		//画列表歌手
		if(list.x<Width)
			for(int i=0;i<LoadedFileN;i++){
				if(i==FileNum)
					SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL,255);
				else if (MouseX>list.x 
				&&MouseX<Width 
				&& MouseY>list.y + i * (title2.size + artist2.size + 10) - artist2.size 
				&& MouseY<list.y + i * (title2.size + artist2.size + 10) + title2.size)
				{
					SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL,255);
					if(GameState==GAMEMENU&&MouseCL!=0)
					{
						FileNum=i;
						ScriptNum=LoadScript(song[FileNum].filePath,Script,&ScriptX,SCRIPTMAX);
						GameState=GAMEPLAY;
						PlaySoundMem(SE[2],DX_PLAYTYPE_BACK);
					}
				}
				else SetDrawBlendMode(DX_BLENDGRAPHTYPE_WIPE,150);
				DrawStringToHandle(list.x,list.y+i*(title2.size+artist2.size+10),song[i].title,0xffffff, title2.font,0xff9999);
				DrawStringToHandle(list.x,list.y+i*(title2.size+artist2.size+10)-artist2.size, song[i].artist,0xffffff,artist2.font,0xff9999);
			}
		//鼠标先选择歌
		
		//画选中的线
		drawSelectLine();
		
		//运行状态
		switch(GameState)
		{
		case GAMEINIT:if(GameInit()==-1)return -1;
			break;
		case GAMELOGO:if(GameLogo()==-1)return -1;
			break;
		case GAMEMENU:if(GameMenu()==-1)return -1;
			break;
		case GAMEPLAY:if(GamePlay()==-1)return -1;
			break;
		case GAMESCORE:if(GameScore()==-1)return -1;
			break;

		default:
			break;
		}
		
		//FPS显示开关
		if(Key[KEY_INPUT_F1][1]!=0)
			if(FPSFlag==0)FPSFlag=1;
			else FPSFlag=0;
		if(FPSFlag==1)
		{
			DrawNumberPlusToI(580,0,_T("FPS"),FPS,10,GetColor(255,0,0));
	
		}
		ScreenFlip();

		if(LyricKana[LyricKanaX]!=0)InputTime+=17;
	}
	DxLib_End( ); 
	return 0;

}
