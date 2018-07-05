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

void do_sclaws( CHAR_DATA *ch, char *argument )
{

   if (IS_NPC(ch))
	return;

   if (ch->power[DISC_WERE_LUNA] < 5)
   {
	send_to_char("You need level 5 in Luna to use Silver Claws.\n\r",ch);
	return;
   }

   if (!IS_SET(ch->newbits, NEW_SCLAWS))
   {
	send_to_char("Your claws turn into pure silver!\n\r",ch);
        act("$n's claws turn to pure silver!",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->newbits, NEW_SCLAWS);
	return;
   }

   if (IS_SET(ch->newbits, NEW_SCLAWS))
   {
	send_to_char("Your claws return to their normal state.\n\r",ch);
	act("$n's claws are no longer composed of pure silver.\n\r",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_SCLAWS);
	return;
   }

}

void do_moonbeam( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];

 
    argument = one_argument( argument, arg1 );
  
 
    if ( IS_NPC(ch) )
    	return;
 
 
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
      return;
    }
 
    if (ch->power[DISC_WERE_LUNA] < 8 )
    {
	send_to_char("You need to obtain level 8 in Luna to use Moonbeam.\n\r",ch);
    	return;
    }
 

    if (ch->mana < 500)
    {
	send_to_char("You do not have enough energy to summon a Moonbeam.\n\r",ch);
	return;
    }
 
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Who do you wish to strike?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
   	 send_to_char( "They aren't here.\n\r", ch );
	 return;
    }

    if (is_safe(ch,victim) == TRUE) return;
  
    	act("$N is struck by a huge moonbeam.", ch, NULL, victim,TO_CHAR);
	act("$N is struck by a deadly beam of moonlight from $n.", ch, NULL, victim,TO_NOTVICT);
	act("You are struck by a deadly beam of moonlight!", ch, NULL, victim,TO_VICT);
    if (IS_GOOD(victim))
	victim->hit -= 400;
    if (IS_EVIL(victim))
	victim->hit -= 500;
    if (IS_NEUTRAL(victim))
	victim->hit -= 350;
	ch->mana -= 500;
    if (ch->fighting == NULL)
 	set_fighting(ch,victim);
	update_pos(victim);
	WAIT_STATE(ch,36);
	return;
    
}


void do_moongate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if (ch->power[DISC_WERE_LUNA] < 6)
    {
      send_to_char("You need to obtain level 6 Luna to use moongate.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0')
    {
        send_to_char( "Who do you wish to gate to?\n\r", ch );
	return;
    }
 
    if ( (victim = get_char_world( ch, arg ) ) == NULL )
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
    if (IS_IMMORTAL(victim))
    {
	send_to_char("Not to immortals!\r\n", ch);
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

 
    if ( (victim == ch) 
    ||   victim->in_room == NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );

    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = ch->power[DISC_WERE_LUNA] / 3;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL, TO_ROOM );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act( "$p appears in front of $n in a beam of moonlight.", ch, obj, NULL, TO_ROOM );
    act( "$p appears in front of you in a beam of moonlight.", ch, obj, NULL, TO_CHAR );
    act( "$p appears in front of $n in a beam of moonlight.", victim, obj, NULL, TO_ROOM );
    act( "$p appears in front of you in a beam of moonlight.", ch, obj, victim, TO_VICT );
    return;
}

void do_gmotherstouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg1 );
  
 
    if ( IS_NPC(ch) )
    	return;
 
 
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
      	return;
    }
 
    if (ch->power[DISC_WERE_LUNA] < 4 )
    {
	send_to_char(
	"You need to obtain level 4 in Luna to use Grandmother's Touch.\n\r",ch);
    	return;
    }
 

    if (ch->mana < 400)
    {
	send_to_char("You do not have enough energy to summon the Grandmother's Touch.\n\r",ch);
	return;
    }
 
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Who is in need of your touch?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
   	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim->hit >= victim->max_hit)
    {
	send_to_char("They are already full.\r\n", ch);
	return;
    }  
        
    if (ch->fighting != NULL)
    {
	if (ch != victim)
    	act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_ROOM);
	stc("You are surrounded by a beam of moonlight, and feel restored.\r\n", victim);
	victim->hit += 500;
	ch->mana -= 300;
	WAIT_STATE(ch,16);
	return;
    }

    if (ch->fighting == NULL)
    {
	send_to_char("You are surrounded by a beam of moonlight, and feel restored.\r\n", victim);
	if (ch != victim)
        act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_ROOM);
	victim->hit += 600;
	ch->mana -= 400;
	WAIT_STATE(ch,6);
	return;
    }
}


void do_motherstouch( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
	return;

    if (ch->power[DISC_WERE_LUNA] < 3 )
    {
	send_to_char("You need to obtain level 3 in Luna to use Mother's Touch.\n\r",ch);
	return;
    }

    if (ch->mana < 400)
    {
	send_to_char("You don't have enough energy to use Mother's Touch.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You are touched by a beam of light and feel healed.\n\r",ch);
	act("$n is touched by a beam of moonlight, and looks refreshed.\n\r",ch,NULL,NULL,TO_ROOM);
        ch->hit += 500;
        ch->mana -= 400;
	WAIT_STATE(ch,16);
	return;
    }

    if (ch->fighting == NULL)
    {
	send_to_char("You are surrounded by a beam of light, and feel healed.\n\r",ch);
	act("$n is surrounded by a beam of moonlight, and looks much more refreshed.\n\r",ch,NULL,NULL,TO_ROOM);
	ch->hit += 500;
	ch->mana -= 400;
	WAIT_STATE(ch,8);
	return;
    }

}

void do_flameclaws( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch))
	return;

    if (ch->power[DISC_WERE_LUNA] < 1)
    {
	send_to_char("You need to obtain level 1 in the Luna totem to use flame claws.\n\r",ch);
	return;
    }

    if (IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	send_to_char("The flames surrounding your claws fade.\n\r",ch);
	act("The flames surrounding $n's claws fade.\n\r",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
	return;
    }

    if (!IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	send_to_char("Your claws are surrounded by blazing flames.\n\r",ch);
        act("$n's claws are surrounded by blazing flames.\n\r",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->newbits, NEW_MONKFLAME);
	return;
    }

}

void do_moonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }
    
    if (ch->power[DISC_WERE_LUNA] < 2)
    {
	send_to_char("You must attain level 2 in Luna before you can create moon armour.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("What piece of moon armour do you wish to create?.\n\r", ch);
    send_to_char("A Ring, Plate, or Belt? \n\r",ch);
    return;
    }


    if      (!str_cmp(arg,"ring"     )) vnum = 29696;
    else if (!str_cmp(arg,"belt"   )) vnum = 29697;
    else if (!str_cmp(arg,"plate"    )) vnum = 29698;
    else
    {
	send_to_char("What piece of moon armour do you wish to create?.\n\r",ch);
    send_to_char("A Ring, Plate, or Belt? \n\r",ch);
    return;
    }
    if (ch->practice < 65)
    {
	send_to_char("It costs 65 primal to create a piece of moon armour\n\r",ch);
	return;
    }
   
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Infidel.\n\r",ch);
	return;
    }
    ch->practice -= 65;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_CHAR);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_ROOM);
    return;
}



void do_rend(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BOAR] < 7)
  {
  send_to_char("You need Boar 7 to Rend equipment.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_REND))
  {
  send_to_char("You will now ruthlessly rend your opponents eq.\n\r",ch);
  SET_BIT(ch->newbits, NEW_REND);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_REND))
  { 
  send_to_char("Your no longer mean to your enemies eq.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_REND);
  return;
  }
return;
}

void do_skin(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 7)
  {
  send_to_char("You need Bear 7 to toughen your skin.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SKIN))
  {
  send_to_char("Your skin hardens.\n\r",ch);
  ch->armor -= 100 ;
  SET_BIT(ch->newbits, NEW_SKIN);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SKIN))
  { 
  send_to_char("Your skin softens.\n\r",ch);
  ch->armor += 100;
  REMOVE_BIT(ch->newbits,NEW_SKIN);
  return;
  }
return;
}

void do_jawlock(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_RAPT] < 8)
  {
  send_to_char("You need Raptor 8 to use locked jaw.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
  send_to_char("Your jaw grows stronger and more ready to clamp on your enemy.\n\r",ch);
  SET_BIT(ch->newbits, NEW_JAWLOCK);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  { 
  send_to_char("Your jaw weakens.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
  return;
  }
return;
}

void do_perception(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_RAPT] < 3)
  {
  send_to_char("You need Raptor 3 to heighten your Perception.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_PERCEPTION))
  {
  send_to_char("You become more aware of stealthy enemies.\n\r",ch);
  SET_BIT(ch->newbits, NEW_PERCEPTION);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_PERCEPTION))
  { 
  send_to_char("Your perception dulls.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_PERCEPTION);
  return;
  }
return;
}

void do_roar(CHAR_DATA *ch,char *argument)
{
CHAR_DATA *victim;
char buf[MAX_STRING_LENGTH];
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_WEREWOLF))
{send_to_char("Huh?\n\r",ch);return;}
if (ch->power[DISC_WERE_BEAR] < 6)
{
send_to_char("You need bear 6 to roar.\n\r",ch);
return;
}
if ((victim=ch->fighting) == NULL)
{
send_to_char("You arent fighting anyone.\n\r",ch);
return;
}
sprintf(buf,"%s roars and screams, intimidating the hell out of %s.",
ch->name,victim->name);
act(buf,ch,NULL,NULL,TO_ROOM);
sprintf(buf,"You roar and scream, intimidating the hell out of %s.\n\r",
victim->name);
send_to_char(buf,ch);
do_flee(victim,"");
WAIT_STATE(ch,20);
return;
}
 

void do_quills(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 5)
  {
  send_to_char("You need Bear 5 to extend your quills.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_QUILLS))
  {
  send_to_char("Sharp quills extend from your back.\n\r",ch);
  SET_BIT(ch->newbits, NEW_QUILLS);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_QUILLS))
  { 
  send_to_char("Your quills sink into your back.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_QUILLS);
  return;
  }
return;
}

void do_slam(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 8)
  {
  send_to_char("You need Bear 8 to attempt to shoulder slam.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SLAM))
  {
  send_to_char("You will now shoulder slam when you get a chance.\n\r",ch);
  SET_BIT(ch->newbits, NEW_SLAM);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SLAM))
  { 
  send_to_char("You now dont shoulderslam when given a chance.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_SLAM);
  return;
  }
return;
}
 
void do_congregation(CHAR_DATA *ch, char *argument )
{
 
    char buf [MAX_INPUT_LENGTH];
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int blah;
    AFFECT_DATA af;
    argument=one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
if (arg[0]=='\0')
{
send_to_char("Summon what?\n\r",ch);
send_to_char("L1-Scorpion, L2-Cobra, L3-Lynx, L4-Timber, L5-Boar\n\r",ch);
send_to_char("L6-Rhino, L7-Black, L8-Whitewolf, L9-Clone, L10-Fenris.\n\r",ch);
return;
}
 
blah=ch->power[DISC_WERE_CONG];
 
    if (ch->pcdata->followers > 0)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;
    victim=create_mobile( get_mob_index( 15402 ) );
 if (!str_cmp(arg,"scorpion")&& blah>0)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("scorpion");
    victim->short_descr=str_dup("a Huge Scorpion");
    victim->long_descr=str_dup("a Huge Scorpion stands here.");
    victim->level = 40;
    victim->hit = 300;
    victim->max_hit = 300;
    victim->hitroll = 40;
    victim->damroll = 40;
}
else if (!str_cmp(arg,"cobra")&& blah>1)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("huge cobra");
    victim->short_descr=str_dup("a Huge Cobra");
    victim->long_descr=str_dup("a Huge Cobra stands here.");
    victim->level = 50;
    victim->hit = 400;
    victim->max_hit = 400;
    victim->hitroll = 50;
    victim->damroll = 50;
}
else if (!str_cmp(arg,"lynx")&&blah>2)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("sneaky lynx");
    victim->short_descr=str_dup("a Sneaky Lynx");
    victim->long_descr=str_dup("a stealthy lynx stands here.");
    victim->level = 60;
    victim->hit = 500;
    victim->max_hit = 500;
    victim->hitroll = 60;
    victim->damroll = 60;
}
else if ((!str_cmp(arg,"timber")||!str_cmp(arg,"wolf"))&& blah>3)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("timber wolf");
    victim->short_descr=str_dup("a Huge Timber Wolf");
    victim->long_descr=str_dup("a Huge Timber Wolf stands here.");
    victim->level = 70;
    victim->hit = 2000;
    victim->max_hit = 500;
    victim->hitroll = 70;
    victim->damroll = 70;
}
else if (!str_cmp(arg,"boar")&&blah>4)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("boar");
    victim->short_descr=str_dup("a Huge Boar");
    victim->long_descr=str_dup("a Huge Boar stands here.");
    victim->level = 100;
    victim->hit = 1000;
    victim->max_hit = 1000;
    victim->hitroll = 80;
    victim->damroll = 80;
}
else if ((!str_cmp(arg,"rhino")||!str_cmp(arg,"white"))&&blah>5)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("white rhino");
    victim->short_descr=str_dup("a Huge White Rhino");
    victim->long_descr=str_dup("a Huge Rhino stands here.");
    victim->level = 125;
    victim->hit = 1500;
    victim->max_hit = 1500;
    victim->hitroll = 90;
    victim->damroll = 90;
}
else if (!str_cmp(arg,"black")&& blah>6)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("black rhino");
    victim->short_descr=str_dup("a Huge Black Rhino");
    victim->long_descr=str_dup("a Huge Rhino stands here.");
    victim->level = 150;
    victim->hit = 2000;
    victim->max_hit = 2000;
    victim->hitroll = 100;
    victim->damroll = 100;
}
else if (!str_cmp(arg,"whitewolf")&& blah>7)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("white wolf");
    victim->short_descr=str_dup("a Huge White Wolf");
    victim->long_descr=str_dup("a Huge Wolf stands here.");
    victim->level = 200;
    victim->hit = 3000;
    victim->max_hit = 3000;
    victim->hitroll = 150;
    victim->damroll = 150;
}
else if (!str_cmp(arg,"clone")&&blah>8)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup(ch->name);
    victim->short_descr=str_dup(ch->name);
    victim->long_descr=str_dup(ch->name);
    victim->level = 400;
    victim->hit = 4000;
    victim->max_hit = 4000;
    victim->hitroll =200;
    victim->damroll =200;
} 
else if (!str_cmp(arg,"fenris")&&blah>9)
{
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("fenris");
    victim->short_descr=str_dup("Fenris Wolf");
    victim->long_descr=str_dup("Fenris Wolf stands here.");
    victim->level =500;
    victim->hit = 5000;
    victim->max_hit = 5000;
    victim->hitroll = 200;
    victim->damroll = 200;
}
else
{
send_to_char("Your incorrect call attracts a weak partner\n\r",ch);
}
 
    sprintf(buf,"%s runs out of nowhere.\n\r",victim->short_descr);
    send_to_char(buf,ch);
    
    act(buf,ch,NULL,NULL,TO_ROOM);
   sprintf(buf,"%s\n\r",victim->long_descr);
     free_string(victim->long_descr);
     victim->long_descr=str_dup(buf);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    return;
}

void do_shred( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
    argument = one_argument( argument, arg );
 
 
    if(!IS_CLASS(ch,CLASS_WEREWOLF))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_WERE_RAPT] < 7)
    {
    send_to_char("You need Raptor 7 to Shred.\n\r",ch);
    return;
    }
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Shred whom?\n\r", ch );
	return;
    }
 
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( victim == ch )
    {
        send_to_char( "How can you shred yourself?\n\r", ch );
	return;
    }
 
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't shred a fighting person.\n\r", ch );
	return;
    }
 
    if ( victim->hit < victim->max_hit )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
    if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    {
    send_to_char("You must be ShadowPlaned to Shred!\n\r",ch);
    return;
    }
do_shadowplane(ch,"");
if (is_safe(ch,victim)==TRUE) return;

    dam = char_damroll(ch);

    if (IS_NPC(victim))
	dam += number_range(300, 600);
    else
	dam += number_range(100, 200);

WAIT_STATE(ch,16);
if (ch->power[DISC_WERE_RAPT] >6)
damage(ch,victim,dam,gsn_shred);
damage(ch,victim,dam,gsn_shred);
if (ch->power[DISC_WERE_RAPT] > 7)
damage(ch,victim,dam,gsn_shred);
if (ch->power[DISC_WERE_RAPT] == 10)
damage(ch,victim,dam,gsn_shred);
    return;
}

void do_talons( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
 
char buf[MAX_STRING_LENGTH];
 
int dam;
if (IS_NPC(ch)) return;
 
if (!IS_CLASS(ch,CLASS_WEREWOLF))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_WERE_RAPT] < 10)
{
send_to_char("You need level 10 Raptor to use talons.\n\r",ch);
    return;
}
 
if (ch->move < 300)
{
    send_to_char("You are too exhausted.\r\n", ch);
    return;
}
 
if ( ( victim = ch->fighting ) == NULL )
{
send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
 
}
WAIT_STATE( ch, PULSE_VIOLENCE-2 );
if (!IS_NPC(victim))
{
dam = ch->power[DISC_WERE_RAPT] * 150;
}
else if (IS_NPC(victim))
{
dam = ch->power[DISC_WERE_RAPT] * 150;
}
if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
{
    if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.9;
}
 
dam += number_range(1,30);
if (IS_INFORM(victim,GOLEM_CRYSTAL)) dam /= 10;
if ( dam <= 0 )
   dam = 1;
sprintf(buf,"Your talons strike $N incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's talons strike you incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's talons strike $N incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
  
send_to_char("\n\r",ch);
hurt_person(ch, victim, dam);
ch->move -= number_range(200, 300);
return;
}
 
 
 
void do_devour( CHAR_DATA *ch, char *argument )
{
char arg[MAX_INPUT_LENGTH];
OBJ_DATA *obj;
char buf[MAX_INPUT_LENGTH];
int hpgain;
 
one_argument( argument, arg );
if (!IS_CLASS(ch,CLASS_WEREWOLF))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_WERE_RAPT] < 5)
{
send_to_char("You require Raptor 5 to devour.\n\r",ch);
return;
}
 
 
if ( arg[0] == '\0' )
{
    send_to_char("Devour what?\n\r",ch);
    return;
    }
obj = get_obj_list( ch, arg, ch->in_room->contents );
if ( obj == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
    return;
}
if (obj->item_type != ITEM_CORPSE_NPC)
{
    act( "You are unable to devour $p.", ch, obj, 0, TO_CHAR);
return;
}
else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
obj->chobj->pcdata->obj_vnum != 0)
{
    act( "You are unable to devour $p.", ch, obj, 0, TO_CHAR);
    return;
}
hpgain = number_range(100,250);
ch->hit += hpgain;        
if (ch->hit > ch->max_hit)
{
ch->hit = ch->max_hit;
} 
sprintf( buf,"You devour %d hp of life from $p.", hpgain );
act( buf, ch, obj, NULL, TO_CHAR );
act( "You devour $p.", ch, obj, NULL, TO_CHAR );
act( "$p is devoured by $n.", ch, obj, NULL, TO_ROOM );
extract_obj( obj );
return;
}
 
void do_wolfritual( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Need two arguments.\n\r", ch );
	send_to_char( "Syntax is: ritual <target> <action>\n\r", ch );
	send_to_char( "Actions are leave (self only), banish, induct, and shaman.\n\r", ch );
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


    if ( !IS_CLASS(victim,CLASS_WEREWOLF) )
    {
	send_to_char( "But they are not a werewolf!\n\r", ch );
	return;
    }

    if ( (ch->clan && victim->clan && !str_cmp(ch->clan, victim->clan)) ||
	( !str_cmp(victim->clan,"") && !str_cmp(arg2,"induct")) )
    {

	if ( ch->generation >= victim->generation && ch != victim )
	{
	    send_to_char( "You can only grant your favour to someone of a lower generation.\n\r", ch );
	    return;
	}

	if ( !str_cmp(arg2,"shaman") )
	{
	    if  (ch->generation > 2  )
	    {
		send_to_char("Only the chief can grant shaman powers!\n\r", ch);
		return;
	    }
	    if (IS_SET(victim->special,SPC_PRINCE))
	    {
		act("You remove $N's shaman privilages!",ch,NULL,victim,TO_CHAR);
		act("$n removes $N's shaman privilages!",ch,NULL,victim,TO_NOTVICT);
		act("$n removes your shaman privilages!",ch,NULL,victim,TO_VICT);
		if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
		REMOVE_BIT(victim->special,SPC_PRINCE);return;
	    }
	    act("You make $N a shaman!",ch,NULL,victim,TO_CHAR);
	    act("$n has made $N a shaman!",ch,NULL,victim,TO_NOTVICT);
	    act("$n has made you a shaman!",ch,NULL,victim,TO_VICT);
	    SET_BIT(victim->special,SPC_PRINCE);
	    if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
	    free_string(victim->lord);
	    if (ch->generation == 1)
		sprintf(buf,"%s", ch->pcdata->switchname);
	    else
		sprintf(buf, "%s %s",  ch->lord, ch->pcdata->switchname);
	    victim->lord = str_dup(buf);
	    victim->generation = ch->generation + 1;
	    return;
	}
	else if ( !str_cmp(arg2,"banish") )
	{
	    if ( !IS_SET(ch->special,SPC_PRINCE) && ch->generation > 2 )
	    {
		send_to_char("Only shamans and chiefs can banish members.\n\r", ch);
		return;
	    }
	    act("You banish $N from your Tribe!",ch,NULL,victim,TO_CHAR);
	    act("$n has made $N a Ronin!",ch,NULL,victim,TO_NOTVICT);
	    act("$n has made you a Ronin!",ch,NULL,victim,TO_VICT);
	    free_string(victim->clan);
	    victim->clan = str_dup( "" );
	    return;
	}
	else if ( !str_cmp(arg2,"leave") && ch == victim 
	    && victim->generation > 2)
	{
	    act("You leave your tribe!!",ch,NULL,victim,TO_CHAR);
	    act("$n has left $s tribe!",ch,NULL,victim,TO_ROOM);
	    free_string(victim->clan);
	    victim->clan = str_dup( "" );
	    return;
	}

	else if ( !str_cmp(arg2,"induct") && victim->generation > 2
	    && ch->generation < 4 && strlen(victim->clan) < 2)
	{
	    act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
	    act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	    act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
	    free_string(victim->clan);
	    victim->clan = str_dup( ch->clan );
	    return;
	}

	send_to_char( "Syntax is: ritual <target> <action>\n\r", ch );
	send_to_char( "Actions are leave (self only), banish, and shaman.\n\r", ch );
	return;
    } else {
	send_to_char( "They are not of your tribe.\n\r", ch );
	return;
    }
}

/* why the hell would someone use a recursive function for run? re-writen
   by smoo. */
void do_run(CHAR_DATA *ch, char *argument)
{
    int dir = 0, count;
    EXIT_DATA *ex;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("Run north, south, east or west?\n\r", ch);
	return;
    }

    if (ch->power[DISC_WERE_RAPT] < 2)
    {
	stc("You need raptor 2 to use run.\n\r", ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	stc("Not with all this sticky webbing on.\n\r", ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
	stc("You are unable to move tangled in these tendrils.\n\r", ch);
	return;
    }

    if (ch->move <= 0)
    {
	stc("You are too exhausted.\n\r", ch);
	return;
    }

    dir = !str_cmp(argument,"s") || !str_cmp(argument, "south") ? 2
	: !str_cmp(argument,"n") || !str_cmp(argument, "north") ? 0
	: !str_cmp(argument,"e") || !str_cmp(argument, "east" ) ? 1
	: !str_cmp(argument,"w") || !str_cmp(argument, "west" ) ? 3
	: 4;

    if (dir == 4)
    {
	stc("You may only run north, east, south or west.\n\r", ch);
	return;
    }

    for (count = 0, ex = ch->in_room->exit[dir]; ex && count < 50 && ex->to_room;
	 ex = ch->in_room->exit[dir], count++)
    {
	sprintf(log_buf, "&wYou run %s.\n\r", dir_name[dir]);
	stc(log_buf, ch);
	act("$n runs $t.", ch, dir_name[dir], 0, TO_ROOM);
	char_from_room(ch);
	char_to_room(ch,ex->to_room);
	sprintf(log_buf, "&g%s&w\n\r", ch->in_room->name);
	stc(log_buf, ch);
    }

    act("$n comes to a screeching halt.", ch, 0, 0, TO_ROOM);
    stc("You stop running.\n\r", ch);
    WAIT_STATE(ch,10);
    return;
}

void do_adrenorush(CHAR_DATA*ch, char*argument)
{if (!IS_CLASS(ch, CLASS_WEREWOLF))
{send_to_char("Fuck you, you can't control your adrenaline.\n\r",ch);
return;
}

if(ch->move<ch->max_move/3)
{send_to_char("You need at least 1 third of your movement points\n\r",ch);
return;
}
if (!ch->fighting)
{stc("You need to be fighting to use this.\n\r",ch);
return;
}
ch->move -=ch->max_move/3;
act("$n recieves a sudden adrenaline rush.", ch, 0, 0, TO_ROOM);
act("You recieve a sudden adrenaline rush.", ch, 0, 0, TO_CHAR);

ch->pcdata->powers[WWPOW_ADRENORUSH] +=2;
return;
}
