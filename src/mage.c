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

char *target_name;
int stack_count;
int totdam;

/*

 * Local functions.

 */

#define DROW_MAGIC_CONT(ch,vch) \
	    if (IS_CLASS((vch), CLASS_DROW) && number_percent( ) < (vch)->pcdata->stats[DROW_MAGIC])\
	    {\
		act("$N resists your spell.", ch, 0, (vch), TO_CHAR);\
		act2("$N resists $n's spell.", ch, 0, (vch), TO_ROOM);\
		act("You resists $n's spell.", ch, 0, (vch), TO_VICT);\
		continue;\
	    }

#define DROW_MAGIC_RET(ch,victim) \
	    if (IS_CLASS((victim), CLASS_DROW) && number_percent( ) < (victim)->pcdata->stats[DROW_MAGIC])\
	    {\
		act("$N resists your spell.", ch, 0, (victim), TO_CHAR);\
		act2("$N resists $n's spell.", ch, 0, (victim), TO_ROOM);\
		act("You resists $n's spell.", ch, 0, (victim), TO_VICT);\
		return;\
	    }




void	adv_spell_stack		args( ( CHAR_DATA *ch, OBJ_DATA *book,
				OBJ_DATA *page, char *argument) );

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 

				OBJ_DATA *page, char *argument) );

void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 

				OBJ_DATA *page, char *argument) );

void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 

				OBJ_DATA *page, char *argument) );



/*Ceremony stuff Listy*/

void do_ceremony (CHAR_DATA *ch, char *argument)

{

	if (IS_NPC(ch))

		return;

	if (!IS_CLASS(ch, CLASS_MAGE))

	{

		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);

		return;

	}

	if (ch->fight_timer > 0)

	{

		send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);

		return;

	}

	if (ch->hit < 50)

	{

		send_to_char("You do not have the required health to do that.\n\r", ch);

		return;

	}

	if (ch->move < 50)

	{

		send_to_char("You do not have the required vitality to do that.\n\r", ch);

		return;

	}

	ch->hit -= 50;

	ch->move -= 50;

	ch->mana += 500;

	if (ch->mana > (ch->max_mana + 3000))

		ch->mana = (ch->max_mana + 3000);

	send_to_char("You initiate an ancient ceremony filling you with new found power.\n\r",ch);

	act("$n glows slightly as they mutter ancient texts of power.",ch,NULL,NULL,TO_ROOM);

	WAIT_STATE(ch, 5);

	return;

}

void do_advocate (CHAR_DATA *ch, char *argument)
{
	int count;
	int mastered;
	int cost;

	if (IS_NPC(ch))
	{
		stc("Hmmm.. the mud thinks your a mob, inform an imm!\n\r", ch);
		return;
	}
	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You mutter words in an attempt to look like a mage.\n\r", ch);
		return;
	}
	if (ch->level == LEVEL_ARCHMAGE)
	{
		send_to_char("You have been advocated to the highest level.\n\r", ch);
		return;
	}
	if (ch->fight_timer > 0)
	{
		send_to_char("You may not master yourself whilst you are tainted from a fight.\n\r", ch);
		return;
	}
	if (ch->level < 3)
	{
		send_to_char("You must be an avatar to use this command.\n\r", ch);
		return;
	}
	count = 0;
	mastered = 0;
	while (count < 5)
	{
		if (ch->spl[count] > 274)
			mastered++;
		count++;
	}
	if (ch->level == LEVEL_APPRENTICE)
	{
		cost = EXP(ch,10000000);
		if (ch->exp < cost)
		{
			sprintf(log_buf, "You do not have the %d experience required to become a mage.\n\r", cost);
			stc(log_buf, ch);
			return;
		}
		if (ch->max_mana < 5000)
		{
			send_to_char("You mana level has not reached the 5000 required.\n\r", ch);
			return;
		}
		if (ch->max_hit < 7500)
		{
			send_to_char("You hit point level has not reached the 7500 required.\n\r", ch);
			return;
		}
		if (mastered < 2)
		{
			send_to_char("You need to be a rogue archmage in at least 2 magic colours", ch);
			return;
		}
		ch->level = LEVEL_MAGE;
		ch->exp -= cost;
		ch->pcdata->powers[MPOWER_RUNE1] = 2047;
		ch->pcdata->powers[MPOWER_RUNE2] = 1023;
		ch->pcdata->powers[MPOWER_RUNE3] = 15; 
		send_to_char("You become a mage.\n\r",ch);
		return;
	}
	else if (ch->level == LEVEL_MAGE)
	{
		cost = EXP(ch,25000000);
		if (ch->exp < cost)
		{
			sprintf(log_buf, "You do not have the %d required experience to become an archmage.\n\r", cost);
			stc(log_buf, ch);
			return;
		}
		if (ch->max_mana < 12500)
		{
			send_to_char("You mana level has not reached the 12500 required.\n\r", ch);
			return;
		}
		if (ch->max_hit < 12500)
		{
			send_to_char("You hit point level has not reached the 12500 required.\n\r", ch);
			return;
		}
		if (mastered < 5)
		{
			send_to_char("You need to be a rogue archmage in at all magic colours", ch);
			return;
		}
		ch->level = LEVEL_ARCHMAGE;
		ch->exp -= cost;
		ch->pcdata->powers[MPOWER_RUNE1] = 2047;
		ch->pcdata->powers[MPOWER_RUNE2] = 1023;
		ch->pcdata->powers[MPOWER_RUNE3] = 15; 
		send_to_char("You become an archmage.\n\r", ch);
		return;
	}
}
 void do_soulbond (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
      send_to_char("You cannot feel the magic.\n\r", ch);
      return;
    }
    if (IS_SET(ch->extra,EXTRA_TRANSC))
    {
      send_to_char("You have already become one with the magic.\n\r", ch);
      return;
    }
    if (ch->level != LEVEL_ARCHMAGE)
    {
      send_to_char("You must be an archmage to bond with the magic.\n\r", ch);
      return;
    }
    if (ch->fight_timer > 0)
    {
      send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);
      return;
    }
    if (ch->level < 3)
    { 
      send_to_char("You must be an avatar to use this command.\n\r", ch);
      return;
    }
    if (ch->exp < EXP(ch,5000000))
    { 
      sprintf(log_buf, "You require %d experience to transcend.\n\r", EXP(ch,5000000));
      return;
    }
    ch->exp -= EXP(ch,5000000);
    SET_BIT(ch->extra,EXTRA_TRANSC);
    send_to_char("You scream in sheer ecstacy as the magic bonds with your soul.\n\r", ch);
    return;
    }
    void do_planeshift ( CHAR_DATA *ch, char *argument)
    {
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *victim;
    one_argument (argument, arg);
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (!IS_SET(ch->extra,EXTRA_TRANSC))
    { 
      send_to_char("You must first bond with the magic.\n\r", ch);
      return;
    }
    if (ch->fight_timer > 5)
    { 
      send_to_char("You may not travel the path of magic whilst tainted from a fight.\n\r", ch);
      return;
    }
    if (ch->level < 3)
    {
      send_to_char("You must be an avatar to use this command.\n\r", ch);
      return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
      send_to_char("Walk the pathway of magic to find whom?\n\r", ch);
      return;
    }
    location = victim->in_room;
    if (ch->mana < 2500)
    {
      send_to_char("You do not have the required 2500 mana to enter the rifts of magic.\n\r", ch);
      return;
    }
    if (victim->mana > ch->mana)
    {
      send_to_char("You cannot seem to overpower their magical energies.\n\r", ch);
      return;
    }
    act("You shift into the plane of magic.", ch, NULL, NULL, TO_CHAR);
    act("$n shifts into a field of magical energy.", ch, NULL, NULL, TO_ROOM);
    ch->mana -= 2500;
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "auto");
    act("You emerge from the plane of magic.", ch, NULL, NULL, TO_CHAR);
    act("$n emerges from the magical plane.", ch, NULL, NULL, TO_ROOM);
    WAIT_STATE(ch, 12);
    return;
    }
void do_solar (CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE) )
    {
      send_to_char("huh?\n\r", ch);
      return;
    }
    if (ch->level < 3)
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->generation > 1)
    {
      send_to_char("You must be a Grand Sorcerer to call upon the sun.\n\r", ch);
      return;
    }
    if (!IS_SET(ch->extra,EXTRA_TRANSC))
    {
      send_to_char("You have not yet bonded with the magic.\n\r", ch);
      return;
    }
    if (ch->mana < 10000)
    {
      send_to_char("You do not have enough mana.\n\r", ch);
      return;
    }
    if (ch->practice < 50)
    {
      send_to_char("You require 50 primal energy to summon this power.\n\r", ch);
      return;
    }
    if (ch->hit < 9001)
    {
      send_to_char("You do not have enough vitality to summon a solar flare.\n\r", ch);
      return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char("They aren't here.\n\r", ch);
      return;
    }
//    if (is_safe(ch, victim))
//    return;
    if (victim->level < 3)
    {
      send_to_char("Not on a mortal.\n\r", ch);
      return;
    }
    if (IS_NPC(victim))
    {
      send_to_char("Huh?", ch);
      return;
    }
    act("You raise your arms to the sky commanding the power of the sun to strike down $N.",ch,NULL,victim,TO_CHAR);
    act("$n raises their arms to the sky chanting as the sun streams forth a deadly explosion.",ch,NULL,victim,TO_NOTVICT);
    act("$n chants in rage as a mammoth solar flare descends from the sky and strikes you down!",ch,NULL,victim,TO_VICT);
    victim->hit -= IS_CLASS(victim,CLASS_DROW) ? 17500 :10000;
    ch->mana -= 10000;
    ch->practice -= 50;
    if (ch->fighting == NULL)
    set_fighting(ch,victim);
    update_pos(victim);
    update_pos(ch);
    WAIT_STATE(ch, 5);
    act("You feel the weakening power of the sun wash over you in agony!",ch,NULL,victim,TO_CHAR);
    act("$n is engulfed in a flaming aura and slumps to the ground weakened from the spell.",ch,NULL,victim,TO_NOTVICT);
    act("$n is engulfed in a flaming aura and slumps to the ground weakened from the spell.",ch,NULL,victim,TO_VICT);
    ch->hit -= 9000;
    ch->position = POS_STUNNED;
    WAIT_STATE(ch, 40);
    return;
}
void do_stop (CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE) )
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->level < 3)
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (!IS_SET(ch->extra,EXTRA_TRANSC))
    {
      send_to_char("You must first bond with the magic.\n\r", ch);
      return;
    }
    if (ch->mana < 2000)
    {
      send_to_char("You do not have enough mana.\n\r", ch);
      return;
    }
    if (ch->hit < 1501)
    {
      send_to_char("You do not have enough vitality.\n\r", ch);
      return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
//    if (is_safe(ch, victim))
//    return;
    if (victim->level < 3) 
    {
      send_to_char("You may not cast stop on a mortal!\n\r", ch);
      return;
    }
    act("You point at $N and utter the word 'Xanthus!'",ch,NULL,victim,TO_CHAR);
    act("$n points at $N and utters the word 'Xanthus!'",ch,NULL,victim,TO_NOTVICT);
    act("$n utters the word 'Xanthus!' at you and you can't seem to move!",ch,NULL,victim,TO_VICT);
    victim->position = POS_STUNNED;
    ch->mana -= 4000;
    act("You can feel the powerful magic drain away your very lifeforce.",ch,NULL,victim,TO_CHAR);
    act("$n grimaces in pain from casting the powerful magic.",ch,NULL,victim,TO_NOTVICT);
    act("$n grimaces in pain from casting this potent magic.",ch,NULL,victim,TO_VICT);
    ch->hit -= 2500;
    return;
    }
void do_necromancy(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
      send_to_char("You cannot feel the magic.\n\r", ch);
      return;
    }
    if (!IS_SET(ch->extra,EXTRA_TRANSC))
    {
      send_to_char("You must first obtain the rank of sorcerer before studying a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->extra, EXTRA_NECRO))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->extra, EXTRA_ILLUS))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->newbits, NEW_INVK))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (ch->fight_timer > 0)
    {
      send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);
      return;
    }
    if (ch->level < 3)
    {
      send_to_char("You must be an avatar to use this command.\n\r", ch);
      return;
    }
    if (ch->exp < EXP(ch,10000000))
    {
      sprintf(log_buf, "You do not have the required %d experience to study.\n\r", EXP(ch,10000000));
      stc(log_buf, ch);
      return;
    }
    ch->exp -= EXP(ch,10000000);
    SET_BIT(ch->extra, EXTRA_NECRO);
    send_to_char("You study the Ancient ways of Necromancy.\n\r", ch);
    send_to_char("You feel the power of the dead surge through your veins!\n\r", ch);
    return;
}
void do_illusion(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    return;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (!IS_SET(ch->extra, EXTRA_TRANSC))
    {
      send_to_char("You must first obtain the rank of Sorcerer before studying a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->extra, EXTRA_NECRO))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->extra, EXTRA_ILLUS))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (IS_SET(ch->newbits, NEW_INVK))
    {
      send_to_char("You have already chosen a profession.\n\r", ch);
      return;
    }
    if (ch->fight_timer > 0)
    {
      send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);
      return;
    }
    if (ch->level < 3)
    {
      send_to_char("You must be an avatar to use this command.\n\r", ch);
      return;
    }
    if (ch->exp < EXP(ch,10000000))
    {
      sprintf(log_buf, "You do not have the required %d experience to study this profession.\n\r", EXP(ch,10000000));
      stc(log_buf, ch);
      return;
    }
    ch->exp -= EXP(ch,10000000);
    SET_BIT(ch->extra, EXTRA_ILLUS);
    send_to_char("You study the ancient art of illusion.\n\r", ch);
    send_to_char("You feel a surge of magical energy wave over you!\n\r", ch);
    return;
}
void do_invocation(CHAR_DATA *ch, char *argument)
{
   if (IS_NPC(ch))
   return;
   if (!IS_CLASS(ch, CLASS_MAGE))
   {
     send_to_char("Huh?\n\r", ch);
     return;
   }
   if (!IS_SET(ch->extra, EXTRA_TRANSC))
   {
     send_to_char("You must first obtain the rank of Sorcerer before studying this profession.\n\r", ch);
     return;
   }
   if (IS_SET(ch->extra, EXTRA_NECRO))
   {
     send_to_char("You have already chosen a profession.\n\r", ch);
     return;
   }
   if (IS_SET(ch->extra, EXTRA_ILLUS))
   {
     send_to_char("You have already chosen a profession.\n\r", ch);
     return;
   }
   if (IS_SET(ch->newbits, NEW_INVK))
   {
     send_to_char("You have already chosen a profession.\n\r", ch);
     return;
   }
   if (ch->fight_timer > 0)
   {
     send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);
     return;
   }
   if (ch->level < 3)
   {
     send_to_char("You must be an avatar to use this command.\n\r", ch);
     return;
   }
   if (ch->exp < EXP(ch,10000000))
   {
     sprintf(log_buf, "You do not have the required %d experience to train this profession.\n\r", EXP(ch,10000000));
     stc(log_buf, ch);
     return;
   }
   ch->exp -= EXP(ch, 10000000);
   SET_BIT(ch->newbits, NEW_INVK);
   send_to_char("You begin your studies into the ancient art of invocation.\n\r", ch);
   send_to_char("You are blessed with the knoweledge of invocation!\n\r", ch);
   return;
}
  int chain = 0;
void do_chant( CHAR_DATA *ch, char *argument )

{

    CHAR_DATA *victim;

    OBJ_DATA *book;

    OBJ_DATA *page;

    OBJ_DATA *obj;

    char arg[MAX_INPUT_LENGTH];

    bool victim_target = FALSE;

    bool object_target = FALSE;

    bool global_target = FALSE; /* Target object/victim may be anywhere */

    int spellno = 1;

    int spellcount = 0;

    int spelltype;

    int sn = 0;

    int level;



    one_argument( argument, arg );



	if (IS_NPC(ch) )

		return;

	if (!IS_CLASS(ch, CLASS_MAGE) && (!IS_IMMORTAL(ch)))

	{

		send_to_char("You cannot speak in tongues you do not understand", ch);

		return;

	}

/*

	if (ch->level < 3)

	{

		send_to_char("Your mortal form cannot understand the arcane writing.\n\r", ch);

		return;

	}
*/
    if ( ( book = get_eq_char(ch, WEAR_WIELD) ) == NULL || book->item_type != ITEM_BOOK)



    {



	if ( ( book = get_eq_char(ch, WEAR_HOLD) ) == NULL || book->item_type != ITEM_BOOK)



	{



	    send_to_char( "First you must hold a spellbook.\n\r", ch );



	    return;



	}



    }

    chain = -1;

    if (IS_SET(book->value[1], CONT_CLOSED))



    {



	send_to_char( "First you better open the book.\n\r", ch );



	return;



    }



    if (book->value[2] < 1)



    {



	send_to_char( "There are no spells on the index page!\n\r", ch );



	return;



    }



    if ( ( page = get_page( book, book->value[2] ) ) == NULL )



    {



	send_to_char( "The current page seems to have been torn out!\n\r", ch );



	return;



    }



	if (IS_AFFECTED(ch,AFF_ETHEREAL))
        {
                send_to_char("You cannot cast spells in this form.\n\r",ch);
                return;
        }



    spellcount = (page->value[1] *10000) + (page->value[2] *10) + page->value[3];
stack_count = 0;
totdam = 0;


    act("You chant the arcane words from $p.",ch,book,NULL,TO_CHAR);



    act("$n chants some arcane words from $p.",ch,book,NULL,TO_ROOM);


    if (IS_SET(page->quest, QUEST_MASTER_RUNE))



    {



	ch->spectype = 0;



	if (IS_SET(page->spectype, ADV_FAILED) || 



	    !IS_SET(page->spectype, ADV_FINISHED))



	    send_to_char( "The spell failed.\n\radv_failed/!finished", ch );

	else if (IS_SET(page->spectype, ADV_STACK))
	    adv_spell_stack(ch, book, page, argument);

	else if (IS_SET(page->spectype, ADV_DAMAGE)) 



	    adv_spell_damage(ch,book,page,argument);

	    else if (IS_SET(page->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page, argument);


	else if (IS_SET(page->spectype, ADV_AFFECT))



	    adv_spell_affect(ch,book,page,argument);



	else if (IS_SET(page->spectype, ADV_ACTION))



	    adv_spell_action(ch,book,page,argument);



	else send_to_char( "The spell failed.\n\r", ch );

	return;



    }



    switch ( spellcount )



    {



    default:



	send_to_char( "Nothing happens.\n\r", ch );



	return;



    case 10022:



	/* FIRE + DESTRUCTION + TARGETING */



	sn = skill_lookup( "fireball" );



	victim_target = TRUE;



	spellno = 7;



	break;



    case 640322:



	/* LIFE + ENHANCEMENT + TARGETING */



	sn = skill_lookup( "heal" );



	victim_target = TRUE;



	spellno = 5;



	break;



    case 6400044:



	/* DEATH + SUMMONING + AREA */



	sn = skill_lookup( "guardian" );



	spellno = 4;



	break;



    case 2565128:



	/* MIND + INFORMATION + OBJECT */



	sn = skill_lookup( "identify" );



	object_target = TRUE;



	global_target = TRUE;



	break;



    }



    if ( arg[0] == '\0' && (victim_target == TRUE || object_target == TRUE))



    {



	send_to_char( "Please specify a target.\n\r", ch );



	return;



    }







    if (victim_target && sn > 0)



    {



	if ( !global_target && ( victim = get_char_room( ch, arg ) ) == NULL )



	{



	    send_to_char( "They are not here.\n\r", ch );



	    return;



	}



	else if ( global_target && ( victim = get_char_world( ch, arg ) ) == NULL )



	{



	    send_to_char( "They are not here.\n\r", ch );



	    return;



	}


	if (victim->level < 3)

	{

		send_to_char("You cannot seem to affect them through your chantings.\n\r", ch);

		return;

	}

	if (victim->max_hit < 3000 && !IS_NPC(victim))

	{

		send_to_char("You cannot affect someone who does not have the vitality to defend themselves.\n\r", ch);

		return;

	}

/* End */

	if (IS_ITEMAFF(victim, ITEMA_REFLECT))



	{



	    send_to_char( "You are unable to focus your spell on them.\n\r", ch );



	    return;



	}







        if (!IS_NPC(victim) && IS_CLASS( victim, CLASS_DROW ))



        {



            if (ch == victim )



            {



                send_to_char("You lower your magical resistance....\n\r", ch);



            }



            else if (number_percent ( )  <=  victim->pcdata->stats[DROW_MAGIC])



            {



               send_to_char("Your spell does not affect them.\n\r", ch);



               return;



            }



        }







	spelltype = (skill_table[sn].target);



	level = ch->spl[spelltype] * 0.30;



	(*skill_table[sn].spell_fun) ( sn, level, ch, victim );



	if (spellno > 1) (*skill_table[sn].spell_fun) ( sn, level, ch, victim );



	if (spellno > 2) (*skill_table[sn].spell_fun) ( sn, level, ch, victim );

	if (spellno > 3) (*skill_table[sn].spell_fun) ( sn, level, ch, victim );

	if (spellno > 4) (*skill_table[sn].spell_fun) ( sn, level, ch, victim );



        if (!IS_IMMORTAL( ch ))



		WAIT_STATE(ch, skill_table[sn].beats);



    }



    else if (object_target && sn > 0)



    {



	if ( !global_target && ( obj = get_obj_carry( ch, arg ) ) == NULL )



	{



	    send_to_char( "You are not carrying that object.\n\r", ch );



	    return;



	}



	else if ( global_target && ( obj = get_obj_world( ch, arg ) ) == NULL )



	{



	    send_to_char( "You cannot find any object like that.\n\r", ch );



	    return;



	}



	spelltype = (skill_table[sn].target);



	level = ch->spl[spelltype] * 0.30;



	(*skill_table[sn].spell_fun) ( sn, level, ch, obj );



	if (spellno > 1) (*skill_table[sn].spell_fun) ( sn, level, ch, obj );



	if (spellno > 2) (*skill_table[sn].spell_fun) ( sn, level, ch, obj );


	if (spellno > 3) (*skill_table[sn].spell_fun) ( sn, level, ch, obj );


	if (spellno > 4) (*skill_table[sn].spell_fun) ( sn, level, ch, obj );



	if (!IS_IMMORTAL( ch))	



		WAIT_STATE(ch, skill_table[sn].beats);



    }



    else if (sn > 0)



    {



	spelltype = (skill_table[sn].target);



	if (spelltype == TAR_OBJ_INV)



	{



	    send_to_char( "Nothing happens.\n\r", ch );



	    return;



	}



	level = ch->spl[spelltype] * 0.30;



	(*skill_table[sn].spell_fun) ( sn, level, ch, ch );



	if (spellno > 1) (*skill_table[sn].spell_fun) ( sn, level, ch, ch );



	if (spellno > 2) (*skill_table[sn].spell_fun) ( sn, level, ch, ch );


	if (spellno > 3) (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
 

	if (spellno > 4) (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
  


	if (!IS_IMMORTAL( ch ))



		WAIT_STATE(ch, skill_table[sn].beats);



    }



    else send_to_char( "Nothing happens.\n\r", ch );



    return;



}







OBJ_DATA *get_page( OBJ_DATA *book, int page_num )



{



    OBJ_DATA *page;



    OBJ_DATA *page_next;







    if (page_num < 1) return NULL;



    for ( page = book->contains; page != NULL; page = page_next )



    {



	page_next = page->next_content;



	if (page->value[0] == page_num) return page;



    }



    return NULL;



}







void adv_spell_damage( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)



{



    char arg [MAX_STRING_LENGTH];



    char buf [MAX_STRING_LENGTH];



    OBJ_DATA *page_next;



    ROOM_INDEX_DATA *old_room;



    char next_par [MAX_INPUT_LENGTH];



    int mana_cost = (page->points * 1) * 1.5;


    int min = page->value[1] > 500 ? 500 : page->value[1];



    int max = page->value[2] > 2000 ? 2000 : page->value[2];



    int dam;



    int level;



    bool area_affect = FALSE;



    bool victim_target = FALSE;



    bool global_target = FALSE;



    bool next_page = FALSE;



    bool parameter = FALSE;



    bool not_caster = FALSE;



    bool no_players = FALSE;



    bool cast_message = FALSE;



    bool reversed = FALSE;





    if (ch->mana < mana_cost)



    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}






    if (min < 1 || max < 1)

    {send_to_char("The spell failed.\n\r",ch);return;}

    if (max < min)

    {send_to_char("The spell failed. (The max must be over the min!\r\n", ch); return;}

    if (++chain > 10)
    {
	stc("You may only chain together 10 spells.\n\rThe spell failed.\n\r", ch);
	return;
    }



    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	IS_SET(page->spectype, ADV_SPELL_FIRST))



    {



    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\r",ch);return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,arg);



	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )



	{send_to_char( "The spell failed.\n\r", ch );return;}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\r", ch );return;}



    	}



	else {send_to_char( "The spell failed.\n\r", ch );return;}



    }







    one_argument( argument, arg );







    if (strlen(page->victpoweron) > 0 || str_cmp(page->victpoweron,"(null)"))



    {



	if (strlen(page->victpoweroff) > 0 || str_cmp(page->victpoweroff,"(null)"))



	    cast_message = TRUE;



    }







    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;



    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;



    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;



    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;



    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;



    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;



    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;



    if (IS_SET(page->spectype, ADV_REVERSED     )) reversed      = TRUE;







    if (victim_target)



    {



	CHAR_DATA *victim;



	



	if ( !global_target && ( victim = get_char_room( ch, arg ) ) == NULL )



	{



	    send_to_char( "They are not here.\n\r", ch );



	    return;



	}



	else if ( global_target && ( victim = get_char_world( ch, arg ) ) == NULL )



	{



	    send_to_char( "They are not here.\n\r", ch );



	    return;



	}



	/* AREA LIMIT */
/*
	if ( global_target && (victim->in_room->area != ch->in_room->area) )

	{

	    send_to_char( "They are not in this area.\n\r", ch );

	    return;

	}
*/

	if ( global_target && (victim->in_room != ch->in_room))
	mana_cost *= 2;

/*
	if (IS_SET(ch->in_room->room_flags, ROOM_SAFE) && !reversed

	    && (victim->fight_timer == 0 || ch->in_room != victim->in_room) )

	{

	    send_to_char("You cannot fight in a safe room.\n\r",ch);

	    return;

	}
*/

if (ch != victim && IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You emerge from your cloak of magic.\n\r", ch );
        act("$n slowly emerges from a cloak of magic.",ch,NULL,NULL,TO_ROOM);
    }
DROW_MAGIC_RET(ch,victim);
	if (reversed)


{improve_spl(ch,2,0);
	    level = ch->spl[BLUE_MAGIC];}



	else {



improve_spl(ch,1,0);
	    level = ch->spl[RED_MAGIC];}



	if (ch->in_room == victim->in_room)



	    dam = number_range(min,max) + level;



	else



	    dam = number_range(min,max);



	if (ch->spectype < 5000)



	{

	    ch->spectype += dam;

	}



	else dam = 0;

	old_room = ch->in_room;



	if (victim->in_room != NULL && victim->in_room != ch->in_room)



	{



	    char_from_room(ch);



	    char_to_room(ch,victim->in_room);



	}



	if (!reversed)



	{


/*
	    if (is_safe(ch,victim))



	    {



		char_from_room(ch);



		char_to_room(ch,old_room);



		return;



	    }

*/

	}



	else if (IS_ITEMAFF(victim, ITEMA_REFLECT))



	{



	    send_to_char( "You are unable to focus your spell on them.\n\r", ch );



	    char_from_room(ch);



	    char_to_room(ch,old_room);



	    return;



	}



	char_from_room(ch);



	char_to_room(ch,old_room);


	if (cast_message)



	{



	    strcpy(buf,page->victpoweron);



	    act2(buf,ch,NULL,victim,TO_CHAR);



	    strcpy(buf,page->victpoweroff);



	    act2(buf,ch,NULL,victim,TO_ROOM);



	}



	if (reversed)



	{


if (dam + totdam > 1500) dam = 1500 - totdam;
totdam += dam;

	    victim->hit += dam;



	    if (victim->hit > victim->max_hit) victim->hit = victim->max_hit;



	}



	else



	{

if (is_safe(ch, victim)) return;


	    adv_damage(ch,victim,dam);



	    sprintf(buf,"Your %s strikes $N incredibly hard!",page->chpoweroff);



	    act2(buf,ch,NULL,victim,TO_CHAR);



	    sprintf(buf,"$n's %s strikes $N incredibly hard!",page->chpoweroff);



	    act2(buf,ch,NULL,victim,TO_NOTVICT);



	    sprintf(buf,"$n's %s strikes you incredibly hard!",page->chpoweroff);



	    act2(buf,ch,NULL,victim,TO_VICT);



	    if (number_percent() <= victim->atm)
	    {
		act( "Your spell is unable to penetrate $n's antimagic shield.", ch, NULL, victim, TO_CHAR );


		act( "$n's spell is unable to penetrate your antimagic shield.", ch, NULL, victim, TO_VICT );
	    }


	    else



		hurt_person(ch,victim,dam);



	}



	if (!IS_IMMORTAL( ch ))



	{



		WAIT_STATE(ch, PULSE_VIOLENCE);



		ch->mana -= mana_cost;



	}



    }



    else if (area_affect)



    {



	CHAR_DATA *vch = ch;



	CHAR_DATA *vch_next;







	if (reversed)



	    level = ch->spl[BLUE_MAGIC];



	else



	    level = ch->spl[RED_MAGIC];



	dam = number_range(min,max) + (level * 0.10);



	if (ch->spectype < 5000)



	{



	    ch->spectype += dam;



	}



	else dam = 0;



/*	if (IS_SET(ch->in_room->room_flags, ROOM_SAFE) && !reversed)



	{



	    send_to_char("You cannot fight in a safe room.\n\r",ch);



	    return;



	}

*/

	if (cast_message)



	{



	    strcpy(buf,page->victpoweron);



	    act2(buf,ch,NULL,vch,TO_CHAR);



	    strcpy(buf,page->victpoweroff);



	    act2(buf,ch,NULL,vch,TO_ROOM);



	}







	for ( vch = char_list; vch != NULL; vch = vch_next )



	{



	    vch_next	= vch->next;



	    if ( vch->in_room == NULL ) continue;



	    if ( ch == vch && not_caster ) continue;



	    if ( !IS_NPC(vch) && no_players ) continue;



	    if (!reversed)



	    {


/*
		if (is_safe(ch,vch)) continue;
*/


	    }



	    if (IS_ITEMAFF(vch, ITEMA_REFLECT))



	    {



		send_to_char( "You are unable to focus your spell on them.\n\r", ch );



		continue;



	    }


	    if ( vch->in_room == ch->in_room )



	    {



		if (reversed)



		{


if (dam + totdam > 1500) dam = 1500 - totdam;
totdam += dam;

		    vch->hit += dam;



		    if (vch->hit > vch->max_hit) vch->hit = vch->max_hit;



		}



		else



		{

		    adv_damage(ch,vch,dam);



		    sprintf(buf,"Your %s strikes $N incredably hard!",page->chpoweroff);



		    act2(buf,ch,NULL,vch,TO_CHAR);



		    sprintf(buf,"$n's %s strikes $N incredably hard!",page->chpoweroff);



		    act2(buf,ch,NULL,vch,TO_NOTVICT);



		    sprintf(buf,"$n's %s strikes you incredably hard!",page->chpoweroff);



		    act2(buf,ch,NULL,vch,TO_VICT);
		DROW_MAGIC_CONT(ch,vch);


		    if (number_percent() <= vch->atm)
		    {
			act( "Your spell is unable to penetrate $n's antimagic shield.", ch, NULL, vch, TO_CHAR );


			act( "$n's spell is unable to penetrate your antimagic shield.", ch, NULL, vch, TO_VICT );
		    }


		    if (IS_ITEMAFF(vch, ITEMA_RESISTANCE) && dam > 1)



			hurt_person(ch,vch,number_range(1,dam));



/*		    else if (!IS_NPC(vch) && IS_SET(vch->act, PLR_WEREWOLF) && 



			vch->pcdata->disc[TOTEM_BOAR] > 2 && dam > 1)



			hurt_person(ch,vch,(dam*0.5));*/



		    else hurt_person(ch,vch,dam);



		}



		continue;



	    }



	}



	if (!IS_IMMORTAL(ch))



	{



		WAIT_STATE(ch, PULSE_VIOLENCE);



		ch->mana -= mana_cost;



	}



    }



    else {send_to_char("The spell failed.\n\r",ch);return;}



    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	!IS_SET(page->spectype, ADV_SPELL_FIRST))



    {



    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\r",ch);return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,arg);




	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )



	{send_to_char( "The spell failed.\n\r", ch );return;}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\r", ch );return;}



    	}



	else {send_to_char( "The spell failed.\n\r", ch );return;}



    }



    return;



}







void adv_spell_affect( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)



{



    char arg [MAX_STRING_LENGTH];



    char c_m [MAX_INPUT_LENGTH];



    char c_1 [MAX_INPUT_LENGTH];



    char c_2 [MAX_INPUT_LENGTH];



    OBJ_DATA *page_next;



    OBJ_DATA *obj = NULL;



    CHAR_DATA *victim = ch;



    char next_par [MAX_INPUT_LENGTH];



    int mana_cost = page->points;



    int apply_bit = page->value[1];



    int bonuses = page->value[2];



    int affect_bit = page->value[3];



    int sn;



    int level = page->level;



    bool any_affects = FALSE;



    bool area_affect = FALSE;



    bool victim_target = FALSE;



    bool object_target = FALSE;



    bool global_target = FALSE;



    bool next_page = FALSE;



    bool parameter = FALSE;



    bool not_caster = FALSE;



    bool no_players = FALSE;



    bool cast_message = FALSE;



    bool message_one = FALSE;



    bool message_two = FALSE;



    bool is_reversed = FALSE;
#define erui {}

mana_cost /= 1.5;
mana_cost/=2;

    if (++chain > 10)
    {
	stc("You may only chain together 10 spells.\n\rThe spell failed.\n\r", ch);
	return;
    }


    if (ch->mana < mana_cost)



    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}


	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot chant your way out of the arena.\n\r", ch);
		return;
	}
	if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
	{
		send_to_char("You cannot chant your way out of the arena.\n\r", ch);
		return;
	}





    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	IS_SET(page->spectype, ADV_SPELL_FIRST))



    {



    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\r",ch);erui;return;
}



	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,argument);




	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )



	{send_to_char( "The spell failed.\n\r", ch );return;	erui
}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\r", ch );return;	erui
}



    	}



	else {send_to_char( "The spell failed.\n\r", ch );return;}



    }







    one_argument( argument, arg );







    if (strlen(page->chpoweroff) > 0 && str_cmp(page->chpoweroff,"(null)"))



	{strcpy(c_m,page->chpoweroff);cast_message = TRUE;}



    if (strlen(page->victpoweron) > 0 && str_cmp(page->victpoweron,"(null)"))



	{strcpy(c_1,page->victpoweron);message_one = TRUE;}



    if (strlen(page->victpoweroff) > 0 && str_cmp(page->victpoweroff,"(null)"))



	{strcpy(c_2,page->victpoweroff);message_two = TRUE;}







    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;



    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;



    if (IS_SET(page->spectype, ADV_OBJECT_TARGET)) object_target = TRUE;



    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;



    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;



    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;



    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;



    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;



    if (IS_SET(page->spectype, ADV_REVERSED     )) 



	{is_reversed = TRUE;bonuses = 0 - bonuses;}







    if (victim_target && !area_affect && !global_target && !object_target)



    {



	if ( ( victim = get_char_room( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);erui;return;
}



	if (victim->in_room == NULL)



    	{send_to_char("The spell failed.\n\r",ch);erui;return;
}



    }



    else if (victim_target && area_affect && !global_target && !object_target)



    {



	if ( ( victim = get_char_world( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	if (victim->in_room == NULL ||



	    victim->in_room->area != ch->in_room->area)



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (victim_target && global_target && !object_target)



    {



	if ( ( victim = get_char_world( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	if (victim->in_room == NULL)



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (object_target && !area_affect && !global_target && !victim_target)



    {



	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (object_target && area_affect && !global_target && !victim_target)



    {



	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (object_target && global_target && !victim_target)



    {



	if ( ( obj = get_obj_world( ch, arg ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	if (obj->in_room == NULL)



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }







    if (page->toughness < PURPLE_MAGIC || page->toughness > WHITE_MAGIC)



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    if (page->toughness == PURPLE_MAGIC)



    {



	if ( ( sn = skill_lookup( "purple sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }


    else if (page->toughness == BLACK_MAGIC)



    {



	if ( ( sn = skill_lookup( "black sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }
    else if (page->toughness == WHITE_MAGIC)



    {



	if ( ( sn = skill_lookup( "white magic" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (page->toughness == RED_MAGIC)



    {



	if ( ( sn = skill_lookup( "red sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (page->toughness == BLUE_MAGIC)



    {



	if ( ( sn = skill_lookup( "blue sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (page->toughness == GREEN_MAGIC)



    {



	if ( ( sn = skill_lookup( "green sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else if (page->toughness == YELLOW_MAGIC)



    {



	if ( ( sn = skill_lookup( "yellow sorcery" ) ) < 0 )



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    }



    else {send_to_char("The spell failed.\n\r",ch);	erui
return;}




if (victim_target && ch != victim && IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You emerge from your cloak of magic.\n\r", ch );
        act("$n slowly emerges from a cloak of magic.",ch,NULL,NULL,TO_ROOM);
    }

    DROW_MAGIC_RET(ch,victim);


    if (!victim_target && victim != ch)



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    if (not_caster && ch == victim)



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



    else if (no_players && !IS_NPC(victim))



	{send_to_char("The spell failed.\n\r",ch);	erui
return;}







    if ( is_affected( victim, sn ) && ch->trust < 7)



    {



	send_to_char("They are already affected by a spell of that colour.\n\r",ch);



	return;



    }







    if ( apply_bit == 0 )



    {



	enhance_stat(sn,level,ch,victim,APPLY_NONE,bonuses,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_STR) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_STR,bonuses * 0.1,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_DEX) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_DEX,bonuses * 0.1,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_INT) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_INT,bonuses * 0.1,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_WIS) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_WIS,bonuses * 0.1,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_CON) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_CON,bonuses * 0.1,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_MANA) )



    {

	enhance_stat(sn,level,ch,victim,APPLY_MANA,bonuses * 5,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_HIT) )



    {


	enhance_stat(sn,level,ch,victim,APPLY_HIT,bonuses * 5,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_MOVE) )



    {


	enhance_stat(sn,level,ch,victim,APPLY_MOVE,bonuses * 5,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_AC) )



    {



if (IS_CLASS(victim,CLASS_MAGE)|| bonuses<0)

	enhance_stat(sn,level,ch,victim,APPLY_AC,0 - (bonuses * 5),affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_HITROLL) )



    {



if (IS_CLASS(victim,CLASS_MAGE)|| bonuses<0)

	enhance_stat(sn,level,ch,victim,APPLY_HITROLL,bonuses*.60,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_DAMROLL) )



    {


if (IS_CLASS(victim,CLASS_MAGE)|| bonuses<0)


	enhance_stat(sn,level,ch,victim,APPLY_DAMROLL,bonuses*.60,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if ( IS_SET(apply_bit, ADV_SAVING_SPELL) )



    {



	enhance_stat(sn,level,ch,victim,APPLY_SAVING_SPELL,bonuses*0.3,affect_bit);



	affect_bit = 0;



	any_affects = TRUE;



    }



    if (!any_affects) {send_to_char("The spell failed.\n\r",ch);	erui
return;}







    if (cast_message) act2(c_m,ch,NULL,victim,TO_CHAR);



    if (message_one)  act2(c_1,ch,NULL,victim,TO_VICT);



    if (message_two)  act2(c_2,ch,NULL,victim,TO_NOTVICT);



    if (!IS_IMMORTAL(ch))



    {



    	WAIT_STATE(ch,PULSE_VIOLENCE);



    	ch->mana -= mana_cost;



    }







    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	!IS_SET(page->spectype, ADV_SPELL_FIRST))



    {



    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\r",ch);	erui
return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,argument);




	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )



	{send_to_char( "The spell failed.\n\r", ch );	erui
return;}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\r", ch );	erui
return;}



    	}



	else {send_to_char( "The spell failed.\n\r", ch );	erui
return;}



    }



    return;



}







void adv_spell_action( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)



{



    char arg1 [MAX_STRING_LENGTH];



    char arg2 [MAX_STRING_LENGTH];



    char c_m [MAX_INPUT_LENGTH];



    char c_1 [MAX_INPUT_LENGTH];



    char c_2 [MAX_INPUT_LENGTH];



    OBJ_DATA *page_next;



    OBJ_DATA *obj = NULL;



    OBJ_DATA *obj2 = NULL;



    CHAR_DATA *victim = NULL;



    CHAR_DATA *victim2 = NULL;



    ROOM_INDEX_DATA *old_room = ch->in_room;



    char next_par [MAX_INPUT_LENGTH];



    int mana_cost = page->points;



    int action_bit = page->value[1];



    int action_type = page->value[2];



    bool area_affect = FALSE;



    bool victim_target = FALSE;



    bool object_target = FALSE;



    bool second_victim = FALSE;



    bool second_object = FALSE;



    bool global_target = FALSE;



    bool next_page = FALSE;



    bool parameter = FALSE;



    bool not_caster = FALSE;



    bool no_players = FALSE;



    bool cast_message = FALSE;



    bool message_one = FALSE;



    bool message_two = FALSE;



    bool is_reversed = FALSE;

if (IS_SET(page->spectype, ADV_PARAMETER)) mana_cost /=3;

mana_cost /= 2;


    if (++chain > 10)
    {
	stc("You may only chain together 10 spells.\n\rThe spell failed.\n\r", ch);
	return;
    }


    if (ch->mana < mana_cost)



    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}







    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	IS_SET(page->spectype, ADV_SPELL_FIRST))



    {



    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\rchpowoff=0/act",ch);return;}



	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\rpar=0/act",ch);return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,argument);


	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )



	{send_to_char( "The spell failed.\n\rv[0]+1/act", ch );return;}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\rcont2?/act", ch );return;}



    	}



	else {send_to_char( "The spell failed.\n\relse/act/?", ch );return;}



    }







    argument = one_argument( argument, arg1 );



    argument = one_argument( argument, arg2 );







    if (strlen(page->chpoweroff) > 0 && str_cmp(page->chpoweroff,"(null)"))



	{strcpy(c_m,page->chpoweroff);cast_message = TRUE;}



    if (strlen(page->victpoweron) > 0 && str_cmp(page->victpoweron,"(null)"))



	{strcpy(c_1,page->victpoweron);message_one = TRUE;}



    if (strlen(page->victpoweroff) > 0 && str_cmp(page->victpoweroff,"(null)"))



	{strcpy(c_2,page->victpoweroff);message_two = TRUE;}







    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;



    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;



    if (IS_SET(page->spectype, ADV_OBJECT_TARGET)) object_target = TRUE;



    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;



    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;



    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;



    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;



    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;



    if (IS_SET(page->spectype, ADV_SECOND_VICTIM)) second_victim = TRUE;



    if (IS_SET(page->spectype, ADV_SECOND_OBJECT)) second_object = TRUE;



    if (IS_SET(page->spectype, ADV_REVERSED     )) is_reversed   = TRUE;







    if (victim_target && !area_affect && !global_target && !object_target)



    {



	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\rgchr=0/act",ch);return;}



	if (victim->in_room == NULL)



    	{send_to_char("The spell failed.\n\rchin=0/act",ch);return;}



	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rimm_sum/act",ch);return;}



	if (victim == ch)



    	{send_to_char("The spell failed.\n\rvict=ch/act",ch);return;}



    }



    else if (victim_target && area_affect && !global_target && !object_target)



    {



	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\rvict=0/act",ch);return;}



	if (victim->in_room == NULL ||



	    victim->in_room->area != ch->in_room->area)



    	{send_to_char("The spell failed.\n\rarea!=area/act",ch);return;}



	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rimm_sum/act",ch);return;}

	if (IS_NPC(victim) && victim->level > ch->spl[0]*1.5)
	{
	stc("Spell failed. The mob is too high level.\n\r", ch);
	return;
	}


	if (victim == ch)



    	{send_to_char("The spell failed.\n\rvict=ch/act2",ch);return;}



    }



    else if (victim_target && global_target && !object_target)



    {



	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\rvict=0/act2",ch);return;}



	if (victim->in_room == NULL)



    	{send_to_char("The spell failed.\n\rv->rm=0/act",ch);return;}



 	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rimm_sum/act3",ch);return;}

	if (IS_NPC(victim) && victim->level > ch->spl[0]*1.5)
	{
	stc("Spell failed. The mob is too high level.\n\r", ch);
	return;
	}



	if (victim == ch)



    	{send_to_char("The spell failed.\n\rvict=ch/act3",ch);return;}



    }



    else if (object_target && !area_affect && !global_target && !victim_target)



    {



	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj_car=0/act",ch);return;}



    }



    else if (object_target && area_affect && !global_target && !victim_target)



    {



	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj_here=0/act",ch);return;}



    }



    else if (object_target && global_target && !victim_target)



    {



	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj_world=0/act",ch);return;}



	if (obj->in_room == NULL)



    	{send_to_char("The spell failed.\n\robj_in=0/act",ch);return;}



    }







    if ( arg2[0] == '\0' && (second_victim || second_object) )



    {



	send_to_char("Please specify a target.\n\r",ch);



	return;



    }



    else if (second_victim && victim_target && !area_affect && !global_target && !object_target)



    {



	if ( ( victim2 = get_char_room( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\rv2_rm=0/act",ch);return;}



	if (victim2->in_room == NULL)



    	{send_to_char("The spell failed.\n\rv2->in=0/act",ch);return;}



	if (victim == NULL || IS_NPC(victim) || IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rv2,imm_sum/act",ch);return;}



	if (!IS_NPC(victim2) && !IS_IMMUNE(victim2, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rv2,imm_sum/act2",ch);return;}



	if (victim == victim2)



    	{send_to_char("The spell failed.\n\rv=v2/act",ch);return;}



    }



    else if (second_victim && victim_target && area_affect && !global_target && !object_target)



    {



	if ( ( victim2 = get_char_world( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\rv2_world=0/act",ch);return;}



	if (victim2->in_room == NULL ||



	    victim2->in_room->area != ch->in_room->area)



    	{send_to_char("The spell failed.\n\rv2-are!=are/act",ch);return;}



	if (victim == NULL || IS_NPC(victim) || IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rv_imm_sum/act",ch);return;}



	if (!IS_NPC(victim2) && !IS_IMMUNE(victim2, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\rv_2_imm_sum/act",ch);return;}



	if (victim == victim2)



    	{send_to_char("The spell failed.\n\rv1=v2/act3",ch);return;}



    }



    else if (second_victim && victim_target && global_target && !object_target)



    {



	if ( ( victim2 = get_char_world( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (victim2->in_room == NULL)



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (victim == NULL || IS_NPC(victim) || IS_IMMUNE(victim, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (!IS_NPC(victim2) && !IS_IMMUNE(victim2, IMM_SUMMON))



    	{send_to_char("The spell failed.\n\r",ch);return;}



	if (victim == victim2)



    	{send_to_char("The spell failed.\n\r",ch);return;}



    }



    else if (second_object && object_target && !area_affect && !global_target && !victim_target)



    {



	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj2,car=0/act",ch);return;}



    }



    else if (second_object && object_target && area_affect && !global_target && !victim_target)



    {



	if ( ( obj2 = get_obj_here( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj2,here=0/act",ch);return;}



    }



    else if (second_object && object_target && global_target && !victim_target)



    {



	if ( ( obj2 = get_obj_world( ch, arg2 ) ) == NULL )



    	{send_to_char("The spell failed.\n\robj2,world=0/act",ch);return;}



    }







    if (victim != NULL)



    {



	if (victim->level > ch->spl[PURPLE_MAGIC])



	{



	    send_to_char("The spell failed.\n\rpurple magic level too low",ch);



	    return;



	}



    }







    if (victim2 != NULL)



    {



	if (victim2->level > ch->spl[PURPLE_MAGIC])



	{



	    send_to_char("The spell failed.\n\rpurple magic level too low",ch);



	    return;



	}



    }







    switch ( action_bit )



    {



    default:



	send_to_char("The spell failed.\n\ract_bit=?",ch);



	return;



    case ACTION_MOVE:



	if (!victim_target && !second_victim && !object_target && !second_object)



	{



	    if (cast_message) act2(c_m,ch,NULL,NULL,TO_CHAR);



	    if (message_one) act2(c_1,ch,NULL,NULL,TO_ROOM);



	    char_from_room(ch);



	    char_to_room(ch,old_room);



	    if (message_two) act2(c_2,ch,NULL,NULL,TO_ROOM);



	}



	else if ( arg1[0] == '\0' )



	{



	    send_to_char("Please specify a target.\n\r",ch);



	    return;



	}



	else if (victim_target && !second_victim && !object_target && !second_object)



	{



	    if (victim == NULL || victim->in_room == NULL ||



		victim->in_room == ch->in_room)



	    {send_to_char("The spell failed.\n\rnull,in_room/act",ch);return;}



	    if (is_reversed)



	    {



		if (victim->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\rv-post=fighting/act",ch);return;}



		if (cast_message) act2(c_m,victim,NULL,NULL,TO_CHAR);



		if (message_one) act2(c_1,victim,NULL,NULL,TO_ROOM);



		char_from_room(victim);



		char_to_room(victim,ch->in_room);



		if (message_two) act2(c_2,victim,NULL,NULL,TO_ROOM);



		do_look(victim,"");



	    }



	    else



	    {



		if (ch->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\rc-post=fighting/0",ch);return;}



		if (cast_message) act2(c_m,ch,NULL,NULL,TO_CHAR);



		if (message_one) act2(c_1,ch,NULL,NULL,TO_ROOM);



		char_from_room(ch);



		char_to_room(ch,victim->in_room);



		if (message_two) act2(c_2,ch,NULL,NULL,TO_ROOM);



		do_look(ch,"");



	    }



	}



	else if (!victim_target && !second_victim && object_target && !second_object)



	{



	    if (obj == NULL || obj->in_room == NULL || obj->in_room == ch->in_room)
	    {
			send_to_char("The spell failed.\n\ralready here or no room/act",ch);
			return;
		}


		if (obj->pIndexData->vnum >= 29500 && obj->pIndexData->vnum <= 29521)
		{
			send_to_char("You are not permitted to create artifacts.\n\r", ch);
			return;
		}

		if (obj->pIndexData->vnum == 30040)
		{
			send_to_char("You are not permitted to create the quest machine.\n\r", ch);
			return;
		}

	    if (cast_message) act2(c_m,ch,obj,NULL,TO_CHAR);



	    if (message_one) act2(c_1,ch,obj,NULL,TO_ROOM);



	    if (is_reversed)



	    {



		obj_from_room(obj);



		obj_to_room(obj,ch->in_room);



	    }



	    else



	    {



		char_from_room(ch);



		char_to_room(ch,obj->in_room);



		do_look(ch,"");



	    }



	    if (message_two) act2(c_2,ch,obj,NULL,TO_ROOM);



	}



	else if (victim_target && second_victim && !object_target && !second_object)



	{



	    if (victim == NULL || victim->in_room == NULL)



	    {send_to_char("The spell failed.\n\r2nd_1/act",ch);return;}



	    if (victim2 == NULL || victim2->in_room == NULL ||



		victim2->in_room == victim->in_room)



	    {send_to_char("The spell failed.\n\r2nd_2/act",ch);return;}



	    if (is_reversed)



	    {



		if (victim2->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\r2nd_3/act",ch);return;}



		if (cast_message) act2(c_m,victim2,NULL,victim,TO_CHAR);



		if (message_one) act2(c_1,victim2,NULL,victim,TO_ROOM);



		char_from_room(victim2);



		char_to_room(victim2,victim->in_room);



		if (message_two) act2(c_2,victim2,NULL,victim,TO_ROOM);



		do_look(victim2,"");



	    }



	    else



	    {



		if (victim->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\r2nd_4/act",ch);return;}



		if (cast_message) act2(c_m,victim,NULL,victim2,TO_CHAR);



		if (message_one) act2(c_1,victim,NULL,victim2,TO_ROOM);



		char_from_room(victim);



		char_to_room(victim,victim2->in_room);



		if (message_two) act2(c_2,victim,NULL,victim2,TO_ROOM);



		do_look(victim,"");



	    }



	}



	else if (victim_target && !second_victim && !object_target && second_object)



	{



	    if (victim == NULL || victim->in_room == NULL)



	    {send_to_char("The spell failed.\n\rnul_in_v/act",ch);return;}



	    if (obj2 == NULL || obj2->in_room == NULL ||



		obj2->in_room == victim->in_room)



	    {send_to_char("The spell failed.\n\ralreadyhereobj2vict/act",ch);return;}



		if (obj->pIndexData->vnum >= 29500 && obj->pIndexData->vnum <= 29521)
		{
			send_to_char("You are not permitted to create artifacts.\n\r", ch);
			return;
		}
		if (obj->pIndexData->vnum == 30040)
		{
			send_to_char("You are not permitted to create the quest machine.\n\r", ch);
			return;
		}

	    if (cast_message) act2(c_m,victim,NULL,NULL,TO_CHAR);



	    if (message_one) act2(c_1,victim,obj2,NULL,TO_ROOM);



	    if (is_reversed)



	    {



		obj_from_room(obj2);



		obj_to_room(obj2,victim->in_room);



	    }



	    else



	    {



		if (victim->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\rv=pos_fight/act",ch);return;}



		char_from_room(victim);



		char_to_room(victim,obj2->in_room);



		do_look(victim,"");



	    }



	    if (message_two) act2(c_2,victim,obj2,NULL,TO_ROOM);



	}



	else if (!victim_target && !second_victim && object_target && second_object)



	{



	    if (obj == NULL || obj->in_room == NULL)



	    {send_to_char("The spell failed.\n\robj=0 or in_rm=0/act",ch);return;}


		if (obj->pIndexData->vnum >= 29500 && obj->pIndexData->vnum <= 29521)
		{
			send_to_char("You are not permitted to create artifacts.\n\r", ch);
			return;
		}
		if (obj->pIndexData->vnum == 30040)
		{
			send_to_char("You are not permitted to create the quest machine.\n\r", ch);
			return;
		}

	    if (obj2 == NULL || obj2->in_room == NULL ||



		obj2->in_room == obj->in_room)



	    {send_to_char("The spell failed.\n\robj2,rm=obj,rm",ch);return;}



	    if (cast_message) act2(c_m,ch,obj,NULL,TO_CHAR);



	    if (is_reversed)



	    {



		old_room = ch->in_room;



		if (message_one) act2(c_1,ch,obj2,NULL,TO_ROOM);



		obj_from_room(obj2);



		obj_to_room(obj2,obj->in_room);



		char_from_room(ch);



		char_to_room(ch,obj->in_room);



		if (message_two) act2(c_2,ch,obj2,NULL,TO_ROOM);



		char_from_room(ch);



		char_to_room(ch,old_room);



	    }



	    else



	    {



		old_room = ch->in_room;



		if (message_one) act2(c_1,ch,obj,NULL,TO_ROOM);



		obj_from_room(obj);



		obj_to_room(obj,obj2->in_room);



		char_from_room(ch);



		char_to_room(ch,obj2->in_room);



		if (message_two) act2(c_2,ch,obj,NULL,TO_ROOM);



		char_from_room(ch);



		char_to_room(ch,old_room);



	    }



	}



	else if (!victim_target && second_victim && object_target && !second_object)



	{



	    if (victim2 == NULL || victim2->in_room == NULL)



	    {send_to_char("The spell failed.\n\rv2 or v2,rm=0/act",ch);return;}



	    if (obj == NULL || obj->in_room == NULL ||



		obj->in_room == victim2->in_room)



	    {send_to_char("The spell failed.\n\robj,rm=v2,rm/act",ch);return;}


		if (obj->pIndexData->vnum >= 29500 && obj->pIndexData->vnum <= 29521)
		{
			send_to_char("You are not permitted to create artifacts.\n\r", ch);
			return;
		}
		if (obj->pIndexData->vnum == 30040)
		{
			send_to_char("You are not permitted to create the quest machine.\n\r", ch);
			return;
		}

	    if (cast_message) act2(c_m,victim2,NULL,NULL,TO_CHAR);



	    if (message_one) act2(c_1,victim,obj2,NULL,TO_ROOM);



	    if (is_reversed)



	    {



		if (victim2->position == POS_FIGHTING)



		{send_to_char("The spell failed.\n\rv2,pos=fight/act",ch);return;}



		char_from_room(victim2);



		char_to_room(victim2,obj->in_room);



		do_look(victim2,"");



	    }



	    else



	    {



		obj_from_room(obj);



		obj_to_room(obj,victim2->in_room);



	    }



	    if (message_two) act2(c_2,victim2,obj,NULL,TO_ROOM);



	}



	else {send_to_char("The spell failed.\n\rv,oth,fail/act",ch);return;}



	break;



    case ACTION_MOB:



	if ( action_type < 1)



	{send_to_char("The spell failed.\n\ract<1/actmob",ch);return;}



	if (IS_NPC(ch) || ch->pcdata->followers > 4)



	{send_to_char("The spell failed.\n\rfollowers>4/act",ch);return;}



	if (( victim = create_mobile( get_mob_index( action_type ))) == NULL )



	{send_to_char("The spell failed.\n\rmob[vnum]=0/act",ch);return;}



	if (cast_message) act2(c_m,ch,NULL,victim,TO_CHAR);



	if (message_one) act2(c_1,ch,NULL,victim,TO_ROOM);



	ch->pcdata->followers++;



	char_to_room( victim, ch->in_room );



	SET_BIT(victim->act, ACT_NOEXP);



	free_string(victim->lord);



	victim->lord = str_dup(ch->name);



	if (victim->level > ch->spl[PURPLE_MAGIC])



	{



	    send_to_char("The spell failed.\n\rpurpletoolow/actmob",ch);



	    extract_char(victim, TRUE);



	    return;



	}



	break;



    case ACTION_OBJECT:



	if ( action_type < 1)



	{send_to_char("The spell failed.\n\ract_type<1/act",ch);return;}



	if (( obj = create_object( get_obj_index( action_type ),0 )) == NULL )



	{send_to_char("The spell failed.\n\rcreate_obj=0/act",ch);return;}



	if (cast_message) act2(c_m,ch,obj,NULL,TO_CHAR);


		if (obj->pIndexData->vnum >= 29500 && obj->pIndexData->vnum <= 29521)
		{
			send_to_char("You are not permitted to create artifacts.\n\r", ch);
			return;
		}
		if (obj->pIndexData->vnum == 30040)
		{
			send_to_char("You are not permitted to create the quest machine.\n\r", ch);
			return;
		}

	if (message_one) act2(c_1,ch,obj,NULL,TO_ROOM);



	free_string(obj->questmaker);



	obj->questmaker = str_dup(ch->name);



	obj_to_room( obj, ch->in_room );



	break;



    }



    if (!IS_IMMORTAL( ch ))



    {



    	WAIT_STATE(ch,PULSE_VIOLENCE);



    	ch->mana -= mana_cost;



    }







    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&



	!IS_SET(page->spectype, ADV_SPELL_FIRST))



    {


/*
    	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))



    	{send_to_char("The spell failed.\n\rchpoweroff<2,adv/act",ch);return;}

*/

	if (IS_SET(page->spectype, ADV_PARAMETER))



	{



	    if (!str_cmp(page->chpoweron,"(null)"))



		{send_to_char("The spell failed.\n\rparam=0/act",ch);return;}



	    else strcpy(next_par,page->chpoweron);



	}



	else strcpy(next_par,argument);

	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )


	{send_to_char( "The spell failed.\n\rv[0]+1/act", ch );return;}



    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))



    	{



	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))



	    	adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, next_par);



	    else if (IS_SET(page_next->spectype, ADV_AFFECT))



	    	adv_spell_affect(ch,book,page_next,next_par);



	    else if (IS_SET(page_next->spectype, ADV_ACTION))



	    	adv_spell_action(ch,book,page_next,next_par);



	    else {send_to_char( "The spell failed.\n\rnextpage,spectype=?", ch );return;}



    	}



	else {send_to_char( "The spell failed.\n\ractwhole_do=?", ch );return;}



    }



    return;
}

    void do_magearmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
        send_to_char("What?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Please specify which piece of mage armor you wish to make: Cape Mask
Girth.\n\r",ch);
    return;
    }

    if ( ch->practice < 80 )
    {
        send_to_char("It costs 80 points of primal to create mage equipment.\n\r",ch);
        return;
    }

    if      (!str_cmp(arg,"mask"     )) vnum = 29699;
    else if (!str_cmp(arg,"girth"   )) vnum = 29700;
    else if (!str_cmp(arg,"cape"    )) vnum = 29701;
    else
    {
    send_to_char("Please specify which piece of mage armor you wish to make: Cape Mask Girth.\n\r",ch);
    return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
    }
    ch->practice -= 80;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_undeadcall(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
	int value;
	argument = one_argument( argument, arg );

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->extra, EXTRA_NECRO))
	{
		send_to_char("Only Necromancers can do this!\n", ch);
		return;
	}

	if (ch->fight_timer > 0)
	{
		send_to_char("You may not do this whilst you are tainted from a fight.\n\r", ch);
		return;
	}

	value = atoi(arg);
	
	if (value < 1)
	{
		send_to_char("You need to use at least 1 primal!\n", ch);
		return;
	}

	if (value > 60)
	{
	    stc("The necromancers aren't learned enough to use more than 60 primal on this.\n\r", ch);
	    return;
	}

	if (ch->practice < value)
	{
		send_to_char("You do not have enough primal!\n",ch);
		return;
	}
	
    victim=create_mobile( get_mob_index( MOB_VNUM_MOUNT ) );
    victim->level = value * 25;
    victim->armor = 0 - (50*value);
    victim->hitroll = value*30;
    victim->damroll = value*30;
    victim->hit = 200*value;
    victim->max_hit = 200*value;
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    SET_BIT(victim->affected_by,AFF_FLYING);
    SET_BIT(victim->act, ACT_NOEXP);
    free_string(victim->name);
	victim->name = str_dup("skeleton undead dead");
	sprintf(buf,"%s's skeleton",ch->name);
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("A large undead skeleton stands here.\n\r");
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
	send_to_char("You summon the undead to rise and do your bidding!\n\r",ch);
	act("$n summons the undead to rise and do thier bidding!",ch,NULL,NULL,TO_ROOM);
	ch->practice -= value;
	WAIT_STATE(ch, 5);
	do_group(ch,"undead");
	return;
}

void do_airflesh(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = NULL;

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->extra, EXTRA_NECRO))
	{
		send_to_char("Only Necromancers can do this!\n", ch);
		return;
	}

	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You may only do this in a fight.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Please specify a target.\n",ch);
		return;
	}

 	if ((victim = get_char_room(ch, argument)) == NULL)
    {
            send_to_char("They aren't here.\n\r", ch);
            return;
    }
	if (victim->hit > 4000)
	{
		send_to_char("Your victim must have at most 4k hp!\n",ch);
		return;
	}
	
	send_to_char("You make arcane motions and the air lunges and devours your enemy!\n\r",ch);
	act("$n makes arcane motions and the air lunges and devours thier enemy!",ch,NULL,NULL,TO_ROOM);
	ch->practice -= 100;
nodamcap = TRUE;
damage(ch,victim,number_range(400000,500000),skill_lookup("magic missile"));
nodamcap = FALSE;
	WAIT_STATE(ch, 5);
	return;
}

void do_drainlife(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = NULL;

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->extra, EXTRA_NECRO))
	{
		send_to_char("Only Necromancers can do this!\n", ch);
		return;
	}

	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You may only do this in a fight.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Please specify a target.\n",ch);
		return;
	}

 	if ((victim = get_char_room(ch, argument)) == NULL)
    {
            send_to_char("They aren't here.\n\r", ch);
            return;
    }

	if (ch->mana < 2000)
	{
		send_to_char("You do not have enough mana!\n",ch);
		return;
	}
	
	if (victim->hit < 1000)
	{
		send_to_char("Your victim must have at most 1000 hp!\n",ch);
		return;
	}
	
	send_to_char("You sketch ancient runes in the air and devour your enemies lifeforce!\n\r",ch);
	act("$n sketches ancient runes in the air and devours thier enemies lifeforce!",ch,NULL,NULL,TO_ROOM);
	ch->mana -= 2000;
	ch->hit += 1500;
	if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
	victim->hit -= 2000;
	WAIT_STATE(ch, 12);
	return;
}

void do_ddoor(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->extra, EXTRA_ILLUS))
	{
		send_to_char("Only Illusionists can do this!\n", ch);
		return;
	}

	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You may only do this in a fight.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Please specify a target.\n",ch);
		return;
	}

 	if ((victim = get_char_room(ch, argument)) == NULL)
    {
            send_to_char("They aren't here.\n\r", ch);
            return;
    }

	if (victim == ch)
	{
	send_to_char("Not yourself!\n",ch);
	return;
	}

	if (ch->mana < 500)
	{
		send_to_char("You do not have enough mana!\n",ch);
		return;
	}
	
	if (ch->practice < 4)
	{
		send_to_char("You need at least 4 primal to use this!\n",ch);
		return;
	}
	
	act("You open a dimensional door and push $n through it!",victim,NULL,NULL,TO_CHAR);
	act("$n opens a dimensional door and pushes $n through it!",ch,victim,NULL,TO_ROOM);
	ch->mana -= 500;
	ch->practice -= 4;
	stop_fighting( victim, TRUE );
    char_from_room(victim);
    char_to_room(victim, get_random_room(victim));
    stc("You fall out from a dimensional door, which closes behind you.\n\r",victim);
    act("$n bursts out from a dimensional door, which closes behind him.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,"auto");
	WAIT_STATE(ch, 7);
	return;
}

void do_fade(CHAR_DATA *ch, char *argument)
{

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->extra, EXTRA_ILLUS))
	{
		send_to_char("Only Illusionists can do this!\n", ch);
		return;
	}

	if (ch->mana < 3000)
	{
		send_to_char("You do not have enough mana!\n",ch);
		return;
	}

/*	if (ch->fight_timer > 0)
	{
	send_to_char("Not with a fight timer!\n\r",ch);
	return;
	}*/
	
	if (ch->practice < 2)
	{
		send_to_char("You need at least 2 primal to use this!\n",ch);
		return;
	}
	
    if ( IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You emerge from your cloak of magic.\n\r", ch );
        act("$n slowly emerges from a cloak of magic.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
        if (has_timer(ch)) return;

        send_to_char( "You slowly fade into a cloak of magic.\n\r", ch );
        act("$n slowly fades into a cloak of magic..",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->act, PLR_SACREDINVIS);
    }	
    ch->mana -= 3000;
	ch->practice -= 2;
	WAIT_STATE(ch, 3);
	return;
}

void do_pacify(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = NULL;
	one_argument (argument, arg);

	if (IS_NPC(ch))
		return;

	if (!IS_CLASS(ch, CLASS_MAGE))
	{
		send_to_char("You are not wizened in the ways of the mage.\n\r", ch);
		return;
	}
	
    if (!IS_SET(ch->newbits, NEW_INVK))
	{
		send_to_char("Only Invokers can do this!\n", ch);
		return;
	}

	if (ch->mana < 2500)
	{
		send_to_char("You do not have enough mana!\n",ch);
		return;
	}
	
	if (ch->practice < 30)
	{
		send_to_char("You need at least 30 primal to use this!\n",ch);
		return;
	}
	
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting to use this power!\n\r",ch);
		return;
	}

	 if (argument[0] == '\0')
        {
                send_to_char("Please specify a target.\n",ch);
                return;
        }

        if ((victim = get_char_room(ch, argument)) == NULL)
    {
            send_to_char("They aren't here.\n\r", ch);
            return;
    }    

        ch->mana -= 2500;
	ch->practice -= 30;
	WAIT_STATE(ch, 3);
	stop_fighting(victim, TRUE);
	victim->position = POS_RESTING;
	act("$n sits down and rests, willing to talk peacefully.",victim,NULL,NULL,TO_ROOM);
	return;
}

void adv_spell_stack( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)
{
 int manacost = 0;
 OBJ_DATA *obj_targ = NULL;
 OBJ_DATA *page_next;
 CHAR_DATA *char_targ = NULL;
 void *vo = NULL;
 int spell_type = 0;
 SPELL_FUN *spell = NULL;
 int sn = 0;
 int level = 0;
 char full[MSL];
 char arg1[MSL];
 char arg2[MSL];

 if (++stack_count > 4)
 {
     stc("Failed. You may only have 4 none-true magic spells stacked.\n\r", ch);
     return;
 }

 if (strlen(argument) >= MSL)
 {
     stc("Failed.\n\r", ch);
     return;
 }
    if (++chain > 10)
    {
	stc("You may only chain together 10 spells.\n\rThe spell failed.\n\r", ch);
	return;
    }


 strcpy(full, argument);

 if ((sn = skill_lookup(page->questowner)) <= 0)
 {
     stc("Failed.\n\r", ch);
erui     return;
 }
 if ((spell = skill_table[sn].spell_fun) == NULL)
 {
     stc("Failed.\n\r", ch);
erui     return;
 }

 if ((spell_type = skill_table[sn].target) >= 5 || spell_type < 0)
 {
     stc("Failed.\n\r", ch);
erui     return;
 }

if (spell_type == TAR_IGNORE) target_name = argument;
else {
 target_name = one_argument(argument, arg1);
 strcpy(arg2, target_name);}

 manacost = skill_table[sn].min_mana * 3;

 if (spell_type == TAR_CHAR_OFFENSIVE)
	manacost *= 5.3;

 if (skill_table[sn].skill_level > ch->level)
 {
erui    stc("Failed.\n\r", ch);
    return;
 }

 if (ch->mana < manacost)
 {
     stc("You don't have enough mana to continue that spell!\n\r", ch);
     return;
 }

 if (spell_type == TAR_CHAR_OFFENSIVE || spell_type == TAR_CHAR_DEFENSIVE)
 {
     if ((char_targ = get_char_room(ch, arg1)) == NULL)
     {
         stc("Failed. Target not in room.\n\r", ch);
         return;
     }
     vo = (void *) char_targ;
     if (spell_type == TAR_CHAR_OFFENSIVE
	&& ch != char_targ && IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You emerge from your cloak of magic.\n\r", ch );
        act("$n slowly emerges from a cloak of magic.",ch,NULL,NULL,TO_ROOM);
    }

 }
 else if (spell_type == TAR_CHAR_SELF)
     vo = (void *) ch;
 else if (spell_type == TAR_OBJ_INV)
 {
     if ((obj_targ = get_obj_carry(ch, arg1)) == NULL)
     {
         stc("Failed. You aren't carrying that object.\n\r", ch);
         return;
     }
     vo = (void *) obj_targ;
 }
 else if (spell_type == TAR_IGNORE)
     vo = NULL;
 else
 {
erui     stc("Failed.\n\r", ch);
     return;
 }

 if (!vo && spell_type != TAR_IGNORE)
 {
erui     stc("Failed.\n\r", ch);
     return;
 }
if (spell_type != TAR_CHAR_OFFENSIVE && spell_type != TAR_IGNORE) {
 if (page->toughness < PURPLE_MAGIC || page->toughness > WHITE_MAGIC)
 {
erui     send_to_char("The spell failed.\n\r",ch);
     return;
 }

 if (page->toughness == PURPLE_MAGIC)
 {
     if ( ( sn = skill_lookup( "purple sorcery" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }



 }
 else if (page->toughness == RED_MAGIC)
 {
     if ( ( sn = skill_lookup( "red sorcery" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }
 }
 else if (page->toughness == BLACK_MAGIC)
 {
     if ( ( sn = skill_lookup( "black sorcery" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }
 }
 else if (page->toughness == WHITE_MAGIC)
 {
     if ( ( sn = skill_lookup( "white magic" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }
 }
 else if (page->toughness == BLUE_MAGIC)
 {
     if ( ( sn = skill_lookup( "blue sorcery" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }
 }
 else if (page->toughness == GREEN_MAGIC)
 {
     if ( ( sn = skill_lookup( "green sorcery" ) ) < 0 )
     {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
     }
 }
 else if (page->toughness == YELLOW_MAGIC)
 {
    if ( ( sn = skill_lookup( "yellow sorcery" ) ) < 0 )
    {
erui        send_to_char("The spell failed.\n\r",ch);
        return;
    }
 }
 else
 {
erui     send_to_char("The spell failed.\n\r",ch);
     return;
 }
 if (char_targ && is_affected( char_targ, sn ) )
 {
     send_to_char("They are already affected by a spell of that colour.\n\r",ch);
     return;
 }

 if (char_targ)
 {
     DROW_MAGIC_RET(ch,char_targ);
    if (number_percent() <= char_targ->atm)
    {
	act( "Your spell is unable to penetrate $n's antimagic shield.", ch, NULL, char_targ, TO_CHAR );
	act( "$n's spell is unable to penetrate your antimagic shield.", ch, NULL, char_targ, TO_VICT );
	return;
    }
 }
}
 level = ch->spl[skill_table[sn].target] * 0.32;

ch->mana -= manacost;

 (spell)(sn, level, ch, vo);

WAIT_STATE(ch, PULSE_VIOLENCE);
 if (IS_SET(page->spectype, ADV_NEXT_PAGE)) {
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{erui send_to_char( "The spell failed.\n\r", ch );return;}
    	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
    	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
	    	adv_spell_damage(ch,book,page_next,full);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
	    	adv_spell_affect(ch,book,page_next,full);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
	    	adv_spell_action(ch,book,page_next,full);
	    else if (IS_SET(page_next->spectype, ADV_STACK))
		adv_spell_stack(ch, book, page_next, full);
	    else {send_to_char( "The spell failed.\n\r", ch );erui return;}
    	}
	else {erui send_to_char( "The spell failed.\n\r", ch );return;
}
}
WAIT_STATE(ch, PULSE_VIOLENCE);

 return;

 
}

