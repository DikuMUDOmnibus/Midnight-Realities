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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "merc.h"
#include "player.h"

#define REQUIRE(a) if (!IS_CLASS(ch, CLASS_NINJA)) {\
stc("You are not a ninja!\n\r", ch);return;} else if (ch->pcdata->powers[NPOWER_HAMASHAKA] < a) {\
sprintf(log_buf, "You need %d in Hamashaka training to use that.\n\r", a);\
stc(log_buf, ch);return;}

#define damageskl(skl,dam) { if (!*argument && !ch->fighting)\
{sprintf(log_buf, "%s who?\n\r", skill_table[skl].name);stc(log_buf,ch);return;}\
if (!*argument) vict = ch->fighting; \
else if ((vict=get_char_room(ch,argument))==NULL && !ch->fighting)\
{stc("Do that to who?\n\r", ch);return;}\
else if (!vict) vict = ch->fighting;\
damage(ch,vict,dam,skl);WAIT_STATE(ch,skill_table[skl].beats);}

void do_yashakin(CHAR_DATA *ch, char *argument)
{
    if (!IS_CLASS(ch, CLASS_NINJA))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    REQUIRE(4)

    if (ch->fighting && !IS_SET(nbit(ch), NBIT_SHIELD))
    {
	stc("It is too late to erect that.\n\r", ch);
	return;
    }

    nbit(ch) ^= NBIT_SHIELD;
    if (!(nbit(ch) & NBIT_SHIELD))
    {
	stc("&CYou loosen the mental energy field around yourself.#n\n\r", ch);
	act("&C$n loosens the mental energy field around $mself.#n", ch,0,0,TO_ROOM);
	return;
    }

    act("&M$n raises a shield of mental energy around $mself.#n", ch,0,0,TO_ROOM);
    stc("&MYou raise a shield of mental energy around yourself.#n\n\r", ch);
    return;
}

void do_tsunaki(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict;
    REQUIRE(1)

    act("&W$n shouts out, \"&ySUPER LABOYA MASHATA!&W\".#n",ch,0,0,TO_ROOM);
    stc("&WYou shout out, \"&ySUPER LABOYA MASHATA!&W\".#n\n\r", ch);

    damageskl(skill_lookup("tsunaki"), number_range(250,650));
    if (number_range(1,5)==3 && vict->hit > 0)
    {ch->fighting=0;vict->fighting=0;ch->position=POS_STANDING;vict->position=POS_STANDING;
     act("$n's punch hits you in the stomach and you are left gasping for breath.", ch,0,vict,TO_VICT);
     act("$n's punch hits $N in the stomach and $E is left gasping for breath.",ch,0,vict,TO_NOTVICT);
     act("Your punch hits $N in the stomach and $E is left gasping for breath.",ch,0,vict,TO_CHAR);
     vict->position = POS_STUNNED;
    }

    return;
}

void do_jusitori(CHAR_DATA *ch, char *argument)
{
    REQUIRE(2)

    if (!ch->fighting)
    {
	stc("You need to be fighting to use that.\n\r", ch);
	return;
    }

    if (nbit(ch) & NBIT_REDIRECT)
    {
	stc("&CYour mind has already focused energy towards that.#n\n\r", ch);
	return;
    }

    act("&Y$n summons up a globe of mental energy.#n", ch,0,0,TO_ROOM);
    stc("&YYou summon up a globe of mental energy.#n\n\r", ch);

    nbit(ch) |= NBIT_REDIRECT;
    WAIT_STATE(ch,PULSE_VIOLENCE/2);
    return;
}

void do_jeritsu(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict;
    bool sac = FALSE;
    REQUIRE(3)

    if (!*argument)
    {
	stc("&BSummon the jeritsu upon who?#n\n\r", ch);
	return;
    }

    if ((vict = get_char_room(ch,argument)) == NULL)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (vict->fighting || ch->fighting)
    {
	stc("Stop the violence first!\n\r", ch);
	return;
    }

    if (vict->hit < vict->max_hit/2)
    {
	stc("They are too hurt!\n\r", ch);
	return;
    }

    act("&Y$n &Gshouts out, \"&MHUMASHAKA TORI BLADJORA&G\" as he attacks.#n", ch, 0, 0, TO_ROOM);
    stc("&GYou shout out, \"&RYOU SHALL DIE THE DEATH OF THREE&G\" as you attack.#n\n\r", ch);
    vict->hit = vict->max_hit;
if (IS_SET(ch->act, PLR_SACREDINVIS)) sac = TRUE;
REMOVE_BIT(ch->act, PLR_SACREDINVIS);
    damage(ch,vict,number_range(4000,5700),gsn_backstab);
if (ch->fighting)
    damage(ch,vict,number_range(2000,4200),gsn_backstab);
if (ch->fighting)
    damage(ch,vict,number_range(1500,3200),gsn_backstab);
if (sac && ch->fighting)
{
    SET_BIT(ch->act, PLR_SACREDINVIS);
	multi_hit(ch, vict, TYPE_HIT);}
    if (ch->fighting)
	multi_hit(ch, vict, TYPE_HIT);
    if (ch->fighting)
	damage(ch,vict,number_range(1,700),TYPE_HIT+5);

    if (ch->fighting)
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
    return;
}
