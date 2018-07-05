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

#ifdef NB
#undef NB
#endif
#define NB number_bits

#ifdef CD
#undef CD
#endif
#define CD CHAR_DATA

int	getdamage (CD *ch, CD *vict, int damtype, int element);
int	count_attacks (CD *ch, CD *vict, int damtype);
void	one_round(CD *ch, CD *vict, sh_int *wield, int weapons);
void	newdamage(CD *ch, CD *vict, int damage, OBJ_DATA *weap);
void	new_dammessage( CD *ch, CD *vict, int dam, OBJ_DATA *weap );


int getdamage(CD *ch, CD *vict, int damtype, int element)
{
 OBJ_DATA *weap;
 int dam = 0;

 weap = get_eq_char (ch,WEAR_WIELD);
 if (weap && weap->item_type != ITEM_WEAPON) weap = NULL;

 if (weap) dam = dice (weap->value[1]/3,weap->value[2]/3);
    else dam = dice (3,2);

 if (IS_NPC(ch))
 {
    dam += ch->level * number_fuzzy2 (3);

    if (ch->level < 40) dam *= .20;
    else if (ch->level < 80) dam *= .40;
    else if (ch->level < 140) dam *= .70;
    else if (ch->level < 200) dam *= .85;
 }
 else
 {
    if (IS_CLASS(ch, CLASS_MONK))
    {
	if (!weap && ch->newbits & NEW_MONKADAM) 
	    dam = dice (25,35);
    }

    dam += (char_damroll(ch)/2);

    if (!IS_AWAKE(vict))
	dam *= 1.75;

    if (damtype == gsn_backstab)
	dam *= dice (3,3);

    if (damtype == gsn_circle && weap)
	dam *= dice (1,5);

    else if (damtype == gsn_circle)
	dam *= 2;

    if (IS_CLASS(ch,CLASS_HIGHLANDER))
	dam += (dam * ((ch->wpn[damtype-1000]+1) / 170));

    else
	dam += (dam * ((ch->wpn[damtype-1000]+1) / 130));

    if (dam < 1) dam = 1;

    if (IS_CLASS(ch,CLASS_DEMON))
    {
	if (ch->power[DISC_DAEM_TEMP] > 5)
	    dam *= 2.3;
    }

    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if (ch->power[DISC_WERE_PAIN] > 10)
	    dam *= 1.1;
	else if (ch->power[DISC_WERE_PAIN] > 7)
	    dam *= .9;
	else if (ch->power[DISC_WERE_PAIN] > 3)
	    dam *= .8;
	else
	    dam *= .5;

	if (!IS_NPC(vict) && ch->rage >= 1)
	    dam *= 1.1;

	if (IS_NPC(vict))
	    dam *= 1.1;
    }
    if (IS_CLASS(ch, CLASS_MONK))
    {
	//daytime/nighttime dam put in cap_dam ()
	if (ch->pcdata->powers[PMONK] >= 12)
	    dam *= 1.9;
	else if (ch->pcdata->powers[PMONK] >= 6)
	    dam *= 1.8;
	else if (ch->pcdata->powers[PMONK] >= 3)
	    dam *= 1.7;

	if (!IS_NPC(vict))
	    dam *= 1.6;
	else
	    dam *= 1.4;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	switch (ch->pcdata->rank)
	{
	    case AGE_METHUSELAH: dam *= 1.5;
	    case AGE_ELDER:	 dam *= 1.4;
	    case AGE_ANCILLA:	 dam *= 1.5;
	    default:		 dam *= 1.3; break;
	}

	if (weather_info.sunlight == SUN_LIGHT)
	    dam *= .5;
	else
	    dam *= 2.5;
    }
    if (IS_CLASS(ch, CLASS_JEDI))
    {
	switch(ch->generation)
	{
	    case 1: dam *= 2;
	    case 2: dam *= 1.5;
	    case 3: ;
	    case 4: if (ch->special & SPC_PRINCE) dam *= 2;
		    else dam *= 1.5;
	    default: dam *= 2; break;
	}

	if (ch->pcdata->powers[JPOWER_SPIRIT] > 4)
	    dam *= 1.5;

	dam *= 1 + ( ( ch->pcdata->powers[JEDI_STATE]) * 0.4 );
    }
    if (IS_CLASS(ch, CLASS_MAGE))
    {
	if (ch->generation <= 2)
	    dam *= 1.2;
	else
	    dam *= 1.1;

	if (check_poly(ch, POLY_HUGE_SERPENT) || check_poly(ch, POLY_WYVERN))
	    dam *= 3;
    }
    if (IS_CLASS(ch, CLASS_SHAPE))
    {
	if ((get_weapon_type(ch,weap) == UNARMED && will_use(ch, "Enhanced Unarmed")) || (get_weapon_type(ch,weap) == SWORD && will_use(ch, "Enhanced Sword")) || (get_weapon_type(ch,weap) == DAGGER && will_use(ch, "Enhanced Dagger")) || (get_weapon_type(ch,weap) == WHIP && will_use(ch, "Enhanced Whip")) || (get_weapon_type(ch,weap) == MACE && will_use(ch, "Enhanced Mace")) || (get_weapon_type(ch,weap) == FIREARM && will_use(ch, "Enhanced Firearm")))
	    dam *= 1.4;

	if (will_use(ch, "Spirit")) dam *= 2;

	if (will_use(ch, "Unbelievable Power")) dam *= 1.3;
	if (will_use(ch, "Enhanced Damage")) dam *= 1.6;
	if (will_use(ch, "Unnatural Strength")) dam *= 1.4;
    }
    if (IS_CLASS(vict, CLASS_SHAPE))
    {
	if (will_use(vict, "Rock-Like Skin")) dam /= 1.4;
	if (will_use(vict, "Immobility")) dam /= 1.6;
    }
    if (IS_CLASS(ch, CLASS_NINJA))
    {
	if (ch->generation <= 3)
	    dam *= 1.7;
	else
	    dam *= 1.8;

	dam *= 1.6;
    }
    if (IS_CLASS(ch, CLASS_AMAZON))
    {
	if (ch->generation <= 3)
	    dam *= 2.8;
	else
	    dam *= 2.6;

	if (ch->pcdata->powers[PAMAZON] > 8 && get_weapon_type(ch,weap) == UNARMED)
	    dam *= 1.9;
	else dam *= 1.4;
    }

    if (IS_CLASS(ch, CLASS_DROW))
    {
	if (IS_SET(ch->newbits, NEW_DARKNESS))
	    dam *= 2.4;
	else
	    dam *= 1.8;
    }

    if (IS_CLASS(ch, CLASS_HIGHLANDER))
	dam *= (ch->max_hit / 15000)+1;

    if (IS_CLASS(vict, CLASS_JEDI))
    {
	if (vict->pcdata->powers[JEDI_STATE] > 5)
	    dam *= 0.75;

	dam -= (ch->pcdata->powers[JEDI_STATE] * 50);
    }

    if (IS_UNI(ch) && IS_NPC(vict))
	dam *= 2.3;
    else if (IS_UNI(ch))
	dam *= 1.8;

    if (IS_CLASS(vict, CLASS_WEREWOLF) && !IS_NPC(vict) && vict->rage > 0)
    {
	int silv = 0;
	float mult = 0.0;

	dam += number_range(1, vict->rage);

	if (IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER))
	    silv++;
	if (IS_ITEMAFF(ch, ITEMA_LEFT_SILVER))
	    silv++;

	mult = 120 - vict->siltol;

	mult /= 40;

	mult += silv;

	if (silv > 0)
	    dam *= mult;
    }

 }

 if (IS_UNI(vict) && IS_NPC(ch))
    dam /= 7;
 else if (IS_UNI(vict))
    dam /= 3.2;

 { int insp = countinspires(ch) - countinspires(vict);
   insp /= 2;

   if (insp > 8) insp = 8;
   if (insp < -8) insp = -8;

   {float a = (float)insp *.28;
    if (a < 0) a = 0; dam *= (a+1);}
 }


 return dam;
}

void do_mock(CHAR_DATA *ch, char *argument)
{
 CD *vict;
 CD *vict2;
 char arg1[MIL];
 char arg2[MIL];
 int dam;
 int a, b;

 if (!*argument)
 {
    stc("Check out the new fight code's damage on who?\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {
    stc("Syntax: mock (hitter) (tanker)\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("Person 1 isn't here.\n\r", ch);
    return;
 }

 if ((vict2 = get_char_world(ch, arg2)) == NULL)
 {
    stc("Person 2 isn't here.\n\r", ch);
    return;
 }

for (b = 0; b < 2; b++)
{CD *swap;
 swap = vict;
 vict = vict2;
 vict2 = swap;

 for (a = 0; a < 5; a++)
 {
dam = getdamage(vict2, vict, get_eq_char(vict2,WEAR_WIELD) &&
get_eq_char(vict2,WEAR_WIELD)->item_type == ITEM_WEAPON ?
get_eq_char(vict2,WEAR_WIELD)->value[3] + TYPE_HIT : TYPE_HIT, 0);
    sprintf(log_buf, "&R%s's mock hit (new fight code) would have done &Y%d&R damage to %s.#n\n\r", PERS(vict2,ch),dam, PERS(vict, ch));
    stc(log_buf, ch);
    stc("\n\r", ch);
 }
}

 return;
}

int count_attacks(CHAR_DATA *ch, CHAR_DATA *vict, int damtype)
{
 float count = 1;
 int insp = countinspires(ch) - countinspires(vict);

 if (insp > 4) insp = 4;
 if (insp <-4) insp =-4;

 if (damtype < TYPE_HIT)
    return 1;

 if ( IS_NPC(ch) )
 {
    if (ch->level > 35)
	count++;
    if (ch->level > 75)
	count++;
    if (ch->level > 150)
	count++;
    if (ch->level > 335)
	count++;
    if (ch->level > 550)
	count++;
    if (ch->level > 800)
	count++;
    if (ch->level > 1200)
	count += 2;
    if (ch->level > 1400)
	count += (((ch->level - 1200) / 500) * 2.5);
    count += insp;

    if (count < 1) count = 1;

    return (int)count;
 }

 if ((IS_STANCE(ch,STANCE_VIPER) && NB(150) < ch->stance[STANCE_VIPER])
    || (IS_STANCE(ch,STANCE_MANTIS) && NB(175) < ch->stance[STANCE_MANTIS])
    || (IS_STANCE(ch,STANCE_TIGER) && NB(175) < ch->stance[STANCE_TIGER])
    || (IS_STANCE(ch,STANCE_WOLF) && NB(140) < ch->stance[STANCE_WOLF]))
	count++;

 if (IS_STANCE(ch,STANCE_WOLF) && NB(200) < ch->stance[STANCE_WOLF])
	count++;

 if ((IS_STANCE(ch,STANCE_MANTIS) || IS_STANCE(ch,STANCE_TIGER)) && IS_GOLEM(ch))
	count++;

 if (IS_CLASS(ch,CLASS_MONK) && weather_info.sunlight == SUN_LIGHT)
	count += 2;

 if (IS_ITEMAFF(ch, ITEMA_SPEED))
	count += 1.5;

 if (is_affected(ch, skill_lookup("haste")) && IS_CLASS(ch,CLASS_MAGE))
	count += 2.5;
 else if (is_affected(ch, skill_lookup("haste")))
	count++;

 if (IS_CLASS(ch, CLASS_AMAZON))
	count += (ch->pcdata->powers[PAMAZON] / 3);

 if (IS_CLASS(ch, CLASS_VAMPIRE))
	count += (ch->power[DISC_VAMP_CELE] / 3);

 if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] >= 3)
	count += 1.5;
 if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_BOAR] > 6)
	count += 1.5;

 if (IS_CLASS(ch, CLASS_DROW) && (ch->special & SPC_DROW_WAR || ch->generation < 3))
	count++;
 else if (IS_CLASS(ch, CLASS_DROW))
	count--;

 if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 5)
    count += 2;
 if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 3)
    count += 1.5;

 if (IS_CLASS(ch, CLASS_MAGE))
    count += (ch->max_mana / 11700);

 if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] >= 8)
    count++;

 if (IS_CLASS(ch, CLASS_MONK)) count += 2;

 if (IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_SPIRIT] >= 5)
    count++;

 if (IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_MIND] >= 4)
    count++;

 if (IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_BODY] >= 6)
    count++;

 if (IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_TECHNIQUE] >= 9)
    count++;

 if (IS_INFORM(ch,GOLEM_RUBY))
    count += 7;

 if (IS_INFORM(ch,GOLEM_CRYSTAL))
    count -= 5;

 if (IS_INFORM(ch, GOLEM_OBSIDIAN))
    count += 6;

 if (ch->generation <= 4)
    count++;
 if (ch->generation <= 3)
    count++;
 if (ch->generation <= 2)
    count++;
 if (ch->generation <= 1)
    count += 2;

 count += ch->max_move / 13000;

 if (IS_NPC(vict) && !IS_NPC(ch))
    count += 2.5;

 if (IS_CLASS(ch, CLASS_AMAZON) && IS_CLASS(vict,CLASS_SHAPE))
    count += 2.5;

 if (IS_CLASS(ch, CLASS_SHAPE) )
    if ((get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == UNARMED && will_use(ch, "Enhanced Unarmed")) || (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == SWORD && will_use(ch, "Enhanced Sword")) || (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == DAGGER && will_use(ch, "Enhanced Dagger")) || (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == WHIP && will_use(ch, "Enhanced Whip")) || (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == MACE && will_use(ch, "Enhanced Mace")) || (get_weapon_type(ch,get_eq_char(ch,WEAR_WIELD)) == FIREARM && will_use(ch, "Enhanced Firearm")))
	count++;

 if (will_use(ch, "speed")) count++;
 if (will_use(ch, "super speed")) count++;
 if (will_use(ch, "lightning speed")) count++;
 if (will_use(ch, "unbelieveable power")) count++;
 if (IS_INFORM(vict,GOLEM_CRYSTAL)) count -= 3;

 count += insp;

 if (count < 1) count = 1.0;

 return (int)count;
}

void newviolence_update( void )
{
 CHAR_DATA *ch;
 CHAR_DATA *vict;

 for (ch = char_list; ch; ch = ch->next)
 {
    if ((vict = ch->newfighting) != NULL)
    {
	int max = 1;
	sh_int arr[] = {0,0,0,0};

	if (get_eq_char(ch,WEAR_WIELD) &&
get_eq_char(ch,WEAR_WIELD)->item_type == ITEM_WEAPON)
	{arr[0] = get_eq_char(ch,WEAR_WIELD)->value[3];}
	if (get_eq_char(ch,WEAR_HOLD) &&
get_eq_char(ch,WEAR_HOLD)->item_type == ITEM_WEAPON)
	{arr[1] = get_eq_char(ch,WEAR_HOLD)->value[3];max++;}
	if (get_eq_char(ch,WEAR_THIRD) &&
get_eq_char(ch,WEAR_THIRD)->item_type == ITEM_WEAPON)
	{arr[2] = get_eq_char(ch,WEAR_THIRD)->value[3];max++;}
	if (get_eq_char(ch,WEAR_FOURTH) &&
get_eq_char(ch,WEAR_FOURTH)->item_type == ITEM_WEAPON)
	{arr[3] = get_eq_char(ch,WEAR_FOURTH)->value[3];max++;}

	one_round(ch,vict,arr,max);
    }
 }
return;
}

void one_round(CD *ch, CD *vict, sh_int *wield, int weapons)
{
 OBJ_DATA *weap = NULL;
 int damtype;
 int attacks;
 int a;

 for (a = 0; a < 4; a++)
    if (wield[a]) break;

 if (a == 4)
    damtype = 0;
 else
    damtype = wield[a];

 damtype += TYPE_HIT;

 attacks = count_attacks(ch,vict,damtype);
 attacks += weapons;

 for (a = 0; a < attacks; a++)
 {
    int b;
    int c;
    int d;
    c = number_range(0,weapons);

    for (d = 0, b = 0; b < c; d++)
	if (wield[d]) b++;

    if (weapons == 0)
	weap = NULL;
    else if (b == 0)
	weap = get_eq_char(ch,WEAR_WIELD);
    else if (b == 1)
	weap = get_eq_char(ch,WEAR_HOLD);
    else if (b == 2)
	weap = get_eq_char(ch,WEAR_THIRD);
    else if (b == 3)
	weap = get_eq_char(ch,WEAR_FOURTH);
    else weap = NULL;

    damtype = weap ? weap->value[3] : 0;

    damtype += TYPE_HIT;

    newdamage(ch, vict, getdamage(ch,vict,damtype,0), weap);

    if (!ch->newfighting || ch->in_room != vict->in_room)
	break;

 }

 return;
}

void newdamage(CD *ch, CD *vict, int dam, OBJ_DATA *weap)
{
 if (ch->in_room != vict->in_room)
 {stop_newfighting(ch,TRUE);return;}

 if (!IS_NPC(vict) && !IS_NPC(ch)) {
 ch->fight_timer = 20;
 vict->fight_timer = 20;}

 ch->position = POS_FIGHTING;
 vict->position = POS_FIGHTING;
 if (!ch->newfighting) ch->newfighting = vict;
 if (!vict->newfighting) vict->newfighting = ch;

 if (IS_ITEMAFF(vict, ITEMA_RESISTANCE))
    dam /= 2;

 if (IS_AFFECTED(vict, AFF_SANCTUARY))
    dam /= 2;

 new_dammessage(ch, vict, dam, weap);
 vict->hit -= dam;
 if (vict->hit < -10) vict->hit = -10;

 if (vict->hit < 1)
    stop_newfighting(vict,TRUE);

 update_pos(vict);

 switch (vict->position)
 {
    case POS_MORTAL:
	act("$n is mortally wounded, and blood is spraying everywhere.",
	    vict,0,0,TO_ROOM);
	stc("You are mortally wounded, and spraying blood everywhere.",
	    vict);
	break;
    case POS_INCAP:
	act("$n is incapacitated, and bleeding badly.",
	    vict,0,0,TO_ROOM);
	stc("You are incapacitated, and bleeding badly.",
	    vict);
	break;
    case POS_STUNNED:
	act("$n is stunned, but will soon recover.",
	    vict,0,0,TO_ROOM);
	stc("You are stunned, but will soon recover.",
	    vict);
	break;
    case POS_DEAD:
	act("$n is dead!!", vict,0,0,TO_ROOM);
	stc("You have been killed!!\n\r", vict);
	break;
    default:  break;
 }
 if (vict->hit < 1 && IS_NPC(vict))
    deadstuff(ch,vict);

}

void new_dammessage( CD *ch, CD *vict, int dam, OBJ_DATA *weap )
{
 char act1[MIL],act2[MIL],act3[MIL];
 char *dtype1[] =
{"hit","slice","stab","slash","whip","claw","blast","pound","crush","grep","bite","pierce","suck"};
 char *dtype2[] =
{"hits","slices","stabs","slashes","whips","claws","blasts","pounds","crushes","grep","bites","pierces","sucks"};
 char *mes1;
 char *mes2;
 char *damtype1 = weap ? dtype1[weap->value[3]] : dtype1[0];
 char *damtype2 = weap ? dtype2[weap->value[3]] : dtype2[0];

 if (dam < 50) {mes1 = " lightly";mes2 = " lightly";}
 else if (dam < 100) {mes1 = "";mes2 = "";}
 else if (dam < 175) {mes1 = " hard";mes2 = " hard";}
 else if (dam < 250) {mes1 = " extreemly hard";mes2 = " extreemly hard";}
 else if (dam < 600) {mes1 = " amazingly hard";mes2 = " amazingly hard";}
 else if (dam < 900) {mes1 = " incredibly hard";mes2= " incredibly hard";}
 else if (dam <1600) {mes1 = " for an insane ammount of damage!";mes2 = " for an insane ammount of damage!";}
 else if (dam <3000) {mes1 = " too hard for your knowledge!"; mes2 = " too hard for $S knowledge!";}
 else if (dam <4500) {mes1 = " for an UNSPEAKABLE ammount of damage!"; mes2 = " for an UNSPEAKABLE ammount of damage!";}
 else if (dam <6250) {mes1 = " and you say HOLY SHIT!"; mes2 = " and $N says HOLY SHIT!";}
 else {mes1 = " and tears you a new inside with $p#*!#*!#n"; mes2 = " and then tear $M a new inside with $p#*!#*!#n";}

 if (dam >= 6250 && !weap)
 {
    mes1 = " and tears you a new inside#*!#*!#n";
    mes2 = " and you tear them a new inside#*!#*!#n";
 }

 sprintf(act1, "$n %s $N%s[%d]", damtype2, mes2,dam);
 sprintf(act2, "Your %s $N%s[%d]", damtype1, mes2,dam);
 sprintf(act3, "$n %s you%s[%d]", damtype2, mes1,dam);
 
 act(act1,ch,weap,vict,TO_NOTVICT);
 act(act2,ch,weap,vict,TO_CHAR);
 act(act3,ch,weap,vict,TO_VICT);

 return;
}

void do_newkill(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;

 if (!*argument)
 {
    stc("Attack who?\n\r", ch);
    return;
 }

 if ((vict = get_char_room(ch, argument)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

    {
	int max = 0;
	sh_int arr[] = {0,0,0,0};

	if (get_eq_char(ch,WEAR_WIELD) &&
get_eq_char(ch,WEAR_WIELD)->item_type == ITEM_WEAPON)
	{arr[0] = get_eq_char(ch,WEAR_WIELD)->value[3];}
	if (get_eq_char(ch,WEAR_HOLD) &&
get_eq_char(ch,WEAR_HOLD)->item_type == ITEM_WEAPON)
	{arr[1] = get_eq_char(ch,WEAR_HOLD)->value[3];max++;}
	if (get_eq_char(ch,WEAR_THIRD) &&
get_eq_char(ch,WEAR_THIRD)->item_type == ITEM_WEAPON)
	{arr[2] = get_eq_char(ch,WEAR_THIRD)->value[3];max++;}
	if (get_eq_char(ch,WEAR_FOURTH) &&
get_eq_char(ch,WEAR_FOURTH)->item_type == ITEM_WEAPON)
	{arr[3] = get_eq_char(ch,WEAR_FOURTH)->value[3];max++;}

	one_round(ch,vict,arr,max);
    }

    return;
}
