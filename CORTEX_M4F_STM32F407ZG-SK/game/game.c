#include "game.h"
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Player1
int16_t player1X = 10;
int16_t player1Y = 10;
uint16_t player1W = 60;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;

//Player2
int16_t player2X = LCD_PIXEL_WIDTH - 20;
int16_t player2Y = LCD_PIXEL_HEIGHT - 20;
uint16_t player2W = 60;
uint16_t player2H = 10;
uint8_t player2IsReversed = 0;

//Ball
uint16_t ballSize = 5;
int16_t ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
int16_t ballY = ( LCD_PIXEL_HEIGHT - 5 ) / 2;
int16_t ballVX ;
int16_t ballVY ;
uint8_t ballIsRun = 0;

//Mode
uint8_t demoMode = 0;

//wall
uint16_t wall1W = 50;
uint16_t wall1H = 10;
int16_t wall1X = 50;
int16_t wall1Y = 155;

uint16_t wall2W = 50;
uint16_t wall2H = 10;
int16_t wall2X = 160;
int16_t wall2Y = 155;

int num,i=0,count,player2,player1;


void
BallReset()
{
	ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
	ballY = ( LCD_PIXEL_HEIGHT - 5 ) / 2;

  	num=rand()%4+1;
	switch(num){
		case 1:
			ballVX = 5;
			ballVY = 2; 
			break;
		case 2:
			ballVX = 5;
			ballVY = -2; 
			break;
		case 3:
			ballVX = -5;
			ballVY = 2; 
			break;		
		case 4:
			ballVX = -5;
			ballVY = -2;
			break;		
		}
	ballIsRun = 1;
}

void
GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ) ){

		player1IsReversed = 0;

		while( STM_EVAL_PBGetState( BUTTON_USER ) );

		player1IsReversed = 1;
	}
}

void
GAME_EventHandler2()
{
	if( IOE_TP_GetState()->TouchDetected ){

		player2IsReversed = 1;

		while( IOE_TP_GetState()->TouchDetected );

		player2IsReversed = 0;
	}
}

void
GAME_EventHandler3()
{
	if( ballIsRun == 0 ){
		BallReset();
	}
}

void
GAME_Update()
{
	//Player1
	LCD_SetTextColor( LCD_COLOR_BLACK );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );


	if( demoMode == 0 ){

		if( player1IsReversed )
			player1X -= 9;
		else
			player1X += 9;

		if( player1X <= 0 )
			player1X = 0;
		else if( player1X + player1W >= LCD_PIXEL_WIDTH )
			player1X = LCD_PIXEL_WIDTH - player1W;

		//Player2
//		if( player2IsReversed )
//			player2X -= 5;
//		else
//			player2X += 5;

	//Player2 move
		if( ballVY > 0 ){
			if( player2X + player2W/2 < ballX + ballSize/2 ){
				//player1X += 4;
				player2X += 9;
			}
			else{
				//player1X -= 4;
				player2X -= 9;
			}

		}

		if( player2X <= 0 )
			player2X = 0;
		else if( player2X + player2W >= LCD_PIXEL_WIDTH )
			player2X = LCD_PIXEL_WIDTH - player2W;

		//Ball
		if( ballIsRun == 1 ){
			
			LCD_SetTextColor( LCD_COLOR_BLACK );
			LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
										
		//Touch wall
			ballX += ballVX;
			if( ballX <= 0 ){
				ballX = 0;
				ballVX *= -1;
			}
			else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
				ballX = LCD_PIXEL_WIDTH - ballSize;
				ballVX *= -1;
			}

		//PONG!
 		//wall
			ballX += ballVX;
			ballY += ballVY;
			
//			if( ballX >= 50 && ballX <= 100 && (ballY ==155 || ballY ==156) ){		
//			         ballVX *= -1;
//				 ballVY *= -1;
//						}


		//PONG!
			ballY += ballVY;
			//ballX += ballVX;
			if( ballY + ballSize >= player2Y ){
				if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
					if( ballX - ballSize <= player2Y + player2W/4 ){
						ballVY =-2;
						ballVX =-3;
					}
					else if( ballX >= player2Y + player2W - player2W/4 ){
						ballVY =-2;
						ballVX = 3;
					}
					else if( ballX + ballSize < player2Y + player2W/2 ){
						ballVY =-2;
						ballVX =-4;
					}
					else if( ballX > player2Y + player2W/2 ){
						ballVY =-2;
						ballVX = 4;
					}

					else{
						ballVY =-5;
						ballVX = 0;
					}


				}
				else{
					BallReset();
                                        i++;
					player1++;		}
			}

			if( ballX >= 150 && ballX <= 210 && (ballY >=153 && ballY <=158) ){
				 ballVX = -1*ballVX-1;
				 ballVY = -1*ballVY-1;
						}
			if( ballX >= 40 && ballX <= 90 && (ballY >=150 && ballY <=155) ){		
			         ballVX = -1*ballVX-1;
				 ballVY = -1*ballVY-1;
						}
//////////////////////////////////////////////////////////////////////////////////////////


			if( ballY <= player1Y + player1H ){
					if( ballX + ballSize >= player1X && ballX <= player1X + player1W ){
						if( ballX - ballSize <= player1Y + player1W/4 ){
							ballVY = 4;
							ballVX =-2;
						}
						else if( ballX >= player1Y + player1W - player1W/4 ){
							ballVY = 3;
							ballVX = 2;
						}
						else if( ballX + ballSize < player1Y + player1W/2 ){
							ballVY = 2;
							ballVX =-2;
						}
						else if( ballX > player1Y + player1W/2 ){
							ballVY = 4;
							ballVX = 2;
						}

						else{
							ballVY = 3;
							ballVX = 4;
						}

					}
					else{
						BallReset();
						i++;         	
						player2++;		}
				}




			}				
		}
					
////////////////////////////////////////////////////////////////////////////////////////////		
		else{	//if demoMode == 1

			//Player1 move
			if( ballVY < 0 ){
				if( player1X + player1W/2 < ballX + ballSize/2 ){
					player1X += 8;
					player2X += 2;
				}
				else{
					player1X -= 8;
					player2X -= 2;
				}
			}

			//Player2 move
			if( ballVY > 0 ){
				if( player2X + player2W/2 < ballX + ballSize/2 ){
					player1X += 2;
					player2X += 8;
				}
				else{
					player1X -= 2;
					player2X -= 8;
				}

			}

			if( player1X <= 0 )
				player1X = 0;
			else if( player1X + player1W >= LCD_PIXEL_WIDTH )
				player1X = LCD_PIXEL_WIDTH - player1W;

			if( player2X <= 0 )
				player2X = 0;
			else if( player2X + player2W >= LCD_PIXEL_WIDTH )
				player2X = LCD_PIXEL_WIDTH - player2W;


			//Ball
			if( ballIsRun == 1 ){

				LCD_SetTextColor( LCD_COLOR_BLACK );
				LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );

				//Touch wall
				ballX += ballVX;
				if( ballX <= 0 ){
					ballX = 0;
					ballVX *= -1;
				}
				else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
					ballX = LCD_PIXEL_WIDTH - ballSize;
					ballVX *= -1;
				}

				//PONG!
				ballY += ballVY;

				if( ballY + ballSize >= player2Y ){
					if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
					if( ballX - ballSize <= player2Y + player2W/4 ){
						ballVY =-3;
						ballVX =-7;
					}
					else if( ballX >= player2Y + player2W - player2W/4 ){
						ballVY =-3;
						ballVX = 7;
					}
					else if( ballX + ballSize < player2Y + player2W/2 ){
						ballVY =-7;
						ballVX =-3;
					}
					else if( ballX > player2Y + player2W/2 ){
						ballVY =-7;
						ballVX = 3;
					}
					else{
						ballVY =-9;
						ballVX = 0;
					}

				}
				else
					BallReset();

			}

			if( ballY <= player1Y + player1H ){
				if( ballX + ballSize >= player1X && ballX <= player1X + player1W ){
					if( ballX - ballSize <= player1Y + player1W/4 ){
						ballVY = 3;
						ballVX =-7;
					}
					else if( ballX >= player1Y + player1W - player1W/4 ){
						ballVY = 3;
						ballVX = 7;
					}
					else if( ballX + ballSize < player1Y + player1W/2 ){
						ballVY = 7;
						ballVX =-3;
					}
					else if( ballX > player1Y + player1W/2 ){
						ballVY = 7;
						ballVX = 3;
					}
					else{
						ballVY = 9;
						ballVX = 0;
					}

				}
				else
					BallReset();
			}
		}
	}
}

void
GAME_Render()
{

		LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );

	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
count=i%5+1;
switch(count){
	case 1:		
		LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
		break;	
	case 2:		
		LCD_SetTextColor( LCD_COLOR_GREY );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
		break;
	case 3:		
		LCD_SetTextColor( LCD_COLOR_BLUE2 );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
		break;	
	case 4:		
		LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
		break;
	case 5:		
		LCD_SetTextColor( LCD_COLOR_MAGENTA );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
		break;
}
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawLine( 10, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH - 20, LCD_DIR_HORIZONTAL );
	LCD_SetTextColor( LCD_COLOR_CYAN );
	LCD_DrawFullRect( wall1X, wall1Y, wall1W, wall1H);
	LCD_DrawFullRect( wall2X, wall2Y, wall2W, wall2H);
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayChar(LCD_PIXEL_HEIGHT/2+5,10, player2+48);
	LCD_DisplayChar(LCD_PIXEL_HEIGHT/2-25,10, player1+48);
if(player1>9||player2>9){
	player1=0;
	player2=0;
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-30, "  Game Over !!");
			}

}

