#include "UI.h"
#include "SpriteData.h"
#include "3DMap.h"
#include "playerManager.h"
#include "SceneMain.h"
// 移動アルゴリズムの実体
UIListMain		    UIMain;         //主メニュー
UIListSecondary     UISecondary;    //建築メニュー
UIListAttribute     UIAttribute;    //属性UI 
UIListDay           UIDay;          //日付 上旬とか
UIListMonth         UIMonth;        //日付 月の字
UIListMonthNo       UIMonthNo;      //日付 月の番号
UIListHelp          UIHelp;         //メニューのヘルプ選択肢
UIListChooseFrame   UIchooseFrame;  //メニューから選択時の枠
UIListMoney1         UIMoney1;      //お金の表示  左から一桁目
UIListMoney2         UIMoney10;     //お金の表示　左から二桁目
UIListMoney3         UIMoney100;    //お金の表示　左から三桁目
UIListMoney4         UIMoney1000;   //お金の表示　左から四桁目
UIListMoney          UIMoneyCharacter; //お金の字
UIListArrowLeft      UIArrowLeft;
UIListArrowRight     UIArrowRight;

bool MousePosCheck(POINT point, int x, int y, int x2, int y2) {
	if (point.x >= x&&point.x <= x2) {
		if (point.y >= y&&point.y <= y2)
			return true;
	}
	return false;
}

#define LIST_SIZE_X   (64)
#define LIST_SIZE_Y   (64)
#define LIST_TEX_SIZE_X   (62)
#define LIST_TEX_SIZE_Y   (256)
void UIListMain::move(OBJ2D* obj,bool leftFlag,int month,int day,int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/UI1.png");
		obj->pos = VECTOR2(0, 0);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(LIST_SIZE_X, LIST_TEX_SIZE_Y);
		obj->texSize = VECTOR2(LIST_TEX_SIZE_X, LIST_TEX_SIZE_Y);
		obj->canDraw = true;   //いつでも見える
		obj->type = frame;
		stage++;
	case 1:	
		point = mScreenToClient();
		if (leftFlag) {
			if (MousePosCheck(point, LIST_SIZE_X*0, LIST_SIZE_Y *0, LIST_SIZE_X*1, LIST_SIZE_Y*1)) {
					UISecondary.drawFlag = true;
				}
			if (MousePosCheck(point, LIST_SIZE_X*0, LIST_SIZE_Y*1, LIST_SIZE_X*1, LIST_SIZE_Y*2)) {
				playerManager.addPlayFlag = true;
			}
			if (MousePosCheck(point, LIST_SIZE_X*0, LIST_SIZE_Y*2, LIST_SIZE_X*1, LIST_SIZE_Y*3)) {
				UIHelp.drawFlag = true;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
#define UILIST_SECONDARY_POS_X (64)
#define UILIST_SECONDARY_POS_Y (0)
#define UILIST_SECONDARY_SIZE_X (64)
#define UILIST_SECONDARY_SIZE_Y (256)
#define UILIST_SECONDARY_TEX_SIZE_X (62)
#define UILIST_SECONDARY_TEX_SIZE_Y (256)
#define UILIST_SECONDARY_TEX_POS_X (0)
#define UILIST_SECONDARY_TEX_POS_Y (0)
void UIListSecondary::move(OBJ2D* obj,bool leftFlag, int month, int day, int money)
{
	
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/UI2.png",UI::Secondary_rood,UI::Secondary_max);
		obj->SetSprite(L"./Data/picture/UI3.png",UI::Secondary_gymnasium,UI::Secondary_max);
		obj->pos = VECTOR2(UILIST_SECONDARY_POS_X, UILIST_SECONDARY_POS_Y);
		obj->size = VECTOR2(UILIST_SECONDARY_SIZE_X, UILIST_SECONDARY_SIZE_Y);
		obj->texPos = VECTOR2(UILIST_SECONDARY_TEX_POS_X, UILIST_SECONDARY_TEX_POS_Y);
		obj->texSize = VECTOR2(UILIST_SECONDARY_TEX_SIZE_X, UILIST_SECONDARY_TEX_SIZE_Y);
		obj->canDraw = false;
		stage++;
		//break;
	case 1:
		drawNO = 0;
		obj->canDraw = drawFlag;
		if (obj->canDraw) {
			stage++;
		}
		break;
	case 2:
		obj->ChangeSprite(drawNO);
		point = mScreenToClient();
		if (leftFlag) {
			if (MousePosCheck(point, UILIST_SECONDARY_POS_X, UILIST_SECONDARY_SIZE_Y/4*0, UILIST_SECONDARY_POS_X+ UILIST_SECONDARY_SIZE_X, UILIST_SECONDARY_SIZE_Y/4*1)) 
			{
				if (drawNO == T_L_A) {
					mapData.canCreate = road;
				}
				else if (drawNO == G_M_T) {
					mapData.canCreate = gymnasium;
				}
				drawFlag = false;
				stage = 1;
			}
			else if (MousePosCheck(point, UILIST_SECONDARY_POS_X, UILIST_SECONDARY_SIZE_Y / 4 * 1, UILIST_SECONDARY_POS_X + UILIST_SECONDARY_SIZE_X, UILIST_SECONDARY_SIZE_Y / 4 * 2)) {
				if (drawNO == T_L_A) {
					mapData.canCreate = teacherRoom;
				}
				else if (drawNO == G_M_T) {
					mapData.canCreate = musicRoom;
				}
				drawFlag = false;
				stage = 1;
			}
			else if (MousePosCheck(point, UILIST_SECONDARY_POS_X, UILIST_SECONDARY_SIZE_Y / 4 * 2, UILIST_SECONDARY_POS_X + UILIST_SECONDARY_SIZE_X, UILIST_SECONDARY_SIZE_Y / 4 * 3)) {
				if (drawNO == T_L_A) {
					mapData.canCreate = library;
				}
				else if (drawNO == G_M_T) {
					mapData.canCreate = toilet;
				}
				drawFlag = false;
				stage = 1;
			}
			else if (MousePosCheck(point, UILIST_SECONDARY_POS_X, UILIST_SECONDARY_SIZE_Y / 4 * 3, UILIST_SECONDARY_POS_X + UILIST_SECONDARY_SIZE_X, UILIST_SECONDARY_SIZE_Y / 4 * 4)) {
				if (drawNO == T_L_A) {
					mapData.canCreate = autoShop;
				}
				
				drawFlag = false;
				stage = 1;
			}
			
			if (MousePosCheck(point, ARROW_RIGHT_POS_X, ARROW_RIGHT_POS_Y, ARROW_RIGHT_POS_X + ARROW_SIZE, ARROW_RIGHT_POS_Y + ARROW_SIZE)) {
				drawNO++;
				if (drawNO >= 1)drawNO = 1;
			}
			else if (MousePosCheck(point, ARROW_LEFT_POS_X, ARROW_LEFT_POS_Y, ARROW_LEFT_POS_X + ARROW_SIZE, ARROW_LEFT_POS_Y + ARROW_SIZE)) {
				drawNO--;
				if (drawNO <= 0)drawNO = 0;
			}
			else {
				drawFlag = false;
				stage = 1;
			}
		}
		obj->ChangeSprite(drawNO);
		break;
	}
}


#define UILIST_ATTR
void UIListAttribute::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/background.png");
		obj->pos = VECTOR2(15, 350);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(300, 200);
		obj->texSize = VECTOR2(200, 200);
		obj->canDraw = false;   
		obj->type = character;
		obj->learning = 0;
		obj->spirit = 0;
		obj->communication = 0;
		obj->earnMoneyDarwFlag = false;
		earnMoneyDarwFlag = false;
		PlayerOrBillboard = NONE;
		stage++;
	case 1:
		obj->canDraw = drawFlag;
		obj->earnMoneyDarwFlag = earnMoneyDarwFlag;
		if (obj->canDraw) {
			stage++;
		}
		break;
	case 2:
		point = mScreenToClient();
		obj->learning = learning;
		obj->spirit = spirit;
		obj->communication = communication;
		obj->earnMoney = earnMoney;
		obj->earnMoneyDarwFlag = earnMoneyDarwFlag;
		if (leftFlag) {
			if (MousePosCheck(point, 15, 350, 15+300, 350+200)) {
				break;
			}
			else {
				earnMoneyDarwFlag = false;
				drawFlag = false;
				learning = 0;
				spirit = 0;
				communication = 0;
				PlayerOrBillboard = NONE;
				stage = 1;
			}
		}

		break;
	}
}

//月（字の表示）
void UIListDay::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/month.png");
		obj->pos = VECTOR2(530, 10);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(32, 32);
		obj->texSize = VECTOR2(64, 64);
		obj->canDraw = true;   //いつでも見える
		obj->type = frame;
		stage++;
	case 1:
		break;
	}
}
//日表示
void UIListMonth::move(OBJ2D* obj, bool leftFlag, int month, int day,int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/early.png", UIDate::DateEarly, UIDate::DateMax);
		obj->SetSprite(L"./Data/picture/mid.png", UIDate::DateMid, UIDate::DateMax);
		obj->SetSprite(L"./Data/picture/late.png", UIDate::DateLate, UIDate::DateMax);
		obj->pos = VECTOR2(620, 10);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(64, 32);
		obj->texSize = VECTOR2(128, 64);
		obj->canDraw = true;   //いつでも見える
		obj->type = frame;
		stage++;
	case 1:
		obj->ChangeSprite(day);
		break;
	}
}
//月（番号の表示）
void UIListMonthNo::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/No.png");
		obj->pos = VECTOR2(500, 15);
		obj->texPos = VECTOR2(64, 0);
		obj->size = VECTOR2(32, 32);
		obj->texSize = VECTOR2(64, 64);
		obj->canDraw = true;   //いつでも見える
		obj->type = frame;
		stage++;
	case 1:
		if (month >= 12)month = 12;
		obj->texPos = VECTOR2((float)((month-1) * 64), 0);
		break;
	}
}
#define HELP_POS_X (320)
#define HELP_POS_Y (180)
#define HELP_TEX_X (700)
#define HELP_TEX_Y (300)
void UIListHelp::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/help.png");
		obj->pos = VECTOR2(HELP_POS_X, HELP_POS_Y);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(HELP_TEX_X, HELP_TEX_Y);
		obj->texSize = VECTOR2(HELP_TEX_X, HELP_TEX_Y);
		obj->canDraw = false;  
		drawFlag = false;
		stage++;
	case 1:
		obj->canDraw = drawFlag;
		if (obj->canDraw)stage++;
		break;
	case 2:
		point = mScreenToClient();
		if (leftFlag) {
			if (!MousePosCheck(point, 320, 180, 320 + 700, 180 + 300)) {
				drawFlag = false;
				stage = 1;
			}
		}
		break;
	}
}


void UIListChooseFrame::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/UIwaku.png");
		obj->pos = VECTOR2(0, 0);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(32, 32);
		obj->texSize = VECTOR2(32, 32);
		posNum = 0;
		obj->canDraw = false;
		drawFlag = false;
		stage++;
	case 1:
		obj->canDraw = drawFlag;
		if (obj->canDraw)stage++;
		break;
	case 2:
		point = mScreenToClient();
		obj->size = VECTOR2(FRAME_SIZE, FRAME_SIZE);
		if(posNum==UIFrame::eUIBuilding)obj->pos = VECTOR2(FRAME_SIZE*0,FRAME_SIZE *0);
		if(posNum==UIFrame::eUIEvent)obj->pos =    VECTOR2(FRAME_SIZE*0,FRAME_SIZE *1);
		if(posNum==UIFrame::eUIHelp)obj->pos =     VECTOR2(FRAME_SIZE*0,FRAME_SIZE *2);
		if(posNum==UIFrame::eUIRoad)obj->pos =     VECTOR2(FRAME_SIZE*1, FRAME_SIZE *0);
		if(posNum==UIFrame::eUITeachroom)obj->pos =VECTOR2(FRAME_SIZE*1, FRAME_SIZE *1);
		if(posNum==UIFrame::eUILibrary)obj->pos =  VECTOR2(FRAME_SIZE*1, FRAME_SIZE *2);
		if(posNum==UIFrame::eUIVendingMachine)obj->pos = VECTOR2(FRAME_SIZE, FRAME_SIZE*3);
		if (posNum == UIFrame::eUIArrowRight) {
			obj->size = VECTOR2(FRAME_SIZE/2, FRAME_SIZE/2);
			obj->pos = VECTOR2(ARROW_RIGHT_POS_X, ARROW_RIGHT_POS_Y);
		}
		if (posNum == UIFrame::eUIArrowLeft) {
			obj->size = VECTOR2(FRAME_SIZE/2, FRAME_SIZE/2);
			obj->pos = VECTOR2(ARROW_LEFT_POS_X, ARROW_LEFT_POS_Y);
		}
		if (!drawFlag) { 
			stage = 1; 
			posNum = 0;
		}
		break;
	}
}

void UIListMoney::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/money.png");
		obj->texPos = VECTOR2(0, 0);
		obj->texSize = VECTOR2(DIGIT_SIZE, DIGIT_SIZE);
		obj->canDraw = true;
		obj->pos = VECTOR2(NUMBER_POS_X-DIGIT_SIZE, NUMBER_POS_Y);
		obj->size = VECTOR2(45, 45);
		stage++;
	case 1:
		break;
	}
}
void UIListMoney1::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/number.png");
		obj->texPos = VECTOR2(0, 0);
		obj->texSize = VECTOR2(DIGIT_SIZE, DIGIT_SIZE);
		obj->canDraw = true;
		SetDigitDispSize(DigitDispSize);  //数値表示の際の文字サイズを設定
		numberPosX = NUMBER_POS_X;
		numberPosY = NUMBER_POS_Y;
		numberPosX += digitDispSize * DIGIT_MAX;  //  一の位から描画するので右端にずらす
		numberPosX -= digitDispSize * 1;        //  描画桁を左にずらす     
		obj->pos = VECTOR2(numberPosX, numberPosY);
		obj->size = VECTOR2(digitDispSize, digitDispSize);
		stage++;
	case 1:
		SetNumber(money);      //各桁の値を設定
		
		float sx = (float)((digit[0] % NUM_DIGIT_1LINE) * DIGIT_SIZE);
		float sy = (float((digit[0] / NUM_DIGIT_1LINE) * DIGIT_SIZE));
		obj->texPos = VECTOR2(sx, sy);
		
		break;
	}
}
void UIListMoney2::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/number.png");
		obj->texPos = VECTOR2(0, 0);
		obj->texSize = VECTOR2(DIGIT_SIZE, DIGIT_SIZE);
		obj->canDraw = true;
		SetDigitDispSize(DigitDispSize);  //数値表示の際の文字サイズを設定
		numberPosX = NUMBER_POS_X;
		numberPosY = NUMBER_POS_Y;
		numberPosX += digitDispSize * DIGIT_MAX;  //  一の位から描画するので右端にずらす
		numberPosX -= digitDispSize * 2;        //  描画桁を左にずらす     
		obj->pos = VECTOR2(numberPosX, numberPosY);
		obj->size = VECTOR2(digitDispSize, digitDispSize);
		stage++;
	case 1:
		SetNumber(money);      //各桁の値を設定
		float  sx = (float)((digit[1] % NUM_DIGIT_1LINE) * DIGIT_SIZE);
		float  sy = (float)((digit[1] / NUM_DIGIT_1LINE) * DIGIT_SIZE);
		obj->texPos = VECTOR2(sx, sy);
		break;
	}
}
void UIListMoney3::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/number.png");
		obj->texPos = VECTOR2(0, 0);
		obj->texSize = VECTOR2(DIGIT_SIZE, DIGIT_SIZE);
		obj->canDraw = true;
		SetDigitDispSize(DigitDispSize);  //数値表示の際の文字サイズを設定
		numberPosX = NUMBER_POS_X;
		numberPosY = NUMBER_POS_Y;
		numberPosX += digitDispSize * DIGIT_MAX;  //  一の位から描画するので右端にずらす
		numberPosX -= digitDispSize * 3;        //  描画桁を左にずらす     
		obj->pos = VECTOR2(numberPosX, numberPosY);
		obj->size = VECTOR2(digitDispSize, digitDispSize);
		stage++;
	case 1:
		SetNumber(money);      //各桁の値を設定
		float  sx = (float)((digit[2] % NUM_DIGIT_1LINE) * DIGIT_SIZE);
		float  sy = (float)((digit[2] / NUM_DIGIT_1LINE) * DIGIT_SIZE);
		obj->texPos = VECTOR2(sx, sy);
		break;
	}
}
void UIListMoney4::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/number.png");
		obj->texPos = VECTOR2(0, 0);
		obj->texSize = VECTOR2(DIGIT_SIZE, DIGIT_SIZE);
		obj->canDraw = true;
		SetDigitDispSize(DigitDispSize);  //数値表示の際の文字サイズを設定
		numberPosX = NUMBER_POS_X;
		numberPosY = NUMBER_POS_Y;
		numberPosX += digitDispSize * DIGIT_MAX;  //  一の位から描画するので右端にずらす
		numberPosX -= digitDispSize * 4;        //  描画桁を左にずらす     
		obj->pos = VECTOR2(numberPosX, numberPosY);
		obj->size = VECTOR2(digitDispSize, digitDispSize);
		stage++;
	case 1:
		SetNumber(money);      //各桁の値を設定
		int  sx = (digit[3] % NUM_DIGIT_1LINE) * DIGIT_SIZE;
		int  sy = (digit[3] / NUM_DIGIT_1LINE) * DIGIT_SIZE;
		obj->texPos = VECTOR2(sx, sy);
		break;
	}
}


void UIListArrowRight::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/arrowRight.png");
		obj->pos = VECTOR2(ARROW_RIGHT_POS_X, ARROW_RIGHT_POS_Y);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(ARROW_SIZE, ARROW_SIZE);
		obj->texSize = VECTOR2(ARROW_TEX_SIZE, ARROW_TEX_SIZE);
		obj->canDraw = false;   //いつでも見える
		stage++;
	case 1:
		obj->canDraw = UISecondary.drawFlag;
		break;
	}
}
void UIListArrowLeft::move(OBJ2D* obj, bool leftFlag, int month, int day, int money)
{
	switch (stage) {
	case 0:
		obj->SetSprite(L"./Data/picture/arrowLeft.png");
		obj->pos = VECTOR2(ARROW_RIGHT_POS_X, ARROW_RIGHT_POS_Y+ ARROW_TEX_SIZE);
		obj->texPos = VECTOR2(0, 0);
		obj->size = VECTOR2(ARROW_SIZE, ARROW_SIZE);
		obj->texSize = VECTOR2(ARROW_TEX_SIZE, ARROW_TEX_SIZE);
		obj->canDraw = false;   //いつでも見える
		stage++;
	case 1:
		obj->canDraw = UISecondary.drawFlag;
	break;
	}
}
// 初期設定
void UIManager::init()
{
	OBJ2DManager::init();
	add(&UIMain);
	add(&UISecondary);
	add(&UIArrowRight);
	add(&UIArrowLeft);
	add(&UIAttribute);
	add(&UIMonth);
	add(&UIDay);
	add(&UIMonthNo);
	add(&UIHelp);
	add(&UIchooseFrame);
	add(&UIMoney1);
	add(&UIMoney10);
	add(&UIMoney100);
	add(&UIMoney1000);
	add(&UIMoneyCharacter);
	
}

void UIManager::Delete() {
	if (!objList.empty()) {
		objList.clear();
	}
}

// プレイヤー生存チェック
bool UIManager::isAllive()
{
	return (!objList.empty());
}

//  表示文字サイズ変更関数
void  UIListMoney1::SetDigitDispSize(int size)
{
	if (size > 0)    digitDispSize = size;
	if (size < 0)    digitDispSize = DIGIT_SIZE;
}

//  数値設定関数         
void  UIListMoney1::SetNumber(int num)
{
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		digit[i] = num % 10;      //  一の位を抽出
		num /= 10;              //  桁を1つシフト
	}
}
