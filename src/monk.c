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



void do_guide( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    int ch_age = years_old(ch);

    char      arg [MAX_INPUT_LENGTH];



    argument = one_argument( argument, arg );



    if (IS_NPC(ch)) return;


    if (!IS_CLASS(ch, CLASS_MONK) )

    {

	send_to_char("Huh?\n\r",ch);

	return;

    }



    if ( arg[0] == '\0' )

    {

	send_to_char( "Guide whom?\n\r", ch );

	return;

    }



    if ( ch_age < 20 )

    {

       send_to_char("You must be 20 to guide a monk!\n\r", ch);

       return;

    }

    

	if (ch->generation > 2)

	{

		send_to_char("You are not a master to guide.\n\r", ch);

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

	send_to_char( "You cannot guide yourself.\n\r", ch );
	return;

    }



    if (IS_CLASS(victim, CLASS_MONK))

    {

	send_to_char( "They are already guided.\n\r", ch );

	return;

    }



    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )

    {

	send_to_char( "You can only guide avatars.\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_VAMPIRE) || 

IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))



    {

	send_to_char( "You are unable to guide vampires!\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_WEREWOLF))

    {

	send_to_char( "You are unable to guide werewolves!\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special,

SPC_CHAMPION))

    {

	send_to_char( "You are unable to guide demons!\n\r", ch );

	return;

    }



    if (IS_CLASS(victim, CLASS_HIGHLANDER))

    {

	send_to_char( "You are unable to guide highlanders.\n\r", ch );

	return;

    }



    



/*



    if (IS_CLASS(victim, CLASS_PALADIN))



    {



    send_to_char( "You are unable to guide paladins.\n\r", ch);



    return;



    }



*/



	if (IS_CLASS(victim, CLASS_NINJA))

	{

	send_to_char( "You are unable to guide ninjas.\n\r", ch);

	return;

	}

	



	if (IS_CLASS(victim, CLASS_DROW))

	{

	send_to_char( "You are unable to guide drow.\n\r", ch);

	return;

	}



	if (victim->class != 0)

	{

	send_to_char("You cannot guide a classed person.\n\r", ch);

	return;

	}



    if (!IS_IMMUNE(victim,IMM_VAMPIRE))

    {

	send_to_char( "You cannot guide an unwilling person.\n\r", ch );

	return;

    }



    if (ch->exp < 50000)

    {

	send_to_char("You cannot afford the 50000 exp required to guide them.\n\r", ch);



	return;



    }



    if (victim->exp < 50000)

    {

	send_to_char("They cannot afford the 50000 exp required to be guided from you.\n\r",ch);

	return;

    }



    ch->exp -= 50000;

    victim->exp -= 50000;



    act("You guide $N in the ways of god.", ch, NULL, victim, TO_CHAR);

    act("$n guide $N in the ways of god.", ch, NULL, victim, TO_NOTVICT);

    act("$n guide you in the way of god.", ch, NULL, victim, TO_VICT);

    victim->level = LEVEL_ARCHMAGE;

    if (victim->trust > 6)

    victim->trust = victim->trust;

    else

    victim->trust = LEVEL_ARCHMAGE;

    send_to_char( "You are now a monk.\n\r", victim );

    free_string(victim->lord);

    victim->lord = str_dup(ch->name);

    victim->class    = CLASS_MONK;

	free_string(victim->clan);

	victim->clan = str_dup( ch->clan );

    save_char_obj(ch);

    save_char_obj(victim);

    return;

}

void do_mastery (CHAR_DATA *ch, char *argument)

{

	if (IS_NPC(ch))

		return;

	if (!IS_CLASS(ch, CLASS_MONK))

	{

		send_to_char("You wave your arms around trying to look like a master.\n\r", ch);

		return;

	}

	if (ch->generation < 3)

	{

		send_to_char("You cannot master a master.\n\r", ch);

		return;

	}

	if (ch->fight_timer > 0)

	{

		send_to_char("You may not master yourself whilst you are tainted from a fight.\n\r", ch);

		return;

	}

	if (ch->exp < EXP(ch,50000000))

	{

		send_to_char("You do not have the experience to become a master.\n\r", ch);

		return;

	}

	if (ch->pcdata->powers[PMONK] < 10)

	{

		send_to_char("You need to have learnt at least 10 mantra's to become a master.\n\r", ch);

		return;

	}

	ch->generation = 2;

	ch->exp -= EXP(ch,50000000);

	send_to_char("You become a master of your clan.\n\r",ch);

	return;

}

void do_empower(CHAR_DATA *ch, char *argument)
{
    int cost;

    if (!IS_CLASS(ch, CLASS_MONK))
    {
	do_huh(ch,"");
	return;
    }

    if (!*argument)
    {
	sprintf(log_buf, "&WYour current chi level is: &B[&R%2d&B]&W, Fury: &B[&R%2d&B]&G\n\r", ch->chi[0], ch->chi[1]);
	stc(log_buf, ch);
	stc("Possible arguments: &Cchi&G, &RFury&w
\rRemember, it's best to keep them in balance.\n\r", ch);
	return;
    }

    if(!str_cmp(argument, "chi"))
    {
	cost = ((ch->chi[0]*150000)+75000)+(ch->chi[1]*25000);
	cost = EXP(ch, cost);
	if (ch->chi[0] >= 6)
	{
	    stc("&CYou already have maximum chi.\n\r", ch);
	    return;
	}

	if (ch->exp < cost)
	{
	    sprintf(log_buf, "You need %d exp to raise your chi level.\n\r", cost);
	    stc(log_buf, ch);
	    return;
	}

	ch->exp -= cost;
	ch->chi[0]++;

	act("&W$n screams out as a $t forcefield surrounds $m.", ch, ch->chi[0]==1?"&BBlue&W":ch->chi[0]==2?
		"&MMagenta&W":ch->chi[0]==3?"&CCyan&W":ch->chi[0]==4?"&YYellow&W":ch->chi[0]==5?"&GGreen&W":"&WWhite",
		NULL, TO_ROOM);
	act("&WYou screams out as a $t forcefield surrounds yourself.", ch, ch->chi[0]==1?"&BBlue&W":ch->chi[0]==2?
		"&MMagenta&W":ch->chi[0]==3?"&CCyan&W":ch->chi[0]==4?"&YYellow&W":ch->chi[0]==5?"&GGreen&W":"&WWhite",
		NULL, TO_CHAR);
	return;
    }
    else if (!str_cmp(argument, "Fury"))
    {
	cost = ((ch->chi[1]*150000)+50000)+(ch->chi[0]*50000);
	cost = EXP(ch, cost);

	if (ch->chi[1] >= 6)
	{
	    stc("&RYou are already angry enough, try to control yourself.\n\r", ch);
	    return;
	}

	if (ch->exp < cost)
	{
	    sprintf(log_buf, "You need %d exp to raise your fury level.\n\r", cost);
	    stc(log_buf, ch);
	    return;
	}

	ch->exp -= cost;
	ch->chi[1]++;

	act("&W$n screams out as a bright $t light breaches in to the room.", ch, ch->chi[1]==1?"&BBlue&W":ch->chi[1]==2?
		"&MMagenta&W":ch->chi[1]==3?"&CCyan&W":ch->chi[1]==4?"&YYellow&W":ch->chi[1]==5?"&GGreen&W":"&WWhite",
		NULL, TO_ROOM);
	act("&WYou screams out as a bright $t light breaches in to the room.", ch, ch->chi[1]==1?"&BBlue&W":ch->chi[1]==2?
		"&MMagenta&W":ch->chi[1]==3?"&CCyan&W":ch->chi[1]==4?"&YYellow&W":ch->chi[1]==5?"&GGreen&W":"&WWhite",
		NULL, TO_CHAR);
	return;
    }
    do_empower(ch,"");
    return;
}

void do_monkclan (CHAR_DATA *ch, char *argument)

{

    char arg1[MAX_INPUT_LENGTH];

	int mastered;

	int advmastered;

    argument = one_argument( argument, arg1 );



	if (IS_NPC(ch))

		return;

	if (!IS_CLASS(ch, CLASS_MONK))

	{

		send_to_char("You are not versed in the ways of the monk to guide your own clan.\n\r", ch);

		return;

	}

	if (ch->generation > 2)

	{

		send_to_char("You are not a master to use this command.\n\r", ch);

		return;

	}

	if (ch->fight_timer > 0)

	{

		send_to_char("You may not think straight whilst you are tainted from a fight.\n\r", ch);

		return;

	}

	if ((ch->max_hit < 17500) || (ch->max_mana < 5000) || (ch->max_move < 8000))

	{

		send_to_char("You have not achieved the stats required to be a monk clan leader.\n\r", ch);

		send_to_char("You need 17500 hit points, 5000 mana, 8000 move.\n\r", ch);

		return;

	}

	if (ch->pcdata->quest < 3000)

	{

		send_to_char("You do not have the 3000 quest points required to make your own clan.\n\r", ch);

		return;

	}

	if (ch->pcdata->powers[PMONK] < 12)

	{

		send_to_char("You need to have learnt at 12 mantra's to become a clan leader.\n\r", ch);

		return;

	}

	mastered = 0;

	advmastered = 0;

	if (ch->stance[STANCE_VIPER] >= 250)

		mastered++;

	if (ch->stance[STANCE_CRANE] >= 250)

		mastered++;

	if (ch->stance[STANCE_MONGOOSE] >= 250)

		mastered++;

	if (ch->stance[STANCE_BULL] >= 250)

		mastered++;

	if (ch->stance[STANCE_CRAB] >= 250)

		mastered++;

	if (ch->stance[STANCE_MANTIS] >= 250)

		advmastered++;

	if (ch->stance[STANCE_TIGER] >= 250)

		advmastered++;

	if (ch->stance[STANCE_DRAGON] >= 250)

		advmastered++;

	if (ch->stance[STANCE_MONKEY] >= 250)

		advmastered++;

	if (ch->stance[STANCE_SWALLOW] >= 250)

		advmastered++;

	if (mastered < 5)

	{

		send_to_char("You must have all basic stances to rogue master\n\r", ch);

		return;

	}

	if (advmastered < 4)

	{

		send_to_char("You must have 4 advanced stances to rogue master\n\r", ch);

		return;

	}

	if (arg1[0] == '\0')

	{

		send_to_char("You have not specified a clan name.\n\r", ch);

		return;

	}

	if ((strlen(arg1) < 3) || (strlen(arg1) > 7 ) )

	{

		send_to_char("Clan name must be of more than 3 characters and less than 8", ch);

		return;

	}

	ch->pcdata->quest -= 3000;

    free_string(ch->lord);

    ch->lord = str_dup("");

	free_string(ch->clan);

	ch->clan = str_dup( arg1 );

	send_to_char("All abusive of offensive names will result in character deletion.\n\r", ch);

	send_to_char("You are now the founder of a monk clan.\n\r", ch);

	return;

}

void do_mantra( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];

    char arg2[MAX_INPUT_LENGTH];

    char buf[MAX_STRING_LENGTH];



    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );



    if (IS_NPC(ch)) return;



    if (!IS_CLASS(ch, CLASS_MONK))

    {

	send_to_char("Huh?\n\r",ch);

	return;

    }



    if (arg1[0] == '\0' && arg2[0] == '\0')

    {

	sprintf(buf,"Mantras Learned (%d).\n\r", ch->pcdata->powers[PMONK]);

	send_to_char(buf,ch);

	send_to_char("Powers you have:\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] < 1 )

	   send_to_char("You have no knowledge of any Mantras.\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 1 )

	   send_to_char("Eyes of God.\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 2 )

	   send_to_char("Shield/Read Aura/Scry\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 3 )

	   send_to_char("Sacred Invisibility\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 4 )

	   send_to_char("Heart of the Oak\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 5 )

	   send_to_char("Adamantium Hands\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 6 )

	   send_to_char("Skin of Steel\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 7 )

	   send_to_char("The Almighty Favor\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 8 )

	   send_to_char("Dark Blaze\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 9 )

	   send_to_char("Celestial Path\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 10 )

	   send_to_char("Prayer of the Ages\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 11 )

	   send_to_char("Cloak of Life\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 12 )

			send_to_char("God's Embrace\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 13 )

	   send_to_char("You have completed your Mantra training.\n\r", ch);

	return;

    }



	if (arg2[0] == '\0')

    {

	if (!str_cmp(arg1,"power"))

	{

	  if ( ch->pcdata->powers[PMONK] < 1 )

	   send_to_char("You have no knowledge of any Mantras.\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 1 )

	   send_to_char("Eyes of God.\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 2 )

	   send_to_char("Shield/Read Aura/Scry\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 3 )

	   send_to_char("Sacred Invisibility\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 4 )

	   send_to_char("Heart of the Oak\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 5 )

	   send_to_char("Adamantium Hands\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 6 )

	   send_to_char("Skin of Steel\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 7 )

	   send_to_char("The Almighty Favor\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 8 )

	   send_to_char("Dark Blaze\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 9 )

	   send_to_char("Celestial Path\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 10 )

	   send_to_char("Prayer of the Ages\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 11 )

	   send_to_char("Cloak of Life\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 12 )

			send_to_char("God's Embrace\n\r", ch);

	  if ( ch->pcdata->powers[PMONK] >= 13 )

	   send_to_char("You have completed your Mantra training.\n\r",ch);

	  return;

	}

	sprintf(buf,"Mantras Learned (%d).\n\r", ch->pcdata->powers[PMONK]);

	sprintf(buf, "\n\rType Mantra power improve to improve amantra.\n\r");



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

	improve = PMONK;   

	max=12;

	}

	else

	{

	    send_to_char("________________________________\n\r",ch);

	    send_to_char("|                               |\n\r",ch);

	    send_to_char("|             Mantras           |\n\r",ch);

	    send_to_char("|_______________________________|\n\r",ch);

	    send_to_char("| 1.        Eye of God          |\n\r",ch);

	    send_to_char("| 2.   Shield/Read Aura/Scry    |\n\r",ch);

	    send_to_char("| 3.    Sacred Invisibility     |\n\r",ch);

	    send_to_char("| 4.     Adamantium Hands       |\n\r",ch);

	    send_to_char("| 5.       Flaming Hands        |\n\r",ch);

	    send_to_char("| 6.       Skin of Steel        |\n\r",ch);

	    send_to_char("| 7.    The Almighty's Favor    |\n\r",ch);

	    send_to_char("| 8.        Dark Blaze          |\n\r",ch);

	    send_to_char("| 9.      Celestial Path        |\n\r",ch);

	    send_to_char("| 10.   Pray of the Ages        |\n\r",ch);

	    send_to_char("| 11.     Cloak of Life         |\n\r",ch);

		send_to_char("| 12.     God's Embrace         |\n\r",ch);

	    send_to_char("|_______________________________|\n\r",ch);

	    return;

	}						

	cost = (ch->pcdata->powers[improve]+1) * 10;

	arg1[0] = UPPER(arg1[0]);

	if ( ch->pcdata->powers[improve] >= max )

	{

	    sprintf(buf,"You have already gained all the known Mantras.\n\r");

	    send_to_char(buf,ch);

	    return;

	}



	if ( cost > ch->practice )

	{

	    sprintf(buf,"It costs you %d primal learn the Mantra.\n\r", cost);



	    send_to_char(buf,ch);

	    return;

	}



	ch->pcdata->powers[improve] += 1;

	ch->practice -= cost;

	sprintf(buf,"You learn a Mantra.\n\r");

	send_to_char(buf,ch);

    }



    else send_to_char("To learn a Mantra, type: Mantra power improve.\n\r",ch);

	return;



}







void do_cloak( CHAR_DATA *ch, char *argument )



{



	if ( IS_NPC(ch) )



     return;







    if (!IS_CLASS(ch, CLASS_MONK))



    {



	  send_to_char("Huh?\n\r",ch);



	  return;



    }







    if (ch->pcdata->powers[PMONK] < 11 )



    {



	  send_to_char("You do not have the mantra of the Cloak of Life.\n\r",ch);



	  return;



    }







    if (ch->move < 1000)



    {



	 send_to_char("You do not have enough vitality to do that.\n\r",ch);



	 return;



    }







    if (IS_SET(ch->newbits, NEW_MONKCLOAK))



    {



       REMOVE_BIT(ch->newbits, NEW_MONKCLOAK);



       send_to_char("You are no longer protected by the Almighty's cloak.\n\r",ch);



       act("$n is no longer protected by the Almighty's cloak of life.\n\r",ch,NULL,NULL,TO_ROOM);



       return;



    }



    if (!IS_SET(ch->newbits,NEW_MONKCLOAK))



    {



       SET_BIT(ch->newbits, NEW_MONKCLOAK);



	 send_to_char("You are protected by the Almighty's cloak.\n\r",ch);



	 act("$n is protected by the Almighty's cloak of life.\n\r",ch,NULL,NULL,TO_ROOM);



	 ch->move = ch->move - 1000;



	 return;



    }







}







void do_prayofages( CHAR_DATA *ch, char *argument )



{



    char      arg [MAX_INPUT_LENGTH];



    CHAR_DATA *victim;







    argument = one_argument( argument, arg );







    if (IS_NPC(ch)) return;







    if (IS_CLASS(ch, CLASS_MONK))



    {



	if (ch->pcdata->powers[PMONK] < 10)



	{



	    send_to_char("You haven't gotten the Pray of Ages mantra.\n\r",ch);



	    return;



	}



    }



    else



    {



	send_to_char("Huh?\n\r",ch);



	return;



    }







    if (arg[0] == '\0')



    {



	send_to_char("Who do you wish to be teleported to?\n\r",ch);



	return;



    }







    if ( ( victim = get_char_world( ch, arg ) ) == NULL )



    {



	send_to_char( "Nobody by that name.\n\r", ch );



	return;



    }







    if ( IS_NPC(victim) )



    {



	send_to_char( "Not on NPC's.\n\r", ch );



	return;



    }



    if ( victim == ch ) 



    {



		send_to_char( "Nothing happens.\n\r", ch);



		return; 



    }







    if ( victim->level != LEVEL_AVATAR && !IS_CLASS(victim, CLASS_MONK))



    {



	send_to_char( "Nothing happens.\n\r", ch );



	return;



    }



/* boggle to monks only Listy */

    if (!IS_CLASS(victim, CLASS_MONK))



    {

	send_to_char("Prayer of Ages only to other monks.\n\r", ch);

	return;

    }





    if (victim->in_room == NULL)



    {



	send_to_char( "Nothing happens.\n\r", ch );



	return;



    }







    if ( victim->position != POS_STANDING )



    {



	send_to_char( "You are unable to focus on their location.\n\r", ch );



	return;



    }







    send_to_char("You pray to god and vanish in a flash of light.\n\r",ch);



    act("$n utters a prayer to god and vanishes in a flash of light.",ch,NULL,NULL,TO_ROOM);



    char_from_room(ch);



    char_to_room(ch,victim->in_room);



    do_look(ch,"");



    send_to_char("You emerge from a portal in the sky.\n\r",ch);



    act("$n appears from a portal in the sky.",ch,NULL,NULL,TO_ROOM);



    return;



}







void do_sacredinvis( CHAR_DATA *ch, char *argument)

{



    if ( IS_NPC(ch) )

     return;



    if (!IS_CLASS(ch, CLASS_MONK))

    {

	  send_to_char("Huh? not a monk\n\r",ch);

	  return;

    }



    if (ch->pcdata->powers[PMONK] < 3)

    {

	send_to_char("You do not have the mantra of Sacred Invisibility.\n\r", ch);

	return;

    }

	if (weather_info.sunlight == SUN_DARK)

	{

		send_to_char("You may not go into invisibility whilst the sun is not out.\n\r", ch);

		return;

	}



    if ( IS_SET(ch->act, PLR_SACREDINVIS) )

    {

	REMOVE_BIT(ch->act, PLR_SACREDINVIS);

	send_to_char( "You emerge from your cloak of invisibility.\n\r", ch );

	act("$n slowly emerges from a cloak of holy invisibility.",ch,NULL,NULL,TO_ROOM);

    }

    else

    {

	if (has_timer(ch)) return;



	send_to_char( "You slowly fade into a cloak of invisibility.\n\r", ch );

	act("$n slowly fades into a cloak of holy invisibility..",ch,NULL,NULL,TO_ROOM);

	SET_BIT(ch->act, PLR_SACREDINVIS);

    }



    return;

}











void do_flaminghands( CHAR_DATA *ch, char *argument)



{



    if ( IS_NPC(ch) )



     return;







    if (!IS_CLASS(ch, CLASS_MONK))



    {



	  send_to_char("Huh?\n\r",ch);



	  return;



    }







    if (ch->pcdata->powers[PMONK] <5)



    {



	  send_to_char("You do not have the mantra of Flaming Hands.\n\r",ch);



	  return;



    }







    if (IS_SET(ch->newbits, NEW_MONKFLAME))



    {



	  REMOVE_BIT(ch->newbits, NEW_MONKFLAME);



        send_to_char("Your hands are no longer engulfed by flames.\n\r",ch);



	  act("$n's hands are no longer engulfed by flames.\n\r",ch,NULL,NULL,TO_ROOM);



	  return;



    }



    if (!IS_SET(ch->newbits, NEW_MONKFLAME))



    {



	  SET_BIT(ch->newbits, NEW_MONKFLAME);



	  send_to_char("Your hands are engulfed by flames!\n\r",ch);



	  act("$n's hands are engulfed by flames!\n\r",ch,NULL,NULL,TO_ROOM);



	  return;



    }



}







void do_adamantium( CHAR_DATA *ch, char *argument)



{



     if ( IS_NPC(ch) )



     return;







    if (!IS_CLASS(ch, CLASS_MONK))



    {



	  send_to_char("Huh?\n\r",ch);



	  return;



    }







    if (ch->pcdata->powers[PMONK] < 4 )



    {



	  send_to_char("You do not have the mantra of Adamantium Hands.\n\r",ch);



	  return;



    }







    if (IS_SET(ch->newbits, NEW_MONKADAM))



    {



	  REMOVE_BIT(ch->newbits, NEW_MONKADAM);



        send_to_char("Your hands resume their normal toughness.\n\r",ch);



	  act("$n's hands resume their normal toughness.\n\r",ch,NULL,NULL,TO_ROOM);



	  return;



    }



    if (!IS_SET(ch->newbits, NEW_MONKADAM))



    {



	  SET_BIT(ch->newbits, NEW_MONKADAM);



	  send_to_char("Your hands turn as hard as adamantium!\n\r",ch);



	  act("$n's hands turn as hard as adamantium!\n\r",ch,NULL,NULL,TO_ROOM);



	  return;



    }



}







void do_celestial( CHAR_DATA *ch, char *argument)

{

    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];

    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);



    if ( IS_NPC(ch) )

	return;



    if (!IS_CLASS(ch, CLASS_MONK))

    {

	send_to_char("Huh?\n\r",ch);

	return;

    }







    if (ch->pcdata->powers[PMONK] < 9)

    {

	send_to_char("You do not have the Celestial Path mantra!\n\r",ch);

	return;

    }



    if (ch->move < 2000)

    {

	send_to_char("You do not have enough vitality to do that!\n\r",ch);

	return;

    }

    if (ch->fight_timer > 0)

    {

       send_to_char("You may not open a celestial path with a fight timer.\n\r", ch);

       return;

    }

    if ( arg[0] == '\0' )

    {

	send_to_char( "Open a celestial path to who?\n\r", ch );

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

	send_to_char("You can't seem to overcome your victim's will.\n\r", ch);

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

		send_to_char("You may not celestial path to them whilst mortal.\n\r", ch);

		return;

	}



    char_from_room(ch);

    char_to_room(ch,victim->in_room);



    if (IS_NPC(victim))



    {

	sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->short_descr);

	send_to_char(buf, ch);

    }



    if (!IS_NPC(victim))

    {

	sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->name);

	send_to_char(buf, ch);

	WAIT_STATE(ch, 6);

    }

    act("$n appears before you through a Celestial portal!\n\r",ch,NULL,NULL,TO_ROOM);

    do_look(ch, "auto");

    ch->move = ch->move - 2000;

    return;

}







void do_godseye( CHAR_DATA *ch, char *argument )



{



    if ( IS_NPC(ch) )



	return;



  



  	if ( !IS_CLASS(ch, CLASS_MONK) )



  	{



  	send_to_char("Huh?\n\r", ch);



  	return;



  	}



  	



  	if ( ch->pcdata->powers[PMONK] < 1 )



  	{



  	  send_to_char("You have not learned the Eyes of God Mantra.\n\r", ch);



  	  return;



  	}



  



    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )



    {



	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);



	send_to_char( "Your view shimmers into mortal vision.\n\r", ch );



    }



    else



    {



	send_to_char( "God answers your prayers and gives you his vision.\n\r", ch );



	if (ch->trust > 6)



	{



	 ch->trust = ch->trust;



	}



	else



	{



	 ch->trust = 6;



	}



	SET_BIT(ch->act, PLR_HOLYLIGHT);



    }



    return;



}







void do_steelskin( CHAR_DATA *ch, char *argument )



{



    if ( IS_NPC(ch) )



	return;



  



  	if (!IS_CLASS(ch, CLASS_MONK) )



  	{



  	send_to_char("Huh?\n\r", ch);



  	return;



  	}



  	



  	if ( ch->pcdata->powers[PMONK] < 6 )



  	{



  	  send_to_char("You have not learned the Steel of Skin Mantra.\n\r", ch);



  	  return;



  	}







/* Remove steel shield in handler.c */



  



    if ( IS_SET(ch->newbits, NEW_MONKSKIN) )



    {



	REMOVE_BIT( ch->newbits, NEW_MONKSKIN );



	send_to_char( "Your steel shield disappears.\n\r", ch );



    }



    else



    {



	send_to_char( "You are surrounded by a steel shield\n\r", ch );



	SET_BIT(ch->newbits, NEW_MONKSKIN);



    }



    return;



}







/* Monk Targeting in ninja.c */







void do_godsbless( CHAR_DATA *ch, char *argument )



{







    int sn;





    int spelltype;







    if ( IS_NPC(ch) )



	return;



  



  	if (!IS_CLASS(ch, CLASS_MONK))



  	{



  	send_to_char("Huh?\n\r", ch);



  	return;



  	}



  	



  	if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 7 )



  	{



  	send_to_char("You have not learned the Almighty Favor Mantra.\n\r", ch);



  	return;



  	}



  	



  	if (ch->mana < 3000) 



  	{



	send_to_char("You don't have enough mana.\n\r", ch );



	return;



	}



	



    if ( ( sn = skill_lookup( "godbless" ) ) < 0 ) return;



    spelltype = skill_table[sn].target;





    (*skill_table[sn].spell_fun) ( sn, 300, ch, ch );



    WAIT_STATE( ch, 12 );



    ch->mana = ch->mana - 3000;



    return;



}







void do_godsfavor( CHAR_DATA *ch, char *argument )



{



	



	if ( IS_NPC(ch) )



	return; 







   if (!IS_CLASS(ch, CLASS_MONK))



      {



	send_to_char("Huh?\n\r",ch);



	return;



	}







   if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 8 )



  	{



  	send_to_char("You have not learned the Almighty's Favor Mantra.\n\r", ch);



  	return;



  	}







   if (ch->move < 1500)



	{



	send_to_char("You do not have the vitality to do that!\n\r",ch);



	return;



	}



 if (!IS_SET(ch->newbits, NEW_MONKFAVOR))



   {



     SET_BIT(ch->newbits, NEW_MONKFAVOR);



     send_to_char("God smiles upon you and you feel strengthened.\n\r",ch);



     act("$n is blessed by the Almighty.",ch,NULL,NULL,TO_ROOM);



     ch->move = ch->move - 1500;



     WAIT_STATE(ch, 4);



     return;



   }



 if (IS_SET(ch->newbits, NEW_MONKFAVOR))



   {



     REMOVE_BIT(ch->newbits, NEW_MONKFAVOR);



     send_to_char("You feel weaker as The Almighty's favor leaves you.\n\r",ch);



     act("$n no longer looks so holy.",ch,NULL,NULL,TO_ROOM);



     return;



   }



}





/* -Nuit was here ;) */

void do_darkblaze( CHAR_DATA *ch, char *argument )



{

	CHAR_DATA *victim;

        char arg [MAX_INPUT_LENGTH];

	

        argument = one_argument( argument, arg );

       

        if (IS_NPC(ch))

        return;



        if (!IS_CLASS(ch, CLASS_MONK) )

        {

        send_to_char("Huh?\n\r", ch);

        return;

        }

        if (ch->level < 3)

        {

        send_to_char("Huh?\n\r", ch);

        return;

        }

        if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 8 )

        {

        send_to_char("You have not learned the Dark Blaze Mantra.\n\r", ch);

        return;

        }

		/* Slap Nuitari - Listy :P*/

		if (arg[0] == '\0')

		{

			send_to_char("Use this skill on whom\n\r", ch);

			return;

		}

        if (ch->mana < 1000) 

        {

        send_to_char("You don't have enough mana.\n\r", ch );

        return;

        }

        if ( ( victim = get_char_room( ch, arg ) ) == NULL )

        {

        send_to_char( "They aren't here.\n\r", ch );

        return;

        }

		if (is_safe(ch, victim))

			return;

        if (ch->fight_timer > 0)

        {

        send_to_char("You may not stun with a fight timer active.\n\r", ch);

        return;

        }

        if (victim->level < 3)        {        send_to_char("Can't do that to a mortal\n\r", ch);        return;         }        

        act("You pray to God and a holy force stuns $N.",ch,NULL,victim,TO_CHAR);

        act("$n prays to God and a holy force stuns $N",ch,NULL,victim,TO_NOTVICT);

        act("$n prays to God and a holy spirit stuns you.",ch,NULL,victim,TO_VICT);  

        victim->position = POS_STUNNED;

        ch->mana -= 1000;

        return;

        

        }

/* Monk eq creation by Adreno */



void do_monkcreate( CHAR_DATA *ch, char *argument )

{

    OBJ_INDEX_DATA *pObjIndex;

    OBJ_DATA *obj;

    char arg[MAX_INPUT_LENGTH];

    int vnum = 0;



    argument = one_argument( argument, arg );



    if (IS_NPC(ch)) return;



    if (!IS_CLASS(ch, CLASS_MONK))

    {

	    send_to_char("Huh?\n\r",ch);

	    return;

    }



    if (arg[0] == '\0')

    {

		send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );

		send_to_char("Bracer, Cloak or Gloves.\n\r", ch );

		return;

    }

    if      (!str_cmp(arg,"bracer"     )) vnum = 29706;

    else if (!str_cmp(arg,"cloak"      )) vnum = 29707;

    else if (!str_cmp(arg,"gloves"     )) vnum = 29708;

    else

    {

	send_to_char("That is an invalid type.\n\r", ch );

	return;

    }



   if ( ch->practice < 80)



    {



		send_to_char("It costs 80 points of primal to create a piece of Vampire armour.\n\r",ch);



		return;



    }



    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)



    {



	send_to_char("Missing object, please inform Adreno.\n\r",ch);



	return;



    }



    ch->practice -= 80;



    obj = create_object(pObjIndex, 50);



    obj_to_char(obj, ch);



    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);



    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);



    return;



}

void do_palmthrust( CHAR_DATA *ch, char *argument )

{  

char arg[MAX_INPUT_LENGTH];   

CHAR_DATA *victim;    

int dam;

int loop;

one_argument( argument, arg );

if (IS_NPC(ch))

return;

if (ch->level < 3)

{

send_to_char("God thinks you are crazed.\n\r", ch);

return;

}		

if ( !IS_CLASS(ch, CLASS_MONK) )	

{	

send_to_char("Huh?\n\r", ch);	

return;	

}    

if ( arg[0] == '\0' && !ch->fighting)    

{	

send_to_char( "Thrust what?\n\r", ch );	

return;    

}	            
if (arg[0] == '\0') victim = ch->fighting; else
if ( ( victim = get_char_room( ch, arg ) ) == NULL )    

{	

send_to_char( "They aren't here.\n\r", ch );	

return;    

}

    if (is_safe(ch, victim))

		return;

	if (victim->level < 3){send_to_char("God doesn't like that very much!\n\r", ch);return;}if ( ch->move < 500 )	

{	

send_to_char( "You don't have the required 500 move.\n\r", ch);	

return;	

}

if ( ch->mana < 750 )	

{	

send_to_char( "You don't have the required 750 mana.\n\r", ch);	

return;	}		

loop = 0;

if (victim->position < POS_SLEEPING)

{

    send_to_char("They are almost dead!\r\n", ch);

    return;

}

while ( loop++ < 5 && victim->position > POS_DEAD && ch->in_room == victim->in_room)	

{	 

dam=number_range(400,1200);
if (number_range(1,5)==1) dam = 0;
if ( dam <= 0)       

{

act("Your palm thrust misses $N.", ch, NULL, victim, TO_CHAR);       

act("$n's palm thrust misses you.", ch,NULL, victim, TO_VICT);       

act("$n's palm thrust misses $N.", ch,NULL, victim, TO_NOTVICT);      

}          

else if ( dam <= 600 && dam >= 400 )      

{       

act("Your palm thrust barely hits $N.", ch, NULL, victim, TO_CHAR);  

act("$n's palm thrust barely hits you.", ch,NULL, victim, TO_VICT); 

act("$n's palm thrust barely hits $N.", ch,NULL, victim, TO_NOTVICT); 

act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);

spell_blindness( skill_lookup( "blindness" ), ch->spl[1], ch, victim );
if (victim->trust < 7)
REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
}      

else if ( dam <= 800 && dam >= 601 )

{       

act("Your palm thrust thrusts $N weakly.", ch, NULL, victim, TO_CHAR);

act("$n's palm thrust thrusts you weakly.", ch,NULL, victim, TO_VICT);

act("$n's palm thrust thrusts $N weakly.", ch,NULL, victim, TO_NOTVICT);

act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);	

spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );      
if (number_range(1,2)==1)
spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );      

}

else

if ( dam <= 900 && dam >= 801 )

{       

act("Your palm thrust thrusts $N.", ch, NULL, victim, TO_CHAR);       

act("$n's palm thrust thrusts you.", ch,NULL, victim, TO_VICT);       

act("$n's palm thrust thrusts $N.", ch,NULL, victim, TO_NOTVICT);       

act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);

spell_magic_missile( skill_lookup( "magic missile" ), ch->spl[1]/3, ch, victim );      

}      

else if ( dam <= 1000 && dam >= 901 )

{       

act("Your palm thrust thrusts $N hard.", ch, NULL, victim, TO_CHAR);

act("$n's palm thrust thrusts you hard.", ch,NULL, victim, TO_VICT);

act("$n's palm thrust thrusts $N hard.", ch,NULL, victim, TO_NOTVICT); 

act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM); 

spell_harm( skill_lookup( "harm" ), ch->spl[1], ch, victim );      

}      

else if ( dam <= 1100 && dam >= 1001 )

{       

act("Your palm thrust thrusts $N deeply.", ch, NULL, victim, TO_CHAR);

act("$n's palm thrust thrusts you deeply.", ch,NULL, victim, TO_VICT); 

act("$n's palm thrust thrusts $N deeply.", ch,NULL, victim, TO_NOTVICT);	

act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM); 

spell_faerie_fire( skill_lookup( "faerie fire" ), ch->spl[1]*2, ch, victim );

}      

else if ( dam <= 1200 && dam >= 1101 )

{       

act("Your palm thrust blasts through $N's armor.", ch, NULL, victim, TO_CHAR);       

act("$n's palm thrust blasts through your armor.", ch,NULL, victim, TO_VICT);       

act("$n's palm thrust blasts through $N's armor.", ch,NULL, victim, TO_NOTVICT);	   

act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);

spell_magic_missile( skill_lookup( "magic missile" ), ch->spl[1]/1.5, ch, victim ); 

}

else

	send_to_char("BUG: INFORM A GOD.\n\r", ch);
if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
	break;
hurt_person(ch, victim, dam );

}  	

ch->mana -= 750; 	

ch->move -= 500;

WAIT_STATE( ch, skill_table[gsn_kick].beats ); 	    

return;

}



void do_mclan( CHAR_DATA *ch, char *argument ){ 

char buf[MAX_STRING_LENGTH];

CHAR_DATA *gch;



if (IS_NPC(ch)) return;

if (!IS_CLASS(ch, CLASS_MONK) )

{

send_to_char("Huh?\n\r",ch);

return;

}

if (strlen(ch->clan) < 2)

{

send_to_char("You are an outcast!\n\r",ch);

return;

}

sprintf( buf, "%s Monks:\n\r", ch->clan );

send_to_char( buf, ch );

send_to_char("[      Name      ] [ Hits   ] [ Mana   ] [ Move   ]\n\r", ch );

for ( gch = char_list; gch != NULL; gch = gch->next )

{

if ( IS_NPC(gch) ) continue;

if ( !IS_CLASS(gch, CLASS_MONK) ) continue;

if ( !str_cmp(ch->clan,gch->clan) )

{

sprintf( buf,"[%-16s] [%-6d%3d] [%-6d%3d] [%-6d%3d]\n\r",

capitalize( gch->name ),

gch->hit,  (gch->hit  * 100 / gch->max_hit ),

gch->mana, (gch->mana * 100 / gch->max_mana),

gch->move, (gch->move * 100 / gch->max_move) );

send_to_char( buf, ch );

}

}

return;

}

void do_healingtouch(CHAR_DATA *ch, char *argument)

{



    if (ch->mana < 300)

    {

	send_to_char("You do not have enough mana to use healingtouch.\r\n", ch);

	return;

    }

    ch->mana -= 300;

    ch->hit += number_range(200,400);



    if (ch->hit > ch->max_hit)

	ch->hit = ch->max_hit;



    send_to_char("You revive you body's health.\r\n", ch);

    return;

}



void do_decide( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];   

   CHAR_DATA *victim;    

   one_argument( argument, arg );
   
   if (!IS_CLASS(ch, CLASS_MONK) )
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
    }
    if ( victim->level != LEVEL_ARCHMAGE && !IS_IMMORTAL(victim) )
    {
      send_to_char( "You can only induct avatars.\n\r", ch );
      return;
    }
    act("You induct $N into your clan.", ch, NULL, victim, TO_CHAR);
    act("$n inducts $N into his clan.", ch, NULL, victim, TO_NOTVICT);
    act("$n inducts you in his clan.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now a in monk clan.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    free_string(victim->clan);
    victim->clan = str_dup( ch->clan );
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_expel( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];   

   CHAR_DATA *victim;    

   one_argument( argument, arg );
   
   if (!IS_CLASS(ch, CLASS_MONK) )
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
    if ( victim->level != LEVEL_ARCHMAGE && !IS_IMMORTAL(victim) )
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
