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
/* * *    Midnight Realities was created originally by Smoo (David Brandon) based off of
  * *    Midnight Dreams 1.0[Smoo/Jace/Berelain], based off of Rogue Edition[Listy/Adreno/Others]
 * * *   Based off of Lords of War[Sage/Dunkirk], Based off of Godwars[KaVir], Based off of Merc[Merc Team/merc license]
* * *    Based off of Diku[Diku team/diku license]
 * * *          By using this code, Midnight Realities, You agree to not claim the
 * * * *        work here as your own, and to follow the terms of conditions in the
  * * * *       merc license, diku license, and godwar license, and any licenses that
   * * *        follow this one, as long as they do not break any laws in Canada, the
  * * *         United States, or England, and do not take credit for others work.
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

char *get_line(char *, int);

void do_scribble(CHAR_DATA *ch, char *arg)
{
    OBJ_DATA *scroll;
    int x;
    char *newline;

    if (!IS_CLASS(ch,CLASS_FLUFF))
    {
	stc("&CHehehhehehehehe. That's so cute. You think you're good enough to be a fluffie?\n\r", ch);
	return;
    }

    if (((scroll = get_eq_char(ch,WEAR_WIELD)) && scroll->item_type == ITEM_PAGE)
	&& ((scroll = get_eq_char(ch,WEAR_HOLD)) && scroll->item_type == ITEM_PAGE))
    {
	stc("&MFluffie is confusseled. Which scroll write on now?\n\r", ch);
	return;
    }

    if ((!(scroll = get_eq_char(ch,WEAR_WIELD)) || scroll->item_type != ITEM_PAGE)
	&& (!(scroll = get_eq_char(ch,WEAR_HOLD)) || scroll->item_type != ITEM_PAGE))
    {
	stc("&MFluffie is confusseled. Which scroll write on now?\n\r", ch);
	return;
    }

    if (IS_SET(scroll->spectype, ADV_STARTED))
    {
	stc("&GYou should be &s&Ycheesified&t off that a mage even touched this scroll!", ch);
	return;
    }

    if (!str_cmp(scroll->chpoweruse, "(null)"))
    {
	free_string(scroll->chpoweruse);
	scroll->chpoweruse = str_dup("");
    }

    if (!*arg)
    {
	char *line;
	x = 0;
	while ((line = get_line(scroll->chpoweruse, x))[0])
	{
	    sprintf(log_buf, "&M%02d&c] &W%s\n\r", x, line);
	    stc(log_buf, ch);
	    x++;
	}
	if (x == 0)
	    stc("&RNothing is written on the scroll.\n\r", ch);
	return;
    }

    newline = malloc(strlen(scroll->chpoweruse)+strlen(arg)+3);
    strcpy(newline, scroll->chpoweruse);
    strcat(newline, arg);
    strcat(newline, "\n");
    free_string(scroll->chpoweruse);
    scroll->chpoweruse = str_dup(newline);
    free(newline);
    act("&M$n scribbles something on to $p.", ch, scroll, 0, TO_ROOM);
    act("&MYou scribble something on to $p.", ch, scroll, 0, TO_CHAR);
    return;
}

char *get_line(char *text, int num)
{
    static char line[MAX_INPUT_LENGTH];
    char *p = line;

    *p = '\0';

    while (num >0)
    {
	while (*text && *text != '\n')
	    text++;

	while (*text == '\n' || *text == '\r')
	    text++;

	if (!*text) return line;

	num --;
    }

    while (*text && *text != '\n' && *text != '\r')
    {
	*p = *text;
	p++, text++;
    }

    *p = '\0';

    return line;
}

void do_pluck(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *crea;
    OBJ_INDEX_DATA *index;
    int vnum = 0;

    if (!IS_CLASS(ch, CLASS_FLUFF))
    {
	stc("&CHehehhehehehehe. That's so cute. You think you're good enough to be a fluffie?\n\r", ch);
	return;
    }

    vnum =
	!str_cmp(argument, "pen") ? 26 :
	!str_cmp(argument, "book")? 25 :
	!str_cmp(argument, "page")? 27 :
	0;

    if (vnum == 0)
    {
	stc("&MYou may pluck a pen, book, or page out of your side:)\n\r", ch);
	return;
    }

    if (ch->pcdata->powers[0] <= 0)
    {
	stc("&CHehe. you're out of fur=\\\n\r", ch);
	return;
    }

    if ((index = get_obj_index(vnum)) == NULL)
    {
	stc("&WAwwwww, poor fluffy, that object is broken.", ch);
	return;
    }

    crea = create_object(index,0);

    obj_to_char(crea, ch);
    act("&G$n plucks a few &Mfeathers&G out of $s side and creates $p.", ch, crea, 0, TO_ROOM);
    act("&GYou pluck a few &Mfeathers&G out of your side and create $p.", ch, crea, 0, TO_CHAR);
    ch->pcdata->powers[0]--;
    return;
}

void fluffysing(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *scroll;

    if (((scroll = get_eq_char(ch,WEAR_WIELD)) && scroll->item_type == ITEM_PAGE)
	&& ((scroll = get_eq_char(ch,WEAR_HOLD)) && scroll->item_type == ITEM_PAGE))
    {
	stc("&MFluffie is confusseled. Which scroll has song now?\n\r", ch);
	return;
    }

    if ((!(scroll = get_eq_char(ch,WEAR_WIELD)) || scroll->item_type != ITEM_PAGE)
	&& (!(scroll = get_eq_char(ch,WEAR_HOLD)) || scroll->item_type != ITEM_PAGE))
    {
	stc("&MFluffie is confusseled. Which scroll has song now?\n\r", ch);
	return;
    }

    ch->pcdata->powers[1] = scroll->ObjId;
    scroll->value[0] = 0;
    act("&C$n begins to sing a song, confusing you with $s timing.", ch, 0, 0, TO_ROOM);
    act("&CEveryone looks confused as you begin to sing a song.", ch, 0, 0, TO_CHAR);
}

void sing(CHAR_DATA *ch)
{
    OBJ_DATA *pg;
    char *l;

    if (!IS_CLASS(ch, CLASS_FLUFF)) return;

    if (ch->hit <= 0) {	act("&C$n sniffs as $e is unable to finish $s song.", ch, 0, 0, TO_ROOM);
			act("&CYou sniff because you are unable to finish your song.", ch, 0, 0, TO_CHAR);
			ch->pcdata->powers[1] = 0;}

    if (ch->pcdata->powers[1] == 0) return;

    for (pg = ch->carrying; pg; pg = pg->next_content)
    {
	if (pg->ObjId == ch->pcdata->powers[1] && pg->item_type == ITEM_PAGE) break;
    }

    if (!pg) {stc("&RThe song you were singing has been destroyed.\n\r", ch);
	ch->pcdata->powers[1] = 0;return;}

    if ((l = get_line(pg->chpoweruse, pg->value[0]))[0])
    {
	verse(ch,l);
	pg->value[0]++;
    }
    else
    {
	ch->pcdata->powers[1] = 0;
	act("&CYou finish, bow your head, and wait for a clap.", ch, 0, 0, TO_CHAR);
	act("&C$n finishes, bows $s head, and waits for a clap.", ch, 0, 0, TO_ROOM);
    }

}

void verse(CHAR_DATA *ch, char *line)
{
    char buf[MIL*2];
//    char *p = line;

    sprintf(buf, "&M$n &Rsings, \'&C%s&R\'\n\r", line);
    act(buf, ch, 0, 0, TO_ROOM);
    sprintf(buf, "&MYou &Rsing, \'&C%s&R\'\n\r", line);
    act(buf, ch, 0, 0, TO_CHAR);
}
