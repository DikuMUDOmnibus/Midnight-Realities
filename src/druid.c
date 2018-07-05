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

extern char *target_name;

void spell_angel_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dur;

    if (ch->pcdata->powers[DRUID_POWER] < 300 && ch->level < 8) {
	stc("You need 300 points of druid power to use this\n\r", ch);
	return;
	}
if (IS_CLASS(victim, CLASS_DEMON))
{
    int dam = number_range(4000,6000);
act("You breathe the breath of angels on to the damned soul.", ch,0,0,TO_CHAR);
act("$n breathes the breath of angels on to your damned soul.", ch,0,victim,TO_VICT);
act("$n breathes the breath of angels on to $N's damned soul.",ch,0,victim,TO_NOTVICT);
    damage(ch, victim, dam, sn);
    return;
}

if (IS_WARP(ch, REWARD_RESTRICTED)) level -= 25;
dur = level +10;
if (dur < 1) dur = 1;
    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = dur;
    af.location  = APPLY_HITROLL;
    af.modifier  = (level * 4) * 1.2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - (level / 2);
    affect_to_char( victim, &af );
    send_to_char( "&CYou are covered in the breath of angels.&x\n\r", victim );
    if ( ch != victim )
	act( "You breath the breath of the angelss on $N", ch, NULL, victim, TO_CHAR );
    act( "$n is covered in the breath of angels..", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_dragon_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dur;

    if (ch->pcdata->powers[DRUID_POWER] < 400 && ch->level < 8) {
	stc("You need 400 points of druid power to use this\n\r", ch);
	return;
	}
if (IS_WARP(ch, REWARD_RESTRICTED)) level -= 25;
    if ( is_affected( victim, sn ) ) return;
dur = level +10;
if (dur < 1) dur = 1;
    af.type      = sn;
    af.duration  = dur;
    af.location  = APPLY_DAMROLL;
    af.modifier  = (level * 4) * 1.2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - (level / 2);
    affect_to_char( victim, &af );
    send_to_char( "&RYou are covered in the breath of dragon.&x\n\r", victim );
    if ( ch != victim )
	act( "You breath the breath of the dragons on $N", ch, NULL, victim, TO_CHAR );
    act( "$n is covered in the breath of dragons.", victim, NULL, NULL, TO_ROOM );
    return;
}


void spell_shillelagh( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA    *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if (ch->pcdata->powers[DRUID_POWER] < 200 && ch->level < 8) {
	stc("You need 200 points of druid power to use this\n\r", ch);
	return;
	}
if (!get_obj_index(30038)) {stc("Bug. Inform an imm. Error code 001 on spell_shillelagh in druid.c.\n\r", ch);return;}
obj = create_object( get_obj_index( 30038 ), 0 );

obj->item_type = ITEM_WEAPON;
free_string(obj->short_descr);
obj->short_descr = str_dup("&rA &yW&bi&yll&bo&yw &bT&yree &bS&ytaff&x");
free_string(obj->name);
obj->name = str_dup("willow tree staff druid shillelagh");
free_string(obj->description);
obj->description = str_dup("A branch from a willow tree has been carved and is lying here..");
obj->value[3] = 13;
obj->wear_flags = ITEM_TAKE + ITEM_WIELD;
if (!IS_WARP(ch, REWARD_RESTRICTED))
{obj->timer = 15;
obj->value[1] = 50;
obj->value[2] = 60;}
else {
obj->timer = 5;
obj->value[1] = 20;
obj->value[2] = 30;
}
obj->value[0] = 17000 + skill_lookup("poison");
SET_BIT(obj->spectype, ITEMA_WOODEN);

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= IS_WARP(ch, REWARD_RESTRICTED) ? 40 : 90;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= IS_WARP(ch, REWARD_RESTRICTED) ? 40 : 90;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

obj_to_char(obj, ch);
act("&mYou pull $p &mout of a tree and carve it.&w", ch, obj, NULL, TO_CHAR); 
act("&m$n pulls $p &mout of a tree and carves on it.&w", ch, obj, NULL, TO_ROOM);
return;
}

void spell_berry( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA    *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf, af;
    int ppower;
    char power[MAX_INPUT_LENGTH];
    const char *berry1 = "";
    const char *berry2 = "";
    int max = 0;
    int times = 1;
    char buf[MAX_INPUT_LENGTH];

    if (ch->pcdata->powers[DRUID_POWER] < 450 && ch->level < 8) {
	stc("You need 450 points of druid power to use this\n\r", ch);
	return;
	}
for (max = 0,paf = ch->affected; paf; paf = paf->next)
    if (paf->type == sn) max++;

if (max > 10)
{
    stc("You are too tired.\n\r", ch);
    return;
}

if (IS_WARP(ch, REWARD_RESTRICTED)) max = 600;
if (IS_WARP(ch, REWARD_CHEF)) max = 1850; else 
if (!IS_WARP(ch, REWARD_RESTRICTED))
max = 1570;
    target_name = one_argument(target_name, power);
    ppower = atoi(power);

    if (ppower < 1 || ppower > max) {
	sprintf(buf, 
"You can only make berries potented from 0 to %d..\n\r", max);
	stc(buf, ch);
	return;
	}
    if (ch->mana - (ppower*times) < 0) {sprintf(log_buf, 
	"You need %d mana to create that. (Power*%d)\n\r", ppower*times, times);
	stc(log_buf, ch);return;}

    if (ppower < 1851) berry2 = "rainbow";
    if (ppower < 1650) berry2 = "pink";
    if (ppower < 1475) berry2 = "red";
    if (ppower < 1270) berry2 = "blue";
    if (ppower < 1090) berry2 = "purple";
    if (ppower < 890) berry2 = "green";
    if (ppower < 590) berry2 = "yellow";
    if (ppower < 290) berry2 = "orange";
    if (ppower < 20)  berry2 = "stone-like";

    if (ppower < 1851) berry1 = "lucious";
    if (ppower < 1650) berry1 = "delicious";
    if (ppower < 1475) berry1 = "SmooBerry";
    if (ppower < 1200) berry1 = "guisering";
    if (ppower < 925) berry1 = "juicy";
    if (ppower < 790) berry1 = "yummy";
    if (ppower < 675) berry1 = "disguisting";
    if (ppower < 400) berry1 = "sour";
    if (ppower < 225) berry1 = "crunchy";
    if (ppower < 150) berry1 = "liquidated"; 
    if (ppower < 75) berry1 = "mushy";
    if (ppower == 1) {berry1 = "moldy";berry2 = "yellow";}
if (ppower > 599 && ppower < 676)
sprintf(buf, "A %s %s.", berry2, berry1);
else
sprintf(buf, "A %s, %s berry.", berry1, berry2);
if (!get_obj_index(30038)) {stc("Bug. Inform an imm. Error code 001 on spell_berry in druid.c.\n\r", ch);return;}
obj = create_object( get_obj_index( 30038 ), 0 );
obj->value[0] = 0;
obj->value[1] = 0;
obj->value[2] = 0;
obj->value[3] = 0;
free_string(obj->short_descr);
obj->short_descr = str_dup(buf);
sprintf(buf, "berry %s %s", berry1, berry2);
free_string(obj->name);
obj->name = str_dup(buf);
free_string(obj->description);
obj->description = str_dup("A berry, picked by a druid, is on the ground..\n\r");
obj->miscbit[0] = ppower;
obj->miscbit[1] = ch->class;
obj->item_type = ITEM_FOOD;
obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
obj_to_char(obj, ch);
act("You create $p!", ch, obj, NULL, TO_CHAR);
act("$n creates $p..your mouth waters..", ch, obj, NULL, TO_ROOM);
ch->mana -= ppower*times;

    af.type		= sn;
    af.duration	= 1;
    af.location	= APPLY_NONE;
    af.modifier = 0;
    af.bitvector	= 0;
    affect_to_char(ch, &af);

return;
}
/*
void spell_befriend( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (!IS_CLASS(ch, CLASS_DRUID)) {stc("Nooo..\n\r", ch);return;}

    
    if (!IS_NPC(vict)) {stc("You can only befriend animals.\n\r", ch); return;}

    if (!is_name("bunny", vict->name) && !is_name("rabbit", vict->name) &&
!is_name("wolf", vict->name) &&  !is_name("fox", vict->name) &&
!is_name("lizard", vict->name) &&  !is_name("bear", vict->name) &&
!is_name("dog", vict->name) &&  !is_name("fido", vict->name) &&
!is_name("cat", vict->name) &&  !is_name("kitten", vict->name) &&
!is_name("bunny", vict->name) &&  !is_name("snail", vict->name) &&
!is_name("boar", vict->name) &&  !is_name("spider", vict->name) &&
!is_name("snake", vict->name) &&  !is_name("orc", vict->name) &&
!is_name("worm", vict->name) &&  !is_name("warg", vict->name))
{stc("You may only do that to animals..other peoples are too mean.\n\r",
ch);return;}
*/

void spell_reward( int sn, int level, CHAR_DATA *ch, void *vo )
{
/*
    if (number_range(1,3) == 3)
    {
	stc("Spell failed. Try again.\n\r", ch);
	return;
    }
*/
    do_reward(ch, "");
}

void spell_infest( int sn, int level, CHAR_DATA *ch, void *vo )
{
    int app;
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

if (ch->pcdata->powers[DRUID_POWER] < 500 && ch->level < 8) {
stc("You need 500 points of druid power to use that.\n\r", ch);
return;
}

if (victim == ch && !IS_WARP(ch, REWARD_INFEST)) {
stc("You might not want to do that to yourself..\n\r",
ch);return;}
if (victim != ch) {
    if ( is_affected( victim, sn ) ) return;


act("You call an army of spiders to infest $N's brains!",
ch, NULL, victim, TO_CHAR);
act("$n calls an army of spiders to infest your brains!",
ch, NULL, victim, TO_VICT);
act("$n calls an army of spiders to infest $N's brains!",
ch, NULL, victim, TO_NOTVICT);

    af.type      = sn;
    af.duration  = 3;
    af.location  = APPLY_HITROLL;
    af.modifier  = -(level * 1.5);
    af.bitvector = 0;
    affect_to_char( victim, &af );

send_to_char("You feel weak.\n\r", victim);
return;
}
if (victim == ch) {
for (victim = char_list; victim != NULL; victim = victim->next)
{
if (victim == ch) continue;
if (victim->in_room != ch->in_room) continue;
if (is_safe(ch, victim)) continue;
    if ( is_affected( victim, sn ) ) continue;

app = level + 15;
act("You call an army of spiders to infest $N's brains!",
ch, NULL, victim, TO_CHAR);
act("$n calls an army of spiders to infest your brains!",
ch, NULL, victim, TO_VICT);
act("$n calls an army of spiders to infest $N's brains!",
ch, NULL, victim, TO_NOTVICT);

    af.type      = sn;
    af.duration  = 3;
    af.location  = APPLY_HITROLL;
    af.modifier  = -(level * 1.5);
    af.bitvector = 0;
    affect_to_char( victim, &af );

send_to_char("You feel weak.\n\r", victim);
}
return;
}
}

void do_reward(CHAR_DATA *ch, char *argument)
{
 int max_reward = 23;
 int b, count, c;
 bool taken = FALSE;

 if (ch->pcdata->powers[DRUID_POWER]<500)
 {
    stc("You need 500 points of druid power to do this. It costs 200.\n\r", ch);
    return;
 }

 if ((count = bit_count(ch->warp, max_reward)) >= 6)
 {
    stc("The gods have already granted you 6 rewards. Don't be greedy!\n\r", ch);
    return;
 }

 for ( ; !taken ; )
 {
    c = 1;
    for (b = 0; b <= max_reward; b++)
    {
	if (ch->warp & c) {c *= 2; continue;}
	if (taken) break;
	if (number_range(1,max_reward*2) == 1)
	{SET_BIT(ch->warp, c);taken = TRUE; break;}
	c *= 2;
    }
 }

 if (count != bit_count(ch->warp, max_reward))
 {
    stc("You have been rewarded with new found power!\n\r", ch);
    ch->pcdata->powers[DRUID_POWER] -= 200;
 }
 else
    stc("Spell failed. Try again. (no dp deducted)\n\r", ch);
 return;
}

void spell_redeem( int sn, int level, CHAR_DATA *ch, void *vo )
{
    int evil = 2 | 8 | 256 | 512 | 1024 | 8192 | 16384 | 262144 | 2097152 | 4194304;
    int good = ~evil;
    int c;
    int which = 0;

    if (ch->pcdata->powers[DRUID_POWER] < 400)
    {
	stc("You need 400 dp to use that. It costs 400 dp.\n\r", ch);
	return;
    }

    if (!str_cmp(target_name, "good"))
	which = good;
    else if (!str_cmp(target_name, "evil") || !str_cmp(target_name, "bad"))
	which = evil;
    else {stc("Choose 'good' or 'evil' rewards. This clears all of the specified type for the same price.\n\r", ch); return; }

    c = bit_count(ch->warp, 50);

    REMOVE_BIT(ch->warp, which);

    c -= bit_count(ch->warp, 50);

    if (c > 0)
    {
	sprintf(log_buf, "You have redeemed %d %s rewards.\n\r", c, target_name);
	stc(log_buf, ch);
	ch->pcdata->powers[DRUID_POWER] -= 400;
	return;
    }
    else {sprintf(log_buf, "You have no %s warps to redeem. (no dps deducted)\n\r", target_name); stc(log_buf, ch);}
    return;
}

void do_naturetalk(CHAR_DATA *ch, char *argument)
{
    if (!IS_CLASS(ch, CLASS_DRUID) && !IS_IMMORTAL(ch))
    {
	stc("Huh?\n\r", ch);
	return;
    }

stc("You channel your energy towards the forests.\n\r", ch);
if (ch->position > POS_STUNNED)
act("$n channels $s energy towards the forests.", ch, 0, 0, TO_ROOM);
    talk_channel(ch, argument, CHANNEL_NATURE, "NatureTalks");
}
/*
void spell_redeem( int sn, int level, CHAR_DATA *ch, void *vo )
{*/
    
void spell_phoenix_sight( int sn, int level, CHAR_DATA *ch, void *vo )
{
    if (ch->pcdata->powers[DRUID_POWER] < 100)
    {
	stc("You need 100 druid points to use phoenix sight.\n\r", ch);
	return;
    }

    act("&RYour eyes glow with fire as you gain sight.&x\n\r", ch,0,0,TO_CHAR);
    act("&R$n's eyes glow with fire as $e gains sight.&x\n\r", ch, 0, 0, TO_ROOM);
    spell_auto_shield(sn, 20, ch, ch);

    //yeah I know this gives it permenantly, so what? :P
    ch->pcdata->stats[UNI_AFF] |= VAM_NIGHTSIGHT;

    return;
}

void spell_spy( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch = get_char_world(ch, target_name);
    ROOM_INDEX_DATA *rm;

    if (!IS_CLASS(ch, CLASS_DRUID))
    {
	stc("Nah, you don't really want to do that..\n\r", ch);
	return;
    }

    if (ch->pcdata->powers[DRUID_POWER] < number_range(580,640))
    {
	stc("You need the forests to infect you about 600 times to do that!\n\r", ch);
	return;
    }

    if (ch->fighting) {stc("Stop fighting first.\n\r", ch);return;}

    if (!vch) {stc("The trees can't find them!\n\r", ch);return;}

    if (!vch->in_room) {stc("The trees can't find them!\n\r", ch);return;}

    if (vch->in_room == ch->in_room)
    {
	stc("They are already here!\n\r", ch);
	return;
    }

if (IS_WARP(ch, REWARD_LONELY))
    act("&s&GYou silently sneak in to find $N.&t", ch, 0, vch, TO_CHAR);
else {
    act("&s&GYou ask the trees to locate $N.&t", ch, 0, vch, TO_CHAR);
    act("&s&G$n asks the trees to locate $N.&t", ch, 0, vch, TO_ROOM);
}
    rm = ch->in_room;
    char_from_room(ch);
    char_to_room(ch,vch->in_room);
    do_look(ch,"auto");
if (!IS_WARP(ch, REWARD_LONELY))
{
    char_from_room(ch);
    char_to_room(ch, rm);
}
    return;
}

void do_naturalize(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    char arg2[MIL];
    CHAR_DATA *vict;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (!IS_CLASS(ch, CLASS_DRUID)) {do_huh(ch,"");return;}

    if (!arg1[0])
    {
	if (ch->generation < 3)
	    sprintf(log_buf, "You may: induct outcast promote demote\n\r");
	else if (ch->generation ==3)
	    sprintf(log_buf, "You may: induct outcast\n\r");
	else sprintf(log_buf, "You need more experience. (You need to be a teacher)\n\r");

	stc(log_buf, ch);
	return;
    }

    if (ch->generation > 3) {do_naturalize(ch,"");return;}

    if ((vict = get_char_room(ch, arg1)) == NULL)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (vict->generation <= ch->generation)
    {
	stc("You can only do that to people with less power than yourself.\n\r", ch);
	return;
    }

    if (!str_cmp(arg2, "induct"))
    {
	if (!(!str_cmp(vict->clan,"(null)") || strlen(vict->clan) < 4))
	{
	    stc("They are already in a clan.\n\r", ch);
	    return;
	}
	free_string(vict->clan);
	vict->clan = str_dup(ch->clan);
	act("$n inducts you in to $t!", ch, ch->clan, vict, TO_VICT);
	act("You induct $N in to $t!", ch, ch->clan, vict, TO_CHAR);
	act("$n inducts $N in to $t!", ch, ch->clan, vict, TO_NOTVICT);
	vict->generation = 4;
	return;
    }

    if (!str_cmp(arg2, "outcast"))
    {
	char cl[200];
	strcpy(cl, bash_color(ch->clan));
	if (str_cmp(bash_color(vict->clan), cl))
	{
	    stc("They aren't in your clan!\n\r", ch);
	    return;
	}

	free_string(vict->clan);
	vict->clan = str_dup("");
	act("$n outcasts you from $t.", ch, cl, vict, TO_VICT);
	act("You outcast $n from $t.", ch, cl, vict, TO_CHAR);
	act("$n outcasts $N from $t.", ch, cl, vict, TO_NOTVICT);
	vict->generation = 4;
	return;
    }

    if (ch->generation > 2) {do_naturalize(ch,"");return;}

    if (!str_cmp(arg1, "promote"))
    {
	char cl[200];
	strcpy(cl, bash_color(ch->clan));
	if (str_cmp(bash_color(vict->clan), cl))
	{
	    stc("They aren't in your clan!\n\r", ch);
	    return;
	}

	if (vict->generation <= 3)
	{
	    stc("They are already the maximum promotable rank.\n\r", ch);
	    return;
	}

	vict->generation--;

	act("$n has promoted you!", ch, 0, vict, TO_VICT);
	act("You promote $N!", ch, 0, vict, TO_CHAR);
	act("$n promotes $N!", ch, 0, vict, TO_NOTVICT);
	return;
    }

    if (!str_cmp(arg1, "demote"))
    {
	char cl[200];
	strcpy(cl, bash_color(ch->clan));
	if (str_cmp(bash_color(vict->clan), cl))
	{
	    stc("They aren't in your clan!\n\r", ch);
	    return;
	}

	if (vict->generation >= 5)
	{
	    stc("They are already the minimum rank.\n\r", ch);
	    return;
	}

	vict->generation++;

	act("$n has demoted you!", ch, 0, vict, TO_VICT);
	act("You demote $N!", ch, 0, vict, TO_CHAR);
	act("$n demotes $N!", ch, 0, vict, TO_NOTVICT);
	return;
    }

    do_naturalize(ch,"");
    return;
}

void spell_lend( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vict = (CHAR_DATA*)vo;

    if (!IS_CLASS(ch, CLASS_DRUID))
    {
	stc("Yeah, sure.\n\r", ch);
	return;
    }

    if (IS_NPC(vict))
    {
	stc("It's a waste to use that on a mob.\n\r", ch);
	return;
    }

    if (ch==vict)
    {
	stc("Why would you lend stuff to yourself..\n\r", ch);
	return;
    }

    if (ch->hit < 990)
    {
	stc("You are not healthy enough.\n\r", ch);
	return;
    }

    act("$n lends some health to $N.", ch, 0, vict, TO_NOTVICT);
    act("$n lends some health to you.", ch, 0, vict, TO_VICT);
    act("You lend some health to $N.", ch, 0, vict, TO_CHAR);
    smooregen(vict,1000,0,0);
    ch->hit -= 1000;
    update_pos(ch);
    return;
}
