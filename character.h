
#ifndef CHARACTER_H
#define CHARACTER_H

using namespace std;

class Position
{
private:
	int x;//pixel
	int y;//pixel
	int row;
	int col;
	int direction;
public:
	int getX();//pixel
	int getY();//pixel
	int getRow();
	int getCol();
	bool equals();
	void setDir(int);
	int getDir();
	//void moveCardinalDirection(int);
	void moveCardinalDirectionPixel(int,int,int);//input is pixel
	void moveCardinalDirection(int,int,int);
};



class Compoent
{
private:
	string name;
	int value;
	int id;
public:
	string getName();
	int getValue();
	int getID();
};



class Item
{
private:
	
	int value;
	int id;
	float weight;
	int usage_type;//1: for attacking 2: for protection 3:equipment
	int usage_value;
	int usage_range;
	Compoent compoent;
	string name;
public:
    
	Item();
    Item(string name,int value, int id, float weight,int usage_type,int usage_value,int usage_range);
	string getName();
	int getValue();
	int getID();
	float getWeight();
	Compoent getCompoent();
	int getCompoentValue();
	int getUsageType();
	int getUsageValue();
	int getUsageRange();
	void setName(string);
};


class Inventory
{
private:
	Item* item[20];
	int nrOfitems;
public:
	Inventory();
	~Inventory();
	int getNrOfitems();
	Item* getItem(Item* n);
	bool addItem(Item*);
	bool removeItem(Item*);
	float getTotalWeight();
	float getTotalValue();
	Item** getItemList();
};
class Skill
{
private:
	string name;
	string description;
	int usage_type;	//1 attacking,  2 restoring
	int amountOfDamage;
	int usage_range;
public:
	Skill();
	Skill(string name, string description, int usage_type, int amountOfDamage, int usage_range);
	string getName();
	string getDescription();
	bool doesDamage();
	int getDamage();
	int getUsageType();
	int getUsageRange();
};

class Major
{

private:
	Skill* skill[20];
	int nrOfskill;
	string name;

	int health;
	int dexterity;
	int endurance;
	int weapon_skill;
	int willpower;
	int base_physical_bonus;
	int base_skill_bonus;
	int physical_resistance;
	int skill_resistance;
	double physical_to_hit;
	double skill_to_hit;
	double defense;
	int level;
	int major_attribute;//low,medium,high

	bool ProvideCover;

public:
	Major();
	Major(string name,int level);
	~Major();
	string getName();
	int getHealth();
	int getDexterity();
	int getWeaponSkill();
	int getWillPower();
	int getEndurance();
	int getBasePhysicalBonus();
	int getBaseSkillBonus();
	int getPhysicalResistance();
	int getSkillResistance();
	double getPhysicalToHit();
	double getSkillToHit();
	double getDefense();

	int low();
	int medium();
	int high();
	void updateHP();
	void updateBasePhysicalBonus();
	void updateBaseSkillBonus();
	void updatePhysicalResistance();
	void updateSkillResistance();
	void updatePhysicalToHit();
	void updateSkillToHit();
	void updateDefense();
	void setHP(int);

	void updateLevelup(int lv, bool silent);

	bool canProvideCover();
	bool addSkill(Skill*);
	bool removeSkill(Skill);
	int getArmorClass();
	Skill** getSkillList();
	Skill* getSkill(Skill*);
	int getNrOfSkill();
};

class Character
{
protected:
	string name;
	int id;
	int level;
	int map;
	int currentHP;
	
	Inventory* inventory;
	Position* position;//absolute pixel/tile in the whole map
	//Position* battleposition;
	Major* major;
	Item* equipWeapon;
	bool Hostile;

	int AttackDamage;
	int MoveRange;
	int AttackRange;
	bool isSelected;
	bool isAttacking;
	bool isTakingCover;
	bool isMoving;
	bool alive;
	HBITMAP currentSprite;
	HBITMAP previousSprites;
	int idle_frame_counter;

public:
    clock_t lastclock;
	Character()	{/*alive=true;*/lastclock=0;}
	Character(string n) : name(n){}
	~Character(){}
	const string getName() {return name;}
	const int getID(){return id;}
	bool isAlive();
	void setAlive(bool);

	Inventory* getInventory();
	Position* getPosition();
	//Position* getBattlePosition();
	Major* getMajor();
	void increIdle_Frame_counter();
	int getIdle_Frame_counter();
	void resetIdle_Frame_counter();
	int getMap();
	void setMap(int map);
	Item* getEquipWeapon();
	void setEquipWeapon(Item* weapon);
	HBITMAP getCurrentSprite();
	HBITMAP getPreviousSprite();
	void setPreviousSprites(HBITMAP n);
	void setCurrentSprite(HBITMAP n);
	int getLevel();
	int getAttackDamage();
	int getAttackRange();
	int getMoveRange();
	void setBattlePosition(int,int);
	bool isHostile();
	bool select(Character);
	bool attack(Character);
	bool takeCover(Position);
	void moveTo(Position);
	int getCurrentHP();
	void setCurrentHP(int n);

};


class Player: public Character
{
private:

	int experience;
	int leveldata[20];
	int age;
	int SP;
	string sex;
	string personality;
	string backstory;


public:
	Player();
	Player(string,int map,int x,int y,bool live);
	~Player();
	string getSex();
	string getPersonality();
	string getBackstroy();
	int getAge();
	int getLevel();
	int getExperience();
	int getSP();
	void addExperience(int);
	bool interact(Character);
};

class NPC: public Character
{
private:

	int fraction;
	int attackRange;

public:
	NPC();
	NPC(string name, bool hostile,int frac,int mapfile,int x, int y,HBITMAP curSp,HBITMAP preSP,bool live);
	~NPC();
	int getFraction();
	int getAttackRange();
	void setAttackRange(int);
	Position* getPosition();
};


int rounded(double n);






#endif

