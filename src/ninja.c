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



void do_principles( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if (IS_NPC(ch)) return;



    if (!IS_CLASS(ch, CLASS_NINJA))

    {

	send_to_char("Huh?\n\r",ch);

	return;

    }



    if (arg1[0] == '\0' && arg2[0] == '\0')

    {

	sprintf(buf,"Principles: &BSora &R(&Y%d&R)&B, Chikyu &R(&Y%d&R)&B, Ningenno &R(&Y%d&R)#n\n\r",

	ch->pcdata->powers[NPOWER_SORA], ch->pcdata->powers[NPOWER_CHIKYU],

	ch->pcdata->powers[NPOWER_NINGENNO]);

	send_to_char(buf,ch);
	sprintf(log_buf, "            &BHamashaka &R(&Y%d&R)#n\n\r", ch->pcdata->powers[NPOWER_HAMASHAKA]);
	stc(log_buf, ch);

	return;

    }

    if (arg2[0] == '\0')

    {

	if (!str_cmp(arg1,"hamashaka"))
	{
#define inf(a,b,c) if (ch->pcdata->powers[NPOWER_HAMASHAKA] >= a){\
	sprintf(log_buf, "&Y%-15s &Y- &C%s#n\n\r", b,c);\
	stc(log_buf, ch);}
	    inf(0,"&CHamashaka","&Ymental enhancement.")

	    if (ch->pcdata->powers[NPOWER_HAMASHAKA] == 0)
		stc("&RYou have no hamashaka principles.#n\n\r", ch);

	    inf(1,"Tsunaki","Devastating punch to the ribs")
	    inf(2,"Jusitori","Incomparable redirection ability.")
	    inf(3,"Jeritsu","Initiation attack")
	    inf(4,"Yashakin","Shielding ability.")
	    inf(5,"Konochi","Blast-pack")
	    inf(6,"Youzame","Enhanced circle")
	    inf(7,"Taosu","Enhanced palm strike to the ribs")
	    return;
#undef inf
	}
	if (!str_cmp(arg1,"sora"))

	{
#define inf(a,b,c) if (ch->pcdata->powers[NPOWER_SORA] >= a){\
	sprintf(log_buf, "&B%-15s &Y- &R%s#n\n\r", b,c);\
	stc(log_buf, ch);}

	inf(0,"&RSora","&BThe finding, observing, and locating principle.")

	    if (ch->pcdata->powers[NPOWER_SORA] < 1) 

		send_to_char("You have none of the Sora principles.\n\r",ch);
	    inf(1,"Mitsukeru","The scrying power to find an enemy.")
	    inf(2,"Koryou","The read aura power, to learn about the enemy.")
	    inf(3,"Kakusu","Enhanced hide. Most people can not see you.")
	    inf(4,"Uro-Uro","You leave no footprints behind.")
	    inf(5,"Kanzuite","The truesight power.")
	    inf(6,"Bomuzite","By mixing a potion, you can put people in the room to sleep.")
	    return;
#undef inf
	}

	else if (!str_cmp(arg1,"chikyu"))

	{

#define inf(a,b,c) if (ch->pcdata->powers[NPOWER_CHIKYU] >= a){\
	sprintf(log_buf, "&G%-15s &Y- &M%s#n\n\r", b,c);\
	stc(log_buf, ch);}

	    inf(0,"&MChikyu","&GPreperation for battle.")
	    if (ch->pcdata->powers[NPOWER_CHIKYU] == 0)
		send_to_char("You have none of the Chikyu principles.\n\r",ch);
	    inf(1,"Tsuyoku","Toughness of the boar")
	    inf(2,"Songai","Damage of the bear")
	    inf(3,"Isogu","Haste of the rabbit")
	    inf(4,"Tsuiseki","Hunt of the fox")
	    inf(5,"Sakeru","Speed of the Cougar")

	    return;
#undef inf
	}
                        
	else if (!str_cmp(arg1,"ningenno"))

	{
#define inf(a,b,c) if (ch->pcdata->powers[NPOWER_CHIKYU] >= a){\
	sprintf(log_buf, "&b%-15s &Y- &r%s#n\n\r", b,c);\
	stc(log_buf, ch);}
	    inf(0,"&rNingenno","&bThe battle, attacking, and getting away.")
	    if (ch->pcdata->powers[NPOWER_NINGENNO] < 1) 
		send_to_char("You have none of the Ningenno principles.\n\r",ch);

	    inf(1,"Tsume","IronClaw always worn on wrist to aid in unarmed combat.")
	    inf(2,"Hakunetsu","The power of the first strike exposed.")
	    inf(3,"Mienaku","Never fail to flee (type mienaku,not flee)")
	    inf(4,"Shiroken","Added attack per round")
	    inf(5,"Dokuyaku","Adds poison to your weapons.")

	    return;
#undef inf
	}

	do_principles(ch,"");

	return;

    }

    if (!str_cmp(arg2,"improve"))

    {

	int improve;

	int cost;

	int max;



	     if (!str_cmp(arg1,"sora"   )) {improve = NPOWER_SORA;   max=6;}

	else if (!str_cmp(arg1,"chikyu"   )) {improve = NPOWER_CHIKYU;   max=5;}

	else if (!str_cmp(arg1,"ningenno"   )) {improve = NPOWER_NINGENNO;   max=5;}

	else if (!str_cmp(arg1,"hamashaka")){improve=NPOWER_HAMASHAKA; max=8;}

	else

	{

	    send_to_char("Principles: Sora, Chikyu, Ningenno.\n\r",ch);

	    return;

	}							

	cost = (ch->pcdata->powers[improve]+1) * (improve==NPOWER_HAMASHAKA?40:10);

	arg1[0] = UPPER(arg1[0]);

	if ( ch->pcdata->powers[improve] >= max )

	{

	    sprintf(buf,"You have already gained all the powers of the %s principle.\n\r", arg1);

	    send_to_char(buf,ch);

	    return;

	}

	if ( cost > ch->practice )

	{

	    sprintf(buf,"It costs you %d primal to improve your %s principle.\n\r", cost, arg1);

	    send_to_char(buf,ch);

	    return;

	}

	ch->pcdata->powers[improve] += 1;

	ch->practice -= cost;

	sprintf(buf,"You improve your ability in the %s principle.\n\r", arg1);

	send_to_char(buf,ch);

    }

    else send_to_char("To improve a principle, type: Principle <principle type> improve.\n\r",ch);

	return;

}


/*
void do_michi(  CHAR_DATA *ch, char *argument  )

{

    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}



  if ( IS_CLASS(ch, CLASS_NINJA) )

  {

	

	if ( ch->rage >= 50 )

	{

	    send_to_char("But you are already in the state of Michi.\n\r",ch);

	    return;

	}

	if ( ch->move < 1000 )

	{

		send_to_char("But you don't have enough move to perform the michi.\n\r", ch);

		return;

	} 

	

	send_to_char("You are gifted positive energy while performing the michi.\n\r",ch);

	act("$n is gifted positives energy while performing the michi.",ch,NULL,NULL,TO_ROOM);

	ch->rage += 200;

	ch->move -= 1000; 

	WAIT_STATE(ch,12);

	return;

  }

	else

    send_to_char("But you are already in the state of Michi.\n\r",ch);

    return;

}
*/


void do_discipline( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    char      arg [MAX_INPUT_LENGTH];



    argument = one_argument( argument, arg );



    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

	    

    if (IS_CLASS(ch, CLASS_NINJA) && !IS_SET(ch->special, SPC_PRINCE)

        && ch->generation != 1

        && ch->generation != 2 )

    {

	send_to_char("You do not have disciplining abilities.\n\r",ch);

	return;

    }

    

    if ( arg[0] == '\0' )

    {

	send_to_char( "Discipline whom?\n\r", ch );

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



    if ( IS_IMMORTAL(victim) )

    {

	send_to_char( "Not on Immortals's.\n\r", ch );

	return;

    }



    if ( ch == victim )

    {

	send_to_char( "You cannot discipline yourself.\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_NINJA))

    {

	send_to_char( "They are already disciplined.\n\r", ch );

	return;

    }



    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )

    {

	send_to_char( "You can only teach avatars.\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))

    {

	send_to_char( "You are unable to discipline vampires!\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_WEREWOLF))

    {

	send_to_char( "You are unable to discipline werewolves!\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))

    {

	send_to_char( "You are unable to discipline demons!\n\r", ch );

	return;

    }





    if (IS_CLASS(victim, CLASS_HIGHLANDER))

    {

	send_to_char( "You are unable to discipline highlanders.\n\r", ch );

	return;

    }

    

    if (IS_CLASS(victim, CLASS_DROW))

    {

    send_to_char( "Not on drows!\n\r",ch);

    return;

    }

    

/*


    

    if (IS_CLASS(victim, CLASS_PALADIN))

    {

    send_to_char("Not on paladins!\n\r", ch);

    return;

    }

*/
   if (victim->class != 0)
	{
	send_to_char("You cannot discipline a classed person.\n\r", ch);
	return;
	}


    if (!IS_IMMUNE(victim,IMM_VAMPIRE))

    {

	send_to_char( "You cannot discipline an unwilling person.\n\r", ch );

	return;

    }



    if (ch->exp < 100000)

    {

	send_to_char("You cannot afford the 100000 exp required to discipline them.\n\r",ch);

	return;

    }



    if (victim->exp < 100000)

    {

	send_to_char("They cannot afford the 100000 exp required to be disciplined from you.\n\r",ch);

	return;

    }



    ch->exp -= 100000;

    victim->exp -= 100000;



    act("You teach $N the ways of the ninja.", ch, NULL, victim, TO_CHAR);

    act("$n teaches $N the ways of the ninja.", ch, NULL, victim, TO_NOTVICT);

    act("$n teaches you the way of the ninja.", ch, NULL, victim, TO_VICT);

    victim->class    = CLASS_NINJA;

    victim->level = LEVEL_ARCHMAGE;

    victim->pcdata->powers[NPOWER_NINGENNO] = 0;
    victim->pcdata->powers[NPOWER_CHIKYU] = 0;
    victim->pcdata->powers[NPOWER_SORA] = 0;

    if (victim->trust > 6)

    victim->trust = victim->trust;

    else

    victim->trust = LEVEL_ARCHMAGE;

    
    victim->generation = ch->generation + 1;


    send_to_char( "You are now a ninja.\n\r", victim );

    free_string(victim->lord);

    victim->lord = str_dup(ch->name);

	free_string(victim->clan);

    victim->clan=str_dup(ch->clan);

    victim->generation = ch->generation + 1;

    save_char_obj(ch);

    save_char_obj(victim);

    return;

}



void do_kakusu( CHAR_DATA *ch, char *argument )

{

    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

	

	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_SORA] < 3 )

	{

	  send_to_char("You have not learned the Sora principle to 3.\n\r", ch);

	  return;

	}

	

	if (ch->move < 500)

	{

		send_to_char("You don't have 500 move to activate your power.\n\r", ch);

		return;

	} 

	if (ch->fight_timer > 0)
	{
		send_to_char("You may not enter the shadows whilst you are still tainted from a fight.\n\r", ch);
		return;
	}

    if ( IS_SET(ch->act, PLR_SACREDINVIS) )

    {

	REMOVE_BIT(ch->act, PLR_SACREDINVIS);

	act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );

	send_to_char( "You appear from the shadows.\n\r", ch );

    }

    else

    {

	act( "$n disappears into the shadows.", ch, NULL, NULL, TO_ROOM );

	send_to_char( "You disappear into the shadows.\n\r", ch );

	SET_BIT(ch->act, PLR_SACREDINVIS);

	ch->move -= 500; 

    }

    return;

}



void do_kanzuite( CHAR_DATA *ch, char *argument )

{

    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

	

    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_SORA] < 5 )

	{

	  send_to_char("You have not learned the Sora principle to 5.\n\r", ch);

	  return;

	}

	

	else if (ch->move < 500)

	{

		send_to_char("You don't have 500 move to increase your awareness.\n\r", ch);

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

	ch->move -= 500; 

    }



    return;

}



void do_mienaku( CHAR_DATA *ch, char *argument )

{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] < 3 )
	{
		send_to_char("You have not learned the Ningenno principle to 3.\n\r", ch);
		return;
	}
	else if (!IS_CLASS(ch, CLASS_NINJA) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

	if ( IS_NPC(ch) )
		return;
    if ( ( victim = ch->fighting ) == NULL )
    {
		if ( ch->position == POS_FIGHTING )
		    ch->position = POS_STANDING;
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }
    if (ch->special & SPC_GOLEMBIND)
    {stc("Your feet are too hard to flee away!\n\r", ch);return;}
	if(ch->move <= 0)
	{
	   send_to_char("You can't find the energy to leave.\n\r",ch);
	   return;
	}
	if (ch->move < 2500)
	{
		send_to_char("You don't have enough movement points to flee.\n\r", ch);
		return;
	} 
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
		send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
		return;
	}
    was_in = ch->in_room;
	{
		EXIT_DATA *pexit;
		int door;
		for ( attempt =  0; attempt < 6; attempt++ ) 
		{
			door = number_door( );
			if ( ( pexit = was_in->exit[door] ) == 0 
				||   pexit->to_room == NULL
				||   IS_SET(pexit->exit_info, EX_CLOSED)
				|| ( IS_NPC(ch)
				&&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
				continue;
			move_char( ch, door );
			if ( ( now_in = ch->in_room ) == was_in )
				continue;
			/* Use escape instead of flee so people know it's the ninja power */
			ch->in_room = was_in;
			act( "$n has escaped!", ch, NULL, NULL, TO_ROOM );
			ch->in_room = now_in;
			if ( !IS_NPC(ch) )
				send_to_char( "You escape from combat!\n\r", ch ); 
			ch->move -= 2500;
			ch->fight_timer += 5;
			stop_fighting( ch, TRUE );
			return;
		}
	}
//    send_to_char( "BUG: Inform an Implmentor\n\r", ch );
    return;
}



void do_target( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    char      arg1 [MAX_INPUT_LENGTH];

    char      arg2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

	    

    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

	send_to_char( "Syntax is: target <person> <prince/outcast/induct>\n\r", ch );

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



    if ( !IS_CLASS(victim,CLASS_NINJA) )

    {

	send_to_char( "But they are not a ninja!\n\r", ch );

	return;

    }



    if ( str_cmp(victim->clan,ch->clan) && str_cmp(arg2,"induct") )

    {

	send_to_char( "You can only grant your target to someone in your clan.\n\r", ch );

	return;

    }



    if ( ch->generation >= victim->generation )

    {

	send_to_char( "You can only grant your target to someone of a lower generation.\n\r", ch );

	return;

    }



    if ( !str_cmp(arg2,"prince") && ch->generation == 2)

    {

	if (IS_SET(victim->special,SPC_PRINCE)) 

	{

	act("You remove $N's disciplining privilages!",ch,NULL,victim,TO_CHAR);

	act("$n removes $N's disciplining privilages!",ch,NULL,victim,TO_NOTVICT);

	act("$n removes your disciplining privilages!",ch,NULL,victim,TO_VICT);

	REMOVE_BIT(victim->special,SPC_PRINCE);

	return;

	}

	act("You make $N a discipliner!",ch,NULL,victim,TO_CHAR);

	act("$n has made $N a discipliner!",ch,NULL,victim,TO_NOTVICT);

	act("$n has made you a discipliner!",ch,NULL,victim,TO_VICT);

	SET_BIT(victim->special,SPC_PRINCE);

	return;

    }

/* Clan Leader Outcasts Someone */

    else if ( !str_cmp(arg2,"outcast") && victim->generation > 2 && 

	ch->generation == 2)

    {

	act("You make $N a Ninja Outcast!",ch,NULL,victim,TO_CHAR);

	act("$n has made $N a Ninja Outcast!",ch,NULL,victim,TO_NOTVICT);

	act("$n has made you a Ninja Outcast!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( "" );

	return;

    }

/* Prince outcasts a non-prince */

    else if ( !str_cmp(arg2,"outcast") && victim->generation > 2 && 

	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE))

    {

	act("You make $N a Ninja Outcast!",ch,NULL,victim,TO_CHAR);

	act("$n has made $N a Ninja Outcast!",ch,NULL,victim,TO_NOTVICT);

	act("$n has made you a Ninja Outcast!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( "" );

	return;

    }

/* Leader Inducts */

    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 

	ch->generation == 2 && strlen(victim->clan) < 2)

    {

	if (IS_SET(victim->special, SPC_ROGUE))

	{

	    send_to_char("You cannot induct an Rogue!\n\r",ch);

	    return;

	} 

	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);

	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);

	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( ch->clan );

	return;

    }

/* Prince Inducts */

    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 

	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE) &&

	strlen(victim->clan) < 2)

    {

	if (IS_SET(victim->special, SPC_ROGUE))

	{

	    send_to_char("You cannot induct an Rogue!\n\r",ch);

	    return;

	}

	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);

	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);

	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( ch->clan );

	return;

    }

/* Clan Leader inducts a person */

    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 

	ch->generation == 2 && strlen(victim->clan) < 2)

    {

	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);

	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);

	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( ch->clan );

	return;

    }

/* Prince inducts a person */

    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 

	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE) &&

	strlen(victim->clan) < 2)

    {

	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);

	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);

	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);

	free_string(victim->clan);

	victim->clan = str_dup( ch->clan );

	return;

    }

    else send_to_char( "You are unable to grant that sort of favour.\n\r", ch );

    return;

}



void do_nclan( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    CHAR_DATA *gch;



    one_argument( argument, arg );



    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_NINJA) )

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

	if ( !IS_CLASS(gch, CLASS_NINJA) ) continue;

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



void do_rogue( CHAR_DATA *ch, char *argument )

{

    char      arg [MAX_INPUT_LENGTH];

    char      buf [MAX_INPUT_LENGTH];



    argument = one_argument( argument, arg );



    if (IS_NPC(ch)) return;



    if (!IS_CLASS(ch, CLASS_NINJA) )

    {

	send_to_char("Huh?\n\r",ch);

	return;

    }



    if (strlen(ch->clan) > 1)

    {

	send_to_char("But you are already in a clan!\n\r",ch);

	return;

    }



    if ( IS_SET(ch->special, SPC_ROGUE) )

    {

	send_to_char("You are no longer a Rogue.\n\r",ch);

	sprintf(buf,"%s is no longer a Rogue!",ch->name);

	do_info(ch,buf);

	REMOVE_BIT(ch->special, SPC_ROGUE);

	return;

    }

    send_to_char("You are now a Rogue.\n\r",ch);

    sprintf(buf,"%s is now a Rogue!",ch->name);

    do_info(ch,buf);

    SET_BIT(ch->special, SPC_ROGUE);

    return;

}



void do_bomuzite( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    CHAR_DATA *victim;

    char      arg1 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

        

    if (IS_NPC(ch)) return;

	

	if (!IS_CLASS(ch, CLASS_NINJA))

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

	else if ( ch->pcdata->powers[NPOWER_SORA] < 6 )

	{

	    send_to_char("You have not learned the Sora principle to 6.\n\r", ch);

	    return;

	}

	if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

    if ( arg1[0] == '\0' )

    {

	send_to_char( "Bomb who?\n\r", ch );

	return;

    }



    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )

    {

	send_to_char( "They aren't here.\n\r", ch );

	return;

    }


	if (victim->level < 3)
	{
		send_to_char("You may not do this to a mortal.\n\r", ch);
		return;
	}
    if ( ch == victim )

    {

	send_to_char( "Not on yourself!\n\r", ch );

	return;

    }
	if (is_safe(ch, victim))
		return;
	



    if (victim->position == POS_FIGHTING )
    {
	send_to_char("No on a fighting person!\r\n", ch);
	return;
    }
    if ( ch->position == POS_FIGHTING )

    {

	send_to_char( "Not while fighting!\n\r", ch );

	return;

    }

    

	if (ch->move < 500)

	{

	send_to_char("You don't have enough movement points.\n\r", ch);

	return;

	}     

{float ha = victim->hit;
ha /= (float)victim->max_hit;
ha *= 100;
if (ha < 80)
{stc("They are too hurt!\n\r", ch);
return;}}	  

    if ( victim->in_room == ch->in_room )

	{

	act("You toss your bomb onto the floor and put $N to sleep.",ch,NULL,victim,TO_CHAR);

	act("$n tosses a bomb onto the floor.  You feel sleepy.",ch,NULL,victim,TO_VICT);

	af.type = skill_lookup("sleep");
	af.duration = 0;
	af.modifier = 2000;
	af.location = APPLY_AC;
	af.bitvector = AFF_STUNNED;
	affect_to_char(victim, &af);

	ch->move -= 500;

	WAIT_STATE(ch, 6);

	return;

	}

	

	return;

}

void do_ninjacreate( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
	    send_to_char("You mutter some incoherent japanese.\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
		send_to_char("Ninja-to, claws, robes, sandals.\n\r", ch );
		return;
    }
    if      (!str_cmp(arg,"ninja-to"     )) vnum = 29670;
    else if (!str_cmp(arg,"claws"   )) vnum = 29673;
    else if (!str_cmp(arg,"robes"   )) vnum = 29671;
	else if (!str_cmp(arg,"sandals")) vnum = 29672;
    else
    {
	send_to_char("That is an invalid type.\n\r", ch );
	return;
    }
    if ( ch->practice < 80)
    {
		send_to_char("It costs 80 points of primal to create a piece ninja armour.\n\r",ch);
		return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform an Implementor.\n\r",ch);
	return;
    }
    ch->practice -= 80;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
/*olc isn't working*/
if (!str_cmp(arg,"ninja-to")) obj->value[3] = 11;
if (!str_cmp(arg,"ninja-to")) obj->value[0] = 70;
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_dokomo(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

           if (!IS_CLASS(ch, CLASS_NINJA))
         {
          send_to_char("Huh?\n\r",ch);
            return;
        }  

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Doko to whom?\n\r", ch );
	return;}

	location = victim->in_room;

	if (ch->move < 500) {
	send_to_char("You are too tired to go into the shadows.\n\r", ch );
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
