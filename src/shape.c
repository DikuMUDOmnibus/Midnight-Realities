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


void do_shift(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict;	
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int sn;

	argument = one_argument(argument, arg2);	
	argument = one_argument(argument, arg3);	

	if (!IS_CLASS(ch, CLASS_SHAPE))
	{ send_to_char("Your not a shapeshifter!\n\r", ch);return;}

    if (arg2[0] == '\0')
    {
	send_to_char("Please enter a mobs name, back, or list.\n\r", ch);
	return;
    }
    if (!str_cmp(arg2, "list"))
	{for (sn = 1;sn < MAX_SHAPE;sn++) {
	if (shape_skill[sn].name != NULL) {
	sprintf(buf, "%-23s %d\n\r", shape_skill[sn].name, shape_skill[sn].percent);
	send_to_char(buf, ch);}
	}
	return;}
    if (!str_cmp(arg2, "back"))
	{send_to_char("You shift back into your normal form.\n\r", ch);
	 act("$n shifts back into $s normal form.", ch, NULL, NULL, TO_ROOM);
	 ch->shapespec = NULL;
	 ch->shape[dopplevnum] = 0;
	 return;
	}
if ( ( vict = get_char_room( ch, arg2) ) == NULL ) 
 { send_to_char("It's not here!\n\r", ch);return;}
if (!IS_NPC(vict)) {send_to_char("Not on a player!\n\r", ch);return;}
ch->shape[dopplevnum] = vict->pIndexData->vnum;
ch->shapespec	      = vict->spec_fun;
ch->shape[doppleweapon] = get_weapon_type(vict, get_eq_char(vict, WEAR_WIELD));
sprintf(buf, "%s: %s", ch->name, vict->short_descr);
sprintf(buf2, "You shift into %s\n\r", buf);
send_to_char(buf2, ch);
sprintf(buf2, "$n shifts into %s.\n\r", buf);
act(buf2, ch, NULL, NULL, TO_ROOM);
return;
}


void do_toggle(CHAR_DATA *ch, char *argument)
{
char arg1[MAX_INPUT_LENGTH];
argument = one_argument(argument, arg1);

if (!IS_CLASS(ch, CLASS_SHAPE))
{stc("Huh?\n\r", ch);return;}
if (arg1[0] == '\0') {
	send_to_char("Choose mob 'specs' or 'autodecap'\n\r", ch);
	return;
	}
if (!str_cmp(arg1, "specs")) {
	if (!IS_CLASS(ch, CLASS_SHAPE)) {
		send_to_char("You are not a shapeshifter.\n\r", ch);
		return;}
	if (ch->shape[spectoggle] != TRUE) ch->shape[spectoggle] = TRUE;
	else if (ch->shape[spectoggle] == TRUE) ch->shape[spectoggle] = FALSE;
 
	if (ch->shape[spectoggle] == TRUE)
		send_to_char("Specs toggled ON.\n\r", ch); 
	if (ch->shape[spectoggle] == FALSE)
		send_to_char("Specs toggled OFF.\n\r", ch);
	return;
	}
if (!str_cmp(arg1, "autodecap"))
	{
	if (!IS_CLASS(ch, CLASS_SHAPE)) {
		send_to_char("You are not a shapeshifter.\n\r", ch);
		return;}
	if (ch->shape[autodecap] != TRUE) ch->shape[autodecap] = TRUE;
	else if (ch->shape[autodecap] == TRUE) ch->shape[autodecap] = FALSE;
 
	if (ch->shape[autodecap] == TRUE)
		send_to_char("Autodecap toggled ON.\n\r", ch); 
	if (ch->shape[autodecap] == FALSE)
		send_to_char("Autodecap toggled OFF.\n\r", ch);
	return;
	}
return;	
}


const   struct  shapeskill_type      shape_skill     [MAX_SHAPE]     =
{
  {"Enhanced Sword", 	9,	9,	9, 	NULL,	70,	NULL},
  {"Enhanced Dagger", 	9,	9, 	9, 	NULL,	70, 	NULL},
  {"Enhanced Whip", 	9, 	9, 	9,	NULL,	70, 	NULL},
  {"Enhanced Mace", 	9, 	9, 	9, 	NULL,	70, 	NULL},
  {"Enhanced Unarmed",  9, 	9, 	9, 	NULL,	70, 	NULL},
  {"Enhanced FireArm", 	9, 	9, 	9, 	NULL,	70, 	NULL},
  {"Spirit", 		2449,	2450, 0, 	NULL,	80, 	NULL},
  {"Enhanced Dodge",	2411,	8123,	9405,	NULL,	70,	NULL},
  {"UltraVision", 	6307,	6301, 	0, 	NULL,	40, 	NULL},
  {"ShiftGate", 	2318, 	25015,	0,	NULL,	20,	NULL},
  {"Fast Healing", 	2803, 	2226, 	2802,	NULL,	35,	NULL},
  {"Scry",		1352, 	0,	0,	NULL,	20,	NULL},
  {"Circle",		1101,	18036,	0,	NULL,	40,	NULL},
  {"Rock-Like Skin",	9229,	3413,	0,	NULL,	70,	NULL},
  {"Immobility",	8306, 	0,	0,	NULL,	70,	NULL},
  {"Superior Knowledge",2603, 	0, 	0, 	NULL, 	25,	NULL},
  {"Speed",		18036,	23400,	0,	NULL,	50,	NULL},
  {"Super Speed",	23400,	0,	0,	NULL,	55,	NULL},
  {"Lightning Speed",	18036,	0,	0,	NULL,	60,	NULL},
  {"Unnatural Strength",9309,	0,	0,	NULL,	60,	NULL},
  {"Enhanced Damage",	6506,	8320,	0,	NULL,	60,	NULL},
  {"AutoDecapitate",	2300,	0,	0,	NULL,	15,	NULL},
  {"Warcry",		4051,	2325,	10412,	NULL,	20,	NULL},
  {"Leave Without Notice",2604,	8515,	0,	NULL,	30,	NULL},
  {"Double Heal",	9900,	401,	14904,	NULL,	40,	NULL},
  {"Double Bullet",	3005,	2332,	8515,	NULL,	30,	NULL},
  {"Unbelieveable Power",901,	0,	0,	NULL,	40,	NULL},
  {"Enhanced Stealth",	11011,	11010,	6303,	NULL,	20,	NULL},
  {"Rejuvination",	2226,	0,	0,	NULL,	5,	NULL},
  {"Randomize Weapon",	5106,	0,	0,	NULL,	20,	NULL},
  {"Enhanced Kick",	14113,	0,	0,	NULL,	35,	NULL},
  {"Darkland",		1300,	0,	0,	NULL,	90,	NULL},
  {"Slaiyth",		11331,	0,	0,	NULL,	35,	NULL}
};


void do_talents(CHAR_DATA *ch, char *argument)
{
    int count;
    int sn;
    int usable;
    char buf[MAX_STRING_LENGTH];
    char prefix[5];

    buf[0] = '\0';
count = 0;  
    for (sn = 0; sn < MAX_SHAPE; sn++ )
    {
	usable = shape_skill[sn].percent;
	if (ch->pcdata->shape[sn] > (usable - 1)) sprintf(prefix, "&R*&C");
	if (ch->pcdata->shape[sn] < usable) sprintf(prefix, "&M=&C");
      if (shape_skill[sn].name != NULL && can_get(ch, sn) && can_use(ch, shape_skill[sn].name))
	{
	count++;
	if (count == 1) {
	sprintf(buf, "&W%-20s %s%-3d ", shape_skill[sn].name,
prefix, ch->pcdata->shape[sn]);}
	if (count == 2) {
	sprintf(buf, "&W%-20s %s%-3d ", shape_skill[sn].name,
prefix, ch->pcdata->shape[sn]);}
	if (count == 3) {
	sprintf(buf, "&W%-21s %s%-3d\n\r", shape_skill[sn].name,
prefix, ch->pcdata->shape[sn]);count = 0;}

	send_to_char(buf, ch);
        }
    }
sn = 0;
if (ch->shapespec != 0) {
    for (sn = 0; *spec_table[sn].spec_fun; sn++)
{       if (spec_table[sn].spec_fun == ch->shapespec) 
	break;
}}
	if (ch->shapespec != NULL){ send_to_char("\n\r&YYou are holding special emotions inside you.#n\n\r", ch);sprintf(buf, " %s", spec_table[sn].name);send_to_char(buf, ch);} 
return;
}
	

bool can_get(CHAR_DATA *ch, int sn)
{
if (able_to_use(ch, sn)) return TRUE;
return FALSE;
}

int get_weapon_type(CHAR_DATA *ch, OBJ_DATA *obj)
{
	if (!obj) return UNARMED;
	if (obj->value[3]== 0) return UNARMED;
	if (obj->value[3]== 1) return SWORD;
	if (obj->value[3]== 2) return DAGGER;
	if (obj->value[3]== 3) return SWORD;
	if (obj->value[3]== 4) return WHIP;
	if (obj->value[3]== 5) return UNARMED;
	if (obj->value[3]== 6) return FIREARM;
	if (obj->value[3]== 7) return MACE;
	if (obj->value[3]== 8) return MACE;
	if (obj->value[3]== 9) return FIREARM;
	if (obj->value[3]==10) return UNARMED;
	if (obj->value[3]==11) return DAGGER;
	if (obj->value[3]==12) return UNARMED;
    return UNARMED;
}

bool can_use(CHAR_DATA *ch, char *skill_name)
{
    int sn;
	
	if (!IS_CLASS(ch, CLASS_SHAPE)) return FALSE;

	for (sn = 0; sn  < MAX_SHAPE; sn++)
	{   if (!str_cmp(shape_skill[sn].name, skill_name)) break;
	    else continue;
	}

	
	if (shape_skill[sn].name == NULL) return FALSE;
	if (able_to_use(ch, sn)) return TRUE;
return FALSE;
}

bool will_use(CHAR_DATA *ch, char *skill_name)
{
	bool true = FALSE;
	bool improve = FALSE;
	int sn;
	if (!IS_CLASS(ch, CLASS_SHAPE)) return FALSE;

	for (sn = 0; sn  < MAX_SHAPE; sn++)
	{   if (!str_cmp(shape_skill[sn].name, skill_name)) break;
	    else continue;
	}
	if (shape_skill[sn].name == NULL) return FALSE;
	if (ch->pcdata->shape[sn] == 100) return TRUE;	

	if (!can_use(ch, skill_name)) return FALSE;
	if (can_use(ch, skill_name) && number_percent() <  ch->pcdata->shape[sn]) true = TRUE;

	if (true == TRUE) {if (number_percent() > ch->pcdata->shape[sn]) improve = TRUE;else improve = FALSE;}
	if (true == TRUE) {if (number_percent() > ch->pcdata->shape[sn]) improve = TRUE;else improve = FALSE;}
	if (true == TRUE) {if (number_percent() > ch->pcdata->shape[sn]) improve = TRUE;else improve = FALSE;}

	if (true == FALSE) {if (number_percent() > ch->pcdata->shape[sn]) improve = TRUE;else improve = FALSE;}
	
	if (improve == TRUE && !str_cmp(shape_skill[sn].name, "AutoDecapitate")) ch->pcdata->shape[sn] += 1;
	if (improve == TRUE) {ch->pcdata->shape[sn] += 1;if (ch->pcdata->shape[sn] > 100) ch->pcdata->shape[sn] = 100;}
	
	if (true == TRUE) return TRUE; else return FALSE;
}


bool able_to_use(CHAR_DATA *ch, int sn)
{
int hmm = (shape_skill[sn].percent - 1);
if (ch->shape[dopplevnum] == 14127) return TRUE;
if (shape_skill[sn].name == NULL) return FALSE;
if (ch->shape[dopplevnum] > 0) {
if (ch->pcdata->shape[sn] > hmm) return TRUE;
if (shape_skill[sn].vnum != 9) {
if (shape_skill[sn].vnum  == ch->shape[dopplevnum]) return TRUE;
if (shape_skill[sn].vnum2 == ch->shape[dopplevnum]) return TRUE;
if (shape_skill[sn].vnum3 == ch->shape[dopplevnum]) return TRUE;
}

if (shape_skill[sn].vnum == 9) {
			if (!str_cmp(shape_skill[sn].name, "Enhanced Sword") && ch->shape[doppleweapon] == SWORD) return TRUE;
			else if (!str_cmp(shape_skill[sn].name, "Enhanced Dagger") && ch->shape[doppleweapon] == DAGGER) return TRUE;
			else if (!str_cmp(shape_skill[sn].name, "Enhanced Whip") && ch->shape[doppleweapon] == WHIP) return TRUE;
			else if (!str_cmp(shape_skill[sn].name, "Enhanced Mace") && ch->shape[doppleweapon] == MACE) return TRUE;
			else if (!str_cmp(shape_skill[sn].name, "Enhanced Unarmed") && ch->shape[doppleweapon] == UNARMED) return TRUE;
			else if (!str_cmp(shape_skill[sn].name, "Enhanced Firearm") && ch->shape[doppleweapon] == FIREARM) return TRUE;
			else return FALSE;
		}

return FALSE;
}
else {
if (ch->pcdata->shape[sn] >= shape_skill[sn].percent) return TRUE;
return FALSE;
}
}

void do_shiftgate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
        return;

    if (!will_use(ch, "ShiftGate"))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }



    if (ch->mana < 1500)
    {
        send_to_char("You do not have enough vitality to do that!\n\r",ch);
        return;
    }
    if (ch->fight_timer > 0)
    {
       send_to_char("You may not open a gate path with a fight timer.\n\r", ch);
       return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Open a gate path to who?\n\r", ch );   
        return;
    }

    if ((victim = get_char_world(ch, arg)) == NULL)
    {
	send_to_char("They aren't here!\n\r", ch);
	return;
    }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
        return;
    }

    if (victim->in_room->vnum == ch->in_room->vnum)
    {
        send_to_char("But you're already there!\n\r",ch);
        return;
    }
    if (victim->mana > ch->mana && !IS_NPC(victim)) 
    {
	send_to_char("Get more mana.\n\r", ch);
	return;
    }

        act("You open up a gate to $N and enter it.", ch, NULL, victim, TO_CHAR);
        act("$n opens up a gate and enters through it.", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch,victim->in_room);
        act("$n appears through a gate, infront of $N.", ch, NULL, victim, TO_ROOM);
        act("A gate appears and $n steps out.", ch, NULL, victim, TO_VICT);
	if (!IS_NPC(victim)) ch->mana -= 1000;
	do_look(ch, "");
WAIT_STATE(ch, 2);
    return;
}
 
void do_warcry(CHAR_DATA *ch, char *argument)
{
    if (!will_use(ch, "Warcry")) {send_to_char("Huh?\n\r", ch);return;}
	
	if (ch->shape[warcry] == TRUE) {send_to_char("You are already in a frenzy like state.\n\r", ch);
						return;}
	ch->shape[warcry] = TRUE;
send_to_char("You scream out a powerfull battlecry, and you become empowered by the gods.\n\r", ch);
act("$n screams out a powerfull warcry, and turns into a mad frenzy.", ch, NULL, NULL, TO_ROOM);
WAIT_STATE(ch, 10);
return;
}

void do_leave(CHAR_DATA *ch, char *argument)
{
if (!will_use(ch, "Leave Without Notice")) {send_to_char("Huh?\n\r", ch);return;}


    if ( IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You are now able to be seen..\n\r", ch );
        act("$n slowly emerges into sight.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
        if (has_timer(ch)) return;

        send_to_char( "You hide yourself.\n\r", ch );
        act("$n slowly fades into the background.",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->act, PLR_SACREDINVIS);
    }
return;         
}

void do_double(CHAR_DATA *ch, char *argument)
{
 int a;
 int times;
 char arg1[10];
 CHAR_DATA *vict = NULL;
 int sn;
 char *spell;

 argument = one_argument(argument, arg1);

if (!str_cmp(arg1, "heal"))
 { 
	if (!will_use(ch, "Double Heal")) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (*argument && (vict = get_char_room(ch, argument)) != NULL)
       ;
    else vict = NULL;
    if (vict == NULL) vict = ch;

    times = number_range(1, 4);
    times += 2;
WAIT_STATE(ch, PULSE_VIOLENCE*.75);

    act("&B$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
    act("&BYour eyes glow bright blue for a moment.",ch,NULL,NULL,TO_CHAR);
	    for (a=1;a <= times; a++)
            spell_heal( skill_lookup( "heal" ), ch->level, ch, vict );
}
if (!str_cmp(arg1, "bullet"))
 { if (!will_use(ch, "Double Bullet")) {send_to_char("Huh?\n\r", ch);return;}
   times = number_range(1, 6);
if (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == FIREARM)
    times = number_range(2,5)+1;
if (ch->fighting == NULL) return;
vict = ch->fighting;
    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
        return;
for (a=1;a <= times; a++)
    (*skill_table[sn].spell_fun) ( sn, 240, ch, vict );           
 }
WAIT_STATE(ch, PULSE_VIOLENCE*.75);

return;
}


void do_randomize(CHAR_DATA *ch, char *argument)
{
    char arg[MSL];
    int special;
    int sn;
    int a;
    OBJ_DATA *obj;

	argument = one_argument(argument, arg);

	if (!will_use(ch, "Randomize Weapon")) {
	send_to_char("Huh?\n\r", ch);
	return;}

    if ((obj = get_obj_carry(ch,arg)) == NULL)
    {
    send_to_char("You dont have that weapon.\n\r",ch);
    return;
    }  

if (obj->item_type != ITEM_WEAPON) return;
special = obj->value[0] / 1000;
for (sn = 0; sn == 0; )
{
a = number_range(1, MAX_SKILL);
if (skill_table[a].spell_fun == spell_null) sn = 0; 
else if (skill_table[a].name == NULL) sn = 0;
else if (skill_table[a].skill_level > ch->trust) sn = 0;
else sn = a;
}
obj->value[0] = sn + (special * 1000);
act("$p glows bright blue as $n utters a few words.", ch, obj, NULL, TO_ROOM);
act("$p glows bright blue as you utter a few words.", ch, obj, NULL, TO_CHAR);
return;
}

void do_shapecreate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int store;
    int store2;
    argument = one_argument( argument, arg );

if (IS_NPC(ch)) return;

    if (ch->generation > 2)
    {
	send_to_char("You cannot do that.. yet.\n\r",ch);
	return;
    }

if ( arg[0] == '\0' )
    {
        send_to_char( "Make whom a shapeshift?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    } if ( IS_NPC(victim) )
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
        send_to_char("You can't make a classed person a Shape.\n\r", ch);
        return;}

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
        send_to_char( "Only on avatars.\n\r", ch );
        return;
    }

        send_to_char("You are now a shapeshifter!\n\r", victim);
        send_to_char("Ok.\n\r", ch);
        victim->class = CLASS_SHAPE;
        store = victim->trust;
        store2 = victim->level;
        victim->trust = 6.9;
        victim->level = 2;
        if (victim->trust < store) victim->trust = store;
        if (victim->level < store2) victim->level = store2;
        return;
}

void do_sinduct( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];   

   CHAR_DATA *victim;    

   one_argument( argument, arg );
   
   if (!IS_CLASS(ch, CLASS_SHAPE) )
   {
     send_to_char("Huh?\n\r",ch);
     return;
   }
   if (ch->generation > 2) 
   {
     send_to_char("Sorry, you cannot decide a clan member's fate.\n\r",ch);
     return;
   }
   
   if ( arg[0] == '\0' )
   {
     send_to_char( "Decide who's fate?\n\r", ch );
     send_to_char( "Syntax 'Decide <victim>'\n\r", ch );
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
    if (!str_cmp(victim->clan,ch->clan) )
    {
      send_to_char( "They're already in your clan.\n\r", ch );
      return;
    }
    act("You induct $N into your clan.", ch, NULL, victim, TO_CHAR);
    act("$n inducts $N into his clan.", ch, NULL, victim, TO_NOTVICT);
    act("$n inducts you in his clan.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now in $n's clan.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    free_string(victim->clan);
    victim->clan = str_dup( ch->clan );
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_sexpel( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];   

   CHAR_DATA *victim;    

   one_argument( argument, arg );
   
   if (!IS_CLASS(ch, CLASS_SHAPE) )
   {
     send_to_char("Huh?\n\r",ch);
     return;
   }
   if (ch->generation > 2) 
   {
     send_to_char("Sorry, you cannot decide a clan member's fate.\n\r",ch);
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
    if ( arg[0] == '\0' )
    {
      send_to_char( "Expel who?\n\r", ch );
      send_to_char( "Syntax 'Expel <victim>'\n\r", ch );
	   return;
    }
    if (str_cmp(victim->clan,ch->clan) )
    {
      send_to_char( "They're aren't in your clan.\n\r", ch );
      return;
    }
    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
      send_to_char( "You can only expel avatars.\n\r", ch );
      return;
    }
    act("You expel $N from your clan.", ch, NULL, victim, TO_CHAR);
    act("$n expels $N from his clan.", ch, NULL, victim, TO_NOTVICT);
    act("$n expelled you from his clan.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now clanless.\n\r", victim );
    free_string(victim->lord);
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->lord     = str_dup( "" );
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}


void do_darkland(CHAR_DATA *ch, char *argument)
{
    if (!IS_CLASS(ch, CLASS_SHAPE) || !can_use(ch, "darkland"))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if (!will_use(ch, "darkland"))
    {
	will_use(ch, "darkland");
	stc("You have failed to step in to the darkland.\n\r", ch);
	return;
    }

    if (ch->affected_by & AFF_SHADOWPLANE)
    {
	stc("You are cold. You start to shiver. You think of a warmer place and appear in the vitten land.\n\r", ch);
	act("$n starts to shiver, then dissolves out of the area.", ch, 0, 0, TO_ROOM);
	ch->affected_by &= ~AFF_SHADOWPLANE;
	char_from_room(ch);
	char_to_room(ch, get_room_index(3001));
	do_look(ch, "auto");
	return;
    }

    stc("Your body color turns blue and you start to get cold. You have entered the darkland.\n\r", ch);
    act("$n's body color turns blue as he fades in to another phase of existance.", ch, 0, 0, TO_ROOM);
    ch->affected_by |= AFF_SHADOWPLANE;
    do_look(ch,"auto");

    return;
}

void do_slaiyth(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if (!IS_CLASS(ch, CLASS_SHAPE) || !can_use(ch, "slaiyth"))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if (!will_use(ch, "Slaiyth"))
    {
	stc("You are not skilled enough for that, child.\n\r", ch);
	return;
    }

    if (IS_SLAIYTH(ch))
    {
	stc("Your scythes begin to rot and your hands become visable.\n\r", ch);
	act("$n's scythes begin to rot as his hands become visable.", ch, 0, 0, TO_ROOM);
	REM_SLAIYTH(ch);
	return;
    }

    stc("You drop your weapons and you begin to notice a curve form in your hands.\n\r", ch);
    stc("The curve turns in to a sharp metal and takes the form of a scythe!\n\r", ch);
    act("$n drops his weapons as his hands begin to turn in to a violent metal.", ch, 0, 0, TO_ROOM);

    SET_SLAIYTH(ch);
#define getobjoff(kode) if ((obj = get_eq_char(ch,kode)) != NULL){obj_from_char(obj);obj_to_room(obj,ch->in_room);}
    getobjoff(WEAR_WIELD);getobjoff(WEAR_HOLD);
    getobjoff(WEAR_WIELD);getobjoff(WEAR_HOLD);
    getobjoff(WEAR_THIRD);getobjoff(WEAR_FOURTH);getobjoff(WEAR_SCABBARD_R);
    getobjoff(WEAR_SCABBARD_L);

    return;
}

typedef enum {Fire=0, Ice=1, Air=2, Earth=3, Poison=4} entropyName;
int entropy_bonus(CHAR_DATA *ch, entropyName entrope); // prototype...


//called from the fight code  , 1 = offense 2 = defense
int entropy_modify(CHAR_DATA *ch, CHAR_DATA *vict, int type)
{
    int bonus = 0;

    if (!IS_CLASS(ch, CLASS_SHAPE)) return bonus;

    if (type == 1) // computing offense
    {
	if (IS_NPC(vict)) bonus += entropy_bonus(ch, Earth);
	if (IS_CLASS(vict, CLASS_JEDI) || IS_CLASS(vict, CLASS_DEMON)
		|| IS_CLASS(vict, CLASS_GOLEM))
		bonus += entropy_bonus(ch, Ice);
	if (IS_CLASS(vict, CLASS_DRUID) || IS_CLASS(vict, CLASS_HIGHLANDER)
		|| IS_CLASS(vict, CLASS_WEREWOLF))
		bonus += entropy_bonus(ch, Air);
	if (IS_CLASS(vict, CLASS_AMAZON) || IS_CLASS(vict, CLASS_MONK)
		|| IS_CLASS(vict, CLASS_MAGE))
		bonus += entropy_bonus(ch, Poison);
	if (IS_CLASS(vict, CLASS_NINJA) || IS_CLASS(vict, CLASS_DROW)
		|| IS_CLASS(vict, CLASS_VAMPIRE) || IS_CLASS(vict, CLASS_FLUFF))
		bonus += entropy_bonus(ch, Fire);
    }

    if (type == 2) // computing defense
    {
	if (IS_NPC(vict)) bonus += entropy_bonus(ch, Poison);
	if (IS_CLASS(vict,CLASS_DRUID) || IS_CLASS(vict, CLASS_VAMPIRE)
		|| IS_CLASS(vict, CLASS_DROW))
		bonus += entropy_bonus(ch,Poison);
	if (IS_CLASS(vict,CLASS_JEDI) || IS_CLASS(vict, CLASS_MONK) ||
		IS_CLASS(vict, CLASS_GOLEM) || IS_CLASS(vict, CLASS_HIGHLANDER)
		|| IS_CLASS(vict, CLASS_WEREWOLF))
		bonus += entropy_bonus(ch,Earth);
	if (IS_CLASS(vict, CLASS_AMAZON) || IS_CLASS(vict, CLASS_DEMON))
		bonus += entropy_bonus(ch,Fire);
	if (IS_CLASS(vict, CLASS_NINJA) || IS_CLASS(vict, CLASS_FLUFF))
		bonus += entropy_bonus(ch,Air);
	if (IS_CLASS(vict, CLASS_MAGE))
		bonus += entropy_bonus(ch,Ice);
    }

    if (IS_CLASS(vict, CLASS_MOOGLE))
    {   // Average of all entropy stones, to act as one. "All"
	bonus += (entropy_bonus(ch,Poison)+entropy_bonus(ch,Fire)+entropy_bonus(ch,Earth)
		 +entropy_bonus(ch,Air)+entropy_bonus(ch,Ice))/5;
    }

    if (type == 1) // offensive return value, right now it is a  out of 100, 200 for hybrids.
	bonus /= 7;  // adds on bonus / 7 attacks.

    if (type == 2) // gives an extra  chance of an enthalpy change (new parry/dodge)
    {
	bonus /= 3;
	if (bonus > 50) bonus = 50;
    }

    return bonus;
}

int entropy_bonus(CHAR_DATA *ch, entropyName entrope)
{
    int ret = 0;
    sh_int entropes[5] = {0,0,0,0,0};
    OBJ_DATA *obj;
    int x = 0;
    int begin = 29;

    for (obj = ch->carrying; obj; obj = obj->next_content)
	if (obj->pIndexData->vnum >= begin && obj->pIndexData->vnum <= begin+25)
	    entropes[obj->pIndexData->vnum-begin]++;

    ret = entropes[entrope]*100;
    if (ret == 0) return 0;

    for (begin = 0; begin < 5; begin ++)
	 x += entropes[begin];

    ret /= x;
    return ret;
}
