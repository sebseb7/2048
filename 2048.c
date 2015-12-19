#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//srand((unsigned int)time(NULL));  #include <time.h>
#include "2048.h"

#include "mcugui/rect.h"
#include "mcugui/text.h"



unsigned int* pixbuf;

static int field[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

void setLedXY(uint16_t x, uint16_t y, uint8_t red,uint8_t green, uint8_t blue) {
	
	pixbuf[y*90*4+x] = (red<<16)+(green<<8)+blue;

}
void getLedXY(uint16_t x, uint16_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {

	int col = pixbuf[y*90*4+x];

	*red = col>>16;
	*green = (col>>8)&0xff;
	*blue = col&0xff;

}
void invLedXY(uint16_t x, uint16_t y) {
}

static int curr=0;

static void setrand(void)
{
	int free=0;
	
	for(int x=0;x<4;x++)
	{
		for(int y=0;y<4;y++)
		{
			if(field[x][y]==0)
				free++;
		}
	}

	int choosen = rand()%free;

	for(int x=0;x<4;x++)
	{
		for(int y=0;y<4;y++)
		{
			if((choosen==0)&&(field[x][y]==0))
			{
				curr++;
				field[x][y]=2;
				return;
			}
			if(field[x][y]==0)
				choosen--;
		}
	}

}

static int move(int dir_x, int dir_y)
{
	int newfield[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	int move =0;
	for(int x=0;x<4;x++)
	{
		for(int y=0;y<4;y++)
		{
			if(
				( x+dir_x >= 0)&&(x+dir_x < 4)&&( y+dir_y >= 0)&&(y+dir_y < 4)
			)
			{
				if((field[x+dir_x][y+dir_y]==0)&&(field[x][y] != 0)&&(newfield[x+dir_x][y+dir_y] == 0))
				{
					newfield[x+dir_x][y+dir_y]=field[x][y];
					newfield[x][y]=0;
					move++;
				}
				else
				{
					if(newfield[x][y] == 0)newfield[x][y]=field[x][y];
				}

			}
			else
			{
					if(newfield[x][y] == 0)newfield[x][y]=field[x][y];
			}
		}
	}
	if(move)
	{
		for(int x=0;x<4;x++)
		{
			for(int y=0;y<4;y++)
			{
				field[x][y]=newfield[x][y];
			}
		}
		return 1;
	}
	return 0;
	
}

static int addup(int dir_x, int dir_y)
{
	int newfield[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	int move =0;
	for(int x=0;x<4;x++)
	{
		for(int y=0;y<4;y++)
		{
			if(
				( x+dir_x >= 0)&&(x+dir_x < 4)&&( y+dir_y >= 0)&&(y+dir_y < 4)
			)
			{
				if((field[x][y] != 0)&&(field[x+dir_x][y+dir_y]==field[x][y])&&((newfield[x+dir_x][y+dir_y] == 0)||(newfield[x][y] == 0)))
				{
					newfield[x+dir_x][y+dir_y]=field[x][y]*2;
					newfield[x][y]=0;
					field[x+dir_x][y+dir_y]=0;
					field[x][y]=0;
					move++;
				}
				else
				{
					if(newfield[x][y] == 0)newfield[x][y]=field[x][y];
				}

			}
			else
			{
				if(newfield[x][y] == 0)newfield[x][y]=field[x][y];
			}
		}
	}
	if(move>0)
	{
		for(int x=0;x<4;x++)
		{
			for(int y=0;y<4;y++)
			{
				field[x][y]=newfield[x][y];
			}
		}
		return 1;
	}
	return 0;
	
}

static int init = 0;


void update_2048(int getkey(int))
{
	if(init==0)
	{
		init=1;
		setrand();
		setrand();
	}

	int movx = getkey(1)-getkey(3);
	int movy = getkey(2)-getkey(0);


	if(movx != 0)
	{
		int cnt = 0;
		while(move(movx,0)){cnt++;};
		cnt+=addup(movx,0);
		cnt+=move(movx,0);
		if(cnt>0) setrand();
	}
	else if(movy !=0)
	{
		int cnt = 0;
		while(move(0,movy)){cnt++;};
		cnt+=addup(0,movy);
		cnt+=move(0,movy);
		if(cnt>0) setrand();
	}
};




void render_2048(unsigned int* pixelbuffer,int zoom) 
{
	pixbuf=pixelbuffer;



	for(int x=0;x<4;x++)
	{
		for(int y=0;y<4;y++)
		{
			draw_filledRect(x*zoom+5,y*zoom+5,zoom-10,zoom-10,0,100,100);
			if(field[x][y] == 0)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,150,100,30);
			else if(field[x][y]== 2)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,210,0);
			else if(field[x][y]== 4)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,180,30);
			else if(field[x][y]== 8)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,150,60);
			else if(field[x][y]== 16)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,120,90);
			else if(field[x][y]== 32)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,90,120);
			else if(field[x][y]== 64)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,60,150);
			else if(field[x][y]== 128)
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,30,180);
			else
				draw_filledRect(x*zoom+10,y*zoom+10,zoom-20,zoom-20,100,0,210);


			char text[10];

			snprintf(text,10, "%i", field[x][y]);

			draw_text_16pt(x*zoom+30,y*zoom+30,text,255,255,255);

		}
	}


}

