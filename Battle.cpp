#include <Windows.h>
#include <vector>
#include <string>

#include<ctime>
#include "character.h"
#include "pqastar.h"
#include "Battle.h"

#include "astar.h"
#include <queue>
#include "timer.h"
#include "game.h"



//========battle.cpp shared global variables definitions start==============//
int itemindex;
int skillindex;
int currentPlayer;
vector<int> mobs;
int n_mobs;
//========battle.cpp shared global variables definitions end==============//


//====battle.cpp dedicated global variable definition===========//
HRGN bgRgn;
HBRUSH hBrush;
RECT textRect; 

string int_string(int n)
{
	string r;
	char buffer[1024];
	sprintf_s(buffer,"%d",n);
	r=buffer;
	return r;
}
bool Battlehelper::Checkalive(Character* c)
{
	bool alive;
	if(c->getCurrentHP()>0)
		alive=true;
	else
		alive=false;
	return alive;
}
string Battlehelper::DealDamage(Character* attacker, Character* defender)
{
	string r;
	double attackSuccess=attacker->getMajor()->getPhysicalToHit() - defender->getMajor()->getDefense();
	if(attackSuccess < 0)
		attackSuccess = 0;
	int totalDamage = attacker->getMajor()->getBasePhysicalBonus() + attacker->getEquipWeapon()->getUsageValue()
						- defender->getMajor()->getPhysicalResistance();
	if(totalDamage<0)
		totalDamage=0;
	if(rand()/(double)RAND_MAX < attackSuccess)
	{
		defender->setCurrentHP((defender->getCurrentHP()-totalDamage > 0) ? (defender->getCurrentHP() - totalDamage) : 0);
		r = attacker->getName() + " attacks " + defender->getName() + " "+ int_string(totalDamage) + "points HP.";
	}
	else
		r="Attacking miss!";
	if(defender->getCurrentHP()<=0)
		defender->setAlive(false);

	return r;
}


string Battlehelper::DealItem(Item* item, Character* attacker, Character* defender)
{
	string r;

	/*
	if(item==NULL)
		r="Nothing is chosen.";*/
	if(item->getUsageType()==1)//attacking type
	{
		defender->setCurrentHP(defender->getCurrentHP()-item->getUsageValue());
		attacker->getInventory()->removeItem(item);
		r="Item: "+item->getName()+"damages "+defender->getName()+int_string(item->getUsageValue())+" points HP.";
	}
	else if(item->getUsageType()==2)//restoring health type
	{
		if(attacker->getCurrentHP()+item->getUsageValue()>attacker->getMajor()->getHealth())
		    attacker->setCurrentHP(attacker->getMajor()->getHealth());
		else
		attacker->setCurrentHP(attacker->getCurrentHP()+item->getUsageValue());
		attacker->getInventory()->removeItem(item);
		r=attacker->getName()+" uses Item: "+item->getName()+" restoring "+int_string(item->getUsageValue())+" points HP.";
	}
	else
		r="The item can not be used here!";

	return r;
}

string Battlehelper::DealSkill(Skill* skill, Character* attacker, Character* defender)
{
	string r;
	/*if(skill==NULL)
		r="Nothing is chosen.";*/
	if(skill->getUsageType()==1)//attacking type
	{
		defender->setCurrentHP(defender->getCurrentHP()-skill->getDamage()>0?defender->getCurrentHP()-skill->getDamage():0);
		r=attacker->getName()+" uses skill "+skill->getName()+" damaging "+defender->getName()+" by "+int_string(skill->getDamage())+" points HP.";
	}
	else if(skill->getUsageType()==2)//heal type
	{
		if(attacker->getCurrentHP()+skill->getDamage()>attacker->getMajor()->getHealth())
		    attacker->setCurrentHP(attacker->getMajor()->getHealth());
		else
		attacker->setCurrentHP(attacker->getCurrentHP()+skill->getDamage());
		r=attacker->getName()+" uses skill "+skill->getName()+" restoring "+attacker->getName()+" by "+int_string(skill->getDamage())+" points HP.";
	}
	else
	{
		//under construction
	}
	return r;
}


vector<Position> Battlehelper::availableMove(Character* c)
{
	vector<Position> r;
	int x=c->getPosition()->getRow();
	int y=c->getPosition()->getCol();
	for(int i=0;i<c->getMoveRange();i++)
	{
		for(int j=1;j<=c->getMoveRange()-i;j++)
		{
			if(x+i<20&&y+j<20)//map edge verification
			{
				if(map[x+i][y+j]!='w'&&map[x+i][y+j]!='#'&&map[x+i][y+j]!='o')
				{
					bool occupied=false;
					for(int k=0;k < static_cast<int>(sprites.size()); k++)
					{
						if(sprites[k]->getPosition()->getRow()==x+i&&sprites[k]->getPosition()->getCol()==y+j)
						{
							occupied=true;
							break;
						}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+i,y+j, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x+j<20&&y-i>0)//map edge verification
			{
				if(map[x+j][y-i]!='w'&&map[x+j][y-i]!='#'&&map[x+j][y-i]!='o')
				{
					bool occupied=false;
					for(int k = 0;k < static_cast<int>(sprites.size()); k++)
					{
						if(sprites[k]->getPosition()->getRow()==x+j&&sprites[k]->getPosition()->getCol()==y-i)
						{
							occupied=true;
							break;
						}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+j,y-i, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x-i>0&&y-j>0)//map edge verification
			{
				if(map[x-i][y-j]!='w'&&map[x-i][y-j]!='#'&&map[x-i][y-j]!='o')
				{
					bool occupied=false;
					for(int k = 0; k < static_cast<int>(sprites.size()); k++)
					{
						if(sprites[k]->getPosition()->getRow()==x-i&&sprites[k]->getPosition()->getCol()==y-j)
						{
							occupied=true;
							break;
						}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x-i,y-j, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x+j<20&&y-i>0)//map edge verification
			{
				if(map[x+j][y-i]!='w'&&map[x+j][y-i]!='#'&&map[x+j][y-i]!='o')
				{
					bool occupied=false;
					for(int k = 0; k < static_cast<int>(sprites.size()); k++)
					{
						if(sprites[k]->getPosition()->getRow()==x+j&&sprites[k]->getPosition()->getCol()==y-i)
						{
							occupied=true;
							break;
						}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+j,y-i, c->getMap());
						r.push_back(p);
					}
				}
			}

		}
	}

	return r;
}

vector<Position> Battlehelper::availableAttack(Character* c)
{


	vector<Position> r;
	int x=c->getPosition()->getRow();
	int y=c->getPosition()->getCol();
	for(int i=0;i<c->getEquipWeapon()->getUsageRange();i++)
	{
		for(int j=1;j<=c->getEquipWeapon()->getUsageRange()-i;j++)
		{
			if(x+i<20&&y+j<20)//map edge verification
			{
				if(map[x+i][y+j]!='w'&&map[x+i][y+j]!='#'&&map[x+i][y+j]!='o')
				{
					bool occupied=false;
					for(int k = 0; k < static_cast<int>(sprites.size()); k++)
					{
						if(sprites[k]->getPosition()->getRow()==x+i&&sprites[k]->getPosition()->getCol()==y+j)
						{
							if(!sprites[k]->isHostile())
							{
								occupied=true;
								break;
							}
						}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+i,y+j, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x+j<20&&y-i>0)//map edge verification
			{
				if(map[x+j][y-i]!='w'&&map[x+j][y-i]!='#'&&map[x+j][y-i]!='o')
				{
					bool occupied=false;
					for(int k = 0; k < static_cast<int>(sprites.size()); k++)
					{
							if(!sprites[k]->isHostile())
							{
								occupied=true;
								break;
							}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+j,y-i, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x-i>0&&y-j>0)//map edge verification
			{
				if(map[x-i][y-j]!='w'&&map[x-i][y-j]!='#'&&map[x-i][y-j]!='o')
				{
					bool occupied=false;
					for(int k = 0;k < static_cast<int>(sprites.size()); k++)
					{
							if(!sprites[k]->isHostile())
							{
								occupied=true;
								break;
							}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x-i,y-j, c->getMap());
						r.push_back(p);
					}
				}
			}

			if(x+j<20&&y-i>0)//map edge verification
			{
				if(map[x+j][y-i]!='w'&&map[x+j][y-i]!='#'&&map[x+j][y-i]!='o')
				{
					bool occupied=false;
					for(int k = 0; k < static_cast<int>(sprites.size()); k++)
					{
							if(!sprites[k]->isHostile())
							{
								occupied=true;
								break;
							}
					}

					if(!occupied)
					{
						Position p;
						p.moveCardinalDirection(x+j,y-i, c->getMap());
						r.push_back(p);
					}
				}
			}

		}
	}

	return r;

}
Character* Battlehelper::CheckValidAttack(Character* attacker, int row, int col)
{
	for(int i=0;i<n_mobs;i++)
	{
		if(sprites[mobs[i]]->getPosition()->getRow()==row && sprites[mobs[i]]->getPosition()->getCol()==col)
		{
			if(sprites[mobs[i]]->isAlive())
				return sprites[mobs[i]];
		}
	}

	return NULL;


}

bool Battlehelper::CheckValidMove(vector<Position> p, int row, int col)
{
	for(int i = 0; i < static_cast<int>(p.size()); i++)
	{
		if(p[i].getRow()==row &&p[i].getCol()==col)
			return true;
	}

	return false;
}


void Battle::InitialPosition()
{
	/*
	int n;//for minimum possible moves
	n=0;
	while(n<3)
	{
		int radius=1;
		if(map[sprites[0]->getPosition()->getRow()+radius][sprites[0]->getPosition()->getCol()]!="w"
			&&map[sprites[0]->getPosition()->getRow()+radius][sprites[0]->getPosition()->getCol()]!="#"
			&&map[sprites[0]->getPosition()->getRow()+radius][sprites[0]->getPosition()->getCol()]!="o")

	}*/


	/* just hard coded the other player's positions here */
	sprites[1]->getPosition()->moveCardinalDirection(sprites[0]->getPosition()->getRow()+1,sprites[0]->getPosition()->getCol()-1,sprites[1]->getMap());
	sprites[2]->getPosition()->moveCardinalDirection(sprites[0]->getPosition()->getRow()-1,sprites[0]->getPosition()->getCol()-1,sprites[2]->getMap());
	sprites[3]->getPosition()->moveCardinalDirection(sprites[0]->getPosition()->getRow()-2,sprites[0]->getPosition()->getCol(),sprites[3]->getMap());
}
Battle::Battle()
{
	turncounter=0;
	whichpicking=0;
	currentCharacter=NULL;
	int playercounter=0;
	int mobcounter=0;
	state=1;
	pick=NULL;
	inner_c=0;
	step=4;
	player_death=0;
	availablemoves.clear();
	deading_frame_counter=0;
	move_frame_counter=0;
	attack_frame_counter=0;
	temp_cursorX=sprites[0]->getPosition()->getX();
	temp_cursorY=sprites[0]->getPosition()->getY();
	cursorX=temp_cursorX;
	cursorY=temp_cursorY;
}


void Battle::BattleLoopUpdate()
{
	switch(state)
	{
		case 1:
			{
				InitialPosition();//put 4 character to the position within 1 tile radius
				mob_reaction=0;
				player_reaction=0;
				mob_turn=false;
				state=2;
				break;
			}



		case 2:
			{

				turncounter=0;
				for(int i=0;i<n_mobs;i++)
				{
					if(!bp.Checkalive(sprites[mobs[i]]))// check the mob if it is dead  
					{	
						n_mobs--;
						mobs.erase(mobs.begin()+i);
					}
					else
					{
						mob_reaction+=sprites[mobs[i]]->getMajor()->getDexterity();
					}
				}
	
	
					if(n_mobs<=0)
					{
						mode=1;//victory return exploring mode

								//need calculate experience function
						return;
					}

					for(int i=0;i<4;i++)
					{
						if(!bp.Checkalive(sprites[i]))
							player_death++;
						else
							player_reaction+=sprites[i]->getMajor()->getDexterity();
					}

					if(player_death==4)
					{
						mode=4;//game over
						return;
					}

					mob_reaction=mob_reaction/n_mobs;
					player_reaction=player_reaction/4-player_death;



				if(player_reaction<mob_reaction)
				{
					mob_turn=true;
					mobcounter=0;
					state=3;
				}
				else
				{
					mob_turn=false;
					playercounter=0;
					state=4;
				}
					break;
			}

		case 3://player party's turn
		{
		mob_turn=false;
		turncounter++;
		if(turncounter<=2) //if two parties have not finished
		{
			if(playercounter<4-player_death)//if player party has not finished
			{
				currentCharacter=sprites[playercounter];
				currentPlayer=playercounter;
				//PrintStatus(currentCharacter);//call user interface function display;

  			for(int i=0;i<sprites[currentPlayer]->getInventory()->getNrOfitems();i++)
			{
				LRESULT pos=SendMessage(hwndItemListBox,CB_ADDSTRING ,0,(LPARAM)(sprites[currentPlayer]->getInventory()->getItemList()[i]->getName().c_str()));  
				SendMessage(hwndItemListBox, CB_SETITEMDATA ,pos,(LPARAM)i);
			}
  

			for(int i=0;i<sprites[currentPlayer]->getMajor()->getNrOfSkill() ;i++)
			{
				LRESULT pos=SendMessage(hwndSkillDropBox,CB_ADDSTRING ,0,(LPARAM)(sprites[currentPlayer]->getMajor()->getSkillList()[i]->getName().c_str()));
				SendMessage(hwndSkillDropBox, CB_SETITEMDATA ,pos,(LPARAM)i);
			}


				state=5;//goto pick choice state
			}
			else
				state=4;//player party has finished this round goto mob party
		}
		else
			state=2;//if two parties have finished then goto initialize new round state

		break;
		}

		case 4: //mob party's turn
			{
				mob_turn=true;
				turncounter++;
				if(turncounter<=2)//if two parties have not finished
				{
					if(mobcounter<n_mobs)//if mob party has not finished
					{
						currentCharacter=sprites[mobs[mobcounter]];
						//PrintStatus(currentCharacter);
						state=14;//mob action
					}
					else
						state=3;//mob party has finished this round goto player party
				}
				else
					state=2;//if two parties have finished then goto initialize new round state
				break;
			}
		case 5:
		{
			userchoice=0;
			state=50;
		}

		case 50:
			{
				if(userchoice!=0)
				{
					state=6;
				}
			break;
			}
		case 6:
			{
						switch(userchoice)
						{
						case 0://empty slot
						break;
						case 1://attack
							{
								//shadeAvailable(bp.availableAttack(currentCharacter));
								whichpicking=1;//attack picking
								state=7;
								break;
							}

						case 2://move
							{
							availablemoves=bp.availableMove(currentCharacter);
							//shadeAvailable(availablemoves);
							whichpicking=2;//move picking
							state=7;
							break;
							}
						case 3://item
							{
								//......//
								//itemindex is global intger for item combobox choosen
								bp.DealItem((currentCharacter->getInventory()->getItemList())[itemindex],currentCharacter,sprites[5]);
								//state=xx; using item animation
								state=3;
								break;
							}
						case 4: //skill
							{
								//......//
								//state=xx; using skill animation
								bp.DealSkill((currentCharacter->getMajor()->getSkillList())[skillindex],currentCharacter,sprites[5]);
								state=3;
								break;
							}
						default:
							break;
						}
			}


		case 7:
			{
				if(cursorX != temp_cursorX || cursorY!= temp_cursorY)
				{
					pick=new Position;
					pick->moveCardinalDirectionPixel(cursorX,cursorY,currentCharacter->getMap());
				}
				else
					pick=NULL;

				if(pick!=NULL)
				{
				if(whichpicking==1) //attack picking
					state=8;
				else if(whichpicking==2)//move picking
					state=9;
				//else if(whichpicking==3)//item(attacking) picking
					//state=
				else
					MessageBox(NULL,"Should not be here (waiting picking)!","Fail",MB_ICONEXCLAMATION | MB_OK);

				}
				break;
			}

		case 8://validate attack
						{
								whichpicking=0;
								beatenMob=NULL;
								whichpicking=0;
								beatenMob=bp.CheckValidAttack(currentCharacter,pick->getRow(),pick->getCol());
								//delete pick;

								if(beatenMob!=NULL)
								{
									state=10;//attacking animation
									attack_frame_counter=0;
								}
								else
									{
									 MessageBox(NULL,"You can't attack there!","Fail",MB_ICONEXCLAMATION | MB_OK);
									 state=7;//go back to picking
									 delete pick;
									 pick=NULL;
									 whichpicking=1;
									}
								
								break;
						
						}


			case 9://validate move:
			{
			bool flag=false;
			whichpicking=0;
			flag=bp.CheckValidMove(availablemoves,pick->getRow(),pick->getCol());
			if(flag==false)
				{
				MessageBox(NULL,"You can't move there!","Fail",MB_ICONEXCLAMATION | MB_OK);
				state=7;
				delete pick;
				pick=NULL;
				whichpicking=2;
				}
			else
				{
				availablemoves.clear();
				state=11; //move animation+A*
				}	
						
			break;
			}

			case 10://attack animation frame control: we only need one time animation
				{
					idle_disable=true;
					if(attack_frame_counter<6)
					{
						currentCharacter->setCurrentSprite(player_attacking[currentCharacter->getPosition()->getDir()][attack_frame_counter]);
						attack_frame_counter++;
					}
					else
					{
						//PrintBattleStatus(hdc,bp.DealDamage(currentCharacter,beatenMob));
						
						if(!bp.Checkalive(beatenMob))//mob is dead
						{
							attack_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//
							delete pick;
							pick=NULL;
							state=13;//deading animation;
							deading_frame_counter=0;
							currentDeadingChar=beatenMob;
							beatenMob=NULL;
						}
						else
						{
							beatenMob=NULL;
							attack_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//
							delete pick;
							pick=NULL;
							if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
						}
					}
					break;
				}

			case 11: //call a* function to find a path
				{
					FindPath(pick->getRow(),pick->getCol());
					if(path==NULL)
					{
						MessageBox(NULL,"Should not be empty path of battle astar","Fail",MB_ICONEXCLAMATION | MB_OK);
						exit(1);
					}
					state=12;
					break;
				}

			case 12:// move animation control and update move x,y coordinates
				{
					idle_disable=true;
					if(move_frame_counter<6)
					{
						currentCharacter->setCurrentSprite(player_moving[currentCharacter->getPosition()->getDir()][move_frame_counter]);
						move_frame_counter++;
					}
					else
						move_frame_counter=0;

					//decompose the tile coordinates into 1/4 pixel coordinates
						int Rd,Cd;
						if(p != NULL)
						{
								  if(inner_c%step == 0)
									{
										  inner_c = 0;
										  preRow = p->info.y;
										  preCol = p->info.x;
										  temp=p;
										  p = p->next;
	  
										  currRow = p->info.y;
										  currCol = p->info.x;
	  
										  Rd = (currRow - preRow)*40/step;
										  Cd = (currCol - preCol)*40/step;
	  
										  delete temp;
										  temp=NULL;
									}
								  currentCharacter->getPosition()->moveCardinalDirectionPixel(currentCharacter->getPosition()->getX() + Cd, currentCharacter->getPosition()->getY() + Rd,currentCharacter->getMap());
								  inner_c++;

						}
						else
						{
							delete pick;
							pick=NULL;
							move_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//
							if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
						}
					break;
				}

			case 13: // deading animation
				{
					idle_disable=true;
					if(deading_frame_counter<6)
					{
						currentDeadingChar->setCurrentSprite(mob_dead[currentCharacter->getPosition()->getDir()][deading_frame_counter]);
						deading_frame_counter++;
					}
					else
					{
						idle_disable=false;
						deading_frame_counter=0;
						currentDeadingChar=NULL;

						if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
					}

					break;
				}


			case 14://mob action
				{
					
					if(currentCharacter->getCurrentHP()/(float)currentCharacter->getMajor()->getHealth()>0.33)
					{
						Character* target=NULL;
						int i=0;
						//choose the first alive player
						while(target==NULL)
						{
							if(sprites[i]->isAlive())
								target=sprites[i];
							i++;
						}


						for(int i=0;i<4;i++)	//choose the loweset hp "alive" player to attack
						{
							if(sprites[i]->getCurrentHP()<target->getCurrentHP())
							{
								if(sprites[i]->isAlive())
									target=sprites[i];
							}
						}

						//if not in the attacking range then move
						if((currentCharacter->getAttackRange())<AttackDistance(target->getPosition()->getRow(),target->getPosition()->getCol()))
							{
								availablemoves.clear();
								availablemoves=bp.availableMove(currentCharacter);
								double close=10000;
								int index;
								for(int i = 0; i < static_cast<int>(availablemoves.size()); i++)
								{
									double temp=sqrt(pow((availablemoves[i].getCol()-target->getPosition()->getCol()),2.0)+pow((availablemoves[i].getRow()-target->getPosition()->getRow()),2.0));
									if(close>temp)
									{
										close=temp;
										index=i;
									}
								}
								FindPath(availablemoves[index].getRow(),availablemoves[index].getCol());
								if(path==NULL)
								{
									MessageBox(NULL,"Should not be empty path of battle astar","Fail",MB_ICONEXCLAMATION | MB_OK);
									exit(1);
								}
								state=15;
							}
						else
							{
								beatenPlayer=target;
								state=16;
							}
					}
					else //if mob hp is lower than 1/3
					{

								Character* target=NULL;
						int i=0;
						//choose the first alive player
						while(target==NULL)
						{
							if(sprites[i]->isAlive())
								target=sprites[i];
							i++;
						}

						int temp=target->getAttackDamage();
						int tempindex=i;
						for(int i=0;i<4;i++)	//choose the loweset hp "alive" player to attack
						{
							if(sprites[i]->getAttackDamage()>temp)
							{
								if(sprites[i]->isAlive())
								{
									temp=sprites[i]->getAttackDamage();
									tempindex=i;
								}
							}
						}
						target=sprites[i];

								availablemoves.clear();
								availablemoves=bp.availableMove(currentCharacter);
								double close=0;
								int index;

								for(int i = 0; i < static_cast<int>(availablemoves.size()); i++)
								{



									double temp=sqrt(pow((availablemoves[i].getCol()-target->getPosition()->getCol()),2.0)+pow((availablemoves[i].getRow()-target->getPosition()->getRow()),2.0));
									if(close<temp)
									{
										close=temp;
										index=i;
									}
								}
								FindPath(availablemoves[index].getRow(),availablemoves[index].getCol());
								if(path==NULL)
								{
									MessageBox(NULL,"Should not be empty path of battle astar","Fail",MB_ICONEXCLAMATION | MB_OK);
									exit(1);
								}
								state=15;


					}

					break;
				}
			case 15:// move animation control and update move x,y coordinates
				{
					idle_disable=true;
					if(move_frame_counter<6)
					{
						currentCharacter->setCurrentSprite(mob_moving[currentCharacter->getPosition()->getDir()][move_frame_counter]);
						move_frame_counter++;
					}
					else
						move_frame_counter=0;

					//decompose the tile coordinates into 1/4 pixel coordinates
						int Rd,Cd;
						if(p != NULL)
						{
								  if(inner_c%step == 0)
									{
										  inner_c = 0;
										  preRow = p->info.y;
										  preCol = p->info.x;
										  temp=p;
										  p = p->next;
	  
										  currRow = p->info.y;
										  currCol = p->info.x;
	  
										  Rd = (currRow - preRow)*40/step;
										  Cd = (currCol - preCol)*40/step;
	  
										  delete temp;
										  temp=NULL;
									}
								  currentCharacter->getPosition()->moveCardinalDirectionPixel(currentCharacter->getPosition()->getX() + Cd, currentCharacter->getPosition()->getY() + Rd,currentCharacter->getMap());
								  inner_c++;

						}
						else
						{
							move_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//
							if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
						}
					break;
				}



			case 16://attack animation frame control: we only need one time animation
				{
					idle_disable=true;
					if(attack_frame_counter<6)
					{
						currentCharacter->setCurrentSprite(mob_attacking[currentCharacter->getPosition()->getDir()][attack_frame_counter]);
						attack_frame_counter++;
					}
					else
					{
						//PrintBattleStatus(hdc,bp.DealDamage(currentCharacter,beatenPlayer));
						if(!bp.Checkalive(beatenPlayer))//Player is dead
						{
							attack_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//;
							state=17;//deading player animation;
							deading_frame_counter=0;
							currentDeadingChar=beatenPlayer;
						}
						else
						{
							attack_frame_counter=0;//resume animation frame to the starting frame
							idle_disable=false;//
							if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
						}
					}
					break;
				}

			case 17: // deading animation
				{
					idle_disable=true;
					if(deading_frame_counter<6)
					{
						currentDeadingChar->setCurrentSprite(player_dead[currentDeadingChar->getPosition()->getDir()][deading_frame_counter]);
						deading_frame_counter++;
					}
					else
					{
						idle_disable=false;
						deading_frame_counter=0;
						currentDeadingChar=NULL;

						if(mob_turn)
							{
								mobcounter++;//change to the next mob of mob party
								state=4;//go back to the mob party turn control
							}
							else
							{
								playercounter++;//change to the next player of our party
								state=3;//go back to player party turn control
							}
					}

					break;
				}






	}
				


	// update the rest of character's idle animation frame

	for(int i=0;i<n_mobs;i++)
	{
	if(!bp.Checkalive(sprites[mobs[i]]))// check the mob if it is dead  
	{
		if(sprites[mobs[i]]!=currentDeadingChar)
			sprites[mobs[i]]->setCurrentSprite(mob_dead[currentDeadingChar->getPosition()->getDir()][6]);//always last frame of deading
	}
	else
	{


		if(sprites[mobs[i]]==currentCharacter)//? can we compare two pointer?
		{
			if(idle_disable!=true)//idle
			{
				if(currentCharacter->getIdle_Frame_counter()<6)
				{
					currentCharacter->setCurrentSprite(player_stopping[currentDeadingChar->getPosition()->getDir()][currentCharacter->getIdle_Frame_counter()]);
					currentCharacter->increIdle_Frame_counter();
				}
				else
					currentCharacter->resetIdle_Frame_counter();
			}

		}
		else
		{
				if(sprites[mobs[i]]->getIdle_Frame_counter()<6)
				{
					sprites[mobs[i]]->setCurrentSprite(player_stopping[currentDeadingChar->getPosition()->getDir()][sprites[mobs[i]]->getIdle_Frame_counter()]);
					sprites[mobs[i]]->increIdle_Frame_counter();
				}
				else
					sprites[mobs[i]]->resetIdle_Frame_counter();
		}
	 }
	}



			for(int i=0;i<4;i++)
			{
				if(!bp.Checkalive(sprites[i]))
				{
				if(sprites[i]!=currentDeadingChar)
					sprites[i]->setCurrentSprite(player_dead[currentDeadingChar->getPosition()->getDir()][6]);//always last frame of deading
				}
				else
				{
					if(sprites[i]==currentCharacter)//? can we compare two pointer?
					{
						if(idle_disable!=true)//idle
						{
									if(currentCharacter->getIdle_Frame_counter()<6)
									{
										currentCharacter->setCurrentSprite(player_stopping[currentDeadingChar->getPosition()->getDir()][currentCharacter->getIdle_Frame_counter()]);
										currentCharacter->increIdle_Frame_counter();
									}
									else
										currentCharacter->resetIdle_Frame_counter();
						}
					}
					else
					{
						if(sprites[i]->getIdle_Frame_counter()<6)
						{
							sprites[i]->setCurrentSprite(player_stopping[sprites[i]->getPosition()->getDir()][sprites[i]->getIdle_Frame_counter()]);
							sprites[i]->increIdle_Frame_counter();
						}
						else
							sprites[i]->resetIdle_Frame_counter();
					}
				}
			}


}


nodeptr Battle::FindPath(int row, int col)
{

	int startX, startY, endX, endY;

  
	if (currentCharacter->getPosition()->getCol()-currentCharacter->getMoveRange() < 0) startX = 0;
	else startX = currentCharacter->getPosition()->getCol()-currentCharacter->getMoveRange();
	if (currentCharacter->getPosition()->getRow()-currentCharacter->getMoveRange() < 0) startY = 0;
	else startY = currentCharacter->getPosition()->getRow()-currentCharacter->getMoveRange();
	if (currentCharacter->getPosition()->getCol()+currentCharacter->getMoveRange() > 8) endX = 8;
	else endX = currentCharacter->getPosition()->getCol()+currentCharacter->getMoveRange();
	if (currentCharacter->getPosition()->getRow()+currentCharacter->getMoveRange() > 7) endY = 7;
	else endY = currentCharacter->getPosition()->getRow()+currentCharacter->getMoveRange();
  

  char** screen = new char*[endY - startY];
  
  for(int i=0;i<endY - startY;i++)
    {
      screen[i]=new char[endX - startX];
    }
  
  for (int i=0; i<endY - startY; i++)
    {
      for(int j=0; j<endX - startX; j++)
		{
		screen[i][j] = map[i+startY][j+startX];
		}
    }

  
  path = astar1_search(screen, endY - startY, endX - startX,row, col,currentCharacter->getPosition()->getRow(), currentCharacter->getPosition()->getCol());
  p=path;
  for(int i=0;i<endY - startY;i++)
    {
      delete []screen[i];
    }
  delete []screen;
  return p;

}

int Battle::AttackDistance(int row, int col)
{

	int startX, startY, endX, endY;

  
	if (currentCharacter->getPosition()->getCol()-10 < 0) startX = 0;
	else startX = currentCharacter->getPosition()->getCol()-10;
	if (currentCharacter->getPosition()->getRow()-10 < 0) startY = 0;
	else startY = currentCharacter->getPosition()->getRow()-10;
	if (currentCharacter->getPosition()->getCol()+10 > 8) endX = 8;
	else endX = currentCharacter->getPosition()->getCol()+10;
	if (currentCharacter->getPosition()->getRow()+10 > 7) endY = 7;
	else endY = currentCharacter->getPosition()->getRow()+10;
  

  char** screen = new char*[endY - startY];
  
  for(int i=0;i<endY - startY;i++)
    {
      screen[i]=new char[endX - startX];
    }
  
  for (int i=0; i<endY - startY; i++)
    {
      for(int j=0; j<endX - startX; j++)
		{
		screen[i][j] = map[i+startY][j+startX];
		}
    }

  
  path = astar1_search(screen, endY - startY, endX - startX,row, col,currentCharacter->getPosition()->getRow(), currentCharacter->getPosition()->getCol());
  int n=0;
  while(path!=NULL)
  {
	  n++;
	  temp=path;
	  delete temp;
	  temp=NULL;
	  p=p->next;
  }

  for(int i=0;i<endY - startY;i++)
    {
      delete []screen[i];
    }
  delete []screen;
  return n;
}
/*
void Battle::PrintALL(HDC hdc ,Inventory i,Skill* skill)
{
    	PrintAttack(hdc);
	PrintFlee(hdc);
    	PrintMobStatus(hdc,mob);
	PrintHeroStatus(hdc,hero);
	PrintInventory(hdc,hero.getInventory());
	PrintSkill(hdc,hero.getMajor().getSkillList());
	

}
void Battle::PrintMobStatus(HDC hdc,Character mob)
{
    char buffer[128];
    sprintf_s(buffer, "Name: %s", mob.getName().c_str());
    SetRect (&textRect, 859, 21, 1000, 43);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Health: %d", mob.getHealth());
    SetRect (&textRect, 859, 58, 1000, 80);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Strength: %d", mob.getStrength());
    SetRect (&textRect, 859, 95, 1000, 117);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Dexterity: %d", mob.getDexterity());
    SetRect (&textRect, 859, 132, 1000, 154);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Intelligence: %d", mob.getIntelligence());
    SetRect (&textRect, 859, 169, 1000, 191);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
}

void	Battle::PrintHeroStatus(HDC hdc,Character hero)
{
    char buffer[128];
    sprintf_s(buffer, "Name: %s", hero.getName().c_str());
    SetRect (&textRect, 14, 21, 155, 43);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Health: %d", hero.getHealth());
    SetRect (&textRect, 14, 58, 155, 80);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Strength: %d", hero.getStrength());
    SetRect (&textRect, 14, 95, 155, 117);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Dexterity: %d", hero.getDexterity());
    SetRect (&textRect, 14, 132, 155, 154);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Intelligence: %d", hero.getIntelligence());
    SetRect (&textRect, 14, 169, 155, 191);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Wisdom: %d", hero.getWisdom());
    SetRect (&textRect, 14, 206, 155, 228);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Charisma: %d", hero.getCharisma());
    SetRect (&textRect, 14, 243, 155, 265);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

    sprintf_s(buffer, "Constitution: %d", hero.getConstitution());
    SetRect (&textRect, 14, 280, 155, 302);
    SetBkColor(hdc, RGB(255,216,0));
    DrawText( hdc, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

}
void	Battle::PrintInventory(HDC hdc,Inventory inventory)
{
    SetRect (&textRect, 129, 587, 175, 603);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, "Items:", -1, &textRect,DT_LEFT| DT_VCENTER);

    if (hero.getInventory().getNrOfitems() >=1 )
    {
    SetRect (&textRect, 174, 610, 274, 626);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[0].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }
    if (hero.getInventory().getNrOfitems() >=2 )
    {
    SetRect (&textRect, 174, 647, 274, 663);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[1].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }
    if (hero.getInventory().getNrOfitems() >=3 )
    {
    SetRect (&textRect, 174, 684, 274, 700);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[2].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }

    if (hero.getInventory().getNrOfitems() >=4 )
    {
    SetRect (&textRect, 318, 610, 418, 626);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[3].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }
    if (hero.getInventory().getNrOfitems() >=1 )
    {
    SetRect (&textRect, 318, 647, 418, 663);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[4].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }
    if (hero.getInventory().getNrOfitems() >=1 )
    {
    SetRect (&textRect, 318, 686, 418, 700);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getInventory().getItemList()[5].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);
    }
}

void	Battle::PrintSkill(HDC hdc,Skill* skill)
{
    SetRect (&textRect, 496, 587, 545, 603);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, "Skills:", -1, &textRect,DT_LEFT| DT_VCENTER);


    SetRect (&textRect, 535, 610, 635, 626);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[0].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 535, 647, 635, 663);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[1].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 535, 684, 635, 700);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[2].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 673, 169, 773, 626);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[3].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 673, 206, 773, 663);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[4].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 673, 243, 773, 700);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[5].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 821, 243, 921, 626);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[6].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 821, 243, 921, 663);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[7].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

    SetRect (&textRect, 821, 243, 921, 700);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, hero.getMajor().getSkillList()[8].getName().c_str(), -1, &textRect,DT_LEFT| DT_VCENTER);

}


void	Battle::PrintAttack(HDC hdc)
{
    SetRect (&textRect, 11, 587, 59, 603);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, "Attack", -1, &textRect,DT_LEFT| DT_VCENTER);
}
void	Battle::PrintFlee(HDC hdc)
{

    SetRect (&textRect, 11, 654, 59, 670);
    SetBkColor(hdc, RGB(218,255,127));
    DrawText( hdc, "Flee", -1, &textRect,DT_LEFT| DT_VCENTER);






}
*/

/*
void Battle::PlayerAttackMove(HDC hdc)
{
    for (int i=1; i<9; i++)
    {
        SetRect (&textRect, 285+i*40, 488-i*40, 325+i*40, 528-i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(255,0,0));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	SetRect (&textRect, 285+(i-1)*40, 488-(i-1)*40, 325+(i-1)*40, 528-(i-1)*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(161,127,255));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	Sleep (200);
    }

    for (int i=1; i<9; i++)
    {
        SetRect (&textRect, 605-i*40, 168+i*40, 645-i*40, 208+i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(255,0,0));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	SetRect (&textRect, 645-i*40, 128+i*40, 685-i*40, 168+i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(161,127,255));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	Sleep (200);
    }
}

void Battle::MobAttackMove(HDC hdc)
{
    for (int i=1; i<9; i++)
    {
        SetRect (&textRect, 675-i*40, 88+i*40, 715-i*40, 128+i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(0,0,0));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	SetRect (&textRect, 715-i*40, 48+i*40, 755-i*40, 88+i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(161,127,255));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	Sleep (200);
    }


    for (int i=1; i<9; i++)
    {
        SetRect (&textRect, 355+i*40, 408-i*40, 395+i*40, 448-i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(0,0,0));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	SetRect (&textRect, 315+i*40, 448-i*40, 355+i*40, 488-i*40);  
	bgRgn = CreateRectRgnIndirect(&textRect);
	hBrush = CreateSolidBrush( RGB(161,127,255));
	FillRgn(hdc, bgRgn, hBrush);
	DeleteObject(hBrush);
	DeleteObject(bgRgn);

	Sleep (200);
    }


}

void Battle::PrintBattleStatus(HDC hdc,string s)
{
    SetRect (&textRect, 180, 541, 841, 567);
    SetBkColor(hdc, RGB(161,127,255));
    DrawText( hdc, s.c_str(), -1, &textRect,DT_CENTER| DT_VCENTER);
}
void Battle::Animation(HDC hdc)
{

    	HRGN gRgn;
	HBRUSH Brush;
	RECT Rect;

        for (int i=1; i<9; i++)
    {




	SetRect (&Rect, 285, 488, 325, 528);  
	gRgn = CreateRectRgnIndirect(&Rect);
	Brush = CreateSolidBrush( RGB(255,0,0));
	FillRgn(hdc, gRgn, Brush);
	DeleteObject(Brush);
	DeleteObject(gRgn);

//	SetRect (&textRect, 285+(i-1)*40, 488-(i-1)*40, 325+(i-1)*40, 528-(i-1)*40);  
//	bgRgn = CreateRectRgnIndirect(&textRect);
//	hBrush = CreateSolidBrush( RGB(161,127,255));
//	FillRgn(hdc, bgRgn, hBrush);
//	DeleteObject(hBrush);
//	DeleteObject(bgRgn);

	Sleep (100);
    }


    for (int i=1; i<9; i++)
    {
        SetRect (&Rect, 285, 488, 325, 528);  
	gRgn = CreateRectRgnIndirect(&Rect);
	Brush = CreateSolidBrush( RGB(255,0,0));
	FillRgn(hdc, gRgn, Brush);
	DeleteObject(Brush);
	DeleteObject(gRgn);

//	SetRect (&textRect, 645-i*40, 128+i*40, 685-i*40, 168+i*40);  
//	bgRgn = CreateRectRgnIndirect(&textRect);
//	hBrush = CreateSolidBrush( RGB(161,127,255));
//	FillRgn(hdc, bgRgn, hBrush);
//	DeleteObject(hBrush);
//	DeleteObject(bgRgn);

	Sleep (100);
    }
}*/