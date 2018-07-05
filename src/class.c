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

/* By Smoo */
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

/*
the structure is:
the class number/definition (as defined in class.h),
class name, starting trust(when classed, for invis things), starting 
level (spells) bits (as defined in class.h)
*///add new classes to the bottom or you'll fuck up classconfig
const   struct  strctr_class_table  class_table       []            =
{
  {0,		"Mortal",	2,	2,	0,	BIT_CLOSED},
  {CLASS_DEMON,	"Demon",	4,	3,	1,	0	},
  {CLASS_MAGE,	"Mage",		5,	4,	3,	0	},
  {CLASS_WEREWOLF,"Werewolf",	3,	3,	7,	0	},
  {CLASS_VAMPIRE,"Vampire",	3,	3,	4,	0	},
  {CLASS_HIGHLANDER,"Highlander",3,	3,	2,	0	},
  {CLASS_DROW,	"Drow", 	4,	3,	5,	0	},
  {CLASS_MONK,	"Monk",		6,	3,	4,	0	},
  {CLASS_NINJA,	"Ninja",	5,	3,	4,	0	},
  {CLASS_AMAZON,"Amazon",	3,	3,	3,	0	},
  {CLASS_JEDI,	"Jedi",		5,	5,	6,	0	},
  {CLASS_SHAPE,	"Shape",	4,	4,	4,	0	},
  {CLASS_GOLEM,	"Golem",	3,	4,	10,	0	},
  {CLASS_UNICORN // hybrid placed in to make it tierIII only.
   | HYBRID	,"Unicorn",	3,	3,	5,	0	},
  {CLASS_MOOGLE,"Moogle",	4,	4,	5, 	BIT_CLOSED|BIT_DONTCLASS|BIT_PTEST_TOGGLE|BIT_HIDDEN},
  {CLASS_OBJECTOR, "Objector",  3,      3,      4,      BIT_DONTCLASS|BIT_CLOSED|BIT_HIDDEN}, 
  {CLASS_HYDRA, "Hydra",	4,	4,	4,	BIT_CLOSED|BIT_HIDDEN},
  {CLASS_DRUID, "Druid",	5,	5,	4,	0	},
  {CLASS_FLUFF,	"Fluffie",	4,	4,	5,	BIT_CLOSED|BIT_PTEST_TOGGLE},

// HYBRID CLASSES - please continue to put the non-hybrid classes BELOW
// the hybrid classes.
  {CLASS_KNIGHT,"Knight",	5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_LICH,	"Lich",		5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_SAMURAI,"Samurai",	5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_SUCCUBUS,"Succubus",	5,	5,	3,	BIT_PTEST_TOGGLE},
  {CLASS_GARGOYLE,"Gargoyle",	5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_MOUNTED,"Mounted",	5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_GAROU,	"Garou",	5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_SITH,  "Sith",		5,	5,	3,	BIT_PTEST_SAFE},
  {CLASS_WHATEV, "Whatever", 5,5,6,
BIT_PTEST_TOGGLE|BIT_CLOSED|BIT_HIDDEN|BIT_NOWHO},
  {0,		"",		0,	0,	0,	0	}//keep this as
								 //last
};

const char clan_rank[10][16][100] =
{
 {"", "Lemure", "Nupperibo", "Spinagon", "Barbazu", "Abishai",
  "Osyluth", "Hamatula","Erinyes", "Amnizuo", "Cornugon", "Gelugon", 
  "Pit Fiend", "Pit Lord", "Pit Lord", "&RSatan's Right Hand"},
 {"","&MPurple&C","&RRed&C","&BBlue&C","&GGreen&C","&YYellow&C",
  "&DBlack&C","&WWhite&C"},
 {"", "&GGaia's &YAvatar&C", "&CChief&C", "&RShaman&C",
  "&GPackMember&C", "&GPackMember&C", "&GPackMember&C", "&GPackMember&C",
"&GPackMember&C", "&GPackMember&C", "&GPackMember&C" },
 {"", "&CSupreme &RMaster&C", "&WFounder&C of ", "&GPrince&C of ",
"&YYoungling of &C", "&YYoungling of &C", "&YYoungling of &C",
"&YYoungling of &C", "&YYoungling of &C", "&YYoungling of &C",
"&YYoungling of &C", "&YYoungling of &C", "&YYoungling of &C" },
 {"", "Lloth's Avatar", "House Matron", "Noble", "Childae", "Unwanted"},
 {"", "Master", "Nabishyo", "Takuha", "Ninja", "Silibing"},
 {"","General","Moogle Knight","Soldier","Fluffy","Baby Moogle"},
//  |
//7\./
//  `
{""," Carmen", " Monger", " Darshel", " Manoa", " Serra",
  " Noa", " Harmen", " Gures", " Blaze", " Penutila", " Shereed",
  " Blezeo", " Aztrx", " Seductress", " Temptress"},
};

#define CLAN(xch) ((xch->clan && str_cmp(xch->clan,"") && str_cmp(xch->clan, "(null)")) ? xch->clan : IS_CLASS(xch, CLASS_WEREWOLF) ? "Ronin" : IS_CLASS(xch, CLASS_VAMPIRE) ? "Caitiff" :"Rogue") 
#define TABLE(a, b) (clan_rank[a][b > 15 ? 15 : b])


char *get_class_name( CHAR_DATA *ch )
{
 static char retur[MSL];

    if (IS_CLASS(ch, CLASS_SUCCUBUS))
    {
	sprintf(retur, "&R_[-[&B%s%s&R]-]_", str_cmp(CLAN(ch), "rogue")?CLAN(ch):"Succubus", str_cmp(CLAN(ch), "rogue")?TABLE(7,ch->generation):"");
    }
    else if (IS_CLASS(ch, CLASS_SITH))
    {
	sprintf(retur, "&R^&D{&r***&B/&c\\&GSith &C<>&G Master&c/&B\\&r***&D}&R^");
    }
    else if (IS_CLASS(ch, CLASS_SAMURAI))
    {
	sprintf(retur, "&r--&R==&D==&Y-&W|&Y*&W>");
    }
    else if (IS_CLASS(ch, CLASS_LICH))
    {
	sprintf(retur, "&DX:x&RDa&rrk Li&Rch&Dx:X");
    }
    else if (IS_CLASS(ch, CLASS_MOUNTED))
	sprintf(retur, "&W-{&G+&W}&cM&Counte&cd H&Cighlande&cr&W{&G+&W}-");
    else if (IS_CLASS(ch, CLASS_KNIGHT))
    {
	if (strlen(ch->clan)<2)
	    sprintf(retur, "&G-=+._ &RArcane Knight&G _.+=-");
	else
	    sprintf(retur, "&G-=+._ &B%s &B%s &G_.+=-", ch->generation==2?"Druid":ch->clan, ch->generation > 3?"Student":ch->generation==3?"Teacher":ch->generation==2?"Adept":"Master");
    }
    else if (IS_CLASS(ch, CLASS_FLUFF))
    {
	sprintf(retur, "&M*&c-&C*&Y.&G} &MFluffball &G{&Y.&C*&c-&M*");
    }
    else if (IS_CLASS(ch, CLASS_DEMON))
    {
	sprintf(retur, "&C{[%s&C%s%s&C]}", ch->generation<15?CLAN(ch):"",
	 ch->generation<15?" ":"", TABLE(0, ch->generation));
    }
    else if (IS_CLASS(ch, CLASS_OBJECTOR))
    {
        sprintf(retur, "&B{&M>&GObjector&M<&B}&x");
    }
    else if (IS_CLASS(ch, CLASS_DRUID))
    {
	if(strlen(ch->clan)<2)
	    sprintf(retur, "&G[{)&Y Lonely Druid &G(}]");
	else if (ch->generation > 3)
	    sprintf(retur, "&G[{)&Y %s Student &G(}]", ch->clan);
	else if (ch->generation == 3)
	    sprintf(retur, "&G[{)&Y %s Teacher &G(}]", ch->clan);
	else if (ch->generation == 2)
	    sprintf(retur, "&G[{)&Y %s Adept &G(}]", ch->clan);
	else if (ch->generation == 1)
	    sprintf(retur, "&G[{)&Y Druid Master &G(}]");
	else sprintf(retur, "&G[{)&Y Druid &G(}]");
    }
    else if (IS_CLASS(ch, CLASS_MAGE))
    {
	sprintf(retur, "{%s %s}", TABLE(1,ch->pcdata->powers[0]+1), 
	    ch->generation == 1 ? "Grand Sorcerer" : !IS_SET(ch->extra,
EXTRA_TRANSC) ? (ch->level == 6 ? "ArchMage" : ch->level == 5 ? "Mage" :
"Apprentice") : IS_SET(ch->extra, EXTRA_NECRO) ? "Necromancer" :
IS_SET(ch->extra, EXTRA_ILLUS) ? "Illusionist" : IS_SET(ch->newbits,
NEW_INVK) ? "Invoker" : "Sorcerer");
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	sprintf(retur, "(%s&C%s%s)", TABLE(2, ch->generation),
ch->generation != 1 ? " of " : "",
ch->generation == 1 ? "" : CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	sprintf(retur, "<%s%s&C>",
TABLE(3,ch->generation),ch->generation == 1 ? " Vampire" : CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	sprintf(retur, "&C<(>HighLander<)>#n");
    }
    else if (IS_CLASS(ch, CLASS_DROW))
    {
	sprintf(retur, ".oO %s&s&C %s&t0o.", ch->generation == 1 ? "Lloth's Avatar"
	    : TABLE(4,ch->generation), ch->generation == 1 ? "" : CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_MONK))
    {
	if (ch->generation == 1)
	    sprintf(retur, "*** &s&WGod&B'&Ws &BAbbot&t ***");
	else
	    sprintf(retur, "*** %s&C %s ***", ch->generation == 1 ? "Monk" :
	CLAN(ch), ch->generation == 4 ? "Monk" : "Master");
    }
    else if (IS_CLASS(ch, CLASS_NINJA))
    {
	sprintf(retur, "&C**[) %s&C %s&C (]**", ch->generation == 1 ? TABLE(5,ch->generation) : CLAN(ch),ch->generation == 1 ? "Ninja" : TABLE(5, ch->generation));
    }
    else if (IS_CLASS(ch, CLASS_AMAZON))
    {
	if (ch->sex == SEX_MALE)
	    sprintf(retur, "&M-+{ &YAmazon Slave&M }+-#n");
	else if (ch->generation == 1)
	    sprintf(retur, "&M-+{ &CAmazon Queen&M }+-#n");
	else if (!ch->clan || !str_cmp(ch->clan,"") || !str_cmp(ch->clan, "(null)"))
	{
	    if (ch->generation == 2)
	 	sprintf(retur, "&M-+{ Amazon Princess }+-&x");
	    else
		sprintf(retur, "&M-+{ Amazon }+-#n");
	}
	else if (ch->generation == 2)
	    sprintf(retur, "&M-+{ &C%s&C Princess&M }+-#n", ch->clan);
	else 
	    sprintf(retur, "&M-+{ &C%s&C Amazon&M }+-#n", ch->clan);
    }
    else if (IS_CLASS(ch, CLASS_JEDI))
    {
	if (ch->generation == 1)
	    sprintf(retur, "-<[ &GForce &YLord&x ]>-");
	else if (ch->generation == 2)
	    sprintf(retur, "-<[ %s&C Master ]>-", CLAN(ch));
	else if (IS_SET(ch->special, SPC_PRINCE))
	    sprintf(retur, "-<[ %s&C Knight ]>-", CLAN(ch));
	else if (ch->generation == 6)
	    sprintf(retur, "-<[ %s&C Apprentice ]>-", CLAN(ch));
	else
	    sprintf(retur, "-<[ Adept of %s&C ]>-", CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_SHAPE))
    {
	if (!ch->clan || !str_cmp(ch->clan, "") || !str_cmp(ch->clan, "(null)"))
	    sprintf(retur, "&R{-=+._Shape_.+=-}");
	else if (ch->generation == 1)
	    sprintf(retur, "&R{-=+._PolyMorphanter_.+=-}");
	else if (ch->generation == 2)
	    sprintf(retur, "&R{-King of %s&R-}", CLAN(ch));
	else if (ch->generation == 3)
	    sprintf(retur, "&R{-Forceman of %s&R-}", CLAN(ch));
	else
	    sprintf(retur, "&R{-ArmyMan of %s&R-}", CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_GOLEM))
    {
	sprintf(retur," &Y[][#n%s&Y][&RGoLeM&Y][#n%s&Y][]#n",
	CLAN(ch), golem_ranks[ch->generation]);	
    }
    else if (IS_MOOGLE(ch))
    {
	sprintf(retur,"&D[&R\\/&D]&C<&W%s&W&W%s&W%s&C>&D[&R\\/&D]#n", TABLE(6,ch->generation), str_cmp(ch->clan,"") ? " of ":"", str_cmp(ch->clan,"") ? CLAN(ch) : "");
    }
    else if (IS_UNI(ch))
    {
	sprintf(retur,"&G<&Y:)&G> &WUnicorN &G<&Y(:&G>#n");
    }
    else
	sprintf(retur, "Mortal");

 return retur;
}


int class_lookup(int class)
{
int a;
for (a = 0;class_table[a].class_name[0] != '\0';a++)
if (class_table[a].class == class) break;
if (class_table[a].class_name[0] != '\0') return a;
return -1;
}

int class_lookupn(char * class)
{
int a;
for (a = 0;class_table[a].class_name[0] != '\0';a++)
if (!str_cmp(class_table[a].class_name, class)) break;
if (class_table[a].class_name[0] != '\0') return a;
return -1;
}


void do_class(CHAR_DATA *ch, char *argument)
{
  int a = 0;
  CHAR_DATA *victim;
  int cl = 0;
  char buf[MIL];
  char arg1[MIL];
  char arg2[MIL];

argument = one_argument(argument, arg1);
argument = one_argument(argument, arg2);


if (arg1[0] == '\0') {
send_to_char("You may choose from:\n\r",ch);
for (cl = 0;class_table[cl].class_name[0] != '\0'; cl++)
{if (class_table[cl].bits & BIT_HIDDEN) continue;
if (ch->tier <3 && ch->trust < 7 && class_table[cl].class & HYBRID)
    continue;
if (++a % 3 == 0) stc("\n\r", ch);
sprintf(log_buf, "%s%s&C%-12s  ", class_table[cl].bits & BIT_CLOSED ?
"&M|" : "&G-",
class_table[cl].bits & BIT_DONTCLASS ? "&R*" :
"&G-", class_table[cl].class_name); stc(log_buf, ch);
}
send_to_char("\n\r#n",ch);
return;}

if ((victim = get_char_world(ch,arg1)) == NULL)
    {
	send_to_char("They aren't here!\n\r",ch);
	return;
    }

if (!str_cmp(arg2, "none") || !str_cmp(arg2,"mortal"))
    {
    int a;
if (victim->class == 0)
return;
	do_mclear(ch, victim->pcdata->switchname);

	for (a = 0;a <= 20;a++)
	victim->pcdata->powers[a] = 0;
	set_learnable_disciplines(victim);

REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
if (victim->trust < 7)
{
REMOVE_BIT(victim->act, PLR_WIZINVIS);
REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
}
victim->rage = 0; // the hit/dam automatically goes down
victim->glorify = 0;
for (a = 0; a < STANCE_WOLF; a++)
    if (ch->stance[a] > 200) ch->stance[a] = 200;

set_form(ch,0);

ch->autostance = -1;

	if (IS_AFFECTED(victim,AFF_POLYMORPH))
	{do_unpolymorph(victim,"");
	 free_string(victim->morph);
	 victim->morph = str_dup("");
	 REMOVE_BIT(victim->affected_by,AFF_POLYMORPH);
	}

victim->morphy = FALSE;
victim->warp = 0;

	for (a = 1; a <= 11; a++)
	victim->pcdata->stats[a] = 0;

        if ( IS_EXTRA(victim, EXTRA_OSWITCH) )
        {
         do_humanform(victim,"");
        } 

	victim->special = 0;
	victim->polyaff = 0;


	victim->morphy = FALSE;
	REMOVE_BIT(victim->immune, IMM_SHIELDED);    
	victim->warp = 0;
	victim->warpcount = 0;
	victim->extra = 0;
	victim->newbits = 0;
	REMOVE_BIT(victim->newbits, THIRD_HAND);
	REMOVE_BIT(victim->newbits, FOURTH_HAND);
        REMOVE_BIT(victim->act, PLR_WIZINVIS);
                        REMOVE_BIT(victim->act, OLD_PLR_DEMON);
                        REMOVE_BIT(victim->act, OLD_PLR_CHAMPION);
                        REMOVE_BIT(victim->act, OLD_PLR_WEREWOLF);
                        REMOVE_BIT(victim->act, OLD_PLR_WOLFMAN);
                        REMOVE_BIT(victim->act, OLD_PLR_VAMPIRE);       

      REMOVE_BIT(victim->affected_by, AFF_SHIFT); 
victim->newbits = 0;
for (a = 0; a <= 13; a++)
reset_weapon(victim, a);
for (a = 0; a <= 4; a++)
reset_spell(victim, a);
victim->move = victim->max_move;
victim->mana = victim->max_mana;
victim->hit = victim->max_hit;
victim->pcdata->rank = 0;
victim->generation = 4;
if (victim->trust < 7) {
    free_string(victim->lord);
    victim->lord     = str_dup( "" );}
    free_string(victim->clan);
    victim->clan     = str_dup( "" );

victim->class = 0;
if (victim->level < 7)
{
victim->level = 3;
victim->trust = 0;
}
stc("Ok!\n\r",ch);
stc("You are classless!\n\r",victim);
return;
}

    if (class_lookupn(arg2) == -1) {
stc("That class doesn't exist!\n\r",ch);return;}
if (IC_BIT_NAME(arg2, BIT_CLOSED) && ch->trust < 7 &&
!is_pure("class",ch->immpower))
{stc("That class is closed.\n\r", ch);return;}
if (IC_BIT_NAME(arg2, BIT_DONTCLASS) && victim->level < 7 &&
!is_pure("class",ch->immpower) && ch->level < 12) {
stc("That class is closed to the mortals.\n\r",ch);return;}
cl = class_lookupn(arg2);
if (class_table[cl].class & HYBRID && victim->tier < 7 && victim->tier < 3)
{stc("That class is tier III and above ONLY.\n\r", ch);return;}
sprintf(log_buf, "%s none",victim->pcdata->switchname);
do_class(ch,log_buf);

    victim->class = class_table[cl].class;
if (victim->trust < 7) {
    victim->trust = class_table[cl].start_trust;
    victim->level = class_table[cl].start_level;
}
if (victim->class == CLASS_VAMPIRE) victim->beast = 40;
if (victim->class == CLASS_AMAZON) victim->sex = 2;
sprintf(log_buf, "%s %d", victim->pcdata->switchname,class_table[cl].start_gen);
do_generation(ch, log_buf);
    set_learnable_disciplines(victim);
    sprintf(buf,"&CYou are now a %s!#n\n\r",class_table[cl].class_name);
    stc(buf,victim);
if (IS_CLASS(victim, CLASS_MAGE) && !(class_table[cl].class & HYBRID) && ch==victim)
    do_magecolor(ch,(a=number_range(1,7)) == 1 ? "self black" : a==2 ? "self white" : a==3 ? "self red" : a==4 ? "self blue" : a==5 ? "self green" : a==6 ? "self yellow" : "self purple");

    stc("Ok.\n\r",ch);
return;
}

void do_ptesttoggle(CHAR_DATA *ch, char *argument)
{

if (IS_NPC(ch)) return;

if (!IC_BIT_NUMB(ch->class,BIT_PTEST_TOGGLE) &&
!IS_PTEST_SAFE(ch))
{stc("Huh?\n\r",ch);return;}

if (ch->fight_timer > 0) {
stc("Wait til your fight timer runs out. Do not abuse this!\n\r",ch);
return;
}

if (IS_PTEST_SAFE(ch))
{REM_PTEST_SAFE(ch);
 stc("You are no longer safe from pk (ptesting mode off)\n\r",ch);
 return;
}
else
{SET_PTEST_SAFE(ch);
 stc("You are now in safe (ptest) mode. DO NOT abuse this!\n\r",ch);
 return;
}

return;
}

void do_magecolor(CHAR_DATA *ch, char *argument)
{
 char arg[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg);

 if (arg[0] == '\0')
 {
    stc("Change what mage to what color?
\r(magecolor [victim] [purple/red/blue/green/yellow/black/white])\n\r",ch);
    return;
 }

 if ((vict = get_char_world(ch, arg)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 if (!IS_CLASS(vict, CLASS_MAGE))
 {
    stc("They aren't a mage!\n\r", ch);
    return;
 }

 if (IS_CLASS(vict, HYBRID))
 {
    stc("Not on a hybrid!\n\r", ch);
    return;
 }

 if (!*argument)
 {
    stc("What color? red/purple/blue/green/yellow/black/white\n\r",ch);
    return;
 }

#define mageecol(a,b) if (!str_cmp(argument, a))\
{\
 vict->pcdata->powers[0] = b;\
 sprintf(log_buf, "You are now a %s mage!\n\r", a);\
 stc(log_buf, vict);\
 return;\
}

 mageecol("purple",PURPLE_MAGIC)
 mageecol("red",RED_MAGIC)
 mageecol("blue",BLUE_MAGIC)
 mageecol("yellow",YELLOW_MAGIC)
 mageecol("green",GREEN_MAGIC)
 mageecol("black",BLACK_MAGIC)
 mageecol("white",WHITE_MAGIC)

 stc("That color doesn't exist! Choose purple/red/blue/yellow/green/black/white!\n\r", ch);
 return;
}
