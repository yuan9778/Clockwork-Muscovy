#ifndef BATTLE_H
#define BATTLE_H

using namespace std;
class Battlehelper
{
private:
	int damage;
public:
	bool Checkalive(Character* c);
	string DealDamage(Character* attacker, Character* defender);
	string DealItem(Item* item,Character* attacker,Character* defender);
	string DealSkill(Skill* skill, Character* attacker, Character* defender);
	vector<Position> availableMove(Character*);
	vector<Position> availableAttack(Character*);
	Character* CheckValidAttack(Character* attacker, int x, int y);
	bool CheckValidMove(vector<Position> p, int x, int y);
	
};

class Battle
{
private:
	int userchoice;
	Battlehelper bp;
	int turncounter;
	int whichpicking;
	Character* currentCharacter;
	Character* currentDeadingChar;
	Character* beatenMob;
	Character* beatenPlayer;
	int playercounter;
	int mobcounter;
	int player_death;
	int state;
	double mob_reaction;
	double player_reaction;
	bool mob_turn;
	Position* pick;
	int attack_frame_counter;
	int move_frame_counter;
	int deading_frame_counter;
	bool idle_disable;
	nodeptr path,p,temp;
	int inner_c,step,preRow,preCol,currRow,currCol;
	vector<Position> availablemoves;
	int  temp_cursorX, temp_cursorY;

public:
	Battle();
	nodeptr FindPath(int,int);
	int AttackDistance(int row, int col);
	void BattleLoopUpdate();
	void Highlight(Character*);
	void PrintMobStatus(HDC, Character);
	void PrintHeroStatus(HDC,Character);
	void PrintInventory(HDC hdc,Inventory);
	void PrintSkill(HDC hdc,Skill*);
	void PrintFlee(HDC);
	void PrintAttack(HDC);
	void PlayerAttackMove(HDC);
	void MobAttackMove(HDC);
	void Animation(HDC);
	void PrintBattleStatus(HDC,string);
	void PrintALL(HDC,Inventory,Skill*);
	void InitialPosition();
	Item ShowInventory(Inventory);
	Skill ShowSkill(Character);
};
#endif