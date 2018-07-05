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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


/* Needs Major Revision!!! -Gol*/
void do_detox( CHAR_DATA *ch, char *argument )
{        
    if ( IS_NPC(ch) )
		return;
  

	if (ch->class == 0)
	{	
		send_to_char("You are already classless.\n\r", ch);
		return;
	}
	else if (ch->in_room->vnum != 3206)
	{
		send_to_char("You are not in the detox chamber.\n\r", ch);
		return;
	}
	else if (ch->pcdata->quest < 1000)
	{
		send_to_char("You cannot afford the 1000 qp required to detox.\n\r", ch);
		return;
	}
	else if ( IS_AFFECTED(ch, AFF_CHARM)   )
	{
		stc("You can't do that while charmed!\n\r", ch);
		return;
	}

	do_class(ch,"self none");
	ch->pcdata->quest -= 1000;
	// damn why the hell would the team at rogue edition write a detox like
	// this one was..it's this simple..class self none, and it's easier to 
	// update.
	return;
}
