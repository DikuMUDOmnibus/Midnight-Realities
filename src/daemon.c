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

	/*   Daemons   */

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

void frost_breath args(( CHAR_DATA *ch, CHAR_DATA *victim, bool all));
void fire_effect  args(( void *vo, int level, int dam, int target));
DECLARE_DO_FUN( do_stance );

void do_leech(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = NULL;
	int cost;

	cost = (number_range(700,1000));

    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\r\n", ch);
	return;
    }
    if (ch->power[DISC_DAEM_NETH] < 4)
    {
		stc("You must obtain a level 4 mastery of Nether first.\n\r",ch);
		return;
    }

    if (argument[0] == '\0')
    {
		stc("Who's life do you wish to leech off of?\n\r", ch);
		return;
    }

    if ((victim = get_char_room(ch, argument)) == NULL)
    {
	    stc("They aren't here.\n\r", ch);
	    return;
    }

    if (is_safe(ch, victim))
		return;
	
	if (ch->move <(cost+500))
	{
	send_to_char("Get more moves first.\n",ch);
	return;
	}

	WAIT_STATE(ch, 10);

	act("$n stares intently at $N.", ch, NULL, victim, TO_NOTVICT);
	act("You stare intently at $N.", ch, NULL, victim, TO_CHAR);
	act("$n stares intently at you.", ch, NULL, victim, TO_VICT);

    if (victim->hit >= 1000)
    {
		victim->hit -= number_range( (700 + ch->power[DISC_DAEM_NETH]),(1000 + ch->power[DISC_DAEM_NETH]) );
        ch->hit += number_range( 700, 1000 );
	ch->move -= (cost+500);
		if (ch->hit >= ch->max_hit + 200)
			ch->hit = ch->max_hit + 200;
    }
    else
		stc("Nothing seemed to happen.\n\r",ch);
        WAIT_STATE(ch, 6);
	return;

        
}

void do_entomb (CHAR_DATA *ch, char *argument)
{
    int door;

    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_DAEM_GELU] < 6)
    {
		stc("You must achieve a level six mastery of Geluge to use Entomb.\n\r",ch);
		return;
    }

    if (ch->in_room->sector_type == SECT_AIR)
    {
		send_to_char("This ability cannot be used in the air.\n\r", ch);
		return;
    }
if (ch->in_room->vnum >= 50 && ch->in_room->vnum <= 68)
{stc("You may not entomb out of the arena.\n\r", ch);return;}
    if (!TIME_UP(ch, TIMER_ENTOMB) && ch->level < 12)
    {
		send_to_char("This ability can only be used once every 12 hours.\n\r", ch);
		return;
    }

    for (door = 0 ; door < 4 ; door++)
    {
		if (ch->in_room->exit[door] != NULL)
			make_wall(ch->in_room, door, EX_ICE_WALL);
    }

    ch->hit = ch->max_hit;
    ch->move = ch->max_move;
    SET_TIMER(ch, TIMER_ENTOMB, 12);

    act("$n calls forth sheets of ice, blocking your path in every direction.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You call forth sheets of ice to entrap your prey.\n\r", ch);
    act("$n melts into a puddle of slime on the floor, which drains away.", ch, NULL, NULL, TO_ROOM); 
    send_to_char("You teleport to safety.\n\r", ch);

    char_from_room(ch);
    char_to_room(ch, get_room_index(3054));
    act("$n forms out of the ground in front of you.", ch, NULL, NULL, TO_ROOM);
    do_look(ch, "auto");
    WAIT_STATE(ch, 20);
}

void do_gust( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    ROOM_INDEX_DATA *to_room = NULL;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    char            arg1      [MAX_INPUT_LENGTH];
    char            arg2      [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
    int             dam;
     
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (ch->power[DISC_DAEM_GELU] < 7)
    {
	stc("You must achieve a level seven mastery of Geluge to use Gust.\n\r",ch);
	return;

    }

    if (!TIME_UP(ch, TIMER_CAN_GUST) && ch->level < 120)
    {
	send_to_char("You are out of breath.\n\r", ch);
	return;
    }
     
    if ( arg1[0] == '\0' )
    {
        send_to_char("Who do you wish to blow a gust of wind at?\n\r", ch);
        return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }
    
    if ( victim == ch )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
    }
     
    if ( is_safe( ch, victim ) )
      return;
     
    if ( victim->size > ch->size)
    {
        send_to_char("They are too large to affect.\n\r",ch);
        return;
    }
    
    WAIT_STATE( ch, 12 );
    if ( !IS_NPC(ch) && number_percent( )/2 > ch->power[DISC_DAEM_GELU] + ch->generation)
    {
            send_to_char("You huff and puff but they do not budge.\n\r",ch);
	    act("$n blows a gust of cold air into the room.", ch, NULL, NULL, TO_ROOM);
            return;
    }
    SET_TIMER(ch, TIMER_CAN_GUST, dice(1,3)+1);    
    act("You breathe a gust of cold air at $N.", ch, NULL, victim, TO_CHAR);
    act("$n breathes a gust of cold air at $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n breathes a gust of cold air at you.", ch, NULL, victim, TO_VICT);

    rev_dir = 0;
    if ( arg2[0] == '\0' )
        door = number_range(0,3);
    else
    {
        if      ( !str_prefix( arg2, "north" ) )
            door = 0;
        else if ( !str_prefix( arg2, "east" ) )
            door = 1;
        else if ( !str_prefix( arg2, "south" ) )
            door = 2;
        else if ( !str_prefix( arg2, "west" ) )
            door = 3;
        else
        {
            send_to_char("You can only blow people north, south, east or west.\n\r", ch);
            return;
        }
    }
     
    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
     
    if (( pexit = ch->in_room->exit[door]) == NULL)
    {
        sprintf(buf,"$N flies into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N flies into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You smash into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        dam = dice(victim->size*3+1, ch->power[DISC_DAEM_GELU] * 3);
	  damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
        return;
    }
    
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )
    {
        if (IS_SET(pexit->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit->exit_info, EX_LOCKED);
        if (IS_SET(pexit->exit_info, EX_CLOSED))
            REMOVE_BIT(pexit->exit_info, EX_CLOSED);
        sprintf(buf,"$N is blown into the air and flies %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N is blown into the air and flies %s.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You are blown into the air and fly %s.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"There is a loud crash as $n smashes through the $d.");
        act(buf,victim,NULL,pexit->keyword,TO_ROOM);
     
        if ( ( to_room   = pexit->to_room               ) != NULL && ( pexit_rev = to_room->exit[rev_dir] ) != NULL
        &&   pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL )
        {
            if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
                REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
            if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
                REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
            if (door == 0) sprintf(direction,"south");
            if (door == 1) sprintf(direction,"west");
            if (door == 2) sprintf(direction,"north");
            if (door == 3) sprintf(direction,"east");
            char_from_room(victim);
            char_to_room(victim,to_room);
            sprintf(buf,"$n comes smashing in through the %s $d.", direction);
            act(buf,victim,NULL,pexit->keyword,TO_ROOM);
            dam = dice(victim->size*3+1, ch->power[DISC_DAEM_GELU] * 7);
	    	damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
        }
    }
    else
    {
        sprintf(buf,"$N flies %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N flies %s.", direction);   
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You fly %s.", direction);  
        act(buf,ch,NULL,victim,TO_VICT);
        if (door == 0) sprintf(direction,"south");
        if (door == 1) sprintf(direction,"west");
        if (door == 2) sprintf(direction,"north");
        if (door == 3) sprintf(direction,"east");
	  char_from_room(victim);
        char_to_room(victim,pexit->to_room);
        sprintf(buf,"$n comes flying in from the %s.", direction);
        act(buf,victim,NULL,NULL,TO_ROOM);
        dam = dice(victim->size*3+1, ch->power[DISC_DAEM_GELU] * 5);
	  damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
    }
    return;
}

void do_caust(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (ch->power[DISC_DAEM_CORR] < 4)
    {
		stc("You must obtain level four Corruption to use Caust.\n\r",ch);
		return;
    }

    if (argument[0] == '\0')
    {
		send_to_char("Which item do you wish to caust?\n\r", ch);
		return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
		send_to_char("You are not carrying that item.\n\r", ch);
		return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
		send_to_char("That is not a weapon!\n\r", ch);
		return;
    }

    if (dice(1, 100) == 1)
    {
		act("$p is disintegrated by $n's poison.", ch,obj, NULL, TO_ROOM);
		act("Your poison eats through $p.", ch,obj, NULL, TO_CHAR);
		extract_obj(obj);
		return;
    }

    if (IS_WEAP(obj, WEAPON_POISON))
    {
		act("$p is already coated with deadly poison.", ch, obj, NULL, TO_CHAR);
		return;
    }

    act("You run your tongue along $p, coating it with a sickly venom.", ch, obj, NULL, TO_CHAR);
    act("$n runs $m tongue along $p, coating it with a sickly venom.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_POISON);
    obj->value[1] += ch->generation;
    obj->value[2] += ch->generation;
}

void do_chaosport( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\r\n", ch);
	return;
    }


    if (ch->power[DISC_DAEM_DISC] < 4)
    {
		stc("You need at least level 4 in Discord to use Chaos Portal\n\r",ch);
		return;
    }

    if (ch->fighting != NULL)
    {
		if (number_range(1,3) == 1)
		{
			stc("The powers of Chaos fail you.\n\r",ch);
		    act("$n tries in vain to summon a chaos portal.",ch,NULL,NULL,TO_ROOM);
		    return;
		}	
    }
    if (ch->fight_timer > 0)
    {
     send_to_char("The powers of Chaos fail you!\n\r",ch);
     return;
    }
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
    act("$n disappears through a rift in time.",ch,NULL,NULL,TO_ROOM);
    stc("You are swept away through a rift in time.\n\r",ch);
    if (ch->fighting != NULL)
        stop_fighting( ch, TRUE );
{ROOM_INDEX_DATA *rm = get_random_room(ch);
while (rm->vnum >= 50 && rm->vnum <= 68) rm = get_random_room(ch);
}
    char_from_room(ch);
    char_to_room(ch, get_random_room(ch));
    stc("You reappear in a swirling storm of chaos.\n\r",ch);
    act("$n appears in a swirling wind of Chaos.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");
    SET_TIMER(ch, TIMER_CHAOSPORT, 10);
    return;
}


void do_bane (CHAR_DATA *ch, char *argument )
{

    char buf [MAX_INPUT_LENGTH];
    char arg [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int blah;
    AFFECT_DATA af;
    argument=one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON))
     {
    send_to_char("Huh?\n\r",ch);
    return;
     }
if (arg[0]=='\0')
{
send_to_char("Summon what?\n\r",ch);
send_to_char("L1-Hound, L2-Wight, L3-Mdemon, L4-Ldemon, L5-Gdemon.\n\r",ch);
send_to_char("L6-Garg, L7-Reaper, L8-Succub, L9-Ghoul, L10-Cerebus.\n\r",ch);
return;
}

blah=ch->power[DISC_DAEM_CORR];

  if (ch->pcdata->followers > 2)
  {
   send_to_char("You can't seem to summon any of your minions.\n\r",ch);
   return;
  }
  ch->pcdata->followers++;
  victim=create_mobile( get_mob_index( 6 ) );
  if (!str_cmp(arg,"hound")&& blah>0)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("hound");
  victim->short_descr=str_dup("A Hell Hound");
  victim->long_descr=str_dup("A hound of hell snarls at you.");
  victim->level = 50;
  victim->hit = 450;
  victim->max_hit = 450;
  victim->hitroll = 50;
  victim->damroll = 50;
 }
else if (!str_cmp(arg,"wight")&& blah>1)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("wight");
  victim->short_descr=str_dup("An Evil Wight");
  victim->long_descr=str_dup("An Evil Wight glares evilly at you.");
  victim->level = 60;
  victim->hit = 500;
  victim->max_hit = 500;
  victim->hitroll = 60;
  victim->damroll = 60;
 }
else if (!str_cmp(arg,"mdemon")&& blah>2)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("mdemon");
  victim->short_descr=str_dup("A Minor Demon");
  victim->long_descr=str_dup("A Minor Demon forms here from the depths of hell.");
  victim->level = 70;
  victim->hit = 600;
  victim->max_hit = 600;
  victim->hitroll = 70;
  victim->damroll = 70;
 }
else if (!str_cmp(arg,"ldemon")&& blah>3)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("ldemon");
  victim->short_descr=str_dup("A Lesser Demon");
  victim->long_descr=str_dup("A Lesser Demon rises up out of the ground.");
  victim->level = 80;
  victim->hit = 1000;
  victim->max_hit = 1000;
  victim->hitroll = 90;
  victim->damroll = 90;
 }
else if (!str_cmp(arg,"gdemon")&& blah>4)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("gdemon");
  victim->short_descr=str_dup("A Greater Demon");
  victim->long_descr=str_dup("A Greater Demon emits an ear piercing scream.");
  victim->level = 90;
  victim->hit = 3000;
  victim->max_hit = 3000;
  victim->hitroll = 100;
  victim->damroll = 100;
 }
else if (!str_cmp(arg,"garg")&& blah>5)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("garg");
  victim->short_descr=str_dup("A Huge Obsidian Gargoyle");
  victim->long_descr=str_dup("A Huge Obsidian Gargoyle animates to life.");
  victim->level = 100;
  victim->hit = 900;
  victim->max_hit = 900;
  victim->hitroll = 100;
  victim->damroll = 100;
 }
else if (!str_cmp(arg,"reaper")&& blah>6)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("reaper");
  victim->short_descr=str_dup("A Skeletal Reaper");
  victim->long_descr=str_dup("A Skeletal Reaper swings his huge scythe at you.");
  victim->level = 150;
  victim->hit = 1250;
  victim->max_hit = 1250;
  victim->hitroll = 150;
  victim->damroll = 150;
 }
else if (!str_cmp(arg,"succub")&& blah>7)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("succub");
  victim->short_descr=str_dup("A Succubus");
  victim->long_descr=str_dup("A beautiful woman stands here smiling at you.");
  victim->level = 200;
  victim->hit = 1500;
  victim->max_hit = 1500;
  victim->hitroll = 200;
  victim->damroll = 200;
 }
else if (!str_cmp(arg,"ghoul")&& blah>8)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("ghoul");
  victim->short_descr=str_dup("A Ghoul");
  victim->long_descr=str_dup("A horridly disfigured ghoul lurks here.");
  victim->level = 300;
  victim->hit = 3000;
  victim->max_hit = 3000;
  victim->hitroll = 250;
  victim->damroll = 250;
 }
else if (!str_cmp(arg,"cerebus")&& blah>9)
 {
  free_string(victim->short_descr);
  free_string(victim->name);
  free_string(victim->long_descr);
  victim->name=str_dup("cereb");
  victim->short_descr=str_dup("Cerebus");
  victim->long_descr=str_dup("A mammoth hound of Cerebus stands here snarling.");
  victim->level = 400;
  victim->hit = 5000;
  victim->max_hit = 5000;
  victim->hitroll = 300;
  victim->damroll = 300;
 }
else 
 {
send_to_char("You have failed in the art of summoning and have summoned a weaker partner.\n\r",ch);
 }

    sprintf(buf,"%s Screams as they are pulled from their reality.\n\r",victim->short_descr);
    send_to_char(buf,ch);
    
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"%s\n\r",victim->long_descr);
    free_string(victim->long_descr);
    victim->long_descr=str_dup(buf);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    victim->hit = blah*2999;
    victim->max_hit = victim->hit;
    return;
 }
 
void do_unnerve(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_DISC] < 1)
    {
		stc("You need a level one mastery of Discord to use Unnerve.\n\r",ch);
		return;
    }

    if (argument[0] == '\0')
    {
		stc("Who do you wish to unnerve?\n\r", ch);
		return;
    }

    if ( ( victim = get_char_room(ch, argument) ) == NULL)
    {
		stc("They aren't here.\n\r", ch);
		return;
    }

    WAIT_STATE(ch, 7);
    do_say(ch,"&RXeus Dominus Mortai!#n");
    do_stance(victim, "");
    victim->stance[0] = 0;
}

void do_wfreeze(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_GELU] < 1)
    {
		stc("You need a level one mastery of Geluge to freeze a weapon.\n\r",ch);
		return;
    }

    if (argument[0] == '\0')
    {
		send_to_char("Which item do you wish to freeze?\n\r", ch);
		return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
		send_to_char("You are not carrying that item.\n\r", ch);
		return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
		send_to_char("That is not a weapon!\n\r", ch);
		return;
    }

    if (dice(1, 100) == 1)
    {
		act("$p freezes and shatters.", ch,obj, NULL, TO_ROOM);
		act("$p freezes and shatters.", ch,obj, NULL, TO_CHAR);
		extract_obj(obj);
		return;
    }

    if (IS_WEAP(obj, WEAPON_FROST))
    {
		act("$p is already frozen.", ch, obj, NULL, TO_CHAR);
		return;
    }

    act("$p is surrounded by ice crystals.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n is surrounded by ice crystals.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FROST);
}



void do_transform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int trans = 0;

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_MORP] < 2)
    {
		stc("You must obtain a level two mastery of Morphosis.\n\r",ch);
		return;
    }
    if (ch->fight_timer > 0)
    { 
               stc("You may not transform with a fight timer.\n\r",ch);
               return;
    }
    if (argument[0] == '\0')
    {
        send_to_char("You can transform into the following items:\n\r",ch);
        send_to_char("    A Daemonic Helm\n\r",ch);
        if (ch->power[DISC_DAEM_MORP] > 2)
            send_to_char("    Black Steel Greaves\n\r",ch);
        if (ch->power[DISC_DAEM_MORP] > 3)
		{
            send_to_char("    A Black Opera Cloak\n\r",ch);
            send_to_char("    Daemonic Plate Armor\n\r",ch);
		}
        if (ch->power[DISC_DAEM_MORP] > 4)
		{
            send_to_char("    Ring of Cacophony\n\r",ch);
            send_to_char("    Blood-Ruby Sword\n\r",ch);
		}
        if (ch->power[DISC_DAEM_MORP] > 5)
            send_to_char("    Shield of Bone\n\r",ch);
        return;
    }

    if (!str_cmp(argument, "back") )
    {
		if ( ( obj = ch->pcdata->chobj ) == NULL )
		{
			send_to_char("You are already in human form.\n\r",ch);
			return;
		}

		ch->pcdata->obj_vnum = 0;
		obj->chobj = NULL;
		ch->pcdata->chobj = NULL;
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
		free_string(ch->morph);
		ch->morph = str_dup("");
		act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
		act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
		extract_obj(obj);
		if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
		{
			char_from_room(ch);
			char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
		}
        return;
    }
    if (!str_prefix(argument, "helm") && ch->power[DISC_DAEM_MORP] > 1)
		trans = OBJ_VNUM_DHELM;
	else if (!str_prefix(argument, "greaves") && ch->power[DISC_DAEM_MORP] > 2)
		trans = OBJ_VNUM_DGREAVES;
    else if (!str_prefix(argument, "cloak")	&& ch->power[DISC_DAEM_MORP] > 3)
		trans = OBJ_VNUM_DCLOAK;
    else if (!str_prefix(argument, "armor")	&& ch->power[DISC_DAEM_MORP] > 3)
		trans = OBJ_VNUM_DARMOR;
    else if (!str_prefix(argument, "ring") && ch->power[DISC_DAEM_MORP] > 4)
		trans = OBJ_VNUM_DRING;
    else if (!str_prefix(argument, "sword")	&& ch->power[DISC_DAEM_MORP] > 4)
		trans = OBJ_VNUM_DSWORD;
    else if (!str_prefix(argument, "shield") && ch->power[DISC_DAEM_MORP] > 5)
		trans = OBJ_VNUM_DSHIELD;
    else
    {
		do_transform(ch, "");
		return;
    }	

    if ((obj = create_object(get_obj_index(trans),60)) == NULL)
    {
		send_to_char( "You are unable to polymorph into that weapon.\n\r", ch);
		return;
    }

    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
		send_to_char( "Not with all this sticky webbing on.\n\r", ch);
		return;
    }

    obj_to_room(obj,ch->in_room);
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = trans;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    SET_TIMER(ch, TIMER_DAEMONIC_TRANSFORM , 24);
    free_string(ch->morph);
sprintf(log_buf,"&R[&G%s&R]&C%s", ch->pcdata->switchname, obj->short_descr);
    ch->morph = str_dup(log_buf);
    if (!str_cmp(argument, "sword"))
    {
		SET_BIT(obj->weapflags, WEAPON_FLAMING);
		SET_BIT(obj->weapflags, WEAPON_VAMPIRIC);
		SET_BIT(obj->weapflags, WEAPON_FROST);
		SET_BIT(obj->weapflags, WEAPON_SUNBLADE);
		SET_BIT(obj->weapflags, WEAPON_ELE_EARTH);
		SET_BIT(obj->weapflags, WEAPON_ELE_AIR);
		SET_BIT(obj->weapflags, WEAPON_SHOCKING);
    }
    return;
}


int get_polymorph_power (CHAR_DATA *ch)
{
    if (ch->class == CLASS_DEMON)
		return ch->power[DISC_DAEM_MORP] - 1;

    if (ch->level == 12)
		return 100;

    return 0;
}

void do_polymorph (CHAR_DATA *ch, char *argument)
{
    int i, col = 0;
    char buf[MAX_INPUT_LENGTH];
    bool changed = FALSE;


    if (ch->power[DISC_DAEM_MORP] < 2 )
    {
		stc("You require level 2 morphosis.\n\r",ch);
		return;
    }

    if (argument[0] == '\0')
    {
		stc("Syntax : polymorph [<form> list back]\n\r", ch);
		return;
    }

    if (!str_cmp(argument, "list"))
    {
		cent_to_char("Available forms", ch);
		cent_to_char("---===> <===---", ch);

		for (i = 0 ; i < MAX_FORMS ; i++)
		{
			if (form_data[i].polymorph_level > 0 && form_data[i].polymorph_level <= get_polymorph_power(ch)
			&& form_data[i].lookname != NULL )
			{
				sprintf(buf, "   %-12s", form_data[i].lookname);
				stc(buf, ch);
				if ( ++col % 5 == 0)
					send_to_char("\n\r", ch);
			}
		}
		if (col % 5 != 0)
			stc("\n\r", ch);
		return;
    }

    if (!str_cmp(argument, "back"))
    {
		if (ch->cur_form == get_normal_form(ch))
		{
			stc("You are already in your normal form.\n\r", ch);
			return;
		}
		sprintf(buf, "$n polymorphs back into %s.",
	    GET_PROPER_NAME(ch));
		act(buf, ch, NULL, NULL, TO_ROOM);
        stc("You return to your demonic form.\n\r", ch);
		set_form(ch, get_normal_form(ch) );
		WAIT_STATE(ch, 7);
		return;
    }

    if (!TIME_UP(ch, TIMER_CAN_POLYMORPH) && ch->level != 12)
    {
        stc("You have not got enough energy to polymorph so soon.\n\r", ch);
        return;
    }

    for ( i = 0 ; i < MAX_FORMS ; i++ )
    {
		if (form_data[i].lookname != NULL && !str_prefix(argument, form_data[i].lookname)
	    && form_data[i].polymorph_level > 0  && form_data[i].polymorph_level <= get_polymorph_power(ch) )
		{
			if (ch->cur_form == i)
			{
				stc("You are already in that form.\n\r", ch);
				return;
  			}
			changed = TRUE;
			sprintf(buf, "$n transforms into a%s %s.", (is_vowel(form_data[i].lookname[0])) ? "n" : "",
			form_data[i].lookname);
			act(buf, ch, NULL, NULL, TO_ROOM);

			sprintf(buf, "You transform into a%s %s.\n\r",(is_vowel(form_data[i].lookname[0])) ? "n" : "",
			form_data[i].lookname);
			stc(buf, ch);
			WAIT_STATE(ch, 8);
			set_form(ch, i);
			SET_TIMER(ch, TIMER_CAN_POLYMORPH, dice(2, 3) );
		}
    }	 
    if (!changed)    
		stc("No forms of that name found.\n\r", ch);
    return;
}

void do_hellfire (CHAR_DATA *ch, char *argument)
{
    int door;
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_HELL] < 7)
    	return;

    if (ch->in_room->sector_type == SECT_AIR)
    {
		send_to_char("This ability cannot be used in the air.\n\r", ch);
		return;
    }

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
		stc("You cannot desecrate this Holy Place.\n\r",ch);
		return;
    }

    if (!TIME_UP(ch, TIMER_HELLFIRE_SUMMON) && ch->level < 12)
    {
		send_to_char("This ability can only be used once every 12 hours.\n\r", ch);
		return;
    }

    for (door = 0 ; door < 4 ; door++)
    {
		if (ch->in_room->exit[door] != NULL)
			make_wall(ch->in_room, door, EX_FIRE_WALL);
    }

    SET_TIMER(ch, TIMER_HELLFIRE_SUMMON, 12);
    stc("You summon the fires of hell!\n\r",ch);
    act("$n summons the Abyssal fires of Hell!",ch,NULL,NULL,TO_ROOM);
}

const sh_int  revdir [6] =
{
    2, 3, 0, 1, 5, 4
};

void put_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{
    int timer;

    timer = dice(2, 2);

    switch (dir)
    {
	case DIR_NORTH :
	    SET_RTIMER(room, RTIMER_WALL_NORTH, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_SOUTH, timer);
	break;
	case DIR_EAST :
	    SET_RTIMER(room, RTIMER_WALL_EAST, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_WEST, timer);
	break;
	case DIR_SOUTH :
	    SET_RTIMER(room, RTIMER_WALL_SOUTH, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_NORTH, timer);
	break;
	case DIR_WEST :
	    SET_RTIMER(room, RTIMER_WALL_WEST, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_EAST, timer);
	break;
	case DIR_UP :
	    SET_RTIMER(room, RTIMER_WALL_UP, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_DOWN, timer);
	break;
	case DIR_DOWN :
	    SET_RTIMER(room, RTIMER_WALL_DOWN, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_UP, timer);
	break;
    }	
    room->exit[dir]->exit_info = wall;

}

void make_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{
    int was;
    bool makewall = TRUE;

    if (dir < 0 || dir > 5)
    {
	bug("Invalid direction (create wall)", 0);
	return;
    }

    if (room->exit[dir] == NULL)
    {
	bug ("Cant make wall!!!!", 0);
	return;
    }

    was = room->exit[dir]->exit_info;

    if (was == EX_ICE_WALL && wall == EX_FIRE_WALL)
    {
        room_message(room, "The wall of ice bursts into flames.");
        room_message(room->exit[dir]->to_room, "The wall of ice bursts into flames.");
    }

    if (was == EX_FIRE_WALL && wall == EX_ICE_WALL)
    {
        room_message(room, "The wall of flames flickers blue for a second.");
        room_message(room->exit[dir]->to_room, "The wall of flames flickers blue for a second.");
	makewall = FALSE;
    }

    if (makewall) put_wall(room, dir, wall);

    if (room->exit[dir]->to_room == NULL)
    {
	bug("Exit doesnt lead anywhere (create wall).", 0);
	return;
    }

    if (room->exit[dir]->to_room->exit[revdir[dir]] == NULL)
    {
	bug("Reverse Exit doesnt lead anywhere (create wall).", 0);
	return;
    }

    if (makewall) put_wall(room->exit[dir]->to_room, revdir[dir], wall);
}

ROOM_INDEX_DATA * locate_obj (OBJ_DATA *obj)
{
    if (obj->in_obj != NULL)
        return locate_obj(obj->in_obj);

    if (obj->carried_by != NULL)
	return obj->carried_by->in_room;

    return obj->in_room;
}

void do_seed(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_HELL] < 7)
    {
		stc("You must obtain a level seven Mastery of the Hellfire Discipline.\n\r",ch);
		return;
	}

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
	stc("You cannot desecrate this Holy Place!\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to plant with Deamonseed?\n\r", ch);
	return;
    }

    if ((obj = get_obj_carry(ch, argument) ) == NULL)
    {
	send_to_char("You do not have that item in your inventory.\n\r",ch);
	return;
    }
  
    if (IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
    {
	send_to_char("That item is already set to explode.\n\r", ch);
	return;
    }

    if (IS_OBJ_STAT(obj, ITEM_BLESS)
	|| IS_OBJ_STAT(obj, ITEM_ANTI_EVIL) )
    {
	send_to_char("You cannot affect such a holy item.\n\r", ch);
	return;
    }

    if (IS_SET(obj->quest, QUEST_ARTIFACT)
	|| IS_SET(obj->quest, QUEST_RELIC) )
    {
	send_to_char("You cannot affect such a powerful item.\n\r", ch);
	return;
    }

    if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_HEAD)
    {
	send_to_char("Not on severed heads!\n\r",ch);
	return;
    }

    act("$p starts to shudder and vibrate in your hands.", ch, obj, NULL, TO_CHAR);
    act("$n lays $s hands on $p.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->extra_flags2, ITEM_DAEMONSEED);
{int tim = (15-ch->generation)/2+dice(1,3);
    obj->timer = obj->timer<tim?obj->timer:tim;
}}


void do_immolate(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if (ch->power[DISC_DAEM_HELL] < 2)
    {
	stc("You must obtain a level 2 mastery of Hellfire.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to immolate?\n\r", ch);
	return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That is not a weapon!\n\r", ch);
	return;
    }

    if (dice(1, 100) == 1)
    {
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_ROOM);
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_CHAR);
        damage(ch,ch,500,gsn_inferno);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_FLAMING))
    {
	act("$p is already flaming.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p bursts into flames.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n bursts into flames.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FLAMING);
}

void do_destroy (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    
    if (ch->power[DISC_DAEM_HELL] < 5)
    {
	send_to_char("Only true masters of the hellfire discipline may use this ability.\n\r", ch);
	return;
    }


    if ( arg[0] == '\0' )
    {
	send_to_char( "Which daemon do you wish to destroy?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char("You cannot destroy yourself.\n\r",ch);
	return;
    }

    if ( victim->class != ch->class && ch->level != 120)
    {
	send_to_char("You may only destroy daemons.\n\r",ch);
	return;
    }

    if ( victim->generation >= ch->generation)
    {
	send_to_char("They are too powerful to destroy.\n\r",ch);
	return;
    }
    
    if (ch->generation < 12)
    {
	send_to_char("Only Pit Fiends or above may use the destroy ability.\n\r", ch);
	return;
    }

    if ( str_cmp(ch->clan, victim->clan) )
    {
	send_to_char("They are not in your clan.\n\r", ch);
	return;
    }

    ch->alignment -= 30;
    if (ch->alignment < -1000) ch->alignment = -1000;

    act("You breathe out a huge gout of abyssal flame and incinerate $N.", ch, NULL, victim, TO_CHAR);
    act("$n breathes out a huge gout of abyssal flame and incinerates you.", ch, NULL, victim, TO_VICT);
    act("$n breathes out a huge gout of flame and incinerates $N.", ch, NULL, victim, TO_NOTVICT);
    send_to_char("You have been KILLED!", victim);
    act("$n drops dead.", victim, NULL, NULL, TO_ROOM);
    victim->hit = -10;
    victim->move /= 3;
    update_pos(victim);
}

void do_dinferno(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch, *vch_next;
    
    if (ch->power[DISC_DAEM_HELL] < 3)
    {
	send_to_char("You need more experience in the Hellfire discipline.\n\r", ch);
	return;
    }

    if ( ch->hit > 0 )
    {
	send_to_char("This is only used as a last resort.\n\r",ch);
	return;
    }

    act("$n explodes, sending forth a massive fireballs in all directions.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You explode in a huge blast of abyssal flame.\n\r",ch);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	    vch_next = vch->next_in_room;
	    if (vch != ch)
	    {
		damage(vch, ch, ch->power[DISC_DAEM_HELL] * number_range(80,100), gsn_inferno);
		damage(ch, ch, ch->power[DISC_DAEM_HELL] * number_range(250,300), gsn_inferno);
		fire_effect(vch, ch->level, 2000, TARGET_CHAR);
	    }
    }
    WAIT_STATE(ch, 56);
    ch->hit = -10;
    SET_TIMER(ch, TIMER_INFERNO, (20 - ch->generation) * 5);
    raw_kill( ch );
}   

void acid_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM) /* nail objects on the floor */
     {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    acid_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)  /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* let's toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    acid_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ) /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	char *msg;

	
	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	 if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

	chance -= obj->level * 2;

	switch (obj->item_type)
	{
	    default:
		return;
	    case ITEM_CONTAINER:
	    case ITEM_CORPSE_PC:
	    case ITEM_CORPSE_NPC:
		msg = "$p fumes and dissolves.";
		break;
	    case ITEM_STAFF:
	    case ITEM_WAND:
		chance -= 10;
		msg = "$p corrodes and breaks.";
		break;
	    case ITEM_SCROLL:
		chance += 10;
		msg = "$p is burned into waste.";
		break; 
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	/* get rid of the object */
	if (obj->contains)  /* dump contents */
	{
	    for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
	    {
		n_obj = t_obj->next_content;
		obj_from_obj(t_obj);
		if (obj->in_room != NULL)
		    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}

		acid_effect(t_obj,level/2,dam/2,TARGET_OBJ);
	    }
 	}

	extract_obj(obj);
	return;
    }
}


void shock_effect(void *vo,int level, int dam, int target)
{
    if (target == TARGET_ROOM)
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    shock_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    shock_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ)
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int chance;
	char *msg;

	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) /2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

 	chance -= obj->level * 2;

	switch(obj->item_type)
	{
	    default:
		return;
	   case ITEM_WAND:
	   case ITEM_STAFF:
		chance += 10;
		msg = "$p overloads and explodes!";
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	extract_obj(obj);
	return;
    }
}	

void fire_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM)  /* nail objects on the floor */
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    fire_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)   /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* chance of blindness */
	if (!IS_AFFECTED(victim,AFF_BLIND))
	{
            AFFECT_DATA af;
            act("$n is blinded by smoke!",victim,NULL,NULL,TO_ROOM);
            act("Your eyes tear up from smoke...you can't see a thing!",
		victim,NULL,NULL,TO_CHAR);

            af.type         = skill_lookup("fire breath");
            af.duration     = number_range(0,level/10);
            af.location     = APPLY_HITROLL;
            af.modifier     = -20;
            af.bitvector    = AFF_BLIND;

            affect_to_char(victim,&af);
	}

	/* let's toast some gear! */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    fire_effect(obj,level,dam,TARGET_OBJ);
        }
	return;
    }

    if (target == TARGET_OBJ)  /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	char *msg;

        chance = level / 4 + dam / 10;
 
        if (chance > 25)
            chance = (chance - 25) / 2 + 25;
        if (chance > 50)
            chance = (chance - 50) / 2 + 50;

        if (IS_OBJ_STAT(obj,ITEM_BLESS))
            chance -= 5;
        chance -= obj->level * 2;

        switch ( obj->item_type )
        {
        default:         
	    return;
        case ITEM_CONTAINER:
            msg = "$p ignites and burns!";
            break;
        case ITEM_POTION:
            chance += 25;
            msg = "$p bubbles and boils!";
            break;
        case ITEM_SCROLL:
            chance += 50;
            msg = "$p crackles and burns!";
            break;
        case ITEM_STAFF:
            chance += 10;
            msg = "$p smokes and chars!";
            break;
        case ITEM_WAND:
            msg = "$p sparks and sputters!";
            break;
        case ITEM_FOOD:
            msg = "$p blackens and crisps!";
            break;
        case ITEM_PILL:
            msg = "$p melts and drips!";
            break;
        }

        chance = URANGE(5,chance,95);

        if (number_percent() > chance)
            return;
 
	if (IS_SET(obj->quest, QUEST_SPELLPROOF))
            return;      

	if (obj->carried_by != NULL)
            act( msg, obj->carried_by, obj, NULL, TO_ALL );
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

        if (obj->contains)
        {
            /* dump the contents */
 
            for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
            {
                n_obj = t_obj->next_content;
                obj_from_obj(t_obj);
		if (obj->in_room != NULL)
                    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}
		fire_effect(t_obj,level/2,dam/2,TARGET_OBJ);
            }
        }

        extract_obj( obj );
	return;
    }
}

void do_stigmata( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
    char arg [MAX_INPUT_LENGTH];
	int touchdamage;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) 
		return;
	else if (!IS_CLASS(ch, CLASS_DEMON) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < 2000)
	{
		send_to_char("You do not have the vitality to do this\n\r", ch);
		return;
	}
    else if ( arg[0] == '\0' )
    {
		send_to_char( "Who do you wish to attack with a stigmata?\n\r", ch );
		return;
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
	else if (victim->level < 3)
	{
		send_to_char("You cannot do that to them.\n\r", ch);
		return;
	}
	else if ( is_safe( ch, victim ) )
	{
		send_to_char("You cannot seem to do it.\n\r", ch);
		return;
	}
	else
	{

		if (IS_ITEMAFF(ch, ITEMA_SPEED))
		    touchdamage = number_range(10000,17000);	
		else
		    touchdamage = number_range(10000,17000);//toughness lowers these
                ch->fight_timer += 10;
		if (touchdamage > 0)
		{
			sprintf(buf,"You chant at $N cursing him with the hand of Satan! [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"You feel the raw power of the stigmata![%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n grins and prays to Satan to destroy $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			WAIT_STATE(ch, 10);
			damage(ch,victim,touchdamage,skill_lookup("stigmata"));
			ch->move /= 3;
		}
	}
}
void do_graft(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("Huh?\r\n", ch);
	return;
    }


    if (ch->power[DISC_DAEM_ATTA] < 5)
    {
	send_to_char("You need to research attack to five before you can graft.\r\n", ch);
	return;
    }
    if ( arg[0] == '\0')
    {
	stc("Which limb do you wish to graft on to yourself?\n\r",ch);
	return;
    }

    if ( ( obj = get_obj_carry(ch,arg) ) == NULL )
    {
	stc("You do not have that limb.\n\r",ch);
	return;
    }

    if (!IS_SET(obj->extra_flags2, ITEM_ARM))
    {
	stc("That's not even an arm!\n\r",ch);
	return;
    }

    if (IS_SET(ch->newbits, THIRD_HAND) && IS_SET(ch->newbits, FOURTH_HAND))
    {
        stc("You already have four arms!\n\r",ch);
        return;
    }

    if (!IS_SET(ch->newbits, FOURTH_HAND))
    {
        act("You graft an arm onto your body.",ch,NULL,obj,TO_CHAR);
        act("$n grafts an arm onto $m body.",ch,NULL,obj,TO_ROOM);
        SET_BIT(ch->newbits, FOURTH_HAND);
        SET_TIMER(ch, TIMER_FOURTH_ARM_GOING, dice(ch->generation, 6) );
        WAIT_STATE(ch, 18);
	extract_obj(obj);
        return;    
    }

    if (!IS_SET(ch->newbits, THIRD_HAND))
    {
        act("You graft an arm onto your body.",ch,NULL,obj,TO_CHAR);
        act("$n grafts an arm onto $m body.",ch,NULL,obj,TO_ROOM);
        SET_BIT(ch->newbits, THIRD_HAND);
        SET_TIMER(ch, TIMER_THIRD_ARM_GOING, dice(ch->generation, 6) );
        WAIT_STATE(ch, 18);
	extract_obj(obj);
        return;    
    }    

    return;
}

/* Trying to make sense of Demons Listy */
/*
void do_shadowdemon(CHAR_DATA *ch, char *argument)
{
      char buf[MAX_STRING_LENGTH];
      char arg[MAX_INPUT_LENGTH];
      char *fname;
      int form;

      argument = one_argument(argument, arg);
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


      if (ch->power[DISC_DAEM_TEMP] < 6)
      {
          stc("You must obtain level six in Temptation to use Shadow Demon.\n\r",ch);
          return;
      }

      if (arg[0] == '\0')
      {
          stc("What demonic form do you wish to mimic?\n\r",ch);
	  return;
      }

      if (!str_cmp(arg, "return") && ch->cur_form == get_normal_form(ch))
      {
          stc("You are already in your normal demonic form!\n\r",ch);
          return;
      }

	     if (!str_cmp(arg, "lemure"   )) form = FRM_LEMURE;
      else if (!str_cmp(arg, "nupperibo")) form = FRM_NUPPERIBO;
      else if (!str_cmp(arg, "spinagon" )) form = FRM_SPINAGON;
      else if (!str_cmp(arg, "barbazu"  )) form = FRM_BARBAZU;
      else if (!str_cmp(arg, "abishai"  )) form = FRM_ABISHAI;
      else if (!str_cmp(arg, "osyluth"  )) form = FRM_OSYLUTH;
      else if (!str_cmp(arg, "hamatula" )) form = FRM_HAMATULA;
      else if (!str_cmp(arg, "erinyes"  )) form = FRM_ERINYES;
      else if (!str_cmp(arg, "amnizu"   )) form = FRM_AMNIZU;
      else if (!str_cmp(arg, "cornugon" )) form = FRM_CORNUGON;
      else if (!str_cmp(arg, "gelugon"  )) form = FRM_GELUGON;
      else if (!str_cmp(arg, "return"   )) form = get_normal_form(ch);
      else
      {
        stc("That is an invalid demonic form.\n\r",ch);
        return;
      }

      if ((form - 23) <= ch->generation)
      {
          fname = str_dup(ch->name);
          stc("Your skin slowly melts away and you form a puddle on the floor.\n\r",ch);
          act("$n's body slowly melts into a puddle on the ground.",ch,NULL,NULL,TO_ROOM);
          set_form(ch, form);
          sprintf(buf,"Your body slowly reforms as %s.",ch->name);
          stc(buf,ch);
          sprintf(buf,"%s's body slowly reforms as %s.",fname,ch->name);
          act(buf,ch,NULL,NULL,TO_ROOM);
          return;
      }
          
	
}*/


void do_dgate( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
	
    argument = one_argument( argument, arg1 );
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}


    if ( arg1[0] == '\0' )
    {
	send_to_char( "Gate whom?\n\r", ch );
	return;
    }
    if (number_range(0,1)==1)
    {
	send_to_char("You lost you concentration!\r\n", ch);
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	if ( ch->generation < 12 )
	{
	    send_to_char("You do not yet have the power to gate all demons.\n\r",ch);
	}
	else
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		    if ( d->connected == CON_PLAYING
		    &&   d->character != ch
		    &&   d->character->in_room != NULL
		    &&   can_see( ch, d->character ) 
		    &&   d->character->generation < ch->generation
		    &&   d->character->class == CLASS_DEMON)
		    {
			if ( d->character->fighting != NULL ) stop_fighting( d->character, TRUE );
			    act( "A scaly hand reaches down and drags $n away.", d->character, NULL, NULL, TO_ROOM );
			    char_from_room( d->character );
			    char_to_room( d->character, ch->in_room );
			    act( "You have successfully gated $N.", ch, NULL, d->character, TO_CHAR );
			    act( "$n has successfully gated $N.", ch, NULL, d->character, TO_NOTVICT );
			    if ( ch != d->character )
				act( "$n has pulled you through the demonic void.", ch, NULL, d->character, TO_VICT );
			    do_look( d->character, "auto" );
			    send_to_char( "Ok.\n\r", ch );			
		    }
		}
	}
	return;
    }
 
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( !IS_NPC(victim) && victim->class != CLASS_DEMON )
    {
	send_to_char( "You may only gate in demons.\n\r", ch );
	return;
    }

    if (IS_NPC(victim) && victim->level > (ch->generation*70)+150)
    {
	stc("That monster is too powerful!(it is based on generation)\n\r", ch);
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim) && victim->generation >= ch->generation )
    {
	send_to_char("You may only gate one of lesser power than yourself.\n\r",ch);
	return;
    }    

    if ( number_range(1, ch->generation) == 1 )
    {
	send_to_char("Your powers seem to have failed you.\n\r",ch);
	send_to_char("You hear demonic chanting in the distance.\n\r",victim);
	return;
    }
    
    if ( victim->fighting != NULL ) stop_fighting( victim, TRUE );
    act( "A scaly hand reaches down and drags $n away.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room);
    act( "You have successfully gated $N.", ch, NULL, victim, TO_CHAR );
    act( "$n has successfully gated $N.", ch, NULL, victim, TO_NOTVICT );
    if ( ch != victim )
	act( "$n has pulled you through the demonic void.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
    WAIT_STATE(ch, IS_NPC(victim) ? PULSE_VIOLENCE*2.5 : PULSE_VIOLENCE);
}

void do_frostbreath (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);
	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (ch->power[DISC_DAEM_GELU] < 2)
    {
	stc("You must obtain level two in Geluge to use Frostbreath.\n\r",ch);
	return;
    }

    if (!TIME_UP(ch, TIMER_CAN_BREATHE_FROST) && ch->level < 12)
    {
	send_to_char("You are out of breath.\n\r", ch);
	return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Who do you wish to breathe on?\n\r", ch);
	return;
    }

    if (!str_cmp(arg, "all"))
    {
      CHAR_DATA *vch, *vch_next;
      for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
      {
          vch_next = vch->next_in_room;
          if (vch == ch) continue;

  	  if (is_safe(ch,vch) 
	  &&   (ch->fighting != vch || vch->fighting != ch))
	     continue;

        WAIT_STATE(ch, 24);
        SET_TIMER(ch, TIMER_CAN_BREATHE_FROST, 11 - ch->generation / 2);
        frost_breath(ch, vch, TRUE);
      }
    }

    if ( (victim = get_char_room(ch, arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r", ch);
	return;
    }

    WAIT_STATE(ch, 12);
    SET_TIMER(ch, TIMER_CAN_BREATHE_FROST, 9 - ch->generation / 2);
    frost_breath(ch, victim, FALSE);
}


void cold_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM) /* nail objects on the floor */
    {
        ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
        OBJ_DATA *obj, *obj_next;
 
        for (obj = room->contents; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            cold_effect(obj,level,dam,TARGET_OBJ);
        }
        return;
    }

    else if (target == TARGET_CHAR) /* whack a character */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* chill touch effect */
	    AFFECT_DATA af;

            af.type      = skill_lookup("chill touch");
            af.duration  = 4;
            af.location  = APPLY_STR;
            af.modifier  = -1;
            af.bitvector = 0;
            affect_join( victim, &af );

	/* let's toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    cold_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
   }

   else if (target == TARGET_OBJ) /* toast an object */
   {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int chance;
	char *msg;

	chance = level / 5 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) / 2 + 25;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

 	chance -= obj->level * 2;

	switch(obj->item_type)
	{
	    default:
		return;
	    case ITEM_POTION:
		msg = "$p freezes and shatters!";
		chance += 25;
		break;
	    case ITEM_DRINK_CON:
		msg = "$p freezes and shatters!";
		chance += 5;
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (IS_SET(obj->quest, QUEST_SPELLPROOF))
            return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	extract_obj(obj);
	return;
    }
}


void frost_breath(CHAR_DATA *ch, CHAR_DATA *victim, bool all)
{
    CHAR_DATA *vch, *vch_next;
    int dam;
    int power;

    if (IS_CLASS(ch, CLASS_DEMON)) power = ch->power[DISC_DAEM_GELU];
    else power = 0;

    if (all)
    {
	dam = (ch->generation + 1 * power) * 3;
	
        act("$n breathes forth a stream of frost over you.",ch,NULL,NULL,TO_ROOM);
        act("You breath forth a stream of frost.",ch,NULL,NULL,TO_CHAR);

        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
	    vch_next = vch->next_in_room;
	    if (vch == ch) continue;

	    if (is_safe(ch,vch) 
	    &&   (ch->fighting != vch || vch->fighting != ch))
	        continue;

		cold_effect(vch,ch->spl[RED_MAGIC],dam,TARGET_CHAR);
		damage(ch,vch,dam,skill_lookup("frost breath"));
	}
	return;
    }

    if (is_safe(ch, victim)) return;
    dam = (ch->generation +1 * power) * 2;

    act("$n breathes forth a stream of frost over $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a stream of frost over you!",ch,NULL,victim,TO_VICT);
    act("You breath forth a stream of frost over $N.",ch,NULL,victim,TO_CHAR);
    
    damage(ch,victim,dam,skill_lookup("frost breath"));
    cold_effect(victim,ch->spl[RED_MAGIC],dam,TARGET_CHAR);
}

void do_demonic( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_SET(ch->special, SPC_CHAMPION) || !IS_SET(ch->special, SPC_PRINCE))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }

    if (arg[0] == '\0')
    {
	send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor Longsword Shortsword.\n\r",ch);
	return;
    }
    if      (!str_cmp(arg,"ring"     )) vnum = 29650;
    else if (!str_cmp(arg,"collar"   )) vnum = 29651;
    else if (!str_cmp(arg,"plate"    )) vnum = 29652;
    else if (!str_cmp(arg,"helmet"   )) vnum = 29653;
    else if (!str_cmp(arg,"leggings" )) vnum = 29654;
    else if (!str_cmp(arg,"boots"    )) vnum = 29655;
    else if (!str_cmp(arg,"gauntlets")) vnum = 29656;
    else if (!str_cmp(arg,"sleeves"  )) vnum = 29657;
    else if (!str_cmp(arg,"cape"     )) vnum = 29658;
    else if (!str_cmp(arg,"belt"     )) vnum = 29659;
    else if (!str_cmp(arg,"bracer"   )) vnum = 29660;
    else if (!str_cmp(arg,"visor"    )) vnum = 29661;
    else if (!str_cmp(arg,"longsword"   )) vnum = 29662;
    else if (!str_cmp(arg,"shortsword"   )) vnum = 29663;
    else
    {
	send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor Longsword Shortsword.\n\r",ch);
	return;
    }
    if ( ch->pcdata->stats[DEMON_TOTAL] < 5000 || ch->pcdata->stats[DEMON_CURRENT] < 5000)
    {
	send_to_char("It costs 5000 points of power to create a piece of demon armour.\n\r",ch);
	return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Adreno.\n\r",ch);
	return;
    }
    ch->pcdata->stats[DEMON_TOTAL]   -= 5000;
    ch->pcdata->stats[DEMON_CURRENT] -= 5000;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    obj->cost = 5000;
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_hellish( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    int cost = 7500;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\r\n", ch);
	return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Please specify which piece of hellish armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }
    if      (!str_cmp(arg,"ring"     )) vnum = 27931;
    else if (!str_cmp(arg,"collar"   )) vnum = 27932;
    else if (!str_cmp(arg,"plate"    )) vnum = 27933;
    else if (!str_cmp(arg,"helmet"   )) vnum = 27934;
    else if (!str_cmp(arg,"leggings" )) vnum = 27935;
    else if (!str_cmp(arg,"boots"    )) vnum = 27936;
    else if (!str_cmp(arg,"gauntlets")) vnum = 27937;
    else if (!str_cmp(arg,"sleeves"  )) vnum = 27938;
    else if (!str_cmp(arg,"cape"     )) vnum = 27939;
    else if (!str_cmp(arg,"belt"     )) vnum = 27940;
    else if (!str_cmp(arg,"bracer"   )) vnum = 27941;
    else if (!str_cmp(arg,"visor"    )) vnum = 27942;
    else if (!str_cmp(arg,"shortsword")) vnum=29714;
    else if (!str_cmp(arg,"longsword")) vnum=29713;
    else
    {
	send_to_char("Please specify which piece of hellish armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }
if (!str_cmp(arg,"shortsword")||!str_cmp(arg,"longsword")) cost=50000;
    if ( ch->pcdata->stats[DEMON_TOTAL] < cost || ch->pcdata->stats[DEMON_CURRENT] < cost)
    {
	sprintf(log_buf, "It costs %d to create that.\n\r", cost);
	stc(log_buf, ch);
	return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Adreno.\n\r",ch);
	return;
    }
    ch->pcdata->stats[DEMON_TOTAL]   -= cost;
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    obj = create_object(pObjIndex, 50);
    obj->cost = cost;
    obj_to_char(obj, ch);
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}



void do_champions( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char lord[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->lord) < 2 && !IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("But you don't follow any demon!\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_DEMON)) strcpy(lord,ch->name);
	else strcpy(lord,ch->lord);
    sprintf( buf, "The champions of %s:\n\r", lord );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Hits ] [ Mana ] [ Move ] [  Exp  ] [       Power        ]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_DEMON) && !IS_SET(gch->special, SPC_CHAMPION) ) 
	    continue;
	if ( !str_cmp(ch->lord,lord) || !str_cmp(ch->name,lord))
	{
	    sprintf( buf,
	    "[%-16s] [%-6d] [%-6d] [%-6d] [%7ld] [ %-9d%9d ]\n\r",
		capitalize( gch->name ),
		gch->hit,gch->mana,gch->move,
		gch->exp, gch->pcdata->stats[DEMON_CURRENT], gch->pcdata->stats[DEMON_TOTAL]);
		send_to_char( buf, ch );
	}
    }
    return;
}

void do_cull( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
		return;

	if (!IS_CLASS(ch, CLASS_DEMON) )
    {
		send_to_char( "Huh?\n\r", ch );
		return;
    }

	if (ch->generation < 13)
	{
		send_to_char("You are not a pit lord to do this\n\r", ch);
		return;
	}

    if (( arg1[0] == '\0' || arg2[0] == '\0' ))
    {
		send_to_char( "Syntax is: cull <target> <accept/banish>\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
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
		send_to_char( "Not on yourself!\n\r", ch );
		return;
    }

    if ( !IS_CLASS(victim,CLASS_DEMON) )
    {
		send_to_char( "But they are not a demon!\n\r", ch );
		return;
    }

	if ( str_cmp(ch->clan, victim->clan) && !str_cmp(arg2, "banish") )
	{
		send_to_char("They are not in your clan\n\r", ch);
		return;
	}

    if ( victim->generation == 13)
    {
		send_to_char( "You may not do this to another Pit Lord.\n\r", ch );
		return;
    }

    if ( !str_cmp(arg2,"banish") )
    {
		act("You make $N a rogue demon!",ch,NULL,victim,TO_CHAR);
		act("$n has made $N a rogue demon!",ch,NULL,victim,TO_NOTVICT);
		act("$n has made you a rogue demon!",ch,NULL,victim,TO_VICT);
		free_string(victim->clan);
		victim->clan = str_dup( "" );
		return;
    }
    
    else if ( !str_cmp(arg2,"accept") )
    {
		if (str_cmp(victim->clan, ""))
		{
			send_to_char ("You may not induct a demon from another clan\n\r", ch);
			return;
		}
		act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
		act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
		act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
		free_string(victim->clan);
		victim->clan = str_dup( ch->clan );
		return;
    }
    return;
}

void do_firewalk( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);


    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }



	if (ch->power[DISC_DAEM_NETH] < 10)
    {
		stc("You must obtain a level 10 mastery of Nether first.\n\r",ch);
		return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

	if (!IS_NPC(victim))
	{
	if (ch->move < 2000)
	{
    	send_to_char("Get more moves!\n\r",ch);
    	return;
	}
	}
    
    if (ch->fight_timer > 0)
    {
       send_to_char("You may not firewalk with a fight timer.\n\r", ch);
       return;
    }
    if ( arg[0] == '\0' )
    {
	send_to_char( "Firewalk to who?\n\r", ch );
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

    if ( victim->move > ch->move || IS_IMMORTAL(victim) )
    {
	send_to_char("Your fire cannot seem to reach them.\n\r", ch);
	return;
    }
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot do this whilst you are in the arena.\n\r", ch);
		return;
	}
		if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
	{
		send_to_char("You cannot do this whilst they are in the arena.\n\r", ch);
		return;
	}
	if (!IS_NPC(victim) && ch->level < 3)
	{
		send_to_char("You may not firewalk while mortal.\n\r", ch);
		return;
	}

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    if (IS_NPC(victim))

    {
	sprintf(buf,"You open a path of fire and step through to %s!\n\r",victim->short_descr);
	send_to_char(buf, ch);
    }

    if (!IS_NPC(victim))
    {
	sprintf(buf,"You open a path of fire and step through to %s!\n\r",victim->name);
	send_to_char(buf, ch);
	ch->move -= 2000;
	WAIT_STATE(ch, PULSE_VIOLENCE*1.5);
    }
    act("$n appears from a path of fire!\n\r",ch,NULL,NULL,TO_ROOM);
    do_look(ch, "auto");
    return;
}


void do_blink(CHAR_DATA *ch, char *argument)
{CHAR_DATA *victim, *vch, *vch_next;

 if (!IS_CLASS(ch,CLASS_DEMON))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 if (*argument == '\0' && !ch->fighting)
 {
    stc("Blink who?\n\r", ch);
    return;
 }

 if (*argument == '\0')
    victim = ch->fighting;
 else 
 if ((victim = get_char_room(ch,argument)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 if (ch->move < 5000)
 {
    stc("You need 5000 movement points to use that\n\r", ch);
    return;
 }

 ch->move -= 4000; // leave them some mv to flee with

 if (ch->fighting)
    stop_fighting(ch,TRUE);
 
 for (vch = ch->in_room->people; vch; vch = vch_next)
 {vch_next = vch->next_in_room;
    if (vch->fighting == ch)
	stop_fighting(vch,TRUE);
 }

 act("&Y$n pops out of existance.#n", ch,0,0,TO_ROOM);
 stc("&YYou pop out of existance.\n\r", ch);

 ch->blinkykill = victim;

 ch->flag3 |= AFF3_BLINK_1ST_RD;
WAIT_STATE(ch, PULSE_VIOLENCE*2);
 return;
}

void do_demonwall(CHAR_DATA *ch, char *argument)
{
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    ch->newbits ^= NEW_DEMONWALL;
    sprintf(log_buf, "&RDemonwall turned &Y%s&R.\n\r", 
	ch->newbits & NEW_DEMONWALL ? "on" : "off");
    stc(log_buf, ch);
    return;
}
