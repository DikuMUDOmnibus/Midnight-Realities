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

//I started this class right before the mud went down, bryantos didn't
//give me a warning.
/* OH WOW its ANOTHER SmOO ClASS!! */
#ifdef macintosh
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

void do_sing(CHAR_DATA *ch, char *argument)
{
 char arg[MIL];
 CHAR_DATA *vict;

 if (IS_CLASS(ch, CLASS_FLUFF))
 {
    fluffysing(ch,argument);
    return;
 }

 if (!IS_UNI(ch))
 {
    if (!check_social(ch,"sing",argument,TYPE_SOCIAL))
	stc("Huh?\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg);

 if (!*arg)
 {
     stc("Sing what song?
\rHeal
\rInspire
\rQuell\n\r", ch);
    return;
 }

 if (ch->pcdata->powers[UNI_SONG] != 0)
 {
    stc("You are already singing a song!\n\r", ch);
    return;
 }
 else if (!str_cmp(arg, "heal"))
 {
    ch->pcdata->powers[UNI_WAIT] = 2;
    ch->pcdata->powers[UNI_SONG] = SONG_HEAL;
 }
 else if (!str_cmp(arg, "inspire"))
 {
    ch->pcdata->powers[UNI_SONG] = SONG_INSPIRE;
    ch->pcdata->powers[UNI_WAIT] = 2;
 }
 else if (!str_cmp(arg, "quell"))
 {
    ch->pcdata->powers[UNI_SONG] = SONG_QUELL;
    ch->pcdata->powers[UNI_WAIT] = 3;
 }
 else
 {
    do_sing(ch,"");
    return;
 }

 if (*argument && (vict = get_char_room(ch,argument)) != NULL)
    ch->uni = (void *) vict;

 act("&W$n begins to sing a song.#n",ch,0,0,TO_ROOM);
 act("&WYou begin to sing a song.#n",ch,0,0,TO_CHAR);

 return;
}

void inspire(CHAR_DATA *ch)
{
 AFFECT_DATA af;

 af.type = skill_lookup("inspire");
 af.duration = NR(4,7);
 af.location = APPLY_NONE;
 af.modifier = 0;
 af.bitvector = 0;
 affect_to_char(ch,&af);
 return;
}

int countinspires(CHAR_DATA *ch)
{int retur = 0;
 AFFECT_DATA *paf;

 for (paf = ch->affected; paf; paf = paf->next)
    if (paf->type == skill_lookup("inspire")) retur++;

 return retur;
}

void do_raise(CHAR_DATA *ch, char *argument)
{
 int cost;

 if (!IS_UNI(ch))
 {
    check_social(ch,"raise",argument,TYPE_SOCIAL);
    return;
 }

 if (!str_prefix("heal",argument))
 {int a = ch->pcdata->powers[UNI_DISC_HEAL];

  for (cost = 1000000; a > 0; a--)
    cost *= ch->tier >= 3? 1.56 : 1.8;

  cost = EXP(ch, cost);

  argument += 5;
  while (*argument == ' ')
    argument++;

 if (str_cmp(argument, "improve"))
 {
    sprintf(log_buf, "&G[&Y%3d&G] &WHeal        Next Level:&C%d#n\n\r",
	ch->pcdata->powers[UNI_DISC_HEAL], cost);
    stc(log_buf, ch);
    stc("Type raise heal improve to improve your healing power.\n\r",ch);
    return;
 }


 if (ch->exp < cost)
 {
    stc("You don't have enough exp to raise that!\n\r", ch);
    do_raise(ch,"heal");
    return;
 }

 ch->exp -= cost;
 ch->pcdata->powers[UNI_DISC_HEAL]++;
 act("&W$n raises $m horn in defense, as $e improves $m power.",
	ch,0,0,TO_ROOM);
 act("&WYou raise your horn in defense as you improve your power.",
	ch,0,0,TO_CHAR);
 return;
 }
 
 stc("Right now you may only raise the heal power.
\rType raise heal, or raise heal improve.\n\r", ch);
 return;

}

void uni_heal(CHAR_DATA *ch, CHAR_DATA *vict)
{
    int hp = ch->pcdata->powers[UNI_DISC_HEAL] * (IS_CLASS(ch,CLASS_MOUNTED)?75:100);
    int mana = ch->pcdata->powers[UNI_DISC_HEAL] * 75;
    int move = ch->pcdata->powers[UNI_DISC_HEAL] * 75;

    if (vict->fighting)
    {
	hp /= 3;
    }
    hp = number_range(hp*.75,hp*1.23);
    mana = number_range(mana*.75,mana*1.23);
    move = number_range(move*.75,move*1.23);

    smooregen2(vict,hp,mana,move,750);

    return;
}

void do_trample(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch, *rch_next;
    if (!IS_UNI(ch)) {do_huh(ch,"");return;}

    act("&WYou raise your hoofs and stomp down on everything in the room.",ch,0,0,TO_CHAR);
    act("&W$n raises $S's hoofs and stomps down on everything in the room.",ch,0,0,TO_ROOM);

    for (rch = ch->in_room->people; rch; rch = rch_next)
    {
	rch_next = rch->next_in_room;
	if (rch == ch) continue;

	multi_hit(ch, rch, skill_lookup("charge"));
	multi_hit(ch, rch, skill_lookup("charge"));
    }

    act("&GYou suddle down.",ch,0,0,TO_CHAR);
    act("&G$n suddles down.",ch,0,0,TO_ROOM);
    WAIT_STATE(ch,PULSE_VIOLENCE);
    return;
}

void do_grow(CHAR_DATA *ch, char *argument)
{
    int *s;

    if (!IS_UNI(ch))
    {
	stc("You are not a unicorn!\n\r", ch);
 	return;
    }

    if (IS_NPC(ch))
    {
	stc("Mobs can't use this!\n\r", ch);
	return;
    }

    s = &ch->pcdata->powers[UNI_SIZE];

    if (*s > 7)
    {
	stc("You're already HUGE!\n\r", ch);
	return;
    }

    else (*s)++;

    act("$n grows a little.", ch, 0, 0, TO_ROOM);
    act("You grow a little.", ch, 0, 0, TO_CHAR);
  if (*s == 7)
    act("You are HUGE!", ch, 0, 0, TO_CHAR);

    WAIT_STATE(ch, PULSE_VIOLENCE);

    return;
}
