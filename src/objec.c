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
#include "player.h"
#include "merc.h"

/*Jeremy's first class*/

void do_ofocus(CHAR_DATA *ch, char *argument)
{
    int sn;
    int spelltype;
    if (!IS_CLASS(ch, CLASS_OBJECTOR))
        {
	      stc("Huh?\n\r", ch);
	      return;
	  }
    if (ch->mana < 500)
        {
            stc("You don't have enough mana.\n\r", ch);
	      return;
        }
    if (( sn = skill_lookup("focus")) < 0) return;
        spelltype = skill_table[sn].target;
        (*skill_table[sn].spell_fun) (sn, 300, ch, ch);
        WAIT_STATE(ch, 12);
	  ch->mana = ch-> mana - 500;
        return;
}

/*void do_objectwalk(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if (!IS_CLASS(ch, CLASS_OBJECTOR))
	{
	    stc("Huh?\n\r", ch);
	    return;
	}
    if (ch->mana < 500)
	{
	    stc("You don't have enough mana.\n\r", ch);
	    return;
	} */
// This class power will let the objector goto an object
// I'll do more tomarrow when I'm not so tired heh.

void do_oscry(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *room, *old_room;
    int num = 1;

    if (!IS_CLASS(ch, CLASS_OBJECTOR))
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    num = number_argument(argument, arg1); // formats stuff like 1.objectname 2.objectname
    if (arg1[0] == 0) // not scrying for anything, scry for what?
    {
	stc("Scry through what object?\n\r", ch);
	return;
    }

    for (obj = object_list; obj; obj = obj->next) // goes through the list of objects
	if (is_name(arg1,obj->name) && --num==0) // checks if the object has the
						 // same name as what is being checked for
						 // decrease num until it is 0 to make sure it's the right one
	    break;				 // break from the loop (we found it!)

    if (!obj) // make sure we didn't break from the list of objects because it went through all
    {
	stc("That object doesn't seem to be in existance.\n\r", ch);
	return;
    }

    while (obj->in_obj) // gets the container of the object (objects in containers aren't in rooms)
	obj = obj->in_obj;

    if (!obj->in_room && !obj->carried_by) // object is not in a room and is not carried by someone
    {
	stc("That object doesn't seem to be in existance.\n\r", ch);
	return;
    }

    room = obj->in_room ? obj->in_room : obj->carried_by->in_room;
    // if object is in a room, ->in_room else ->carried_by->in_room
    // (the player that's holding it's room)

    if (!room) // still a chance that the person carrying the object
	       // is in a null room
    {
	stc("That object doesn't seem to be in existance.\n\r", ch);
	return;
    }

    old_room = ch->in_room;  // make sure we don't lose the room the player is in

    char_from_room(ch); // remove the player from his/her current room
    char_to_room(ch,room); // place him/her in the objects room
    do_look(ch,""); // make the player look (making the scry effect)
    act("$p grows eyes and looks around.", ch, obj, NULL, TO_ROOM);
	// this sends a message to the room 'object looks around'
	// note this does not send it to the player
    char_from_room(ch); // remove player from the objects room
    char_to_room(ch,old_room); // place in the old room
}


