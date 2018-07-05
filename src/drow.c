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
#include "player.h"


void	check_killer	args ( (CHAR_DATA *ch, CHAR_DATA *victim) );
/*
 * Local functions.
 */

void do_throttle (CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int dam;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DROW) || (!IS_SET(ch->special,
SPC_DROW_WAR)))
	{
		send_to_char("You wave your arms in a motion trying to throttle someone and fail.\n\r", ch);
		return;
	}
	if (ch->pcdata->stats[DROW_POWER] < 2500)
	{
		send_to_char("You do not have the necessary power to throttle.(2500)\n\r", ch);
		return;
	}
	else if (ch->move < 6000)
	{
		stc("You need 6k movement points to use that.\n\r", ch);
		return;
	}
	if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 4 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL  || obj->value[3] != 4 ) )
    {
        send_to_char( "You need to wield a whip.\n\r", ch );
        return;
    }
	if (!*argument || (victim = get_char_room(ch,argument)) == NULL)
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}
	dam = ch->hitroll/14;
	dam *= number_range(6,9);
	dam *= 10;
	if (number_range(1,3)==1) {
		act("$N struggles and escapes the end of your strangle.", ch, 0, victim, TO_CHAR);
		act("You struggle and escape the end of $N's strangle.", ch, 0, victim, TO_VICT);
		act("$n struggles and escapes the end of $N's strangle.", ch, 0, victim, TO_NOTVICT);
		dam /= 2;
	}

	ch->pcdata->stats[DROW_POWER] -= 2500;
	if ( dam <= 0 )
		dam = 1;
	if (!ch->fighting) set_fighting(ch,victim);
	sprintf(buf,"Your strangling move strikes $N [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n's strangling move strike you [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	sprintf(buf,"$n's strangling move strikes $N [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	victim->hit -= dam;
	ch->move -= 6000;
	update_pos(victim);

	WAIT_STATE(ch,PULSE_VIOLENCE*.9);
	return;
}
/* Drow thing to give all drow points to the matron or Lloth's Avatar */
void do_supplicate ( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg );

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DROW))
	{
		send_to_char("Excuse me?\n\r", ch);
		return;
	}
      if (ch->generation == 1)
	{
		send_to_char("You are Lloth's avatar where the hell are you giving your points to?\n\r", ch);
		return;
}
	if (arg[0] == '\0')
	{
		send_to_char("Supplicate to who?\n\r",ch);
		return;
	}
      if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
		send_to_char( "Nobody by that name.\n\r", ch );
		return;
    }

		if (IS_NPC(victim))
	{
		send_to_char("You may not supplicate a mob!\n\r", ch);
		return;
	}
	if (ch == victim)
	{	
		send_to_char("You cannot do that\n\r", ch);
		return;
	}
    	if (victim->generation > 2)
	{
		send_to_char("You may only supplicate to matron mothers and Lloth's avatar", ch);
		return;
	}
	if (ch->pcdata->stats[DROW_POWER] < 1)
	{
		send_to_char("You have no drow points of power to supplicate.\n\r", ch);
		return;
	}
	send_to_char("You have supplicated all of your drow power.\n\r", ch);
	send_to_char("You have been strengthened by one of your follwers.\n\r", victim);
	victim->pcdata->stats[DROW_POWER] += ch->pcdata->stats[DROW_POWER];
	ch->pcdata->stats[DROW_POWER] = 0;
	return;
}

void do_grant( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DROW))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Grant <person> <power>\n\r", ch );
	send_to_char("Drowfire (2500),  Darkness (7500),  Drowpoison (2500).\n\r",ch);
	send_to_char("Drowsight (5000),  Drowshield (5000),  Levitation (1000).\n\r", ch );
	send_to_char("Shadowwalk (10000),  Garotte (5000),  Spiderarms (7500).\n\r", ch );
	send_to_char("Drowhate (10000),  Spiderform (15000),  Web (5000).\n\r",ch );
	send_to_char("Dgarotte (2500), Circle. (1000)\n\r", ch );
	if (ch->generation < 3) {
	send_to_char("Warrior (0), Mage (0), Cleric (0).\n\r", ch );}
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }
	if (str_cmp(victim->clan,ch->clan) && ch->generation >1) {
		send_to_char("Only on your clan!.\n\r", ch );
		return;}

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }
    if (!str_cmp(arg2,"mage") || !str_cmp(arg2,"warrior") || !str_cmp(arg2, "cleric" ))
    {
	if (victim->generation < 2 )
	{	send_to_char("Not on them!\n\r", ch );
		return;
	}
	if(IS_IMMORTAL(victim))
	{
		send_to_char("Not on an Immortal.\n\r", ch);
		return;
	}
	if (!IS_CLASS(victim, CLASS_DROW))
	{
		send_to_char("Only on drows.\n\r", ch );
		return;
	}
/*	if (ch->generation != 2) {
		send_to_char("Only for matrons.\n\r", ch );
		return; }
*/
	if (IS_SET(victim->special, SPC_DROW_WAR) || IS_SET(victim->special, SPC_DROW_MAG) ||IS_SET(victim->special, SPC_DROW_CLE))
	{		
		send_to_char( "They already have a profession.\n\r", ch );
		return;
	}
	if (!str_cmp(arg2, "mage"))
		SET_BIT(victim->special, SPC_DROW_MAG);
	else if (!str_cmp(arg2, "cleric"))
		SET_BIT(victim->special, SPC_DROW_CLE);
	else if (!str_cmp(arg2, "warrior"))
		SET_BIT(victim->special, SPC_DROW_WAR);
	save_char_obj(victim);
	return;
    }


    if (!str_cmp(arg2,"drowfire")) 
	{inpart = DPOWER_DROWFIRE; cost = 2500;}
    else if (!str_cmp(arg2,"darkness"))
	{inpart = DPOWER_DARKNESS; cost= 7500;}
    else if (!str_cmp(arg2,"drowsight"))
	{inpart = DPOWER_DROWSIGHT; cost= 5000;}
    else if (!str_cmp(arg2,"spiderarms"))
	{inpart = DPOWER_ARMS; cost = 7500;}
    else if (!str_cmp(arg2,"web"))
	{inpart = DPOWER_WEB; cost = 5000;}
    else if (!str_cmp(arg2,"spiderform"))
	{inpart = DPOWER_SPIDERFORM; cost = 15000;}
    else if (!str_cmp(arg2,"drowhate"))
	{inpart = DPOWER_DROWHATE; cost = 10000;}
    else if (!str_cmp(arg2,"drowshield"))
    {inpart = DPOWER_DROWSHIELD; cost = 5000;}
    else if (!str_cmp(arg2,"levitation"))
	{inpart = DPOWER_LEVITATION; cost = 1000;}
    else if (!str_cmp(arg2,"shadowwalk"))
	{inpart = DPOWER_SHADOWWALK; cost = 10000;}
    else if (!str_cmp(arg2,"garotte")) 
	{inpart = DPOWER_GAROTTE; cost = 5000;}
    else if (!str_cmp(arg2,"dgarotte"))
	{inpart = DPOWER_DGAROTTE; cost = 2500;}
    else if (!str_cmp(arg2,"drowpoison"))
	{inpart = DPOWER_DROWPOISON; cost = 2500;}
    else if (!str_cmp(arg2,"circle"))
	{inpart = DPOWER_CIRCLE; cost = 1000;}
    else
    {
		send_to_char("Please select a power from:\n\r",ch);
		send_to_char("Drowfire (2500),  Darkness (7500),  Drowpoison  (1000).\n\r",ch);
		send_to_char("Drowsight (5000),  Drowshield (5000),  Levitation (1000).\n\r", ch );
		send_to_char("Shadowwalk (10000),  Garotte (5000),  Spiderarms (7500).\n\r", ch );
		send_to_char("Drowhate (10000),  Spiderform (15000),  Web (5000).\n\r",ch );
		send_to_char("Dgarotte (2500) Circle (1000).\n\r", ch);
        if (ch->generation < 3)
		{
			send_to_char("Warrior (0), Mage (0), Cleric (0).\n\r", ch );}
			return;
		}
    if (IS_SET(victim->pcdata->powers[1], inpart))
    {
		send_to_char("They have already got that power.\n\r",ch);
		return;
    }
    if (ch->pcdata->stats[DROW_POWER] < cost) 
    {
		send_to_char("You have insufficient power to grant that gift.\n\r",ch);
		return;
    }
    SET_BIT(victim->pcdata->powers[1], inpart);
    ch->pcdata->stats[DROW_POWER]   -= cost;
    if (victim != ch) send_to_char("You have been granted a gift from your matron!\n\r",victim);
		send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
		save_char_obj(victim);
    return; 
}

void do_chaosblast(CHAR_DATA *ch, char *argument) 
{

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DROW))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (!IS_SET(ch->special, SPC_DROW_MAG))
	{
	if (ch->generation > 2)
	{
		send_to_char("Drow mages/matrons and Lloths Avatar only.\n\r", ch);
		return;
	}
	}

    if (!*argument || (victim = get_char_room(ch,arg))==NULL) victim = ch->fighting;
    if (!victim)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
    if (ch->mana < 100) 
	{
		send_to_char("You don't have enough mana.\n\r", ch);
        return;
	}
    if ( ( sn = skill_lookup( "chaos blast" ) ) < 0 )
		return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 1.1;
    level = level * 2;
    act("You concentrate your power on $N.",ch,NULL,victim,TO_CHAR);
    act("$n concentrates $s power on you.",ch,NULL,victim,TO_VICT);
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, PULSE_VIOLENCE );
    ch->mana = ch->mana - 100;
    return;
}


void do_drowcreate( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DROW))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
		send_to_char("Whip, Sword, Dagger, Piwafwi, Armor, Boots, Mask, Bracers.\n\r", ch );
		return;
    }
    if      (!str_cmp(arg,"whip"     )) vnum = 29606;
    else if (!str_cmp(arg,"piwafwi"   )) vnum = 29601;
    else if (!str_cmp(arg,"armor"    )) vnum = 29602;
    else if (!str_cmp(arg,"boots"   )) vnum = 29603;
    else if (!str_cmp(arg,"mask" )) vnum = 29604;
    else if (!str_cmp(arg,"bracers"    )) vnum = 29605;
    else if (!str_cmp(arg,"dagger"	)) vnum = 29607;
    else if (!str_cmp(arg,"sword"       )) vnum = 29600;
    else
    {
	send_to_char("That is an invalid type.\n\r", ch );
	return;
    }
    if ( ch->pcdata->stats[DROW_POWER] < 7500)
    {
	send_to_char("It costs 7500 points of power to create a piece of drow armour.\n\r",ch);
	return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform an Implementor.\n\r",ch);
	return;
    }
    ch->pcdata->stats[DROW_POWER] -= 7500;
    obj = create_object(pObjIndex, 50);
    obj->cost = 7500;
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_favor( CHAR_DATA *ch, char *argument )
{
    char      buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DROW) || ch->generation > 2) 
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Give Lloth's favor to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char("Not on yourself.\n\r", ch );
	return;
    }

    if (victim->class != 0 || IS_SET(victim->special, SPC_CHAMPION)) {
	send_to_char("You can't make a classed person a Drow.\n\r", ch);
	return;}

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "Only on avatars.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_DEMON))
    {
	send_to_char( "Not on an unwilling person.\n\r", ch);
	return;
    }

    if (ch->exp < 10000)
    {
	send_to_char("You cannot afford the 10,000 exp.\n\r",ch);
	return;
    }

    ch->exp = ch->exp - 10000;
    act("You make $N a drow!", ch, NULL, victim, TO_CHAR);
    act("$n makes $N a drow!", ch, NULL, victim, TO_NOTVICT);
    act("$n makes you a drow!", ch, NULL, victim, TO_VICT);
    victim->class=0;
    victim->class=CLASS_DROW;
    victim->special=0;
    victim->generation=3;

    if (IS_CLASS(victim, CLASS_VAMPIRE)) do_mortalvamp(victim,"");
    REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
    REMOVE_BIT(victim->act, PLR_WIZINVIS);
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_ANARCH);
    victim->rage = 0;
    victim->generation = ch->generation + 1;
	victim->special = 0;
    free_string(victim->morph);
    victim->morph=str_dup("");
                free_string(victim->clan);
            victim->clan=str_dup(ch->clan);

    free_string(victim->lord);
    if (ch->generation == 1)
        victim->lord=str_dup(ch->name);
    else
    {
        sprintf(buf,"%s %s",ch->lord,ch->name);
        victim->lord=str_dup(buf);
    }
 
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

/* Ahem, the plural of drow is drow */

void do_drows( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char lord[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DROW))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (ch->generation < 3)
		strcpy(lord,ch->name);
	else strcpy(lord,ch->lord);
    sprintf( buf, "The drow followers of %s:\n\r", lord );
    send_to_char( buf, ch );
    send_to_char("&G[      Name      ] [ Hits ] [ Mana ] [ Move ] [  Exp  ] [   Power  ]#n\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_DROW)) 
	    continue;
	if ( !str_cmp(ch->lord,lord) || !str_cmp(ch->name,lord))
	{
	    sprintf( buf,
	    "&G[#n%-16s&G] [#n%-6d&G] [#n%-6d&G] [#n%-6d&G] [#n%7ld&G] [ #n%-9d &G]#n\n\r",
		capitalize( gch->name ),
		gch->hit,gch->mana,gch->move,
		gch->exp, gch->pcdata->stats[DROW_POWER]);
		send_to_char( buf, ch );
	}
    }
    return;
}

void do_drowfire( CHAR_DATA *ch, char *argument ) {

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DROWFIRE))
	{
		send_to_char("Huh?\n\r", ch );
		return;
	}

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }

	if (ch->mana < 100) {
	send_to_char("You don't have enough mana.\n\r", ch );
		return;}

    if ( ( sn = skill_lookup( "drowfire" ) ) < 0 )
		return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
	ch->mana = ch->mana - 100;
    return;
}

void do_heal ( CHAR_DATA *ch, char *argument ) 
{
    if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DROW) ) 
    {
		send_to_char("Huh?\n\r", ch );
		return;
    }

	if (ch->mana < 300)
	{
		send_to_char("You don't have enough mana.\n\r", ch );
		return;
	}
	if (ch->position == POS_FIGHTING)
	{
		if (ch->pcdata->stats[DROW_POWER] < 50)
		{
			send_to_char("You do not have the necessary power to heal, while you are fighting.\n\r",ch);
			return;
		}
	}
	ch->mana = ch->mana - 300;
	if (ch->position == POS_FIGHTING)
		ch->pcdata->stats[DROW_POWER] -= 50;


	if (IS_SET(ch->special, SPC_DROW_CLE))
		ch->hit += number_range(750,800);
	else if (IS_SET(ch->special, SPC_DROW_MAG))
		ch->hit += number_range(600, 650);
	else if (IS_SET(ch->special, SPC_DROW_WAR))
		ch->hit += number_range(500,650);
	else
		ch->hit += number_range(550,600);

    if (!ch->fighting && ch->tier >= 2)
	ch->hit += 600;

	if (ch->hit > ch->max_hit)
		ch->hit = ch->max_hit;

	send_to_char("The power of Lloth encompasses you.\n\r",ch);
	act("$n is surrounded by the power of Lloth.",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch, 8);
	return;
}

void do_shadowwalk(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_SHADOWWALK)) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Shadowwalk to whom?\n\r", ch );
	return;}

	location = victim->in_room;

	if (ch->move < 500) {
	send_to_char("You are too tired to go shadowwalking.\n\r", ch );
	return;}
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
	if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
	{
		send_to_char("You cannot transport inside the arena.\n\r", ch);
		return;
	}

	if (ch->fight_timer > 0)
        {
        send_to_char("Not with a fight timer!\n\r",ch);
        return;
        }

/*	if (!IS_NPC(victim)) {
	send_to_char("You failed.\n\r", ch );
	return;}
*/

	if (!IS_NPC(victim))
	{
	if (ch->max_move < victim->max_move)
	{send_to_char("Get more move!\n\r",ch);
	return;}
	}
/*
	if (!IS_SET(victim->in_room->room_flags, ROOM_DARK) && 
	!IS_SET(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS)) {
	send_to_char("You failed.\n\r", ch );
	return;}
*/
	act("You walk into the shadows.", ch, NULL, NULL, TO_CHAR);
	act("$n walks into the shadows.", ch, NULL, NULL, TO_ROOM);
	ch->move -= 500;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");	
	act("You walk out of the shadows.", ch, NULL, NULL, TO_CHAR);
	act("$n walks out of the shadows.", ch, NULL, NULL, TO_CHAR);

	return;
}

void do_drowhate(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_DROWHATE)) {
	send_to_char("You calm your hatred.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_DROWHATE);
	}
	else {
	send_to_char("You invoke your hatred for others.\n\r", ch );
	SET_BIT(ch->newbits, NEW_DROWHATE);
	}

	return;
}

void do_spiderform( CHAR_DATA *ch, char *argument ) 
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
/* Not currently in use
	OBJ_DATA *obj;
        OBJ_DATA *obj_next;
*/
 
	argument = one_argument(argument,arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_AFFECTED(ch, AFF_POLYMORPH)) 
      {
	send_to_char("You can't spiderform while changed.\n\r", ch );
	return;
      }

      if (ch->cur_form != get_normal_form(ch))
      {
		sprintf(buf, "$n morphs back into %s.", GET_PROPER_NAME(ch));
        act(buf, ch, NULL, NULL, TO_ROOM);
        stc("You return to your normal form.\n\r", ch);
        set_form(ch, get_normal_form(ch) );
        WAIT_STATE(ch, 7);
        return;   
      }

	if (ch->move < 500) {
	send_to_char("You don't have the energy to change.\n\r", ch );
	return;}


      if (ch->cur_form == get_normal_form(ch))
      {
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	ch->move -= 500;
	
	act("You mutate into a giant spider.",ch,NULL,NULL,TO_CHAR);
        act("$n mutates into a giant spider.",ch,NULL,NULL,TO_ROOM);
        set_form(ch, FRM_DROWSPIDER);
	return;
    }

	return;
}


void do_drowsight(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSIGHT)) {
	send_to_char("Huh?\n\r", ch );
	return;}

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {   
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your senses increase to incredible proportions.\n\r", ch);
    }

	return;
}

void do_drowshield(CHAR_DATA *ch, char *argument)
{

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD)) {
	send_to_char("Huh?\n\r", ch );
	return;}

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
        send_to_char("You shield your aura from those around you.\n\r",ch);
        SET_BIT(ch->immune, IMM_SHIELDED);
        return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);


	return;
}


void do_drowpowers(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW))  {
	send_to_char("Huh?\n\r", ch );
	return;}

	send_to_char("&CThe powers Lloth has bestowed upon you...#n\n\r", ch );
	send_to_char("&B-----------------------------------------#n\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWFIRE))
	send_to_char("You can call Drowfire upon your enemies.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS))
	send_to_char("You can summon a globe of darkness.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWSIGHT))
	send_to_char("You can use enhanced sight.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))
	send_to_char("You can levitate in the air.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD))
	send_to_char("You can shield your aura.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
	send_to_char("You have control over deadly poisons.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWWALK))
	send_to_char("You can walk through the shadows.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)) 
	send_to_char("You have mastered the art of garotte.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE))
	send_to_char("You have dark-enhanced garotte.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_ARMS))
	send_to_char("You have extra spidery arms.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE))
	send_to_char("You can invoke the hatred of the Drow.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM))
	send_to_char("You can mutate into a large spider.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_WEB))
	send_to_char("You can shoot a spidery web at opponents.\n\r", ch);

	if (IS_SET(ch->pcdata->powers[1], DPOWER_CIRCLE))
	send_to_char("You can circle your opponents in combat.\n\r", ch);

	send_to_char("\n\r", ch);
	
	if (ch->generation == 1)
	send_to_char("You are Lloth's Avatar.\n\r", ch );
	
	if (ch->generation == 2)
	send_to_char("You are a Drow Matron.\n\r", ch );
	
	if (IS_SET(ch->special, SPC_DROW_WAR))
	send_to_char("You are a Drow Warrior.\n\r", ch);

	if (IS_SET(ch->special, SPC_DROW_MAG))
	send_to_char("You are a Drow Mage.\n\r", ch );

	if (IS_SET(ch->special, SPC_DROW_CLE))
	send_to_char("You are a Drow Cleric.\n\r", ch );

        sprintf( buf, "You have %d drow power points!.\n\r",
        ch->pcdata->stats[DROW_POWER] );
        send_to_char( buf, ch );

        sprintf( buf, "You have %d points of magic resistance.\n\r", 
        ch->pcdata->stats[DROW_MAGIC] );
        send_to_char( buf, ch );

        if (weather_info.sunlight == SUN_DARK)
        send_to_char("You feel strong in the night.\n\r", ch );

	send_to_char("\n\r", ch );
	
	return;
}


void do_darkness(CHAR_DATA *ch, char *argument) 
{
	bool blah = FALSE;

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS)) {
	do_vampdarkness(ch,argument);
	return;}

	if (ch->fight_timer > 0)
	{
	    send_to_char("Not with an active fight timer!\r\n", ch);
	    return;
	}
	if (IS_SET(ch->newbits, NEW_DARKNESS)) {
	send_to_char("You banish your globe of darkness.\n\r", ch );
	act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_DARKNESS);
	if (ch->in_room != NULL)
	if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	return;

	}

/*      if (ch->fight_timer > 0) {
        send_to_char("You may not summon darkness with a fight timer.\n\r", ch );
        return;} */
  
        if (weather_info.sunlight != SUN_DARK)
        {
        send_to_char("You may not summon the darkness during the day.\n\r", ch);
        return;
        }

	if (ch->mana < 500) {
	send_to_char("You don't have enough mana to summon the darkness.\n\r",ch );
	return;}

	send_to_char("You summon a globe of darkness.\n\r", ch );
	act("$n summons a globe of darkness.",ch,NULL,NULL,TO_ROOM);
	ch->mana -= 500;
	if (IS_SET(ch->extra, TIED_UP)) {
		REMOVE_BIT(ch->extra, TIED_UP);
		blah = TRUE;}

	if (IS_AFFECTED(ch, AFF_WEBBED)) {
		REMOVE_BIT(ch->affected_by, AFF_WEBBED);
		blah = TRUE;}

	if (blah) send_to_char("The darkness sets you free.\n\r", ch );		

	SET_BIT(ch->newbits, NEW_DARKNESS);
	if (!IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	return;

        
        if (IS_SET(ch->newbits, NEW_DARKNESS) &&
        (weather_info.sunlight != SUN_DARK)) 
        send_to_char("The darkness leaves you.\n\r", ch);
        act("The globe of darkness around $n fades.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->newbits, NEW_DARKNESS);
        if (ch->in_room != NULL)
        if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
        REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
        return; 
}

void do_circle( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *victim;

    OBJ_DATA *obj;

//    int chance = 75; /* 75 base percentage */


    one_argument( argument, arg );

 
if (!will_use(ch, "Circle")) {
 	if ( !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_NINJA) )

 	{

 	  send_to_char("Huh?\n\r", ch);

 	  return;

 	}
      if(IS_CLASS(ch, CLASS_DROW) &&
	!IS_SET(ch->pcdata->powers[1],DPOWER_CIRCLE))
	{
		send_to_char("You have not been granted the power to circle!\n\r", ch );
		return;
	}
 
}
    if (arg[0] == '\0')

    {

        send_to_char("Circle whom?\n\r",ch);

        return;

    }

 

    else if ((victim = get_char_room(ch,arg)) == NULL)

    {

        send_to_char("They aren't here.\n\r",ch);

        return;

    }

 

    if ( is_safe( ch, victim ) )

      return;

 

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL

    ||   obj->value[3] != 11 )

    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL

    ||   obj->value[3] != 11 ) )

    {
      if (!IS_SLAIYTH(ch)) {
	send_to_char( "You need to wield a piercing weapon.\n\r", ch );

	return;}

    }

    if ( ( victim = ch->fighting ) == NULL )

    {

        send_to_char( "You must be fighting in order to circle.\n\r", ch );

        return;

    }  

    check_killer( ch, victim );

    WAIT_STATE( ch, skill_table[gsn_circle].beats );
/*
    chance += get_curr_dex(ch) ;

    chance -= get_curr_dex(victim);

    if ( !IS_AWAKE(victim) || IS_NPC(ch)

    || number_percent( ) < chance )
*/
    {int b;
     int max = IS_SLAIYTH(ch)?number_range(3,4):1;
	    for (b = 0; b < max; b++)
		multi_hit( ch, victim, gsn_circle );

    }
//    else

//		damage( ch, victim, 0, gsn_circle );

    

    return;

}
/*LLothbless by bry to make up for spiderform with no bonus*/
 void do_llothsbless( CHAR_DATA *ch, char *argument )
 {
    int sn;
    int spelltype;

    if ( IS_NPC(ch)) return;
 
  	if (!IS_CLASS(ch, CLASS_DROW))
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
 
 	if (ch->mana < 500) 
  	{
	send_to_char("You don't have enough mana.\n\r", ch );
	return;
       }	

    if ( ( sn = skill_lookup( "llothbless" ) ) < 0 ) return;

      spelltype = skill_table[sn].target;

    (*skill_table[sn].spell_fun) ( sn, 300, ch, ch );

    WAIT_STATE( ch, 12 );

    ch->mana = ch->mana - 500;

    return;

}

/* smoo's clan thing */
void do_clanship(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL], arg2[MIL];
 char *full;

 if (!IS_CLASS(ch,CLASS_DROW))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 if (ch->generation > 3)
 {
    stc("You are not powerful enough!\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg1);
 full = argument;
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {
    stc("You may choose out of clanship (a drow) (remove/give)\n\r",ch);
    return;
 }

 if ((vict = get_char_room(ch, arg1)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 if (!IS_CLASS(vict,CLASS_DROW))
 {
    stc("They aren't a drow!\n\r", ch);
    return;
 }

 if (vict->generation < ch->generation)
 {
    stc("They are too powerful for you!\n\r", ch);
    return;
 }

 if (str_cmp(arg2, "remove") && str_cmp(arg2, "give"))
 {
    stc("Choose out of the commands \"clanship (player) (Give/Remove)\"\n\r", ch);
    return;
 }

 if (!str_cmp(arg2, "give"))
 {
    free_string(vict->clan);
    vict->clan = str_dup(ch->clan);
    act("$n inducts $N into $t!",ch,ch->clan,vict,TO_NOTVICT);
    act("You induct $N into $t!",ch,ch->clan,vict,TO_CHAR);
    act("$n inducts you into $t!",ch,ch->clan,vict,TO_VICT);
 }
 else if (!str_cmp(arg2, "remove"))
 {
    free_string(vict->clan);
    vict->clan = str_dup("Rogue");
    act("$n outcasts $N from $t!",ch,ch->clan,vict,TO_NOTVICT);
    act("You outcast $N from $t!",ch,ch->clan,vict,TO_CHAR);
    act("$n outcasts you from $t!",ch,ch->clan,vict,TO_VICT);
 }
 else {do_clanship(ch,"");return;}

 stc("Done.\n\r", ch);
 return;
}

