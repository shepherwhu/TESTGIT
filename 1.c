#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <ctype.h>
#include <bios.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <sys\timeb.h>
#include <malloc.h>
#include "mymenu.h"
#include "mytool.h"
#include "xzoran.h"
extern largedata rd56[5];
extern   VGASTRUC NowVGA;
extern   ZORANSTRUC ZoranStatus;
extern  int wtest[];

extern s_POSITION                  StartPos;
extern  s_POSITION                  WinPos;
extern int                         WIN_W,WIN_H;
extern int imagemess[6];
extern WORD   hxms;
extern WORD   xmsmostk;
extern int    xmsmostpage;
extern char PubStr[200];
extern int snd;
extern int esc;

mydebug()
{
	char filename[20],str[100];
	int ret;
	if(filedialog("*.rec", "打开批次纪录", filename, SEARCH_FILE) == SUCCESS)
		{
		if (JudgeFile(filename) == 1) ret = dialog(1,filename);
		else if (judgefile(filename) == 0) ret = dialog(2,filename);
			 else {keymessage("警告!!!","该文件不是记录文件");return 0;}
		//if (ret == 0) return 1;//selected cancel
		return 1;
		}
	else
		{
		messagebox("不能打开文件");
		return 0;
		}
}
int judgeFile(char filename[20])
{
	char fullname[100],head[6];
	FILE *fp;
	int data,i;
	sprintf(fullname, "data\\");
	strcat(fullname, filename);
	if((fp=fopen(fullname, "rb+"))==NULL)  return -1;
	fseek(fp,0,SEEK_SET);
	fread(head, 6, 1, fp);
	if (strcmp(head,"RECORD") != 1)//判断相不相等
	{
		return 2;
	}
	fseek(fp, 6, SEEK_SET);
	fread(&data, 2, 1, fp);
	fclose(fp);
	if (data == 10) return 1;
	else return 0;
}
int dialog(int flag,char filename[20])
{
	int i,select[11] = {1,0,0,0,0,0,0,0,0,0,0},all = 0;
	INFORMATION info;
	char fullname[40];
	const int LX = 200, LY = 140, WX = 600, WY = 380,CEN = (WX - LX)/2 + LX;
	int outflag = 0,sum;
	unsigned char key;

	SaveScreen();
	HideMenuCursor();
	_setcolor(192);
	_rectangle(3, LX, LY, WX, WY + 4);
	_setcolor(255);
	_xline(LX, LY, LX, WY + 4);//上
	_setcolor(240);
	_xline(LX, LY, WX, LY);//左
	_setcolor(0);
	_xline(WX, LY, WX, WY + 4);//下
	_setcolor(140);
	_xline(LX, WY + 4, WX, WY + 4);//右
	drawcontrol(LX + 4, LY + 30, WX - LX - 8, WY - LY - 32, 1);
	for(i = LY + 40; i < WY - 60; i += 30)
		{
		_setcolor(230);
		_rectangle(0, LX + 50, i, LX + 60, i + 10);
		_rectangle(0, CEN + 50, i, CEN + 60, i + 10);

		_setcolor(140);
		_xline(CEN + 50, i, CEN + 60, i);
		_xline(LX + 50, i, LX + 60, i);//上
		_setcolor(0);
		_xline(CEN + 50, i, CEN + 50, i + 10);
		_xline(LX + 50, i, LX + 50, i + 10);//左
		_setcolor(255);
		_xline(CEN + 60, i, CEN + 60,i + 10);
		_xline(LX + 60, i, LX + 60,i + 10);//右
		_setcolor(200);
		_xline(CEN + 50, i + 10, CEN + 60, i + 10);
		_xline(LX + 50, i + 10, LX + 60, i + 10);//下
		}
	_setcolor(0);
	_rectangle(0, LX + 50, LY + 40, LX + 60, LY + 50);
	
	//画对话框
	writeboxstring(LX + 130, LY + 5, 160, 20, "请选择要打印的项目", 1, 1);
	drawcontrol(LX + 40, WY - 30, 62, 25, 2);
	writeboxstring(LX + 45, WY - 28, 60, 20, "全选[a]", 1, 1);
	drawcontrol(WX - 160, WY - 30, 50, 25, 2);
	writeboxstring(WX - 150, WY - 28, 50, 20, "确定", 1, 1);
	drawcontrol(WX - 60, WY - 30, 50, 25, 2);
	writeboxstring(WX - 50, WY - 28, 50, 20, "放弃", 1, 1);
	if (flag == 1) //file flag = 10
		{
		_setcolor(230);
		_rectangle(0, LX + 50, WY - 50, LX + 60, WY - 40);
		_setcolor(140);
		_xline(LX + 50, WY - 50, LX + 60, WY - 50);//上
		_setcolor(0);
		_xline(LX + 50, WY - 50, LX + 50, WY - 40);//左
		_setcolor(255);
		_xline(LX + 60, WY - 50, LX + 60,WY - 40);//右
		_setcolor(200);
		_xline(LX + 50, WY - 40, LX + 60, WY - 40);//下

		writeboxstring(LX + 70, LY + 35, 100, 20, "编号", 1, 1);
		writeboxstring(LX + 270, LY + 35, 100, 20, "极距", 1, 1);
		writeboxstring(LX + 70, LY + 65, 100, 20, "焊点1宽", 1, 1);
		writeboxstring(LX + 270, LY + 65, 100, 20, "焊点1长", 1, 1);
		writeboxstring(LX + 70, LY + 95, 100, 20, "丝径1", 1, 1);
		writeboxstring(LX + 270, LY + 95, 100, 20, "焊点2宽", 1, 1);
		writeboxstring(LX + 70, LY + 125, 100, 20, "焊点2长", 1, 1);
		writeboxstring(LX + 270, LY + 125, 100, 20, "丝径2", 1, 1);
		writeboxstring(LX + 70, LY + 155, 100, 20, "弧高", 1, 1);
		writeboxstring(LX + 270, LY + 155, 100, 20, "电阻", 1, 1);
		writeboxstring(LX + 70, LY + 185, 100, 20, "合格否", 1, 1);
		}
	else
		{
		writeboxstring(LX + 70, LY + 35, 100, 20, "编号", 1, 1);
		writeboxstring(LX + 270, LY + 35, 100, 20, "极距", 1, 1);
		writeboxstring(LX + 70, LY + 65, 100, 20, "丝径", 1, 1);
		writeboxstring(LX + 270, LY + 65, 100, 20, "弧高", 1, 1);
		writeboxstring(LX + 70, LY + 95, 100, 20, "左焊帽", 1, 1);
		writeboxstring(LX + 270, LY + 95, 100, 20, "右焊帽", 1, 1);
		writeboxstring(LX + 70, LY + 125, 100, 20, "左润角", 1, 1);
		writeboxstring(LX + 270, LY + 125, 100, 20, "右润角", 1, 1);
		writeboxstring(LX + 70, LY + 155, 100, 20, "电阻", 1, 1);
		writeboxstring(LX + 270, LY + 155, 100, 20, "合格否", 1, 1);
		}
	//上面是画好框后写字
	for( ; outflag == 0; )
		{
		GetInformation(&info);
		if (info.Flag == Mouse)
			{
			if (info.sBtn == 1)
				{
				if ((info.y < LY + 30) || (info.y > WY))
					continue;
				if ((info.x < LX) || (info.x > WX))
					continue;
				if ((info.x >= WX - 160) && (info.x <= WX - 110) && (info.y >= WY - 30) && (info.y <= WY - 5))
					{
					outflag = 1;
					HideMenuCursor();//selected ok then goto show data
					continue;
					}
				if ((info.x >= WX - 60) && (info.x <= WX - 10) && (info.y >= WY - 30) && (info.y <= WY - 5))
					{
					//writeboxstring(LX, LY, 100, 20, "放弃", 1, 1);
					RestoreScreen();
					return 0; //selected cancel
					}
				if ((info.x >= LX + 40) && (info.y >= WY - 30) && (info.x <= LX + 100) && (info.y <= WY - 5))
					{
					HideMenuCursor();
					if ( all == 0){
						_setcolor(0);
						for(i = LY + 40; i < WY - 60; i += 30)
							{
							_rectangle(0, LX + 50, i, LX + 60, i + 10);
							_rectangle(0, CEN + 50, i, CEN + 60, i + 10);
							}
						for (i = 0; i < 11;i ++) select[i] = 1;
						if (flag == 1)
							{
							_rectangle(0, LX + 50, WY - 50, LX + 60, WY - 40);
							select[10] = 1;
							}
						all = 1;
						}
					else{
						 for(i = LY + 40; i < WY - 60; i += 30){
							_setcolor(230);
							_rectangle(0, LX + 50, i, LX + 60, i + 10);
							_rectangle(0, CEN + 50, i, CEN + 60, i + 10);

							_setcolor(140);
							_xline(CEN + 50, i, CEN + 60, i);
							_xline(LX + 50, i, LX + 60, i);//上
							_setcolor(0);
							_xline(CEN + 50, i, CEN + 50, i + 10);
							_xline(LX + 50, i, LX + 50, i + 10);//左
							_setcolor(255);
							_xline(CEN + 60, i, CEN + 60,i + 10);
							_xline(LX + 60, i, LX + 60,i + 10);//右
							_setcolor(200);
							_xline(CEN + 50, i + 10, CEN + 60, i + 10);
							_xline(LX + 50, i + 10, LX + 60, i + 10);//下
						 }
						 for (i = 1; i < 11;i ++) select[i] = 0;
						 if (flag == 1)
							 {
							 _setcolor(230);
							 _rectangle(0, LX + 50, WY - 50, LX + 60, WY - 40);
							 _setcolor(140);
							 _xline(LX + 50, WY - 50, LX + 60, WY - 50);//上
							 _setcolor(0);
							 _xline(LX + 50, WY - 50, LX + 50, WY - 40);//左
							 _setcolor(255);
							 _xline(LX + 60, WY - 50, LX + 60,WY - 40);//右
							 _setcolor(200);
							 _xline(LX + 50, WY - 40, LX + 60, WY - 40);//下
							 }
						_setcolor(0);
						_rectangle(0, LX + 50, LY + 40, LX + 60, LY + 50);
						select[0] = 1;
						all = 0;
					}
					ShowMenuCursor();
				}//全选
				sum = ((info.y - (LY + 30))/30);
				if ((info.x >= LX + 40) && (info.x <= LX + 70))
					{
					if (flag != 1) {if (info.y > WY - 60) continue;}
					else if (info.y > WY - 30) continue;
					if ((info.y >= LY + 30) && (info.y <= LY + 60))
						{
						messagebox("序号必须选中！");
						_setcolor(0);
						_rectangle(0, LX + 50, LY + 40, LX + 60, LY + 50);
						select[0] = 1;
						continue;
						}
					if (select[sum * 2] == 1)
						{
						select[sum * 2] = 0;
						_setcolor(230);
						HideMenuCursor();
						_rectangle(0, LX + 50, sum * 30 + 180, LX + 60, sum * 30 + 190);
						_setcolor(140);
						_xline(LX + 50, sum * 30 + 180, LX + 60, sum * 30 + 180);//上
						_setcolor(0);
						_xline(LX + 50, sum * 30 + 180, LX + 50, sum * 30 + 190);//左
						_setcolor(255);
						_xline(LX + 60, sum * 30 + 180, LX + 60, sum * 30 + 190);//右
						_setcolor(200);
						_xline(LX + 50, sum * 30 + 190, LX + 60, sum * 30 + 190);//下
						ShowMenuCursor();
						}
					else
						{
						select[sum * 2] = 1;
						_setcolor(0);
						HideMenuCursor();
						_rectangle(0, LX + 50, sum * 30 + 180, LX + 60, sum * 30 + 190);
						ShowMenuCursor();
						}
					}//MOUSE PRESSED LEFT CHECKBOX
				else if ((info.x >= LX + 240) && (info.x <= LX + 270))
					{
					if(info.y > WY - 60) continue;
					if (select[sum * 2 + 1] == 1)
						{
						select[sum * 2 + 1] = 0;
						_setcolor(230);
						HideMenuCursor();
						_rectangle(0, LX + 250, sum * 30 + 180, LX + 260, sum * 30 + 190);
						_setcolor(140);
						_xline(LX + 250, sum * 30 + 180, LX + 260, sum * 30 + 180);//上
						_setcolor(0);
						_xline(LX + 250, sum * 30 + 180, LX + 250, sum * 30 + 190);//左
						_setcolor(255);
						_xline(LX + 260, sum * 30 + 180, LX + 260, sum * 30 + 190);//右
						_setcolor(200);
						_xline(LX + 250, sum * 30 + 190, LX + 260, sum * 30 + 190);//下
						ShowMenuCursor();
						}
					else
						{
						select[sum * 2 + 1] = 1;
						_setcolor(0);
						HideMenuCursor();
						_rectangle(0, LX + 250, sum * 30 + 180, LX + 260, sum * 30 + 190);
						ShowMenuCursor();
						}
					}//MOUSE PRESSED WRIGHT CHECKBOX
				}//END IF     FOR LEFT BUTTON DOWN
			}//END IF    FOR INFORMATION FROM MOUSE
		else if (info.Flag == Key)
		{
			switch(info.sKey)
			{
				case ENTER:
					{
						outflag = 1; continue;
					}
				case ESC:
					{
					RestoreScreen();
					return 0;
					}
				case 'a':
					{
					HideMenuCursor();
					if ( all == 0){
						_setcolor(0);
						for(i = LY + 40; i < WY - 60; i += 30)
							{
							_rectangle(0, LX + 50, i, LX + 60, i + 10);
							_rectangle(0, CEN + 50, i, CEN + 60, i + 10);
							}
						for (i = 0; i < 11;i ++) select[i] = 1;
						if (flag == 1)
							{
							_rectangle(0, LX + 50, WY - 50, LX + 60, WY - 40);
							select[10] = 1;
							}
						all = 1;
						}
					else{
						 for(i = LY + 40; i < WY - 60; i += 30){
							_setcolor(230);
							_rectangle(0, LX + 50, i, LX + 60, i + 10);
							_rectangle(0, CEN + 50, i, CEN + 60, i + 10);

							_setcolor(140);
							_xline(CEN + 50, i, CEN + 60, i);
							_xline(LX + 50, i, LX + 60, i);//上
							_setcolor(0);
							_xline(CEN + 50, i, CEN + 50, i + 10);
							_xline(LX + 50, i, LX + 50, i + 10);//左
							_setcolor(255);
							_xline(CEN + 60, i, CEN + 60,i + 10);
							_xline(LX + 60, i, LX + 60,i + 10);//右
							_setcolor(200);
							_xline(CEN + 50, i + 10, CEN + 60, i + 10);
							_xline(LX + 50, i + 10, LX + 60, i + 10);//下
						 }
						 for (i = 1; i < 11;i ++) select[i] = 0;
						 if (flag == 1)
							 {
							 _setcolor(230);
							 _rectangle(0, LX + 50, WY - 50, LX + 60, WY - 40);
							 _setcolor(140);
							 _xline(LX + 50, WY - 50, LX + 60, WY - 50);//上
							 _setcolor(0);
							 _xline(LX + 50, WY - 50, LX + 50, WY - 40);//左
							 _setcolor(255);
							 _xline(LX + 60, WY - 50, LX + 60,WY - 40);//右
							 _setcolor(200);
							 _xline(LX + 50, WY - 40, LX + 60, WY - 40);//下
							 }
						_setcolor(0);
						_rectangle(0, LX + 50, LY + 40, LX + 60, LY + 50);
						select[0] = 1;
						all = 0;
					}
					ShowMenuCursor();
				}
			}
			//这儿主要是确定和放弃的选择
		}//INFORMATIONG FROM KEYBOARD
	}//END FOR
	showdata(filename,select,flag);
}
int showdata(char filename[20],int select[11],int flag)
{
	int i,j,isprint = 0,page,curpage = 0;
	INFORMATION info;
	FILE *fp,*fprn;
	char fullname[40];
	int outflag = 0,sum;
	int scrnum = 1;
	int x,y,x1,y1;
	int lx[12];
	int sx,sy,idata,k;
	unsigned char key;
	long int offset;
	int ppnum,dispnum,cpoint;
	char str[80],ch[4];
	float fdata;
	sx=4; sy=47;
	x=sx+44;
	y=sy+40;
	lx[0] = x;

	sprintf(fullname, "data\\");
	strcat(fullname, filename);
	if((fp=fopen(fullname, "rb+"))==NULL)  return -1;
	sprintf(str,"");
	if (flag == 1)
	{
	sprintf(str,"序号 极 距 焊点1宽度 焊点1长度 丝径1 焊点2宽度 焊点2长度 丝径2  弧高  电阻  合格否");
	lx[1]=lx[0]+5*8 ;  lx[2]=lx[1]+6*8; lx[3]=lx[2]+10*8;
	lx[4]=lx[3]+10*8; lx[5]=lx[4]+6*8; lx[6]=lx[5]+10*8; 
	lx[7]=lx[6]+10*8; lx[8]=lx[7]+6*8; lx[9]=lx[8]+7*8; 
	lx[10]=lx[9]+6*8; lx[11]=lx[10]+7*8;
	}
	else
	{	
	sprintf(str,"序号   极距   丝径   弧高   左焊帽   右焊帽   左润角   右润角   电阻   合格否");
	lx[1]=lx[0]+7*8; lx[2]=lx[1]+7*8; lx[3]=lx[2]+7*8;
	lx[4]=lx[3]+7*8; lx[5]=lx[4]+9*8; lx[6]=lx[5]+9*8; 
	lx[7]=lx[6]+9*8; lx[8]=lx[7]+9*8; lx[9]=lx[8]+7*8; 
	lx[10]=lx[9]+9*8;
	}
	_setcolor(255);
	_rectangle(3,sx,sy-1,sx+WIN_W,sy+WIN_H);
	_setcolor(0);
	if (flag == 1) WriteBoxString(x+5,y+2,700,20,str,1,1);
	else WriteBoxString(x+10,y+2,700,20,str,1,1);
	sprintf(str,"批次              元件数       合格数     日期            温度       湿度");
	WriteBoxString(x,y-25,700,20,str,0,1);
	WriteBoxString(x+40,y-25,150,20,filename,1,1);
	fseek(fp,8l,SEEK_SET);
	fread(&sum,2,1,fp);
	if (sum == 0)
	{
	if (flag == 1){
		for(i=0;i <= 11;i++)
			_xline(lx[i],y + 25,lx[i],y+50);
		_xline(lx[0],y + 50, lx[11], y + 50);
		}
	else  {
		for(i=0;i <= 10;i++)
			_xline(lx[i],y + 25 ,lx[i],y + 50);
		_xline(lx[0], y + 50, lx[10], y + 50);
		}
	}
	sprintf(str,"%d",sum);
	WriteBoxString(x+200,y-25,60,20,str,1,1);
	fread(&idata,2,1,fp);
	sprintf(str,"%d",idata);
	WriteBoxString(x+300,y-25,60,20,str,1,1);
	fseek(fp,14l,SEEK_SET);
	fread(str,10,1,fp); str[10]=0;
	WriteBoxString(x+375,y-25,100,20,str,1,1);
	fseek(fp,24l,SEEK_SET);
	fread(&idata,2,1,fp);
	sprintf(str,"%d℃",idata);
	WriteBoxString(x+500,y-25,60,20,str,1,1);
	fread(&idata,2,1,fp);
	sprintf(str,"%d%c",idata,'%');
	WriteBoxString(x+600,y-25,100,20,str,1,1);

	outflag=0;
	cpoint=0;
	ppnum=16;
	if (sum<16)  dispnum=sum; else dispnum=ppnum;

	//if (flag == 1)  for(i=0;i <= 11;i++) _xline(lx[i],y,lx[i],y+25);
	//else for(i=0;i <= 10;i++) _xline(lx[i],y,lx[i],y + 25);
	if (flag == 1)
			for(i=0;i <= 11;i++)
				_xline(lx[i],y,lx[i],y + 25);
		else
			for(i=0;i <= 10;i++)
				_xline(lx[i],y,lx[i],y + 25 );
		offset=(long)cpoint*32l+100l;
	for(; outflag == 0 ;)
	{
		_setcolor(0);
		if (flag == 1)
			for(i=0;i <= 11;i++)
				_xline(lx[i],y,lx[i],y + 25 * (dispnum + 1));
		else
			for(i=0;i <= 10;i++)
				_xline(lx[i],y,lx[i],y + 25 * (dispnum + 1));
		if (flag == 1) for(i=0;i<dispnum+2;i++)  _xline(x,y+i*25,lx[11],y+i*25);
		else for(i=0;i<dispnum+2;i++)  _xline(x,y+i*25,lx[10],y+i*25);
		if (flag == 1)
		{	
			for(k = 1; k <= dispnum; k++)
			{
				fseek(fp,(k - 1) * 40 + offset + cpoint * 40, SEEK_SET);
				if (select[0] == 1){
					fread(&idata,2,1,fp);
					sprintf(str,"%3d",idata);
					WriteBoxString(lx[0]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[1] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[1] + 2,y+k*25+2,50,20,str,1,0);
				}
				if (select[2] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[2]+2,y+k*25+2,50,20,str,1,0);
				}
				if (select[3] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[3]+2,y+k*25+2,40,20,str,1,0);
				}
				if (select[4] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[4]+2,y+k*25+2,80,20,str,1,1);
				}
				if (select[5] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[5]+2,y+k*25+2,80,20,str,1,1);
				}
				if (select[6] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[6]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[7] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[7]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[8] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%4.1f",fdata);
					WriteBoxString(lx[8]+2,y+k*25+2,60,20,str,1,0);
				}
				if (select[9] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.3f",fdata);
					WriteBoxString(lx[9]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[10] == 1){
					fread(&idata,2,1,fp);
					sprintf(str,"%4d",idata);
					WriteBoxString(lx[10]+2,y+k*25+2,40,20,str,1,1);
				}
		   }
		}
		if (flag != 1)
		{
			for(k=1;k<=dispnum;k++)	
			{
				fseek(fp,(k - 1) * 32 + offset + 32 * cpoint, SEEK_SET);
				if (select[0] == 1){
					Fread(&idata,2,1,fp);
					sprintf(str,"%4d",idata);
					WriteBoxString(lx[0]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[1] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[1]+2,y+k*25+2,50,20,str,1,0);
				}
				if (select[2] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[2]+2,y+k*25+2,50,20,str,1,0);
				}
				if (select[3] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.1f",fdata);
					WriteBoxString(lx[3]+2,y+k*25+2,40,20,str,1,0);
				}
				if (select[4] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%6.1f",fdata);
					WriteBoxString(lx[4]+2,y+k*25+2,80,20,str,1,1);
				}
				if (select[5] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%6.1f",fdata);
					WriteBoxString(lx[5]+2,y+k*25+2,80,20,str,1,1);
				}
				if (select[6] == 1){
					fread(&idata,2,1,fp);
					sprintf(str,"%4d",idata);
					WriteBoxString(lx[6]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[7] == 1){
					fread(&idata,2,1,fp);
					sprintf(str,"%4d",idata);
					WriteBoxString(lx[7]+2,y+k*25+2,40,20,str,1,1);
				}
				if (select[8] == 1){
					fread(&fdata,4,1,fp);
					sprintf(str,"%5.3f",fdata);
					WriteBoxString(lx[8]+2,y+k*25+2,60,20,str,1,0);
				}
				if (select[9] == 1){
					fread(&idata,2,1,fp);
					sprintf(str,"%4d",idata);
					WriteBoxString(lx[9]+2,y+k*25+2,40,20,str,1,1);
				}
	 		}   // read data  circle
		}

	if (isprint == 1)
	{	
		printscr(scrnum,fprn);
		if (cpoint < sum - ppnum) 
		{
			cpoint = cpoint + ppnum;
			if ((sum - cpoint) >= ppnum)	dispnum = ppnum; 
			else dispnum = sum - cpoint;
			_setcolor(255);
			_rectangle(3,lx[0],y+25,lx[0]+664,y+460);			
			if (curpage == 1)
			{
				if (scrnum %2 == 0)	{
						outflag = 1;
						messagebox("打印完成。");
						fclose(fprn); 					
					}
			}//如果是打印当前页的话
			scrnum ++;
			if (scrnum%2 == 0)
			{ 
				page = scrnum / 2;
				sprintf(str,"第");
				sprintf(ch, "%2d", page);
				strcat(str,ch);
				strcat(str," 页");
				WriteBoxString(x + 300,y+440,100,20,str,1,1);
			}
		}
		else{
			if (scrnum%2 != 0)
				{
				_setcolor(255);
				_rectangle(3,lx[0],y - 40,lx[0]+664,y+460);
				scrnum ++;
				page = scrnum /2;
				sprintf(str,"第");
				sprintf(ch, "%2d", page);
				strcat(str,ch);
				strcat(str," 页");
				WriteboxString(x + 300, y + 440, 100, 20,str,1,1);
				printscr(scrnum,fprn);
				}
			 outflag = 1; messagebox("打印完成"); fclose(fprn);
		     }
	}//当isprint = 1时的循环
	else
	{
		key=getch();
		switch(key)
			{
			case mEnter: //全打印
				if( choicedialog("检查打印","接好打印机后继续",1)==1)
					{
					cpoint=0;
					if ((sum - cpoint) >= ppnum)    dispnum = ppnum;
					else dispnum = sum - cpoint;
					_setcolor(255);
					_rectangle(3,lx[0],y+25,lx[0]+664,y+460);
					isprint = 1;
					scrnum = 1;
					fprn=fopen("PRN","wb");
					if (fprn == NULL)
						{
						messagebox("error");
						return 0;
						}
					fprintf(fprn,"%cE",0x1b);
					break;
					}
				else break;
			case 'p'://打印当前页
				if( choicedialog("检查打印","接好打印机后继续",1)==1)
					{
					if (scrnum%2 == 0) {cpoint = cpoint - ppnum;scrnum --;}
					if ((sum - cpoint) >= ppnum)    dispnum = ppnum;
					else dispnum = sum - cpoint;
					_setcolor(255);
					_rectangle(3,lx[0],y+25,lx[0]+664,y+460);
					isprint = 1;
					curpage = 1;
					fprn = fopen("PRN","wb");
					if (fprn == NULL)
						{
						messagebox("error");
						return 0;
						}
					fprintf(fprn,"%cE",0x1b);
					break;
					}
				else    break;
			case ESC:  outflag=1; break;
			case mPageup:
				if (cpoint>0) {
		  			cpoint=cpoint-ppnum;
		  			dispnum=ppnum;
	     			_setcolor(255);
					_rectangle(3,lx[0],y+25,lx[0]+664,y+460);
					scrnum --;
					if (scrnum%2 == 0)
						{
						page = scrnum / 2;
						sprintf(str,"第");
						sprintf(ch,"%2d",page);
						strcat(str,ch);
						strcat(str," 页");
						WriteBoxString(x + 300,y+440,100,20,str,1,1);
						}
				}
				break;
			case mPagedown:
				if (cpoint < sum - ppnum) {
					cpoint = cpoint + ppnum;
					if ((sum - cpoint) >= ppnum)
						dispnum = ppnum; 
					else dispnum = sum - cpoint;
					_setcolor(255);
					_rectangle(3,lx[0],y+25,lx[0]+664,y+460);
					scrnum ++;
					if (scrnum%2 == 0)
						{
						page = scrnum / 2;
						sprintf(str,"第");
						sprintf(ch, "%2d", page);
						strcat(str,ch);
						strcat(str," 页");
						WriteBoxString(x + 300,y+440,100,20,str,1,1);
						}
					}
				break;
			case mHome:
				if (cpoint>0) {
					cpoint=0;
					dispnum=ppnum;
					_setcolor(255);
					_rectangle(3,lx[0],y+25,lx[0]+664,y+460);
					scrnum = 1;
				}
				break;
			default: break;
			}
		}//judge if is print
	}  // for(;outflag==0;) circrle
	restoreScreen();
	fclose(fp);
}

int printscr(int scrnum,FILE *fp)
{
	unsigned char c,str[5],cc[20]="中ab";
	static int levels[]={1,3,11,21,36,56,81,100};
	int i,j,k,m;
	int x,y,col;
	int port;
	int off,offy;
	unsigned char  gray[800];
	unsigned char  pdata[100];

	//FILE *fp;
       // fp = fopen("PRN","wb");
	esc=0x1b;
	HideMenuCursor();
	//if (fp==NULL) {printf("printer open erro"); return 0;}
       // fprintf(fp,"%cE",esc);
	x=150;
	if ((scrnum%2)==0)  {  y = 240; off=65; }
	   else {   y = 240; off=15; }

	   offy=1335;
	if ((scrnum%2)==0)
	{ pcursor(fp, x, y + offy);
	 }
	else
	{ pcursor(fp,x,y);
	}

	fprintf(fp,"%c*t100R",esc);   // reslotion
	fprintf(fp,"%c*r1A",esc);   // set graph mode  cursor poistion  must be set
			    // at first the ,or will not be used
    // sendimage
    col=95;
	for(i=1;i<=450;i++)//520行数
	{
	    ReadingVGAWin(40,i+46+off,720,1,gray);//列数必须是8的倍数
	    fprintf(fp,"%c*b90W",esc);
	    for(j=0;j<90;j++)
		{
			c=0;
			m=128;
			for(k=0;k<4;k++)
			{
				//if (gray[j*8+k]<100) c+=m; m/=2;
				if (gray[j*8+k]<100) c+=1;
				if (k==3) break;
				c=c<<1;
			}
			c=c<<4;
			pdata[j]=c;
		    fwrite(&c,1,1,fp);
			// fprintf(fp,"%crB",esc);
		}
	}
	fprintf(fp,"%crB",esc);

	if ((scrnum%2)==0 )
	{  pcursor(fp,x,y+offy);    }
	else
	{  pcursor(fp, x, y);}

	fprintf(fp,"%c*t100R",esc);   // reslotion
	fprintf(fp,"%c*r1A",esc);   // set graph mode  cursor poistion  must be set

	for(i=1;i<=450;i++)
	{
		ReadingVGAWin(40,i+46+off,720,1,gray);
		fprintf(fp,"%c*b90W",esc);
	    for(j=0;j<90;j++)
		{
			c=0;
			for(k=4;k<8;k++)
			{
				if (gray[j*8+k]<100) c+=1;
				if (k==7) break;
				c=c<<1;
			}
			pdata[j]=c;
			fwrite(&c,1,1,fp);
	    }
	}

	fprintf(fp,"%crB",esc);  // end graphic

	if (scrnum%2==0)
	{
		fprintf(fp,"%c&l0H",esc);  // eject paper
	}
       // fclose(fp);
	return 1;
}
