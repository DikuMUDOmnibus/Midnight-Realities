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

//this class has great potential, although I started it shortly before the
//mud went down. (within a few days even) so I didn't finish it.
/*Smoo class, created under the orders of izumo*/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "merc.h"

#define DMF(fun)	void fun()

DMF(moog_sun_bath);
DMF(moog_wind_slash);
DMF(moog_cokatrice);
DMF(moog_wild_bear);
DMF(moog_elf_fire);
DMF(moog_rage);
DMF(moog_wombat);
DMF(moog_sand_storm);
DMF(moog_antlion);
DMF(moog_sand_blast);
DMF(moog_kitty);
DMF(moog_waterwield);

const struct moogle_dance_stats moogle_table[] =
{
    {"Sun Bath", 	MOOG_IN_FIGHT,	PULSE_VIOLENCE*2,
     0,			1000,		500,
     moog_sun_bath,	FALSE,
     MOOG_WIND_SONG,	2},
    {"Wind Slash",	MOOG_IGNORE,	PULSE_VIOLENCE*2,
     0,			400,		400,
     moog_wind_slash,	FALSE,
     MOOG_WIND_SONG,	3},
    {"Cokatrice",	MOOG_VIOLENCE,	PULSE_VIOLENCE*3,
     200,		250,		500,
     moog_cokatrice,	FALSE,
     MOOG_WIND_SONG,	4},
    {"Wild Bear",	MOOG_VIOLENCE,	PULSE_VIOLENCE,
     0,			500,		700,
     moog_wild_bear,	TRUE,
     MOOG_FOREST_SUIT,	1},
    {"Elf Fire",	MOOG_IGNORE,	PULSE_VIOLENCE*2,
     0,			250,		400,
     moog_elf_fire,	FALSE,
     MOOG_FOREST_SUIT,	2},
    {"Rage",		MOOG_IGNORE,	PULSE_VIOLENCE,
     750,		500,		500,
     moog_rage,		FALSE,
     MOOG_FOREST_SUIT,	3},
    {"Wombat",		MOOG_VIOLENCE,	PULSE_VIOLENCE*2,
     0,			300,		200,
     moog_wombat,	TRUE,
     MOOG_FOREST_SUIT,	4},
    {"Sand Storm",	MOOG_GLOBAL_PERS,PULSE_VIOLENCE*3,
     0,			0,		500,
     moog_sand_storm,	FALSE,
     MOOG_DESERT_ARIA,	1},
    {"Antlion",		MOOG_GLOBAL_PERS,PULSE_VIOLENCE*4,
     200,		400,		400,
     moog_antlion,	TRUE,
     MOOG_DESERT_ARIA,	2},
    {"Sand Blast",	MOOG_IGNORE,	PULSE_VIOLENCE*2,
     0,			300,		400,
     moog_sand_blast,	FALSE,
     MOOG_DESERT_ARIA,	3},
    {"WaterWield",	MOOG_IGNORE,	PULSE_VIOLENCE*3,
     200,		400,		400,
     moog_waterwield,	FALSE,
     MOOG_WATER_RONDO,	4},



{""}
};

void do_dance(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 void *vo;
 int sn;

 argument = one_argument(argument, arg1);

 if (!IS_MOOGLE(ch))
 {
    check_social(ch,"dance",argument,TYPE_SOCIAL);
    return;
 }

 if (arg1[0] == '\0')
 {
    stc("Dance what dance?\n\r", ch);
    return;
 }

 if (ch->pcdata->powers[MOOG_WAIT] > 0)
 {
    stc("You are already dancing!\n\r", ch);
    return;
 }

 if ((sn = moogle_lookup(arg1)) == -1)
 {
    stc("That dance doesn't exist!\n\r", ch);
    return;
 }

 if (ch->move < moogle_table[sn].mv_cost)
 {
    sprintf(log_buf, "You don't have enough energy![%d]\n\r", moogle_table[sn].mv_cost);
    stc(log_buf, ch);
    return;
 }

 if (ch->mana < moogle_table[sn].mn_cost)
 {
    sprintf(log_buf, "You don't have enough power![%d]\n\r", moogle_table[sn].mn_cost);
    stc(log_buf, ch);
    return;
 }

 if (ch->hit < moogle_table[sn].hp_cost)
 {
    sprintf(log_buf, "You don't have enough stamina![%d]\n\r", moogle_table[sn].hp_cost);
    stc(log_buf, ch);
    return;
 }

 act("&M$n runs around shouting, \"Kuppo! Kuppo!\"",
	ch,0,0,TO_ROOM);
 act("&MYou run around shouting, \"Kuppo! Kuppo!\"",
	ch,0,0,TO_CHAR);

 switch(moogle_table[sn].type)
 {
    case MOOG_GLOBAL_PERS:
	if ((ch->uni = get_char_world(ch,argument)) == NULL)
	{stc("They aren't here!\n\r", ch);return;}
	if (moogle_table[sn].pk_check && is_safe(ch,ch->uni)) return;
	ch->pcdata->powers[MOOG_WAIT] = moogle_table[sn].lag/PULSE_VIOLENCE;
	if (ch->pcdata->powers[MOOG_WAIT] ==0)ch->pcdata->powers[MOOG_WAIT] = 1;
	ch->sn = sn;
	break;
    case MOOG_VIOLENCE:
	if ((ch->uni = get_char_room(ch,argument)) == NULL)
	ch->uni = (void *)ch->fighting;
	if (!ch->uni)
	{stc("They aren't here!\n\r", ch);return;}
	if (moogle_table[sn].pk_check && is_safe(ch,ch->uni)) return;
	ch->sn = sn;
	ch->pcdata->powers[MOOG_WAIT] = moogle_table[sn].lag/PULSE_VIOLENCE;
	if (ch->pcdata->powers[MOOG_WAIT] ==0)ch->pcdata->powers[MOOG_WAIT] = 1;
	break;
    case MOOG_IGNORE:
	ch->sn = sn;
	ch->uni = (void *)ch;
	ch->pcdata->powers[MOOG_WAIT] = moogle_table[sn].lag/PULSE_VIOLENCE;
	if (ch->pcdata->powers[MOOG_WAIT] ==0)ch->pcdata->powers[MOOG_WAIT] = 1;
	break;
    case MOOG_IN_FIGHT:
	if (!ch->fighting)
	{
	    stc("You need to be fighting to use that!\n\r", ch);
	    return;
	}
	vo = (void *)ch->fighting;
	ch->sn = sn;
	ch->uni = vo;
	if (moogle_table[sn].pk_check && is_safe(ch,ch->uni)) return;
	ch->pcdata->powers[MOOG_WAIT] = moogle_table[sn].lag/PULSE_VIOLENCE;
	if (ch->pcdata->powers[MOOG_WAIT] ==0)ch->pcdata->powers[MOOG_WAIT] = 1;
	break;
    default:
	log_string("unknown moogle dance type:");
	log_string(CHAR2(moogle_table[sn].type));
	break;
 }

 ch->hit  -= moogle_table[sn].hp_cost;
 ch->mana -= moogle_table[sn].mn_cost;
 ch->move -= moogle_table[sn].mv_cost;
 WAIT_STATE(ch, moogle_table[sn].lag);

 return;

}

bool dance_checkup(CHAR_DATA *ch)
{
if (ch->uni && ((CHAR_DATA *)ch->uni)->in_room != ch->in_room
&& moogle_table[ch->sn].type != MOOG_GLOBAL_PERS)
{stc("They have left the room, or died!\n\r", ch);
    return FALSE;}
if (moogle_table[ch->sn].type == MOOG_GLOBAL_PERS && !ch->uni)
{stc("They have died or quit!\n\r", ch);return FALSE;}

    moogle_table[ch->sn].run(ch,ch->sn,ch->uni);
ch->sn = 0;
return TRUE;
}

int moogle_lookup(char *name)
{
 int a;

 for (a = 0; moogle_table[a].name[0]; a++)
    if (!str_cmp(name, moogle_table[a].name)) return a;

 for (a = 0; moogle_table[a].name[0]; a++)
    if (!str_prefix(name, moogle_table[a].name)) return a;

 return -1;
}

void moog_sun_bath(CHAR_DATA *ch, int sn, void *vo)
{
stc("&YYour body feels relaxed in the sun.\n\r#n", ch);
act("&Y$n stops and relaxes in the sun.", ch,0,0,TO_ROOM);
ch->pcdata->powers[KUPPO_BITS] |= KUPPO_SUN_BATH;
}

void moog_wind_slash(CHAR_DATA *ch, int sn, void *vo)
{
AFFECT_DATA af;

sn = skill_lookup("wind slash");

if (is_affected(ch,sn))
{stc("&CThe power of the wind is already on your side.\n\r",ch);return;}

af = blank_aff(sn);
af.duration = NR(20,30);
act("&C$n&C calls upon the power of the wind to aid $mself.",
    ch,0,0,TO_ROOM);
act("&CYou call upon the power of the wind to aid yourself.",
    ch,0,0,TO_CHAR);
affect_to_char(ch,&af);
}

void moog_cokatrice(CHAR_DATA *ch, int sn, void *vo)
{
 CHAR_DATA *vict = (CHAR_DATA *) vo;

 vict->wait += PULSE_VIOLENCE*NR(4,5);
 vict->spec_fun = NULL;
 vict->shapespec = NULL;

 act("&C$n calls upon the cocatrice to freeze $N!",
	ch,0,vict,TO_NOTVICT);
 act("&C$n calls upon the cocatrice to freeze you.",
	ch,0,vict,TO_VICT);
 act("&CYou call upon the cocatrice to freeze $N.",
	ch,0,vict,TO_CHAR);
}

void moog_wild_bear(CHAR_DATA *ch, int sn, void *vo)
{
 CHAR_DATA *vict = (CHAR_DATA *) vo;

 vict->position = POS_STUNNED;
 act("&BThe wild bear inside $n knocks $N down and attacks!",
	ch,0,vict,TO_NOTVICT);
 act("&BThe wild bear in you knocks $N down and attacks!",
	ch,0,vict,TO_CHAR);
 act("&BThe wild bear in $N knocks you down and attacks!",
	ch,0,vict,TO_VICT);
 multi_hit(ch,vict,TYPE_UNDEFINED);
 multi_hit(ch,vict,TYPE_UNDEFINED);

 return;
}

void moog_elf_fire(CHAR_DATA *ch, int sn, void *vo)
{
AFFECT_DATA af;

sn = skill_lookup("elf fire");

if (is_affected(ch,sn))
{stc("The elves have already blessed you with the power of fire!\n\r",ch);
 return;}

af = blank_aff(sn);
af.duration = NR(20,30);
act("&R$n&R asks the elven god, Melkor, to bless $m with the power of fire.",
	ch,0,0,TO_ROOM);
act("&RYou ask the elven god, Melkor, to bless you with the power of fire.",
	ch,0,0,TO_CHAR);
affect_to_char(ch,&af);
}

void moog_rage(CHAR_DATA *ch, int sn, void *vo)
{
 AFFECT_DATA af;
 sn = skill_lookup("rage");

 if (is_affected(ch,sn))
 {stc("You are already in a screaming rage.\n\r",ch);return;}

 af = blank_aff(sn);
 af.duration = 200;
 af.location = APPLY_DAMROLL;
 af.modifier = 200;
 affect_to_char(ch,&af);
 af.location = APPLY_HITROLL;
 affect_to_char(ch,&af);
 af.location = APPLY_AC;
 af.modifier = -850;
 affect_to_char(ch,&af);

 act("$n screams in rage as the forest gods bless $m.",
    ch,0,0,TO_ROOM);
 act("You scream in rage as the forest gods bless you.",
    ch,0,0,TO_CHAR);
 return;
}

void moog_wombat(CHAR_DATA *ch, int sn, void *vo)
{
 CHAR_DATA *vict = (CHAR_DATA *) vo;
 CHAR_DATA *wombat;

 wombat = create_mobile(get_mob_index(MOB_VNUM_WOMBAT));
 wombat->hit = ch->hit/2;
 wombat->max_hit = ch->hit/2;

 char_to_room(wombat, ch->in_room);

 act("$n calls a wombat to attack $N!",
	ch,0,vict,TO_NOTVICT);
 act("$n calls a wombat to violently attack you!",
	ch,0,vict,TO_VICT);
 act("You call a wombat to attack $N!",
	ch,0,vict,TO_CHAR);
 multi_hit(wombat,vict,TYPE_UNDEFINED);
 multi_hit(wombat,vict,TYPE_UNDEFINED);

 return;
}

void moog_sand_storm(CHAR_DATA *ch, int sn, void *vo)
{
 ROOM_INDEX_DATA *to;
 CHAR_DATA *vict = (CHAR_DATA *)vo;

 to = vict->in_room;

 if (to == NULL)
 {stc("You can't go to them!\n\r", ch);return;}

 act("&DA sand storm comes in, picks up $n, and leaves.",
	ch,0,0,TO_ROOM);
 act("&DA sand storm comes in, picks you up, and leaves.",
	ch,0,0,TO_CHAR);
 char_from_room(ch);
 char_to_room(ch, to);
 act("&DA sand storm comes in, and drops off $n.",
	ch,0,0,TO_ROOM);
 act("&DYou ride the sand storm, until it finally drops you off at $t.",
	ch,to->name,0,TO_CHAR);
 do_look(ch,"auto");
}

void moog_antlion(CHAR_DATA *ch, int sn, void *vo)
{
    CHAR_DATA *vict = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *to;

    if (ch == vict)
    {stc("The Antlion thinks your too wierd to send yourself around the mud, and leaves.\n\r", ch);return;}

    while ((to = get_room_index(number_range(1000,30000))) == NULL)
	;

    act("&DAntlion grabs you and throws you #*a#*c#*r#*o#*s#*s&D the world&W!",
	vict,0,0,TO_CHAR);
    act("&DAntlion grabs $n and throws $m #*a#*c#*r#*o#*s#*s&D the world&W!",
	vict,0,0,TO_ROOM);
    act("&DYou send an Antlion to throw $N #*a#*c#*r#*o#*s#*s&D the world&W!",
	ch,0,vict,TO_CHAR);
    act("&D$n sends an Antlion to throw $t #*a#*c#*r#*o#*s#*s&D the world&W!",
	ch,PERS(vict,ch),0,TO_ROOM);
    char_from_room(vict);
    char_to_room(vict,to);
    act("&YYou are thrown into $t and fly towards a wall, blacking out.",
	vict,to->name,0,TO_CHAR);
    do_look(vict,"auto");
    vict->position = POS_STUNNED;
}

void moog_sand_blast(CHAR_DATA *ch, int sn, void *vo)
{
AFFECT_DATA af;

sn = skill_lookup("sand blast");

if (is_affected(ch,sn))
{stc("Sand already swirls around you in an infinate vortex.\n\r",ch);
 return;}

af = blank_aff(sn);
af.duration = NR(20,30);
act("&W$n&W calls a swirling vortex of sand to surround $m in battles.",
	ch,0,0,TO_ROOM);
act("&WYou call a swirling vortex of sand to surround you in battles.",
	ch,0,0,TO_CHAR);
affect_to_char(ch,&af);
}

void moog_waterwield( CHAR_DATA *ch, int sn, void *vo)
{
 AFFECT_DATA af;
 sn = skill_lookup("waterwield");

 af = blank_aff(sn);

 af.duration = NR(4,5);

 act("&BYou call a waterwield to heal your wounds.",
	ch,0,0,TO_CHAR);
 act("&B$n&B calls a waterwield to heal $s wounds.",
	ch,0,0,TO_ROOM);

 affect_to_char(ch,&af);
}
