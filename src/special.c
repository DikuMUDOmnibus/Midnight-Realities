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

#if !defined(INVASION_1)
#define INVASION_1
#endif

/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_eater		);
DECLARE_SPEC_FUN(	spec_gremlin_original	);
DECLARE_SPEC_FUN(	spec_gremlin_born	);
DECLARE_SPEC_FUN(	spec_rogue		);
DECLARE_SPEC_FUN(	spec_clan_guardian	);
DECLARE_SPEC_FUN(	spec_clan_torcalta	);
DECLARE_SPEC_FUN(	spec_clan_spiritknights	);
/*DECLARE_SPEC_FUN(	spec_clan_werewolf	);*/
DECLARE_SPEC_FUN(	spec_clan_silver_fangs   );
DECLARE_SPEC_FUN(	spec_clan_black_spiral_dancers );
DECLARE_SPEC_FUN(	spec_clan_darkblade );
DECLARE_SPEC_FUN(	spec_kavir_guardian	);
DECLARE_SPEC_FUN(	spec_zombie_lord	);
DECLARE_SPEC_FUN(	spec_dog		);
DECLARE_SPEC_FUN(	spec_killer		);
DECLARE_SPEC_FUN(	spec_jesus		);
DECLARE_SPEC_FUN(	spec_gummy_bear		);
DECLARE_SPEC_FUN(	spec_hustler		);
DECLARE_SPEC_FUN(	spec_fetch		);
DECLARE_SPEC_FUN(	spec_questspider	);
DECLARE_SPEC_FUN(	spec_kingdom_guard	);
DECLARE_SPEC_FUN(	spec_kingdom_healer	);

const   struct  spec_table	spec_table     [50]     = 
{
    {"spec_breath_any",			spec_breath_any},
    {"spec_breath_acid",		spec_breath_acid},
    {"spec_breath_fire",		spec_breath_fire},
    {"spec_breath_frost",		spec_breath_frost},
    {"spec_breath_gas",		spec_breath_gas},
    {"spec_breath_lightning",	spec_breath_lightning},
    {"spec_cast_adept",		spec_cast_adept},
    {"spec_cast_cleric",		spec_cast_cleric},
    {"spec_cast_judge",		spec_cast_judge},
    {"spec_cast_mage",		spec_cast_mage},
    {"spec_cast_undead",		spec_cast_undead},
    {"spec_fido",			spec_fido},
    {"spec_guard",			spec_guard},
    {"spec_janitor",		spec_janitor},
    {"spec_mayor",			spec_mayor},
    {"spec_poison",			spec_poison},
    {"spec_thief",			spec_thief},
    {"spec_eater",			spec_eater},
    {"spec_gremlin_original",	spec_gremlin_original},
    {"spec_gremlin_born",		spec_gremlin_born},
    {"spec_rogue",			spec_rogue},
    {"spec_clan_guardian",		spec_clan_guardian},
    {"spec_clan_torcalta",		spec_clan_torcalta},
    {"spec_clan_spiritknights",	spec_clan_spiritknights},
   /* {"spec_clan_werewolf",	spec_clan_werewolf},
    */
    {"spec_kavir_guardian",		spec_kavir_guardian},
#if defined(INVASION_1)
    {"spec_jesus",			spec_jesus},
#endif
    {"spec_zombie_lord",		spec_zombie_lord},
    {"spec_clan_silver_fangs",	spec_clan_silver_fangs},
    {"spec_clan_black_spiral_dancers", spec_clan_black_spiral_dancers},
    {"spec_clan_darkblade",		spec_clan_darkblade},
    {"spec_dog",			spec_dog},
    {"spec_killer",			spec_killer},
    {"dancing_gummy_bear",		spec_gummy_bear},
    {"hustler",				spec_hustler},
    {"fetch",				spec_fetch},
    {"questspider",			spec_questspider},
    {"kguard",				spec_kingdom_guard},
    {"khealer",				spec_kingdom_healer},
};


/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( const char *name )
{
    if ( !str_cmp( name, "spec_breath_any"	  ) ) return spec_breath_any;
    if ( !str_cmp( name, "spec_breath_acid"	  ) ) return spec_breath_acid;
    if ( !str_cmp( name, "spec_breath_fire"	  ) ) return spec_breath_fire;
    if ( !str_cmp( name, "spec_breath_frost"	  ) ) return spec_breath_frost;
    if ( !str_cmp( name, "spec_breath_gas"	  ) ) return spec_breath_gas;
    if ( !str_cmp( name, "spec_breath_lightning"  ) ) return
							spec_breath_lightning;
    if ( !str_cmp( name, "spec_cast_adept"	  ) ) return spec_cast_adept;
    if ( !str_cmp( name, "spec_cast_cleric"	  ) ) return spec_cast_cleric;
    if ( !str_cmp( name, "spec_cast_judge"	  ) ) return spec_cast_judge;
    if ( !str_cmp( name, "spec_cast_mage"	  ) ) return spec_cast_mage;
    if ( !str_cmp( name, "spec_cast_undead"	  ) ) return spec_cast_undead;
    if ( !str_cmp( name, "spec_fido"		  ) ) return spec_fido;
    if ( !str_cmp( name, "spec_guard"		  ) ) return spec_guard;
    if ( !str_cmp( name, "spec_janitor"		  ) ) return spec_janitor;
    if ( !str_cmp( name, "spec_mayor"		  ) ) return spec_mayor;
    if ( !str_cmp( name, "spec_poison"		  ) ) return spec_poison;
    if ( !str_cmp( name, "spec_thief"		  ) ) return spec_thief;
    if ( !str_cmp( name, "spec_eater"		  ) ) return spec_eater;
    if ( !str_cmp( name, "spec_gremlin_original"  ) ) return spec_gremlin_original;
    if ( !str_cmp( name, "spec_gremlin_born"	  ) ) return spec_gremlin_born;
    if ( !str_cmp( name, "spec_rogue"		  ) ) return spec_rogue;
    if ( !str_cmp( name, "spec_clan_guardian"     ) ) return spec_clan_guardian;
    if ( !str_cmp( name, "spec_clan_torcalta"     ) ) return spec_clan_torcalta;
    if ( !str_cmp( name, "spec_clan_spiritknights") ) return spec_clan_spiritknights;
   /* if ( !str_cmp( name, "spec_clan_werewolf"     ) ) return spec_clan_werewolf;
    */
    if ( !str_cmp( name, "spec_kavir_guardian"    ) ) return spec_kavir_guardian;
    if ( !str_cmp( name, "spec_zombie_lord"       ) ) return spec_zombie_lord;
    if ( !str_cmp( name, "spec_clan_silver_fangs"     ) ) return spec_clan_silver_fangs;
    if ( !str_cmp( name, "spec_clan_black_spiral_dancers"     ) ) return
spec_clan_black_spiral_dancers;
    if ( !str_cmp( name, "spec_clan_darkblade"	) ) return spec_clan_darkblade;
    if ( !str_cmp( name, "spec_dog"		  ) ) return spec_dog;
    if ( !str_cmp( name, "spec_killer"		)) return spec_killer;
#if defined(INVASION_1)
    if ( !str_cmp( name, "spec_jesus"		)) return spec_jesus;
#endif
    if ( !str_cmp(name, "dancing_gummy_bear")) return spec_gummy_bear;
    if ( !str_cmp(name, "hustler")) return spec_hustler;
    if ( !str_cmp(name, "spec_fetch")) return spec_fetch;
    if (!str_cmp(name,"questspider")) return spec_questspider;
    if (!str_cmp(name,"kguard")) return spec_kingdom_guard;
    if (!str_cmp(name, "khealer")) return spec_kingdom_healer;
//    if ( !str_cmp(name, "spec_lightthrower"	)) return
//spec_lightthrower;
    return 0;
}



/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}


/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( ch );
    }

    return FALSE;
}
bool spec_breath_acid( CHAR_DATA *ch )
{ int sn;   
    if ( ch->position != POS_FIGHTING )	return FALSE;
if ( ( sn = skill_lookup( "acid breath" ) ) < 0 )	return FALSE;
(*skill_table[sn].spell_fun) ( sn, ch->level, ch, ch->fighting );
    return TRUE;
}
/*{    return dragon( ch, "acid breath" );
}*/

bool spec_breath_fire( CHAR_DATA *ch )
{ int sn;   
    if ( ch->position != POS_FIGHTING )	return FALSE;
if ( ( sn = skill_lookup( "fire breath" ) ) < 0 )	return FALSE;
(*skill_table[sn].spell_fun) ( sn, ch->level, ch, ch->fighting );
    return TRUE;
}
/*{    return dragon( ch, "fire breath" );
}
*/
bool spec_breath_frost( CHAR_DATA *ch )
{ int sn;   
    if ( ch->position != POS_FIGHTING )	return FALSE;
if ( ( sn = skill_lookup( "frost breath" ) ) < 0 )	return FALSE;
(*skill_table[sn].spell_fun) ( sn, ch->level, ch, ch->fighting );
    return TRUE;
}
/*{    return dragon( ch, "frost breath" );
}*/

bool spec_breath_gas( CHAR_DATA *ch )
{int sn;
    if ( ch->position != POS_FIGHTING )	return FALSE;
if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )	return FALSE;
(*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL );
    return TRUE;
}

bool spec_breath_lightning( CHAR_DATA *ch )
{ int sn;   
    if ( ch->position != POS_FIGHTING )	return FALSE;
if ( ( sn = skill_lookup( "lightning breath" ) ) < 0 )	return FALSE;
(*skill_table[sn].spell_fun) ( sn, ch->level, ch, ch->fighting );
    return TRUE;
}
/*{    return dragon( ch, "lightning breath" );
} */

bool spec_cast_adept( CHAR_DATA *ch ) {
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0:
	act( "$n focuses his protective power on $N.", ch, NULL, victim, TO_ROOM);
	spell_armor( skill_lookup( "armor" ), ch->level, ch, victim );
	return TRUE;

    case 1:
	act( "$n calls upon his holy power to bless $N", ch, NULL, victim, TO_ROOM);
	spell_bless( skill_lookup( "bless" ), ch->level, ch, victim );
	return TRUE;

    case 2:
	act( "$n washes $N's eyes.", ch, NULL, victim, TO_ROOM);
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim );
	return TRUE;

    case 3:
	act( "$n tends to $N's light wounds.", ch, NULL, victim, TO_ROOM);
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim );
	return TRUE;

    case 4:
	act( "$n administers some antidote to $N.", ch, NULL, victim, TO_ROOM);
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim );
	return TRUE;

    case 5:
	act( "$n gives $N some refreshing beer.", ch, NULL, victim, TO_ROOM);
	spell_refresh( skill_lookup( "refresh" ), ch->level, ch, victim );
	return TRUE;

    }

    return FALSE;
}



bool spec_cast_cleric( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "cause serious";  break;
	case  2: min_level =  7; spell = "earthquake";     break;
	case  3: min_level =  9; spell = "cause critical"; break;
	case  4: min_level = 10; spell = "dispel evil";    break;
	case  5: min_level = 12; spell = "curse";          break;
	case  6: min_level = 12; spell = "change sex";     break;
	case  7: min_level = 13; spell = "flamestrike";    break;
	case  8:
	case  9:
	default:
	case 10: min_level = 15; spell = "harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "chill touch";    break;
	case  2: min_level =  7; spell = "weaken";         break;
	case  3: min_level =  8; spell = "teleport";       break;
	case  4: min_level = 11; spell = "colour spray";   break;
	case  5: min_level = 12; spell = "change sex";     break;
	case  6: min_level = 13; spell = "energy drain";   break;
	case  7:
	case  8:
	case  9: min_level = 15; spell = "fireball";       break;
	default: min_level = 20; spell = "acid blast";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "chill touch";    break;
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	default: min_level = 24; spell = "gate";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC )
	    continue;
	if (!IS_NPC(ch))
        act( "You savagely devour a corpse.", ch, NULL, NULL, TO_CHAR );
	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	if (corpse != NULL) extract_obj( corpse );
	return TRUE;
    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    int max_evil;
    int rand;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    max_evil = 300;
    ech      = NULL;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && victim->race > 24 && number_percent() > 95)
	    break;

	if ( victim->fighting != NULL
	&&   victim->fighting != ch
	&&   victim->alignment < max_evil )
	{
	    max_evil = victim->alignment;
	    ech      = victim;
	}
    }

    if ( victim != NULL )
    {
	rand=number_range(1,5);
	if (rand == 1)
	{
	    sprintf( buf, "It is an honour to meet you, %s!",
	        victim->name);
	    do_say( ch, buf );
	}
	else if (rand == 2)
	{
	    act("You bow deeply before $N.",ch,NULL,victim,TO_CHAR);
	    act("$n bows deeply before you.",ch,NULL,victim,TO_VICT);
	    act("$n bows deeply before $N.",ch,NULL,victim,TO_NOTVICT);
	}
	else if (rand == 3)
	{
	    act("You shake $N's hand.",ch,NULL,victim,TO_CHAR);
	    act("$n shakes your hand.",ch,NULL,victim,TO_VICT);
	    act("$n shakes $N's hand.",ch,NULL,victim,TO_NOTVICT);
	    sprintf( buf, "It's a pleasure to see you again, %s!",
	        victim->name);
	    do_say( ch, buf );
	}
	else if (rand == 4)
	{
	    act("You pat $N on the back.",ch,NULL,victim,TO_CHAR);
	    act("$n pats you on the back.",ch,NULL,victim,TO_VICT);
	    act("$n pats $N on the back.",ch,NULL,victim,TO_NOTVICT);
	    sprintf( buf, "Greetings %s!  If you need anything, just say!",
	        victim->name);
	    do_say( ch, buf );
	}
	else
	{
	    act("You beam a smile at $N.",ch,NULL,victim,TO_CHAR);
	    act("$n beams a smile at you.",ch,NULL,victim,TO_VICT);
	    act("$n beams a smile at $N.",ch,NULL,victim,TO_NOTVICT);
	}
	return TRUE;
    }

    if ( ech != NULL )
    {
	if (!IS_NPC(ech) && ech->race >= 25)
	{
	    sprintf( buf, "How DARE you attack %s? You shall DIE!",
	        ech->name);
	    do_say( ch, buf );
	    do_rescue( ch, ech->name );
	    return TRUE;
	}
	act( "$n screams 'PROTECT THE INNOCENT!!  BANZAI!!",
	    ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}



bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) )
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
	    act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const char open_path[] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( ch->fighting != NULL )
	return spec_cast_cleric( ch );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0' );
	break;

    case 'W':
	ch->position = POS_STANDING;
	act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
	act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
	act( "$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'b':
	act( "$n says 'What a view!  I must do something about that dump!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'c':
	act( "$n says 'Vandals!  Youngsters have no respect for anything!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'd':
	act( "$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'e':
	act( "$n says 'I hereby declare the city of Vittain open!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Vittain closed!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'O':
	do_unlock( ch, "gate" );
	do_open( ch, "gate" );
	break;

    case 'C':
	do_close( ch, "gate" );
	do_lock( ch, "gate" );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n bites you!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim );
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int gold;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_STEAL))
	||   victim->level >= LEVEL_IMMORTAL
	||   number_bits( 2 ) != 0
	||   !can_see( ch, victim ) )	/* Thx Glop */
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
	    act( "You discover $n's hands in your wallet!",
		ch, NULL, victim, TO_VICT );
	    act( "$N discovers $n's hands in $S wallet!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * number_range( 1, 20 ) / 100;
	    ch->gold     += 7 * gold / 8;
	    victim->gold -= gold;
	    return TRUE;
	}
    }

    return FALSE;
}

bool spec_eater( CHAR_DATA *ch )
{
  /* The spec_eater is a hungry bugger who eats players.  If they get 
   * eaten, they get transported to the room with the same vnum as the 
   * mob Example:  A spec_eater dragon with the vnum 31305 would send 
   * anybody eaten to room 31305.
   * KaVir.
   */
   CHAR_DATA       *victim;
   ROOM_INDEX_DATA *pRoomIndex;
   if ( ch->position != POS_FIGHTING )
   {
      return FALSE;
   }
   	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		return FALSE;
	}
   if (number_percent() > 40 ) return FALSE;
   victim=ch->fighting;
   act( "$n stares at $N hungrily and licks $s lips!",  ch, NULL, victim, TO_NOTVICT    );
   act( "$n stares at you hungrily and licks $s lips!",  ch, NULL, victim, TO_VICT    );
   if (number_percent() > 25 ) return FALSE;
   if (IS_NPC(ch)) {
   if (ch->pIndexData->vnum == 3001) ch->pIndexData->vnum = 6;
   pRoomIndex = get_room_index(ch->pIndexData->vnum);
   } else {
   if (ch->shape[dopplevnum] == 3001) ch->shape[dopplevnum] = 6;pRoomIndex
= get_room_index(ch->shape[dopplevnum]);}
   act( "$n opens $s mouth wide and lunges at you!", ch, NULL, victim, TO_VICT    );
   act( "$n swallows you whole!", ch, NULL, victim, TO_VICT    );
   act( "$n opens $s mouth wide and lunges at $N!",  ch, NULL, victim, TO_NOTVICT );
   act( "$n swallows $N whole!",  ch, NULL, victim, TO_NOTVICT );
   char_from_room(victim);
   char_to_room(victim,pRoomIndex);
   do_emote(ch,"burps loudly.");
   do_look(victim,"auto");
   return TRUE;
}

bool spec_gremlin_original( CHAR_DATA *ch )
{
   OBJ_DATA *object;
   OBJ_DATA *object_next;
   OBJ_DATA *obj;
   char buf  [MAX_STRING_LENGTH];

   if ( !IS_AWAKE( ch ) )
      return FALSE;

   if (number_percent() > 25)
   {
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	/* Lets make this mob DO things!  KaVir */
	int speech;
	speech = number_range(1,8);
	for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
	{
	    v_next = victim->next_in_room;
	    if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
	        break;
	}
	if ( victim != NULL && victim != ch)
	{
	    if (speech == 1) 
	    {
		strcpy(buf,"Anyone got any food? I'm famished!");
		do_say(ch,buf);
	    }
	    if (speech == 2) do_emote(ch,"rubs his tummy hungrily.");
	    if (speech == 3) do_emote(ch,"looks around for any scraps of food.");
	    if (speech == 4)
	    {
		if(!IS_NPC(victim) && victim != ch)
		    sprintf(buf,"Excuse me %s, have you got any spare food?",victim->name);
		else
		    sprintf(buf,"I wonder if %s has got any spare food?",victim->short_descr);
		do_say(ch,buf);
	    }

	    if (speech == 5)
	    {
		strcpy(buf,"If you're not hungry, drop that pie for me!");
		do_say(ch,buf);
	    }
	    if (speech == 6)
	    {
		if(!IS_NPC(victim) && victim != ch)
		    sprintf(buf,"Do you have any food, %s?  Pleeeaaase?",victim->name);
		else
		    sprintf(buf,"I bet %s has lots of spare food!",victim->short_descr);
		do_say(ch,buf);
	    }
	    if (speech == 7)
	    {
		if(!IS_NPC(victim) && victim != ch)
		    sprintf(buf,"Why won't you give me any food, %s?",victim->name);
		else
		    sprintf(buf,"Why won't you give me any food, %s?",victim->short_descr);
		do_say(ch,buf);
	    }
	    if (speech == 8)
	    {
		if(!IS_NPC(victim) && victim != ch)
		    sprintf(buf,"I think i'll follow you around for a while, ok %s?",victim->name);
		else
		    sprintf(buf,"I bet %s has lots of food.",victim->short_descr);
		do_say(ch,buf);
		do_follow(ch,victim->name);
	    }
	}
	/*
        return TRUE;
	*/
   }

   if (IS_NPC(ch)) do_drop(ch,"all");
   for ( object = ch->in_room->contents; object; object = object_next )
   {
      object_next = object->next_content;
      if ( !IS_SET(object->wear_flags, ITEM_TAKE) )
         continue;

      if (object == NULL) continue;

      if ( object->item_type == ITEM_FOOD
        || object->item_type == ITEM_TRASH
        || object->item_type == ITEM_CORPSE_NPC )
      {
         act( "$n picks $p up.", ch, object, NULL, TO_ROOM );
         obj_from_room( object );
         obj_to_char( object, ch );
         strcpy(buf,"Ah....foooood....goooood!");
         do_say(ch,buf);
         if (object->item_type == ITEM_CORPSE_NPC) do_get(ch,"all corpse");
         act( "$n eats $p.", ch, object, NULL, TO_ROOM );
         act( "You eats $p.", ch, object, NULL, TO_CHAR );
         if (object != NULL) extract_obj(object);
	 if (number_percent() > 25) return TRUE;
         act( "$n sits down and curls up into a ball.", ch,  NULL, NULL, TO_ROOM );
         act( "You watch in amazement as a cocoon forms around $n.", ch,  NULL, NULL, TO_ROOM );
         obj=create_object(get_obj_index(30008),0);
         obj->value[0] = 30002;
         obj->timer = 1;
         obj_to_room(obj,ch->in_room);
	if (IS_NPC(ch)) 
         extract_char(ch,TRUE);
	else ch->shapespec = spec_gremlin_born;
         return TRUE;
      }
   }
   return FALSE;
}

bool spec_gremlin_born( CHAR_DATA *ch )
{
   OBJ_DATA        *object;
   OBJ_DATA        *object_next;
   OBJ_DATA        *obj;
   CHAR_DATA	   *victim;
   EXIT_DATA	   *pexit;
   ROOM_INDEX_DATA *to_room;
   int		   door;
   char            buf [MAX_STRING_LENGTH];

   /* When they first appear, they try to equip themselves as best they can */
   if ( ch->max_move == 100 )
   {
         do_get(ch,"all");
         do_wear(ch,"all");
         do_drop(ch,"all");
   }
   ch->max_move = ch->max_move -1;
   if ( ch->max_mana > 249 )
   {
	 strcpy(buf,"I cannot go any further...my task is complete.");
	 do_say(ch,buf);
         act( "$n falls to the floor and crumbles into dust.", ch, NULL, NULL, TO_ROOM );
         extract_char(ch,TRUE);
	 return TRUE;
   }
   if ( ch->max_move < 5 )
   {
	 strcpy(buf,"Alas, there is not enough food to go around...");
	 do_say(ch,buf);
         act( "$n falls to the floor and crumbles into dust.", ch, NULL, NULL, TO_ROOM );
         if (IS_NPC(ch)) extract_char(ch,TRUE); else do_quit(ch, "");
	 return TRUE;
   }
   else if (( ch->max_move < 35) && number_percent() > 95 )
         act( "$n's stomach growls with hunger.", ch, NULL, NULL, TO_ROOM );

   if ( !IS_AWAKE( ch ) )
      return FALSE;

   for ( object = ch->in_room->contents; object; object = object_next )
   {
      object_next = object->next_content;
      if ( !IS_SET(object->wear_flags, ITEM_TAKE) )
         continue;

      if (object == NULL) continue;

      if ( object->item_type == ITEM_FOOD
        || object->item_type == ITEM_TRASH
        || object->item_type == ITEM_CORPSE_NPC )
      {
         act( "$n picks $p up.", ch, object, NULL, TO_ROOM );
         obj_from_room( object );
         obj_to_char( object, ch );
         if (object->item_type == ITEM_CORPSE_NPC) do_get(ch,"all corpse");
         act( "$n eats $p.", ch, object, NULL, TO_ROOM );
	 if (object != NULL) extract_obj(object);
	 if (ch->max_move < 80) ch->max_move = ch->max_move + 20;
         do_wear(ch,"all");
         do_drop(ch,"all");
	 /* Increase the following if gremlins reproduce too fast, and 
	  * reduce it if they reproduce too slowly.  KaVir.
	  */
	 if (ch->max_move > 75 && number_percent() > 25)
	 {
	     if (ch->pIndexData->vnum == 30004)
             	 obj=create_object(get_obj_index(30009),0);
	     else
	     {
             	 obj=create_object(get_obj_index(30008),0);
             	 obj->value[0] = 30003;
             	 if (ch->pIndexData->vnum == 30003)
             	     obj->value[0] = 30004;
             	 obj->timer = number_range(8,10);
	     }
             act( "$n squats down and lays $p!", ch,  obj, NULL, TO_ROOM );
             obj_to_room(obj,ch->in_room);
	     if (number_percent() > 25)
	     	 ch->max_mana = ch->max_mana +50;
	 }
	 return TRUE;
      }
   }

   if ( ch->fighting != NULL )
	return spec_poison( ch );

   /* We'll give him 3 chances to find an open door */
   door = number_range(0,5);
   if (((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL)
	door = number_range(0,5);
   if (((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL)
	door = number_range(0,5);
   if (!(((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL))
   {
   	/* If the door is closed they'll have to open it */
   	if (IS_SET(pexit->exit_info, EX_CLOSED))
   	{
	    if (door == 0) do_open(ch,"north");
	    if (door == 1) do_open(ch,"east");
	    if (door == 2) do_open(ch,"south");
	    if (door == 3) do_open(ch,"west");
	    if (door == 4) do_open(ch,"up");
	    if (door == 5) do_open(ch,"down");
   	}
   	/* Now they know where they are going, they have to move */
   	move_char(ch,door);
/*	return FALSE; */
   }
   /* Now we check the room for someone to kill */
   for (victim = char_list; victim; victim = victim->next)
   {
      if ( victim->in_room != ch->in_room
	 || !victim->in_room
	 || victim == ch
	 || (!IS_NPC(victim) && IS_HERO(victim) && victim->hit < 0)
         || ((victim->name == ch->name) && (ch->max_move > 30))
         || ((victim->level > ch->level) && (ch->level < 12 ))
         || IS_IMMORTAL(victim) )
         continue;

      if (victim->in_room == ch->in_room)
      {
         if (victim->name == ch->name)
	 {
	     strcpy(buf,"Sorry brother, but I must eat you or I'll starve.");
	     do_say(ch,buf);
             do_kill(ch,"gremlin");
             do_kill(ch,"2.gremlin");
             return TRUE;
	 }
	 else
	 {
             do_kill(ch,victim->name);
             return TRUE;
	 }
         return TRUE;
      }
      continue;
   }
   return FALSE;
}

/* Spec_rogue, coded by Malice. */
bool spec_rogue( CHAR_DATA *ch )
{
/* To add to the life of mobs... they pickup and wear equipment */
    OBJ_DATA *object;
    OBJ_DATA *obj2;
    OBJ_DATA *object_next;
    char buf [MAX_STRING_LENGTH];
//    CHAR_DATA *vict;

    if ( !IS_AWAKE( ch ) )
	return FALSE;
/*
    for (vict = ch->in_room->people; vict; vict = vict->next_in_room)
    {
	if (IS_NPC(vict) || vict->hit > 40)
	    continue;
	do_kill(ch,vict->name);
	paradox(vict);
	if (number_range(1,4)==1) spell_heal(skill_lookup("heal"),500, ch, vict);
//	do_decapitate(ch,vict->name);
	return TRUE;
    }
*/
    for ( object = ch->in_room->contents; object; object = object_next )
    {
        object_next = object->next_content;
        if ( object->name == NULL ) // for some reason objects keep popping up with no name, short descr, etc
	    continue;
	    
	if ( !IS_SET( object->wear_flags, ITEM_TAKE ) )
	    continue;

	if ( object->item_type == ITEM_CORPSE_NPC )
	    continue;
	    
	if ( ( object->item_type != ITEM_DRINK_CON
	    && object->item_type != ITEM_TRASH )
	    &&
	    
	   !(( IS_OBJ_STAT( object, ITEM_ANTI_EVIL   ) && IS_EVIL   ( ch ) ) ||
		 ( IS_OBJ_STAT( object, ITEM_ANTI_GOOD   ) && IS_GOOD   ( ch ) ) ||
		 ( IS_OBJ_STAT( object, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL( ch ) ) ) )
		
	{
	    act( "$n picks up $p and examines it carefully.", ch, object, NULL, TO_ROOM );
	    obj_from_room( object );
	    obj_to_char( object, ch );
		/*Now compare it to what we already have*/
		for ( obj2 = ch->carrying; obj2; obj2 = obj2->next_content )
		{
		    if ( obj2->wear_loc != WEAR_NONE
			&& can_see_obj( ch, obj2 )
			&& object->item_type == obj2->item_type
			&& ( object->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
			break;
		}
		if (!obj2)
		{
		    switch (object->item_type)
		    {
			default:
			    strcpy(buf,"Hey, what a find!");
			    do_say(ch,buf);
			    break;
			case ITEM_FOOD:
			    strcpy(buf, "This looks like a tasty morsel!");
			    do_say(ch,buf);
			    do_eat(ch,object->name);
			    break;
			case ITEM_WAND:
			    strcpy(buf,"Wow, a magic wand!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_STAFF:
			    strcpy(buf,"Kewl, a magic staff!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_WEAPON:
			    strcpy(buf,"Hey, this looks like a nifty weapon!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_ARMOR:
			    strcpy(buf,"Oooh...a nice piece of armor!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;/*
			case ITEM_POTION:
			    strcpy(buf, "Great!  I was feeling a little thirsty!");
			    do_say(ch,buf);
			    act( "You quaff $p.", ch, object, NULL ,TO_CHAR );
			    act( "$n quaffs $p.", ch, object, NULL, TO_ROOM );
			    obj_cast_spell( object->value[1], object->level, ch, ch, NULL );
 			    obj_cast_spell( object->value[2], object->level, ch, ch, NULL );
			    obj_cast_spell( object->value[3], object->level, ch, ch, NULL );
			    extract_obj( object );
			    break;
			case ITEM_SCROLL:
			    strcpy(buf,"Hmmm I wonder what this says?");
			    do_say(ch,buf);
			    act( "You recite $p.", ch, object, NULL, TO_CHAR );
			    act( "$n recites $p.", ch, object, NULL, TO_ROOM );
			    obj_cast_spell( object->value[1], object->level, ch, NULL, object );
			    obj_cast_spell( object->value[2], object->level, ch, NULL, object );
			    obj_cast_spell( object->value[3], object->level, ch, NULL, object );
			    extract_obj( object );
			    break;*/
		    }
		    return TRUE;
		}
		
	    if ((object->level > obj2->level))
	    {
	    	strcpy(buf,"Now THIS looks like an improvement!");
		do_say(ch,buf);
	    	remove_obj(ch,obj2->wear_loc,TRUE);
	    	wear_obj(ch,object,FALSE);
	    }
	    else
	    {
	    	strcpy(buf,"I don't want this piece of junk!");
		do_say(ch,buf);
	    	act("You don't like the look of $p.",ch,object,NULL,TO_CHAR);
			do_drop(ch,object->name);
			do_sacrifice(ch,object->name);
		}
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Rotains Clan Guardian Spec
 * Original Code by Malice
 */

bool spec_clan_guardian(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];
   OBJ_DATA              *obj;
if (!IS_NPC(ch)) return FALSE;

   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   /*if (ch->in_room < 6645 && ch->in_room > 6657) break;*/

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
	   || (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU))
           || (victim->clan != NULL && !strcmp(victim->clan,"Manus
Nigrum") ))
           {
             continue;
           }

      if ((obj = get_obj_carry(victim, "dlwr5"))
               && obj->pIndexData->vnum==6641)
      {
         return FALSE; 
      }
      else
      {
        if (ch->in_room != victim->in_room)
        {
           act("$n disappears in a haze of red!",victim,NULL,NULL,TO_ROOM);
           act("You disappear into a haze of red!",victim,NULL,NULL,TO_CHAR);

           char_from_room( victim );
           char_to_room(victim, ch->in_room);

           act("$n appears in a haze of red!",victim,NULL,NULL,TO_ROOM);
           act("You appear into a haze of red!",victim,NULL,NULL,TO_CHAR);
        }
     
        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan headquaters!",ch,NULL,victim,TO_CHAR);
/*
        spell_curse( 24 , 100 , ch , victim );
*/
        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s is in Manus Nigrum Headquarters! Attack!\n\r",
victim->name); 
        do_shout( ch , buf);
      
        multi_hit( ch, victim, gsn_backstab); /* Nice Backstab to start */

        return TRUE;

      }
   }
   return FALSE;
}

bool spec_clan_torcalta(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];
if (!IS_NPC(ch)) return FALSE;

   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
	   || (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU))
           || (victim->clan != NULL && !strcmp(victim->clan,"Torc Alta") ))
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);
        }
     
        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan headquarters!",ch,NULL,victim,TO_CHAR);

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s is in the Torc Alta Headquarters! Attack!\n\r", victim->name); 
        do_shout( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}

bool spec_clan_darkblade(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];
if (!IS_NPC(ch)) return FALSE;

   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
           || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
           || (IS_CLASS(victim, CLASS_AMAZON))
           || (victim->clan != NULL && !strcmp(victim->clan,"darkblade") ))
        {
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);
        }

        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan
             headquarters!",ch,NULL,victim,TO_CHAR);

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
          ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s is in Hell! Attack!\n\r", victim->name);
        if ( !IS_NPC(ch) )
        {
         return FALSE;
        }
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}

            
bool spec_clan_spiritknights(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];

if (!IS_NPC(ch)) return FALSE;
   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
	   || (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU))
           || (victim->clan != NULL && !strcmp(victim->clan,"Spirit Knights") ))
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);
        }
     
        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan headquarters!",ch,NULL,victim,TO_CHAR);

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s Is in the Spirit Knights Headquarters! Attack!\n\r", victim->name); 
        do_shout( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}

bool spec_clan_werewolf(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   ROOM_INDEX_DATA       *ch_room;
   char               buf  [MAX_STRING_LENGTH]; 
 ch_room = ch->in_room;
        if (ch->hit < (ch->max_hit * 0.9) && number_percent() < 85)
	{
  sprintf(buf,"%s's eyes glow bright blue for a moment.",ch->short_descr);
   ADD_COLOUR(ch,buf,D_BLUE);
act(buf,ch,NULL,NULL,TO_ROOM);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
   
if (ch->in_room == NULL) return FALSE;
 
   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room != ch->in_room)
	{
            continue;
        }
 
     
        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
  ch->position == POS_STANDING )
        {
          
         act("$n tears $N's head from $S shoulders!",ch,NULL,victim,TO_NOTVICT);
         act("You tear $N's head from $S shoulders!",ch,NULL,victim,TO_CHAR);
	 send_to_char("Your head is torn from your shoulders!\n\r",victim);
	 sprintf( buf,"%s has been dismembered by the Guardian of Apocalypse.",victim->name);
	 do_info(ch, buf);
         if (victim->level == 3) victim->level=victim->level-1;
         victim->mdeath=victim->mdeath+1;
         act("The Guardian tears $N's arms off!",ch,NULL,victim,TO_NOTVICT);
         act("The Guardian tears $N's legs off!",ch,NULL,victim,TO_NOTVICT);
         act("The Guardian tears $N's guts out!",ch,NULL,victim,TO_NOTVICT);  
  
          behead(victim);
          do_otransfer(ch,"corpse");
          do_get(ch,"head");
make_part(victim,"arm");
make_part(victim,"arm");
make_part(victim,"leg");
make_part(victim,"leg");
make_part(victim,"entrails");

continue;
        }
        return TRUE;
 
   }
   return FALSE;
}

bool spec_kavir_guardian(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
/*
   char                  buf[MAX_STRING_LENGTH];
*/
   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
	   || (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU))
           || (victim->clan != NULL && !strcmp(victim->clan,"DarkBlade") )
	   || IS_ITEMAFF(victim, ITEMA_DBPASS))
           {
             continue;
           }

      if (ch->in_room!=victim->in_room)
      {
         act("A pair of skeletal hands reach up from the ground.",victim,NULL,NULL,TO_ROOM);
         act("A pair of skeletal hands reach up from the ground.",victim,NULL,NULL,TO_CHAR);
         act("The hands grab $n by the ankles and pull $m into the ground!",victim,NULL,NULL,TO_ROOM);
         act("The hands grab you by the ankles and pull you into the ground!",victim,NULL,NULL,TO_CHAR);
         char_from_room( victim );
         char_to_room(victim, ch->in_room);
      }
 
      if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	ch->position == POS_STANDING)
      {
/*
         act("$n tears $N's head from $S shoulders!",ch,NULL,victim,TO_NOTVICT);
         act("You tear $N's head from $S shoulders!",ch,NULL,victim,TO_CHAR);
	 send_to_char("Your head is torn from your shoulders!\n\r",victim);
	 sprintf( buf,"s has been beheaded for his crimes against clan DarkBlade.",victim->name);
	 do_info(ch, buf);
         if (victim->level == 3) victim->level=victim->level-1;
         victim->pdeath=victim->pdeath+1;
*/
	 char_from_room( victim );
	 char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
/*
	 behead( victim );
*/
         continue;
      }
      multi_hit( ch, victim, TYPE_UNDEFINED);
      return TRUE;
   }
   return FALSE;
}

bool spec_clan_black_spiral_dancers(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
           || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
|| victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
          /* || (IS_CLASS(victim, CLASS_WEREWOLF))*/
           || (victim->clan != NULL && !strcmp(victim->clan,"Black Spiral Dancers") ))
        {
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);
        }

        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan
headquarters!",ch,NULL,victim,TO_CHAR);

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
          ch->position == POS_STANDING)
        {
           char_from_room( victim );
 char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s Is in Black Spiral Dancers Head Quarters! Attack!\n\r",
victim->name);
        if ( !IS_NPC(ch) )
{
return FALSE;
}
       multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}

bool spec_clan_silver_fangs(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = descriptor_list; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
           || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
|| victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
          /* || (IS_CLASS(victim, CLASS_WEREWOLF))*/
           || (victim->clan != NULL && !strcmp(victim->clan,"Silver Fangs") ))
        {
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR);
        }

        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT);
        act("You let out a battlecry as you defend the clan
headquarters!",ch,NULL,victim,TO_CHAR);

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
          ch->position == POS_STANDING)
        {
           char_from_room( victim );
 char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s Is in Silver Fangs Head Quarters! Attack!\n\r", victim->name);
        if ( !IS_NPC(ch) )
{
return FALSE;
}
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}

bool spec_zombie_lord( CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    OBJ_DATA		*obj;
    OBJ_DATA		*obj_content;
    OBJ_DATA		*obj_next;
    CHAR_DATA		*victim;
    MOB_INDEX_DATA	*pMobIndex;
    EXIT_DATA		*pexit;
    ROOM_INDEX_DATA	*to_room;
    int door;
    int consider 	= 4;
    bool north_ok 	= TRUE;
    bool east_ok 	= TRUE;
    bool south_ok 	= TRUE;
    bool west_ok 	= TRUE;
    bool up_ok 		= TRUE;
    bool down_ok 	= TRUE;
    int countup 	= 6;
    int option;

    if ( ch->position <= POS_SITTING )
    {
	do_stand(ch,"");
	return TRUE;
    }
    if ( (victim = ch->fighting) != NULL )
    {
	if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_POISON))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_cure_poison( skill_lookup( "cure poison" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_BLIND))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_CURSE))
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(ch, AFF_SANCTUARY))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_sanctuary( skill_lookup( "sanctuary" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(victim, AFF_FAERIE_FIRE) && number_percent() < 50)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_faerie_fire( skill_lookup( "faerie fire" ), ch->level, ch, victim );
	}
	else if (!IS_AFFECTED(victim, AFF_BLIND) && number_percent() < 15)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_blindness( skill_lookup( "blindness" ), ch->level, ch, victim );
	}
	else if (!IS_AFFECTED(victim, AFF_CURSE) && number_percent() < 15)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_curse( skill_lookup( "curse" ), ch->level, ch, victim );
	}
	else if (ch->loc_hp[6] > 0)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_clot( skill_lookup( "clot" ), ch->level, ch, ch );
	}
	else if (ch->hit < (ch->max_hit * 0.5) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
	else if (ch->hit < (ch->max_hit * 0.25) && number_percent() < 50)
	{
	    do_flee(ch,"");
	    ch->spectype = ZOMBIE_REST;
	}
	else if (ch->hit < (ch->max_hit * 0.1) && number_percent() < 25)
	{
	    act("$n's eyes glow bright green for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_teleport( skill_lookup( "teleport" ), ch->level, ch, ch );
	    ch->spectype = ZOMBIE_REST;
	}
	else
	{
	    switch( number_range(1,10) )
	    {
		default:
		    do_kick(ch,"");
		    break;
		case 1:
		    do_disarm(ch,"");
		    break;
		case 2:
		case 3:
		case 4:
		case 5:
		    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
		    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, victim );
		    break;
		case 6:
		case 7:
		case 8:
		    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
		    spell_harm( skill_lookup( "harm" ), ch->level, ch, victim );
		    break;
	    }
	}
	if ( (victim = ch->fighting) == NULL ) return TRUE;
	switch( number_percent() )
	{
	    default:
		break;
	    case 1:
		strcpy(buf,"Foolish mortal, you think you can kill what is already dead?");
		do_say(ch,buf);
		break;
	    case 2:
		if (IS_NPC(victim))
		    sprintf(buf,"I shall feast on your soul for this, %s",victim->short_descr);
		else
		    sprintf(buf,"I shall feast on your soul for this, %s",victim->name);
		do_say(ch,buf);
	    case 3:
		if (IS_NPC(victim))
		    sprintf(buf,"%s shall pay for his arrogance!",victim->short_descr);
		else
		    sprintf(buf,"%s shall pay for his arrogance!",victim->name);
		do_shout(ch,buf);
		break;
	    case 4:
		strcpy(buf,"This fight shall be your last!");
		do_say(ch,buf);
		break;
	}
	return TRUE;
    }

    if (ch->spectype != ZOMBIE_TRACKING && ch->spectype != ZOMBIE_REST)
    {
	if ((obj = get_obj_here(ch,"corpse")) != NULL)
	{
	    if ( ( pMobIndex = get_mob_index( obj->value[2] ) ) == NULL )
		return spec_rogue(ch);
	    victim = create_mobile( pMobIndex );
	    char_to_room(victim,ch->in_room);
	    sprintf(buf,"zombie %s",victim->name);
	    sprintf(buf,"The zombie of %s stands here.\n\r",victim->short_descr);
	    free_string(victim->long_descr);
	    victim->long_descr = str_dup(buf);
	    free_string(victim->name);
	    victim->name = str_dup(buf);
	    sprintf(buf,"the zombie of %s",victim->short_descr);
	    free_string(victim->short_descr);
	    victim->short_descr = str_dup(buf);
	    act("$n makes a few gestures over $p.",ch,obj,NULL,TO_ROOM);
	    act("$n clambers to $s feet.",victim,NULL,NULL,TO_ROOM);
	    free_string(victim->powertype);
	    victim->powertype = str_dup("zombie");
	    for ( obj_content = obj->contains; obj_content != NULL; obj_content = obj_next )
	    {
		obj_next = obj_content->next_content;
		obj_from_obj(obj_content);
		obj_to_char(obj_content,victim);
	    }
	    extract_obj(obj);
/*
	    victim->spec_fun = ch->spec_fun;
*/
	    do_wear(victim,"all");
	    strcpy(buf,"I shall spread the corruption!  The time of the Apocalypse is at hand!");
	    do_say(victim,buf);
	}
    	door = number_range(0,5);
	for ( door = 0; door <= 5; door++ )
	{
	    if (((pexit = ch->in_room->exit[door]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
	    {
		switch (door)
		{
		    case DIR_NORTH: north_ok = FALSE; countup -= 1; break;
		    case DIR_SOUTH: south_ok = FALSE; countup -= 1; break;
		    case DIR_EAST:  east_ok  = FALSE; countup -= 1; break;
		    case DIR_WEST:  west_ok  = FALSE; countup -= 1; break;
		    case DIR_UP:    up_ok    = FALSE; countup -= 1; break;
		    case DIR_DOWN:  down_ok  = FALSE; countup -= 1; break;
		}
	    }
	}

	if (countup < 1)
	{
	    strcpy(buf,"Damn, I hate it when this happens!");
	    do_say(ch,buf);
	    do_recall(ch,"");
	    return TRUE;
	}

	for ( ; ; )
	{
	    option = number_range(0,5);
	    if (((pexit = ch->in_room->exit[option]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
		continue;
	    if (countup > 1 && option == ch->specpower) continue;
	    if (IS_SET(pexit->exit_info, EX_CLOSED))
 	    {
	    	if (option == 0) do_open(ch,"north");
	    	if (option == 1) do_open(ch,"east");
	    	if (option == 2) do_open(ch,"south");
	    	if (option == 3) do_open(ch,"west");
	    	if (option == 4) do_open(ch,"up");
	    	if (option == 5) do_open(ch,"down");
   	    }
	    switch (option)
	    {
		default: break;
		case DIR_NORTH: ch->specpower = DIR_SOUTH; break;
		case DIR_SOUTH: ch->specpower = DIR_NORTH; break;
		case DIR_EAST:  ch->specpower = DIR_WEST;  break;
		case DIR_WEST:  ch->specpower = DIR_EAST;  break;
		case DIR_UP:    ch->specpower = DIR_DOWN;  break;
		case DIR_DOWN:  ch->specpower = DIR_UP;    break;
	    }
   	    move_char(ch,option);
	    break;
	}

    	for (victim = char_list; victim != NULL; victim = victim->next)
    	{
	    if (victim->in_room == NULL) continue;
    	    if ( victim->in_room != ch->in_room
	    || victim == ch
	    || (!IS_NPC(victim) && IS_HERO(victim) && victim->hit < 0)
            || IS_IMMORTAL(victim)
	    || is_safe(ch,victim)
	    || !IS_NPC(victim)
	    || !can_see(ch,victim) )
	    	continue;

	    if (IS_NPC(victim) && !str_cmp(victim->powertype,"zombie")) continue;
	    if (IS_NPC(victim) && victim->pIndexData->vnum == 30011) continue;
	    if (victim->in_room == ch->in_room)
      	    {
	    	act("$n examines $N closely, looking for $S weaknesses.",ch,NULL,victim,TO_NOTVICT);
	    	act("$n examines you closely, looking for your weaknesses.",ch,NULL,victim,TO_VICT);
	    	if      (victim->hit > (ch->hit*1.5)) consider -= 1;
	    	else if ((victim->hit*1.5) < ch->hit) consider += 1;
	    	if      (char_ac(victim)-50 > char_ac(ch)) consider -= 1;
	    	else if (char_ac(victim)+50 < char_ac(ch)) consider += 1;
	    	if      (char_hitroll(victim)+10 < char_hitroll(ch)) consider += 1;
	    	else if (char_hitroll(victim)-10 > char_hitroll(ch)) consider -= 1;
	    	if      (char_damroll(victim)+10 < char_damroll(ch)) consider += 1;
	    	else if (char_damroll(victim)-10 > char_damroll(ch)) consider -= 1;
	    	switch (consider)
	    	{
		    default: break;
		    case 8:
		    	strcpy(buf,"This shouldn't take more than a few seconds!");
			do_say(ch,buf);
		    	break;
		    case 7:
		    	strcpy(buf,"Ha! You are no match for me!");
			do_say(ch,buf);
		    	break;
		    case 6:
		    	strcpy(buf,"I should be able to win this one...");
			do_say(ch,buf);
		    	break;
		    case 5:
		    	strcpy(buf,"Hmmm, close match, but I think I have the edge.");
			do_say(ch,buf);
		    	break;
		    case 4:
		    	strcpy(buf,"This one will be tricky...");
			do_say(ch,buf);
		    	break;
		    case 3:
		    	strcpy(buf,"Hmmm, I'm not sure if I can win this one.");
			do_say(ch,buf);
		    	break;
		    case 2:
		    	strcpy(buf,"Heheh better not risk it...");
			do_say(ch,buf);
		    	break;
		    case 1:
		    	strcpy(buf,"I'd need a lot of luck...better not.");
			do_say(ch,buf);
		    	break;
		    case 0:
		    	strcpy(buf,"I think I'll give this one a miss!!!");
			do_say(ch,buf);
		    	break;
	    	}
		if (IS_IMMORTAL(victim)) continue;
	    	if (consider < 3) continue;
	    	do_kill(ch,victim->name);
            	return TRUE;
	    }
	}
    }

    switch ( ch->spectype )
    {
    default:
    case ZOMBIE_NOTHING:
	ch->spectype = number_range(1, 3);
	return spec_rogue(ch);
    case ZOMBIE_TRACKING:
/*
	if (strlen(ch->hunting) > 1)
	{
	    ch->spectype = number_range(1, 3);
	    return spec_rogue(ch);
	}
	if      (strlen(ch->in_room->track[0]) > 1)
	    strcpy(buf,ch->in_room->track[0]);
	else if (strlen(ch->in_room->track[1]) > 1)
	    strcpy(buf,ch->in_room->track[1]);
	else if (strlen(ch->in_room->track[2]) > 1)
	    strcpy(buf,ch->in_room->track[2]);
	else if (strlen(ch->in_room->track[3]) > 1)
	    strcpy(buf,ch->in_room->track[3]);
	else if (strlen(ch->in_room->track[4]) > 1)
	    strcpy(buf,ch->in_room->track[4]);
	else {ch->spectype = number_range(1, 3);return spec_rogue(ch);}
	do_hunt(ch,buf);
	ch->spectype = number_range(1, 3);
	return spec_rogue(ch);
*/
    case ZOMBIE_ANIMATE:
    case ZOMBIE_CAST:
	if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_POISON) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_cure_poison( skill_lookup( "cure poison" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_BLIND) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_CURSE) && number_percent() < 75)
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(ch, AFF_SANCTUARY) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_sanctuary( skill_lookup( "sanctuary" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else if (ch->loc_hp[6] > 0)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_clot( skill_lookup( "clot" ), ch->level, ch, ch );
	}
	else if (ch->hit < ch->max_hit)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else ch->spectype = 0;
	if (ch->hit < (ch->max_hit*0.25)) ch->spectype = ZOMBIE_REST;
	return TRUE;
    case ZOMBIE_REST:
	if (ch->hit >= ch->max_hit)
	{do_stand(ch,"");ch->spectype = 0;return TRUE;}
	if (IS_AFFECTED(ch, AFF_CURSE))
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	    return TRUE;
	}
	if (ch->in_room->vnum != ROOM_VNUM_ALTAR) do_recall(ch,"");
	if (ch->in_room->vnum == ROOM_VNUM_TEMPLE) do_north(ch,"");
	if (ch->position == POS_STANDING) do_rest(ch,"");
	if (ch->hit < ch->max_hit)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
	return TRUE;
    }
    return FALSE;
}

bool spec_dog( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *o_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    EXIT_DATA		*pexit;
    ROOM_INDEX_DATA	*to_room;
    int door;
    bool north_ok 	= TRUE;
    bool east_ok 	= TRUE;
    bool south_ok 	= TRUE;
    bool west_ok 	= TRUE;
    bool up_ok 		= TRUE;
    bool down_ok 	= TRUE;
    int countup 	= 6;
    int option;
    int random = number_range(1,5);

    if ( !IS_AWAKE(ch) )
    {
	if (random <= 2)
	    do_wake(ch,"");
	return TRUE;
    }
    else if (ch->position < POS_STANDING)
    {
	if (random <= 2)
	    do_stand(ch,"");
	else
	{
	    if (ch->in_room->vnum == 29107 && number_range(1,4) <= 2)
		act("$n eats some dog food from $s food bowl",ch,NULL,NULL,TO_ROOM);
	    else if (ch->in_room->vnum == 29107)
		act("$n drinks some water from $s water bowl",ch,NULL,NULL,TO_ROOM);
	}
	return TRUE;
    }

    if (ch->in_room == NULL) return TRUE;

    if (ch->in_room->vnum == 29104 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	{
	    act("$n curls up in front of the fire.",ch,NULL,NULL,TO_ROOM);
	    do_sleep(ch,"");
	}
	else if (random == 2)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM);
	else if (random == 4)
	{
	    act("$n jumps onto the couch.",ch,NULL,NULL,TO_ROOM);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to crawl under a chair.",ch,NULL,NULL,TO_ROOM);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29104 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	{
	    act("$n curls up under the dining table.",ch,NULL,NULL,TO_ROOM);
	    do_sleep(ch,"");
	}
	else if (random == 2)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM);
	else if (random == 4)
	{
	    act("$n jumps onto a chair.",ch,NULL,NULL,TO_ROOM);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to crawl under a chair.",ch,NULL,NULL,TO_ROOM);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29113 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM);
	else if (random == 2)
	    act("$n tries to climb into the bath.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM);
	else if (random == 4)
	{
	    act("$n sits behind the bathroom door.",ch,NULL,NULL,TO_ROOM);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to squeeze behind the sink.",ch,NULL,NULL,TO_ROOM);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29107 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs around for food on the kitchen floor.",ch,NULL,NULL,TO_ROOM);
	else if (random == 2)
	    act("$n looks up hungrilly at the cupboard.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM);
	else if (random == 4)
	{
	    act("$n walks over to $s food bowl.",ch,NULL,NULL,TO_ROOM);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n looks out the back door for cats to chase.",ch,NULL,NULL,TO_ROOM);
	return TRUE;
    }
    else if ((ch->in_room->vnum == 29100 || ch->in_room->vnum == 29101 || 
	ch->in_room->vnum == 29117) && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs a flower.",ch,NULL,NULL,TO_ROOM);
	else if (random == 2)
	    act("$n rolls around on the grass.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    act("$n tries to eat a blade of grass.",ch,NULL,NULL,TO_ROOM);
	else if (random == 4)
	    act("$n pounces on a grasshopper.",ch,NULL,NULL,TO_ROOM);
	else if (random == 5)
	    act("$n sniffs the air and growls playfully.",ch,NULL,NULL,TO_ROOM);
	return TRUE;
    }
    if (random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs the carpet.",ch,NULL,NULL,TO_ROOM);
	else if (random == 2)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM);
	else if (random == 3)
	    do_sit(ch,"");
	return TRUE;
    }

    if ( number_range(1,5) <= 3 )
    {
    	door = number_range(0,5);
	for ( door = 0; door <= 5; door++ )
	{
	    if (((pexit = ch->in_room->exit[door]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
	    {
		switch (door)
		{
		    case DIR_NORTH: north_ok = FALSE; countup -= 1; break;
		    case DIR_SOUTH: south_ok = FALSE; countup -= 1; break;
		    case DIR_EAST:  east_ok  = FALSE; countup -= 1; break;
		    case DIR_WEST:  west_ok  = FALSE; countup -= 1; break;
		    case DIR_UP:    up_ok    = FALSE; countup -= 1; break;
		    case DIR_DOWN:  down_ok  = FALSE; countup -= 1; break;
		}
	    }
	}

	if (countup < 1)
	{
	    if (number_percent() < 80)
		act("$n barks loudly",ch,NULL,NULL,TO_ROOM);
	    else
		do_sleep(ch,"");
	    return TRUE;
	}

	for ( ; ; )
	{
	    option = number_range(0,5);
	    if (((pexit = ch->in_room->exit[option]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
		continue;
	    if (countup > 1 && option == ch->specpower) continue;
	    if (IS_SET(pexit->exit_info, EX_CLOSED))
 	    {
		if (option == 0)
		    act("$n scratches at the north door.",ch,NULL,NULL,TO_ROOM);
		if (option == 1)
		    act("$n scratches at the east door.",ch,NULL,NULL,TO_ROOM);
		if (option == 2)
		    act("$n scratches at the south door.",ch,NULL,NULL,TO_ROOM);
		if (option == 3)
		    act("$n scratches at the west door.",ch,NULL,NULL,TO_ROOM);
		if (option == 4)
		    act("$n scratches at the upward door.",ch,NULL,NULL,TO_ROOM);
		if (option == 5)
		    act("$n scratches at the downward door.",ch,NULL,NULL,TO_ROOM);
		act("$n barks loudly at the door.",ch,NULL,NULL,TO_ROOM);
		for ( vch = char_list; vch != NULL; vch = vch_next )
		{
		    vch_next	= vch->next;
		    if ( vch->in_room == NULL || IS_NPC(vch) )
			continue;
		    if ( vch->in_room == ch->in_room )
			continue;
		    if ( vch->in_room->area == ch->in_room->area )
			send_to_char("You hear the sound of a dog barking nearby.\n\r",vch);
		}
		return TRUE;
   	    }
	    switch (option)
	    {
		default: break;
		case DIR_NORTH: ch->specpower = DIR_SOUTH; break;
		case DIR_SOUTH: ch->specpower = DIR_NORTH; break;
		case DIR_EAST:  ch->specpower = DIR_WEST;  break;
		case DIR_WEST:  ch->specpower = DIR_EAST;  break;
		case DIR_UP:    ch->specpower = DIR_DOWN;  break;
		case DIR_DOWN:  ch->specpower = DIR_UP;    break;
	    }
   	    move_char(ch,option);
	    break;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = o_next )
    {
	o_next = obj->next_content;
	if (number_range(1,2) == 1) continue;
	act( "$n sniffs $p.", ch, obj, NULL, TO_ROOM );
	if ( obj->item_type == ITEM_FOOD )
	{
	    act( "$n eats $p.", ch, obj, NULL, TO_ROOM );
	    extract_obj(obj);
	}
	return TRUE;
    }
    return TRUE;
}

bool world_peace = FALSE;
bool satan_ready = FALSE;

bool spec_jesus(CHAR_DATA *ch)
{
 static int state = 0;
 static CHAR_DATA *theone = NULL;
 CHAR_DATA *vch;
 int count = 0;
 int count2 = 0;
 if (!IS_NPC(ch)) return FALSE;

#if !defined(INVASION_1)
return FALSE;
#endif

 if (state == 0)
 {
    do_say(ch,"It is time.");
    world_peace = TRUE;
 }
 if (state == 2)
 {
    do_say(ch,"The world itself has become such of great sin and unfaithfulness,");
 }
 if (state == 3)
 {
   do_say(ch,"That the devil has become one with the world,");
 }
 if (state == 4)
 {
   do_say(ch,"It is time that we confess our sins, and defeat Satan once, and for all.");
 }
 if (state == 7)
 {
   do_say(ch,"Remember one thing as you fight the battle against evil,");
 }
 if (state == 10)
 {
   do_say(ch,"If we do not defeat him, he will take over us all, and we will all go to hell");
 }
 if (state == 11)
 {
   do_say(ch,"You must understand the importance of this mission, or you will die.");
 }
 if (state == 15)
 {
    for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
    {
	if (IS_NPC(vch)) continue;
	if (vch->trust > 6) continue;
	count++;
    }
    if (count == 0)
    {do_say(ch,"K I guess theres nobody here..");
     extract_char(ch,TRUE);
     return TRUE;}
    for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
    {
	if (IS_NPC(vch)) continue;
	if (vch->trust > 6) continue;
	if (++count2 == count) break;
    }
    theone = vch;
    sprintf(log_buf,"%s, You have been chosen to lead this fight against evil. Lead well and live long.",vch->name);
    do_say(ch,log_buf); 
 }
 if (state == 19)
 {
    do_say(ch,"&GYou will be given around 5 minutes to prepare, then the devil will &R\"Welcome\"&G you.");
 }

 state++;

 if (state == 60)
 {
    satan_ready = TRUE;
    act("$n floats into the heavens..",ch,0,0,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,get_room_index(1));
 }
 if (state == 50)
 {
    world_peace = FALSE;
    do_echo(ch,"&GJesus wishes all the good in the worlds, good luck, you will need it.");
 }
 return TRUE;
}

bool spec_killer(CHAR_DATA *ch)
{
if (!ch->fighting) return FALSE;

switch(number_range(1,2))
{
     case 1:
	spell_acid_blast(
skill_lookup("acid blast"),IS_NPC(ch) ? ch->level : ch->spl[1], ch,
(void *) ch->fighting);
if (ch->fighting)
	spell_acid_blast(
skill_lookup("acid blast"),IS_NPC(ch) ? ch->level : ch->spl[1], ch,
(void *) ch->fighting);
	break;
    default:
	spell_acid_breath(
skill_lookup("acid breath"),IS_NPC(ch) ? ch->level : ch->spl[1], ch,
(void *) ch->fighting);
if (ch->fighting)
	spell_acid_breath(
skill_lookup("acid breath"),IS_NPC(ch) ? ch->level : ch->spl[1], ch,
(void *) ch->fighting);
	break;
}
return TRUE;

}

#undef INVASION_1

bool spec_gummy_bear( CHAR_DATA *ch )
{
 OBJ_DATA *obj;
 CHAR_DATA *vch;
return FALSE;
 if (ch->fighting)
 {
    stop_fighting(ch, TRUE);
 if (IS_NPC(ch))
    smooregen(ch, 7000, 5000, 5000);
 else
    smooregen(ch, 90000, 40000, 40000);
 }

 if (ch->in_room->area != get_room_index(3001)->area)
 {
int a = number_range(1,100);
while (!get_room_index(3000+a)) a = number_range(1,100);

    do_say(ch, "Dum de dum dum dum, this is not where I want to be!");
    act("&G$n's eyes flicker with gummy energy!#n", ch, 0, 0, TO_ROOM);
    act("&GYour eyes flicker with gummy energy!#n", ch, 0, 0, TO_CHAR);
    char_from_room(ch);
    char_to_room(ch, get_room_index(3000+a));
    do_look(ch, "");
    act("&W$n appears in a blast of milk.", ch, 0, 0, TO_ROOM);
    act("&WYou appear in a new room in a blast of milk.", ch, 0, 0, TO_CHAR);
    do_say(ch, "lalala..fun fun lalala");
    return TRUE;
 }

 if (number_range(1, 2) == 1)
 {ROOM_INDEX_DATA *r = ch->in_room;
    switch(number_range(1, 6))
    {
	case 1: do_north(ch,"");break;
	case 2: do_east(ch,"");break;
	case 3: do_south(ch,"");break;
	case 4: do_west(ch,"");break;
	case 5: do_up(ch,"");break;
	case 6: do_down(ch,"");break;
    }
    if (ch->in_room != r)
    return TRUE;
 }

 switch(number_range(1, 10))
 {
    case 1:
	act("$n dances around in $s jelly jelly way.", 
	    ch,0,0,TO_ROOM);
	act("You dance around in your jelly jelly way.",
	    ch,0,0,TO_CHAR);
	do_say(ch, "lalala.");
	break;
    case 2:
	if (!ch->in_room->contents) break;
	obj = ch->in_room->contents;
	act("$n eats $p.", ch, obj, 0, TO_ROOM);
	act("You eat $p.", ch, obj, 0, TO_CHAR);
	obj_from_room(obj);
	obj->next_content = ch->jellybelly1;
	ch->jellybelly1 = obj;
	break;
    case 3:
	for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
	    if (IS_NPC(vch) && vch->spec_fun != ch->spec_fun && vch->spec_fun != ch->shapespec) break;

	if (!vch) return FALSE;

	act("$n eats $N.", ch, 0, vch, TO_NOTVICT);
	act("$n eats you.", ch, 0, vch, TO_VICT);
	act("You eat $N.", ch, 0, vch, TO_CHAR);

	vch->spec_fun = NULL;

	char_from_room(vch);
	vch->next_in_room = ch->jellybelly2;
	ch->jellybelly2 = vch;
	vch->in_room = get_room_index(9);
	break;
    case 4:
	if (!ch->jellybelly1)
	{
	    if (!get_obj_index(7) || number_range(1,3) == 1) return FALSE;
	    obj = create_object(get_obj_index(7),0);
	    obj_to_room(obj, ch->in_room);
	    act("$n burps up $p.", ch, obj, 0, TO_ROOM);
	    act("You burp up $p.", ch, obj, 0, TO_CHAR);
	    break;
	}
	obj = ch->jellybelly1;
	ch->jellybelly1 = ch->jellybelly1->next_content;
	obj->next_content = NULL;
	obj->in_room = NULL;
	obj_to_room(obj, ch->in_room);
	act("$n burps up $p.", ch, obj, 0, TO_ROOM);
	act("You burp up $p.", ch, obj, 0, TO_CHAR);
	break;
    case 5:
	if (!ch->jellybelly2)
	{
	    if (!get_mob_index(5)) return FALSE;
	    vch = create_mobile(get_mob_index(5));
	    char_to_room(vch, ch->in_room);
	    act("$n burps $N up.", ch, 0, vch, TO_ROOM);
	    act("You burp up $N.", ch, 0, vch, TO_CHAR);
	    break;
	}
	vch = ch->jellybelly2;
	ch->jellybelly2 = ch->jellybelly2->next_in_room;
	vch->in_room = NULL;
	vch->next_in_room = NULL;
	char_to_room(vch, ch->in_room);
	act("$n burps up $N.", ch, 0, vch, TO_ROOM);
	act("You burp up $N.", ch, 0, vch, TO_CHAR);
	break;
    case 6:
	act("$n dances under the moon.", ch, 0, 0, TO_ROOM);
	act("You dance under the moon.", ch, 0, 0, TO_CHAR);
	do_say(ch, "Wiggle wiggle! Wiggle wiggle!");
	break;
    case 7:
	act("$n wiggles his bum at you.", ch, 0, 0, TO_ROOM);
	act("You wiggle your bum at the people in the room.",ch,0,0,TO_CHAR);
	do_say(ch, "Wiggle wiggle! Wiggle wiggle!");
	break;
    case 8:
	act("$n bounces up and down on the floor.", ch, 0, 0, TO_ROOM);
	act("You bounce up and down on the floor.", ch, 0, 0, TO_CHAR);
	do_say(ch, "HAHAHAHAHAHAHAHAHAHAHA FUN FUN FUN!!!!!!!");
	break;
    case 9:
	act("You declare yourself a gummy bear!", ch, 0, 0, TO_CHAR);
	act("$n declares $mself a gummy bear!!!!", ch, 0, 0, TO_ROOM);
	break;
    case 10:
	do_say(ch, "I am..");
	do_say(ch, "I am the gummy");
	do_say(ch, "I am the gummy gummy gummy bear!");
	do_say(ch, "I will rule this world one day");
	do_say(ch, "But until then I dance!");
	act("$n struts $s stuff!", ch, 0, 0, TO_ROOM);
	act("You strut your stuff!", ch, 0, 0, TO_CHAR);
	break;
    }

 return TRUE;
} 

bool spec_hustler(CHAR_DATA *ch)
{
 CHAR_DATA *vict;

 if ((vict = ch->fighting) == NULL)
 {
    if (number_range(1,20) == 1)
    {/*
	do_berserk(ch,"");
	do_berserk(ch,"");
*/    }
    return TRUE;
 }

 switch(number_range(1,17))
 {AFFECT_DATA af;
    case 1: case 2:case 5:
	act("$n rams you in the face and you fall on the ground.",
	    ch,0,vict,TO_VICT);
	act("You ram $N in the face and $E falls on the ground.",
	    ch,0,vict,TO_CHAR);
	act("$n rams $N in the face and $E falls to the ground.",
	    ch,0,vict,TO_NOTVICT);
	stop_fighting(vict,TRUE);
	af.modifier = 0;
	af.location = APPLY_NONE;
	af.type = skill_lookup("ram");
	af.duration = 0;
	af.bitvector = AFF_STUNNED;
	affect_to_char(vict, &af);
	one_hit(ch, vict, skill_lookup("ram"), 1);
	one_hit(ch, vict, skill_lookup("ram"), 1);
	one_hit(ch, vict, skill_lookup("ram"), 1);
	break;
    case 3:case 4:
	if (!get_eq_char(vict,WEAR_WIELD)) break;
	act("$n slams your arms and you drop $p.",
	    ch,get_eq_char(vict,WEAR_WIELD),vict,TO_VICT);
	act("You slam $N in $M arms and $E drops $M weapons.",
	    ch,0,vict,TO_CHAR);
	act("$n slams $N in $M arms and $E drops $M weapons.",
	    ch,0,vict,TO_NOTVICT);
	{OBJ_DATA *obj = get_eq_char(vict,WEAR_WIELD);
	 obj_from_char(obj);
	 obj_to_room(obj,vict->in_room);
	}
	break;
 }

 return TRUE;
}

bool spec_fetch(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) { // initialization scheme
    if (ch->max_mana != 1500) {

    if (ch->max_hit & 1)
    {OBJ_INDEX_DATA *pobj;OBJ_DATA *obj;
     if ((pobj = get_obj_index(ch->max_move)))
     { obj = create_object(pobj,1);
	obj_to_char(obj, ch);
	act("$n creates $p.", ch, obj, 0, TO_ROOM);
	do_wear(ch, obj->name);
	REMOVE_BIT(ch->max_hit, 1);
     ch->max_move = 1;
     }
    }
    if (ch->max_hit & 2)
    {OBJ_INDEX_DATA *pobj;OBJ_DATA *obj;
     if ((pobj = get_obj_index(ch->max_mana)))
     { obj = create_object(pobj,1);
	obj_to_char(obj, ch);
	act("$n creates $p.", ch, obj, 0, TO_ROOM);
	do_wear(ch, obj->name);
     }
     ch->max_mana = 1;
     REMOVE_BIT(ch->max_hit, 2);
    }
    if (ch->max_hit & 4)
    {
     do_echo(0,ch->long_descr);
     free_string(ch->long_descr);
     ch->long_descr = str_dup("A mysterious fetch is here, glaring you in the eyes.\n");
     REMOVE_BIT(ch->max_hit, 4);
    }
    if (ch->max_hit & 8)
    {
	char buf[2000];
	strcpy(buf, "echo:");
	strcat(buf, ch->name);
	free_string(ch->name);
	ch->name = str_dup(buf);
	REMOVE_BIT(ch->max_hit, 8);
    }

    ch->max_hit = 3000000;
    ch->hit = ch->max_hit;
    ch->mana = 1500;ch->max_mana = 1500;
  }
  }

if (!str_prefix(ch->name, "echo:") && number_range(1,100)==1)
{
    char *p = ch->name + 5;
    while (*p == ' ')
	p++;

    do_echo(0,p);
}

  if (ch->fighting)
  {
    int a;
    int b = number_range(3,5);
     for (a = 0; a < b; a++)
	spell_magic_missile(skill_lookup("magic missile"), 300, ch, (void*)ch->fighting);
  }
  return TRUE;
}


void make_questspider(ROOM_INDEX_DATA *rm)
{
    CHAR_DATA *mob;

    if (!rm) return;
    if ((mob = create_mobile(get_mob_index(9))) == NULL)
	return;

    char_to_room(mob,rm);
    act("$n crawls out of the ground.", mob, 0, 0, TO_ROOM);
}

bool spec_questspider(CHAR_DATA *ch)
{
    if (!(ch->in_room->vnum >= 5200 && ch->in_room->vnum <= 5255))
    {ROOM_INDEX_DATA *rm = NULL;
	act("You are called back by your leader.", ch, 0, 0, TO_CHAR);
	act("$n is called back by $s leader.", ch, 0, 0, TO_ROOM);
	char_from_room(ch);
	while ((rm = get_room_index(number_range(5200,5255)))==NULL)
	    ;
	char_to_room(ch, rm);
	act("$n crawls out of a crack in the earth.", ch, 0, 0, TO_ROOM);
	act("You crawl out of a crack in the earth.", ch, 0, 0, TO_CHAR);
	return TRUE;
    }

    switch(number_range(1,9))
    {
	case 1: case 2: case 3: case 6: case 7: case 8:
	  if (!ch->fighting) {
	    switch (number_range(1,6))
	    {
		case 1: do_north(ch,"");
		break; case 2: do_east(ch,"");
		break; case 3: do_south(ch,"");
 		break; case 4: do_west(ch,"");
		break; case 5: do_up(ch,"");
		break; case 6: do_down(ch,"");
	    }}
	    return TRUE;

	case 4: case 9:
		if (number_range(1,5)!=1) return TRUE;
		act("$n splits in two.", ch, 0, 0, TO_ROOM);
		act("You split in two.", ch, 0, 0, TO_CHAR);
		make_questspider(ch->in_room);
		return TRUE;
	case 5:
		if (number_range(1,5)!=1) return TRUE;
		act("$n's life force disinigrates.", ch, 0, 0, TO_ROOM);
		stc("Your life force disinigrates.\n\r", ch);
		if (IS_NPC(ch)) {make_corpse(ch);extract_char(ch,TRUE);
			return TRUE;}
		else ch->hit -= (ch->hit+5);
		return TRUE;
    }

    return FALSE;
}

bool spec_kingdom_guard(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    KINGDOM_DATA *k;

    if (ch->fighting) return TRUE;

    if (!IS_NPC(ch)) {
int abc;
for (abc = 0; abc < 30; abc++)
    stc("&*Don't shift in to this mob!\n\r", ch);
    return TRUE;
}

    smooregen(ch,5000,5000,5000);

    if (IS_SET(ch->affected_by, AFF_SHADOWPLANE))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	act("&W$n snaps back in to this plane.", ch, 0, 0, TO_ROOM);
    }

    if ((k = find_kingdom(ch->kingdom)) == NULL) return TRUE;

    for (vch = char_list; vch; vch = vch->next)
    {
	if (IS_NPC(vch)) continue;
	if (IS_IMMORTAL(vch)) continue;
	if (!vch->in_room) continue;
	if (find_kingdom(vch->in_room->kingdom) != k) continue;
	if (find_kingdom(vch->kingdom) == k) continue;
	if (vch->hit < -4) continue;
	if (vch->fighting && IS_NPC(vch->fighting) && vch->fighting->spec_fun == ch->spec_fun) continue;
	if (vch->pcdata && vch->pcdata->chobj) continue;
	if (IS_SET(vch->affected_by, AFF_SHADOWPLANE))
	{
	    REMOVE_BIT(vch->affected_by, AFF_SHADOWPLANE);
	    act("$n snaps you out of the shadow plane.", ch, 0, vch, TO_VICT);
	    act("$n snaps $N out of the shadow plane.", ch, 0, vch, TO_NOTVICT);
	}
	if (IS_SET(vch->affected_by, AFF_ETHEREAL))
	{
	    REMOVE_BIT(vch->affected_by, AFF_ETHEREAL);
	    act("$n waves $s hand and $N becomes surreal.", ch, 0, vch, TO_NOTVICT);
	    act("$n waves $s and you become surreal.", ch, 0, vch, TO_VICT);
	}

	break;
    }

    if (!vch) return TRUE;

    act("$n notices an intruder and runs to action!", ch,0,0,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, vch->in_room);

    act("&R$n&R screams out in a mad frenzy, \"&YTHOU SHALT NOT BE IN THESE CORRIDORS.&R\".", ch, 0, 0, TO_ROOM);
    multi_hit(ch, vch, TYPE_HIT);

    return TRUE;
}


bool spec_kingdom_healer( CHAR_DATA *ch ) 
{
    KINGDOM_DATA *k = find_kingdom(ch->kingdom);
    CHAR_DATA *vch, *vch_n;

    if (!IS_NPC(ch)) 
    {
	int abc;

	for (abc = 0; abc < 30; abc++)
	    stc("&*Don't shift in to this mob!\n\r", ch);
	    return TRUE;
    }

    for ( vch = ch->in_room->people ; vch; vch = vch_n)
    {
	vch_n = vch->next_in_room;
	if (vch==ch) continue;

	if (find_kingdom(vch->kingdom) != k && number_range(1,3)==1) 
	{
	    act("&D$n burns $N.", ch, 0, vch, TO_NOTVICT);
	    act("&D$n burns you.", ch, 0, vch, TO_VICT);
	    smooregen(vch,0-NR(500,1500),0-NR(500,1500),0-NR(500,1500));
	    continue;
	}
	else if (find_kingdom(vch->kingdom) != k) continue;

	smooregen(vch,NR(100,300),NR(100,300),NR(100,300));

	if (number_range(1,25)==1)
	{
	    act("&W$n prays over $N and light shines brightly over $m.", ch, 0, vch, TO_NOTVICT);
	    act("&W$n prays over you, and light shines bright on you.", ch, 0, vch, TO_VICT);
	    smooregen(vch, IS_NPC(vch)?8000:NR(3000,6000), NR(4000,7000), NR(4000,7000));
	}
	else if (number_range(1,16)==1)
	    spell_smoo(skill_lookup("guardian"), 500, ch, vch);
     }

    return FALSE;
}


