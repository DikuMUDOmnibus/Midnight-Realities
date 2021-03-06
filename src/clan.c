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

/*
 * Local functions.
 */

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	birth_write		args( ( CHAR_DATA *ch, char *argument ) );
bool	birth_ok		args( ( CHAR_DATA *ch, char *argument ) );

/* Need to get rid of those flames somehow - KaVir */


void do_devoursoul(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim; 
	
    argument = one_argument( argument, arg );
    
    if ( !IS_CLASS(ch, CLASS_DEMON))
		return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Whose soul do you wish to devour?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
	if (ch->generation < 10)
	{
		send_to_char("You are not a Cornugon to class.\n\r", ch);
		return;
	}
    if (!IS_NEWFLAG(victim, AFF_ALLOW_DEMON))
    {
	stc("They have not released their soul, yet.\n\r",ch);
	return;
    }

    if ( victim->class > 0 || IS_NPC(victim))
    {
	send_to_char("You cannot wrench their soul from their deity.\n\r",ch);
	send_to_char("You feel a tugging at your soul.\n\r",victim);
	return;
    }

    if (strlen(ch->clan) < 5)
    {
	stc("You are not able to create new demons until you are in a clan.\n\r", ch);
	return;
    }

    victim->class = CLASS_DEMON;
    victim->generation = 1;

    set_learnable_disciplines(victim);

    ch->pcdata->souls++;
    if (ch->pcdata->souls > souls_needed(ch) )
	ch->pcdata->souls = souls_needed(ch);
    set_form(victim,  0);
    victim->pcdata->perm_int -= 4;
    victim->alignment = -666;
    victim->hit = 1;
    victim->move = 1;
    victim->clan = str_dup(ch->clan);
    save_char_obj(victim);
    save_char_obj(ch);
    /*sprintf(buf, "%s's soul has been devoured by %s.", victim->name, ch->name);
    do_info(ch,buf);*/

    /* Now demons actually have forms.  Kooky. */
    /*set_form(victim, FRM_LEMURE);*/
    look_name(victim);
    ch->hit = ch->max_hit;

    WAIT_STATE(ch, 10);
    WAIT_STATE(victim, 30);

    send_to_char("Plunging your hand deep within your victims chest, you rip out their\n\r",ch);
    send_to_char("soul, leaving a lifeless husk motionless at your feet. You have been\n\r",ch);
    send_to_char("rewarded by your diety by enrolling another follower.\n\r",ch);

    act("$N's vast demonic form towers over you.  With a mighty roar, $E
plunges $S\n\rscaly claw into your chest.  Blood sprays everywhere, and
you hear the eldritch\n\rmocking cry of conquest as your mortal
conciousness fades forever.", victim, NULL, ch, TO_CHAR);
    send_to_char("The demonic transformation *****TOTALLY DESTROYS***** you.\n\r",victim);
    send_to_char("That really did HURT!\n\r",victim);
    send_to_char("You have been KILLED!\n\r",victim);

    act("With a deafening roar, the great demon $N plunges $S claw into
$n's chest and rips out $s soul.  $n drops to the ground, lifeless.",
victim, NULL, ch, TO_NOTVICT);
}      

/* Roll Dirt Command added by Listy */
void do_rolldirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    inroom=ch->in_room;
    
    if (!IS_AFFECTED(ch, AFF_FLAMING))
    {
	send_to_char( "You aren't on fire\n\r", ch );
	return;
    }

    if ( number_percent() > 45 )
    {
	send_to_char("You roll in the dirt trying to put out the flames.\n\r", ch);
	act( "$n rolls in the dirt trying to stop the burning flames",  ch, NULL, NULL, TO_ROOM );
	return;
    }

	send_to_char("You manage to roll off the flames around you.\n\r", ch);
	act( "$n manages to roll of the flames around them",  ch, NULL, NULL, TO_ROOM );
    REMOVE_BIT(ch->affected_by, AFF_FLAMING);
    do_humanity(ch,"");
    return;
}
/* End of Roll Dirt Command */

void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    if (IS_NPC( ch )) return;
    inroom=ch->in_room;
    if ( arg[0] == '\0' && !IS_SET(inroom->room_flags,ROOM_FLAMING))
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot smother yourself.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    if ( number_percent() > 45 )
    {
	act( "You try to smother the flames around $N but fail!",  ch, NULL, victim, TO_CHAR    );
	act( "$n tries to smother the flames around you but fails!", ch, NULL, victim, TO_VICT    );
	act( "$n tries to smother the flames around $N but fails!",  ch, NULL, victim, TO_NOTVICT );
	if ( number_percent() > 98 && !IS_AFFECTED(ch,AFF_FLAMING) )
	{
	    act( "A spark of flame from $N's body sets you on fire!",  ch, NULL, victim, TO_CHAR    );
	    act( "A spark of flame from your body sets $n on fire!", ch, NULL, victim, TO_VICT    );
	    act( "A spark of flame from $N's body sets $n on fire!",  ch, NULL, victim, TO_NOTVICT );
	    SET_BIT(ch->affected_by, AFF_FLAMING);
	    do_humanity(ch,"");
	}
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
    do_humanity(ch,"");
    return;
}

/* Loads of Vampire powers follow.  KaVir */
void do_coil( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_SERP] < 8)
    {
      send_to_char("You require level 8 Serpentis to use Body Coil.\n\r", ch);
      return;
    }
    if (!IS_SET(ch->newbits, NEW_COIL))
    {
     send_to_char("You prepare to coil your victims.\n\r", ch);
     SET_BIT(ch->newbits, NEW_COIL);
     return;
    }
    else if (IS_SET(ch->newbits, NEW_COIL))
    {
     send_to_char("You stand down from your coil posture.\n\r", ch);
     REMOVE_BIT(ch->newbits, NEW_COIL);
     return;
    }
 return;
}

void do_tide( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_THAU] < 10)
    {
      send_to_char("You require level 10 Thaumaturgy to use Tide of Vitae.\n\r", ch);
      return;
    }
    if (ch->practice < 20)
    {
     send_to_char("You require at least 20 primal to use Tide of Vitae.\n\r",ch);
     return;
    }
    if (IS_SET(ch->newbits, NEW_TIDE))
    {send_to_char("The tide is already with you.\n\r",ch); return;}
    SET_BIT(ch->newbits, NEW_TIDE);
    ch->practice -= 20;
    send_to_char("You feel a tide of vitae rush over you.\n\r",ch);
 
 
}

void do_flash( CHAR_DATA *ch, char *argument )
{
   char       buf[MAX_STRING_LENGTH];
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 9)
    {
      send_to_char("You require level 9 Quietus to use Flashing
Speed.\n\r", ch);
      return;
    }
    if (IS_EXTRA(ch, EXTRA_FLASH))
    {
     send_to_char("You are already moving swiftly.\n\r", ch);
     return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 200)
    {
     send_to_char("You have insufficient blood.\n\r",ch);
     return;
    }
    SET_BIT(ch->extra, EXTRA_FLASH);
    ch->power[DISC_VAMP_CELE] += 1;
    ch->pcdata->condition[COND_THIRST] -= 200;
    sprintf(buf, "%s starts moving with lightening speed.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("You start moving with lightening speed.\n\r",ch);
    return;
}
void do_death( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *inroom;
    inroom= ch->in_room;
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 5)
    {
      send_to_char("You require level 5 Quietus to use Silence of Death.\n\r", ch);
      return;
    }
    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     sprintf(buf,"The silence leaves the room.\n\r");
     act(buf,ch,NULL,NULL,TO_ROOM);
     send_to_char("The silence leaves the room.\n\r", ch);
     SET_RTIMER(ch->in_room, RTIMER_SILENCE, 0);
     return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 300)
    {
     send_to_char("You have insufficient blood.\n\r", ch);
     return;
    }
    ch->pcdata->condition[COND_THIRST] -= 300;
    SET_RTIMER(inroom, RTIMER_SILENCE, 10);
    sprintf(buf,"A look of concentration passes over %s's face.\n\r", ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    sprintf(buf,"An eerie silence fills the room.\n\r");
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
    return;
}

void do_acid( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_VICI] < 9)
    {
      send_to_char("You require level 9 Vicissitude to use Acid Blood.\n\r", ch);
      return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 500)
    {
     send_to_char("You have insufficient blood.\n\r", ch);
     return;
    }
    if (IS_SET(ch->act, PLR_ACID))
    {
     send_to_char("Your blood is already acid.\n\r",ch);
     return;
    }
    SET_BIT(ch->act, PLR_ACID);
    send_to_char("Your blood turns to a potent acid.\n\r", ch);
    return;
}

void do_retrn( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
 
    act( "$n raises $s arms to the sky.", ch, NULL, NULL, TO_ROOM );
    act( "Your raise your arms to the sky.", ch, NULL, NULL, TO_CHAR );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    location = get_room_index(27000);
    if (ch->power[DISC_VAMP_OBFU] < 8)
    {
     send_to_char("You must obtain at least level 8 in Obuscate to Return to Caine.\n\r", ch);
     return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 250)
    {
	send_to_char("You have insufficient blood.\n\r", ch);
        return;
    }
    if (ch->practice < 20)
    {
	send_to_char("You need 20 primal to use Return to Caine.\n\r", ch);
	return;
    }
    if ( ch->in_room == location )
	return;
    ch->pcdata->condition[COND_THIRST] -= 250;
    ch->practice -= 20;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    return;
}

void do_forget( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1[MAX_INPUT_LENGTH];
    char      arg2[MAX_INPUT_LENGTH];
 
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_DOMI] < 8)
    {
        send_to_char("You must obtain at least level 8 in Dominate to use Forgetful Mind.\n\r",ch);
	return;
    }
 
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Force whom to forget what disc?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("Why use Forgetful Mind on a mob?\n\r", ch);
        return;
    }
    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char("Only useful on vamps?\n\r", ch);
        return;
    }
 
 
    if ( IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only use Forgetful Mind on Avatar's or lower.\n\r", ch );
	return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 250)
    {
    send_to_char("You have insufficient blood.\n\r", ch);
    return;
    }
 
    if (ch->practice < 25)
    {
    send_to_char("You need 25 primal to use Forgetful Mind.\n\r", ch);
    return;
    }
 
    act("You close your eyes and concentrate on $N.", ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.", ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and cencentrates on $N.", ch,NULL,victim,TO_NOTVICT);
    ch->pcdata->condition[COND_THIRST] -= 250;
    ch->practice -= 25;
    WAIT_STATE(ch, 12);
    
 
    if ( number_percent() > 10)
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
 
    if ( !str_cmp(arg2,"animalism") )
    {
    victim->power[DISC_VAMP_ANIM] -= 1;
    act("Your mind becomes hazy about Animalism.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"celerity") )
    {
    victim->power[DISC_VAMP_CELE] -= 1;
    act("Your mind becomes hazy about Celerity.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"dominate") )
    {
    victim->power[DISC_VAMP_DOMI] -= 1;
    act("Your mind becomes hazy about Dominate.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"fortitude") )
    {
    victim->power[DISC_VAMP_FORT] -= 1;
    act("Your mind becomes hazy about Fortitude.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"auspex") )
    {
    victim->power[DISC_VAMP_AUSP] -= 1;
    act("Your mind becomes hazy about Auspex.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"obfuscate") )
    {
    victim->power[DISC_VAMP_OBFU] -= 1;
    act("Your mind becomes hazy about Obfuscate.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"obtenebration") )
    {
    victim->power[DISC_VAMP_OBTE] -= 1;
    act("Your mind becomes hazy about Obtenebration.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"presence") )
    {
    victim->power[DISC_VAMP_PRES] -= 1;
    act("Your mind becomes hazy about Presence.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"potence") )
    {
    victim->power[DISC_VAMP_POTE] -= 1;
    act("Your mind becomes hazy about Potence.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"protean") )
    {
    victim->power[DISC_VAMP_PROT] -= 1;
    act("Your mind becomes hazy about Protean.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"quietus") )
    {
    victim->power[DISC_VAMP_QUIE] -= 1;
    act("Your mind becomes hazy about Quietus.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"serpentis") )
    {
    victim->power[DISC_VAMP_SERP] -= 1;
    act("Your mind becomes hazy about Serpentis.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"thaumaturgy") )
    {
    victim->power[DISC_VAMP_THAU] -= 1;
    act("Your mind becomes hazy about Thaumaturgy.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
    if ( !str_cmp(arg2,"vicissitude") )
    {
    victim->power[DISC_VAMP_VICI] -= 1;
    act("Your mind becomes hazy about Vicissitude.", victim,NULL,NULL,TO_CHAR);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM);
    return;
    }
 
return;
 
}
 
 

void do_rot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_VICI] < 6)
    {
        send_to_char("You must obtain at least level 6 in Vicissitude to use Flesh Rot.\n\r",ch);
	return;
    }
 
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Flesh Rot whom?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim))
    {
        send_to_char(" Why use Flesh Rot on a mob?\n\r", ch);
        return;
    }
    if ( IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only Flesh Rot Avatar's or lower.\n\r", ch );
	return;
    }

/*    if (IS_AFFECTED(AFF_SHADOWPLANE)
{
stc(" Not in Shadowplane.\n\r", ch);
return;
}*/
    if (victim->hit < victim->max_hit)
    {
      send_to_char("They are hurt and suspicious.\n\r", ch);
      return;
    }
    act("You close your eyes and concentrate on $N.", ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.", ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and cencentrates on $N.", ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE(ch, 12);
    if (number_percent( ) < 50)
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
 
    act("Your flesh begins to rot!", victim,NULL,NULL,TO_CHAR);
    act("$n's flesh begins to rot!", victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->extra, EXTRA_ROT);
    return;
}
 

void do_dragon (CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
        int dam;
 
        if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
         send_to_char("Huh?\n\r", ch);
         return;
        }
        if (ch->power[DISC_VAMP_VICI] < 8)
        {
         send_to_char("You must have at least level 8 Vicissitude to use Dragon Breath.\n\r",ch);
         return;
        }
        if ((victim = ch->fighting) == NULL)
	{
	send_to_char("You aren't fighting anyone.\n\r", ch);
	return;
	}
        if (!IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You must be in Dragonform to use Dragonbreath.\n\r",ch);
        return;
        }
        if (ch->pcdata->condition[COND_THIRST] < 6)
        {
         send_to_char("You have insufficient blood.\n\r", ch);
         return;
        }
        WAIT_STATE(ch, 12);
        if (!IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 40;
        }
        if (IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 200;
        }
        ch->pcdata->condition[COND_THIRST] -= 6;
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_VAMPIRE))
        {
         dam *= 1.5;
        }
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_WEREWOLF))
        {
         if (ch->power[DISC_WERE_BOAR] > 2) dam *= 0.8;
        }
 
        if (dam <= 0)
          dam = 1;
sprintf(buf,"Your flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's flame fries you! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

        victim->hit -= dam;
     if(!IS_AFFECTED(victim,AFF_FLAMING)) SET_BIT(victim->affected_by,AFF_FLAMING);
 
       return;
}         

void do_satan( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_DAIM] < 10)
    {
      send_to_char("You require level 10 Daimoinon to use Call of Satan.\n\r", ch);
      return;
    }
 
    if (ch->pcdata->followers > 0)
    {
	send_to_char("Only one Satan can exist in this world.\n\r",ch);
	return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 750)
    {
        send_to_char("You have insufficient blood.\n\r", ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] = ch->pcdata->condition[COND_THIRST] - 750;
    ch->pcdata->followers++;
 
    victim=create_mobile( get_mob_index( MOB_VNUM_SATAN ) );
    victim->level = 800;
    victim->hit = 30000;
    victim->max_hit = 30000;
    victim->hitroll = 1500;
    victim->damroll = 1500;
    victim->armor = 6000;
 
    strcpy(buf,"Lord of all Evil, Join me to crush those who oppose us!");
    do_say( ch, buf );
 
    send_to_char( "You hear a loud tremor and Satan appears before you.\n\r",ch );
    act( "The ground roars and $N bursts appears before $n.", ch, NULL, victim, TO_ROOM );
 
    char_to_room( victim, ch->in_room );
 
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    SET_BIT(victim->act, ACT_NOEXP);
    return;

}
 

void do_awe    ( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
	  send_to_char("Huh?\n\r", ch);
          return;
        }
        if (ch->power[DISC_VAMP_PRES] < 1)
	{
	  send_to_char("You must obtain at least level 1 in Presence to you Awe.\n\r", ch);
	  return;
	}
        if (IS_EXTRA(ch, EXTRA_AWE))
	{
	  REMOVE_BIT(ch->extra, EXTRA_AWE);
	  send_to_char("You are no longer Awe Inspiring.\n\r", ch);
	  act("$n is no longer awe inspiring.", ch, NULL, NULL, TO_ROOM);
	  return;
	}
        SET_BIT(ch->extra, EXTRA_AWE);
	send_to_char("You are now Awe Inspiring.\n\r", ch);
	act("$n is now awe inspiring", ch, NULL, NULL, TO_ROOM);
        return;


}
void do_potency( CHAR_DATA *ch, char *argument )
{

	if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
	  send_to_char("Huh?\n\r", ch);
	  return;
        }
        if (ch->power[DISC_VAMP_THAU] < 3)
	{ 
	  send_to_char("You must obtain at least level 3 in Thaumaturgy to use Blood Potency.\n\r", ch);
          return;
        }
        if (IS_EXTRA(ch, EXTRA_POTENCY))
        {
	  send_to_char("Your blood is already potent.\n\r", ch);
          return;
	}
	if (ch->pcdata->condition[COND_THIRST] < 300)
	{
	  send_to_char("You have insufficient blood.\n\r", ch);
	  return;
	}
/*
        if (ch->generation == 3)
        {
	  send_to_char("Your blood is as potent as it can get.\n\r", ch);
	  return;
	}	
*/

        ch->pcdata->condition[COND_THIRST] = ch->pcdata->condition[COND_THIRST] - 300;
//	ch->generation = ch->generation - 1;
        SET_BIT(ch->extra, EXTRA_POTENCY);
        send_to_char("You feel your blood become more potent.\n\r", ch);
        return;

}


void do_plasma( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
        if ( IS_NPC(ch) )
		return;
     if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
	send_to_char("Huh?\n\r", ch);
	return;
    }
	if (ch->level < 3)
	{
		send_to_char("You cannot do this whilst mortal.\n\r", ch);
		return;
	}
    if (ch->power[DISC_VAMP_VICI] < 5)
    { 
      send_to_char("You must obtain at least level 5 in Vicissitude to use Plasma Form.\n\r", ch);
	       return;    
	}
	save_char_obj(ch);
	if ((obj = create_object(get_obj_index(30007),60)) == NULL)
    {
        send_to_char( "Error - Please inform Maser.\n\r", ch);	
		return;   
	}   
	if ( IS_EXTRA(ch,EXTRA_PLASMA) )    
	{      
		ch->pcdata->obj_vnum = 0;     
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		REMOVE_BIT(ch->extra, EXTRA_OSWITCH);  
		REMOVE_BIT(ch->extra, EXTRA_PLASMA);   
		ch->pcdata->chobj = NULL;    
		obj->chobj = NULL;   
		free_string(ch->morph);  
		ch->morph = str_dup("");     
		act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
		act("Your reform your human body.",ch,obj,NULL,TO_CHAR);  
		extract_obj( obj );   
		return;                
    }
	if (ch->fight_timer > 0)
	{	
		send_to_char("You cannot use plasma with your fight timer active.\n\r", ch);
		return;	
	}        
	if ( IS_AFFECTED(ch, AFF_POLYMORPH) )   
	{	
		send_to_char( "Not while polymorphed.\n\r", ch );
		return;  
	}    
	act("$n transforms into $p and splashes to the ground.",ch,obj,NULL,TO_ROOM); 
	act("You transform into $p and splashes to the ground.",ch,obj,NULL,TO_CHAR);   
	ch->pcdata->obj_vnum = ch->pcdata->powers[VPOWER_OBJ_VNUM];
	obj->chobj = ch;   
	ch->pcdata->chobj = obj; 
	SET_BIT(ch->affected_by, AFF_POLYMORPH);    
	SET_BIT(ch->extra, EXTRA_OSWITCH);
	SET_BIT(ch->extra, EXTRA_PLASMA);  
	free_string(ch->morph);    
	ch->morph = str_dup("a pool of blood");
	obj_to_room(obj,ch->in_room);  
	return;
}

void do_taste( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char      buf [MAX_STRING_LENGTH];
    char      age [MAX_STRING_LENGTH];
    char      lin [MAX_STRING_LENGTH];
    char     clan [MAX_STRING_LENGTH];
    char     lord [MAX_STRING_LENGTH];
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument (argument, arg );

    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_THAU] < 1)
    {
        send_to_char("You must obtain at least level 1 in Thaumaturgy to use Taste of Blood.\n\r",ch);
	return;
    }
 
       if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }

    if (IS_NPC(victim))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }                               

sprintf(buf,"You examine $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n examines your blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n examines $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_NOTVICT);
 

send_to_char("\n\r",ch);
send_to_char("\n\r",victim);


if (victim->pcdata->rank == AGE_ANCILLA)         sprintf( age, "Ancilla");
else if (victim->pcdata->rank == AGE_CHILDE)     sprintf( age, "Childe");
else if (victim->pcdata->rank == AGE_NEONATE)    sprintf( age, "Neonate");
else if (victim->pcdata->rank == AGE_ELDER)      sprintf( age, "Elder");
else if (victim->pcdata->rank == AGE_METHUSELAH) sprintf( age, "Methuselah");
if (victim->clan == NULL) sprintf(clan, "None");
    else sprintf(clan, "%s",victim->clan);
if (victim->lord == NULL) sprintf(lord, "None");
    else sprintf(lord, "%s",victim->lord);
    sprintf( lin,
"---------------------------------------------------------------------------\n\r");
send_to_char( lin,ch);
send_to_char(
"                              Vampire Status\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Generation:%d  Bloodpool:%d  Age:%s  Clan:%s  Lord:%s\n\r",
victim->generation,victim->pcdata->condition[COND_THIRST],
age,clan,lord);
send_to_char(buf,ch);
send_to_char(lin,ch);
send_to_char(
"                              Disciplines\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]             Fortitude: [%d]\n\r",
victim->power[DISC_VAMP_ANIM],
victim->power[DISC_VAMP_CELE],
victim->power[DISC_VAMP_FORT]);
send_to_char(buf,ch);
sprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]             Quietus:   [%d]\n\r",
victim->power[DISC_VAMP_OBTE],
victim->power[DISC_VAMP_PRES],
victim->power[DISC_VAMP_QUIE]);
send_to_char(buf,ch);
sprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]             Dominate:  [%d]\n\r",
victim->power[DISC_VAMP_THAU],
victim->power[DISC_VAMP_AUSP],
victim->power[DISC_VAMP_DOMI]);
send_to_char(buf,ch);
sprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]             Protean:   [%d]\n\r",
victim->power[DISC_VAMP_OBFU],
victim->power[DISC_VAMP_POTE],
victim->power[DISC_VAMP_PROT]);
send_to_char(buf,ch);
sprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]             Daimoinon: [%d]\n\r",
victim->power[DISC_VAMP_SERP],
victim->power[DISC_VAMP_VICI],
victim->power[DISC_VAMP_DAIM]);
send_to_char(buf,ch);
send_to_char(lin,ch);
 
return;
}
 
 




void do_shadowstep( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->power[DISC_VAMP_OBTE] < 6)
	{
           send_to_char("You must obtain at least level 7 in Obtenebration to use this power.\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Shadowstep to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
/* 
    if (!IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim)) 
    {
    send_to_char("He doesnt want you near him!\n\r",ch);
    return;
    }
*/

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (IS_IMMORTAL(victim))
    {
	send_to_char("Not to immortals!\r\n", ch);
	return;
    }

    if(ch->fight_timer > 0)
    {
	send_to_char("Not with a fight timer!\n\r",ch);
	return;
    }

    if(ch->max_move < victim->max_move)
    if(!IS_NPC(victim))
    {
	send_to_char("Get more move!\n\r",ch);
	return;
    }

/*i know it fits in but its kinda stupid..
    if (IS_NPC(victim) && (!room_is_dark(victim->in_room)))
    {
	send_to_char("You cant find a shadow in his room.\n\r",ch);
	return;
    }
*/
    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You step into the shadows and appear before $N.", ch, NULL, victim, TO_CHAR);
    act("$n appears out of the shadows before $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears from the shadows in front of you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}

void do_earthmeld( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_VAMP_PROT] < 4 )
    {
        send_to_char("You must obtain at least level 4 Protean to use Earthmeld.\n\r",ch);
	return;
    }
    

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

	if (has_timer(ch)) return;

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_EARTHMELD) )
    {
            REMOVE_BIT(ch->affected_by, AFF_SHIFT);
            REMOVE_BIT(ch->extra, EXTRA_EARTHMELD);
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
           if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
            free_string(ch->morph);
            ch->morph = str_dup("");
            ch->level = 3;
            ch->trust = 3; 
           send_to_char( "You rise up from the ground.\n\r", ch );
            sprintf(buf, "%s rises up from the ground",ch->name);
            act(buf,ch,NULL,NULL,TO_ROOM);

            return;

    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    SET_BIT(ch->affected_by, AFF_SHIFT);
    SET_BIT(ch->extra, EXTRA_EARTHMELD);
    SET_BIT(ch->act, PLR_WIZINVIS);
    free_string(ch->morph);
    ch->morph = str_dup("Someone");
    ch->level = 4;
    ch->trust = 4;
    send_to_char( "You sink into the ground.\n\r", ch );
    sprintf(buf, "%s sinks into the ground.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    return;
}
/*
void do_burrow( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_WERE_BOAR] < 5 )
    {
        send_to_char("You must obtain at least level 5 boar to use burrow.\n\r",ch);
	return;
    }
    


    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_EARTHMELD) )
    {
            REMOVE_BIT(ch->affected_by, AFF_SHIFT);
            REMOVE_BIT(ch->extra, EXTRA_EARTHMELD);
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
           if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
            free_string(ch->morph);
            ch->morph = str_dup("");
            ch->level = 3;
            ch->trust = 3; 
           send_to_char( "You rise up from your burrow.\n\r", ch );
            sprintf(buf, "%s rises up from a hole",ch->name);
            act(buf,ch,NULL,NULL,TO_ROOM);

            return;

    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    SET_BIT(ch->affected_by, AFF_SHIFT);
    SET_BIT(ch->extra, EXTRA_EARTHMELD);
    SET_BIT(ch->act, PLR_WIZINVIS);
    free_string(ch->morph);
    ch->morph = str_dup("Someone");
    ch->level = 4;
    ch->trust = 4;
    send_to_char( "You burrow into the ground.\n\r", ch );
    sprintf(buf, "%s burrow into the ground.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    return;
}
*/

void do_burrow( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
 {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( ch->power[DISC_WERE_BOAR] < 5 )
    {
    send_to_char("You need the Boar totem to 5!\n\r", ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
    send_to_char("Burrow to who?\n\r", ch);
    return;
    }
if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }        

    if (str_cmp(victim->clan,ch->clan) )
	{
		send_to_char("You can only burrow to members of your own clan.\n\r", ch);
		return;
	}
	if ( victim == ch )
    {
        send_to_char( "You cannot burrow to yourself.\n\r", ch);
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

	if ( victim->level != LEVEL_AVATAR || !IS_CLASS(victim, CLASS_WEREWOLF))
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    if (victim->in_room == NULL)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    send_to_char("You burrow into the ground.\n\r",ch);
    act("$n burrows into the ground.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
	do_look(ch,"");
    send_to_char("You burrow out of the ground.\n\r",ch);
    act("$n burrows out of the ground.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_serenity( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
    if(!IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("huh?.\n\r",ch);
        return;
    }
    if (ch->power[DISC_VAMP_ANIM] < 2)
    {
        send_to_char("You must obtain at least level 2 in Animalism to use Song of Serenity.\n\r",ch);
	return;
    }



    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast < 1)
    {
	if ( ch->rage < 1 )
	{
	    send_to_char("Your beast doesn't control your actions.\n\r",ch);
	    return;
	}
        send_to_char("You chant a soft tune and pacify your inner beast.\n\r",ch);
        act("$n chants a soft tune and pacifies their inner beast.",ch,NULL,NULL,TO_ROOM);
	ch->rage = 0;
	if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
	if (IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	if (IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    else 
    {
     send_to_char("Only those who achieve Golconda can sing the Song of Serenity.\n\r",ch);
     return;
    }
}

void do_theft( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int bloodpool;
    argument = one_argument( argument, arg );

    if (ch->generation <= 0)
        ch->generation = 4;
    WAIT_STATE(ch, 6);
    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    } 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->power[DISC_VAMP_THAU] < 4)
    {
        send_to_char("You must obtain at least level 4 in Thaumaturgy to use Theft of Vitae.\n\r",ch);
	return;
    }


    if ( arg[0] == '\0' )
    {
        send_to_char( "Steal blood from whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
/*
    if ( IS_NPC(victim) ) 
    {
        send_to_char( "Lower life forms are immune to Theft of Vitae.\n\r", ch);
        return;
    }
*/
    if (!IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] <= 0)
    {
        send_to_char( "There isn't enough blood to steal.\n\r", ch);
        return;
    }
    if (!IS_NPC(victim) && IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only steal blood from Avatar's or lower.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;
        sprintf(buf,"A stream of blood shoots from %s into your body.",victim->name);
	act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"A stream of blood shoots from %s into %s.",victim->name,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
        sprintf(buf,"A stream of blood shoots from your body into %s.",ch->name);
        act(buf,ch,NULL,victim,TO_VICT);
        
        ch->pcdata->condition[COND_THIRST] += number_range(30,40);
if (victim->level >1000){stc("Its too high level!! OH NO!!\n\r",ch);return;}
        if (IS_NPC(victim))
        {
	    victim->hit /= 2;
	    act("$n doesn't take kindly to that.", ch, 0, 0, TO_CHAR);
	    multi_hit(victim,ch,TYPE_HIT);
             if (ch->pcdata->condition[COND_THIRST] >= bloodpool / ch->generation)
    {
       ch->pcdata->condition[COND_THIRST] = bloodpool / ch->generation;
    }

         return;
        }
        if (IS_SET(victim->act, PLR_ACID))
        {
        send_to_char("Their blood is a deadly acid!\n\r", ch);
        ch->hit -= 500;
        victim->pcdata->condition[COND_THIRST] -= 30;
        return;
        }

    if (!IS_NPC(victim)) 
{
victim->pcdata->condition[COND_THIRST] -=number_range(30,40);
}
    if (ch->pcdata->condition[COND_THIRST] >= bloodpool / ch->generation)
    {
       ch->pcdata->condition[COND_THIRST] = bloodpool /ch->generation;
    }

   if (victim->pcdata->condition[COND_THIRST] <= 0)
    {
       victim->pcdata->condition[COND_THIRST] = 0; 
    }
   return;
}





void do_zuloform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_VICI] < 4)
    {
        send_to_char("You must obtain at least level 4 in Vicissitude to use Zuloform.\n\r",ch);
	return;
    }

    if (IS_EXTRA(ch,EXTRA_DRAGON))
    {
    send_to_char("You cannot unzulo while a dragon.\n\r",ch);
    return;
    }
    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
		send_to_char("You cant zulo when changed.\n\r",ch);
		return;
	}
  
  
    if ( ch->pcdata->condition[COND_THIRST] < 200 )
    {
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    }

        if (ch->stance[0] != -1) do_stance(ch,"");
		if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(100,200);
        act( "You transform into large beast.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body grows and distorts into a large beast.", ch, NULL, NULL, TO_ROOM );
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
		sprintf(buf, "%s : A big black monster",ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }
  



void do_shiftww( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int  toform = 0;

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
	return;
    }
    if      ( !str_cmp(arg,"homid" ) ) toform = 1;
    else if ( !str_cmp(arg,"glabro") ) toform = 2;
    else if ( !str_cmp(arg,"crinos") ) toform = 3;
    else if ( !str_cmp(arg,"hispo" ) ) toform = 4;
    else if ( !str_cmp(arg,"lupus" ) ) toform = 5;
    else
    {
	send_to_char( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
	return;
    }
    if ( ch->pcdata->stats[UNI_FORM0] < 1 || ch->pcdata->stats[UNI_FORM0] > 5) 
	ch->pcdata->stats[UNI_FORM0] = 1;
    if ( ch->pcdata->stats[UNI_FORM0] == toform )
    {
	send_to_char( "You are already in that form.\n\r", ch );
	return;
    }
    ch->pcdata->stats[UNI_FORM1] = toform;
    return;
}


void horn args((CHAR_DATA *ch));

/* claw and fang commands */

void declaw(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_CLAWS)) return;

    send_to_char("You retract your claws.\n\r",ch);
    act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
}

void dehorn(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_HORNS)) return;

    send_to_char("Your horns burrow back into your skull.\n\r",ch);
    act("$n's horns retract into $s skull.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void do_horns(CHAR_DATA *ch, char *argument)
{
   if (IS_CLASS(ch, CLASS_DEMON))
   {
	if (ch->power[DISC_DAEM_ATTA] < 4)
        {
	  stc("Huh?\n\r",ch);
          return;
	}
   }
   else
   {
	stc("Huh?\n\r",ch);
	return;
   }

    WAIT_STATE(ch, 8);
    if (IS_VAMPAFF(ch, VAM_HORNS)) dehorn(ch);
    else horn(ch);
}

void horn(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
  
    if (IS_VAMPAFF(ch, VAM_HORNS)) return;

    /* Only certain demons may use their horns */
    if (ch->power[DISC_DAEM_ATTA] < 4) return;

    send_to_char("Sharp horns grow out of your skull.\n\r",ch);
    act("Sharp horns grow out of $n's skull.",ch, NULL, NULL, TO_ROOM);

    if ( (obj = get_eq_char(ch, WEAR_HEAD)) != NULL) 
    { 
        act("Your horns shred $p.", ch, obj, NULL, TO_CHAR); 
        act("$n's horns shred $p.", ch, obj, NULL, TO_ROOM); 
        extract_obj(obj); 
    } 

    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void defang(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_FANGS)) return;

    send_to_char("Your fangs slide back into your gums.\n\r",ch);
    act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
}

void do_claws(CHAR_DATA *ch, char *argument)
{
    if (ch->cur_form != 0 && !form_data[ch->cur_form].can_use_claws )
    {
	send_to_char("You cannot use your claws in this form.\n\r", ch);
	return;
    }
   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 1)
      {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_DEMON))
   {
	if (ch->power[DISC_DAEM_ATTA] < 1)
        {
	  stc("Huh?\n\r",ch);
          return;
	}
   }
   else if (IS_CLASS(ch, CLASS_VAMPIRE))
   {
	if (ch->power[DISC_VAMP_PROT] < 2)
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_NINJA))
   {
	if(ch->pcdata->powers[NPOWER_NINGENNO] < 1)
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_CLAWS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your claws while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("You retract your claws.\n\r",ch);
	act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
   else
   {
	send_to_char("Wicked claws extend from your fingers.\n\r",ch);
	act("Long sharp claws extend from $n's fingers.",ch, NULL, NULL, TO_ROOM);
	remove_obj( ch, WEAR_WIELD, TRUE );
	remove_obj( ch, WEAR_HOLD, TRUE );
	remove_obj( ch, WEAR_THIRD, TRUE );
	remove_obj( ch, WEAR_FOURTH, TRUE );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
}

void do_fangs(CHAR_DATA *ch, char *argument)
{
    if (ch->cur_form != 0 && !form_data[ch->cur_form].can_use_claws )
    {
	send_to_char("You cannot use your fangs in this form.\n\r", ch);
	return;
    }

   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 2)
        {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }

   else if (IS_CLASS(ch, CLASS_DEMON))
   {
	if (ch->power[DISC_DAEM_ATTA] < 2)
        {
	    stc("You must obtain level two in Attack to use your fangs.\n\r",ch);
	    return;
	}
   }

   else if (!IS_CLASS(ch, CLASS_VAMPIRE))
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_FANGS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your fangs while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("Your fangs slide back into your gums.\n\r",ch);
	act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
   else
   {
	send_to_char("Sharp fangs slide out of your gums.\n\r",ch);
	act("Sharp fangs slide out of $n's gums.",ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
}

void do_nightsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if (ch->power[DISC_WERE_HAWK] < 1)
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }
    else if (IS_GOLEM(ch) && ch->pcdata->powers[SYMANTICS] < 2)
    {
	stc("You need Symantics 2 to use that!\n\r", ch);
	return;
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_GOLEM(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PROT] < 1 &&
       ch->power[DISC_VAMP_OBTE] < 3) 
    {
        send_to_char("You must obtain at least level 1 in Protean or 3 in Obtenebration.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
    {
	send_to_char("Not while your beast is in control.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	return;
    }
    send_to_char("Your eyes start glowing red.\n\r",ch);
    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    return;
}

void do_anarch( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) > 1)
    {
	send_to_char("But you are already in a clan!\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if ( IS_SET(ch->special, SPC_ANARCH) )
    {
	send_to_char("You are no longer an Anarch.\n\r",ch);
	sprintf(buf,"%s is no longer an Anarch!",ch->name);
	do_info(ch,buf);
	REMOVE_BIT(ch->special, SPC_ANARCH);
	return;
    }
    send_to_char("You are now an Anarch.\n\r",ch);
    sprintf(buf,"%s is now an Anarch!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_ANARCH);
    return;
}

void do_inconnu( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
	int ch_age = years_old(ch);
    int cost = EXP(ch, 30000000);

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (ch_age < 50)
	{
		send_to_char("You are not old enough to join the inconnu.\n\r", ch);
		return;
	}
    if (strlen(ch->clan) > 1 && ch->generation < 3)
	{
	send_to_char("You cannot abandon your clan!\n\r",ch);
	return;
    }

    if (ch->pcdata->rank == AGE_ELDER && ch->generation > 2)
    {
	send_to_char("You must be at least an Elder to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if (ch->exp < cost)
    {
	sprintf(log_buf, "It costs %d exp to become an Inconnu.\n\r",cost);
	stc(log_buf, ch);
	return;
    }

    if (IS_SET(ch->special, SPC_ANARCH)) REMOVE_BIT(ch->special, SPC_ANARCH);
    free_string(ch->clan);
    ch->clan = str_dup( "" );
    ch->exp -= cost;
    send_to_char("You are now an Inconnu.\n\r",ch);
    sprintf(buf,"%s is now an Inconnu!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_INCONNU);
    return;
}

void do_shadowsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_HAWK] < 2)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
        if (ch->power[DISC_VAMP_OBTE] < 2  )
	{
            send_to_char("You must obtain at least Level 2 Obtenebration to use Shadowsight.\n\r",ch);
	    return;
	}
    }
    if (IS_AFFECTED(ch,PLR_SHADOWSIGHT) )
    {
	send_to_char("You can no longer see between planes.\n\r",ch);
	REMOVE_BIT(ch->affected_by, PLR_SHADOWSIGHT);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 10)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(5,10);
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, PLR_SHADOWSIGHT);
    return;
}

void do_bite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int       clancount;
   /* bool      can_sire = FALSE;*/
    bool      outcast = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) || (ch->generation < 1))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[UNI_CURRENT] == -1) 
	ch->pcdata->stats[UNI_CURRENT] = ch->pcdata->stats[UNI_AFF]; 

/*
    if (IS_SET(ch->special,SPC_PRINCE)) can_sire = TRUE;
    if (IS_SET(ch->special,SPC_SIRE)) can_sire = TRUE;
    if (ch->generation < 3 || ch->generation > 6) can_sire = FALSE;
*/
/*    if (ch->generation > 3) can_sire = FALSE;
    else can_sire = TRUE;

    if (!can_sire)
    {
	send_to_char("You are not able to create any childer.\n\r",ch);
	return;
    }*/
/*
    if (!str_cmp(ch->clan,"") && ch->generation == 2)
    {
	send_to_char( "First you need to found a clan.\n\r", ch );
	return;
    }
*/
    clancount = 0;
    if (ch->power[DISC_VAMP_PROT] >= 1)       clancount = clancount + 1;
    if (ch->power[DISC_VAMP_CELE] >= 1)      clancount = clancount + 1;
    if (ch->power[DISC_VAMP_FORT] >= 1)     clancount = clancount + 1;
    if (ch->power[DISC_VAMP_POTE] >= 1)       clancount = clancount + 1;
    if (ch->power[DISC_VAMP_OBFU] >= 1)     clancount = clancount + 1;
    if (ch->power[DISC_VAMP_OBTE] >= 1) clancount = clancount + 1;
    if (ch->power[DISC_VAMP_SERP] >= 1)     clancount = clancount + 1;
    if (ch->power[DISC_VAMP_AUSP] >= 1)        clancount = clancount + 1;
    if (ch->power[DISC_VAMP_DOMI] >= 1)      clancount = clancount + 1;
    if (ch->power[DISC_VAMP_PRES] >= 1)      clancount = clancount + 1;
    if (ch->power[DISC_VAMP_VICI] >= 1)   clancount = clancount + 1;
    if (ch->power[DISC_VAMP_THAU] >= 1)          clancount = clancount + 1;
    if (ch->power[DISC_VAMP_ANIM] >= 1)        clancount = clancount + 1;
    if (ch->power[DISC_VAMP_QUIE] >= 1)       clancount = clancount + 1;

    if ( clancount < 4 )
    {
	send_to_char( "First you need to master 4 disciplines.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Bite whom?\n\r", ch );
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
	send_to_char( "Not on Immortal's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot bite yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "You cannot bite mages.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only bite avatars.\n\r", ch );
	return;
    }
    if (IS_CLASS(victim, CLASS_DROW))
{	send_to_char(" Not on a drow!\n\r", ch);
	return;}

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "You are unable to create werevamps!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char( "But they are already a vampire!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "But they have no soul!\n\r", ch );
	return;
    }


    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You cannot turn highlanders into vampires.\n\r", ch );
	return;
    }

    if (!IS_NEWFLAG(victim, AFF_ALLOW_VAMP))
    {
	stc("They are not a willing subject.\n\r",ch);
	return;
    }

    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("First you better get your fangs out!\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char("You must reveal your true nature to bite someone.\n\r",ch);
	return;
    }

    if (ch->exp < 1000)
    {
	send_to_char("You cannot afford the 1000 exp to create a childe.\n\r",ch);
	return;
    }

    if (ch->beast > 0)
	ch->beast += 1;
    ch->exp = ch->exp - 1000;
    act("You sink your teeth into $N.", ch, NULL, victim, TO_CHAR);
    act("$n sinks $s teeth into $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n sinks $s teeth into your neck.", ch, NULL, victim, TO_VICT);
    victim->class=CLASS_VAMPIRE;
    set_learnable_disciplines(victim);
/*
    if ( victim->generation != 0 && (victim->generation <= ( ch->generation + 1 ) ) ) */
    if ( victim->generation != 0 )
    {
	save_char_obj(ch);
	save_char_obj(victim);
	send_to_char( "Your vampiric status has been restored.\n\r", victim );
	return;
    }
    if (IS_SET(ch->special,SPC_SIRE) || ch->generation < 3 || IS_SET(ch->special,SPC_PRINCE))
    {
	if (IS_SET(ch->special,SPC_SIRE))
	    REMOVE_BIT(ch->special,SPC_SIRE);
	if (IS_SET(ch->special, SPC_ANARCH) && strlen(ch->clan) < 2)
	    outcast = TRUE;
    }
    else outcast = TRUE;
    send_to_char( "You are now a vampire.\n\r", victim );    
    victim->generation = ch->generation + 2;
/*
    clan_table_bite(victim);
*/
    free_string(victim->lord);
    if (ch->generation == 1)
	victim->lord=str_dup(ch->name);
    else
    {
	sprintf(buf,"%s %s",ch->lord,ch->name);
	victim->lord=str_dup(buf);
    }
    if (ch->generation != 1)
    {
    	if (victim->pcdata->stats[UNI_CURRENT] == -1) 
		victim->pcdata->stats[UNI_CURRENT] = victim->pcdata->stats[UNI_AFF];

	/* Remove hp bonus from fortitude */
	if (IS_VAMPPASS(victim,VAM_FORTITUDE) && !IS_VAMPAFF(victim,VAM_FORTITUDE))
	{
	    victim->max_hit = victim->max_hit - 50;
	    victim->hit = victim->hit - 50;
	    if (victim->hit < 1) victim->hit = 1;
	}

	/* Remove any old powers they might have */
	if (IS_VAMPPASS(victim,VAM_PROTEAN))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PROTEAN);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PROTEAN);}
	if (IS_VAMPPASS(victim,VAM_CELERITY))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_CELERITY);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CELERITY);}
	if (IS_VAMPPASS(victim,VAM_FORTITUDE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_FORTITUDE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FORTITUDE);}
	if (IS_VAMPPASS(victim,VAM_POTENCE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_POTENCE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_POTENCE);}
	if (IS_VAMPPASS(victim,VAM_OBFUSCATE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBFUSCATE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBFUSCATE);}
	if (IS_VAMPPASS(victim,VAM_OBTENEBRATION))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBTENEBRATION);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBTENEBRATION);}
	if (IS_VAMPPASS(victim,VAM_SERPENTIS))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_SERPENTIS);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SERPENTIS);}
	if (IS_VAMPPASS(victim,VAM_AUSPEX))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_AUSPEX);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_AUSPEX);}
	if (IS_VAMPPASS(victim,VAM_DOMINATE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_DOMINATE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DOMINATE);}
	if (IS_VAMPPASS(victim,VAM_PRESENCE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PRESENCE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PRESENCE);}
        if (IS_VAMPPASS(victim,VAM_VICISSITUDE))
        {REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_VICISSITUDE);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_VICISSITUDE);}
        if (IS_VAMPPASS(victim,VAM_THAU))
        {REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_THAU);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_THAU);}
        if (IS_VAMPPASS(victim,VAM_ANIMAL))
        {REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_ANIMAL);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_ANIMAL);}
        if (IS_VAMPPASS(victim,VAM_QUIETUS))
        {REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_QUIETUS);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_QUIETUS);}



        if (!outcast)
	{
	    free_string(victim->clan);
	    victim->clan=str_dup(ch->clan);
	}
	/* Give the vampire the base powers of their sire */
	if (IS_VAMPPASS(ch,VAM_FORTITUDE) && !IS_VAMPAFF(victim,VAM_FORTITUDE))
	{
	    victim->max_hit = victim->max_hit + 50;
	    victim->hit = victim->hit + 50;
	}
	if (IS_VAMPPASS(ch,VAM_PROTEAN))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PROTEAN);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_PROTEAN);}
	if (IS_VAMPPASS(ch,VAM_CELERITY))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_CELERITY);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_CELERITY);}
	if (IS_VAMPPASS(ch,VAM_FORTITUDE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_FORTITUDE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_FORTITUDE);}
	if (IS_VAMPPASS(ch,VAM_POTENCE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_POTENCE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_POTENCE);}
	if (IS_VAMPPASS(ch,VAM_OBFUSCATE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBFUSCATE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBFUSCATE);}
	if (IS_VAMPPASS(ch,VAM_OBTENEBRATION))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBTENEBRATION);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBTENEBRATION);}
	if (IS_VAMPPASS(ch,VAM_SERPENTIS))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_SERPENTIS);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_SERPENTIS);}
	if (IS_VAMPPASS(ch,VAM_AUSPEX))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_AUSPEX);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_AUSPEX);}
	if (IS_VAMPPASS(ch,VAM_DOMINATE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_DOMINATE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_DOMINATE);}
	if (IS_VAMPPASS(ch,VAM_PRESENCE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PRESENCE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_PRESENCE);}
        if (IS_VAMPPASS(ch,VAM_VICISSITUDE))
        {SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_VICISSITUDE);
         SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_VICISSITUDE);}
        if (IS_VAMPPASS(ch,VAM_THAU))
        {SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_THAU);
         SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_THAU);}
        if (IS_VAMPPASS(ch,VAM_ANIMAL))
        {SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_ANIMAL);
         SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_ANIMAL);}
        if (IS_VAMPPASS(ch,VAM_QUIETUS))
        {SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_QUIETUS);
         SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_QUIETUS);}



    }
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_claw( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    bool      can_sire = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || (ch->generation < 1))
    {
	do_claws(ch,"");
	return;
    }

    if (IS_SET(ch->special,SPC_PRINCE)) can_sire = TRUE;
//    if (IS_SET(ch->special,SPC_SIRE)) can_sire = TRUE;
    else if (ch->generation == 1 || ch->generation == 2) can_sire = TRUE;

//    if (ch->generation > 3) can_sire = FALSE;
    else can_sire = FALSE;

    if (!can_sire)
    {
	send_to_char("You are unable to spread your gift.\n\r",ch);
	return;
    }

/*
    else if (ch->generation != 1 && ch->pcdata->stats[UNI_AFF] >= 2)
    {
	send_to_char("You have already created 2 pups.\n\r",ch);
	return;
    }
*/
    if (strlen(ch->clan) < 2 && ch->generation != 1 )
    {
	send_to_char( "First you need to create a tribe.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Claw whom?\n\r", ch );
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
	send_to_char( "Not on Immortal's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot claw yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "You cannot bite mages.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only claw avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DROW)) {
	send_to_char( "Not on a drow!\n\r", ch );
		return;}

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to create werevamps!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "But they have no soul!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You cannot turn highlanders into werewolves.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "But they are already a werewolf!\n\r", ch );
	return;
    }   
 
    if (victim->class != 0 || IS_SET(victim->special, SPC_CHAMPION))
	{
	send_to_char("Not on classed characters!\n\r", ch);
	return;
	}

    if (!IS_NEWFLAG(victim, AFF_ALLOW_WERE))
    {
	stc("You are unable to infect your victim.\n\r",ch);
	return;
    }

    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("First you better get your claws out!\n\r",ch);
	return;
    }

    if (ch->exp < 10000)
    {
	send_to_char("You cannot afford the 10000 exp to pass on the gift.\n\r",ch);
	return;
    }

    ch->exp = ch->exp - 10000;
    if (ch->pcdata->stats[UNI_CURRENT] < 1) ch->pcdata->stats[UNI_CURRENT] = 1;
    else ch->pcdata->stats[UNI_CURRENT] += 1;
    if (IS_SET(ch->special,SPC_SIRE)) REMOVE_BIT(ch->special,SPC_SIRE);
    act("You plunge your claws into $N.", ch, NULL, victim, TO_CHAR);
    act("$n plunges $s claws into $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n plunges $s claws into your chest.", ch, NULL, victim, TO_VICT);
    victim->class=CLASS_WEREWOLF;
    set_learnable_disciplines(victim);
    send_to_char( "You are now a werewolf.\n\r", victim );
    victim->generation = ch->generation + 1;
    free_string(victim->lord);
    if (ch->generation == 1)
	victim->lord=str_dup(ch->name);
    else
    {
	sprintf(buf,"%s %s",ch->lord,ch->name);
	victim->lord=str_dup(buf);
    }
    free_string(victim->clan);
    victim->clan=str_dup(ch->clan);
    victim->pcdata->stats[UNI_AFF] = 0;
    victim->pcdata->stats[UNI_CURRENT] = 0;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}
/* REWRITEN INTO class.c TO USE THE TABLE! - smoo
void do_class( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int		i = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: class <char> <class>.\n\r", ch );
	send_to_char("      Classes:\n\r", ch );
    send_to_char("None, ", ch);
    send_to_char("Demon, ", ch);
    send_to_char("Demon Lord , ", ch);
    send_to_char("Purple Mage, Blue Mage, Yellow Mage, Green Mage, Red Mage,\n\r ", ch);
    send_to_char("Werewolf, ", ch);
    send_to_char("Monk, ", ch);
    send_to_char("Vampire, ", ch);
    send_to_char("Drow. ", ch);
    send_to_char("Amazon. ", ch);
    send_to_char("ShapeShifter. ", ch);    

    send_to_char("\r\n", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

   if (IS_NPC(victim)) return;

    if ( !str_cmp( arg2, "none"       ) ) 
    {
	victim->pcdata->condition[COND_THIRST] = 10000;
	victim->move = 10000;
	victim->mana = 10000;

	victim->power[DISC_WERE_BEAR]   = 4;
    victim->power[DISC_WERE_LYNX]   = 4;
	victim->power[DISC_WERE_BOAR]   = 4;
	victim->power[DISC_WERE_OWL]    = 4;
	victim->power[DISC_WERE_SPID] = 4; 
	victim->power[DISC_WERE_WOLF]   = 4;
	victim->power[DISC_WERE_HAWK]   = 4;
	victim->power[DISC_WERE_MANT] = 4;

	if ( IS_CLASS(victim, CLASS_WEREWOLF)
	     || IS_CLASS(victim, CLASS_VAMPIRE) ) do_unwerewolf(victim,"");
	if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    victim->pcdata->stats[UNI_FORM0] = 0;
 
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	 send_to_char("Your fangs slide back into your gums.\n\r",victim);
	 act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}
    else if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
    {
    	send_to_char("Your talons slide back into your fingers.\n\r",victim);
	    act("$N's talons slide back into $s fingers.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (IS_AFFECTED(victim,PLR_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, PLR_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
    
    if (IS_VAMPAFF(victim,VAM_DISGUISED) )
    {
     free_string( victim->morph );
     victim->morph = str_dup( victim->name );
     send_to_char("You transform into yourself.\n\r", victim);
     REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(ch,"auto");
	}
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You slowly fade into existence.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}
	
    if (IS_DEMAFF(victim,DEM_HORNS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your horns slide back into your head.\n\r",victim);
	 act("$N's horns slide back into $s head.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	}
	
	if (IS_DEMAFF(victim,DEM_HOOVES) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your hooves transform into feet.\n\r",victim);
	 act("$N's hooves transform back into $s feet.", ch, NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    }
    
    if (IS_DEMAFF(victim,DEM_WINGS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	if (IS_DEMAFF(victim,DEM_UNFOLDED) && IS_CLASS(victim, CLASS_DEMON) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",victim);
	    act("$N's wings fold up behind $s back.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",victim);
	act("$N's wings slide into $s back.", ch, NULL, victim, TO_ROOM);
	REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    }
    
	if ( IS_EXTRA(victim, EXTRA_OSWITCH) ) 
	{
	 do_humanform(victim,"");
	}
ch->moves = 0;
ch->countdown = 0;	
	REMOVE_BIT(victim->special, SPC_CHAMPION );
	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_DEMON_LORD);
    REMOVE_BIT(victim->special, SPC_DROW_WAR );
    REMOVE_BIT(victim->special, SPC_DROW_MAG );
    REMOVE_BIT(victim->special, SPC_DROW_CLE );
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->pcdata->powers[DPOWER_FLAGS] = 0;
  	victim->pcdata->stats[DEMON_TOTAL] = 0;
	victim->pcdata->stats[DEMON_CURRENT] = 0;
	victim->power[DISC_WERE_BEAR]   = 0;
    victim->power[DISC_WERE_LYNX]   = 0;
	victim->power[DISC_WERE_BOAR]   = 0;
	victim->power[DISC_WERE_OWL]    = 0;
	victim->power[DISC_WERE_SPID] = 0; 
	victim->power[DISC_WERE_WOLF]   = 0;
	victim->power[DISC_WERE_HAWK]   = 0;
	victim->pcdata->powers[1] = 0;
	victim->pcdata->condition[COND_THIRST] = 0;

    for (i =0 ; i < MAX_DISCIPLINES; i++)
    {
	victim->power[i] = -1;
    }

	victim->move = victim->max_move;
	victim->mana = victim->max_mana;
	victim->hit = victim->max_hit;
	victim->pcdata->rank = 0;
	
	victim->rage = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->generation     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->beast = 15;
    
    victim->class = 0;
    victim->trust = 0;
    victim->level = 2;
    send_to_char("You are classless now!\n\r", victim);
	return;
    }
    else if ( !str_cmp( arg2, "demon"      ) ) 
    {
    victim->class = CLASS_DEMON;
    set_learnable_disciplines(victim);
    send_to_char("You are now a demon!\n\r", victim);
    }
	else if (!str_cmp(arg2,"drow"	)){
	victim->class = CLASS_DROW;}
    else if ( !str_cmp( arg2, "dlord"      ) ) 
    {
    victim->class = 1;
    victim->level = 3;
    set_learnable_disciplines(victim);
    send_to_char("You are now a Demon Lord!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "purple mage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust = victim->trust;     
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 0;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a purple mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "black mage"     ) &&
str_cmp(ch->pcdata->switchname, "jaal")) //:P
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust = victim->trust;     
    }
    else
    {
    victim->trust = 4;
    }
    victim->level = 4;
    victim->pcdata->powers[MPOWER_RUNE0] = BLACK_MAGIC;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("&DYou are now a black mage!\n\r", victim);
    stc("Ok!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "white mage"     ))
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust = victim->trust;     
    }
    else
    {
    victim->trust = 4;
    }
    victim->level = 4;
    victim->pcdata->powers[MPOWER_RUNE0] = WHITE_MAGIC;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("&WYou are now a white mage!\n\r", victim);
    stc("Ok!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "red mage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
      victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 1;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a red mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "yellow mage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 4;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a yellow mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "blue mage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
      victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 2;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;
    send_to_char("You are now a blue mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "green mage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 3;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;
    send_to_char("You are now a green mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "werewolf"   ) ) 
    {
    victim->class = CLASS_WEREWOLF;
    set_learnable_disciplines(victim);
    send_to_char("You are now a werewolf!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "vampire"    ) ) 
    {
    victim->class = CLASS_VAMPIRE;
    set_learnable_disciplines(victim);
    send_to_char("You are now a vampire!\n\r", victim);
    }

 if (!str_cmp(arg2, "golem"))
 {
    sprintf(log_buf, "%s none", victim->pcdata->switchname);
    do_class(ch, log_buf);

    victim->class = CLASS_GOLEM;

    free_string(victim->clan);
    victim->clan = str_dup("Rogue");

    sprintf(log_buf, "%s 10", victim->pcdata->switchname);
    do_generation(ch, log_buf);

    stc("&DHey! You wanna be a golem? TOO BAD! You are now a &CGolem!#n\n\r", victim);

    return;
 }
    else if ( !str_cmp( arg2, "highlander" ) ) 
    {
    victim->class = 16;
    victim->level = 3;
    do_info(victim, "&CTHE ONE&G has been born!");
    send_to_char("You are now a highlander!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "ninja"      ) ) 
    {
     victim->class = 128;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now a ninja!\n\r", victim);
    }    
        else if ( !str_cmp( arg2, "mindflayer"      ) )
    {
     victim->class = 16384;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now a mindflayer!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "monk"      ) )
    {
     victim->class = CLASS_MONK;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now a monk!\n\r", victim);
    }

    else if ( !str_cmp( arg2, "amazon"      ) )
    {
     victim->class = 256;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now an Amazon!\n\r", victim);
    }


else if ( !str_cmp( arg2, "drow"       ) ) 
    {
    victim->class = 32;
    send_to_char("You are now a drow!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "jedi"       ) )
    {
		victim->class = 2048;
		send_to_char("You are now a jedi!\n\r", victim);
	    victim->class    = CLASS_JEDI;
		victim->pcdata->powers[JPOWER_BODY] = 0;
		victim->pcdata->powers[JPOWER_MIND] = 0;
	    victim->pcdata->powers[JPOWER_SPIRIT] = 0;
		victim->pcdata->powers[JPOWER_TECHNIQUE] = 0;
		victim->pcdata->powers[JAPP_TAKEN] = 0;
		victim->pcdata->powers[JAPP_MASTERED] = 0;
		victim->pcdata->powers[JEDI_FOCUS] = 0;
		victim->pcdata->powers[JEDI_FOCUS_MAX] = 0;
		victim->pcdata->powers[JEDI_STATE] = 0;
		victim->pcdata->powers[JEDI_STATE_MAX] = 0;
    }
    	else if (is_name(arg2, "shape"))
    {
	int store;
	int store2;
	send_to_char("You are now a shapeshifter!\n\r", victim);
	send_to_char("Ok.\n\r", ch);
	victim->class = CLASS_SHAPE;
	store = victim->trust;
	store2 = victim->level;
	victim->trust = 6.9;
	victim->level = 3;
	if (victim->trust < store) victim->trust = store;
	if (victim->level < store2) victim->level = store2;
	return;
	}
else
    {
	send_to_char( "Syntax: class <char> <class>.\n\r", ch );
	send_to_char("      Classes:\n\r", ch );
    send_to_char("None, ", ch);
    send_to_char("Demon, ", ch);
    send_to_char("Demon Lord , ", ch);
    send_to_char("Purple Mage, Blue Mage, Yellow Mage, Green Mage, Red Mage, ", ch);
    send_to_char("Werewolf, ", ch);
    send_to_char("Vampire, ", ch);
    send_to_char("Monk, ", ch);
    send_to_char("Drow",ch);
    send_to_char("Amazon",ch);
    send_to_char("ShapeShifter. ", ch);    
	return;
    }
    send_to_char("Class Set.\n\r",ch);
    return;
}
*/
void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    bool nor = FALSE;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
    send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if (!str_cmp(arg1, "noreport"))
    {
	nor = TRUE;
	strcpy(arg1,arg2);
	argument = one_argument(argument, arg2);
    }    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;
if (!nor)
    send_to_char("Generation Set.\n\r",ch);
    victim->generation = gen;

if (IS_GOLEM(victim))
{ch = victim; // to make it easier

 if (ch->generation > 10 || ch->generation < 1)
    return;

 sprintf(log_buf, "%s has achieved the ancient rank of %s in the %s monarchy.",
    ch->pcdata->switchname,
    golem_ranks[ch->generation],
    !str_cmp(ch->clan, "") ? "Rogue" : ch->clan);

 do_info(ch, log_buf);
}

    return;
}

void do_darkside( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_DEMON))
    {
/*
        send_to_char("That would be a very bad idea...\n\r",ch);
        return;
*/
        send_to_char("You will now allow Lloth to turn you to the dark side.\n\r",ch);
        SET_BIT(ch->immune, IMM_DEMON);
        return;

    }
    send_to_char("You will no longer allow Lloth into your mind.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_DEMON);
    return;
}
/*
void do_arms( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_HIGHLANDER))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
   
    if (ch->practice < 75)
    {
        send_to_char( "It costs at least 75 points of primal energy to grow arms.\n\r", ch);
        return;
    }

   SET_BIT(ch->newbits, THIRD_HAND);
   SET_BIT(ch->newbits, FOURTH_HAND);
   stc("You scream in pain as two hulking arms grow from your sides.\n\r",ch);
   return;

}*/
void do_outcast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if ( ch->generation != 2 && !(IS_CLASS(ch, CLASS_DEMON) && (ch->generation == 13)) && ch->level != 6 )
	{
		send_to_char("You are not a clan leader!\n\r", ch);
		return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Outcast who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
    if ( victim == ch )
    {
    send_to_char("You cannot outcast youself!\n\r", ch);
    return;
    }

	if (ch->class != victim->class)
	{
		send_to_char("They are not the same class as you.\n\r", ch);
		return;
	}
    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

	if ( IS_CLASS(ch,CLASS_DEMON) && (ch->generation == 13) && IS_CLASS(victim, CLASS_DEMON)
		&& !str_cmp(victim->clan,ch->clan) && (victim->generation != 13))
	{
  		sprintf(buf,"%s has been outcasted from %s", victim->name,ch->clan);
		do_info(victim,buf);
		if (IS_VAMPAFF(victim,VAM_FANGS) )
		{
			send_to_char("Your fangs slide back into your gums.\n\r",victim);
			act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
			REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
		}
	    if (IS_VAMPAFF(victim,VAM_CLAWS) ) 
		{
			send_to_char("Your claws slide back under your nails.\n\r",victim);
			act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
			REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
		}
	    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
		{
			send_to_char("The red glow in your eyes fades.\n\r",victim);
			act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
			REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
		}
        if (IS_IMMUNE(victim,IMM_SHIELDED) )
		{
			send_to_char("You stop shielding your aura.\n\r",victim);
			REMOVE_BIT(victim->immune, IMM_SHIELDED);
		}
		if (IS_SET(victim->act, PLR_HOLYLIGHT) )
		{
			REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
			send_to_char( "Your senses return to normal.\n\r", victim );
		}
		if (IS_DEMAFF(victim,DEM_HORNS) )
		{
			send_to_char("Your horns slide back into your head.\n\r",victim);
			act("$N's horns slide back into $s head.", ch, NULL, victim, TO_ROOM);
			REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
		}
		if (IS_DEMAFF(victim,DEM_HOOVES) )
		{
			send_to_char("Your hooves transform into feet.\n\r",victim);
			act("$N's hooves transform back into $s feet.", ch, NULL,victim,TO_ROOM);
			REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
		}
        if (IS_DEMAFF(victim,DEM_WINGS) )
		{
			if (IS_DEMAFF(victim,DEM_UNFOLDED) )
			{
				send_to_char("Your wings fold up behind your back.\n\r",victim);
				act("$N's wings fold up behind $s back.", ch, NULL, victim, TO_ROOM);
				REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
			}
			send_to_char("Your wings slide into your back.\n\r",victim);
			act("$N's wings slide into $s back.", ch, NULL, victim, TO_ROOM);
			REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
		}
    	if ( IS_EXTRA(victim, EXTRA_OSWITCH) )
		{
			do_humanform(victim,"");
		}
		victim->pcdata->powers[DPOWER_FLAGS] = 0;
		victim->pcdata->stats[DEMON_TOTAL] = 0;
		victim->pcdata->stats[DEMON_CURRENT] = 0;
	
		free_string(victim->lord);
		victim->lord     = str_dup( "" );
		free_string(victim->clan);
		victim->clan     = str_dup( "" );

		REMOVE_BIT(victim->special, SPC_CHAMPION );
		REMOVE_BIT(victim->special, SPC_SIRE);
		REMOVE_BIT(victim->special, SPC_PRINCE);
		REMOVE_BIT(victim->special, SPC_DEMON_LORD);
		victim->rage = 0;
		victim->class = 0;
		send_to_char("You have been outcasted from Hell!\n\r", victim);
	}
   
else if ( ( IS_CLASS(ch, CLASS_MAGE) && ch->level == LEVEL_ARCHMAGE )
	        && ch->pcdata->powers[MPOWER_RUNE0] == victim->pcdata->powers[MPOWER_RUNE0]
	        && victim->level < LEVEL_ARCHMAGE )
  {
  	sprintf(buf,"%s has been outcasted from the Mages Guild.", victim->name);
	do_info(victim,buf);  	

    reset_spell(victim,0);
    reset_spell(victim,1);
    reset_spell(victim,2);
    reset_spell(victim,3);
    reset_spell(victim,4);	
    
	victim->trust = 0;
	victim->class = 0;
	victim->level = 3;
	send_to_char("You have been outcasted from the Mages Guild!\n\r",victim);
  }

else if ( IS_CLASS(ch,CLASS_VAMPIRE)
		  && !str_cmp(victim->clan,ch->clan) 
		  && IS_CLASS(victim, CLASS_VAMPIRE) )   
  {
  	sprintf(buf,"%s has been outcasted from %s.", victim->name,
victim->clan);
	do_info(victim,buf);

	victim->pcdata->condition[COND_THIRST] = 10000;

	if ( IS_CLASS(victim, CLASS_VAMPIRE) ) do_unwerewolf(victim,"");
	if ( IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
	if ( IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	 send_to_char("Your fangs slide back into your gums.\n\r",victim);
	 act("$N's fangs slide back into $s gums.", ch, NULL, victim,
TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your
nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim,
TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim,
TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    
    if (IS_AFFECTED(victim,PLR_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, PLR_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
    
    if (IS_VAMPAFF(victim,VAM_DISGUISED) )
    {
     free_string( victim->morph );
     victim->morph = str_dup( victim->name );
     send_to_char("You transform into yourself.\n\r", victim);
     REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into
existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(victim,"auto");
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);

	victim->rage = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->generation     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;
    victim->beast = 15;
    victim->pcdata->condition[COND_THIRST] = 0;
    	
	send_to_char("You have been outcasted!\n\r", victim);
  }
  
else if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->generation == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim,
CLASS_WEREWOLF) )
  {
  	sprintf(buf,"%s has been outcasted from %s.", victim->name,
victim->clan);
	do_info(victim,buf);

	victim->power[DISC_WERE_BEAR]   = 4;
    victim->power[DISC_WERE_LYNX]   = 4;
	victim->power[DISC_WERE_BOAR]   = 4;
	victim->power[DISC_WERE_OWL]    = 4;
	victim->power[DISC_WERE_SPID] = 4; 
	victim->power[DISC_WERE_WOLF]   = 4;
	victim->power[DISC_WERE_HAWK]   = 4;
	victim->power[DISC_WERE_MANT] = 4;
	
	do_unwerewolf(victim,"");
    
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
    	send_to_char("Your talons slide back into your
fingers.\n\r",victim);
	    act("$N's talons slide back into $s
fingers.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim,
TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    
    if (IS_AFFECTED(victim,PLR_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, PLR_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }

    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into
existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(victim,"auto");
	}
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You slowly fade into existence.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->power[DISC_WERE_BEAR]   = 0;
    victim->power[DISC_WERE_LYNX]   = 0;
	victim->power[DISC_WERE_BOAR]   = 0;
	victim->power[DISC_WERE_OWL]    = 0;
	victim->power[DISC_WERE_SPID] = 0; 
	victim->power[DISC_WERE_WOLF]   = 0;
	victim->power[DISC_WERE_HAWK]   = 0;
	victim->power[DISC_WERE_MANT] = 0;

	victim->rage = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->generation     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
     
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_NINJA) && ch->generation == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim,
CLASS_NINJA) )
  {
  	sprintf(buf,"%s has been outcasted from %s.", victim->name,
victim->clan);
	do_info(victim,buf); 

    reset_weapon(victim,0);
    reset_weapon(victim,1);
    reset_weapon(victim,2);
    reset_weapon(victim,3);
    reset_weapon(victim,4);
    reset_weapon(victim,5);
    reset_weapon(victim,6);
    reset_weapon(victim,7);
    reset_weapon(victim,8);
    reset_weapon(victim,9);
    reset_weapon(victim,10);
    reset_weapon(victim,11);
    reset_weapon(victim,12); 

    if ( IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	 	send_to_char("You shove your iron claws up your
sleeves\n\r",victim);
	    act("$N shoves $S iron claws up $e sleeves.", ch, NULL,
victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You appear from the shadows.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
  	victim->pcdata->powers[NPOWER_SORA]     = 0;
  	victim->pcdata->powers[NPOWER_CHIKYU]   = 0;
	victim->pcdata->powers[NPOWER_NINGENNO] = 0;

	victim->rage = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->generation     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
    victim->trust = 0;
    victim->level = 3;
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_DROW) && ch->generation == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim,
CLASS_DROW) )
  {
    sprintf(buf,"%s has been outcasted from %s.", victim->name,
victim->clan);
	do_info(victim,buf);

    reset_weapon(victim,0);
    reset_weapon(victim,1);
    reset_weapon(victim,2);
    reset_weapon(victim,3);
    reset_weapon(victim,4);
    reset_weapon(victim,5);
    reset_weapon(victim,6);
    reset_weapon(victim,7);
    reset_weapon(victim,8);
    reset_weapon(victim,9);
    reset_weapon(victim,10);
    reset_weapon(victim,11);
    reset_weapon(victim,12);

    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
         send_to_char("Your fangs slide back into your gums.\n\r",victim);
         act("$N's fangs slide back into $s gums.", ch, NULL, victim,
TO_ROOM);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }

    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}
		
	victim->pcdata->powers[1] = 0;
  
	free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
	REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_ANARCH);
    REMOVE_BIT(victim->special, SPC_DROW_WAR );
    REMOVE_BIT(victim->special, SPC_DROW_MAG );
    REMOVE_BIT(victim->special, SPC_DROW_CLE );
    victim->rage = 0;
    victim->class = 0; 
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_MONK) && ch->generation == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim,
CLASS_MONK) )
  {
  	sprintf(buf,"%s has been outcasted from %s.", victim->name,
victim->clan);
	do_info(victim,buf);  
	
    reset_weapon(victim,0);
    reset_weapon(victim,1);
    reset_weapon(victim,2);
    reset_weapon(victim,3);
    reset_weapon(victim,4);
    reset_weapon(victim,5);
    reset_weapon(victim,6);
    reset_weapon(victim,7);
    reset_weapon(victim,8);
    reset_weapon(victim,9);
    reset_weapon(victim,10);
    reset_weapon(victim,11);
    reset_weapon(victim,12);

	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You appear from a shroud of light.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your view shimmers into mortal vision.\n\r",
victim );
	}
	
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->pcdata->powers[PMONK] = 0;

	victim->rage = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->generation     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
    victim->trust = 0;
    victim->level = 3;
	send_to_char("You have been outcasted!\n\r", victim);
  }
  else
  	send_to_char("They are not of your clan!\n\r", ch);
  return;
}  

void reset_weapon( CHAR_DATA *ch, int dtype )
{
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}

/*Added in from sage vic code for multi class clananme*/
void do_clanname( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' )
    {
    send_to_char( "Who's clan do you wish to change or name?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) ) return;

    smash_tilde( argument );
    if ( strlen(bash_color(argument)) < 0 || strlen(bash_color(argument)) > 20 )
    {
        send_to_char( "Clan name should be between 0 and 20 letters long.\n\r", ch );
        send_to_char( "Leave a blank to remove clan.\n\r", ch);
        return;
    }
    free_string( victim->clan );
    victim->clan = str_dup( argument );
        send_to_char("Clan set.\n\r", ch);
        send_to_char("Clan set.\n\r", victim);
    return;
}
void do_stake( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *stake;
    char      arg [MAX_INPUT_LENGTH];
    int       blood;
stc("You want a stake? Go to tony roma's:P I don't like stake because it's unfair, so it's out.\n\r", ch);
return;

    argument = one_argument( argument, arg );
    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stake whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    stake = get_eq_char( ch, WEAR_HOLD );
    if ( stake == NULL || stake->item_type != ITEM_STAKE )
    {
	stake = get_eq_char( ch, WEAR_WIELD );
	if ( stake == NULL || stake->item_type != ITEM_STAKE )
	{
	    send_to_char( "How can you stake someone down without holding a stake?\n\r", ch );
	    return;
	}
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot stake yourself.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char( "You can only stake vampires.\n\r", ch );
	return;
    }

    if (victim->position > POS_MORTAL)
    {
	send_to_char( "You can only stake down a vampire who is mortally wounded.\n\r", ch );
	return;
    }

    act("You plunge $p into $N's heart.", ch, stake, victim, TO_CHAR);
    act("$n plunges $p into $N's heart.", ch, stake, victim, TO_NOTVICT);
    send_to_char( "You feel a stake plunged through your heart.\n\r", victim );
    if (IS_IMMUNE(victim,IMM_STAKE)) return;

    /* Have to make sure they have enough blood to change back */
    blood = victim->pcdata->condition[COND_THIRST];
    victim->pcdata->condition[COND_THIRST] = 666;

    /* To take care of vampires who have powers in affect. */
    if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    if (IS_IMMUNE(victim,IMM_SHIELDED) ) do_shield(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) ) do_shadowplane(victim,"");
    if (IS_VAMPAFF(victim,VAM_FANGS) ) do_fangs(victim,"");
    if (IS_VAMPAFF(victim,VAM_CLAWS) ) do_claws(victim,"");
    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) ) do_nightsight(victim,"");
    if (IS_AFFECTED(victim,PLR_SHADOWSIGHT) ) do_shadowsight(victim,"");
    if (IS_SET(victim->act,PLR_HOLYLIGHT) ) do_truesight(victim,"");
    if (IS_VAMPAFF(victim,VAM_CHANGED) ) do_change(victim,"human");
    if (IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    victim->rage = 0;
    victim->pcdata->condition[COND_THIRST] = blood;

    REMOVE_BIT(victim->class, CLASS_VAMPIRE);
    obj_from_char(stake);
    obj_to_char(stake,victim);
    ch->exp = ch->exp + 1000;
    victim->home = 3001;
    return;
}

void do_mask( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->power[DISC_VAMP_OBFU] < 1  )
    {
        send_to_char("You must obtain at least level 1 in Obfuscate to use Mask.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
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

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
	send_to_char( "You can only mask avatars or lower.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 40 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(30,40);

    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	sprintf(buf,"Your form shimmers and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"%s's form shimmers and transforms into %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}

void do_change( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_PROT] < 3  )
    {
        send_to_char("You must obtain at least level 3 in Protean to use Arcane Change.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can arcane change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
	return;
    }

    if ( !str_cmp(arg,"bat") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	clear_stats(ch);
	act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	SET_BIT(ch->polyaff, POLY_BAT);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf, "%s the vampire bat", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"wolf") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM );
	clear_stats(ch);
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * 3);
    	}
    	ch->pcdata->mod_str = 10;
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	SET_BIT(ch->polyaff, POLY_WOLF);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	ch->max_hit = ch->max_hit + 500;
	ch->hit = ch->hit + 500;
	sprintf(buf, "%s the dire wolf", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"mist") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r",
ch );
	    return;
	}

	if (has_timer(ch)) return;

    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM );
	if (IS_EXTRA(ch, TIED_UP))
	{
	    act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
	    act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    REMOVE_BIT(ch->extra, TIED_UP);
	    REMOVE_BIT(ch->extra, GAGGED);
	    REMOVE_BIT(ch->extra, BLINDFOLDED);
	}
	if (is_affected(ch, gsn_web))
	{
	    act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
	    affect_strip(ch, gsn_web);
	}
	clear_stats(ch);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	SET_BIT(ch->polyaff, POLY_MIST);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->affected_by, AFF_ETHEREAL);
	sprintf(buf, "%s the white mist", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"human") )
    {
       if (!IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You are already in human form.\n\r", ch );
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
	{
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	    REMOVE_BIT(ch->polyaff, POLY_BAT);
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	    REMOVE_BIT(ch->polyaff, POLY_WOLF);
	    ch->max_hit = ch->max_hit - 500;
	    ch->hit = ch->hit - 500;
	    if (ch->hit < 1) ch->hit = 1;
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	    REMOVE_BIT(ch->polyaff, POLY_MIST);
	    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
	}
	else
	{
	    /* In case they try to change to human from a non-vamp form */
	    send_to_char( "You seem to be stuck in this form.\n\r", ch );
	    return;
	}
	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	clear_stats(ch);
  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    else
	send_to_char( "You can arcane change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
    return;
}
void do_clandisc( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        do_stat(ch,"");
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"animalism"))
	{
            send_to_char("Animalism: The discipline of nature control.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] < 1) 
                send_to_char("You have none of the Animalism powers.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] > 0) 
                send_to_char("Serenity: You can sooth the most aggressive soul (Golconda Required).\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"auspex"))
	{
            send_to_char("Auspex: The discipline of extrasensory awareness.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] < 1) 
                send_to_char("You have none of the Auspex discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 0) 
                send_to_char("Truesight: The user can see everything, including invis.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 1) 
                send_to_char("Scry: Allows you to view the room the specified target is in.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 2) 
                send_to_char("Readaura: Gives you detained information about a creature or object.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"celerity"))
	{
            send_to_char("Celerity: The discipline of superior speed.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"daimoinon"))
	{
            send_to_char("Daimoinon: The discipline of dark corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_DAIM] < 1) 
                send_to_char("You have none of the Daimoinon discipline powers.\n\r",ch);
	    return;
        }

        else if (!str_cmp(arg1,"dominate"))
	{
            send_to_char("Dominate: The discipline of control.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] < 1) 
                send_to_char("You have none of the Dominate discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 0) 
                send_to_char("Command: You can plant suggestions in the mind of others.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 1) 
                send_to_char("Shield: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 3) 
                send_to_char("Possession: You can take control of mobs by feeding them blood.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"fortitude"))
	{
            send_to_char("Fortitude: The discipline of supernatural toughness and vigor.\n\r",ch);
            if (ch->power[DISC_VAMP_FORT] < 1) 
		send_to_char("You have no fortitude.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obfuscate"))
	{
            send_to_char("Obfuscate: The discipline of stealth.\n\r",ch);
            if (ch->power[DISC_VAMP_OBFU] < 1) 
                send_to_char("You have none of the Obfuscate discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obtenebration"))
	{
            send_to_char("Obtenebration: The discipline of darkness manipulation.\n\r",ch);
            if (ch->power[DISC_VAMP_OBTE] < 1) 
                send_to_char("You have none of the Obtenebration discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"potence"))
	{
            send_to_char("Potence: The discipline of superior strength.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"presence"))
	{
            send_to_char("Presence: The discipline of appeal and attractiveness.\n\r",ch);
            if (ch->power[DISC_VAMP_PRES] < 1) 
                send_to_char("You have none of the Presence discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"protean"))
	{
            send_to_char("Protean: The discipline of neutral transformation.\n\r",ch);
            if (ch->power[DISC_VAMP_PROT] < 1) 
                send_to_char("You have none of the Protean discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"quietus"))
	{
            send_to_char("Quietus: The discipline of death.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] < 1) 
                send_to_char("You have none of the Quietus discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] > 3)
                send_to_char("Spit: You can project your deadly spit at a foe\n\r",ch); 
           return;
        }
        else if (!str_cmp(arg1,"serpentis"))
	{
            send_to_char("Serpentis: The discipline of temptation and corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_SERP] < 1) 
                send_to_char("You have none of the Serpentis discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"thaumaturgy"))
	{
            send_to_char("Thaumaturgy: The discipline of blood magic.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] < 1) 
                send_to_char("You have none of the Thaumaturgy discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] > 0) 
                send_to_char("Theft: You can take from the blood pool of other players.\n\r",ch);

            return;
        }
        else if (!str_cmp(arg1,"vicissitude"))
	{
            send_to_char("Vicissitude: The discipline of evil body control.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] < 1) 
                send_to_char("You have none of the Vicissitude discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] > 3) 
                send_to_char("Zuloform: Transformation into a powerful being.\n\r",ch);

            return;
        }
        do_stat(ch,"");
        return;

    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;
        int maxlevel;

	stc("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

        if (ch->pcdata->rank == AGE_CHILDE || ch->pcdata->rank ==AGE_NEONATE)
          maxlevel=5;
        else if (ch->pcdata->rank == AGE_ANCILLA)
          maxlevel=7;
        else if (ch->pcdata->rank == AGE_ELDER)
          maxlevel=9;
        else
         maxlevel=10;
             if (!str_cmp(arg1,"animalism"       )) {improve = DISC_VAMP_ANIM;          max=maxlevel;}
        else if (!str_cmp(arg1,"auspex"          )) {improve = DISC_VAMP_AUSP;          max=maxlevel;}
        else if (!str_cmp(arg1,"celerity"        )) {improve = DISC_VAMP_CELE;        max=maxlevel;}
        else if (!str_cmp(arg1,"dominate"        )) {improve = DISC_VAMP_DOMI;        max=maxlevel;}
        else if (!str_cmp(arg1,"daimoinon"       )) {improve = DISC_VAMP_DAIM;max=maxlevel;}
        else if (!str_cmp(arg1,"fortitude"       )) {improve = DISC_VAMP_FORT;       max=maxlevel;}
        else if (!str_cmp(arg1,"obfuscate"       )) {improve = DISC_VAMP_OBFU;       max=maxlevel;}
        else if (!str_cmp(arg1,"obtenebration"   )) {improve = DISC_VAMP_OBTE;   max=maxlevel;}
        else if (!str_cmp(arg1,"potence"         )) {improve = DISC_VAMP_POTE;         max=maxlevel;}
        else if (!str_cmp(arg1,"presence"        )) {improve = DISC_VAMP_PRES;        max=maxlevel;}
        else if (!str_cmp(arg1,"protean"         )) {improve = DISC_VAMP_PROT;         max=maxlevel;}
        else if (!str_cmp(arg1,"quietus"         )) {improve = DISC_VAMP_QUIE;         max=maxlevel;}
        else if (!str_cmp(arg1,"serpentis"       )) {improve = DISC_VAMP_SERP;       max=maxlevel;}
        else if (!str_cmp(arg1,"thaumaturgy"     )) {improve = DISC_VAMP_THAU;            max=maxlevel;}
        else if (!str_cmp(arg1,"vicissitude"     )) {improve = DISC_VAMP_VICI;     max=maxlevel;}
        else
	{
            send_to_char("You can improve: Animalism, Auspex, Celerity, Dominate, Fortitude, Obfuscate,\n\r Obtenebration, Potence, Presence, Protean, Quietus, Serpentis,\n\r Thaumaturgy or Vicissitude.\n\r",ch);
	    return;
	}
        cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max && ch->pcdata->rank ==AGE_METHUSELAH)
	{
            sprintf(buf,"You have already gained all the powers of the %s discipline.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
        if (ch->pcdata->powers[improve] >= max)
        {
           sprintf(buf,"You must gain Age Rank to improve %s farther.\n\r",arg1);
           send_to_char(buf,ch);
           return;
        }
	if ( cost > ch->practice )
	{
            sprintf(buf,"It costs you %d primal to improve your %s discipline.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s discipline.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a discipline, type: Clandisc <discipline type> improve.\n\r",ch);
    return;
}


void do_vampire( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_VAMPIRE))
    {
	send_to_char("You will now allow vampires to bite you.\n\r",ch);
	SET_BIT(ch->immune, IMM_VAMPIRE);
	return;
    }
    send_to_char("You will no longer allow vampires to bite you.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_VAMPIRE);
    return;
}

void do_shadowplane( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 3)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
        if (ch->power[DISC_VAMP_OBTE] < 3)
	{
            send_to_char("You must obtain at least level 3 in Obtenebration to use Shadowplane .\n\r",ch);
	    return;
	}
    }

	if (has_timer(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 75)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(65,75);

    if ( arg[0] == '\0' )
    {
    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
	    send_to_char("You fade into the plane of shadows.\n\r",ch);
	    act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
    	send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}


void do_bloodline( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char      arg3 [MAX_INPUT_LENGTH];
    char      buf  [MAX_STRING_LENGTH];
    char      buf2 [MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if (IS_NPC(ch)) return;

    if (arg1 != '\0') arg1[0] = UPPER(arg1[0]);
    if (arg2 != '\0') arg2[0] = UPPER(arg2[0]);
    if (arg3 != '\0') arg3[0] = UPPER(arg3[0]);

    if (!IS_CLASS(ch, CLASS_WEREWOLF)) {stc("Huh?\n\r", ch);return;}

    if (!str_cmp(arg1,"kavir")) strcpy(arg1,"KaVir");

    if ( !str_cmp(ch->clan,"") && ch->generation != 1 )
    {
	strcpy(buf,"In the name of Gaia, I announce my Garou heritage.");
	do_say(ch,buf);
	sprintf(buf,"My name is %s, I am a Ronin of no tribe.", ch->name);
	do_say(ch,buf);
	return;
    }
    strcpy(buf,"In the name of Gaia, I announce my Garou heritage.");
    do_say(ch,buf);
    if (ch->generation != 1)
    {
	if      (ch->generation == 4) sprintf(buf2,"%s",arg3);
	else if (ch->generation == 3) sprintf(buf2,"%s",arg2);
	else if (ch->generation == 2) sprintf(buf2,"%s",arg1);
	if (ch->generation == 1)
	    sprintf(buf,"My name is %s, chosen Champion of Gaia.", ch->name);
	if (ch->generation == 2)
	    sprintf(buf,"My name is %s, Chieftain of the %s tribe, pup of %s.", ch->name,ch->clan,buf2);
	else
	    sprintf(buf,"My name is %s, of the %s tribe, pup of %s.", ch->name,ch->clan,buf2);
	do_say(ch,buf);
    }
    if ( arg3[0] != '\0' )
    {
	sprintf(buf,"My name is %s, of the %s tribe, pup of %s.",
	arg3,ch->clan,arg2);
	do_say(ch,buf);
    }
    if ( arg2[0] != '\0' )
    {
	if ( arg1[0] != '\0' )
	    sprintf(buf,"My name is %s, Chieftain of the %s tribe, pup of %s.",arg2,ch->clan,arg1);
	else
	    sprintf(buf,"My name is %s, of the %s tribe, pup of %s.",arg2,ch->clan,arg1);
	do_say(ch,buf);
    }
    if ( ch->generation == 1 )
	sprintf(buf,"My name is %s, chosen Champion of Gaia.", ch->name);
    else
	sprintf(buf,"My name is %s, chosen Champion of Gaia.", arg1);
    do_say(ch,buf);
    return;
}

void do_tradition( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char      arg3 [MAX_INPUT_LENGTH];
    char      arg4 [MAX_INPUT_LENGTH];
    char      arg5 [MAX_INPUT_LENGTH];
    char      arg6 [MAX_INPUT_LENGTH];
    char      buf  [MAX_INPUT_LENGTH];
    char      buf2 [MAX_INPUT_LENGTH];
    char      buf3 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );
    argument = one_argument( argument, arg6 );

    if (IS_NPC(ch)) return;

    if (arg1 != '\0') arg1[0] = UPPER(arg1[0]);
    if (arg2 != '\0') arg2[0] = UPPER(arg2[0]);
    if (arg3 != '\0') arg3[0] = UPPER(arg3[0]);
    if (arg4 != '\0') arg4[0] = UPPER(arg4[0]);
    if (arg5 != '\0') arg5[0] = UPPER(arg5[0]);
    if (arg6 != '\0') arg6[0] = UPPER(arg6[0]);

    if (!str_cmp(arg1,"kavir")) strcpy(arg1,"KaVir");

    if (!IS_CLASS(ch, CLASS_VAMPIRE) || (ch->generation < 1) || (ch->generation > 13))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/*
    if ( !str_cmp(ch->clan,"") && ch->generation == 2 )
    {
	send_to_char( "Not until you've created your own clan!\n\r", ch );
	return;
    }
*/
    if      (ch->generation == 2)                   sprintf(buf3,"Antediluvian");
    else if (ch->pcdata->rank == AGE_NEONATE)    sprintf(buf3,"Neonate");
    else if (ch->pcdata->rank == AGE_ANCILLA)    sprintf(buf3,"Ancilla");
    else if (ch->pcdata->rank == AGE_ELDER)      sprintf(buf3,"Elder");
    else if (ch->pcdata->rank == AGE_METHUSELAH) sprintf(buf3,"Methuselah");
    else                                         sprintf(buf3,"Childe");

if (ch->generation==13) sprintf(buf2,"Thirteenth");
else if (ch->generation==12) sprintf(buf2,"Twelth");
else if (ch->generation==11) sprintf(buf2,"Eleventh");
else if (ch->generation==10) sprintf(buf2,"Tenth");

else if (ch->generation==9) sprintf(buf2,"Ninth");
else if (ch->generation==8) sprintf(buf2,"Eighth");

    else if      (ch->generation == 7) sprintf(buf2,"Seventh");
    else if (ch->generation == 6) sprintf(buf2,"Sixth");
    else if (ch->generation == 5) sprintf(buf2,"Fifth");
    else if (ch->generation == 4) sprintf(buf2,"Fourth");
    else if (ch->generation == 3) sprintf(buf2,"Third");
    else if (ch->generation == 2) sprintf(buf2,"Second");
    if (ch->generation == 1)
	sprintf(buf,"As is the tradition, I recite the lineage of %s, Sire of all Kindred.",ch->name);
    else
	sprintf(buf,"As is the tradition, I recite the lineage of %s, %s of the %s Generation.",ch->name,buf3,buf2);
    do_say(ch,buf);
    if ( ch->generation != 1 )
    {
	if      (ch->generation == 7) sprintf(buf2,"%s",arg6);
	else if (ch->generation == 6) sprintf(buf2,"%s",arg5);
	else if (ch->generation == 5) sprintf(buf2,"%s",arg4);
	else if (ch->generation == 4) sprintf(buf2,"%s",arg3);
	else if (ch->generation == 3) sprintf(buf2,"%s",arg2);
	else if (ch->generation == 2) sprintf(buf2,"%s",arg1);

	if (IS_SET(ch->special, SPC_ANARCH) || strlen(ch->clan) < 2)
	    sprintf(buf,"My name is %s.  I am of no clan.", ch->name);
	else if (ch->generation == 2)
	    sprintf(buf,"My name is %s. I founded %s.", ch->name,ch->clan);
	else
	    sprintf(buf,"My name is %s.  I am of %s.", ch->name,ch->clan);
	do_say(ch,buf);
    }
    if ( arg6[0] != '\0' )
    {
	sprintf(buf,"My name is %s.  My sire is %s.",
	arg6,arg5);
/*	do_say(ch,buf);*/
    }
    if ( arg5[0] != '\0' )
    {
	sprintf(buf,"My name is %s.  My sire is %s.",
	arg5,arg4);
/*	do_say(ch,buf);*/
    }
    if ( arg4[0] != '\0' )
    {
	sprintf(buf,"My name is %s.  My sire is %s.",
	arg4,arg3);
/*	do_say(ch,buf);*/
    }
    if ( arg3[0] != '\0' )
    {
	sprintf(buf,"My name is %s.  My sire is %s.",
	arg3,arg2);
/*	do_say(ch,buf);*/
    }
    if ( arg2[0] != '\0' )
    {
	sprintf(buf,"My name is %s.  My sire is %s.",arg2,arg1);
/*	do_say(ch,buf);*/
    }
    if ( ch->generation == 1 )
	sprintf(buf,"My name is %s.  All Kindred are my childer.",ch->name);
    else
	sprintf(buf,"My Master is Caine.  All Kindred are his childer.");
    do_say(ch,buf);
    if ( ch->generation == 7 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s.  Recognize my lineage.",ch->name,arg6,arg5,arg4,arg3,arg2,arg1);
    if ( ch->generation == 6 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s.  Recognize my lineage.",ch->name,arg5,arg4,arg3,arg2,arg1);
    if ( ch->generation == 5 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s.  Recognize my lineage.",ch->name,arg4,arg3,arg2,arg1);
    if ( ch->generation == 4 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s.  Recognize my lineage.",ch->name,arg3,arg2,arg1);
    if ( ch->generation == 3 ) sprintf(buf,"My name is %s, childe of %s, childe of %s.  Recognize my lineage.",ch->name,arg2,arg1);
    if ( ch->generation == 2 ) sprintf(buf,"My name is %s, childe of %s.  Recognize my lineage.",ch->name,arg1);
    if ( ch->generation == 1 ) sprintf(buf,"My name is %s.  Recognize my lineage.",ch->name);
/*    do_say(ch,buf);*/
    return;
}

void do_darkheart( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_SERP] < 1  )
    {
        send_to_char("You must obtain at least level 1 in Serpentis to use Darkheart.\n\r",ch);
	return;
    }
    if (IS_IMMUNE(ch,IMM_STAKE) )
    {
	send_to_char("But you've already torn your heart out!\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 1000 / ch->generation )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= 100;
    send_to_char("You rip your heart from your body and toss it to the ground.\n\r",ch);
    act("$n rips $s heart out and tosses it to the ground.", ch, NULL, NULL, TO_ROOM);
    make_part( ch, "heart" );
    ch->hit = ch->hit - number_range(10,20);
    update_pos(ch);
    if (ch->position == POS_DEAD && !IS_HERO(ch))
    {
	send_to_char( "You have been KILLED!!\n\r\n\r", ch );
	raw_kill(ch);
	return;
    }
    SET_BIT(ch->immune, IMM_STAKE);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
if (!will_use(ch, "UltraVision") && !IS_GOLEM(ch) && !IS_MOOGLE(ch)) {
         if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch,
CLASS_HIGHLANDER) &&!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_WEREWOLF) )
	 {
	     send_to_char("Huh?\n\r",ch);
	     return;
         }

    if ((ch->power[DISC_VAMP_AUSP] < 1) && IS_CLASS(ch, CLASS_VAMPIRE))
	{
	send_to_char("You must obtain at least level 1 in Auspex to use Truesight.\n\r", ch);
        return;
        }

    if ((ch->power[DISC_WERE_OWL] < 3) && IS_CLASS(ch,CLASS_WEREWOLF))
        {
	send_to_char("You must obtain level 3 in the Owl totem to use Truesight.\n\r",ch);
	return;
	}
}
if (IS_GOLEM(ch) && ch->pcdata->powers[SYMANTICS] < 10)
{stc("You need Symantics 10 to use that.\n\r", ch);return;}
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}

/*void do_majesty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 5  )
    {
        send_to_char("You must obtain at least level 5 in Presence to use Majesty.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_PEACE))
    {
        REMOVE_BIT(ch->affected_by, AFF_PEACE);
        act( "You lower your majesty.",  ch, NULL, NULL, TO_CHAR    );
        act( "$n looks less imposing.",  ch, NULL, NULL, TO_NOTVICT );
        return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 45 )
    {
	send_to_char( "You have insufficient blood.\n\r", ch );
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(35,45);
    SET_BIT(ch->affected_by, AFF_PEACE);
    act( "You raise your majesty.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n suddenly looks very imposing.",  ch, NULL, NULL, TO_NOTVICT);
    return;
}*/
void do_majesty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 5  )
    {
        send_to_char("You must obtain at least level 5 in Presence to use Majesty.\n\r",ch);
		return;
    }
	send_to_char("Majesty is now automatic damage cap bonus.\n\r",ch);
	return;
}

void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
if (!will_use(ch, "Scry")) {
    if (!IS_CLASS(ch, CLASS_MONK)
	&& !IS_CLASS(ch, CLASS_VAMPIRE)
	&& !IS_CLASS(ch, CLASS_NINJA)
	&& !IS_CLASS(ch, CLASS_MAGE)
	&& !IS_CLASS(ch, CLASS_WEREWOLF)
	&& !IS_CLASS(ch, CLASS_DROW)
        && !IS_CLASS(ch, CLASS_DEMON)
        && !IS_CLASS(ch, CLASS_AMAZON)
        && !IS_CLASS(ch, CLASS_HIGHLANDER)
	&& !IS_GOLEM(ch)
	&& !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

/* Drow scry */
	if (!IS_SET(ch->pcdata->powers[1],DPOWER_DROWSIGHT)
		&& !IS_CLASS(ch, CLASS_DEMON)
		&& !IS_ITEMAFF(ch, ITEMA_VISION)
        && !IS_SET(ch->special, SPC_CHAMPION)
        && !IS_CLASS(ch, CLASS_MAGE)   
        && !IS_CLASS(ch, CLASS_VAMPIRE)
        && !IS_CLASS(ch, CLASS_MONK)
        && !IS_CLASS(ch, CLASS_NINJA)
		&& !IS_CLASS(ch, CLASS_WEREWOLF)
		&& !IS_CLASS(ch, CLASS_AMAZON)
		&& IS_CLASS(ch, CLASS_DROW))
	{
		send_to_char("You do not have the spidersight power to use scry.\n\r",ch);
		return;
	}

if (IS_GOLEM(ch) && ch->pcdata->powers[SYMANTICS] < 7)
{stc("You need Symantics 4 to use that!\n\r", ch); return;}      
          if (IS_CLASS(ch, CLASS_DEMON)
            && !IS_ITEMAFF(ch, ITEMA_VISION)
        && !IS_SET(ch->special, SPC_CHAMPION)
        && !IS_CLASS(ch, CLASS_MAGE)
        && !IS_CLASS(ch, CLASS_VAMPIRE)
        && !IS_CLASS(ch, CLASS_MONK)
        && !IS_CLASS(ch, CLASS_NINJA)
            && !IS_CLASS(ch, CLASS_WEREWOLF)
            && !IS_CLASS(ch, CLASS_DROW)
		&& !IS_CLASS(ch, CLASS_AMAZON)
        && ch->power[DISC_DAEM_NETH] < 3)
        {
            send_to_char("You require nether 3 to scry.\n\r",ch);
            return;
        }

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_ITEMAFF(ch, ITEMA_VISION)
        && !IS_SET(ch->special, SPC_CHAMPION)
        && IS_CLASS(ch, CLASS_MAGE)   
        && !IS_CLASS(ch, CLASS_VAMPIRE)
        && !IS_CLASS(ch, CLASS_MONK)
        && !IS_CLASS(ch, CLASS_NINJA)
		&& !IS_CLASS(ch, CLASS_WEREWOLF)
		&& !IS_CLASS(ch, CLASS_DROW)
		&& !IS_CLASS(ch, CLASS_AMAZON)
	&& ch->mana < 500)
    {
	send_to_char("You do not have the required mana to scry.\n\r", ch);
	return;
    }


    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_SET(ch->special, SPC_CHAMPION)
	&& !IS_CLASS(ch, CLASS_MAGE) 
	&& !IS_CLASS(ch, CLASS_VAMPIRE) 
	&& !IS_CLASS(ch, CLASS_MONK) 
	&& !IS_CLASS(ch, CLASS_NINJA) 
	&& IS_CLASS(ch, CLASS_WEREWOLF)
	&& !IS_CLASS(ch, CLASS_DROW)
	&& !IS_CLASS(ch, CLASS_AMAZON)
	&& ch->power[DISC_WERE_OWL] < 6)
	{
		send_to_char("You must have the Owl Totem to 6 before you can scry.\n\r", ch);
		return;
	}

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_SET(ch->special, SPC_CHAMPION)
	&& !IS_CLASS(ch, CLASS_MAGE) 
	&& !IS_CLASS(ch, CLASS_VAMPIRE) 
	&& !IS_CLASS(ch, CLASS_MONK) 
	&& IS_CLASS(ch, CLASS_NINJA)
	&& !IS_CLASS(ch, CLASS_DROW)
	&& !IS_CLASS(ch, CLASS_WEREWOLF)
	&& !IS_CLASS(ch, CLASS_AMAZON)
	&& ch->pcdata->powers[NPOWER_SORA] < 1)
    {
	send_to_char("You must obtain the Sora principle to level 1.\n\r", ch);
	return;
    }

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION) && 
        ch->power[DISC_VAMP_AUSP] < 2 && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_CLASS(ch, CLASS_MONK)
	&& !IS_CLASS(ch, CLASS_AMAZON)
	&& !IS_CLASS(ch, CLASS_DROW)
	&& !IS_CLASS(ch, CLASS_NINJA)
	&& !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_WEREWOLF)
	&& IS_CLASS(ch, CLASS_VAMPIRE))

    {
        send_to_char("You must obtain at least level 2 Auspex to use Scry.\n\r",ch);
	return;
    }

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON) &&
	!IS_SET(ch->special, SPC_CHAMPION) && !IS_ITEMAFF(ch, ITEMA_VISION) 
	&& ch->pcdata->powers[PMONK] < 2 
	&& !IS_CLASS(ch, CLASS_DROW)
	&& !IS_CLASS(ch, CLASS_NINJA)
	&& !IS_CLASS(ch, CLASS_AMAZON)
	&& !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_WEREWOLF)
	&& IS_CLASS(ch, CLASS_MONK))
    {
	send_to_char("You need to raise your Mantra to level 2!\n\r",ch);
	return;
    }

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON) &&
	!IS_SET(ch->special, SPC_CHAMPION) && !IS_ITEMAFF(ch, ITEMA_VISION) 
	&& ch->pcdata->powers[PAMAZON] < 2 
	&& !IS_CLASS(ch, CLASS_DROW)
	&& !IS_CLASS(ch, CLASS_NINJA)
	&& !IS_CLASS(ch, CLASS_MONK)
	&& !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_WEREWOLF)
	&& IS_CLASS(ch, CLASS_AMAZON))
    {
	send_to_char("You need to raise your Drills to level 2!\n\r",ch);
	return;
    }

}
    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION) &&
	!IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_DROW) 
	&& ch->pcdata->condition[COND_THIRST] < 25 && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_AMAZON) &&
!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_WEREWOLF) &&
!IS_CLASS(ch, CLASS_SHAPE) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch,
CLASS_NINJA))
	ch->pcdata->condition[COND_THIRST] -= number_range(15,25);
if (IS_GOLEM(ch))
{
sprintf(log_buf, "&YYour Golem senses tell you they are in &R%s&Y.\n\r",
	victim->in_room->area->name);
stc(log_buf, ch);
if (IS_AFFECTED(victim, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_ETHEREAL))
stc("&YYour golem senses also let you know they are in the shadowplane and ethereal.\n\r", ch);
else if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
stc("&YYour golem senses also let you know they are in the shadowplane.\n\r", ch);
else if (IS_AFFECTED(victim,AFF_ETHEREAL))
stc("&YYour golem senses also let you know they are in the ethereal.\n\r", ch);

return;}

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
  if (!IS_NPC(victim));

/* crossed fingers this is the mage scry cost place */
    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_ITEMAFF(ch, ITEMA_VISION)
        && !IS_SET(ch->special, SPC_CHAMPION)
        && IS_CLASS(ch, CLASS_MAGE)
        && !IS_CLASS(ch, CLASS_VAMPIRE)
        && !IS_CLASS(ch, CLASS_MONK) 
        && !IS_CLASS(ch, CLASS_NINJA)
		&& !IS_CLASS(ch, CLASS_DROW)
		&& !IS_CLASS(ch, CLASS_AMAZON)
		&& !IS_CLASS(ch, CLASS_WEREWOLF)
        && ch->mana > 500)
    {
	ch->mana = ch->mana -500;
	return;
    }
/* Throwing a monk a bone apparently, scry detection if they can scry back */
	if (!IS_CLASS(victim, CLASS_DEMON) && !IS_ITEMAFF(victim, ITEMA_VISION)
		&& !IS_SET(victim->special, SPC_CHAMPION)
		&& !IS_CLASS(victim, CLASS_MAGE)
		&& !IS_CLASS(victim, CLASS_DROW)
		&& !IS_CLASS(victim, CLASS_VAMPIRE)
		&& !IS_CLASS(victim, CLASS_NINJA)
		&& !IS_CLASS(victim, CLASS_WEREWOLF)
		&& !IS_CLASS(victim, CLASS_AMAZON)
		&& IS_CLASS(victim, CLASS_MONK)
		&& victim->pcdata->powers[PMONK] > 1
		&& victim->level > 2)
	{
		send_to_char("You feel your soul being invaded by an outside force.\n\r", victim);
		return;
	}
	if (!IS_NPC(victim) && (victim->pcdata->powers[JPOWER_SPIRIT] < 2) && IS_CLASS(victim, CLASS_JEDI))
		send_to_char("You sense the force moving as you are being watched.\n\r", victim);
    return;
}

void do_readaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
if (!will_use(ch, "Superior Knowledge")) {
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ITEMAFF(ch, ITEMA_VISION) && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_AMAZON))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_AUSP] < 3 && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
        send_to_char("You must obtain at least level 3 in Auspex to use Readaura.\n\r",ch);
		return;
    }

	if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->pcdata->condition[COND_THIRST] < 50))
	{
		send_to_char("You have insufficient blood to perform this.\n\r",ch);
		return;
	}
	if (IS_CLASS(ch, CLASS_NINJA) && (ch->pcdata->powers[NPOWER_SORA] < 2))
	{
		send_to_char("You do not have the Sora principle to level 2.\n\r", ch);
		return;
	}
    if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 2)
    {
		send_to_char("You must obtain the level 2 Mantra first!\n\r",ch);
		return;
    }

    if (IS_CLASS(ch, CLASS_AMAZON) && ch->pcdata->powers[PAMAZON] < 2)
    {
		send_to_char("You must obtain the level 2 Drill first!\n\r",ch);
		return;
    }
}

    if ( arg[0] == '\0' )
    {
		send_to_char( "Read the aura on what?\n\r", ch );
		return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
		{
			send_to_char( "Read the aura on what?\n\r", ch );
			return;
		}
		if (!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_CLASS(ch, CLASS_MONK) && IS_CLASS(ch, CLASS_VAMPIRE))
			ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
		act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
		spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
		return;
		}

		if (!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_CLASS(ch, CLASS_SHAPE))
			ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
		if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch,
ITEMA_VISION))
		{if (!will_use(ch, "Superior Knowledge")) {
			send_to_char("You are unable to read their aura.\n\r",ch);
		return;}
		}
		act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
	    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
		if (IS_NPC(victim))
			sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
		else 
		{
	if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}

void do_mortal( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_VAMPAFF(ch, VAM_MORTAL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
if (IS_VAMPAFF(ch, VAM_MORTAL) && ch->class != 0)
{stc("Nah, your too lazy.\n\r", ch);REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);return;}
/*
    if (ch->power[DISC_VAMP_OBFU] < 2)
    {
        send_to_char("You must obtain at least level 2 in Obfuscate to use Mortal Facade.\n\r",ch);
	return;
    }*/
	if (ch->level < 3)
	{
		send_to_char("You must train avatar first\n\r", ch);
		return;
	}
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
        if ( ch->pcdata->condition[COND_THIRST] < 1000 / ch->generation )
        {
	    send_to_char("You must be at full blood to use this power.\n\r",ch);
	    return;
    	}
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,PLR_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
		ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;
	    if (IS_EXTRA(ch, EXTRA_BAAL))
        {
            send_to_char("The spirit of Baal escapes you.\n\r",ch);  
		    ch->power[DISC_VAMP_POTE] -= 2;
            ch->power[DISC_VAMP_CELE] -= 2;
            ch->power[DISC_VAMP_FORT] -= 2;
            REMOVE_BIT(ch->extra, EXTRA_BAAL);
        }   
        if (IS_EXTRA(ch, EXTRA_FLASH))
        {
            send_to_char("Your speed slows.\n\r",ch);  
            ch->power[DISC_VAMP_CELE] -= 1;
            REMOVE_BIT(ch->extra, EXTRA_FLASH);
        }   
    	send_to_char("Colour returns to your skin and you warm up a little.\n\r",ch);
    	act("Colour returns to $n's skin.", ch, NULL, NULL, TO_ROOM);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You skin pales and cools.\n\r",ch);
    act("$n's skin pales slightly.", ch, NULL, NULL, TO_ROOM);
    ch->class=CLASS_VAMPIRE;
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_mortalvamp( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,PLR_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("You loose your vampire powers.\n\r",ch);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You regain your vampire powers.\n\r",ch);
    ch->class= CLASS_VAMPIRE;
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_shield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 2)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DROW)
	 && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_MONK)
	 && !IS_CLASS(ch, CLASS_AMAZON) && !IS_GOLEM(ch))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }
	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSHIELD))
	{  send_to_char("You don't have that power.\n\r", ch );
		return;}
    if (IS_CLASS(ch, CLASS_MONK) && !IS_NPC(ch) && ch->pcdata->powers[PMONK] < 2)
    {
	send_to_char("You must improve your Mantras to level 2 first!\n\r",ch);
	return;
    }
if (IS_GOLEM(ch) && ch->pcdata->powers[SYMANTICS] < 1)
{
    stc("You need symantics 2 to do that!\n\r", ch);
    return;
}
    if (IS_CLASS(ch, CLASS_AMAZON) && !IS_NPC(ch) && ch->pcdata->powers[PAMAZON] < 2)
    {
	send_to_char("You must improve your Drills to level 2 first!\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_OBFU] < 3 && ch->power[DISC_VAMP_DOMI] < 2 && IS_CLASS(ch, CLASS_VAMPIRE) &&
	!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON) )
    {
        send_to_char("You must obtain at least level 3 in Obfuscate or level 2 in Dominate.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 60)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
		ch->pcdata->condition[COND_THIRST] -= number_range(50,60);
    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
		return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}

void do_serpent( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_SERP] < 2)
    {
        send_to_char("You must obtain at least level 2 in Serpentis to use Serpent Form.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	if (!IS_POLYAFF(ch,POLY_SERPENT))
	{
	    send_to_char( "You cannot polymorph from this form.\n\r", ch );
	    return;
	}
	act( "You transform back into human.", ch, NULL, NULL, TO_CHAR );
	act( "$n transform into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->polyaff, POLY_SERPENT);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	clear_stats(ch);
	free_string( ch->morph );
	ch->morph = str_dup( "" );
	ch->max_hit = ch->max_hit - 250;
	ch->hit = ch->hit - 250;
	if (ch->hit < 1) ch->hit = 1;
	ch->max_mana = ch->max_mana + 50;
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
    if (ch->wpn[0] > 0)
    {
    	ch->hitroll += ch->wpn[0] * 0.75;
    	ch->damroll += ch->wpn[0] * 0.75;
    	ch->armor   -= ch->wpn[0] * 3;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    ch->pcdata->mod_str = 10;
    act( "You transform into a huge serpent.", ch, NULL, NULL, TO_CHAR );
    act( "$n transforms into a huge serpent.", ch, NULL, NULL, TO_ROOM );
    SET_BIT(ch->polyaff, POLY_SERPENT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf, "%s the huge serpent", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    ch->max_hit = ch->max_hit + 250;
    ch->hit = ch->hit + 250;
    ch->max_mana = ch->max_mana - 50;
    return;
}

void do_poison( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_SERP] < 3)
    {
        send_to_char("You must obtain at least level 3 in Serpentis to use Poison.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] < 5)
    {
	stc("You need ningenno to five to use that.\n\r", ch);
	return;
    }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
    {
	send_to_char( "You must wield the weapon you wish to poison.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 )
    {
	send_to_char( "This weapon cannot be poisoned.\n\r", ch );
	return;
    }

    if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 15 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
  if (IS_CLASS(ch, CLASS_VAMPIRE))
    ch->pcdata->condition[COND_THIRST] -= number_range(5,15);
    act("You run your tongue along $p, poisoning it.",ch,obj,NULL,TO_CHAR);
    act("$n runs $s tongue along $p, poisoning it.",ch,obj,NULL,TO_ROOM);
    obj->value[0] = 53;

    return;
}

void do_regenerate( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->rage > 0)
    {
	send_to_char("You cannot control your regenerative powers while the beast is so strong.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char("You cannot regenerate while fighting.\n\r",ch);
	return;
    }

    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
	send_to_char("But you are already completely regenerated!\n\r",ch);
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 5 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(10,20);
    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
    	send_to_char("Your body has completely regenerated.\n\r",ch);
    	act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    	send_to_char("Your body slowly regenerates itself.\n\r",ch);
    if (ch->hit < 1 )
    {
	ch->hit = ch->hit + 1;
	update_pos(ch);
	WAIT_STATE( ch, 6 );
    }
    else
    {
    	ch->hit = UMIN(ch->hit + 1050, ch->max_hit);
    	ch->mana = UMIN(ch->mana + 1050, ch->max_mana);
    	ch->move = UMIN(ch->move + 1050, ch->max_move);
	WAIT_STATE(ch,8);
        update_pos(ch);
    }
    return;
}

void werewolf_regen( CHAR_DATA *ch )
{
    int min = 1;
    int max = 25;

    if (IS_CLASS(ch, CLASS_MAGE) && ch->hit > 0) {
	smooregen(ch,number_range(335,445),number_range(175,315),number_range(235,245));
	return;
    }

    if (IS_NPC(ch)) return;

    if (ch->hit < 1 ) 
    {
		smooregen(ch,number_range(1,2),0,0);
		update_pos(ch);
    }
   else if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
		return;
    else
    {
		if (((ch->max_hit) > (ch->max_mana)) && ((ch->max_hit) > (ch->max_move)))
			max = ch->max_hit / 175;
		else if (((ch->max_move) > (ch->max_hit)) && ((ch->max_move) > (ch->max_mana)))
			max = ch->max_move / 175;
		else if (((ch->max_mana) > (ch->max_hit)) && ((ch->max_mana) > (ch->max_move)))
			max = ch->max_mana / 175;
		if (max < 25)
			max = 50;
smooregen(ch,number_range(min,max),number_range(min,max),number_range(min,max));
		if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
			send_to_char("Your body has completely regenerated itself.\n\r",ch);
    }
    return;
}

void mortal_regen( CHAR_DATA *ch )
{
    int min = 5;
    int max = 10;

    if (IS_NPC(ch)) return;

    if (ch->hit < 1 ) 
    {
		send_to_char( "You have been KILLED!!\n\r\n\r", ch );
		raw_kill(ch);
		return;
    }
    else if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
	{
		if (((ch->max_hit) > (ch->max_mana)) && ((ch->max_hit) > (ch->max_move)))
			max = ch->max_hit / 190;
		else if (((ch->max_move) > (ch->max_hit)) && ((ch->max_move) > (ch->max_mana)))
			max = ch->max_move / 190;
		else if (((ch->max_mana) > (ch->max_hit)) && ((ch->max_mana) > (ch->max_move)))
			max = ch->max_mana / 190;
		if (max < 25)
			max = 50;
	smooregen(ch,number_range(min,max),number_range(min,max),number_range(min,max));

	}
    return;
}

void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}

void vamp_rage( CHAR_DATA *ch )
{
    if (IS_NPC(ch)) return;

    if (ch->fighting)
	hurt_person(ch, ch->fighting, number_range(500,1900));

    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM);
    do_beastlike(ch,"");
    do_frenzy(ch,"");
    return;
}
/* Vampires can buy Golconda */
void do_beastdown( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
    int cost = EXP(ch,500000);

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;
	if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}	
	if (ch->exp < cost)
	{
		sprintf(log_buf, "You need %d exp to buy a point towards golconda.\n\r", cost);
		stc(log_buf, ch);
		return;
	}

	if (ch->beast == 0)
	{
		send_to_char("You have already attained golconda.\n\r", ch);
		return;
	}
	if (ch->beast > 99)
	{
		send_to_char("You cannot lower your beast anymore.\n\r", ch);
		return;
	}
	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && (ch->beast > 0) && (ch->beast < 100) && (ch->exp > 100000))
	{
		if (ch->beast == 1)
		{
		 send_to_char("You have attained Golconda!\n\r", ch);
		}
	else
	    send_to_char("You feel slightly more in control of your beast.\n\r", ch);
		ch->beast--;
    }
	ch->exp -= cost;
    return;
}/* End */

void do_daywalker(CHAR_DATA *ch, char *argument )
{
	char		arg [MAX_INPUT_LENGTH];
	argument = one_argument( argument, arg);

	if (IS_NPC(ch)) return;
	if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}	
	if (IS_IMMUNE(ch, IMM_SUNLIGHT))
	{
		send_to_char("You already walk with the mortals.\n\r", ch);
		return;
	}
	if (ch->beast > 0)
	{
		send_to_char("You must have achieved golconda before you can become a day walker.\n\r", ch);
		return;
	}
	if (ch->exp < EXP(ch,50000000))
	{
		sprintf(log_buf, "You need the %d required experience to become a daywalker.\n\r", EXP(ch,50000000));
		stc(log_buf, ch);
		return;
	}
	SET_BIT(ch->immune, IMM_SUNLIGHT);
	send_to_char("You now walk with the living", ch);
	ch->exp -= EXP(ch,50000000);
	return;
}
/* End */
void do_humanity( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast > 0
	&& ch->beast < 100 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast == 1)
	{
	    send_to_char("You have attained Golconda!\n\r", ch);
	    ch->exp += 1000000;
	}
	else
	    send_to_char("You feel slightly more in control of your beast.\n\r", ch);
	ch->beast -= 1;
    }
    return;
}

void do_beastlike( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
    int blood;

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast < 100
	&& ch->beast > 0 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast < 99)
	    send_to_char("You feel your beast take more control over your actions.\n\r", ch);
	else
	    send_to_char("Your beast has fully taken over control of your actions!\n\r", ch);
	ch->beast += 1;
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;
    	if ((IS_VAMPAFF(ch,VAM_PROTEAN) || (IS_VAMPAFF(ch,VAM_OBTENEBRATION)))
		&& !IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (!IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_PROTEAN) &&
		!IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	ch->pcdata->condition[COND_THIRST] = blood;
    }
    return;
}

void do_feed( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    char      bufch [MAX_INPUT_LENGTH];
    char      bufvi [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim))
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char("Blood does them no good at all.\n\r",ch);
	return;
    }
    if (!IS_IMMUNE(victim, IMM_VAMPIRE))
    {
	send_to_char("They refuse to drink your blood.\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 20 )
    {
	send_to_char("You don't have enough blood.\n\r",ch);
	return;
    }

    blood = number_range(10,20);
    ch->pcdata->condition[COND_THIRST] -= (blood * 2);
    victim->pcdata->condition[COND_THIRST] += blood;

    if (IS_AFFECTED(ch,AFF_POLYMORPH)) sprintf(bufch,ch->morph);
    else sprintf(bufch,ch->name);
    if (IS_AFFECTED(victim,AFF_POLYMORPH)) sprintf(bufvi,victim->morph);
    else sprintf(bufvi,victim->name);
    sprintf(buf,"You cut open your wrist and feed some blood to %s.",bufvi);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s cuts open $s wrist and feeds some blood to %s.",bufch,bufvi);
    act(buf, ch, NULL, victim, TO_NOTVICT);
    if (victim->position < POS_RESTING)
	send_to_char("You feel some blood poured down your throat.\n\r",victim);
    else
    {
	sprintf(buf,"%s cuts open $s wrist and feeds you some blood.",bufch);
	act(buf, ch, NULL, victim, TO_VICT);
    }
    return;
}

void do_upkeep( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    char      buf2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
if (IS_CLASS(ch, CLASS_DRUID)) {
stc(centerjob("*",">+-*Druid Rewards*-+<", 70-21), ch);;
stc("\n\r--------------------------------------------------------------------------\n\r", ch);
if (bit_count(ch->warp, 50) == 0) {
stc("You contain no rewards from the gods of nature.\n\r", ch);
return;} else {
if (IS_WARP(ch, REWARD_SILVERARMOR))
stc("Your armor seems to turn to silver when you wear it.\n\r", ch);
if (IS_WARP(ch, REWARD_SOFTBODY))
stc("You get hurt by the weakest pinches.\n\r", ch);
if (IS_WARP(ch, REWARD_SWIFTNESS))
stc("You move at a lightning speed.\n\r", ch);
if (IS_WARP(ch, REWARD_SLOW))
stc("Your body moves as slow as a snail.\n\r", ch);
if (IS_WARP(ch, REWARD_INFEST))
stc("You have an enhanced relationship with spiders.\n\r", ch);
if (IS_WARP(ch, REWARD_MAGICAL))
stc("You are one with the magical side of the world.\n\r", ch);
if (IS_WARP(ch, REWARD_PEACEFUL))
stc("You don't like to kill, except in self defence.\n\r", ch);
if (IS_WARP(ch, REWARD_DEADLYSTAFF))
stc("Your are more skilled with your staff than the deadliest weaponsmasters.\n\r", ch);
if (IS_WARP(ch, REWARD_VULNERABLE))
stc("You can't hold up in a fight too well.\n\r", ch);
if (IS_WARP(ch, REWARD_RESTRICTED))
stc("The gods have restricted your druid powers.\n\r", ch);
if (IS_WARP(ch, REWARD_SPACE))
stc("The gods have given you nothing.\n\r", ch);
if (IS_WARP(ch, REWARD_IRONLEGS))
stc("Your legs are made from iron.\n\r", ch);
if (IS_WARP(ch, REWARD_WEAPONSMASTER))
stc("You are a master of all deadly weapons.\n\r", ch);
if (IS_WARP(ch, REWARD_RICH))
stc("You are rich, and not greedy.\n\r", ch);
if (IS_WARP(ch, REWARD_CLUMSY))
stc("You topple over when you walk, you clumsy fool.\n\r", ch);
if (IS_WARP(ch, REWARD_UNALIGN))
stc("You have been aligned with darkness.\n\r", ch);
if (IS_WARP(ch, REWARD_THORNS))
stc("You have deadly thorns sticking out from your body.\n\r", ch);
if (IS_WARP(ch, REWARD_WARTS))
stc("You have warts showing all over your skin.\n\r", ch);
if (IS_WARP(ch, REWARD_HELPFUL))
stc("You share your powers with those around you.\n\r", ch);
if (IS_WARP(ch, REWARD_WELFARE))
stc("You are in welfare, and are being payed by the rich.\n\r", ch);
if (IS_WARP(ch, REWARD_CHEF))
stc("You are a good with preparing foods.\n\r", ch);
if (IS_WARP(ch, REWARD_REJECT))
stc("You hate druids. You don't want to communicate with any of them.\n\r", ch);
if (IS_WARP(ch, REWARD_LONELY))
stc("You get lonely. You like to stay behind on trips:\\\n\r", ch);
}
}

    else if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                              -= Demonic powers =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);

	cent_to_char("Demons don't have powers now. Laf.\n\r",ch);
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                              -= Vampire upkeep =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("Staying alive...upkeep 1.\n\r",ch);

    	if (IS_VAMPAFF(ch,VAM_DISGUISED) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are disguised as %s...no upkeep.\n\r",ch->morph);
	    else if (ch->beast == 100) sprintf(buf,"You are disguised as %s...upkeep 10-20.\n\r",ch->morph);
	    else sprintf(buf,"You are disguised as %s...upkeep 5-10.\n\r",ch->morph);
	    send_to_char(buf,ch);
	}
    	if (IS_IMMUNE(ch,IMM_SHIELDED) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are shielded...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shielded...upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shielded...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    else sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_FANGS) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have your fangs out...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your fangs out...upkeep 2.\n\r");
	    else sprintf(buf,"You have your fangs out...upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CLAWS) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have your claws out...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your claws out...upkeep 2-6.\n\r");
	    else sprintf(buf,"You have your claws out...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have nightsight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have nightsight...upkeep 2.\n\r");
	    else sprintf(buf,"You have nightsight...upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,PLR_SHADOWSIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have shadowsight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shadowsight...upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shadowsight...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have truesight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have truesight...upkeep 2-10.\n\r");
	    else sprintf(buf,"You have truesight...upkeep 1-5.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CHANGED) )
	{
	    if      (IS_POLYAFF(ch,POLY_BAT))  sprintf(buf2,"bat" );
	    else if (IS_POLYAFF(ch,POLY_WOLF)) sprintf(buf2,"wolf");
	    else                               sprintf(buf2,"mist");
	    if      (ch->beast == 0  ) sprintf(buf,"You have changed into %s form...no upkeep.\n\r",buf2);
	    else if (ch->beast == 100) sprintf(buf,"You have changed into %s form...upkeep 10-20.\n\r",buf2);
	    else sprintf(buf,"You have changed into %s form...upkeep 5-10.\n\r",buf2);
	    send_to_char(buf,ch);
	}
    	if (IS_POLYAFF(ch,POLY_SERPENT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are in serpent form...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in serpent form...upkeep 6-8.\n\r");
	    else sprintf(buf,"You are in serpent form...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
       	}
        if (IS_POLYAFF(ch,POLY_ZULOFORM))
        {
        send_to_char("You are in Zuloform.\n\r",ch);
        if (IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You are a huge dragon.\n\r",ch);
        return;
        }
        }
        if (IS_EXTRA(ch, EXTRA_AWE))
        {
        send_to_char("You are awe inspiring.\n\r",ch);
        }
if (IS_AFFECTED(ch, AFF_PEACE)) send_to_char("Your majesty is up.\n\r",ch);
if (IS_SET(ch->newbits, NEW_MONKFLAME)) send_to_char("Your hands and weapon are engulfed in flames.\n\r",ch);       
if (IS_EXTRA(ch, EXTRA_PLASMA)) send_to_char("You are in plasmaform.\n\r",ch);     
if (ch->power[DISC_VAMP_PROT] > 8) send_to_char("Your flesh is as hard as marble.\n\r",ch);
if (IS_SET(ch->newbits, NEW_SKIN)) send_to_char("You are surrounded by a serpent's scales.\n\r",ch);
if (ch->rage > 0) send_to_char("You are in frenzy.\n\r",ch);
if (IS_SET(ch->act, PLR_WIZINVIS)) send_to_char("You are blended with the Shade.\n\r",ch);
if (IS_EXTRA(ch, EXTRA_POTENCY)) send_to_char("Your blood is more potent then usual.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HEAD)) send_to_char("You have the head of a lion.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_TAIL)) send_to_char("A long tail protrudes from your backside.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_EXOSKELETON)) send_to_char("Your body is covered by a bony exoskeleton.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HORNS)) send_to_char("Large horns protrude from your head.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_WINGS)) send_to_char("Large wings protrude from your back.\n\r",ch);
}
else if (IS_CLASS(ch,CLASS_WEREWOLF))
{
send_to_char("-------------------------------------------------------------------------------\n\r",ch);
send_to_char(
"                          Werewolf Upkeep\n\r",ch);
send_to_char("-------------------------------------------------------------------------------\n\r",ch);
if (ch->rage>100)
send_to_char("You are raged into a werewolf.\n\r",ch);
sprintf(buf,"You have %d points of tolerance against silver.\n\r",ch->siltol);
send_to_char(buf,ch);
if (IS_SET(ch->newbits,NEW_REND))
send_to_char("You are focused on rending your opponents body.\n\r",ch);
if (IS_SET(ch->newbits,NEW_SLAM))
send_to_char("You will stun and strike your victim when he is down.\n\r",ch);
if (IS_SET(ch->newbits,NEW_QUILLS))
send_to_char("Razor sharp quills are extended from your back.\n\r",ch);
if (ch->power[DISC_WERE_RAPT] > 0)
send_to_char("Your fangs are much sharper then usual.\n\r",ch);
if (IS_SET(ch->newbits,NEW_JAWLOCK))
send_to_char("Your jaw is strengthened to clamp on your foes.\n\r",ch);
if (IS_SET(ch->newbits,NEW_SKIN))
send_to_char("Your skin is very tough, making it hard to slash and pierce.\n\r",ch);
if (IS_SET(ch->newbits,NEW_PERCEPTION))
send_to_char("You can sense the slightest movement.\n\r",ch);
if (ch->power[DISC_WERE_BOAR] > 2)
send_to_char("You are extra tough, halving damage.\n\r",ch);

send_to_char("-------------------------------------------------------------------------------\n\r",ch);
return;
}

    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

void do_vclan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) < 2 || IS_SET(ch->special, SPC_ANARCH) )
    {
	send_to_char("But you don't belong to any clan!\n\r",ch);
	return;
    }

    sprintf( buf, "The %s clan:\n\r", ch->clan );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Gen ] [ Hits   ] [ Mana   ] [ Move   ] [  Exp  ] [Blood]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_VAMPIRE) ) continue;
	if ( !str_cmp(ch->clan,gch->clan) )
	{
	    sprintf( buf,
	    "[%-16s] [  %d  ] [%-6d%3d] [%-6d%3d] [%-6d%3d] [%7ld] [ %3d ]\n\r",
		capitalize( gch->name ),
		gch->generation,
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move),
		gch->exp, gch->pcdata->condition[COND_THIRST] );
		send_to_char( buf, ch );
	}
    }
    return;
}

void do_tribe( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char clan[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( strlen(ch->clan) > 20)
    {
	send_to_char("You need no tribe, foul spawn of the wyrm.\n\r",ch);
	return;
    }
    send_to_char("&D[#n      Name     &D] [#n    Tribe   &D] [#n Hits  &D] [#n Mana  &D ] [#n Move &D] [#n Exp &D ]#n\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_WEREWOLF) ) continue;
	if ( strlen(gch->clan) > 20) continue;
	if ( gch->clan != NULL && strlen(gch->clan) > 1 )
	    sprintf(clan,gch->clan);
	else if ( gch->generation == 1 )
	    sprintf(clan,"All");
	else
	    sprintf(clan,"None");
	{
	    sprintf( buf,
	    "[%-16s] [%-13s] [%-6d%3d] [%-6d%3d] [%-6d%3d] [%7ld]\n\r",
		capitalize( gch->name ), clan,
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move),
		gch->exp);
		send_to_char( buf, ch );
	}
    }
    return;
}


void do_werewolf( CHAR_DATA *ch, char *argument )
{
    char       buf[MAX_INPUT_LENGTH];
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (IS_SET(ch->special, SPC_WOLFMAN)) return;
    SET_BIT(ch->special, SPC_WOLFMAN);

    send_to_char("You throw back your head and howl with rage!\n\r",ch);
    act("$n throws back $s head and howls with rage!.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Coarse dark hair sprouts from your body.\n\r",ch);
    act("Coarse dark hair sprouts from $n's body.",ch,NULL,NULL,TO_ROOM);
    if (!IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("Your eyes start glowing red.\n\r",ch);
	act("$n's eyes start glowing red.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("A pair of long fangs extend from your mouth.\n\r",ch);
	act("A pair of long fangs extend from $n's mouth.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if ((obj = get_eq_char(ch,WEAR_WIELD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON))
    {
	act("$p drops from your right hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's right hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON))
    {
	act("$p drops from your left hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's left hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    sprintf(buf,"%s the werewolf",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->rage += 25;
    if (ch->power[DISC_WERE_WOLF] > 3) ch->rage += 100;
    if (ch->rage > 300) 
	ch->rage = 300;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( ch == vch )
	{
	    act("You throw back your head and howl with rage!", ch, NULL, NULL, TO_CHAR);
	    continue;
	}
	if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
	    continue;
	if (!IS_NPC(vch))
	{
	    if (vch->in_room == ch->in_room)
		act("$n throws back $s head and howls with rage!", ch, NULL, vch, TO_VICT);
	    else if (vch->in_room->area == ch->in_room->area)
		send_to_char("You hear a fearsome howl close by!\n\r", vch);
	    else
/*		send_to_char("You hear a fearsome howl far off in the 
   distance!\n\r", vch);*/
	    if (!CAN_PK(vch)) continue;
	}
	if ( vch->in_room == ch->in_room && can_see( ch, vch) )
	{
		if (is_safe(ch, vch)) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	}
    }
    return;
}

void do_unwerewolf( CHAR_DATA *ch, char *argument )
{
    char       arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (!IS_SET(ch->special, SPC_WOLFMAN)) return;
    REMOVE_BIT(ch->special, SPC_WOLFMAN);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string(ch->morph);
    ch->morph = str_dup("");
    if (IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Your talons slide back into your fingers.\n\r",ch);
	act("$n's talons slide back into $s fingers.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if (IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("Your fangs slide back into your mouth.\n\r",ch);
	act("$n's fangs slide back into $s mouth.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    send_to_char("Your coarse hair shrinks back into your body.\n\r",ch);
    act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM);
    ch->rage -= 25;
    if (ch->rage < 0) ch->rage = 0;
    return;
}

void do_favour( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) ) return;

    /*if ( !IS_CLASS(ch,CLASS_VAMPIRE) || !IS_SET(ch->special,SPC_PRINCE) )*/
/* Uh, 2nd generations are the only vamps that should be favouring, not
   the only vamps that are excluded.
|| ch->generation <= 2) )
*/
	if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
/*
	if (!IS_SET(ch->special, SPC_PRINCE) && (ch->generation > 2))
	{
		send_to_char("You are neither a prince nor a founder", ch);
		return;
	}
*/
    if (( arg1[0] == '\0' || arg2[0] == '\0' ) && (ch->generation > 2))
    {
	send_to_char( "Syntax is: favour <target> <induct/accept/sire>\n\r", ch );
	send_to_char("Favour self leave to leave clan.\r\n", ch);
	return;
    }

    if (( arg1[0] == '\0' || arg2[0] == '\0' ) && (ch->generation < 3))
    {
	send_to_char( "Syntax is: favour <target> <prince/induct/accept/sire>\n\r", ch );
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
    if (ch->generation > 2 && ch==victim && !str_cmp(arg2,"leave") )
    {
	send_to_char("You leave your clan!\r\n", ch);
	free_string(ch->clan);
	ch->clan = str_dup("");
	return;
    }
    if (ch->generation > 2 && !IS_SET(ch->special, SPC_PRINCE))
    {
	send_to_char("You are not a prince or a founder!\r\n", ch);
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }

    if ( !IS_CLASS(victim,CLASS_VAMPIRE) )
    {
	send_to_char( "But they are not a vampire!\n\r", ch );
	return;
    }

    if ( str_cmp(victim->clan,ch->clan) && str_cmp(arg2,"induct") )
    {
	send_to_char( "You can only grant your favour to someone in your clan.\n\r", ch );
	return;
    }

    if ( ch->generation >= victim->generation )
    {
	send_to_char( "You can only grant your favour to someone of a lower generation.\n\r", ch );
	return;
    }

    if ( !str_cmp(arg2,"prince") && ch->generation == 2)
    {
	/* remove SPC_PRINCE */
		if (IS_SET(victim->special,SPC_PRINCE)) 
		{
			act("You remove $N's prince privilages!",ch,NULL,victim,TO_CHAR);
			act("$n removes $N's prince privilages!",ch,NULL,victim,TO_NOTVICT);
			act("$n removes your prince privilages!",ch,NULL,victim,TO_VICT);
			if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
				REMOVE_BIT(victim->special,SPC_PRINCE);
			return;
		}

	/* set SPC_PRINCE */
		act("You make $N a prince!",ch,NULL,victim,TO_CHAR);
		act("$n has made $N a prince!",ch,NULL,victim,TO_NOTVICT);
		act("$n has made you a prince!",ch,NULL,victim,TO_VICT);
		SET_BIT(victim->special,SPC_PRINCE);
		if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
			return;
	}
    else if ( !str_cmp(arg2,"sire") && (ch->generation == 2 || IS_SET(ch->special,SPC_PRINCE)))
    {
	/* remove SPC_SIRE */
		if (IS_SET(victim->special,SPC_SIRE)) {
		act("You remove $N's permission to sire a childe!",ch,NULL,victim,TO_CHAR);
		act("$n has removed $N's permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
		act("$n has remove your permission to sire a childe!",ch,NULL,victim,TO_VICT);
		REMOVE_BIT(victim->special,SPC_SIRE);
		return;}
	/* set SPC_SIRE */
	act("You grant $N permission to sire a childe!",ch,NULL,victim,TO_CHAR);
	act("$n has granted $N permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
	act("$n has granted you permission to sire a childe!",ch,NULL,victim,TO_VICT);
	SET_BIT(victim->special,SPC_SIRE);
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->generation > 2 && 
	ch->generation == 2)
    {
	act("You make $N a Caitiff!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a Caitiff!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a Caitiff!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->generation > 2 && 
	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE))
    {
	act("You make $N a Caitiff!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a Caitiff!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a Caitiff!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 
	(ch->generation == 2 || IS_SET(ch->special,SPC_PRINCE)) && strlen(victim->clan) < 2)
    {
	if (IS_SET(victim->special, SPC_ANARCH))
	{
	    send_to_char("You cannot induct an Anarch!\n\r",ch);
	    return;
	}
	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->generation > 2 && 
	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE) &&
	strlen(victim->clan) < 2)
    {
	if (IS_SET(victim->special, SPC_ANARCH))
	{
	    send_to_char("You cannot induct an Anarch!\n\r",ch);
	    return;
	}
	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"accept") &&
	(ch->generation == 2 || IS_SET(ch->special,SPC_PRINCE)))
    {
	if ( victim->pcdata->rank > AGE_CHILDE)
	{ send_to_char("But they are not a childe!\n\r",ch); return; }
	act("You accept $N into the clan!",ch,NULL,victim,TO_CHAR);
	act("$n has accepted $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n accepted you into $s clan!",ch,NULL,victim,TO_VICT);
	victim->pcdata->rank = AGE_NEONATE;
	return;
    }
    else send_to_char( "You are unable to grant that sort of favour.\n\r", ch );
    return;
}

void do_possession( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

/* kavirpoint 
	send_to_char("This ability has been temporarily disabled.\n\r",ch);
	return;
*/
    if (ch->power[DISC_VAMP_DOMI] < 3)
    {
        send_to_char("You must obtain at least level 3 in Dominate to use Possession.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	sprintf(buf,"You break your hold over %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to Possess?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to possess them.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= 50;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}

void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_fcommand( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;
/*
    if ((!IS_CLASS(ch, CLASS_VAMPIRE)  ch->level < LEVEL_APPRENTICE) ||
!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
*/
    if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->power[DISC_VAMP_DOMI] < 3) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }

    if ( ( victim = ch->pcdata->familiar ) == NULL )
    {
	send_to_char( "But you don't have a familiar!\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar do?\n\r", ch );
	return;
    }
    interpret(victim,argument);
    return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 1)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	send_to_char( "You slowly fade into existance.\n\r", ch );
	act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
	send_to_char( "You slowly fade out of existance.\n\r", ch );
	act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->act, PLR_WIZINVIS);
    }
    return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{
    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM);
    if (IS_NPC(ch)) return;

    if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
{
act("The tendrils entrapping $n break away.",ch,NULL,NULL,TO_ROOM);
send_to_char("The tendrils entrapping you break away.\n\r",ch);
REMOVE_BIT(ch->affected_by, AFF_TENDRILS);
}

    WAIT_STATE(ch,12);
    return;
}

void do_rage( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
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
	ch->rage += 400;
	ch->move -= 1000; 
	WAIT_STATE(ch,12);
	return;
  }
/* End */
  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {

    if (ch->power[DISC_WERE_BEAR] < 3)
    {
		stc("You need to obtain level three in the Bear totem.\n\r",ch);
		return;
    }

    if (!IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You start snarling angrilly.\n\r",ch);
	act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);
	ch->rage += number_range(15,30);
	if (ch->rage >= 100) do_werewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are already in a rage!\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (ch->power[DISC_DAEM_ATTA] < 3)
        {
	    stc("You try, but cannot summon your beast.\n\r",ch);
	    return;
	}

     if (ch->beast < 1)
	ch->beast = 1;

        if (ch->beast > ch->rage || ch->rage < 25)
	{
	    ch->rage += number_range(ch->generation,25);
	    stc("You scream as you plunge into a mad attacking frenzy.\n\r",ch);
	    act("$n screams in frenzied rage.",ch,NULL,NULL,TO_ROOM);
	    if (number_percent() < (ch->beast + 1))
	    {
		stc("You feel the beast take more control.\n\r",ch);
		ch->beast += 1;
		if (ch->beast > 100)
		  ch->beast = 100;
	    }
	    WAIT_STATE(ch, 12);
	    return;
	}
       
        stc("You have reached the pinnacle of your frenzy.\n\r",ch);
    }

    return;
}

void do_calm( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/* Ninja calming technique */
	if (IS_CLASS(ch, CLASS_NINJA))
	{
		if (ch->rage <= 0)
		{
			ch->rage = 0;
			send_to_char("You are not in a state of michi to be calmed.\n\r", ch);
			return;
		}
		send_to_char("You release some of the power of the michi back into the ground.\n\r", ch);
		act("$n releases some of the force of the michi back into the ground.", ch, NULL, NULL, TO_ROOM);
		ch->rage -= number_range(20,40);
		WAIT_STATE(ch,6);
		return;
	}
/* End */
	else if (IS_CLASS(ch, CLASS_VAMPIRE) )
	{
		if (ch->rage <= 0)
		{
			ch->rage = 0;
			send_to_char("You are not in a state of frenzy right now.\n\r", ch);
			return;
		}
		send_to_char("You take a deep breath and force your beast back.\n\r", ch);
		act("$n takes a deep breath and forces their beast back", ch, NULL, NULL, TO_ROOM);
		ch->rage -= number_range(5,10);
		WAIT_STATE(ch,3);
		return;
	}
	else if (IS_CLASS(ch, CLASS_WEREWOLF))
	{
	    if (ch->power[DISC_WERE_WOLF] < 3)
		{
			stc("You need level three in the Wolf totem to Calm.\n\r",ch);
			return;
		}  
		if (IS_SET(ch->special, SPC_WOLFMAN))
		{
			send_to_char("You take a deep breath and calm yourself.\n\r",ch);
			act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
			ch->rage -= number_range(15,30);
			if (ch->rage < 100) do_unwerewolf(ch,"");
				WAIT_STATE(ch,12);
			return;
		}
		send_to_char("But you are not in crinos form!\n\r",ch);
	}

   else if (IS_CLASS(ch, CLASS_DEMON))
   {

    if (ch->power[DISC_DAEM_ATTA] < 4)
    {
	stc("You need level four in Attack to calm your frenzy.\n\r",ch);
	return;
    }

    if (ch->beast >= 100) ch->beast = 100;
    if (ch->beast <= 0) ch->beast = 0;

    ch->rage -= (40 - (ch->beast / 3) +dice (1, 10));
    if (ch->rage <= 0)
    {
	ch->rage = 0;
	send_to_char("You have calmed out of your rage completely.\n\r", ch);
    }
    else
    {
      send_to_char("You hold your breath and try to calm your frenzy.\n\r", ch);
      WAIT_STATE(ch, 8);
    }
    return;
  } 
    return;
}

void do_totems( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    send_to_char("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Totems: Bear (%d), Lynx (%d), Boar (%d), Owl (%d), Spider (%d), Wolf (%d),
        Hawk (%d), Mantis (%d), Raptor (%d), Luna (%d), Pain (%d), Congregation (%d)\n\r",
	ch->power[DISC_WERE_BEAR], ch->power[DISC_WERE_LYNX],
	ch->power[DISC_WERE_BOAR], ch->power[DISC_WERE_OWL],
	ch->power[DISC_WERE_SPID], ch->power[DISC_WERE_WOLF],
	ch->power[DISC_WERE_HAWK],ch->power[DISC_WERE_MANT],
        ch->power[DISC_WERE_RAPT],ch->power[DISC_WERE_LUNA],
ch->power[DISC_WERE_PAIN],ch->power[DISC_WERE_CONG]);

	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"bear"))
	{
	    send_to_char("Bear: The totem of strength and aggression.\n\r",ch);
	    if (ch->power[DISC_WERE_BEAR] < 1) 
		send_to_char("You have none of the Bear totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_BEAR] > 0) 
		send_to_char("FLEX: You strength is so great that no ropes can hold you.\n\r",ch);
	    if (ch->power[DISC_WERE_BEAR] > 1) 
		send_to_char("RAGE: You are able to build yourself up a rage at will.\n\r",ch);
	    if (ch->power[DISC_WERE_BEAR] > 2) 
		send_to_char("Steel claws: Your claws are so tough that they can parry weapons.\n\r",ch);
	    if (ch->power[DISC_WERE_BEAR] > 3) 
		send_to_char("Hibernation: Your wounds heal at amazing speeds when you sleep.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"boar"))
	{
	    send_to_char("Boar: The totem of toughness and perserverance.\n\r",ch);
	    if (ch->power[DISC_WERE_BOAR] < 1) 
		send_to_char("You have none of the Boar totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_BOAR] > 0) 
		send_to_char("Shatter: No door is sturdy enough to resist you.\n\r",ch);
	    if (ch->power[DISC_WERE_BOAR] > 1) 
		send_to_char("CHARGE: Your first blow in combat has a +50 damage bonus.\n\r",ch);
	    if (ch->power[DISC_WERE_BOAR] > 2) 
		send_to_char("Toughness: Your skin is extremely tough. You take half damage in combat.\n\r",ch);
	    if (ch->power[DISC_WERE_BOAR] > 3) 
		send_to_char("Immovability: You are able to shrug off blows that would knock out most people.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"lynx"))
	{
	    send_to_char("Lynx: The totem of speed and agility.\n\r",ch);
	    if (ch->power[DISC_WERE_LYNX] < 1) 
		send_to_char("You have none of the Lynx totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_LYNX] > 0) 
		send_to_char("Light footed: You move so lightly that you leave no tracks behind you.\n\r",ch);
	    if (ch->power[DISC_WERE_LYNX] > 1) 
		send_to_char("Stalker: You are able hunt people with much greater speed than normal.\n\r",ch);
	    if (ch->power[DISC_WERE_LYNX] > 2) 
		send_to_char("Combat speed: You have an extra attack in combat.\n\r",ch);
	    if (ch->power[DISC_WERE_LYNX] > 3) 
		send_to_char("Lightning Claws: Yours claws parry blows with lightning fast speed.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Owl"))
	{
	    send_to_char("Owl: The totem of thought and spiritualism.\n\r",ch);
	    if (ch->power[DISC_WERE_OWL] < 1) 
		send_to_char("You have none of the Owl totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_OWL] > 0) 
		send_to_char("VANISH: You are able to conceal yourself from all but the most perceptive.\n\r",ch);
	    if (ch->power[DISC_WERE_OWL] > 1) 
		send_to_char("SHIELD: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
	    if (ch->power[DISC_WERE_OWL] > 2) 
		send_to_char("SHADOWPLANE: You are able to enter the shadow plane.\n\r",ch);
	    if (ch->power[DISC_WERE_OWL] > 3) 
		send_to_char("Magical Control: You are able to fully control your magic in crinos form.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Spider"))
	{
	    send_to_char("Spider: The totem of ambush and cunning.\n\r",ch);
	    if (ch->power[DISC_WERE_SPID] < 1) 
		send_to_char("You have none of the Spider totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_SPID] > 0) 
		send_to_char("Poisonous bite: Your bite injects your opponents with a deadly venom.\n\r",ch);
	    if (ch->power[DISC_WERE_SPID] > 1) 
		send_to_char("WEB: You are able to shoot a web at your opponents to entrap them.\n\r",ch);
	    if (ch->power[DISC_WERE_SPID] > 2) 
		send_to_char("Immunity to poison: Poisons have no affect upon you.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Wolf"))
	{
	    send_to_char("Wolf: Controlling your innate wolf powers.\n\r",ch);
	    if (ch->power[DISC_WERE_WOLF] < 1) 
		send_to_char("You have none of the Wolf totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_WOLF] > 0) 
		send_to_char("CLAWS: You can extend or retract your claws at will.\n\r",ch);
	    if (ch->power[DISC_WERE_WOLF] > 1) 
		send_to_char("FANGS: You can extend or retract your fangs at will.\n\r",ch);
	    if (ch->power[DISC_WERE_WOLF] > 2) 
		send_to_char("CALM: You are able to repress your inner beast at will.\n\r",ch);
	    if (ch->power[DISC_WERE_WOLF] > 3) 
		send_to_char("Spirit of Fenris: You are able to enter rage faster than normal.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Hawk"))
	{
	    send_to_char("Hawk: The totem of vision and perception.\n\r",ch);
	    if (ch->power[DISC_WERE_HAWK] < 1) 
		send_to_char("You have none of the Wolf totem powers.\n\r",ch);
	    if (ch->power[DISC_WERE_HAWK] > 0) 
		send_to_char("NIGHTSIGHT: You can see perfectly well in the dark.\n\r",ch);
	    if (ch->power[DISC_WERE_HAWK] > 1) 
		send_to_char("SHADOWSIGHT: You can see into the plane of shadows.\n\r",ch);
	    if (ch->power[DISC_WERE_HAWK] > 2) 
		send_to_char("TRUESIGHT: You have perfect vision.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Mantis"))
	{
	   send_to_char("Mantis: The totem of dexterity and reflexes.\n\r",ch);
	    if (ch->power[DISC_WERE_MANT] < 1) 
		send_to_char("You have none of the Mantis totem powers.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 9) 
		send_to_char("Insanely fast attacks: Your opponents get -50 to parry and -100 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 8) 
		send_to_char("Insanely fast attacks: Your opponents get -45 to parry and -90 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 7) 
		send_to_char("Insanely fast attacks: Your opponents get -40 to parry and -80 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 6) 
		send_to_char("Insanely fast attacks: Your opponents get -35 to parry and -70 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 5) 
		send_to_char("Insanely fast attacks: Your opponents get -30 to parry and -60 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 4) 
		send_to_char("Insanely fast attacks: Your opponents get -25 to parry and -50 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 3) 
		send_to_char("Incredibly fast attacks: Your opponents get -20 to parry and -40 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 2) 
		send_to_char("Extremely fast attacks: Your opponents get -15 to parry and -30 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 1) 
		send_to_char("Very fast attacks: Your opponents get -10 to parry and -20 to dodge.\n\r",ch);
	    else if (ch->power[DISC_WERE_MANT] > 0) 
		send_to_char("Fast attacks: Your opponents get -5 to parry and -10 to dodge.\n\r",ch);
	    return;
	}
	sprintf(buf,"Totems: Bear (%d), Lynx (%d), Boar (%d), Owl (%d),
Spider (%d), Wolf (%d),\n\r        Hawk (%d), Mantis (%d),
Raptor(%d), Luna (%d), Pain (%d), Congregation(%d)\n\r",
	ch->power[DISC_WERE_BEAR], ch->power[DISC_WERE_LYNX],
	ch->power[DISC_WERE_BOAR], ch->power[DISC_WERE_OWL],
	ch->power[DISC_WERE_SPID], ch->power[DISC_WERE_WOLF],
        ch->power[DISC_WERE_HAWK],ch->power[DISC_WERE_MANT],
	ch->power[DISC_WERE_RAPT],ch->power[DISC_WERE_LUNA],
ch->power[DISC_WERE_PAIN],ch->power[DISC_WERE_CONG]);        
        send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"bear"   )) {improve = DISC_WERE_BEAR;max=10;}
	else if (!str_cmp(arg1,"boar"   )) {improve = DISC_WERE_BOAR;max=10;}
	else if (!str_cmp(arg1,"lynx"   )) {improve = DISC_WERE_LYNX;max=10;}
	else if (!str_cmp(arg1,"owl"    )) {improve = DISC_WERE_OWL;max=10;}
	else if (!str_cmp(arg1,"spider" )) {improve = DISC_WERE_SPID;max=10;}
	else if (!str_cmp(arg1,"wolf"   )) {improve = DISC_WERE_WOLF;max=10;}
	else if (!str_cmp(arg1,"hawk"   )) {improve = DISC_WERE_HAWK;max=10;}
	else if (!str_cmp(arg1,"mantis" )) {improve = DISC_WERE_MANT;max=10;}
	else if (!str_cmp(arg1,"raptor" )) {improve = DISC_WERE_RAPT;max=10;}
        else if (!str_cmp(arg1,"luna"   )) {improve = DISC_WERE_LUNA;max=10;}
        else if (!str_cmp(arg1,"pain"   )) {improve = DISC_WERE_PAIN;max=10;}
        else if (!str_cmp(arg1,"congregation")){improve=DISC_WERE_CONG;max=10;}
 
      else
	{
	    send_to_char("You can improve: Bear, Boar, Lynx, Owl, Spider, Wolf, Hawk, 
Mantis, Raptor, Pain, Luna, Congregation\n\r",ch);
	    return;
	}
	cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the powers of the %s totem.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your %s totem.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You improve your ability in the %s totem.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a totem, type: Totem <totem type> improve.\n\r",ch);
    return;
}

void do_web( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ((!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW)))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	if ((IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW) && ch->power[DISC_WERE_SPID] < 2))
	{
		send_to_char("You do not have the spider totem to level 2 to use web.\n\r", ch);
		return;
	}
	if ( (IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_SET(ch->pcdata->powers[1], DPOWER_WEB) ))
	{
		send_to_char("You have not been granted the web power.\n\r", ch);
		return;
	}
/* End */
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot web yourself.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot web an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

void do_birth( CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_EXTRA(ch, EXTRA_PREGNANT))
    {
	send_to_char("But you are not even pregnant!\n\r",ch);
	return;
    }

    if (!IS_EXTRA(ch, EXTRA_LABOUR))
    {
	send_to_char("You're not ready to give birth yet.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	if (ch->pcdata->genes[4] == SEX_MALE)
	    send_to_char( "What do you wish to name your little boy?\n\r", ch );
	else if (ch->pcdata->genes[4] == SEX_FEMALE)
	    send_to_char( "What do you wish to name your little girl?\n\r", ch );
	else
	    send_to_char( "What do you wish to name your child?\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( char_exists(FALSE,argument) )
    {
	send_to_char( "That player already exists.\n\r", ch );
	return;
    }

    strcpy(buf2,ch->pcdata->cparents);
    strcat(buf2," ");
    strcat(buf2,argument);
    if (!birth_ok(ch, buf2))
    {
	send_to_char( "Bug - please inform KaVir.\n\r", ch );
	return;
    }
    argument[0] = UPPER(argument[0]);
    birth_write( ch, argument );
    ch->pcdata->genes[9] += 1;
    REMOVE_BIT(ch->extra, EXTRA_PREGNANT);
    REMOVE_BIT(ch->extra, EXTRA_LABOUR);
    save_char_obj(ch);
    return;
}

bool birth_ok( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char mum [MAX_INPUT_LENGTH];
    char dad [MAX_INPUT_LENGTH];
    char child [MAX_INPUT_LENGTH];

    argument = one_argument( argument, mum );
    argument = one_argument( argument, dad );
    argument = one_argument( argument, child );

    if (dad[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    if (child[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    dad[0] = UPPER(dad[0]);
    if (!str_cmp(dad,"Kavir")) strcpy(dad,"KaVir");
    child[0] = UPPER(child[0]);
    if (ch->pcdata->genes[4] == SEX_MALE)
    {
	send_to_char("You give birth to a little boy!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's son, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    else if (ch->pcdata->genes[4] == SEX_FEMALE)
    {
	send_to_char("You give birth to a little girl!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's daughter, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    return FALSE;
}


bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    if (backup)
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    else
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
	found = TRUE;
	fclose( fp );
    }
    return found;
}

void birth_write( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    char *strtime;

    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';

    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen(buf, "w") ) != NULL) 
    {  
	fprintf( fp, "#PLAYER\n");
	fprintf( fp, "Name         %s~\n",capitalize( argument ));
	fprintf( fp, "ShortDescr   ~\n"				);
	fprintf( fp, "LongDescr    ~\n"				);
	fprintf( fp, "Description  ~\n"				);
	fprintf( fp, "Lord         ~\n"				);
	fprintf( fp, "Clan         ~\n"				);
	fprintf( fp, "Morph        ~\n"				);
	fprintf( fp, "Createtime   %s~\n",str_dup( strtime )	);
 	fprintf( fp, "Lasttime     ~\n"				);
	fprintf( fp, "Lasthost     ~\n"				);
	fprintf( fp, "Poweraction  ~\n"				);
	fprintf( fp, "Powertype    ~\n"				);
	fprintf( fp, "Prompt       ~\n"				);
	fprintf( fp, "Cprompt      ~\n"				);
	fprintf( fp, "Sex          %d\n",ch->pcdata->genes[4]	);
	fprintf( fp, "Race         0\n"				);
	fprintf( fp, "Immune       %d\n",ch->pcdata->genes[3]	);
	fprintf( fp, "Polyaff      0\n"				);
	fprintf( fp, "Itemaffect   0\n"				);
	fprintf( fp, "Vampaff      0\n"				);
	fprintf( fp, "Vamppass     0\n"				);
	fprintf( fp, "Form         32767\n"			);
	fprintf( fp, "Beast        15\n"			);
	fprintf( fp, "Vampgen      -1\n"			);
	fprintf( fp, "Spectype     0\n"				);
	fprintf( fp, "Specpower    0\n"				);
	fprintf( fp, "Home         3001\n"			);
	fprintf( fp, "Level        2\n"				);
	fprintf( fp, "Trust        0\n"				);
	fprintf( fp, "Played       0\n"				);
	fprintf( fp, "Room         %d\n",ch->in_room->vnum 	);
	fprintf( fp, "PkPdMkMd     0 0 0 0\n"			);
	fprintf( fp, "Weapons      0 0 0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Spells       4 4 4 4 4\n"			);
	fprintf( fp, "Combat       0 0 0 0 0 0 0 0\n"		);
	fprintf( fp, "Stance       0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Locationhp   0 0 0 0 0 0 0\n"		);
	fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",
	    ch->pcdata->genes[0], ch->pcdata->genes[0], ch->pcdata->genes[1], 
	    ch->pcdata->genes[1], ch->pcdata->genes[2], ch->pcdata->genes[2]);
	fprintf( fp, "Gold         0\n"				);
	fprintf( fp, "Exp          0\n"				);
	fprintf( fp, "Act          1600\n"   			);
	fprintf( fp, "Extra        32768\n"   			);
	fprintf( fp, "AffectedBy   0\n"				);
	fprintf( fp, "Position     7\n"				);
	fprintf( fp, "Practice     0\n"				);
	fprintf( fp, "SavingThrow  0\n"				);
	fprintf( fp, "Alignment    0\n"				);
	fprintf( fp, "Hitroll      0\n"				);
	fprintf( fp, "Damroll      0\n"				);
	fprintf( fp, "Armor        100\n"			);
	fprintf( fp, "Wimpy        0\n"				);
	fprintf( fp, "Deaf         0\n"				);
	fprintf( fp, "Password     %s~\n",ch->pcdata->pwd	);
	fprintf( fp, "Bamfin       ~\n"				);
	fprintf( fp, "Bamfout      ~\n"				);
	fprintf( fp, "Title         the mortal~\n"		);
	fprintf( fp, "Conception   ~\n"				);
	fprintf( fp, "Parents      %s~\n",ch->pcdata->cparents	);
	fprintf( fp, "Cparents     ~\n"				);
	fprintf( fp, "AttrPerm     %d %d %d %d %d\n",
	    ch->pcdata->perm_str,
	    ch->pcdata->perm_int,
	    ch->pcdata->perm_wis,
	    ch->pcdata->perm_dex,
	    ch->pcdata->perm_con );
	fprintf( fp, "AttrMod      0 0 0 0 0\n"			);
	fprintf( fp, "Quest        0\n"				);
	fprintf( fp, "Wolf         0\n"				);
	fprintf( fp, "Rank         0\n"				);
	fprintf( fp, "Stage        0 0 0\n"			);
	fprintf( fp, "Wolfform     0 0\n"			);
	fprintf( fp, "Runes        0 0 0 0\n"			);
	fprintf( fp, "Disc         0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Genes        0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Power        0 0\n"			);
	fprintf( fp, "FakeCon      0 0 0 0 0 0 0 0\n"		);
	fprintf( fp, "Condition    0 48 48\n"			);
	fprintf( fp, "End\n\n" );
	fprintf( fp, "#END\n" );
	fclose( fp );
    }
    return;
}

void do_teach( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->generation != 1)
    {
	send_to_char("Only a Lich may teach others in the ways of sorcery.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Teach whom?\n\r", ch );
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
	send_to_char( "You cannot teach yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "They are already a mage.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only teach avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to teach vampires!\n\r", ch );
	return;
    }

	if (IS_CLASS(victim, CLASS_DROW)) {
		send_to_char("Not on a drow!\n\r", ch );
		return; }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "You are unable to teach werewolves!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "You are unable to teach demons!\n\r", ch );
	return;
    }


    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You are unable to teach highlanders.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot teach an unwilling person.\n\r", ch );
	return;
    }

    if (ch->exp < 100000)
    {
	send_to_char("You cannot afford the 100000 exp required to teach them.\n\r",ch);
	return;
    }

    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to learn from you.\n\r",ch);
	return;
    }

    ch->exp -= 100000;
    victim->exp -= 100000;

    act("You teach $N the basics of magic.", ch, NULL, victim, TO_CHAR);
    act("$n teaches $N the basics of magic.", ch, NULL, victim, TO_NOTVICT);
    act("$n teaches you the basics of magic.", ch, NULL, victim, TO_VICT);
    victim->level = LEVEL_APPRENTICE;
    victim->trust = LEVEL_APPRENTICE;
    send_to_char( "You are now an apprentice.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->pcdata->powers[MPOWER_RUNE0] = ch->pcdata->powers[MPOWER_RUNE0];
    victim->class    = CLASS_MAGE;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_katana(CHAR_DATA *ch, char *argument)
{
    OBJ_INDEX_DATA *pobj;

    OBJ_DATA *obj;
    
    if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	send_to_char("Huh?\r\n", ch);
	return;
    }
    if (ch->practice < 60)
    {
	send_to_char("You need 60 primal to create a katana.\r\n", ch);
	return;
    }

    if ( (pobj = get_obj_index(29695)) == NULL)
    {
	send_to_char("NULL object.\r\n", ch);
	return;
    }

    ch->practice -= 60;
    obj = create_object(pobj, 60);
    obj_to_char(obj, ch);
    obj->value[0] = 18000;
    send_to_char("You've created a katana. :D\r\n", ch);

	 return;
}

void do_highstar(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict = NULL;
    int times = 3;

    if (!IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if (!*argument && !ch->fighting)
    {
	stc("Highstar who?\n\r", ch);
	return;
    }

    if (*argument && (vict = get_char_room(ch, argument)) == NULL)
    {
	if ((vict = ch->fighting) == NULL)
	{
	    stc("Highstar who?\n\r", ch);
	    return;
	}
    } else if (!*argument) vict = ch->fighting;

    act("$N is struck by a devastating blow from $n's highstar.", ch, 0, vict, TO_NOTVICT);
    act("You are struck by a devastating blow from $n's highstar.", ch, 0, vict, TO_VICT);
    act("You strike $N with a devastating blow by your highstar.", ch, 0, vict, TO_CHAR);

  while (times>0)
  {
    int dam = number_range(500,ch->hit/10<600?600:ch->hit/10)*3;

    switch(number_range(1,23))
    {
	case 1:
	    do_say(ch,"&RTAKE THAT, VILE FIEND!");
	    dam *= 3;
	    break;
	case 2:
	    do_say(ch,"&GFOR THE SAKE OF THE ONE!");
	    dam *= 2;
	    break;
	case 3:
	    do_say(ch,"&mI suggest you flee.");
	    dam *= 1.5; // no break on purpose
	case 4:
	    do_say(ch,"&RDeath is inevitable.");
	    dam *= 1.7;
	    break;
	case 5:
	    do_say(ch,"&MPower to the People.");
	    dam *= .5;
    }
    damage(ch, vict, dam, skill_lookup("highstar"));
    times--;
  }
    WAIT_STATE(ch,PULSE_VIOLENCE*.75);
    return;
}

void do_planeswalk(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict;

    if (!IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if (ch->fighting)
    {
	stc("You can't enter the planes in a fight!\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("Planeswalk to who?\n\r", ch);
	return;
    }

    if ((vict = get_char_world(ch, argument)) == NULL)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (ch->fight_timer > 0)
    {
	sprintf(log_buf, "You can't do that with a fight timer of %d=(\n\r", ch->fight_timer);
	stc(log_buf, ch);
	return;
    }

    if (!IS_NPC(vict) && ch->move/2 < vict->move/(IS_CLASS(vict,CLASS_HIGHLANDER)?2:(vict->generation?vict->generation:5)))
    {
	stc("They are too powerful!\n\r", ch);
	return;
    }

    if (ch==vict) {stc("That would be stupid.\n\r", ch);return;}
    if (!vict->in_room) {stc("You can't go to him at this moment.\n\r", ch);return;}

    act("$n steps in to the higher planes.", ch, 0, 0, TO_ROOM);
    stc("You step in to the higher planes.\n\r", ch);
    char_from_room(ch);
    char_to_room(ch, vict->in_room);

    act("$n steps out of the planes in front of $N.", ch, 0, vict, TO_NOTVICT);
    act("$n steps out of the planes in front of you.", ch, 0, vict, TO_VICT);
    do_look(ch, "auto");
    return;
}
