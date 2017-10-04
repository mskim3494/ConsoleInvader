#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include "console.h"
using namespace std;

char Screen[HEIGHT][WIDTH];

#define MAX_LEVEL 4
int Level = 1;

int TotalScore = 0;
int FrameScore = 0;
int PreviousScore = 0;

// BASIC UNIT CLASS

class Unit{
protected:
	int x, y;
	int LiveFlag;
	int Size;

public:
	Unit() { LiveFlag = Size = 0; }
	~Unit() {}

	void SetX(int iX) { x = iX; }
	void SetY(int iY) { y = iY; }
	void SetLiveFLag(int iLF) { LiveFlag = iLF; }
	void SetSize(int iS = 1) { Size = iS; }

	int GetX() { return x; }
	int GetY() { return y; }
	int GetLiveFlag() { return LiveFlag; }
	int GetSize() { return Size; }

	void IncX() { x++; }
	void IncX(int iInc) { x += iInc; } 
	void IncY() { y++; }
	void IncY(int iInc) { y += iInc; } 
	void DecX() { x--; }
	void DecX(int iDec) { x -= iDec; } 
	void DecY() { y--; }
	void DecY(int iDec) { y -= iDec; } 
	void IncSize() { Size++; }
	void IncSize(int iSize) { Size += iSize; }

	void KillUnit() { LiveFlag = 0; }
};


// ON PLAYER UNITS

class PlayerInfo : public Unit {
private: 
	char* PlayerUnit;
    int Blink;
    int Life;
	
public:
	PlayerInfo(); 
	~PlayerInfo() { if(PlayerUnit) delete[] PlayerUnit; }

	void GetAll(int& rx, int& ry, int& rs, int& rlf) 
	{ rx = x; ry = y; rs = Size; rlf = LiveFlag; }
	char* GetPlayerUnit() { return PlayerUnit; }
    int GetBlink() { return Blink; }
    int GetLife() { return Life; }

	void SetAll(int iX, int iY, int iLF) { x = iX; y = iY; LiveFlag = iLF; }
	void SetPlayerUnit(char* pPU) { if(PlayerUnit) delete[] PlayerUnit; PlayerUnit = strdup(pPU); }
    void SetBlink(int iBlink) { Blink = iBlink; }
    void SetLife(int iLife) { Life = iLife; }
    
    void IncBlink(int iBlink) { Blink += iBlink; }
    void DecBlink(int iBlink) { Blink -= iBlink; }
    void IncBlink() { Blink++; }
    void DecBlink() { Blink--; }
    
    void DecLife() { Life--; }
};

PlayerInfo::PlayerInfo()
{ 
	x = 39; 
	y = 19; 
	LiveFlag = 1; 
	PlayerUnit = strdup("-=^=-"); 
	Size = strlen(PlayerUnit);
    Blink = 0;
}

PlayerInfo Player;

// ON ENEMY UNITS

class EnemyInfo : public Unit{
protected:
	int StartX;
	int MoveFlag;
	char* EnemyUnit;

public:
	EnemyInfo();
	~EnemyInfo() {if(EnemyUnit!= NULL) delete[] EnemyUnit;}

	int GetMoveFlag() { return MoveFlag; }
	int GetStartX() { return StartX; }
	char* GetEnemyUnit() { return EnemyUnit; }
	void GetAll(int& rx, int& ry, int& rsize, int& rlive);

	void SetMoveFlag(int iMF) { MoveFlag = iMF; }
	void Initialize(int iX, int iY, int iMF)
		{ x = StartX = iX; y = iY; LiveFlag = 1; MoveFlag = iMF; }
    void SetEnemyUnit(char* n)
        { if(EnemyUnit!= NULL) delete[] EnemyUnit; EnemyUnit = strdup(n); }

	void IncX() { x++; }
	void IncX(int iInc) { x += iInc; } 
	void IncY() { y++; }
	void IncY(int iInc) { y += iInc; } 
	void DecX() { x--; }
	void DecX(int iDec) { x -= iDec; } 
	void DecY() { y--; }
	void DecY(int iDec) { y -= iDec; } 
	void FlipMoveFlag() { MoveFlag = !MoveFlag; }

	int xDeviation() { return abs(StartX - x); }
};

EnemyInfo::EnemyInfo()
{ 
	x = y = LiveFlag = StartX = MoveFlag = 0; 
	EnemyUnit = strdup("@-@"); 
	Size = strlen(EnemyUnit);
}

void EnemyInfo::GetAll(int& rx, int& ry, int& rsize, int& rlive)
{
	rx = x;
	ry = y;
	rsize = Size;
	rlive = LiveFlag;
}


#define ENEMY_PER_ROW 10
#define MAX_ENEMY 30
EnemyInfo Enemy[MAX_ENEMY];

// BOSS CLASS





// SHOT MANAGER CLASS

#define ENEMY_SHOT			0
#define PLAYER_SHOT			1

class ShotInfo : public Unit {
protected: 
	int Type;
	char Form;

public:
	void SetType(int iType) { Type = iType; }
	void SetAll(int iX, int iY, int iSize, int iType)
		{ x = iX; y = iY; Size = iSize; Type = iType; LiveFlag = 1;}

	int GetType() { return Type; }
	void GetAll(int& rx, int& ry, int& rs, int& rlf, int& rtype) 
		{ rx = x; ry = y; rs = Size; rlf = LiveFlag; rtype = Type; }
}; 

class ShotManager{
protected:
	ShotInfo* Shot;
	int MaxShotCount;
	char EnemyShot;
	char PlayerShot;

public:
    ShotManager();
	ShotManager( int MSC );
	~ShotManager();

	ShotInfo* GetShot() { return Shot; }
	int GetShotCount() { return MaxShotCount; }
	char GetEnemyShot() { return EnemyShot; }
	char GetPlayerShot() { return PlayerShot; }

	int ChangeShotCount(int nSC);
	void CreateShot(int Type, int x, int y);
	void SetEnemyShot(char ES) { EnemyShot = ES; }
	void SetPlayerShot(char PS) { PlayerShot = PS; }
	void ShotAction();
	int ShotCrash(PlayerInfo &pPlayer);
	int ShotCrash(EnemyInfo &eEnemy);
};

ShotManager::ShotManager()
{
    MaxShotCount = 20;
    Shot = new ShotInfo[MaxShotCount];
    EnemyShot = '*';
    PlayerShot = '|';
}

ShotManager::ShotManager(int MSC )
{ 
	MaxShotCount = MSC;
	Shot = new ShotInfo[MSC];
	EnemyShot = '*';
	PlayerShot = '|';
}

ShotManager::~ShotManager()
{
	if(Shot != NULL)
		delete[] Shot;
}

int ShotManager::ChangeShotCount(int nSC)
{
	if(nSC == MaxShotCount)
		return 2; // nothing to change
	if(nSC <= 0)
		return 0; // invalid

	int i, acc = 0;
	ShotInfo* Tmp = new ShotInfo[nSC];

	for(i=0; acc < nSC || i < MaxShotCount; i++){
		if(Shot[i].GetLiveFlag()){
			Tmp[acc++] = Shot[i];
		} 
	}
	
	MaxShotCount = nSC;
    
    if(Shot != NULL)
        delete[] Shot;
    
	Shot = Tmp;

	return 1; // successful
}

void ShotManager::CreateShot(int Type, int x, int y)
{
	int i;
	
	for(i=0; i < MaxShotCount; i++){
		if(Shot[i].GetLiveFlag() == 0){
			Shot[i].SetAll(x, y, 1, Type);
			return;
		}
	}
}

void ShotManager::ShotAction()
{
	int i, y;

	for(i=0; i < MaxShotCount; i++){
		if(Shot[i].GetLiveFlag()){
			// move the shot
            Shot[i].GetType() == PLAYER_SHOT ? Shot[i].DecY() : Shot[i].IncY();
			
            // check for validity
			y = Shot[i].GetY();
			if(y < 0 || y >= HEIGHT){
				Shot[i].KillUnit();
			}
		}
	}
}

int ShotManager::ShotCrash(PlayerInfo &pPlayer)
{
	int i, px, py, psize, plf;
	pPlayer.GetAll(px, py, psize,plf);
	
	if(!plf)
		return 2; // unsuccessful

	for(i=0; i<MaxShotCount; i++){
		if(Shot[i].GetLiveFlag()){
            
            if(Shot[i].GetType() == ENEMY_SHOT){
                if(py == Shot[i].GetY()
                   && Shot[i].GetX() >= px - psize/2
                   && Shot[i].GetX() <= px + psize/2){
                    Shot[i].KillUnit();
                    return 1; // crashed
                }
            }
        }
	}
	
	return 0; // no crash
}

int ShotManager::ShotCrash(EnemyInfo &eEnemy)
{
	int i, ex, ey, esize, elive;
	eEnemy.GetAll(ex, ey, esize, elive);

	if(!elive)
		return 2; // unsuccessful, not alive

	for(i=0; i<MaxShotCount; i++){
		if(Shot[i].GetLiveFlag() && Shot[i].GetType() == PLAYER_SHOT){
            if(Shot[i].GetY() == ey
                && Shot[i].GetX() >= ex - esize/2
				&& Shot[i].GetX() <= ex + esize/2){
                Shot[i].KillUnit();
                eEnemy.KillUnit();	
                return 1; // crashed
             }  
		}
	}
	return 0;
}

ShotManager Shots;

// CLASS INHERITANCE: BOSS SHOT

#define BOSS_SHOT 2

class DerivedShotManager : public ShotManager {
private:
    char BossShot;
    
public:
    DerivedShotManager() : ShotManager(25) { BossShot = 'v';}
    ~DerivedShotManager();
    
    char GetBossShot() { return BossShot; }
    
    void SetBossShot(char nS) { BossShot = nS; }
    
    void BossShotAction();
    int BossShotCrash(PlayerInfo pPlayer);
};

void DerivedShotManager::BossShotAction()
{
    int i, y;
    
    for(i=0; i < MaxShotCount; i++){
        if(Shot[i].GetLiveFlag()){
            // move the shot
            if(Shot[i].GetType() == BOSS_SHOT)
                Shot[i].IncY();
            
            // check for validity
            y = Shot[i].GetY();
            if(y < 0 || y >= HEIGHT){
                Shot[i].KillUnit();
            }
        }
    }
}

int DerivedShotManager::BossShotCrash(PlayerInfo pPlayer)
{
    int i, px, py, psize, plf;
    pPlayer.GetAll(px, py, psize,plf);
    
    if(!plf)
        return 2; // unsuccessful
    
    for(i=0; i<MaxShotCount; i++){
        if(Shot[i].GetLiveFlag()){
            
            if(Shot[i].GetType() == BOSS_SHOT){
                if(py == Shot[i].GetY()
                   && Shot[i].GetX() >= px - psize/2
                   && Shot[i].GetX() <= px + psize/2){
                    Shot[i].KillUnit();
                    return 1; // crashed
                }
            }
        }
    }
    
    return 0; // no crash
}


// BONUS ITEMS CLASS




// FUNCTIONS
void InitialObject();
void PlayerAction();
void EnemyAction();
void EnemyAction2();
void EnemyAction3();
void BossAction();
void ItemAction();
void Draw();
void DrawPlayer();
void DrawEnemy();
void DrawShot();
void DrawStats();
void DrawBoss();
void DrawItems();
void CheckCrash();
int CheckClear();
int CountEnemy();
int CheckGameEnd();


void main()
{
	Initial();
	InitialObject();
	srand((unsigned int)time(NULL));

	while(1){
		// Make changes to the game board
		switch (Level){
			case 1:
				EnemyAction();
				break;
			case 2:
				EnemyAction2();
				break;
			case 3:
				EnemyAction3();
				break;
			case 4:
				BossAction();
				break;
			default:
					break;
		PlayerAction();
		Shots.ShotAction();
		CheckCrash();

		// Draw changes above to the console
		Draw();

		// Check for game over
//		if(CheckGameEnd())
//			break;

		// Create time delay to control for speed
		Sleep(100);
	}

}


void InitialObject()
{
	Player.SetAll(39, 19, 1);

    int i, j, k, xcount;
    xcount = 17;
    k=0;
    for(i=0; i<3; i++){
        for(j=0; j<10; j++) {
            Enemy[k].Initialize(xcount, i, 1);
            k++;
            xcount += 5;
        }
        xcount = 17;
    }
}

void PlayerAction()
{
	// not else if to account for diagonal movement
	if( GetAsyncKeyState(VK_UP) & 0x8000 ){
		if(Player.GetY() >= 0) 
			Player.DecY(1);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		if(Player.GetY() < HEIGHT - 1)
			Player.IncY(1);
	} 
	if (GetAsyncKeyState(VK_LEFT) & 0x8000){
		if(Player.GetX() - Player.GetSize() >= 0)
			Player.DecX(2);
	} 
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000){
		if(Player.GetX() + Player.GetSize() < WIDTH)
			Player.IncX(2);
	}
	if(GetAsyncKeyState(VK_CONTROL) & 0x8000){ 
		Shots.CreateShot(PLAYER_SHOT, Player.GetX(), Player.GetY());
	}
}

void EnemyAction()
{
    int i;
    for(i=0; i < MAX_ENEMY; i++){
        if(Enemy[i].GetLiveFlag()){
            if(Enemy[i].GetMoveFlag()){
                Enemy[i].IncX();
            } else {
                Enemy[i].DecX();
            }
            if( Enemy[i].xDeviation() > 13){
                Enemy[i].IncY();
                Enemy[i].FlipMoveFlag();
            }
            if(rand() % 100 < 2 ){
                Shots.CreateShot(ENEMY_SHOT, Enemy[i].GetX(), Enemy[i].GetY());
            }
        }
    }
}

void EnemyAction2()
{
    // crossed movement by rows
    int i, j;
    for(i=0; i < MAX_ENEMY / ENEMY_PER_ROW; i++){
        for(j=(i*ENEMY_PER_ROW); j< (i*ENEMY_PER_ROW)+ENEMY_PER_ROW; j++){
            if(Enemy[j].GetLiveFlag()){
                if(Enemy[j].GetMoveFlag()){
                    i%2 == 0 ? Enemy[j].IncX() : Enemy[j].DecX();
                } else {
                    i%2 == 1 ? Enemy[j].IncX() : Enemy[j].DecX();
                }
                if( Enemy[j].xDeviation() > 13){
                    Enemy[j].IncY();
                    Enemy[j].FlipMoveFlag();
                }
                if(rand() % 100 < 4 ){
                    Shots.CreateShot(ENEMY_SHOT, Enemy[i].GetX(), Enemy[i].GetY());
                }
            }
        }
    }
}

void EnemyAction3()
{
    int i;
    // diagonal movement
    
    // move lowest row
    for(i= MAX_ENEMY - ENEMY_PER_ROW; i < MAX_ENEMY; i++){
        if(Enemy[i].GetLiveFlag()){
            // horizontal shift
            Enemy[i].GetMoveFlag() ? Enemy[i].IncX(2) : Enemy[i].DecX(2);
            // vertical shift: only move down on the right
            if ( (Enemy[i].GetStartX() - Enemy[i].GetX()) < -13){
                Enemy[i].IncY();
                Enemy[i].FlipMoveFlag();
            }
            if ((Enemy[i].GetStartX() - Enemy[i].GetX()) > 13){
                Enemy[i].FlipMoveFlag();
            }
            if(rand() % 100 < 5 ){
                Shots.CreateShot(ENEMY_SHOT, Enemy[i].GetX(), Enemy[i].GetY());
            }
        }
    }
    // move first and second
    for(i= MAX_ENEMY - ENEMY_PER_ROW - 1; i >= 0; i--){
        if(Enemy[i].GetLiveFlag()){
            // horizontal shift
            // at edges, continue moving
            if( Enemy[i].xDeviation() > 10 || Enemy[i+ENEMY_PER_ROW].GetLiveFlag() == 0){
                Enemy[i].GetMoveFlag() ? Enemy[i].IncX(2) : Enemy[i].DecX(2);
            } if( abs(Enemy[i+ENEMY_PER_ROW].GetX() - Enemy[i].GetX()) > 2){
                // otherwise continue row above
                Enemy[i].GetMoveFlag() ?
                Enemy[i].SetX(Enemy[i+ENEMY_PER_ROW].GetX() - 2) :
                Enemy[i].SetX(Enemy[i+ENEMY_PER_ROW].GetX() + 2);
            }
            // changing direction at edges
            if( Enemy[i].xDeviation() > 13){
                Enemy[i].FlipMoveFlag();
                Enemy[i].IncY();
            }
            
            // create the shot
            if(rand() % 100 < 3 ){
                Shots.CreateShot(ENEMY_SHOT, Enemy[i].GetX(), Enemy[i].GetY());
            }
        }
    }
}

void BossAction()
{
}


void CheckCrash()
{
	int i;

	if(Shots.ShotCrash(Player)){
        Player.DecLife();
	}
	for(i=0; i < MAX_ENEMY; i++){
		Shots.ShotCrash(Enemy[i]);
	}
}


int CheckClear()
{
    int i;

	for(i=0;i < MAX_ENEMY; i++){
		if(Enemy[i].GetLiveFlag())
			return 0;
	}
	return 1;
}

int CheckGameEnd()
{
	if(CheckClear()){
		MoveCursor(33, 10);
		printf("LEVEL %d COMPLETED", Level++);
        if(Level > MAX_LEVEL) {
            MoveCursor(34, 11);
            printf("CONGRATULATIONS");
            MoveCursor(33, 12);
            printf("ALL STAGES CLEAR");
            Sleep(5000);
            return 1;
        }
        InitialObject();
		Sleep(2000);
		return 0;
	}

	if(Player.GetLife() == 0) {
		MoveCursor(34,10);
		printf("GAME OVER");
		Sleep(1000);
		return 1;
	}
	return 0;
}


// ********** DRAWING PART ************
// ************************************
// ************************************

void Draw()
{
	int i;
	for(i=0; i<HEIGHT; i++){
		memset(Screen[i], ' ', WIDTH);
		Screen[i][WIDTH-1] = NULL;
	}

	DrawPlayer();
	Level == MAX_LEVEL ? DrawBoss() : DrawEnemy();
	DrawShot();
    DrawStats();

	for(i=0;i<HEIGHT; i++){
		MoveCursor(0,i);
		printf(Screen[i]);
	}
}

void DrawPlayer()
{
	int i;
	int x = Player.GetX() - Player.GetSize() / 2;
	int y = Player.GetY();

	if( y < 0 || y >= HEIGHT) 
		return;

	char* Tmp = Player.GetPlayerUnit();
	for(i=0; i<Player.GetSize(); i++){
		if(x >= 0 && x < WIDTH){
			Screen[y][x++] = Tmp[i];
		}
	}
}

void DrawEnemy()
{
	int i, j, x, y;
	char* Tmp;

	for(i=0; i< MAX_ENEMY ; i++){
		x = Enemy[i].GetX() - Enemy[i].GetSize() / 2;
		y = Enemy[i].GetY();

		if(y < 0 || y >= HEIGHT)
			return;

		Tmp = Enemy[i].GetEnemyUnit();
		for(j=0; j< Enemy[i].GetSize(); j++){
			if(x >= 0 && x < WIDTH){
				if(Enemy[i].GetLiveFlag()){
					Screen[y][x] = Tmp[j];
				}
			} 
			x++;
		}
	}
}

void DrawShot()
{
	int i, x, y;
	int SC = Shots.GetShotCount();
	ShotInfo* Tmp = Shots.GetShot();

	for(i=0; i < SC ; i++){
		if(Tmp[i].GetLiveFlag()){
			x = Tmp[i].GetX();
			y = Tmp[i].GetY();
            
            switch(Tmp[i].GetType()){
                case PLAYER_SHOT:
                    Screen[y][x] = Shots.GetPlayerShot();
                    break;
                case ENEMY_SHOT:
                    Screen[y][x] = Shots.GetEnemyShot();
                    break;
                default:
                    break;
            }
		}
	}
}

void DrawBoss()
{
}

int CountEnemy()
{
    int i, acc;
    acc = 0;
    for(i=0; i < MAX_ENEMY; i++){
        if(Enemy[i].GetLiveFlag()){
            acc++;
        }
    }
    return acc;
}

void DrawStats()
{
    int EnemyCount = CountEnemy();
    
    FrameScore = (MAX_ENEMY - EnemyCount) * 100 - (3 - Player.GetLife()) * 400;
    
    if(PreviousScore != FrameScore){
        TotalScore += FrameScore - PreviousScore;
    }
    PreviousScore = FrameScore;
    
    TotalScore < 0 ? TotalScore = 0 : TotalScore = TotalScore;
    
    sprintf(Screen[HEIGHT-1],
            "        LEVEL: %d      LIVES: %d    ENEMIES LEFT: %d       SCORE: %d   ",
            Level, Player.GetLife(), EnemyCount, TotalScore);
}
