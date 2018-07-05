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

//these are snippets, coded in by berelain. Not original
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"
#include "player.h"

/* local declarations */
void save_char_obj_finger(CHAR_DATA *);

void do_bounty( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
//    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
//    argument = one_argument( argument, arg3 );
/* smoo added this part in! Kewl it's not a snippit! */
if (!str_cmp(arg1, "list"))
{do_blist(ch,"");return;}
if (!str_cmp(arg1, "set") && !IS_NPC(ch) && ch->trust >= 11)
{
    if ((victim = get_char_world(ch, arg2)) == NULL)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (IS_NPC(victim)) {stc("Not on a mob!\n\r", ch);return;}

    sprintf(log_buf, "%s's bounty set to %d from %d.", PERS(ch, victim), atoi(argument), victim->pcdata->bounty);
    victim->pcdata->bounty = atoi(argument);
    stc(log_buf, ch);
    return;

}
/* aww damn the rest is a snippit */

 
        if ( arg1[0] == '\0' || arg2[0] == '\0' )
        {
             send_to_char( "Place a bounty on who's head?\n\rSyntax: Bounty <victim> <amount>\n\r", ch);
             return;
        }
        /*
        if ( arg3[0] = "amount")
        {
             stc( "victim->bounty", ch ); return;
        }
        */
        if ( ( victim = get_char_world( ch, arg1 ) ) == NULL)
        {
           send_to_char( "They are currently not logged in!", ch );
           return;
        }
 
      if (IS_NPC(victim))
      {
        send_to_char( "You cannot put a bounty on NPCs!", ch );
        return;
      }
         
        if ( is_number( arg2 ) )
        {
        int dtok;
dtok = atoi(arg2);
if (ch->level < 7) // smoo mod
        if (ch->pcdata->quest < dtok)
        {
                send_to_char("You don't have that much QP!",ch);
                return;
        }
        ch->pcdata->quest -= dtok;
        victim->pcdata->bounty += dtok;
sprintf(buf,"%s has just placed a BOUNTY of &G%d#n quest points on &B%s's#n head may
the hunt be swift and deadly!",
ch->pcdata->switchname, dtok, victim->pcdata->switchname);
save_char_obj_finger(victim);
        do_info(ch, buf);
        return;
        }
}

void do_addlag(CHAR_DATA *ch, char *argument)
{

	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH];
	int x;

	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("addlag to who?", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
		send_to_char("They're not here.", ch);
		return;
	}

	if ((x = atoi(argument)) <= 0)
	{
		send_to_char("That makes a LOT of sense.", ch);
		return;
	}

	if (x > 10000)
	{
		send_to_char("There's a limit to cruel and unusual
punishment", ch);
		return;
	}

	send_to_char("Somebody REALLY didn't like you", victim);
	WAIT_STATE(victim, x);
	send_to_char("Adding lag now...", ch);
	return;
}


void do_mudc( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: MUDC %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Mudc what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char("&WMudControl: &W",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_questinfo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: QuestInfo %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Shed Light on what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char("&RQuest&DInfo&Y ->&G",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_trivia( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Trivia %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Trivia what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char("&GTrivia ->&M",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}


void do_bonus( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char       buf  [ MAX_STRING_LENGTH ];
    char       arg1 [ MAX_INPUT_LENGTH ];
    char       arg2 [ MAX_INPUT_LENGTH ];
    int      value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

stc("Don't use this.\n\r", ch);
return;

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: bonus <char> <Exp>.\n\r", ch );
	return;
    }
    
    if (( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
      send_to_char("That Player is not here.\n\r", ch);
      return;
    }
       
    if ( IS_NPC( victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You may not bonus yourself.\n\r", ch );
	return;
    }

    if (IS_IMMORTAL(victim) || victim->level >= LEVEL_IMMORTAL)
    {
    send_to_char("You can't bonus immortals silly!\n\r", ch);
    return;
    }

    value = atoi( arg2 );

    if ( value < -50000000 || value > 50000000 )
    {
	send_to_char( "Value range is -50000000 to 50000000.\n\r", ch );
	return;
    }

    if ( value == 0 )
    {
	send_to_char( "The value must not be equal to 0.\n\r", ch );
	return;
    }

    gain_exp(victim, value);
   
    sprintf( buf,"You have bonused %s a whopping %d experience points.\n\r",
    		victim->name, value);
    		send_to_char(buf, ch);

    if ( value > 0 )
    { 
      sprintf( buf,"You have been bonused %d experience points.\n\r", value );
      send_to_char( buf, victim );
    }
    else
    {
      sprintf( buf,"You have been penalized %d experience points.\n\r", value );
      send_to_char( buf, victim );
    }
 
    return;
}  

void center_to_char(char *argument, CHAR_DATA *ch, int columns)
{
char centered[MAX_INPUT_LENGTH];
int spaces;

columns = (columns < 2) ? 80 : columns;
spaces=(columns-strlen(argument))/2;

sprintf(centered,"%*c%s",spaces,' ',argument);
send_to_char(centered,ch);

return;
}
 
/*
For this you need to put a line for center_to_char in merc.h
under the send_to_char line, except you will have to add
"int columns" to the args...
 */
 
 
 
 
/** mlk=> 
 ** eg. do_font(ch,argall);  -I used this in my help file titles
 **                           \ Strategically placed so helps with "."
 **                            \ prelude do not get the font.
 **     do_font(ch,"hi");
 **/
void do_font( CHAR_DATA *ch, char *argument)
{
int  place, size;
char buf[10];

    size = strlen(argument);

/* top border */
center_to_char( "&B+&D-",ch,72-(2*size) );

for (place=2;place<size;place++)
send_to_char("&B+&D-",ch);
send_to_char("&B+#n\n\r",ch);

/* middle */
sprintf(buf,"&B%c",UPPER(argument[0]));
center_to_char(buf,ch,72-(2*size) );

for (place=1;place<size;place++)
        {
        sprintf(buf," %c",UPPER(argument[place]));
        send_to_char(buf,ch);
        }
send_to_char("#n\n\r",ch);

/* bottom border */
center_to_char( "&B+&D-",ch,72-(2*size) );

for (place=2;place<size;place++)
send_to_char("&B+&D-",ch);
send_to_char("&B+#n\n\r",ch);

return;
}


void do_jail( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Jail whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    sprintf(buf,"$N jails %s",victim->name);
    send_to_char( "OK.\n\r", ch );
    save_char_obj(victim);
    stop_fighting(victim,TRUE);
    do_freeze( ch, victim->name);
    do_disconnect( ch, victim->name);

    return;
}

       
  /*

void do_gamestats( CHAR_DATA *ch, char *argument ) 
{
	char buf[MAX_STRING_LENGTH];
	sprintf(buf,"Total Logins                     %5d\n\r", logins);
	send_to_char(buf, ch);
	sprintf(buf,"Most ever on                     %5d\n\r",
max_ever_on);
	send_to_char(buf, ch);
	sprintf(buf,"Total Levels                     %5d\n\r", levels);
	send_to_char(buf, ch);
	sprintf(buf,"Total Newbies                    %5d\n\r", newbies);
	send_to_char(buf, ch);
	sprintf(buf,"Total Deletions                  %5d\n\r",
deletions);
	send_to_char(buf, ch);
	sprintf(buf,"Total Mobdeaths                  %5d\n\r",
mobdeaths);
	send_to_char(buf, ch);
	sprintf(buf,"Total Player Deaths              %5d\n\r", pdied);
	send_to_char(buf, ch);
	sprintf(buf,"Total PKills                     %5d\n\r", pk);
	send_to_char(buf, ch);
	sprintf(buf,"Total Auctions                   %5d\n\r", auctions);
	send_to_char(buf, ch);
	sprintf(buf,"Total Auctions Sold              %5d\n\r", sold);
	send_to_char(buf, ch);
	sprintf(buf,"Total Gossips                    %5d\n\r", gossip);
	send_to_char(buf, ch);
	sprintf(buf,"Total oocs                       %5d\n\r", ooc);
	send_to_char(buf, ch);
	sprintf(buf,"Total Arena Battles              %5d\n\r", arena);
	send_to_char(buf, ch);
	sprintf(buf,"Total Notes                      %5d\n\r", tnotes);
	send_to_char(buf, ch);
	sprintf(buf,"Total Quests                     %5d\n\r", quests);
	send_to_char(buf, ch);
	sprintf(buf,"Quests Complete                  %5d\n\r",
questcomplete);
	send_to_char(buf, ch);
	sprintf(buf,"Total QPs Given                  %5d\n\r", qps);
	send_to_char(buf, ch);
	sprintf(buf,"Total Heroes                     %5d\n\r", thero);
	send_to_char(buf, ch);
	sprintf(buf,"Total Remorts                    %5d\n\r", tremort);
	send_to_char(buf, ch);
	sprintf(buf,"Stats as of NULL, NULL 1999\r");
	send_to_char(buf, ch);
}

*/
