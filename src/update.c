/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/* * *	  Midnight Realities was created originally by Smoo (David Brandon) based off of 
  * *    Midnight Dreams 1.0[Smoo/Jace/Berelain], based off of Rogue Edition[Listy/Adreno/Others]
 * * *   Based off of Lords of War[Sage/Dunkirk], Based off of Godwars[KaVir], Based off of Merc[Merc Team/merc license]
* * *    Based off of Diku[Diku team/diku license]
 * * *		By using this code, Midnight Realities, You agree to not claim the
 * * * *	work here as your own, and to follow the terms of conditions in the
  * * *	*	merc license, diku license, and godwar license, and any licenses that
   * * *	follow this one, as long as they do not break any laws in Canada, the
  * * *		United States, or England, and do not take credit for others work.
 *  */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * Local functions.
 */
void	total_update	args( ( void ) );

int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void 	smooregen	args( ( CHAR_DATA *ch, int hp, int mana, int move) );
void    smooregen2      args( ( CHAR_DATA *ch, int hp, int mana, int move,int) );
void randomize_object(int);
void	mobile_update	args( ( void ) );/*I snuck zerks blust in here if someone wants to take it out make there own update for it whatever*/
//void    bloodlust_update args( ( void ) );
void	weather_update	args( ( void ) );
void    room_update     args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void    ww_update       args( ( void ) );
void    embrace_update  args((void));
void    update_shapespec  args((void));

void crystal_change(CHAR_DATA *);

void gain_exp( CHAR_DATA *ch, int gain )
{
    gain = EXP(ch, gain);
    if (ch->exp < (unsigned long)4*1000000000)
    ch->exp += gain;

    if (ch->exp > (unsigned long)4*1000000000)
	ch->exp = (unsigned long)4*1000000000;

    save_char_obj(ch);

    return;
}




/*
 * uRegeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int conamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE) 
		&& ch->position != POS_MEDITATING) return 0;

	gain = number_range( 10, 20 );
	/*gain = number_range(ch->max_hit / 100, ch->max_hit / 50);*/

	if ((conamount = (get_curr_con(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: 
		if (IS_CLASS(ch, CLASS_VAMPIRE))
	        {
		gain *= conamount * 1.1;
	        }
		else gain *= conamount * 0.5;	break;
		case POS_SLEEPING:   gain *= conamount;		break;
		case POS_RESTING:    gain *= conamount * 0.5;	break;
	    }
	}

	if ( ch->pcdata->condition[COND_FULL]   == 0 && !IS_HERO(ch) )
	    gain *= 0.5;

	if ( ch->pcdata->condition[COND_THIRST] == 0 && !IS_HERO(ch) )
	    gain *= 0.5;

    }

    if ( IS_AFFECTED(ch, AFF_POISON))
	gain *= 0.25;
    if ( IS_AFFECTED(ch, AFF_FLAMING))
	gain *= 0.01;


    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int intamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->position != POS_MEDITATING) return 0;
	gain = number_range( 10, 20 );
	/*gain = number_range( ch->max_mana / 100, ch->max_mana / 50);*/

	if ((intamount = (get_curr_int(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: 
 		gain *= intamount * (ch->level*2.5/3); break;
		case POS_SLEEPING:   gain *= intamount;			break;
		case POS_RESTING:    gain *= intamount * 0.5;		break;
	    }
	}

	if ( !IS_HERO(ch) && ch->pcdata->condition[COND_THIRST] == 0 )
	    gain *= 0.5;

    }

    if ( IS_AFFECTED( ch, AFF_POISON ))
	gain *= 0.25;
    if ( IS_AFFECTED( ch, AFF_FLAMING ))
	gain *= 0.01;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;
    int dexamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->position != POS_MEDITATING) return 0;
	gain = number_range( 10, 20 );
/*	gain = number_range( ch->max_move / 100, ch->max_move / 50 );*/

	if ((dexamount = (get_curr_dex(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING:
	  if (IS_CLASS(ch, CLASS_MONK))
	        gain *= dexamount * 8;
		else 
		gain *= dexamount * 0.5;
		break;
		case POS_SLEEPING:   gain *= dexamount;		break;
		case POS_RESTING:    gain *= dexamount * 0.5;	break;
	    }
	}

	if ( !IS_HERO(ch) && ch->pcdata->condition[COND_THIRST] == 0 )
	    gain *= 0.5;
    }

    if ( IS_AFFECTED(ch, AFF_POISON))
	gain *= 0.25;
    if ( IS_AFFECTED(ch, AFF_FLAMING))
	gain *= 0.01;

    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) )
	return;

    if (!IS_NPC(ch) && IS_HERO(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) 
	&& iCond != COND_DRUNK)
	return;

    condition				= ch->pcdata->condition[iCond];
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) )
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );
    else
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value,
2000/ch->generation );
ch->pcdata->condition[COND_FULL] = 0;
    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	    {
		send_to_char( "You are REALLY hungry.\n\r",  ch );
		act( "You hear $n's stomach rumbling.", ch, NULL, NULL, TO_ROOM );
	    }
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You are REALLY thirsty.\n\r", ch );
	    else if (ch->hit > 0)
	    {
		send_to_char( "You are DYING from lack of blood!\n\r", ch );
		act( "$n gets a hungry look in $s eyes.", ch, NULL, NULL, TO_ROOM );
		ch->hit = ch->hit - number_range(2,5);
		if (number_percent() <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }
    else if ( ch->pcdata->condition[iCond] < 10 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel thirsty.\n\r", ch );
	    else
	    {
		send_to_char( "You crave blood.\n\r", ch );
		if (number_range(1,1000) <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (number_percent() > (ch->pcdata->condition[COND_THIRST]+75)
		    && !IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;
	}
    }

    return;
}

/*
 * Mob autonomous action.
 * This function takes 25 to 35 of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    static int gcount = 0;
	CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int count = 0, count2 = 0;
    int door;

    gcount++;
    if (gcount == 9) gcount = 0;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
		ch_next = ch->next;
if (ch->glorify == GLOR_ZARAF && ch->fighting)
    smooregen(ch,number_fuzzy(6)*100, number_fuzzy(6)*100,number_fuzzy(6)*100);
	if (IS_CLASS(ch, CLASS_UNICORN) && ch->position < POS_SLEEPING)
	    smooregen(ch,2,0,0);
	if (IS_CLASS(ch, CLASS_DRUID) && ch->position == POS_SITTING)
	    smooregen(ch, 200, 100, 100);
	if (IS_CLASS(ch, CLASS_DRUID) && ch->position < POS_SLEEPING)
	    smooregen(ch,2,0,0);
if (IS_CLASS(ch, CLASS_DRUID) && IS_WARP(ch, REWARD_HELPFUL))
{CHAR_DATA *rch;
for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
{
    if (rch == ch) continue;
    if (number_range(1,15) == 2) spell_dragon_breath(skill_lookup("dragon breath"), ch->spl[2]/4, ch, rch);
    if (number_range(1,20) == 3) spell_angel_breath(skill_lookup("angel breath"), ch->spl[2]/4, ch, rch);
}
}
if (IS_UNI(ch) && gcount == 8 && IS_NPC(ch) && ch->pcdata->powers[UNI_SIZE] > 0)
{
    ch->pcdata->powers[UNI_SIZE]--;
    act("$n shrinks.", ch, 0, 0, TO_ROOM);
    act("You shrink.", ch, 0, 0, TO_CHAR);
    WAIT_STATE(ch, PULSE_VIOLENCE/2);
}

	if (IS_CLASS(ch, CLASS_VAMPIRE))
	{
	    if (!ch->fighting && !ch->fight_timer && ch->hit >= 0)
		smooregen(ch,700,450,600);
	}

	if (spiderquest && spidernum <= 0)
	{DESCRIPTOR_DATA *d;
	    for (d = descriptor_list; d; d = d->next)
	    {
		if (d->character)
		    stc("&DThe SPIDER of DARKNESS &cshouts, '&GYOU MAY HAVE DEFEATED ME THIS ONCE.&c'\n\r", d->character);
	    }
	spiderquest=FALSE;
	}
	if (IS_NPC(ch) && ch->spec_fun==spec_lookup("questspider")
	    && ch->spec_fun!=NULL) count2++;
	if (IS_NPC(ch) && ch->spec_fun==spec_lookup("questspider")
	    && ch->spec_fun!=NULL && count2>spidernum && !ch->fighting)
	{
	    act("$n dissinigrates.", ch, 0, 0, TO_ROOM);
	    extract_char(ch,TRUE);
	    continue;
	}
if (IS_NPC(ch) && ch->level > 89 && ch->in_room->pccount && ch->stance[0] == -1)
{char *sta;
  switch(number_range(1,STANCE_WOLF))
  {
	case 1: default: sta = "viper"; break;
	case 2: sta = "crane"; break;
	case 3: sta = "crab"; break;
	case 4: sta = "mongoose"; break;
	case 5: sta = "bull"; break;
	case 6: sta = "mantis"; break;
	case 7: sta = "dragon"; break;
	case 8: sta = "tiger"; break;
	case 9: sta = "monkey"; break;
	case 10: sta = "swallow"; break;
	case STANCE_WOLF: sta = "wolf"; break;
  }

  do_stance(ch, sta);
}
if (IS_NPC(ch) && ch->spec_fun == spec_lookup("dancing_gummy_bear") &&
spec_lookup("dancing_gummy_bear") != NULL)
 count++;

if (IS_NPC(ch) && ((ch->spec_fun == spec_lookup("dancing_gummy_bear") &&
spec_lookup("dancing_gummy_bear") != NULL && count >= 25)||(ch->pIndexData->vnum == 5 &&ch->spec_fun==NULL)))
{act("$n melts from the sun.", ch, 0, 0, TO_ROOM);
extract_char(ch, TRUE); continue;}
if (IS_CLASS(ch, CLASS_MAGE) && ch->hit <=0) smooregen(ch,2,1,1);
if (IS_CLASS(ch,CLASS_NINJA) && ch->position ==POS_MEDITATING)
    smooregen(ch,1200,1000,1000);
if (IS_CLASS(ch,CLASS_DROW) && ch->position==POS_MEDITATING)
    smooregen(ch,120,0,120);
if ((IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch, CLASS_GOLEM)) && ch->hit <= 0)
    smooregen(ch,number_range(1,3),1,1);
	if (ch->level < 3)
	smooregen(ch, 200, 200, 200);
        if (IS_GOLEM(ch) && IS_INFORM(ch, GOLEM_RUBY) && ch->hit >= 0)
        {int a = ch->fighting ? 650 : 1500;
         a = number_range(a*.75, a*1.25);
            smooregen2(ch, a, a, a,600);
        }
	if (IS_GOLEM(ch))
	    smooregen2(ch, 0, 0, ch->max_move/100,600);
#define NRP (number_range(1,2))
        else if (IS_GOLEM(ch) && ch->hit <= 0)
            smooregen2(ch,NRP,NRP,NRP,600);
if (IS_MOOGLE(ch) && ch->position == POS_MEDITATING && ch->hit >= 0) smooregen2(ch,NR(1000,4000),NR(1000,4000),NR(1000,4000),900);

    if (IS_CLASS(ch, CLASS_HIGHLANDER))
    {
#define wpnval ch->wpn[1]
	if (ch->hit <= 0) smooregen(ch,1,1,1);
	else if (ch->fighting) ;//smooregen(ch,wpnval/3,wpnval/3,wpnval/3);
	else smooregen2(ch,wpnval/2,wpnval/2,wpnval/2,wpnval/3);
#undef wpnval
    }

if (ch->in_room && !IS_NPC(ch) && ch->trust < 7)
switch (ch->in_room->vnum)
{
    case 11308:
    case 11322:
    case 11369:
    case 11391:
    case 11392:
    case 11393:
        do_info(ch,"DO NOT GO IN THE MOB CANON ROOMS - auto paradox");
        paradox(ch);
        break;
    default: break;
}

		if ( ch->in_room == NULL )
			continue;

		if ( ch->hunting != NULL && ch->hunting != '\0' && strlen(ch->hunting) > 1 )
		{
			check_hunt( ch );
			continue;
		}

		if ( !IS_NPC(ch) )
		{
			if (ch->pcdata->condition[COND_DRUNK] > 10 && number_range(1,10) == 1)
			{
				send_to_char("You hiccup loudly.\n\r",ch);
				act("$n hiccups.",ch,NULL,NULL,TO_ROOM);
			}
			if (ch->pcdata->stage[0] > 0 || ch->pcdata->stage[2] > 0)
			{
				CHAR_DATA *vch;
				if (ch->pcdata->stage[1] > 0 && ch->pcdata->stage[2] >= 225)
				{
					ch->pcdata->stage[2] += 1;
					if ( ( vch = ch->pcdata->partner ) != NULL && !IS_NPC(vch) && vch->pcdata->partner == ch &&
					((vch->pcdata->stage[2] >= 200 && vch->sex == SEX_FEMALE) || (ch->pcdata->stage[2] >= 200 && ch->sex == SEX_FEMALE)))
					{
						if (ch->in_room != vch->in_room)
							continue;
						if (vch->pcdata->stage[2] >= 225 && ch->pcdata->stage[2] >= 225 && vch->pcdata->stage[2] < 240
							&& ch->pcdata->stage[2] < 240)
						{
							ch->pcdata->stage[2] = 240;
							vch->pcdata->stage[2] = 240;
						}
						if (ch->sex == SEX_MALE && vch->pcdata->stage[2] >= 240)
						{
							act("You thrust deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_CHAR);
						    act("$n thrusts deeply between your warm, damp thighs.",ch,NULL,vch,TO_VICT);
						    act("$n thrusts deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_NOTVICT);
							if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
								ch->pcdata->stage[2] = vch->pcdata->stage[2];
						}
						else if (ch->sex == SEX_FEMALE && vch->pcdata->stage[2] >= 240)
						{
							act("You squeeze your legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_CHAR);
							act("$n squeezes $s legs tightly around you, moaning loudly.",ch,NULL,vch,TO_VICT);
							act("$n squeezes $s legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_NOTVICT);
							if (vch->pcdata->stage[2] > ch->pcdata->stage[2])	
								ch->pcdata->stage[2] = vch->pcdata->stage[2];
						}
					}
					if (ch->pcdata->stage[2] >= 250)
					{
						if ( ( vch = ch->pcdata->partner ) != NULL && !IS_NPC(vch) && vch->pcdata->partner == ch && ch->in_room == vch->in_room)
						{
							vch->pcdata->stage[2] = 250;
							if (ch->sex == SEX_MALE)
							{
								stage_update(ch,vch,2);
								stage_update(vch,ch,2);
							}
							else
							{
								stage_update(vch,ch,2);
								stage_update(ch,vch,2);
							}
							ch->pcdata->stage[0] = 0;
							vch->pcdata->stage[0] = 0;
							if (!IS_EXTRA(ch, EXTRA_EXP))
							{
								send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",ch);
								SET_BIT(ch->extra, EXTRA_EXP);
								ch->exp += 100000;
							}
							if (!IS_EXTRA(vch, EXTRA_EXP))
							{
								send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",vch);
								SET_BIT(vch->extra, EXTRA_EXP);
								vch->exp += 100000;
							}
						}
					}
				}
				else
				{
					if (ch->pcdata->stage[0] > 0 && ch->pcdata->stage[2] < 1 && ch->position != POS_RESTING) 
					{
						if (ch->pcdata->stage[0] > 1)
							ch->pcdata->stage[0] -= 1;
						else
							ch->pcdata->stage[0] = 0;
					}
					else if (ch->pcdata->stage[2]>0 && ch->pcdata->stage[0] < 1)
					{
						if (ch->pcdata->stage[2] > 10)
							ch->pcdata->stage[2] -= 10;
						else
							ch->pcdata->stage[2] = 0;
						if (ch->sex == SEX_MALE && ch->pcdata->stage[2] == 0)
							send_to_char("You feel fully recovered.\n\r",ch);
					}
				}
			}
			
			if (ch->class == 0)
			{
				if ((ch->level > 2))
				{
				if (ch->hit < 0)
				smooregen(ch, number_range(1, 3), 2, 2);
				else if (!ch->fighting)
				smooregen(ch, 150,150,150);
				}
				else
				    mortal_regen(ch);
			}
			else if (IS_CLASS(ch, CLASS_SHAPE) && IS_HERO(ch))
			{

			if (ch->hit > 0 && (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move))
			werewolf_regen(ch);
			werewolf_regen(ch);
			}
			else if (IS_CLASS(ch, CLASS_DEMON) && IS_HERO(ch))
			{
				if (ch->hit < 0)
				{
					werewolf_regen(ch);
				}
				else
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
					{
						werewolf_regen(ch);
						werewolf_regen(ch);
						werewolf_regen(ch);
			if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
			if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
			if (ch->move > ch->max_move) ch->move = ch->max_move;
					}
				}

				/* Demon Hell Regen */
			    if (ch->in_room && (ch->in_room->vnum >= 3 && ch->in_room->vnum <=7))
			    {
                  	if (ch->hit > 0 && !ch->fighting)
                  	{
                      	    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
                      			ch->move < ch->max_move )
			{
				send_to_char("You start to regen incredibly fast.\r\n", ch);
if (!ch->fighting && !ch->blinkykill && ch->level >= 3 && ch->hit >= 0)
smooregen(ch,number_range(7550,9550),number_range(5350,5550),number_range(5350,5550));
else if (ch->hit <= 0)
smooregen(ch, number_range(1,2),200,200);
else smooregen(ch, NR(500,700),NR(500,700),NR(500,700));
				werewolf_regen(ch);
				werewolf_regen(ch);

			if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
			if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
			if (ch->move > ch->max_move) ch->move = ch->max_move;
			}
                     	    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
                      			ch->move < ch->max_move )
                      		werewolf_regen(ch);
                      	    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
                      			ch->move < ch->max_move )
                      		werewolf_regen(ch);
                      	    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
                      			ch->move < ch->max_move )
                   		werewolf_regen(ch);
                  	}

                  	if (ch->loc_hp[6] > 0)
                  	{
                      		int sn = skill_lookup( "clot" );
                            (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
                        }
                       else
                       {
                    		if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
                      			ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
                      		reg_mend(ch);
                  	}
			    } /* end of if (ch->in_room && (ch->in_room->vnum == 3 || ch->in_room->vnum == 6)) */


			}
			else if (IS_CLASS(ch, CLASS_MAGE) && IS_HERO(ch))
			{
				if (ch->hit < 0)
				{
					werewolf_regen(ch);
				}
				else if (ch->position == POS_MEDITATING && ch->hit > 0)
				{
				smooregen(ch, 300, 2000, 150);
			if (!ch->fight_timer)
				smooregen(ch, 200, 1000, 350);
				}
				else
				smooregen(ch, 25, 35, 25);
			}
			else if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_HERO(ch))
			{
				if (ch->position == POS_FIGHTING && !IS_ITEMAFF(ch, ITEMA_RAGER))
				{
					if (ch->rage < 300)
						ch->rage +=number_range(10,15);
					if (ch->rage < 300 && ch->pcdata->powers[DISC_WERE_WOLF] > 3)
						ch->rage += number_range(10,15);
					if (!IS_SET(ch->special, SPC_WOLFMAN) && 
						ch->rage >= 100)
					do_werewolf(ch,"");
				}
				else if (ch->rage > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
				{
					ch->rage -= 1;
					if (ch->rage < 100)
						do_unwerewolf(ch,"");
				}
				if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
					werewolf_regen(ch);

				if (ch->position == POS_SLEEPING && ch->power[DISC_WERE_BEAR] > 3 && ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
			smooregen(ch, 100, 100, 150);
 					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						smooregen(ch, 100, 100, 150);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
			smooregen(ch, 100, 400, 450);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
   smooregen(ch, 1700, 3000, 3050);
				}
				if (ch->loc_hp[6] > 0)
				{
					int sn = skill_lookup( "clot" );
					(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
				}
				else
				{
					if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
						reg_mend(ch);
				}
			}
			else if (IS_CLASS(ch, CLASS_VAMPIRE) && IS_HERO(ch))
			{
				if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
					werewolf_regen(ch);
				if (ch->level < 3)
					ch->rage = 0;
				if ( ch->position == POS_FIGHTING && ch->rage > 0 && ch->rage < 25 && !IS_ITEMAFF(ch, ITEMA_RAGER) )		   
					ch->rage += 1;
				else if (ch->rage > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
				    ch->rage -= 1;
				if (ch->rage < 1)
					continue;
				if (ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
				}
				if (ch->loc_hp[6] > 0)
				{
				    int sn = skill_lookup( "clot" );
					(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
				}
				else
				{
				    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
					reg_mend(ch);
				}
		    }
		    else if (IS_CLASS(ch, CLASS_DROW) && IS_HERO(ch))
			{
				if (ch->hit < 0)
				{
					if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
					{
						REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
						act("The darkness around $n fades.",ch,NULL,NULL,TO_ROOM);
					}
					ch->hit +=number_range(1,2);
					update_pos(ch);
				}
				else
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if (ch->position == POS_MEDITATING)
					{
					    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					    if (ch->loc_hp[6] > 0)
					    {
						int sn = skill_lookup( "clot" );
						(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
					    }
					    else
					    {
						if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
							reg_mend(ch);
					    }
					}
				}
			}
			else if (IS_CLASS(ch, CLASS_MONK) &&IS_HERO(ch))
			{
				if ( ch->hit < 0)
				{
					ch->hit += number_range(1,2);
					update_pos(ch);
				}
				else if (ch->fighting == NULL && ch->position == POS_MEDITATING)
				    smooregen(ch, NR(900,1100), NR(500,700), NR(700,900));
				else smooregen(ch, 20, 25, 25);
				if (ch->position == POS_MEDITATING)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if (ch->loc_hp[6] > 0)
					{
						int sn = skill_lookup( "clot" );
						(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
					}
					else
					{
						if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
							reg_mend(ch);
					}
				}
			}
			else if (IS_CLASS(ch, CLASS_NINJA) &&IS_HERO(ch))
			{int num = number_range(200, 250);
				if (ch->position == POS_MEDITATING)
				smooregen(ch, num, num, num);
				if (ch->hit < 0)
				{
					ch->hit += number_range(1,3);
					update_pos(ch);
				}
				else
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
				}

				if (ch->position == POS_FIGHTING)
					continue;
				if (ch->rage > 0)
					ch->rage -= 1;
				if ((ch->rage > 0) && ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
				}
				if ((ch->rage > 50) && (ch->position != POS_FIGHTING) && (ch->hit > 0))
				{
					if (ch->loc_hp[6] > 0)
					{
						int sn = skill_lookup( "clot" );
						(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
					}
					else
					{
						if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
						reg_mend(ch);
					}
				}
				if (ch->position == POS_MEDITATING && ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
                              if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
                                    werewolf_regen(ch);
				}
			}
			else if ( IS_CLASS(ch, CLASS_AMAZON)&&IS_HERO(ch) )
			{
			if (ch->position != POS_FIGHTING && ch->hit > 0)
			smooregen(ch, 260, 150, 220);
				if ( ch->hit < 0)
				{
					ch->hit += number_range(1,4);
					update_pos(ch);
				}
				else
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					werewolf_regen(ch);
				}
				if (ch->position == POS_MEDITATING && ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if (ch->loc_hp[6] > 0)
					{
						int sn = skill_lookup( "clot" );
						(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
					}
					else
					{
						if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
							reg_mend(ch);
					}
				}
			}
			else if (IS_CLASS(ch, CLASS_JEDI)&&IS_HERO(ch))
			{
			int focus = ch->pcdata->powers[JEDI_FOCUS_MAX];
			focus *= 12;
			if (ch->position == POS_MEDITATING)
			smooregen(ch, focus, focus, focus);
				if ( ch->hit < 0)
				{
					werewolf_regen(ch);
					update_pos(ch);
				}
			    else
			    {
				if (IS_ITEMAFF(ch, ITEMA_SPEED))
				{
					// ch->move -= 100; 3-17 Morpheus
					if (ch->move < (ch->max_move / 2))
					{
						REMOVE_BIT(ch->itemaffect, ITEMA_SPEED);
						send_to_char( "Your actions slow down to normal speed as you release yourself from the force.\n\r", ch );
						act("$n releases themself from the force.",ch,NULL,NULL,TO_ROOM);
					}
				}
				if ((ch->position != POS_FIGHTING) && (ch->pcdata->powers[JEDI_STATE] > 0) && (ch->generation < 2))
				{
					if ((number_range(1,8)) == 1)
					{
						ch->pcdata->powers[JEDI_STATE] -= 1;
						ch->move -= (ch->pcdata->powers[JEDI_STATE] * 25);
						if (ch->move < ch->max_move / 4)
						{
							ch->pcdata->powers[JEDI_STATE] = 0;
							send_to_char( "You lose your state.\n\r", ch );
							act( "$n loses their glow.", ch, NULL, NULL, TO_ROOM );
						}
						act("You relax slightly.", ch, NULL, NULL, TO_CHAR);
						act("$n relaxes slighty.", ch, NULL, NULL, TO_NOTVICT);
						if (ch->pcdata->powers[JEDI_STATE] < 0)
							ch->pcdata->powers[JEDI_STATE] = 0;
					}
				}
				if ((ch->position == POS_FIGHTING) && (ch->pcdata->powers[JEDI_STATE] > 0))
				{
					ch->move -= (ch->pcdata->powers[JEDI_STATE] * 25);
					if (ch->move < ch->max_move / 4)
					{
						ch->pcdata->powers[JEDI_STATE] = 0;
						send_to_char( "You lose your jedi state of mind.\n\r", ch );
						act( "$n loses their glow.", ch, NULL, NULL, TO_ROOM );
					}
				}
				if ((ch->position != POS_FIGHTING) && IS_SET(ch->special, SPC_JEDI_QUICKEN))
					REMOVE_BIT(ch->special, SPC_JEDI_QUICKEN);
				else if ((ch->move < 2000) && IS_SET(ch->special, SPC_JEDI_QUICKEN))
					REMOVE_BIT(ch->special, SPC_JEDI_QUICKEN);
				else if (IS_SET(ch->special, SPC_JEDI_QUICKEN) && ch->position == POS_FIGHTING)
					ch->move -= 50;

				if ((ch->position == POS_MEDITATING) && (ch->pcdata->powers[JEDI_FOCUS] < ch->pcdata->powers[JEDI_FOCUS_MAX] && ch->hit > 0))
				{
					ch->pcdata->powers[JEDI_FOCUS] += ((ch->pcdata->powers[JAPP_MASTERED] + 1) * 2);
					if (ch->pcdata->powers[JEDI_FOCUS] > ch->pcdata->powers[JEDI_FOCUS_MAX])
						ch->pcdata->powers[JEDI_FOCUS] = ch->pcdata->powers[JEDI_FOCUS_MAX];
				}
				else
				{
						ch->pcdata->powers[JEDI_FOCUS] += ch->pcdata->powers[JAPP_MASTERED] + 1;
						if (ch->pcdata->powers[JEDI_FOCUS] > ch->pcdata->powers[JEDI_FOCUS_MAX])
							ch->pcdata->powers[JEDI_FOCUS] = ch->pcdata->powers[JEDI_FOCUS_MAX];
				}

				if ((ch->position == POS_MEDITATING) && ch->hit > 0)
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if (ch->loc_hp[6] > 0)
					{
						int sn = skill_lookup( "clot" );
						(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
					}
					else
					{
						if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
							reg_mend(ch);
					}

				}
				else
				{
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
					if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
						werewolf_regen(ch);
				}
			   }
			}
			else if (IS_ITEMAFF(ch, ITEMA_REGENERATE))
			{
			if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
				ch->move < ch->max_move )
				werewolf_regen(ch);
			if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana ||
                                ch->move < ch->max_move )
                                werewolf_regen(ch);
			if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana ||
                                ch->move < ch->max_move )
                                werewolf_regen(ch);
			if (ch->loc_hp[6] > 0)
			{
				int sn = skill_lookup( "clot" );
				(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
			}
			else
			{
				if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
				ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
				reg_mend(ch);
			}
	    }
	    continue;
	}

	if ( IS_AFFECTED(ch, AFF_CHARM) ) continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	    if (ch == NULL) continue;
	}

	/* That's all for sleeping / busy monster */
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 2 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && obj->cost > max )
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n picks $p up.", ch, obj_best, NULL, TO_ROOM );
		act( "You pick $p up.", ch, obj_best, NULL, TO_CHAR );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
	&& ( ch->hunting == NULL || strlen(ch->hunting) < 2 )
	&& ( (!IS_SET(ch->act, ACT_STAY_AREA) && ch->level < 900)
	||   pexit->to_room->area == ch->in_room->area ) 
	&& !find_kingdom(ch->kingdom) && !ch->fighting)
	{
	    move_char( ch, door );
	}
    }
if (spiderquest)
{
    while (spidernum > count2)
    {
	make_questspider(get_room_index(2));
	count2++;
    }
}
    return;
}


void update_shapespec( void )
{
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *d;
	for ( d = descriptor_list ; d != NULL ; d = d->next )
	{
		if (d->connected < CON_PLAYING) continue;
	    ch = d->character;
	    if (ch == NULL) continue;

if (ch->glorify >= GLOR_JAMES && ch->glorify-GLOR_JAMES < 7)
{stc("You grow some fluff.\n\r", ch);ch->glorify++;
 act("$n grows some fluff.\n\r", ch,0,0,TO_ROOM);
 if (ch->glorify-GLOR_JAMES > 7)
    ch->glorify = 7+GLOR_JAMES;
}
if (is_affected(ch,skill_lookup("waterwield")))
    smooregen2(ch,200,200,200,400);

/* golems */
 if (IS_CLASS(ch, CLASS_GOLEM)) {
    if (IS_SET(ch->pcdata->powers[0], GOLEM_CRYSTAL))
{	crystal_change(ch);}
 if (IS_SET(ch->pcdata->powers[0],GOLEM_OBSIDIAN))
{	obsidian_change(ch);}
 if (IS_INFORM(ch,GOLEM_EMERALD))
	emerald_change(ch);
if (IS_INFORM(ch, GOLEM_GOLD))
{       gold_change(ch);}   

  if (ch->countdown > 0) {
  if (ch->convert_to == GOLEM_CRYSTAL)
  {    crystal_change(ch);crystal_change(ch);}
  else if (ch->convert_to == GOLEM_OBSIDIAN)
      obsidian_change(ch);
  else if (IS_INFORM(ch, GOLEM_GOLD))
      gold_change(ch);
  else {stc("Haha. Bugged golem form. Laf, sucks to be you!\n\r", ch);
  ch->convert_to = 0;}
}}


	if (ch->shape[beeping] > 0)
{send_to_char("Moo..annoying..yay..\n\r\a\a\a\a", ch);ch->shape[beeping]
-=
1;}
		if ((will_use(ch, "Rejuvination") || can_use(ch,
"Rejuvination") )
&& ch->hit < 1 && IS_SET(ch->extra, TIED_UP)) {smooregen(ch, 8, 5,
5);REMOVE_BIT(ch->extra, TIED_UP);act("The ropes around $n disinigrate.",
ch, NULL, NULL, TO_ROOM);}
	if (will_use(ch, "Fast Healing") && ch->hit > 0) smooregen(ch, 120,
225, 200);
	    if (IS_CLASS(ch, CLASS_SHAPE) && ch->shapespec != 0 
		&& ch->shape[spectoggle] == TRUE)
	{
	    ( *ch->shapespec) ( ch );
	    if (ch == NULL) continue;
	}
	if (IS_CLASS(ch, CLASS_DROW) && ch->stance[0] == STANCE_RAHM)
	{
	    SPEC_FUN *spec = spec_lookup("hustler");
	    if (!spec) spec = spec_lookup("spec_hustler");
	    if (spec) (*spec)(ch);
	    if (!ch) continue;
	    if (spec) (*spec)(ch);
	    if (!ch) continue;
	}
	}
return;
}

/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch = NULL;
    int diff;
    bool char_up;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   (ch = d->character) != NULL
	    &&  !IS_NPC(ch))
	    {
			if ((IS_CLASS(ch, CLASS_DROW) && (IS_SET(ch->newbits, NEW_DARKNESS)) ) )
			{
				send_to_char("You banish your globe of darkness.\n\r", ch );
				act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
				REMOVE_BIT(ch->newbits, NEW_DARKNESS);
				if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
					REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
			}
		}
	}
	break;
    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 20:
        weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if (( d->connected == CON_PLAYING || d->connected == CON_EDITING ) && (ch = d->character) != NULL && !IS_NPC(ch))
	    {
			if ( IS_SET(ch->act, PLR_SACREDINVIS) && (ch->level < 8) && IS_CLASS(ch, CLASS_MONK))
			{
				REMOVE_BIT(ch->act, PLR_SACREDINVIS);
				send_to_char( "You emerge from your cloak of invisibility.\n\r", ch );
				act("$n slowly emerges from a cloak of holy invisibility.",ch,NULL,NULL,TO_ROOM);
			}
		}
	}

	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   (ch = d->character) != NULL
	    &&  !IS_NPC(ch))
	    {
		send_to_char( "You hear a clock in the distance strike midnight.\n\r",ch);
		if (IS_EXTRA(ch, EXTRA_ROT))
                {
                        send_to_char("Your flesh feels better.\n\r", ch);
                        REMOVE_BIT(ch->extra, EXTRA_ROT);
                        
                }
                if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
                {
                send_to_char("The silence leaves the room.\n\r",ch);
                act("The silence leaves the room.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_SILENCE);
                
                }

                if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
                {
                send_to_char("The flames in the room die down.\n\r",ch);
                act("The flames in the room die down.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_FLAMING);
                }
                if ( IS_CLASS(ch, CLASS_VAMPIRE) )
		{
		    if (ch->hit < ch->max_hit)
			{ ch->hit = ch->max_hit; char_up = TRUE; }
		    if (ch->mana < ch->max_mana)
			{ ch->mana = ch->max_mana; char_up = TRUE; }
		    if (ch->move < ch->max_move)
			{ ch->move = ch->max_move; char_up = TRUE; }
		    if (char_up) send_to_char( "You feel the strength of the kindred flow through your veins!\n\r", ch );
		    ch->position = POS_STANDING;
if (ch->pcdata->gentick < 23-ch->generation)
{
ch->pcdata->gentick += 1;
}
if (ch->pcdata->gentick == 23-ch->generation 
    && ch->generation > 3 && ch->pcdata->lwrgen==FALSE)
{
send_to_char("You become one generation closer to Caine!\n\r",ch);
ch->generation -=1;
ch->pcdata->gentick=0;
ch->pcdata->lwrgen=TRUE;
}



		    if (IS_EXTRA(ch, EXTRA_POTENCY))
		    { 
			send_to_char("You feel your blood potency fade away.\n\r", ch);	
//			ch->generation =ch->generation +1;
			REMOVE_BIT(ch->extra, EXTRA_POTENCY);
		    }
                    if (IS_SET(ch->newbits, NEW_TIDE))
 	            {
		    REMOVE_BIT(ch->newbits, NEW_TIDE);
                     send_to_char("The tide of vitae leaves you.\n\r", ch);
		    }
		    if (IS_EXTRA(ch, EXTRA_BAAL))
                    {
                       send_to_char("The spirit of Baal escapes you.\n\r",ch);  
                       ch->power[DISC_VAMP_POTE] -= 2;
                       ch->power[DISC_VAMP_CELE] -= 2;
                       ch->power[DISC_VAMP_FORT] -= 2;
                       REMOVE_BIT(ch->extra, EXTRA_BAAL);
                    }   
                    if (IS_EXTRA(ch, EXTRA_FLASH))
                    {
                       send_to_char("Your speed slows.\n\r",ch);  
                       ch->power[DISC_VAMP_CELE] -= 1;
                       REMOVE_BIT(ch->extra, EXTRA_FLASH);
                    }   

                   }			
		  } 
                 
	    
	}
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default: 
	bug( "Weather_update: bad sky d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}


int is_wall(EXIT_DATA *ex)
{

    if (IS_SET(ex->exit_info, EX_ICE_WALL)) return 1;
    if (IS_SET(ex->exit_info, EX_FIRE_WALL)) return 2;
    if (IS_SET(ex->exit_info, EX_SWORD_WALL)) return 3;
    if (IS_SET(ex->exit_info, EX_PRISMATIC_WALL)) return 4;
    if (IS_SET(ex->exit_info, EX_IRON_WALL)) return 5;
    if (IS_SET(ex->exit_info, EX_MUSHROOM_WALL)) return 6;
    if (IS_SET(ex->exit_info, EX_CALTROP_WALL)) return 7;
    if (IS_SET(ex->exit_info, EX_ASH_WALL)) return 8;
    return 0;
}

const char * wall[MAX_WALL+1] = 
{
    "","wall of ice", "wall of fire", "wall of swords",
	"prismatic wall", "wall or iron", "wall of mushrooms",
	"wall of caltrops", "wall of ash"
};

/*
 * Update all rooooooms, like gore, poison clouds etc....
 */
void room_update( void )
{
    int i;
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *room;

    
    for ( room = room_list; room != NULL; room = room->next_room)
    {
        
        if (RTIMER(room, RTIMER_WALL_NORTH) == 1
	    && room->exit[DIR_NORTH] != NULL
	    && is_wall(room->exit[DIR_NORTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_NORTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_NORTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_SOUTH) == 1
	    && room->exit[DIR_SOUTH] != NULL
	    && is_wall(room->exit[DIR_SOUTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_SOUTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_SOUTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_EAST) == 1
	    && room->exit[DIR_EAST] != NULL
	    && is_wall(room->exit[DIR_EAST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_EAST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_EAST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_WEST) == 1
	    && room->exit[DIR_WEST] != NULL
	    && is_wall(room->exit[DIR_WEST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_WEST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_WEST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_UP) == 1
	    && room->exit[DIR_UP] != NULL
	    && is_wall(room->exit[DIR_UP])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_UP])]);
	    room_message(room, buf);
	    make_wall(room, DIR_UP, 0);
        }

        if (RTIMER(room, RTIMER_WALL_DOWN) == 1
	    && room->exit[DIR_DOWN] != NULL
	    && is_wall(room->exit[DIR_DOWN])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_DOWN])]);
	    room_message(room, buf);
	    make_wall(room, DIR_DOWN, 0);
        }


	if (RTIMER(room, RTIMER_STINKING_CLOUD) == 1)
	    room_message(room, "The poisonous vapours dissipate and clear.");

	if (RTIMER(room, RTIMER_HIDE_ROOM) == 1)
	    room_message(room, "The shroud leaves the room.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 2)
	    room_message(room, "The vapourous ghosts start howling insanely.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 3)
	    room_message(room, "The vapourous ghosts start moaning.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 1)
	    room_message(room, "The vapourous ghosts dissipate and vanish.");

	if (RTIMER(room, RTIMER_GLYPH_PROTECTION) == 1)
	    room_message(room, "The glyph of protection flares and vanishes.");

	if (RTIMER(room, RTIMER_SWARM_BEES) == 1)
	    room_message(room, "The bees fly away into the sky.");

	if (RTIMER(room, RTIMER_DISCORD) == 1)
	    room_message(room, "The banging and crashing stops.");

	if (RTIMER(room, RTIMER_SWARM_BATS) == 1)
	    room_message(room, "The bats flap away into the night.");

	if (RTIMER(room, RTIMER_SWARM_RATS) == 1)
	    room_message(room, "The rats scurry away into the floorboards.");
  
      if (RTIMER(room, RTIMER_SILENCE) == 1)
	    room_message(room, "The silence disappates.");

	for (i = 0 ; i < MAX_RTIMER ; i++)
	    room->tick_timer[i] = UMAX(room->tick_timer[i] - 1, 0);

    }

    return;

}



/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    OBJ_DATA *obj,*obj_n;
    int count = 0;
    int i;ROOM_INDEX_DATA *rm;
    bool is_obj;
    bool drop_out = FALSE;
    time_t save_time;
    GAMBLE_DATA *gamb, *g_n;
    char buf[MIL];
    sh_int entrope[5] ={2,2,2,2,2};

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;


for (gamb = gamble_first; gamb; gamb = g_n)
{g_n = gamb->next;
    if (gamb->timer > -1)
    {
	gamb->timer--;

	if (gamb->timer == -1)
	    gamble_start(gamb);
	else {sprintf(buf, "&C%d ticks left to join the game %s.", gamb->timer+1, gamb->name);
	      gamble_channel(buf);}
    }
    else gamb->timer--;
}

for (count=0,obj = object_list; obj; obj = obj_n)
{obj_n = obj->next;
    // Shape entropy stones
    if (obj->pIndexData->vnum >= 29 && obj->pIndexData->vnum <= 33)
    {
	entrope[obj->pIndexData->vnum-29]--;
	if (entrope[obj->pIndexData->vnum-29] < 0) {extract_obj(obj);continue;}
    }

    if (obj->pIndexData->vnum == 24 && !obj->carried_by) count++;
}

if (count < 5)
{
    for ( ; count < 5; count++)
	randomize_object(24);
}

//Shape entropy stones
for (count = 0; count < 5; count++)
{
    while (entrope[count] > 0)
    {
	randomize_object(count+29);
	entrope[count]--;
    }
}

for (i = 0; i < MAX_KEY_HASH; i++)
{
    for (rm = room_index_hash[i]; rm; rm = rm->next)
    {
	if (str_cmp(rm->disabled, "") && --rm->disabletick < 0)
	{
	    for (ch = rm->people; ch; ch = ch->next_in_room)
		stc("&CThe power surrounding the room fades..\n\r",ch);
	    free_string(rm->disabled);rm->disabled = str_dup("");
	}
    }
}
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	int i;
	ch_next = ch->next;
	if (!ch->in_room) {char_to_room(ch, get_room_index(2));if (!ch->in_room) continue;}

	if (!IS_NPC(ch) && ch->desc == NULL)
	{
	    if (ch->pnote) {
	    free_string(ch->pnote->text);
	    free_string(ch->pnote->subject);
	    free_string(ch->pnote->to_list);
	    free_string(ch->pnote->date);
	    free_string(ch->pnote->sender);
	    ch->pnote->next = note_free;
	    note_free = ch->pnote;
	    ch->pnote = NULL;}

	    if (ch->hit < 1) ch->hit = 1;
		update_pos(ch);
	    do_quit(ch,"Linkdead quit");
	    continue;
	}
if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0 && ch->pcdata->chobj)
{
  OBJ_DATA *chobj = create_object(get_obj_index(ch->pcdata->obj_vnum), 5);
  if (chobj)
  {
	chobj->chobj = ch;
	ch->pcdata->chobj = chobj;
  }
}
if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[0] > 0 || ch->chi[1] > 0) && number_range(1,2)==1)
{
    if (ch->chi[0] > ch->chi[1])
    {
	act("&CYour chi collapses.", ch, 0, 0, TO_CHAR);
	act("&C$n's chi collapses.", ch, 0, 0, TO_ROOM);
	ch->chi[0]--;
    }
    else
    {
	act("&RYour fury subsides.", ch, 0, 0, TO_CHAR);
	act("&R$n's fury subsides.", ch, 0, 0, TO_ROOM);
	ch->chi[1]--;
    }
}

if (IS_CLASS(ch, CLASS_GOLEM)) SET_BIT(ch->flag3, AFF3_WASGOLEM);
if ((!IS_CLASS(ch, CLASS_GOLEM) && IS_SET(ch->flag3, AFF3_WASGOLEM))
&& ((ch->generation == 1 && !IS_CLASS(ch, CLASS_DEMON)) || (ch->generation == 15 && IS_CLASS(ch, CLASS_DEMON))))
{
    sprintf(log_buf, "%s is no longer gen 1 %s(%s was once a golem..)", ch->pcdata->switchname,
class_table[class_lookup(ch->class)==-1?0:class_lookup(ch->class)].class_name, ch->sex==0?"It":ch->sex==1?"He":"She");
    do_info(ch,log_buf);
    ch->generation = (IS_CLASS(ch, CLASS_DEMON)?12:3);
}
	if (ch->home == 2)
	{
	    ch->home = 3001;
	    sprintf(log_buf, "&G%s's home was in LIMBO! NOOOOOO!!! It is now reset.",
IS_NPC(ch)?ch->short_descr:ch->pcdata->switchname);
	    do_info(ch, log_buf);
	    paradox(ch);
	}

if (IS_CLASS(ch,CLASS_FLUFF) && ch->pcdata->powers[0] < 4)
{
    ch->pcdata->powers[0]++;
    act("&CThe patches of fur on $n grow more immense.", ch, 0, 0, TO_ROOM);
    act("&CThe patches of fur on you grow more immense.", ch, 0, 0, TO_CHAR);
}

	if (!IS_NPC(ch) && (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)))
	    is_obj = TRUE;
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	    {is_obj = TRUE;SET_BIT(ch->extra, EXTRA_OSWITCH);}
	else
	    is_obj = FALSE;
	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->level >= 2
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}

	if  (ch->fighting == NULL
	    && !IS_SET(ch->newbits, NEW_LWRATH)
	    && !IS_NPC(ch)
	    && !is_obj
	    && IS_CLASS(ch, CLASS_WEREWOLF)
	    && ch->power[DISC_WERE_LUNA] > 8
	    && weather_info.sunlight == SUN_DARK)
	{
	   SET_BIT(ch->newbits, NEW_LWRATH);
	   send_to_char("You are protected by Luna.\n\r",ch);
	}


	/* Character Tick Timers */

        if (ch->tick_timer[TIMER_CAN_CALL_WAR_HORSE] == 1)
	    send_to_char("You may now call your war horse again.\n\r", ch);

 	if (ch->tick_timer[TIMER_CAN_POLYMORPH] == 1)
	    send_to_char("You may now polymorph again.\n\r", ch);

 	if (ch->tick_timer[TIMER_MAKE_SNOWMAN] == 1)
	    send_to_char("You are ready to make another snowman now.\n\r", ch);

 	if (ch->tick_timer[TIMER_SPHINX_ROAR] == 1)
	    send_to_char("You are ready to roar again.\n\r", ch);

 	if (ch->tick_timer[TIMER_NEWBIE_IMM] == 1)
	    send_to_char("You are no longer protected by the gods.\n\r", ch);

 	if (ch->tick_timer[TIMER_CAN_DO_NEXUS] == 1)
	    send_to_char("You may now create another nexus.\n\r", ch);

 	if (ch->tick_timer[TIMER_TREE] == 1 && ch->cur_form == FRM_TREE)
	{
	    send_to_char("You start to change back into humanoid form.\n\r", ch);
	    act("$n starts to change back into a humanoid form.",ch,NULL,NULL,TO_ROOM);
	    set_form(ch, get_normal_form(ch) );
	}

        if (ch->tick_timer[TIMER_INFERNO] == 1)
	    stc("You are able to use Inferno again.\n\r",ch);

 	if (ch->tick_timer[TIMER_CAN_SHRIEK] == 1)
	    send_to_char("Your voice has recovered.\n\r", ch);

 	if (ch->tick_timer[TIMER_CAN_CALL_ROCKS] == 1)
	    send_to_char("You may now call the Bane.\n\r", ch);

 	if (ch->tick_timer[TIMER_HELLFIRE_SUMMON] == 1)
	    send_to_char("You may now summon hellfire once more.\n\r", ch);

 	if (ch->tick_timer[TIMER_ENTOMB] == 1)
	    send_to_char("You may now use the entomb ability again.\n\r", ch);

 	if (ch->tick_timer[TIMER_CAN_BREATHE_FROST] == 1)
	    send_to_char("You are ready to breathe frost again.\n\r", ch);

 	if (ch->tick_timer[TIMER_CAN_FEATHER] == 1)
	    send_to_char("You are ready to make another feather.\n\r", ch);

	    if (ch->tick_timer[TIMER_THIRD_ARM_GROWING] == 1)
	    {
	 	send_to_char("A third arm bursts out of your stomach.\n\r", ch);
		act("An arm bursts out of $n's stomach, spraying blood everywhere.", ch, NULL, NULL, TO_ROOM);
		SET_BIT(ch->newbits, THIRD_HAND);
	    }

	    if (ch->tick_timer[TIMER_FOURTH_ARM_GROWING] == 1)
	    {
	 	send_to_char("Another arm bursts out of your stomach.\n\r", ch);
		act("An arm bursts out of $n's stomach, spraying blood everywhere.", ch, NULL, NULL, TO_ROOM);
		SET_BIT(ch->newbits, FOURTH_HAND);
	    }

	for (i = 0; i < MAX_TIMER; i++)
	    if (ch->tick_timer[i] > 0) ch->tick_timer[i] -= 1;

	if (ch->fighting == NULL
	&& !IS_SET(ch->newbits, NEW_CLOAK)
	&& !IS_NPC(ch)
	&& !is_obj
	&& IS_CLASS(ch, CLASS_MONK)
	&& ch->pcdata->powers[PMONK] > 10)
	{
	   SET_BIT(ch->newbits, NEW_CLOAK);
	   send_to_char("Your Cloak of Life is restored.\n\r",ch);
	}


	if ( ch->position > POS_STUNNED && !is_obj)
	{
	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	}

	if ( ch->position == POS_STUNNED && !is_obj && !IS_AFFECTED(ch, AFF_STUNNED))
	{
            ch->hit = ch->hit + number_range(2,4);
	    update_pos( ch );
	}

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL && !is_obj)
	{
	    OBJ_DATA *obj;
	    int blood;

	    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    || ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 ) )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	    }if (str_cmp(ch->immpower,"")) ch->timer = 0;

	    if ( ++ch->timer >= 19 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    if ( ch->timer > 35 ) ch_quit = ch;

	    gain_condition( ch, COND_DRUNK,  -1 );
	    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	    {
	    	gain_condition( ch, COND_FULL, -1 );
	    	gain_condition( ch, COND_THIRST, -1 );
	    }
	    else
	    {
		blood = -1;
		if (ch->beast > 0)
		{
		    if (IS_VAMPAFF(ch, VAM_CLAWS)) blood -= number_range(1,3);
		    if (IS_VAMPAFF(ch, VAM_FANGS)) blood -= 1;
		    if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) blood -= 1;
		    if (IS_VAMPAFF(ch, PLR_SHADOWSIGHT)) blood -= number_range(1,3);
		    if (IS_SET(ch->act, PLR_HOLYLIGHT)) blood -= number_range(1,5);
		    if (IS_VAMPAFF(ch, VAM_DISGUISED)) blood -= number_range(5,10);
		    if (IS_VAMPAFF(ch, VAM_CHANGED)) blood -= number_range(5,10);
		    if (IS_VAMPAFF(ch, IMM_SHIELDED)) blood -= number_range(1,3);
		    if (IS_POLYAFF(ch, POLY_SERPENT)) blood -= number_range(1,3);
		}
               ch->pcdata->condition[COND_THIRST] += blood;
               if (ch->pcdata->condition[COND_THIRST] <= 0)
               {
                ch->pcdata->condition[COND_THIRST] = 0;
               } 
	    }
	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
		paf->duration--;
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off && !is_obj)
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}
	  
		affect_remove( ch, paf );
	    }
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */
	if ( ch->loc_hp[6] > 0 && !is_obj && ch->in_room != NULL )
	{
	    int dam = 0;
	    int minhit = 0;
	    if (!IS_NPC(ch)) minhit = -11;
	    if (IS_BLEEDING(ch,BLEEDING_HEAD) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's neck.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your neck.\n\r", ch );
		dam += number_range(20,50);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_THROAT) && (ch->hit-dam) > minhit )
	    {
		act( "Blood pours from the slash in $n's throat.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "Blood pours from the slash in your throat.\n\r", ch );
		dam += number_range(10,20);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_HERO(ch)) { ch->hit = ch->hit - dam;
		if (ch->hit < 1) ch->hit = 1; }
	    else ch->hit = ch->hit - dam;
	    update_pos(ch);
	    ch->in_room->blood += dam;
	    if (ch->in_room->blood > 1000) ch->in_room->blood = 1000;
	    if (ch->hit <=-11 || (IS_NPC(ch) && ch->hit < 1))
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	if (IS_EXTRA(ch,EXTRA_ROT) && !is_obj && !drop_out)
	{
	 int dam;
	 if (IS_NPC(ch)) break;
         act( "$n's flesh shrivels and tears.", ch, NULL, NULL, TO_ROOM);
	 send_to_char( "Your flesh shrivels and tears.\n\r", ch);
         dam = number_range(250,500);
         ch->hit = ch->hit - dam;
         update_pos(ch);
         if (ch->hit < -10)
         {
            do_killperson(ch,ch->name);
            drop_out = TRUE;
         }
        }
/*
	if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING) && !IS_AFFECTED(ch,AFF_FLAMING))
        {
        send_to_char("You catch on fire!\n\r",ch);
        SET_BIT(ch->affected_by, AFF_FLAMING);
        sprintf(buf,"%s catches on fire!\n\r",ch->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        }
*/

     if ( IS_AFFECTED(ch, AFF_FLAMING) && !is_obj && !drop_out && ch->in_room != NULL )
	{
	    int dam;
	    if (IS_NPC(ch)) break;
	    act( "$n's flesh burns and crisps.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "Your flesh burns and crisps.\n\r", ch );
	    dam = number_range(250,300);
	    ch->hit = ch->hit - dam;
	    update_pos(ch);
	    if (ch->hit <=-9)
	    {
		update_pos(ch);
		drop_out = TRUE;
	    }
	}
	else if ( IS_CLASS(ch, CLASS_VAMPIRE)
	&& (!IS_AFFECTED(ch,AFF_SHADOWPLANE)) && !IS_EXTRA(ch,EXTRA_EARTHMELD)
	&& (!IS_NPC(ch) && !IS_IMMUNE(ch,IMM_SUNLIGHT))
	&& ch->in_room != NULL
	&& (!ch->in_room->sector_type == SECT_INSIDE) && !is_obj
	&& (!room_is_dark(ch->in_room)) && (weather_info.sunlight != SUN_DARK) )
	{
	    act( "$n's flesh smolders in the sunlight!", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "Your flesh smolders in the sunlight!\n\r", ch );
	    /* This one's to keep Zarkas quiet ;) */
	    if (IS_POLYAFF(ch, POLY_SERPENT))
	    	ch->hit = ch->hit - number_range(2,4);
	    else
	    	ch->hit = ch->hit - number_range(5,10);
	    update_pos(ch);
	    if (ch->hit <=-11)
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	else if ( IS_AFFECTED(ch, AFF_POISON) && !is_obj && !drop_out )
	{
	    act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You shiver and suffer.\n\r", ch );
	    damage( ch, ch, 2, gsn_poison );
	}/*
	else if ( !IS_NPC( ch ) && ch->paradox[1] > 0 )
	{
	    if ( ch->paradox[1] > 50 ) paradox( ch );
	    else if ( ch->paradox[2] == 0 && ch->paradox[1] > 0 )
	    {
		ch->paradox[1] --;
		ch->paradox[2] = PARADOX_TICK;
	    }
	    else ch->paradox[3] --;
	}*/
	else if ( ch->position == POS_INCAP && !is_obj && !drop_out )
	{
	    if (ch->level > 0)
                ch->hit = ch->hit + number_range(2,4);
	    else
                ch->hit = ch->hit - number_range(1,2);
	    update_pos( ch );
            if (ch->position > POS_INCAP)
            {
                act( "$n's wounds stop bleeding and seal up.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
            }
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You clamber back to your feet.\n\r", ch );
            }
	}
	else if ( ch->position == POS_MORTAL && !is_obj && !drop_out )
	{
	    drop_out = FALSE;
	   /* if (ch->level > 0)*/
                ch->hit = ch->hit + number_range(2,4);
/*	    else
	    {
                ch->hit = ch->hit - number_range(1,2);
		if (!IS_NPC(ch) && (ch->hit <=-11))
		    do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }*/
	    if (!drop_out)
	    {
	    	update_pos( ch );
            	if (ch->position == POS_INCAP)
            	{
                    act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
                    send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
            	}
	    }
	}
	else if ( ch->position == POS_DEAD && !is_obj && !drop_out )
	{
	   update_pos(ch);
	   if (!IS_NPC(ch))
		do_killperson(ch,ch->name);
	}
	drop_out = FALSE;
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    if ( ch_save != NULL || ch_quit != NULL )
    {
	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
	    ch_next = ch->next;
	    if ( ch == ch_save )
		save_char_obj( ch );
	    if ( ch == ch_quit )
		do_quit( ch, "" );
	    continue;
	}
    }

    return;
}



/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;
	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	switch ( obj->item_type )
	{
	default:              message = "$p vanishes.";         	break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         	break;
	case ITEM_CORPSE_NPC: message = "$p decays into dust."; 	break;
	case ITEM_CORPSE_PC:  message = "$p decays into dust."; 	break;
	case ITEM_FOOD:       message = "$p decomposes.";		break;
	case ITEM_TRASH:      message = "$p crumbles into dust.";	break;
	case ITEM_EGG:        message = "$p cracks open.";		break;
	case ITEM_WEAPON:     message = "$p turns to fine dust and blows away."; break;
        case ITEM_WALL:       message = "$p flows back into the ground.";break;
	}

    if (obj->pIndexData->vnum != 30007)
    {
	if ( obj->carried_by != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, obj->carried_by, obj, NULL, TO_CHAR );
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, rch, obj, NULL, TO_ROOM );
	    act( message, rch, obj, NULL, TO_CHAR );
	}
    }

	/* If the item is an egg, we need to create a mob and shell!
	 * KaVir
	 */
	if ( obj->item_type == ITEM_EGG && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED))
	{
	    CHAR_DATA      *creature;
	    OBJ_DATA       *egg;
	    if ( get_mob_index( obj->value[0] ) != NULL )
	    {
		if (obj->carried_by != NULL && obj->carried_by->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->carried_by->in_room);
		}
		else if (obj->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->in_room);
		}
		else
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,get_room_index(ROOM_VNUM_HELL));
		}
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, creature->in_room );
	    	act( "$n clambers out of $p.", creature, obj, NULL, TO_ROOM );
	    }
	    else if (obj->in_room != NULL)
	    {
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, obj->in_room );
	    }
	}

        if (IS_OBJ_STAT2(obj, ITEM_DAEMONSEED)
	    && obj != NULL
	    && obj->in_obj == NULL
	    && (locate_obj(obj))->people )
	{
	    char buf[MAX_STRING_LENGTH];
	    CHAR_DATA *vch;
	    int wdam;

	    sprintf(buf, "%s suddenly explodes in a ball of flame, incinerating you!\n\r",obj->short_descr);
	    buf[0] = UPPER(buf[0]);
	    if ((locate_obj(obj))->people == NULL) break;
	    for ( vch = (locate_obj(obj))->people; vch != NULL;vch=vch->next_in_room )
    	    {
		if (vch->class == 0 || (!IS_NPC(vch) && vch->level < 3)) continue;
	    if ( IS_SET(vch->in_room->room_flags,ROOM_SAFE) )
	    {
		stc("You are unaffected by the blast.\n\r",vch);
		continue;
	    }
	    wdam = obj->level + dice(100,100);
	    hurt_person(vch, vch, wdam);
	    update_pos(vch);
	    if (vch->position == POS_DEAD) continue;
	    if (is_safe(vch,vch)) continue;
		send_to_char(buf, vch);
	    sprintf(buf,"The flames strike you incredibly hard![%d]\n\r",wdam);
		stc(buf,vch);
 	    }
	}

	if (obj != NULL) extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25 to 35 of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't want the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim = NULL;

    OBJ_DATA *obj = NULL;
    OBJ_DATA *chobj = NULL;
    ROOM_INDEX_DATA *objroom = NULL;
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
/*
	if ( d->connected == CON_PLAYING
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata->stage[1] > 0
	&& ( victim = ch->pcdata->partner ) != NULL
	&&   !IS_NPC(victim)
	&&   ch->in_room != NULL
	&&   victim->in_room != NULL
	&&   victim->in_room != ch->in_room )
	{
	    ch->pcdata->stage[1] = 0;
	    victim->pcdata->stage[1] = 0;
	}
*/
	if (( d->connected == CON_PLAYING
	||    d->connected == CON_EDITING )
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&& ( obj = ch->pcdata->chobj ) != NULL )
	{
	    if (obj->in_room != NULL)
	    	objroom = obj->in_room;
	    else if (obj->in_obj != NULL)
	    	objroom = get_room_index(ROOM_VNUM_IN_OBJECT);
	    else if (obj->carried_by != NULL)
	    {
		if (obj->carried_by != ch && obj->carried_by->in_room != NULL)
		    objroom = obj->carried_by->in_room;
		else continue;
	    }
	    else continue;
	    if (ch->in_room != objroom && objroom != NULL)
	    {
	    	char_from_room(ch);
	    	char_to_room(ch,objroom);
		do_look(ch,"auto");
	    }
	}
	else if (( d->connected == CON_PLAYING
	||         d->connected == CON_EDITING )
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&&   (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0) )
	{
	    if (ch->pcdata->obj_vnum != 0)
	    {
		bind_char(ch);
		continue;
	    }
	    if (IS_HEAD(ch,LOST_HEAD))
	    {
	    	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	    	send_to_char("You are able to regain a body.\n\r",ch);
	    	ch->position = POS_RESTING;
	    	ch->hit = 1;
	    } else {
	    	send_to_char("You return to your body.\n\r",ch);
		REMOVE_BIT(ch->extra,EXTRA_OSWITCH);}
	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	    if ( ( chobj = ch->pcdata->chobj ) != NULL )
		chobj->chobj = NULL;
	    ch->pcdata->chobj = NULL;
	    do_look(ch,"auto");
	}
	continue;
    }

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;
	if ( IS_NPC(wch)
	|| ( wch->desc != NULL && wch->desc->connected != CON_PLAYING && wch->desc->connected != CON_EDITING )
	||   wch->position <= POS_STUNNED
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->pcdata == NULL
	|| ( ( chobj = wch->pcdata->chobj ) != NULL )
	||   wch->in_room == NULL )
	    continue;

	if (IS_HERO(wch) )
	{
	    bool ok = FALSE;
	    if (IS_ITEMAFF(wch, ITEMA_STALKER) )
		ok = TRUE;
	    else if (IS_CLASS(wch, CLASS_WEREWOLF) &&
		wch->power[DISC_WERE_LYNX] > 1)
		ok = TRUE;
	   else if (IS_CLASS(wch, CLASS_NINJA) &&
		wch->pcdata->powers[NPOWER_NINGENNO] >= 4 )
		ok = TRUE;

	    if ( wch->hunting != NULL && wch->hunting != '\0' && 
		strlen(wch->hunting) > 1 && ok)
	    {
		ROOM_INDEX_DATA *old_room = wch->in_room;
		check_hunt( wch );
		if (wch->in_room == old_room)
		{
		    free_string(wch->hunting);
		    wch->hunting = str_dup( "" );
		    continue;
		}
		check_hunt( wch );
		if (wch->in_room == old_room)
		{
		    free_string(wch->hunting);
		    wch->hunting = str_dup( "" );
		}
		continue;
	    }
	}

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   no_attack(ch, wch)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch ) )
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   !no_attack(ch, vch)
		&&   vch->pcdata != NULL
		&& ( ( chobj = vch->pcdata->chobj ) == NULL )
		&&   vch->level < LEVEL_IMMORTAL
		&&   vch->position > POS_STUNNED
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
	    {
/*
		bug( "Aggr_update: null victim attempt by mob d.", ch->pIndexData->vnum );
*/
		continue;
	    }

	    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
    }
    return;
}


 
void embrace_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    CHAR_DATA       *victim;
    char buf[MAX_STRING_LENGTH];
    for ( d = descriptor_list; d != NULL; d = d->next )
      {
      if 
        ((ch = d->character) == NULL
        || IS_NPC(ch)
        || ch->embracing==NULL)
	{
            continue;
        }
if (!IS_CLASS(ch,CLASS_VAMPIRE))
stop_embrace(ch,NULL);
victim=ch->embracing;
if (get_char_world(ch,victim->name) ==NULL)
{stop_embrace(ch,NULL);continue;}
if (ch->in_room != victim->in_room)
stop_embrace(ch,victim);
if (victim->pcdata->condition[COND_THIRST] < 0)
victim->pcdata->condition[COND_THIRST] = 0;
        ch->pcdata->condition[COND_THIRST] += number_range(30,40);
        sprintf(buf,
"%s shudders in ecstacy as he drinks blood from %s's neck.\n\r",
ch->name,victim->name);
act(buf,ch,NULL,NULL,TO_ROOM);
sprintf(buf,
"You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->name);
send_to_char(buf,ch);
sprintf(buf,
"You feel some of your life slip away as %s drinks from your neck.\n\r",
ch->name);
send_to_char(buf,victim);
ch->pcdata->condition[COND_THIRST] += number_range(35,40);
victim->pcdata->condition[COND_THIRST] -= number_range(40,42);

if (ch->pcdata->condition[COND_THIRST] > 2000/ch->generation)
{
  ch->pcdata->condition[COND_THIRST]=2000/ch->generation;  
  send_to_char("Your bloodlust is sated.\n\r",ch);
}

if (victim->pcdata->condition[COND_THIRST] < 0)
  victim->pcdata->condition[COND_THIRST]=0;

if ((victim->pcdata->condition[COND_THIRST] ==0)
 && !IS_CLASS(victim, CLASS_VAMPIRE))
{
  sprintf(buf, "You have had all the blood drained from your body!\n\r");
  send_to_char(buf,victim);
  raw_kill(victim);

  sprintf(buf,"%s's body falls lifeless to the ground.\n\r",victim->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  send_to_char(buf,ch);
  stop_embrace(ch,victim);

  sprintf(buf,"%s has had all the blood drained from their body by %s.",
    victim->name,ch->name);

  victim->pcdata->condition[COND_THIRST]=0;
  do_info(ch,buf);
}
else
{
  sprintf(buf,"You have been diablerized!\n\r");
  send_to_char(buf,victim);
  raw_kill(victim);
  sprintf(buf,"%s's body falls lifeless to the ground.\n\r",victim->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  send_to_char(buf,ch);
  stop_embrace(ch,victim);

  if (victim->generation < ch->generation &&  ch->generation > 3)
  {
    sprintf(buf,"%s has been diablerized by %s.",victim->name,ch->name);
    ch->generation -= 1;
    victim->generation += 1;
    victim->pcdata->condition[COND_THIRST]=0;
    do_info(ch,buf);
  }
  else
  {
  sprintf(buf,"%s has been diablerized by %s for no generation.", victim->name,ch->name);
  victim->pcdata->condition[COND_THIRST]=0;
  do_info(ch,buf);
  
  }

}
continue; 
 
 
 
      }
    return;
}

void ww_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *victim;
    float            dam = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
      {
      if (!IS_PLAYING(d) || (victim = d->character) == NULL
        || IS_NPC(victim) || IS_IMMORTAL(victim)
	|| victim->in_room == NULL || victim->pcdata->chobj != NULL
        || IS_CLASS(victim,CLASS_WEREWOLF))
	{
            continue;
        }
      if ( !IS_SET( d->character->in_room->room_flags, ROOM_BLADE_BARRIER ) )
        continue;

      act( "The scattered blades on the ground fly up into the air ripping into you.", d->character, NULL, NULL, TO_CHAR );
      act( "The scattered blades on the ground fly up into the air ripping into $n.", d->character, NULL, NULL, TO_ROOM );

      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_CHAR );
      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_ROOM );

      dam = number_range( 7, 14 );
      dam = dam / 100;
      dam = d->character->hit * dam;
      if ( dam < 100 ) dam = 100;
      d->character->hit = d->character->hit - dam;
      if ( d->character->hit < -10 ) d->character->hit = -10;
      update_pos( victim );
      }

    return;
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
    static  int     pulse_shapespec;
    static  int     pulse_area;
    static  int     pulse_mobile;
    //static  int 	    pulse_bloodlust;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int     pulse_ww;
    static  int     pulse_embrace;
    static  int     pulse_gamble;

    if (--pulse_gamble <= 0)
    {
	gamble_update();
	pulse_gamble = PULSE_GAMBLE;
    }


    if ( --pulse_shapespec     <= 0 )
    {
	pulse_shapespec 	= PULSE_SHAPESPEC;
	update_shapespec( );
    }
	
    if ( --pulse_ww       <= 0 )
    {
        pulse_ww        = PULSE_WW;
        ww_update       ( );
    }

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
	pulse_area /= 2;
	area_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }
   /*
    if ( --pulse_bloodlust <= 0)
    {
	pulse_bloodlust = PULSE_BLOODLUST;
        bloodlust_update ( );
    }
*/
    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
	newviolence_update ( );
    }
    if ( --pulse_embrace <= 0)
    {
        pulse_embrace = PULSE_EMBRACE;
        embrace_update ( ); 
    }
    if ( --pulse_point    <= 0 )
    {
	pulse_point     = number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 );
	weather_update	( );
	char_update	( );
	obj_update	( );
        room_update	( );
    }

    aggr_update( );
    tail_chain( );
    return;
}

void smooregen(CHAR_DATA *ch, int hp, int mana, int move)
{
int a;
if (ch->hit > 0 && ch->glorify == GLOR_ZARAF) {hp*=2;mana*=2;move*=2;}
ch->hit += hp;
ch->mana += mana;
ch->move += move;
if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
if (ch->move > ch->max_move) ch->move = ch->max_move;
for (a = 1; a < 7; a++)
	ch->loc_hp[a] = 0;
update_pos(ch);
return;
}

void smooregen2(CHAR_DATA *ch, int hp, int mana, int move, int cap)
{
int a;
if (ch->hit > 0 && ch->glorify == GLOR_ZARAF) {hp*=2;mana*=2;move*=2;}
ch->hit += hp;
ch->mana += mana;
ch->move += move;
if (ch->hit > ch->max_hit+cap) ch->hit = ch->max_hit+cap;
if (ch->mana > ch->max_mana+cap) ch->mana = ch->max_mana+cap;
if (ch->move > ch->max_move+cap) ch->move = ch->max_move+cap;
for (a = 1; a < 7; a++)
        ch->loc_hp[a] = 0;
update_pos(ch);
return;
}
