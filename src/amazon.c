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

void do_succour( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
/*    int ch_age = years_old(ch);*/
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_AMAZON) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Succour whom?\n\r", ch );
	return;
    }
/*
    if ( ch_age < 50 )
    {
       send_to_char("You must be 50 to succour Amazons!\n\r", ch);
       return;
    }
  */  
	if (ch->generation > 2)
	{
		send_to_char("Only Princesses and Queens may succour.\n\r", ch);
		return;
	}
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's!\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals!\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot succour yourself!\n\r", ch );
	return;
    }

    if ( victim->sex != 2 )
    {
	send_to_char("Only women may become Amazons!\n\r", ch);
	return;
    }

    if (IS_CLASS(victim, CLASS_MONK))
    {
	send_to_char( "Not on Monks!\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only succour avatars!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || 
IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))

    {
	send_to_char( "Not on vampires!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "Not on werewolves!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "Not on demons!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "Not on highlanders!\n\r", ch );
	return;
    }

   if (IS_CLASS(victim, CLASS_NINJA))
	{
	send_to_char( "Not on ninja!\n\r", ch);
	return;
	}
	

	if (IS_CLASS(victim, CLASS_DROW))
	{
	send_to_char( "Not on drow!\n\r", ch);
	return;
	}

	if (victim->class != 0)
	{
	send_to_char("You cannot guide a classed person!\n\r", ch);
	return;
	}

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot guide an unwilling person!\n\r", ch );
	return;
    }

    if (ch->exp < EXP(ch,10000000))
    {
	sprintf(log_buf, "You can't afford the %d exp required!\n\r", EXP(ch, 10000000));
	stc(log_buf, ch);
	return;

    }

    if (victim->exp < EXP(victim,10000000))
    {
	sprintf(log_buf, "They can't afford the %d exp required!\n\r", EXP(victim, 10000000));
	stc(log_buf, ch);
	return;
    }

    ch->exp -= EXP(ch,10000000);
    victim->exp -= EXP(victim,10000000);

    act("You make $N an Amazon!.", ch, NULL, victim, TO_CHAR);
    act("$n makes $N an Amazon!", ch, NULL, victim, TO_NOTVICT);
    act("$n makes you an Amazon!", ch, NULL, victim, TO_VICT);
    victim->level = LEVEL_ARCHMAGE;
    if (victim->trust > 6)
    victim->trust = victim->trust;
    else
    victim->trust = LEVEL_ARCHMAGE;
    send_to_char( "You are now an Amazon!\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_AMAZON;
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_drill( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_AMAZON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Drills Learned (%d).\n\r", ch->pcdata->powers[PAMAZON]);
	send_to_char(buf,ch);
	send_to_char("Powers you have:\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] < 1 )
	   send_to_char("You have no knowledge of any Drills.\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 1 )
	   send_to_char("cognizance.\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 2 )
	   send_to_char("Shield/Read Aura/Scry\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 3 )
	   send_to_char("Camouflage\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 4 )
	   send_to_char("Seduce\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 5 )
	   send_to_char("Net\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 6 )
	   send_to_char("Barkskin\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 7 )
	   send_to_char("Flare\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 8 )
	   send_to_char("Steelfist\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 9 )
	   send_to_char("You have completed your Drill training.\n\r", ch);
	return;
    }

	if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"power"))
	{
	  if ( ch->pcdata->powers[PAMAZON] < 1 )
	   send_to_char("You have no knowledge of any Drills.\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 1 )
	   send_to_char("Cognizance\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 2 )
	   send_to_char("Shield/Read Aura/Scry\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 3 )
	   send_to_char("Camouflage\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 4 )
	   send_to_char("Seduce\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 5 )
	   send_to_char("Net\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 6 )
	   send_to_char("Barkskin\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 7 )
	   send_to_char("Flare\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 8 )
	   send_to_char("Steelfist\n\r", ch);
	  if ( ch->pcdata->powers[PAMAZON] >= 9 )
	   send_to_char("You have completed your Drill training.\n\r",ch);
	  return;
	}
	sprintf(buf,"Drills Learned (%d).\n\r", ch->pcdata->powers[PAMAZON]);
	sprintf(buf, "\n\rType Drill power improve.\n\r");

	send_to_char(buf,ch);
	return;
    }

    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	if (!str_cmp(arg1,"power"   )) 
	{
	improve = PAMAZON;   
	max=8;
	}
	else
	{
	    send_to_char("~~~~~~~~~~~ DRILLS ~~~~~~~~~~~\n\r",ch);
	    send_to_char("1.     Cognizance             \n\r",ch);
	    send_to_char("2.     Shield/Read Aura/Scry  \n\r",ch);
	    send_to_char("3.     Camouflage             \n\r",ch);
	    send_to_char("4.     Seduce			\n\r",ch);
	    send_to_char("5.     Net		        \n\r",ch);
	    send_to_char("6.     Barkskin               \n\r",ch);
	    send_to_char("7.     Flare			\n\r",ch);
	    send_to_char("8.     Steelfist              \n\r",ch);
	    send_to_char("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\r",ch);
	    return;
	}						
	cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the known Drills.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal learn the Drill.\n\r", cost);

	    send_to_char(buf,ch);
	    return;
	}

	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You learn a new Drill.\n\r");
	send_to_char(buf,ch);
    }

    else send_to_char("To learn a Drill, type: Drill power improve.\n\r",ch);
	return;

}

void do_sisters( CHAR_DATA *ch, char *argument )

{

	 char buf[MAX_STRING_LENGTH];

	 char arg[MAX_INPUT_LENGTH];

	 CHAR_DATA *gch;



	 one_argument( argument, arg );



	 if (IS_NPC(ch)) return;

	 if (!IS_CLASS(ch, CLASS_AMAZON) )

	 {

	send_to_char("Huh?\n\r",ch);

	return;

	 }



	 if (strlen(ch->clan) < 2)

	 {

	send_to_char("But you don't belong to any clan!\n\r",ch);

	return;

	 }



	 sprintf( buf, "%s clan:\n\r", ch->clan );

	 send_to_char( buf, ch );

	 send_to_char("[      Name      ] [ Gen ] [ Hits   ] [ Mana   ] [ Move   ]\n\r", ch );

	 for ( gch = char_list; gch != NULL; gch = gch->next )

	 {

	if ( IS_NPC(gch) ) continue;

	if ( !IS_CLASS(gch, CLASS_AMAZON) ) continue;

	if ( !str_cmp(ch->clan,gch->clan) )

	{

		 sprintf( buf,

		 "[%-16s] [  %d  ] [%-6d%3d] [%-6d%3d] [%-6d%3d] [%7ld]\n\r",

		capitalize( gch->name ),

		gch->generation,

		gch->hit,  (gch->hit  * 100 / gch->max_hit ),

		gch->mana, (gch->mana * 100 / gch->max_mana),

		gch->move, (gch->move * 100 / gch->max_move),

					  gch->exp);

		send_to_char( buf, ch );

	}

	 }

	 return;

}


void do_cognizance( CHAR_DATA *ch, char *argument )

{

	 if (IS_NPC(ch)) return;



	if (!IS_CLASS(ch, CLASS_AMAZON) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}



	 if  (ch->pcdata->powers[PAMAZON] < 1 )

	{

	  send_to_char("You have not learned the Cognizance Drill.\n\r", ch);

	  return;

	}



	else if (ch->move < 750)

	{

		send_to_char("You don't have 750 move to increase your cognizance.\n\r", ch);

		return;

	} 



    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )

    {

	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "You're senses return to normal.\n\r", ch );

    }

    else

    {

	SET_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "You're senses increase into incredible proportions.\n\r", ch );

	ch->move -= 750; 

    }



    return;

}

void do_camouflage( CHAR_DATA *ch, char *argument)
{

    if ( IS_NPC(ch) )
     return;

    if (!IS_CLASS(ch, CLASS_AMAZON))
    {
	  send_to_char("Huh?\n\r",ch);
	  return;
    }

    if (ch->pcdata->powers[PAMAZON] < 3)
    {
	send_to_char("You have not learned the Camouflage Drill.\n\r", ch);
	return;
    }

    if ( IS_SET(ch->act, PLR_SACREDINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_SACREDINVIS);
	send_to_char( "You stop camouflaging yourself.\n\r", ch );
	act("$n slowly emerges into sight.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
	if (has_timer(ch)) return;

	send_to_char( "You camouflage yourself.\n\r", ch );
	act("$n slowly fades into the background.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->act, PLR_SACREDINVIS);
    }

    return;
}

void do_seduce( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
 
    argument = one_argument(argument,arg);
 
    if ( IS_NPC(ch) )
    return;

    if (!IS_CLASS(ch,CLASS_AMAZON))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[PAMAZON] < 4)
    {
	send_to_char("You have not learned the Seduction Drill.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Seduce Whom?\n\r",ch);
    return;
    }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They arent here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
        send_to_char( "You cannot Seduce yourself!\n\r", ch );
	return;
    }
/*    if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
    {
    send_to_char("You cannot attack them here.\n\r",ch);
    return;
    }*/
    if (is_safe(ch,victim)== TRUE)
    return;

    if ( victim->hit < victim->max_hit )
    {
        send_to_char( "They are hurt and alert.\n\r", ch );
	return;
    }
 
 
    act("You sing a seductive song to $N.",ch,NULL,victim,TO_CHAR);
    act("$n sings a seductive song to you.",ch,NULL,victim,TO_VICT);
    act("$n sings a seductive song to $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, 2 );
    if ( number_percent( ) < 50 )
    	dam = number_range(13,23);
    else
    {
    	dam = 0;
        send_to_char("You failed.\n\r",ch);
	return;
    }
 
    dam += char_damroll(ch);
    if (dam == 0) dam = 1;
    if ( !IS_AWAKE(victim) )
	dam *= 4;
 
    if ( dam <= 0 )
	dam = 3;
    victim->hit -= dam;
 
    act("You clutch your head in agony!",victim,NULL,NULL,TO_CHAR);
    act("$n clutches $n head in agony!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    if (IS_NPC(victim) && victim->hit < 0) 
    { 
    raw_kill(victim);
    return;
    }
    if (!IS_NPC(victim) && victim->hit < -2)
    update_pos(ch);
    return;
}

void do_net(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_AMAZON))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if  (ch->pcdata->powers[PAMAZON] < 5 )
    {
	send_to_char("You have not learned the Net Drill.\n\r",ch);
	return;
    }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
  send_to_char("You ready a net to throw over your enemy.\n\r",ch);
  SET_BIT(ch->newbits, NEW_JAWLOCK);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  { 
  send_to_char("You stow away your net.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
  return;
  }
return;
}

void do_barkskin( CHAR_DATA *ch, char *argument )

{

    if ( IS_NPC(ch) )

	return;

  

  	if (!IS_CLASS(ch, CLASS_AMAZON) )

  	{

check_social(ch,"bark",argument,TYPE_SOCIAL);

  	return;

	}
	 if  (ch->pcdata->powers[PAMAZON] < 6 )
               {
	send_to_char("You have not learned the Barkskin Drill.\n\r",ch);
	return;
    }

	

    if ( IS_SET(ch->newbits, NEW_SKIN) )

    {

	send_to_char( "Your skin softens.\n\r", ch );
        ch->armor += 100;
	REMOVE_BIT(ch->newbits, NEW_SKIN);
	return;
    }

    else if (!IS_SET(ch->newbits, NEW_SKIN))

    {

	send_to_char( "Your skin hardens like bark.\n\r", ch );
        ch->armor -= 100;
	SET_BIT(ch->newbits, NEW_SKIN);
	return;
    }

    return;

}

void do_genesis( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_AMAZON))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

   if (arg[0] == '\0')
    {
	send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
	send_to_char("Mask, Bracer, Lorica, Gloves, Cape, Boots or Lasso.\n\r", ch );
	return;
    }
    if      (!str_cmp(arg,"mask"     )) vnum = 27943;   
    else if (!str_cmp(arg,"bracer"   )) vnum = 27944;
    else if (!str_cmp(arg,"lorica"   )) vnum = 27945;
    else if (!str_cmp(arg,"gloves"   )) vnum = 27946;
    else if (!str_cmp(arg,"cape"     )) vnum = 27947;
    else if (!str_cmp(arg,"boots"    )) vnum = 27948;
    else if (!str_cmp(arg,"lasso"    )) vnum = 27949;
    else
    {
	send_to_char("Invalid argument, read the list again!\n\r", ch );
	return;
    }
    if (ch->practice < 60)
    {
	send_to_char("It costs 60 primal to create a piece of Amazon equipment.\n\r",ch);
	return;
    }
   
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Adreno.\n\r",ch);
	return;
    }
    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands in a flash of blinding light!",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a flash of blinding light!.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_flare( CHAR_DATA *ch, char *argument)

{

    if ( IS_NPC(ch) )

     return;



    if (!IS_CLASS(ch, CLASS_AMAZON))

    {

	  send_to_char("Huh?\n\r",ch);

	  return;

    }



    if (ch->pcdata->powers[PAMAZON] < 7)

    {

	  send_to_char("You have not learned the Flare Drill.\n\r",ch);

	  return;

    }



    if (IS_SET(ch->newbits, NEW_MONKFLAME))

    {

	  REMOVE_BIT(ch->newbits, NEW_MONKFLAME);

        send_to_char("Your hands cool and the glow fades.\n\r",ch);

	  act("$n's hands cool down and stop glowing.\n\r",ch,NULL,NULL,TO_ROOM);

	  return;

    }

    if (!IS_SET(ch->newbits, NEW_MONKFLAME))

    {

	  SET_BIT(ch->newbits, NEW_MONKFLAME);

	  send_to_char("Your hands begin to heat up and glow!\n\r",ch);

	  act("$n's hands begin to heat up and glow!\n\r",ch,NULL,NULL,TO_ROOM);

	  return;

    }

}



void do_steelfist( CHAR_DATA *ch, char *argument)

{

     if ( IS_NPC(ch) )

     return;



    if (!IS_CLASS(ch, CLASS_AMAZON))

    {

	  send_to_char("Huh?\n\r",ch);

	  return;

    }



    if (ch->pcdata->powers[PAMAZON] < 8 )

    {

	  send_to_char("You have not learned the Steelfist Drill.\n\r",ch);

	  return;

    }



    if (IS_SET(ch->newbits, NEW_MONKADAM))

    {

	  REMOVE_BIT(ch->newbits, NEW_MONKADAM);

          send_to_char("Your hands resume their normal colour.\n\r",ch);

	  act("$n's hands resume their normal colour.\n\r",ch,NULL,NULL,TO_ROOM);

	  return;

    }

    if (!IS_SET(ch->newbits, NEW_MONKADAM))

    {

	  SET_BIT(ch->newbits, NEW_MONKADAM);

	  send_to_char("Your hands turn to steel!\n\r",ch);

	  act("$n's hands turn to steel!\n\r",ch,NULL,NULL,TO_ROOM);

	  return;

    }

}

void do_combo( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_AMAZON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Combo moves learned (%d).\n\r", ch->pcdata->powers[AMA_COMBO]);
	send_to_char(buf,ch);
	send_to_char("Combo moves you have learned:\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] < 1 )
	   send_to_char("You have no knowledge of any Combo moves.\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 1 )
	   send_to_char("spinkick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 2 )
	   send_to_char("sweepkick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 3 )
	   send_to_char("axekick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 4 )
	   send_to_char("knee\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 5 )
	   send_to_char("backfist\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 6 )
	   send_to_char("forearm\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 7 )
	   send_to_char("You have completed your Combo training.\n\r", ch);
	return;
    }

	if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"power"))
	{
	  if ( ch->pcdata->powers[AMA_COMBO] < 1 )
	   send_to_char("You have no knowledge of any Combo moves.\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 1 )
	   send_to_char("spinkick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 2 )
	   send_to_char("sweepkick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 3 )
	   send_to_char("axekick\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 4 )
	   send_to_char("knee\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 5 )
	   send_to_char("backfist\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 6 )
	   send_to_char("forearm\n\r", ch);
	  if ( ch->pcdata->powers[AMA_COMBO] >= 7 )
	   send_to_char("You have completed your Combo training.\n\r",ch);
	  return;
	}
	sprintf(buf,"Combo moves learned (%d).\n\r", ch->pcdata->powers[AMA_COMBO]);
	sprintf(buf, "\n\rType combo power improve.\n\r");

	send_to_char(buf,ch);
	return;
    }

    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	if (!str_cmp(arg1,"power"   )) 
	{
	improve = AMA_COMBO;   
	max=6;
	}
	else
	{
	    send_to_char("++++++++ Combo  Moves ++++++++\n\r",ch);
	    send_to_char("1.       Spinkick             \n\r",ch);
	    send_to_char("2.       Sweepkick            \n\r",ch);
	    send_to_char("3.       Axekick              \n\r",ch);
	    send_to_char("4.       Knee	                \n\r",ch);
	    send_to_char("5.       Backfist             \n\r",ch);
	    send_to_char("6.       Forearm              \n\r",ch);
	    send_to_char("++++++++++++++++++++++++++++++\n\r",ch);
	    return;
	}						
	cost =  ((ch->pcdata->powers[improve] * 10) + 10);
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the known Combo moves.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	if (ch->practice < cost)
    {
	send_to_char("You do not have the required amount of primal\n\r",ch);
	return;
    }


	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You learn a new Combo move.\n\r");
	send_to_char(buf,ch);
    }

    else send_to_char("To learn a Combo move, type: combo power improve.\n\r",ch);
	return;

}
void do_praise(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
		return;

	else if (!IS_CLASS(ch, CLASS_AMAZON) )
    {
		send_to_char( "Huh?\n\r", ch );
		return;
    }
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->generation > 2)
	{
		send_to_char("You are not a princess or queen to do this\n\r", ch);
		return;
	}
    else if (( arg1[0] == '\0' || arg2[0] == '\0' ))
    {
		send_to_char( "Syntax is: praise <target> <banish/accept/priestess>\n\r", ch );
		return;
    }
    else if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    else if ( IS_NPC(victim) )
    {
		send_to_char( "Not on NPC's.\n\r", ch );
		return;
    }

    else if ( ch == victim )
    {
		send_to_char( "Not on yourself!\n\r", ch );
		return;
    }

    else if ( !IS_CLASS(victim,CLASS_AMAZON) )
    {
		send_to_char( "But they are not an amazon!\n\r", ch );
		return;
    }

	else if ( str_cmp(ch->clan, victim->clan) && !str_cmp(arg2, "banish") && (ch->generation == 2))
	{
		send_to_char("They are not in your clan\n\r", ch);
		return;
	}
    else if ( victim->generation < 3)
    {
		send_to_char( "You may not do this to a Princess.\n\r", ch );
		return;
    }

    else if ( !str_cmp(arg2,"banish") )
    {
		victim->generation = 4;
		act("You make $N a rogue amazon!",ch,NULL,victim,TO_CHAR);
		act("$n has made $N a rogue amazon!",ch,NULL,victim,TO_NOTVICT);
		act("$n has made you a rogue amazon!",ch,NULL,victim,TO_VICT);
		free_string(victim->clan);
		victim->clan = str_dup( "" );
		return;
    }
    
    else if ( !str_cmp(arg2,"accept") )
    {
		if (str_cmp(victim->clan, ""))
		{
			send_to_char ("You may not induct an amazon from another clan\n\r", ch);
			return;
		}
		act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
		act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
		act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
		free_string(victim->clan);
		victim->clan = str_dup( ch->clan );
		return;
    }
	else if (!str_cmp(arg2, "priestess") )
	{
		if (str_cmp(victim->clan, ch->clan))
		{
			send_to_char("Only on an amazon from your own clan.\n\r", ch);
			return;
		}
		if (victim->generation == 3)
		{
			act("You take $N's priestess away!",ch,NULL,victim,TO_CHAR);
			act("$n takess $N's priestess away!",ch,NULL,victim,TO_NOTVICT);
			act("$n takes your priestess away!",ch,NULL,victim,TO_VICT);
			victim->generation = 4;
			return;
		}
		else
		{
			victim->generation = 3;
			act("You make $N a priestess!",ch,NULL,victim,TO_CHAR);
			act("$n makes $N a priestess!",ch,NULL,victim,TO_NOTVICT);
			act("$n makes you a priestess!",ch,NULL,victim,TO_VICT);
			return;
		}
	}
	send_to_char( "Syntax is: praise <target> <accept/banish/priestess>\n\r", ch );
	return;
}

/* Combo Code Original Idea Avanti GW:Unplugged */
/* Reverse Engineered by Listy */

void do_spinkick( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	/* MOVE 1 */
	WAIT_STATE(ch, PULSE_VIOLENCE/2);
		if (ch->pcdata->stats[AMA_COMBO] == 0)
		{
			ch->pcdata->stats[AMA_COMBO] = 1;
			dam = 650;
			dam += number_range(10,175);

			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);

			act("The earth around you surrounds you in a hazy mist (COMBO 1 1st Move).",ch,NULL,victim,TO_CHAR);
    		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_VICT);
    		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[AMA_COMBO] == 4)
		{
			ch->pcdata->stats[AMA_COMBO] = 5;
			dam = 650;
			dam += number_range(25,160);
			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The hazy mist surround you turns a green tint (COMBO 3 2nd Move).",ch,NULL,victim,TO_CHAR);
   			act("$n's hazy mist turns green.",ch,NULL,victim,TO_VICT);
   			act("$n's hazy mist turns a green.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[AMA_COMBO] == 9)
		{
			ch->pcdata->stats[AMA_COMBO] = 10;
			dam = 690;
			dam += number_range(15,150);
			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The hazy mist around you rages violently (COMBO 4 4th Move).",ch,NULL,victim,TO_CHAR);
    		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
    		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
	else if (ch->pcdata->stats[AMA_COMBO] == 11)
	{
		ch->pcdata->stats[AMA_COMBO] = 12;
		dam = 750;
		dam += number_range(15,175);
		sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 5 2nd Move).",ch,NULL,victim,TO_CHAR);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 13)
	{
		ch->pcdata->stats[AMA_COMBO] = 14;
		dam = 850;
		dam += number_range(15,190);
		sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 5 4th Move).",ch,NULL,victim,TO_CHAR);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}

		else
		{
			if (ch->pcdata->stats[AMA_COMBO] > 0)
			{
				dam = 690;
				dam += number_range(5,110);
				sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
    			act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
    			act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
				ch->pcdata->stats[AMA_COMBO] = 0;
				victim->hit -= dam;
				return;
			}
			else
			{
				dam = 670;
				dam += number_range (5, 145);
				sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				victim->hit -= dam;
				return;
			}
		}
}
void do_sweepkick( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	int dam1;
	int dam2;
	int dam3;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	/* MOVE 2 */
	WAIT_STATE(ch, PULSE_VIOLENCE/2);
		if (ch->pcdata->stats[AMA_COMBO] == 0)
		{
			ch->pcdata->stats[AMA_COMBO] = 2;
			dam = 660;
			dam += number_range(10,170);

			sprintf(buf,"You drop to one knee, sweeping at $N's legs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n drops to one knee, sweeping at your legs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n drops to one knee and sweeps at $N's legs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			
			act("The earth around you surrounds you in a hazy mist (COMBO 2 1st Move).",ch,NULL,victim,TO_CHAR);
   			act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_VICT);
   			act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[AMA_COMBO] == 10)
		{
			ch->pcdata->stats[AMA_COMBO] = 0;
			dam = 760;
			dam += number_range(10,500);
			
			dam1 = dam * 1.0;
			dam2 = dam * 1.1;
			dam3 = dam * 1.2;

			sprintf(buf,"You release the build up of power from your body at $N(COMBO 4 End) [%d]\n\r",dam1);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n releases the hazy mist upon you [%d]\n\r",dam1);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n releases the hazy mist upon $N [%d]\n\r",dam1);
			act(buf,ch,NULL,victim,TO_NOTVICT);

			sprintf(buf,"You draw your hands back and thrust them deep within $N's chest [%d]\n\r",dam2);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n drives their hands into your chest [%d]\n\r",dam2);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n drives their hands into $N's chest [%d]\n\r",dam2);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			
			sprintf(buf,"You focus your powers and sear $N's chest with your power [%d]\n\r",dam3);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n releases the power upon your chest, you are hurled back [%d]\n\r",dam3);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n releases the power into $N's chest, hurling $N back [%d]\n\r",dam3);
			act(buf,ch,NULL,victim,TO_NOTVICT);

			victim->hit -= dam;
			return;
		}
		else
		{
			if (ch->pcdata->stats[AMA_COMBO] > 0)
			{
				dam = 760;
				dam += number_range(10, 150);
				sprintf(buf,"You drop to one knee, sweeping at $N's legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n drops to one knee, sweeping at your legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n drops to one knee and sweeps at $N's legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
    			act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
    			act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
				ch->pcdata->stats[AMA_COMBO] = 0;
				victim->hit -= dam;
				return;
			}
			else
			{
				dam = 750;
				dam += number_range (10, 150);
				sprintf(buf,"You drop to one knee, sweeping at $N's legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n drops to one knee, sweeping at your legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n drops to one knee and sweeps at $N's legs [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				victim->hit -= dam;
				return;
			}
		}
}
void do_axekick( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	/* MOVE 3 */
	WAIT_STATE(ch, PULSE_VIOLENCE/2);

		if (ch->pcdata->stats[AMA_COMBO] == 1)
		{
			ch->pcdata->stats[AMA_COMBO] = 0;
			dam = 760;
			dam += number_range(10,150);

			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			
			act("You release the power of the Earth at $N (COMBO 1 End).",ch,NULL,victim,TO_CHAR);
   			act("$n's hazy mist coils violently around you.",ch,NULL,victim,TO_VICT);
   			act("$n's hazy mist coils violently around $N.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[AMA_COMBO] == 5)
		{
			ch->pcdata->stats[AMA_COMBO] = 6;
			dam = 750;
			dam += number_range(10,150);
			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The haze surrounding you glows blue (COMBO 3 3rd move).",ch,NULL,victim,TO_CHAR);
    			act("$n's green hazy mist glows blue.",ch,NULL,victim,TO_VICT);
    			act("$n's green hazy mist glows blue.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[AMA_COMBO] == 14)
		{
			ch->pcdata->stats[AMA_COMBO] = 15;
			dam = 850;
			dam += number_range(10,150);
			sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);

			act("The hazy mist around you rages violently (COMBO 5 5th Move).",ch,NULL,victim,TO_CHAR);
   			act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
   			act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
		else
		{
			if (ch->pcdata->stats[AMA_COMBO] > 0)
			{
				dam = 750;
				dam += number_range(5, 125);
				sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
    				act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
    				act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
				ch->pcdata->stats[AMA_COMBO] = 0;
				victim->hit -= dam;
				return;
			}
			else
			{
				dam = 760;
				dam += number_range (1, 220);
				sprintf(buf,"You spin on one foot and strike $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_CHAR);
				sprintf(buf,"$n spins on one foot and strikes you [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_VICT);
				sprintf(buf,"$n spins on one foot and strikes $N [%d]\n\r",dam);
				act(buf,ch,NULL,victim,TO_NOTVICT);
				victim->hit -= dam;
				return;
			}
		}
}
void do_forearm( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	int dam1;
	int dam2;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];	
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}

	/* MOVE 4 */
	WAIT_STATE(ch, PULSE_VIOLENCE/1);
	if (ch->pcdata->stats[AMA_COMBO] == 2)
	{
		ch->pcdata->stats[AMA_COMBO] = 3;
		dam = 750;
		dam += number_range(10,150);
		sprintf(buf,"You lash out a flurry of fists at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n punches you furiously [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n throws a flurry of punches at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 2 2nd Move).",ch,NULL,victim,TO_CHAR);
  		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
  		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 6)
	{
		ch->pcdata->stats[AMA_COMBO] = 0;
		dam = 850;
		dam += number_range(10,190);
		sprintf(buf,"You release a battle cry lunging at your $N's throat as the trees lash out with their branches (COMBO 3 End) [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"You are lashed with branches as $n reaches for your throat [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"Branches lash at $N as $n reaches for $N's throat [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 0)
	{
		ch->pcdata->stats[AMA_COMBO] = 7;
		dam = 810;
		dam += number_range(10, 150);
		sprintf(buf,"You lash out a flurry of fists at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n punches you furiously [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n throws a flurry of punches at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The earth around you surrounds you in a hazy mist (COMBO 4 1st Move).",ch,NULL,victim,TO_CHAR);
   		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_VICT);
   		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 7)
	{
		ch->pcdata->stats[AMA_COMBO] = 8;
		dam = 880;
		dam += number_range(10,150);
		sprintf(buf,"You lash out a flurry of fists at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n punches you furiously [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n throws a flurry of punches at $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 4 2nd Move).",ch,NULL,victim,TO_CHAR);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 15)
	{
		ch->pcdata->stats[AMA_COMBO] = 0;
		dam = 1360;
		dam += number_range(81,459);
		dam1 = dam * 2;
		dam2 = dam * 2.2;
		act("You are possessed by Artemis (COMBO 5 End).",ch,NULL,victim,TO_CHAR);
		act("$n's raging mist forms into Artemis.",ch,NULL,victim,TO_VICT);
		act("$n's raging mists form into Artemis.",ch,NULL,victim,TO_NOTVICT);

		sprintf(buf,"You lash out with your godly force at $N [%d]\n\r",dam1);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"Artemis' form strikes you with godly force [%d]\n\r",dam1);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"Artemis' godly form lashes out at $N [%d]\n\r",dam1);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam1;

		act("You raise your arms high in the air.",ch,NULL,victim,TO_CHAR);
		act("Artemis raises her arms in the air.",ch,NULL,victim,TO_VICT);
		act("Artemis raises her arms in the air.",ch,NULL,victim,TO_NOTVICT);

		act("You summon all the powers of the heavens down upon $N.",ch,NULL,victim,TO_CHAR);
		act("Artemis summons the heavens to crash down upon you.",ch,NULL,victim,TO_VICT);
		act("Artemis summons the heavens to crash down upon $N.",ch,NULL,victim,TO_NOTVICT);

		sprintf(buf,"You rain destruction down upon $N [%d]\n\r",dam2);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"Artemis swamps you in complete destruction [%d]\n\r",dam2);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"Artemis lets loose the raw power of the heavens upon $N [%d]\n\r",dam2);
		act(buf,ch,NULL,victim,TO_NOTVICT);

		act("Artemis leaves your soul.",ch,NULL,victim,TO_CHAR);
		act("$n's form shimmers back into existence.",ch,NULL,victim,TO_VICT);
		act("$n's form shimmers back into existance.",ch,NULL,victim,TO_NOTVICT);

		victim->hit -= dam2;
		return;
	}
	else
	{
		if (ch->pcdata->stats[AMA_COMBO] > 0)
		{
			dam = 850;
			dam += number_range(5, 225);
			sprintf(buf,"You lash out a flurry of fists at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n punches you furiously [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n throws a flurry of punches at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
   			act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
   			act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
			ch->pcdata->stats[AMA_COMBO] = 0;
			victim->hit -= dam;
			return;
		}
		else
		{
			dam = 850;
			dam += number_range (1, 120);
			sprintf(buf,"You lash out a flurry of fists at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n punches you furiously [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n throws a flurry of punches at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
	}
}
void do_knee( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];		
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	/* MOVE 5 */
	WAIT_STATE(ch, PULSE_VIOLENCE/1);
	if (ch->pcdata->stats[AMA_COMBO] == 0)
	{
		ch->pcdata->stats[AMA_COMBO] = 11;
		dam = 750;
		dam += number_range(10, 50);
		sprintf(buf,"You bring your knee to $N's ribs [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n brings their knee to your ribs [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n's vicious knee attack strikes $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The earth around you surrounds you in a hazy mist (COMBO 5 1st Move).",ch,NULL,victim,TO_CHAR);
		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_VICT);
		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}

	if (ch->pcdata->stats[AMA_COMBO] == 3)
	{
		ch->pcdata->stats[AMA_COMBO] = 0;
		dam = 1450;
		dam += number_range(10,220);
		sprintf(buf,"Your hazy mist encompasses $N sucking their life force (COMBO 2 End) [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n's hazy mist draws energy from you [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n's hazy mist draws life force from $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		if ((ch->hit + dam) >= (ch->max_hit * 1.5))
			ch->hit = ch->max_hit * 1.5;
		else
			ch->hit += dam;
		return;
	}
	if (ch->pcdata->stats[AMA_COMBO] == 8)
	{
		ch->pcdata->stats[AMA_COMBO] = 9;
		dam = 760;
		dam += number_range(5, 215);
		sprintf(buf,"You bring your knee to $N's ribs [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n brings their knee to your ribs [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n's vicious knee attack strikes $N [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 4 3nd Move).",ch,NULL,victim,TO_CHAR);
		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
	}
	else
	{
		if (ch->pcdata->stats[AMA_COMBO] > 0)
		{
			dam = 790;
			dam += number_range(5, 225);
			sprintf(buf,"You bring your knee to $N's ribs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n brings their knee to your ribs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n's vicious knee attack strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
			act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
			act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
			ch->pcdata->stats[AMA_COMBO] = 0;
			victim->hit -= dam;
			return;
		}
		else
		{
			dam = 790;
			dam += number_range (1, 120);
			sprintf(buf,"You bring your knee to $N's ribs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n brings their knee to your ribs [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n's vicious knee attack strikes $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
	}
}
void do_backfist( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];			
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}

	/* MOVE 6 */
	WAIT_STATE(ch, PULSE_VIOLENCE/1);
	if (ch->pcdata->stats[AMA_COMBO] == 0)
	{
		ch->pcdata->stats[AMA_COMBO] = 4;
		dam = 740;
		dam += number_range(10,150);
		sprintf(buf,"You spin around smashing your fist into $N's face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n spins around smashing their fist in your face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n spins around smashing their fist into $N's face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The earth around you surrounds you in a hazy mist (COMBO 3 1st Move).",ch,NULL,victim,TO_CHAR);
   		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_VICT);
   		act("$n is surrounded by a hazy mist.",ch,NULL,victim,TO_NOTVICT);
		victim->hit -= dam;
		return;
	}
	else if (ch->pcdata->stats[AMA_COMBO] == 12)
	{
		ch->pcdata->stats[AMA_COMBO] = 13;
		dam = 800;
		dam += number_range(10,150);
		sprintf(buf,"You spin around smashing your fist into $N's face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n spins around smashing their fist in your face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"$n spins around smashing their fist into $N's face [%d]\n\r",dam);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		act("The hazy mist around you rages violently (COMBO 5 3rd Move).",ch,NULL,victim,TO_CHAR);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_VICT);
   		act("The hazy mist around $n rages more violently.",ch,NULL,victim,TO_NOTVICT);

		return;
	}
	else
	{
		if (ch->pcdata->stats[AMA_COMBO] > 0)
		{
			dam = 850;
			dam += number_range(5, 125);
			sprintf(buf,"You spin around smashing your fist into $N's face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins around smashing their fist in your face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins around smashing their fist into $N's face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			act("The hazy mist around you fades.",ch,NULL,victim,TO_CHAR);
   			act("$n's hazy mist fades away.",ch,NULL,victim,TO_VICT);
   			act("$n's hazy mist fades away.",ch,NULL,victim,TO_NOTVICT);
			ch->pcdata->stats[AMA_COMBO] = 0;
			victim->hit -= dam;
			return;
		}
		else
		{
			dam = 750;
			dam += number_range (10, 50);
			sprintf(buf,"You spin around smashing your fist into $N's face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			sprintf(buf,"$n spins around smashing their fist in your face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			sprintf(buf,"$n spins around smashing their fist into $N's face [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			victim->hit -= dam;
			return;
		}
	}
}

void do_slavecommand( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	argument = one_argument(argument, arg1);
	
	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_AMAZON))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	
	if (ch->sex != SEX_FEMALE)
	{
		send_to_char("Only female amazons may do this!\n\r",ch);
		return;
	}
	
		if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
        send_to_char( "Order which slave?\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
        {
            send_to_char( "They aren't here.\n\r", ch );
				return;
		  }
if (victim->trust > 6
&& victim->trust > ch->trust) {stc("Nope.\n\r",ch);return;}
	    if ( victim == ch )
    {
        send_to_char( "How can you command yourself??\n\r", ch );
        return;
    }
	
	if (victim->sex == SEX_FEMALE)
	{
		send_to_char("You may only order amazon slaves!\n\r",ch);
		return;
	}
	
  	if (!IS_CLASS(victim, CLASS_AMAZON))
	{
	send_to_char("Only amazon slaves!\n\r",ch);
	return;
	}

    
	act("You order $N around.\n\r",ch,NULL,victim,TO_CHAR);
	act("$n orders you to '$t'!\n\r",ch,argument,victim,TO_VICT);
	act("$n orders $N around.\n\r",ch,NULL,victim,TO_NOTVICT);
	interpret( victim, argument );
WAIT_STATE(ch, PULSE_VIOLENCE/3);
}

void do_slaveinsight( CHAR_DATA *ch, char *argument)
{
	 CHAR_DATA *victim;
	 char buf[MAX_STRING_LENGTH];
	 char arg[MAX_INPUT_LENGTH];
	 argument = one_argument(argument, arg);

	 if ( IS_NPC(ch) )
	return;

	 if (!IS_CLASS(ch, CLASS_AMAZON))
	 {
	send_to_char("Huh?\n\r",ch);
	return;
	 }

    	if (ch->sex != SEX_FEMALE)
	{
		send_to_char("Only female amazons may do this!\n\r",ch);
		return;
	}
		 if  (ch->pcdata->powers[PAMAZON] < 8 )

	{

	  send_to_char("You have not learned the slaveinsight Drill.\n\r", ch);

	  return;

	}

	 if (ch->move < 2000)
	 {
	send_to_char("You Cant pick up enough speed to do that!\n\r",ch);
	return;
	 }
	 if (ch->fight_timer > 0)
	 {
		 send_to_char("You may not Run thru a slave with a fight timer.\n\r", ch);
		 return;
	 }
	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Use your slaves life-force to get to who?\n\r", ch );
	return;
	 }

	 if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	 {
	send_to_char( "They aren't here.\n\r", ch );
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
	send_to_char("You cannot seem to pick up enough speed.\n\r", ch);
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
		send_to_char("You may not Run through a Slave whilst mortal.\n\r", ch);
		return;
	}

	 char_from_room(ch);
	 char_to_room(ch,victim->in_room);

	 if (IS_NPC(victim))

	 {
	sprintf(buf,"You Call for your slave and he comes Running in and bows before you, slowly you back up And Charge for him Shouting 'SHOW ME THE WAY SLAVE!' And his body tears thru bringing you the way to %s!\n\r",victim->short_descr);
	send_to_char(buf, ch);
	 }

	 if (!IS_NPC(victim))
	 {
	sprintf(buf,"You Call for your slave and he comes Running in and bows before you, slowly you back up And Charge for him Shouting 'SHOW ME THE WAY SLAVE!' And his body tears thru bringing you the way to %s!\n\r",victim->name);
	send_to_char(buf, ch);
	WAIT_STATE(ch, 6);
	 }
	 act("$n Comes charging at you Carrying a chain attached to a young man dragged through the portal with her!\n\r",ch,NULL,NULL,TO_ROOM);
	 do_look(ch, "auto");
	 ch->move = ch->move - 1000;
	 return;
}
void do_amazonworship( CHAR_DATA *ch, char *argument)
{
	 CHAR_DATA *victim;
	 char buf[MAX_STRING_LENGTH];
	 char arg[MAX_INPUT_LENGTH];
	 argument = one_argument(argument, arg);

	 if ( IS_NPC(ch) )
	return;

	 if (!IS_CLASS(ch, CLASS_AMAZON))
	 {
	send_to_char("Huh?\n\r",ch);
	return;
	 }

		if (ch->sex != SEX_MALE)
	{
		send_to_char("Only amazon Slaves may do this!\n\r",ch);
		return;
	}
		 if  (ch->pcdata->powers[PAMAZON] < 8 )

	{

	  send_to_char("You have not learned the amazonworship Drill.\n\r", ch);

	  return;

	}

	 if (ch->move < 2000)
	 {
	send_to_char("Youre owner wont allow you to do that!\n\r",ch);
	return;
	 }
	 if (ch->fight_timer > 0)
	 {
		 send_to_char("You may not worship your mistress with a fight timer.\n\r", ch);
		 return;
	 }
	 if ( arg[0] == '\0' )
	 {
	send_to_char( "Sacrifice your heart to your mistress to get to who?\n\r", ch );
	return;
	 }

	 if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	 {
	send_to_char( "They aren't here.\n\r", ch );
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
	send_to_char("She refuses to take you with her through your insight.\n\r", ch);
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
		send_to_char("You may not do this whilst mortal.\n\r", ch);
		return;
	}

	 char_from_room(ch);
	 char_to_room(ch,victim->in_room);

	 if (IS_NPC(victim))

	 {
	sprintf(buf,"You beg your Amazon owner to take you through your insight to %s!\n\r",victim->short_descr);
	send_to_char(buf, ch);
	 }

	 if (!IS_NPC(victim))
	 {
	sprintf(buf,"You beg your Amazon owner to take you through your insight to %s!\n\r",victim->name);
	send_to_char(buf, ch);
	WAIT_STATE(ch, 6);
	 }
	 act("An amazon Comes charging at you Carrying a chain Pulling $n through the portal with her!\n\r",ch,NULL,NULL,TO_ROOM);
	 do_look(ch, "auto");
	 ch->move = ch->move - 1000;
	 return;
}

void do_artemisworship( CHAR_DATA *ch, char *argument )
 {
    int sn;
    int spelltype;

    if ( IS_NPC(ch)) return;

        if (!IS_CLASS(ch, CLASS_AMAZON))
        {
        send_to_char("Huh?\n\r", ch);
        return;
        }

        if (ch->mana < 500)
        {
        send_to_char("You don't have enough mana.\n\r", ch );
        return;
       }

    if ( ( sn = skill_lookup( "amazonbless" ) ) < 0 ) return;

      spelltype = skill_table[sn].target;

    (*skill_table[sn].spell_fun) ( sn, 300, ch, ch );

    WAIT_STATE( ch, 12 );

    ch->mana = ch->mana - 500;

    return;

}

