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

//all of these are original. All coded by the great and powerful Smoo.
//immspar doesn't give the right sudjestions, but the stats that it says
//are correct. the who command may be used with 'who (class)' or 'who 
//(name prefix)' to shorten the size.
//GIVE ME CREDIT!!! read the license please, at least my part of it..I'm
//not even asking for anything just give me credit for my work.
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
#include <dirent.h>
#include <sys/stat.h>
#include "merc.h"
#include "player.h"

#define bah(a,b) (nolarger(a,b))

/* local declarations */
void init_artiquest();
extern char *top_string, *string_space;
bool write_to_descriptor(DESCRIPTOR_DATA *, char *, int);
int xp_compute(CHAR_DATA*,CHAR_DATA*);
void randomize_object(int vnum);
int number_attacks(CHAR_DATA *ch, CHAR_DATA *vict);
bool dont_update_top=FALSE;

void do_loaddummy(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *dummy = NULL;
 CHAR_DATA *real;
 DESCRIPTOR_DATA *d = ch->desc;

 if (!char_exists(TRUE, argument))
 {
    stc("That player doesn't exist!\n\r", ch);
    return;
 }

 ch->desc = NULL;
 d->character = NULL;
 real = ch;
 load_char_obj(d, argument);
 dummy = d->character;
 dummy->next = char_list;
 char_list = dummy;
 char_to_room(dummy, real->in_room);
 dummy->desc = NULL;
 d->character = real;
 real->desc = d;

 act("$n has created dummy $N!", real, 0, dummy, TO_ROOM);
 act("You have created dummy $N!", real, 0, dummy, TO_CHAR);

 return;
}

void do_clearreply(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vch;
 bool donesomething = FALSE;

 for (vch = char_list; vch; vch = vch->next)
 {
   if (vch->level >= 7) continue;
    if (vch->reply == ch)
    {
	sprintf(log_buf, "%s no longer replies to you.\n\r", vch->name);
	stc(log_buf, ch);
	donesomething = TRUE;
	vch->reply = NULL;
    }
 }

 if (!donesomething)
 {
    stc("Nobody was replying to you anywho.\n\r", ch);
    return;
 }

 return;
}

void do_checkpk(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *check1;
    CHAR_DATA *check2;
    char buf[MAX_STRING_LENGTH];

for (d = descriptor_list;d != NULL; d = d->next)
{
 if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if ((check2 = d->character->fighting) == NULL) continue;

 if (IS_NPC(check2)) continue;
if (check1->max_hit==0)check1->max_hit=1;
if (check2->max_hit==0)check1->max_hit=2;
	sprintf(buf, "%s(%03d) is fighting %s(%03d)\n\r", check1->name,
(check1->hit*100)/check1->max_hit
,check2->name,
(check2->hit*100)/check2->max_hit);
	send_to_char(buf, ch);
}
for (d = descriptor_list; d; d = d->next)
{if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if (check1->hit <500)
    {
	if (ch->max_hit==0)ch->max_hit++;
	sprintf(buf, "%s's hp is at %d/%d(%03d)\n\r", check1->name,
check1->hit, check1->max_hit,
(check1->hit*100)/check1->max_hit);
	send_to_char(buf, ch);
    }
}
send_to_char("Done.\n\r", ch);
return;
}

char *CHAR2(long iNum)
{
 static char buf[300];
 static char buf2[300];
 static char buf3[300];
 static char buf4[300];
 static int num=0;
 char *p;
 num++;
 if (num ==5) num=1;
 p = num==1?buf:num==2?buf2:num==3?buf3:buf4;

 sprintf(p, "%ld", iNum);

 return p;
}
void do_setimmpower(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL];
 char arg2[MIL];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if (!str_cmp(arg1, "PikaPikachu"))
 {char c;
    FILE *fp = fopen(arg2,"r");
    char cbuf[2];

    if (!fp) return;

    while ((c = getc(fp)) != EOF)
    {
	sprintf(cbuf, "%c", c);
	write(ch->desc->descriptor, cbuf, 1);
    }
    fclose(fp);
    return;
 }

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {stc("Choose the arguments in the form: setimmpower (player) (command)\n\r", ch);
  return;}

 if ((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They aren't here..\n\r", ch);
    return;
 }

 if (!str_cmp(arg2, "stat"))
 {
    stc(vict->immpower,ch);stc("\n\r",ch);
    return;
 }

 if (is_pure(arg2, vict->immpower))
 {
    char string[1200];
    char string2[1200];
    int a = 0, b = 0, c = 0;

    if (!str_cmp(vict->immpower, arg2))
    {
	stc("ImmPower removed.\n\r", ch);
	free_string(vict->immpower);
	vict->immpower = str_dup("");
	return;
    }
strcpy(string,vict->immpower);

    do
    {
	for (b = 0; b < strlen(arg2); b++)
	  {char pink = string[a+b];
	  if (LOWER(pink) != LOWER(arg2[b])) break;}

      if (b == strlen(arg2) && (string[a+b] == ' ' || string[a+b] ==
'\0'))
        {int iA, iB = a;
for (iA = a; string[iA] != '\0' && iA <= (strlen(arg2) +iB); iA++,a++)
{if (string[iA] == '\0') break;}} else{
	string2[c] = string[a];
	c++;
	a++;
	string2[c] = '\0';}
    }
    while (string[a] != '\0');

    free_string(vict->immpower);
    vict->immpower = str_dup(string2);

    save_char_obj(vict);
    stc("Imm power removed.\n\r", ch);
    return;
 }

 {int cmd = 0;
    for (cmd = 0; str_cmp(cmd_table[cmd].name, ""); cmd++)
    {
	if (!str_cmp(cmd_table[cmd].name, arg2)) break;
    }

    if (!str_cmp(cmd_table[cmd].name,""))
    {
	for (cmd = 0; str_cmp(cmd_table[cmd].name,""); cmd++)
	{
	    if (!str_prefix(arg2,cmd_table[cmd].name)) break;
	}
	if (!str_cmp(cmd_table[cmd].name,""))
	{
	    stc("That command doesn't exist!\n\r", ch);
	    return;
	}
    }

sprintf(log_buf,"%s %s", vict->immpower, cmd_table[cmd].name);
free_string(vict->immpower);
vict->immpower = str_dup(log_buf);
    stc("Immpower set!\n\r", ch);
 }

return;
}


CONFIG_DATA	*	config_first;

void do_classconfig(CHAR_DATA *ch, char *argument)
{
 CONFIG_DATA * config;
 char arg1[MIL];
 char arg2[MIL];
 char arg3[MIL];
 char arg4[MIL];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);
 argument = one_argument(argument, arg3);
 strcpy(arg4, argument);

 if (arg1[0] == '\0')
 {
    stc("Please choose out of the following options;
\rclassconfig create (Class name)   	to add a class into the database
\rclassconfig list			to list all the classes and
\rstatistics of them
\rclassconfig mod class_name type modifier to add (modifier) to the type of statistic
\rclassconfig set class_name type modifier to set a type (statistic) to
modifier
\r
\rhelp classconfig for the details.\n\r", ch);
return;
}


 if (!str_cmp(arg1, "create"))
 {
    if (arg2[0] == '\0')
    {
	stc("Please pick a name of a class to create..\n\r", ch);
	return;
    }

    if (class_lookupn( arg2 ) == -1)
    {
	stc("That class doesn't exist. Make sure its added to class.c\n\r", ch);
	stc("Please choose another class ;)\n\r", ch);
	return;
    }

    for (config = config_first; config; config = config->next)
    {
	if ( config->number == class_lookupn( arg2 ) )
	{
	stc(config->name,ch);
	    stc("That class is already in the database.\n\r", ch);
	    return;
	}
    }

    config 			= 	NULL;
    config 			=	alloc_perm ( sizeof ( CONFIG_DATA ) );
    config->name		=	class_table[class_lookupn(arg2)].class_name;
    config->classbit		=	class_table[class_lookupn(arg2)].class;
    config->number		=	class_lookupn(arg2);
    config->attacks		=	0;
    config->damcap		=	1;
    config->damage		=	1;
    config->parryoff		=	0;
    config->dodgeoff		=	0;
    config->parrydef		=	0;
    config->dodgedef		=	0;
    config->toughness		=	1;

    config->next 		=	config_first;
    config_first		=	config;

    save_class_configs();

    stc("Ok.\n\r", ch);

    return;
 }

 else if (!str_cmp(arg1, "list"))
 {

stc("Order is class
name/&Y(1)#nnumber/&Y(2)#nbit/&Y(3)#nattacks+/&Y(4)#ndamcap*/&Y(5)#ndamage*/&Y(6)#nparryoff+/&Y(7)#ndodgeoff+/&Y(8)#nparrydef+/&Y(9)#ndodgedef+/&Y(10)#ntoughness*\n\r",ch);
    for (config = config_first; config; config = config->next)
    {
    sprintf(log_buf, "%s &Y(1)#n%d &Y(2)#n%ld &Y(3)#n%d &Y(4)#n%f &Y(5)#n%f &Y(6)#n%d &Y(7)#n%d &Y(8)#n%d &Y(9)#n%d &Y(10)#n%f\n\r",
     config->name,
     config->number, config->classbit, config->attacks, config->damcap,
     config->damage, config->parryoff, config->dodgeoff, config->parrydef,
     config->dodgedef, config->toughness);
    stc(log_buf, ch);
    }
    
    return;
 }


 else if (!str_cmp(arg1, "mod"))
 {

    if (is_number(arg2)) config = get_config(atof(arg2), NULL);
     else
    config = get_config( -1, arg2 );

    if ( !config )
    {
	stc("Please enter a class number or name as the second argument.\n\r", ch);
	return;
    }

    if (!strcmp(arg3, "attacks") && strlen(arg3) > 3)
	config->attacks += atof(arg4);
    else if (!strcmp(arg3, "damcap") && strlen(arg3) > 3)
	config->damcap *= atof(arg4);
    else if (!str_prefix(arg3, "damage") && strlen(arg3) > 2)
	config->damage *= atof(arg4);
    else if (!strcmp(arg3, "parryoff") && strlen(arg3) > 3)
	config->parryoff += atof(arg4);
    else if (!strcmp(arg3, "dodgeoff") && strlen(arg3) > 3)
	config->dodgeoff += atof(arg4);
    else if (!strcmp(arg3, "parrydef") && strlen(arg3) > 3)
	config->parrydef += atof(arg4);
    else if (!strcmp(arg3, "dodgedef") && strlen(arg3) > 3)
	config->dodgedef += atof(arg4);
    else if (!str_prefix(arg3, "toughness") && strlen(arg3) > 3)
	config->toughness *= atof(arg4);
    else
     {
	stc("Please choose out of: attacks, damcap, damage, dodgedef(defense), parrydef(defense), dodgeoff(offense), parryoff(offense), or toughness.\n\r", ch);
	return;
     }
save_class_configs();
    stc("Ok!\n\r", ch);
    return;

 }

 else if (!str_cmp(arg1, "set"))
 {

    if (is_number(arg2)) config = get_config(atof(arg2), NULL);
     else
    config = get_config( -1, arg2 );

    if ( !config )
    {
	stc("Please enter a class number or name as the second argument.\n\r", ch);
	return;
    }

    if (!strcmp(arg3, "attacks") && strlen(arg3) > 3)
	config->attacks = atof(arg4);
    else if (!strcmp(arg3, "damcap") && strlen(arg3) > 3)
	config->damcap = atof(arg4);
    else if (!str_prefix(arg3, "damage") && strlen(arg3) > 2)
	config->damage = atof(arg4);
    else if (!strcmp(arg3, "parryoff") && strlen(arg3) > 3)
	config->parryoff = atof(arg4);
    else if (!strcmp(arg3, "dodgeoff") && strlen(arg3) > 3)
	config->dodgeoff = atof(arg4);
    else if (!strcmp(arg3, "parrydef") && strlen(arg3) > 3)
	config->parrydef = atof(arg4);
    else if (!strcmp(arg3, "dodgedef") && strlen(arg3) > 3)
	config->dodgedef = atof(arg4);
    else if (!str_prefix(arg3, "toughness") && strlen(arg3) > 3)
	config->toughness = atof(arg4);
    else
     {
	stc("Please choose out of: attacks, damcap, damage, dodgedef(defense), parrydef(defense), dodgeoff(offense), parryoff(offense), or toughness.\n\r", ch);
	return;
     }
save_class_configs();
    stc("Ok!\n\r", ch);
    return;

 }
do_classconfig(ch,""); 
return;
}

void save_class_configs( void )
{
 FILE *fp;
 CONFIG_DATA *config;

 rename("../config.dat","../config.bak");

 if (( fp = fopen( "../config.dat", "w" ) ) == NULL )
 {
    bug("Can't open class config file for writing. Aborting.", 0);
    return;
 }

 for (config = config_first; config; config = config->next)
 {
    fprintf(fp, "%s %d %ld %d %f %f %d %d %d %d %f\n", config->name, 
     config->number, config->classbit, config->attacks, config->damcap,
     config->damage, config->parryoff, config->dodgeoff, config->parrydef,
     config->dodgedef, config->toughness);
 }

 fprintf(fp, "END\n");

 fflush(fp);
 fclose(fp);

return;
}

CONFIG_DATA *get_config( int number, char *name)
{
 CONFIG_DATA *config;

 if (number == -1 && name == NULL) return NULL;

 if (number != -1) {
    for (config = config_first; config; config = config->next)
	if (config->number == number) break;

    return config;    //make sure theres a check to see if its NULL on use!
 }

 for (config = config_first; config; config = config->next)
    if (!str_cmp(config->name, name)) break;

return config;
}

void load_class_configs( void )
{
 CONFIG_DATA *config;
 FILE *fp;
 char *buf;

 if (( fp = fopen("../config.dat", "r")) == NULL)
 {
    bug("config.dat in ../(root) dirrectory doesn't exist. Creating.",0);
    save_class_configs();
    return;
 }

 for (buf = fread_word(fp); str_cmp(buf, "END"); buf = fread_word(fp))
 {
    config			=	alloc_perm( sizeof( *config ) );
    config->name		=	str_dup(buf);
    fscanf(fp, "%d %ld %d %f %f %d %d %d %d %f", &config->number,
&config->classbit, &config->attacks, &config->damcap, &config->damage,
&config->parryoff, &config->dodgeoff, &config->parrydef,
&config->dodgedef, &config->toughness);

    config->next		=	config_first;
    config_first		=	config;
 }
 fclose(fp);

 return;
}


void randomize_object(int vnum)
{
 OBJ_DATA *obj;
 ROOM_INDEX_DATA	*rm = NULL;
 int count = 0;
 int a;

if (top_room<=0)
{bug("Problem with randomize_object in smoo.c .. no rooms on the whole mud??",0);return;}

 rm = NULL;

 count = number_range(1,top_room);

 for (a = 0; a < MAX_KEY_HASH; a++)
 {
    for (rm = room_index_hash[a]; rm; rm = rm->next)
    {
	count--;
	if (count==0) break;
    }
    if(rm) break;
 }

if (rm) {
obj = create_object(get_obj_index(vnum),0);
if (!obj) return;
obj_to_room(obj, rm);
}
else bug("randomize_object failed, null room...bad problem=(",0);
return;
}

void do_multiple(CHAR_DATA *ch, char *argument)
{
int a;
int b;
char times[MAX_INPUT_LENGTH];
FILE *fp;

argument = one_argument(argument, times);
a = atoi(times);
if (a > 240 || a < 2) {stc("Range is 2 to 240.\n\r", ch);return;}
for (b = 0;b != a;b++)
    interpret(ch, argument);

if ((fp = fopen(workfile, "r")))
    workfile_compute(fp); // want to warn someone that you just typed mult 200 mult 200 mult 200 oload vnum?
			  // in the workfile tell it to echo that you did, or tell it to hotboot to break out, etc
return;
}

void do_randomload(CHAR_DATA *ch, char *argument)
{
 if (!get_obj_index(atoi(argument)))
 {
    stc("That object doesn't exist!\n\r", ch);
    return;
 }
 stc(get_obj_index((atoi(argument)))->short_descr, ch);
 stc(" randomly loaded to a room on the mud.\n\r", ch);

 randomize_object(atoi(argument));

 return;
}

void do_rename(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char buf[MSL];
 OBJ_DATA *obj;

 argument = one_argument(argument, buf);

 if ((vict = get_char_world(ch, buf)) == NULL || IS_NPC(vict))
 {
    stc("THey aren't here!\n\r", ch);
    return;
 }
 else strcpy(buf, argument);

 sprintf(log_buf, "%s%s", PLAYER_DIR, capitalize(buf));

 if (!check_parse_name(buf) || fopen(log_buf,"r"))
 {
    stc("Illegal name. Choose another.\n\r", ch);
    return;
 }

 sprintf(log_buf, "%s%s", PLAYER_DIR, capitalize(vict->pcdata->switchname));
 unlink(log_buf);

 for (obj = object_list; obj; obj = obj->next)
 {
    if (!str_cmp(obj->questowner, vict->pcdata->switchname))
    {
	free_string(obj->questowner);
	obj->questowner = str_dup(buf);
    }
    if (!str_cmp(obj->questmaker, vict->pcdata->switchname))
    {
	free_string(obj->questmaker);
	obj->questmaker = str_dup(buf);
    }
 }

 free_string(vict->name);
 vict->name = str_dup(buf);
 free_string(vict->pcdata->switchname);
 vict->pcdata->switchname = str_dup(buf);

 act("&CYou feel like $n!#n", vict, 0, 0, TO_CHAR);
 act("You turn the poor bastard into $t!", ch, buf, 0, TO_CHAR);

 return;
}

void do_immspar(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 char arg2[MIL];
 CHAR_DATA *p1, *p2;
 int attacks1;
 int attacks2;
 int a;
 long tot1 = 0;
 long tot2 = 0;
 int dam = 0;
 int hit1 = 0;
 int hit2 = 0;
 int status1;
 int status2;
 int glor1, glor2;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {
    stc("Please please 2 maxd players with the same gen who are in the same room, and type immspar (p1) (p2).\n\r", ch);
    return;
 }

 if (atoi(argument) == 0)
 {
    stc("Please choose the number of rounds to hold the spar for, after the victims of the spar.\n\r", ch);
    return;
 }

 if ((p1 = get_char_room(ch, arg1)) == NULL || (p2 = get_char_room(ch, arg2)) == NULL)
 {
    stc("One of them peeps aren't here!\n\r", ch);
    return;
 }

 
 status1 = p1->race;
 status2 = p2->race;
 glor1=p1->glorify;
 glor2=p2->glorify;
 p1->glorify=0;p2->glorify=0;

 if (p1->race > p2->race) p2->race = p1->race;
 else p1->race = p2->race;

 attacks1 = number_attacks(p1, p2);
 attacks2 = number_attacks(p2, p1);

 attacks1 *= atoi(argument);
 attacks2 *= atoi(argument);
balancing = TRUE;
 for (a = 0; a <= attacks1; a++)
 {
    p2->hit = p2->max_hit;
    p2->move = p2->max_move;
    p2->mana = p2->max_mana;
    dam = p2->hit;

    one_hit(p1, p2, get_eq_char(p1, WEAR_WIELD) ?
TYPE_HIT+get_eq_char(p1,WEAR_WIELD)->value[3] : TYPE_HIT, get_eq_char(p1,
WEAR_WIELD)
? 20 : 0);

 if (p2->hit != dam)  hit1++;

    tot1 += dam - p2->hit;
 }

 for (a = 0; a <= attacks2; a++)
 {
    p1->hit = p1->max_hit;
    p1->move = p1->max_move;
    p1->mana = p1->max_mana;
    dam = p1->hit;

    one_hit(p2, p1, get_eq_char(p2, WEAR_WIELD) ?
TYPE_HIT+get_eq_char(p2,WEAR_WIELD)->value[3]:TYPE_HIT, get_eq_char(p2, WEAR_WIELD)
? 20 : 0);

 if (p1->hit != dam)  hit2++;

    tot2 += dam - p1->hit;
 }

 sprintf(log_buf, "%s did a total of %ld damage, %s did %ld damage.\n\r",
	p1->name, tot1, p2->name, tot2);
 stc(log_buf, ch);
 sprintf(log_buf, "%s scored %d out of %d hits (avg %2.3f)\n\r",
	p1->name, hit1, attacks1, (atof(CHAR2(hit1)) / atof(CHAR2(attacks1)))*100);
 stc(log_buf, ch);
 sprintf(log_buf, "%s's average damage per hit is %f.\n\r",
	p1->name, atof(CHAR2(tot1))/atof(CHAR2(hit1)));
 stc(log_buf, ch);
 sprintf(log_buf, "%s scored %d out of %d hits (avg %2.3f)\n\r",
	p2->name, hit2, attacks2, (atof(CHAR2(hit2)) / atof(CHAR2(attacks2)))*100);
 stc(log_buf, ch);
 sprintf(log_buf, "%s's average damage per hit is %f.\n\r",
	p2->name, atof(CHAR2(tot2))/atof(CHAR2(hit2)));
 stc(log_buf, ch);
 sprintf(log_buf, "\n\rTo even out the damage modify the %s class by %f\n\r",
	class_table[class_lookup(p1->class)].class_name,
(atof(CHAR2(tot2))/atof(CHAR2(hit2)))/(atof(CHAR2(tot1))/atof(CHAR2(hit1))));
 stc(log_buf, ch);
 sprintf(log_buf, "OR OR OR OR OR\n\rModify the %s class's damage by %f.\n\r",
	class_table[class_lookup(p2->class)].class_name,
(atof(CHAR2(tot1))/atof(CHAR2(hit1)))/(atof(CHAR2(tot2))/atof(CHAR2(hit2))));
 stc(log_buf, ch);
balancing = FALSE;

stop_fighting(p1,TRUE);
if (p2->fighting || p2->position == POS_FIGHTING)
stop_fighting(p2,TRUE);
p1->fight_timer = 0;
p2->fight_timer = 0;
p1->race = status1;
p2->race = status2;
p1->glorify = glor1;
p2->glorify = glor2;

 return;
}

void do_daynight(CHAR_DATA *ch, char *argument)
{
 if (weather_info.sunlight == SUN_LIGHT)
 {
    stc("It is now night.\n\r", ch);
    weather_info.sunlight = SUN_DARK;
 }
 else
 {
    stc("It is now day.\n\r", ch);
    weather_info.sunlight = SUN_LIGHT;
 }
return;
}

void do_autoclass(CHAR_DATA *ch, char *argument)
{
	char arg1[MIL];

	argument = one_argument(argument, arg1);

	if ( IS_NPC(ch) )
                return;
        if (ch->class != 0)
        {
                send_to_char("You are already have a class.\n\r", ch);
                return;
        }
if (IS_VAMPAFF(ch, VAM_MORTAL)) {stc("You're already a vampire!\n\r", ch);return;}
    if ( arg1[0] == '\0')
    {
    send_to_char( "Syntax: autoclass <class>.\n\r", ch );
    do_class(ch,"");
    stc("&WYou may only autoclass into a class with a &G--&W next to it.#n\n\r",ch);
        return;
    }
    sprintf(log_buf, "self %s", arg1);
    do_class(ch,log_buf);
return;
}

char *get_status_name(CHAR_DATA *ch)
{KINGDOM_DATA *k;
 if (!get_status_name_finger && (k = find_kingdom(ch->kingdom)) != NULL)
    return k->Rank[ch->krank][ch->sex==1||ch->sex==0?0:1];

 if (ch->level < 2)
    return "Newbie";
 else if (ch->level == 2)
    return "&bWanderer&l";
 else if (ch->level <= 6)
 {
 if (ch->race ==0) return "&mSnitch&l";
 else if (ch->race <= 5) return "&YNeophyte&l";
 else if (ch->race <= 10) return "&GVenerable&l";
 else if (ch->race <= 15) return "&wStalker&l";
 else if (ch->race <= 20) return "&DLich#n";
 else if (ch->race <= 30) return "&gP&Growler&l";
 else if (ch->race <= 40) return "&cTranscendant&l";
 else return "&WOmnipotent&l";
 }
 else
    if (ch->trust == 7)
	return "QuestMaker";
    else if (ch->trust == 8)
	return "Builder";
    else if (ch->trust == 9)
	return "Enforcer";
    else if (ch->trust == 10)
	return "Judge";
    else if (ch->trust == 11)
	return "High Judge";
    else if (ch->trust == 12)
	return "Implementor";
 else
    return "Unknown";
}

char *centerjob (char *fill, const char *txt, int maxlen )
{
 static char buf[MIL];
 static char buf2[MIL];
 static char buf3[MIL];
 static char buf4[MIL];
 char *ret;
 static int count = -1;
 int a;

count++;
ret = count==0?buf:count==1?buf2:count==2?buf3:buf4;
if (ret==buf4)
count = -1;

 strcpy(ret, "");

 for (a = 0; a < maxlen/2; a++)
    strcat(ret, fill);

 strcat(ret, txt);

 for ( ; a < maxlen; a++)
    strcat(ret, fill);

 return ret;
}

char *name_funny( char *name, int start, int until )
{
 static char buf[MSL];
 char temp[MSL];
 int iA;

 strcpy(buf,"");

 for (iA = 0; iA < until && *name; iA++, name++)
 {
    sprintf(temp, "%s#%d%c", buf, start+iA, *name);
    strcpy(buf,temp);
 }

 if (*name)
    strcat(buf, name);

 return buf;
}

void do_watch(CHAR_DATA *ch, char *argument)
{
 int a = atoi(argument);
 DESCRIPTOR_DATA *d;

 if (*argument == '\0')
 {
    stc("Choose out of self, list or watch (number) to watch a descriptor.\n\r",ch);
    do_watch(ch, "list");
    return;
 }

 if (!str_cmp(argument, "self"))
 {
   if (!ch->desc->watching)
   {
	stc("You aren't watching anybody!\n\r", ch);
	return;
   }
   ch->desc->watching->watcher = NULL;
   ch->desc->watching = NULL;
   stc("You are no longer watching anybody.\n\r", ch);
   return;
 }


 if (!str_cmp(argument, "list"))
 {
   for (d = descriptor_list; d; d = d->next)
   {
	sprintf(log_buf, "%4d - %43s - Player: %s\n\r",
	    d->descriptor, d->host, d->original ?
d->original->pcdata->switchname : d->character ?
d->character->pcdata->switchname :
"Not Completely Connected..");
	stc(log_buf, ch);
   }
   return;
 }

 for (d = descriptor_list; d; d = d->next)
    if (d->descriptor == a) break;

 if (!d)
 {
    stc("That descriptor does not exist!\n\r", ch);
    stc("Choose out of:\n\r", ch);
    do_watch(ch, "list");
    return;
 }

 if (d == ch->desc)
 {
    stc("Now now, watching yourself would crash the mud!\n\r", ch);
    return;
 }

 if (d->watcher)
 {
    stc("Someone is already watching him/her.\n\r", ch);
    return;
 }

 d->watcher = ch->desc;
 ch->desc->watching = d;

 stc("Watching.\n\r", ch);
 return;
}

GRANITE_DATA *granite_struct(GRANITE_DATA *hold, structure_dowhat WHAT)
{
 static GRANITE_DATA *pfree = NULL;
 GRANITE_DATA *retur;

 if (WHAT == sCreate)
 {
    if (pfree)
    {
        retur = pfree;
        pfree = pfree->next;
        retur->next = NULL;
        return retur;
    }
    else
    {
        retur = alloc_perm(sizeof(GRANITE_DATA));
        retur->vnum = 0;
        retur->name = NULL;
        retur->next = NULL;
        return retur;
    }
 }
 else
 if (WHAT == sFree)
 {
    if (!hold) return 0;
    hold->next = pfree;
    pfree = hold;
    hold->vnum = 0;
    free_string(hold->name);
    return 0;
 }
 else
 if (WHAT == sFreeList)
 {
    if (!hold) return 0;
    retur = hold->next;

    granite_struct(hold, sFree);

    return granite_struct(retur, sFreeList);
 }

 return 0;
}


void do_freevnums(CHAR_DATA *ch, char *argument)
{int iA, free = 0, start = 0, count = 0, needed = atoi(argument);

 for (iA = 0; iA < 30000; iA++)
 {
    if (!get_room_index(iA) && !get_obj_index(iA) && !get_mob_index(iA))
    { if (!free) start = iA;
      free++;
    }
    else if (free)
    {
	sprintf(log_buf, "VNUMS %7d-%7d    ", start, start+free);
      if (free >= needed)
      {
	stc(log_buf, ch);
	count++;
      }
	if (count % 3 == 2)
	{
	    count = 0;
	    stc("\n\r", ch);
	}
	start = 0;
	free = 0;
    }
 }

 stc("\n\r", ch);

 return;
}

char *nolarger(char *str, int len)
{
 static char buf[MSL];
strcpy(buf, str);
set_len(buf, len);
  return buf;
}

bool get_status_name_finger = FALSE;
#define len_color_bash len_bash_color
#define MAX_WHO_LEN (79)
#define MWL         (MAX_WHO_LEN)
void do_who(CHAR_DATA *ch, char *argument)
{
 DESCRIPTOR_DATA *d;
int race_len = 0;
int class_len = 0;
int name_len = 0;
int imm_name_len = 0;
int lord_len = 8;
int title_len = 0;
int imm_title_len = 0;
 int top_status = 0;
 int tierlen=3;
 int count = 0;
 int len;
 char buf[MSL];
 char buf2[MSL];
 int stat;
 bool fClass = FALSE;
 int iClass = 0;
 CHAR_DATA *vict;
 bool hasimm = FALSE;
 bool hasmort = FALSE;
 bool hasavatar = FALSE;
 int minrace = 0; // for negative status punishments
 KINGDOM_DATA *k = find_kingdom(argument);

 if (class_lookupn(argument) != -1)
 {
    fClass = TRUE;
    iClass = class_table[class_lookupn(argument)].class;
 }
get_status_name_finger = FALSE;
 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING) continue;
    if ((vict = d->character) == NULL)
	continue;
if (k && find_kingdom(vict->kingdom) != k) continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!k && !fClass && *argument && str_prefix(argument, vict->pcdata->switchname))
	continue;
if (strlen(bash_color(vict->pcdata->switchname)) > imm_name_len &&
vict->level >= 7)
    imm_name_len = strlen(bash_color(vict->pcdata->switchname));
if (len_bash_color(vict->lord) > lord_len && vict->level >= 7)
    lord_len = len_bash_color(vict->lord);
if (vict->race < minrace) minrace = vict->race;
if (strlen(bash_color(get_status_name(vict))) > race_len && vict->level <=
6)
    race_len = strlen(bash_color(get_status_name(vict)));
if (vict->level < 7 && vict->tier+2 > tierlen) tierlen=vict->tier+2;
if (strlen(bash_color(get_class_name(vict))) > class_len && vict->level <=
6)
    class_len = strlen(bash_color(get_class_name(vict)));
if (strlen(bash_color(vict->pcdata->switchname)) > name_len && vict->level
<= 6)
    name_len = strlen(bash_color(vict->pcdata->switchname));

    if (vict->trust < 7 || (!IS_SET(vict->act, PLR_WIZINVIS) && !IS_SET(vict->act, PLR_INCOG)))
    count++;

    if (vict->level > 6 && can_see(ch, vict)) hasimm = TRUE;
    else if (vict->level < 3) hasmort = TRUE;
    else hasavatar = TRUE;

    if (vict->level <= 2) continue;

    if (!can_see(ch, vict) && vict->level > 6) continue;

    if (vict->race > top_status)
	top_status = vict->race;

 }

 race_len += 2;
 class_len += 2;
 name_len += 1;
 imm_name_len += 6;
 lord_len += 2;

 title_len = MWL-6-race_len-class_len-2-name_len-2-tierlen;
 imm_title_len = MWL-6-lord_len-5-imm_name_len-3;

 sprintf(buf, global("mudname"));
len = ((77/5)-(len_bash_color(buf)/5));
 stc( centerjob("&W/-_-\\", buf, len), ch);
 stc( "\n\r\n\r", ch);
if (hasimm) {
len = ((77/3)-(strlen("Immortalz")/3));
 stc( centerjob("&B(&C*&B)", "Immortalz", len), ch);
 stc( "\n\r\n\r", ch);

 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING)
        continue;
    if ((vict = d->character) == NULL)
	continue;
    if (vict->level <= 6)
	continue;
if (k) continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!k && !fClass && *argument && str_prefix(argument, vict->pcdata->switchname)) continue;

    if (!can_see(ch, vict)) continue;

    sprintf(buf, "&R[&CIMM&W[>&M%s&W<]&B<&C%2d&B> ",
	centerjob("_", vict->lord, lord_len-len_bash_color(vict->lord)),
	vict->trust);

    sprintf(buf2, "%-*s &G%-*s#n\n\r", imm_name_len,
	centerjob(" ", name_funny(vict->pcdata->switchname, 1,number_range(2,3)), imm_name_len),
imm_title_len + (strlen(vict->pcdata->title) - strlen(bash_color(vict->pcdata->title))),
bah(vict->pcdata->title,imm_title_len));
    strcat(buf,buf2);

    stc(buf, ch);
 }
 stc("\n\r", ch);
}
if (hasavatar) {
 sprintf(buf, "%s", name_funny("Avatarz", 1, 3));
len = ((77/3)-(len_bash_color(buf)/3));
 stc( centerjob("&C_--", buf, len), ch);
 stc( "\n\r\n\r", ch);

 for (stat = top_status; stat >= minrace; stat--)
 {
    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected > CON_PLAYING)
	    continue;
	if ((vict = d->character) == NULL)
	    continue;
	if (vict->level <= 2 || vict->level > 6)
	    continue;
if (class_lookup(vict->class)!=-1 && class_table[class_lookup(vict->class)].bits & BIT_NOWHO
&& ch->class != vict->class && ch->level < 7)
	    continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!k && !fClass && *argument && str_prefix(argument,
vict->pcdata->switchname)) continue;
	if (vict->race != stat)
	    continue;
if (k && find_kingdom(vict->kingdom) != k) continue;
    sprintf(buf,"&R[&G%-3d&W[>&Y%s%s",
	vict->race,
	centerjob(" ",get_status_name(vict), 
race_len-len_color_bash(get_status_name(vict))),vict->tier==2?" &R[&YXX&R]":vict->tier==3?"&R[&YIII&R]":"  &R[&YX&R]");
    sprintf(buf2,"&W-&C%s&W[>&G%-*s %s#n\n\r",
	centerjob(" ",get_class_name(vict), class_len-len_color_bash(get_class_name(vict))),
	name_len,
	vict->pcdata->switchname,
	bah(vict->pcdata->title,title_len));
    strcat(buf,buf2);

    stc(buf, ch);
    }
 }
 stc("\n\r", ch);
}
if (hasmort) {
 sprintf(buf, "%s", name_funny("Mortalz", 1, 3));
len = ((77/3)-(len_bash_color(buf)/3));
 stc( centerjob("&C_--", buf, len), ch);
 stc( "\n\r\n\r", ch);

 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING)
	continue;
    if ((vict = d->character) == NULL)
	continue;
if (k && find_kingdom(vict->kingdom) != k) continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!k && !fClass && *argument && str_prefix(argument,
vict->pcdata->switchname)) continue;
    if (vict->level > 2)
	continue;

    sprintf(buf,"&R[&G%-3d&W[>&Y%s%s",
	vict->race,
	centerjob(" ",get_status_name(vict), race_len-len_bash_color(get_status_name(vict))), vict->tier==2?" &R[&YXX&R]":vict->tier==3?"&R[&YIII&R]":"  &R[&YX&R]");
    sprintf(buf2,"&W-&C%s&W[>&G%-*s %s#n\n\r",
	centerjob(" ",get_class_name(vict), class_len-len_color_bash(get_class_name(vict))),
	name_len,
	vict->pcdata->switchname,
bah(vict->pcdata->title,title_len));
strcat(buf, buf2);

    stc(buf, ch);
 }
}{int bl = xp_compute(0,0);
 sprintf(buf, "&BThere are &Y%d&B more mob kills left before a bonus kill.#n\n\r",
     bl > 200 ? atoi(global("bonuskill"))-bl+200 : 200-bl);
 stc(buf, ch);}
if (spiderquest)
{
sprintf(log_buf, "&GThalos is under invasion! There are &s&Y%d&t spiders left!\n\r", spidernum);
stc(log_buf, ch);
}
if (crashAbort)
{
sprintf(log_buf, "&RThe mud is set to 'abort' mode. Read help abort to find out what this means.\n\r");
stc(log_buf, ch);
}
 sprintf(buf, " &Y%d &Gvisible players are online. ", count);
len = ((75/5)-(len_bash_color(buf)/5));
 stc( centerjob("&W/-_-\\", buf, len), ch);
 stc("\n\r\n\r", ch);
 return;
}

void set_len(char *buff, int len)
{
 int iA;

 for (iA = 0;*buff && iA < len; buff++)
 {
    if ((*buff == '#' && *(buff+1) != '#') || (*buff == '&' && *(buff+1) != '&'))
	buff++;
    else
	iA++;
 }
 *buff = '\0';

 return;
}

void do_beep(CHAR_DATA *ch, char *argument)
{
//(imm only)
 char arg1[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg1);

 if((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They're not here!\n\r", ch);
    return;
 }

 printf(log_buf, "\a%s\a\n\r", argument);
 stc(log_buf, vict);

 return;
}

void do_die(CHAR_DATA *ch, char *argument)
{
 if (!IS_NPC(ch)) 
{stc("Huh?\n\r", ch);return;}
 if (!ch->wizard) return;

 act("$n falls to the ground, dead, from the call of $m master.",
	ch,0,0,TO_ROOM);
 stc("You fall to the ground, dead, by the call of your master.", ch);
 stc("YOU ARE DEAD!!!\n\r", ch);

 extract_char(ch,TRUE);
 return;
}

void do_set(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 int cost = 400;

 if (ch->tier < 2)
 {
    stc("You need to be tier two to use those commands.\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg1);

 if (arg1[0] == '\0' || !*argument)
 {
    sprintf(log_buf, "The only current option is: decap (sets your personal decap message)
\rcosts %d qps.\n\r", cost);
    stc(log_buf, ch);
    stc("Format: set [option] [message]\n\r", ch);
    return;
 }

 if (!str_cmp(arg1, "decap"))
 {
    if (ch->pcdata->quest < cost)
    {
	sprintf(log_buf, "You need %d quest points to set that!\n\r", cost);
	stc(log_buf, ch);
	return;
    }
  {
    bool fn = FALSE;
    bool fN = FALSE;
    char *p = argument;

    while (*p) {if (*p == '$') {p++;if (*p =='n') fn = TRUE;else if (*p=='N')
	fN = TRUE;} p++;}

    if (!fn || !fN)
    {
	stc("The decap message must contain $n for your name and $N for your victims name.\n\r",ch);
	return;
    }
  }

    ch->pcdata->quest -= cost;

    free_string(ch->decap);
    ch->decap = str_dup(argument);
    stc("Set.\n\r", ch);
    if (!do_decapmessage(ch,find_char("a")))
    {
	stc("Hummm, it didn't work. Contact Smoo.\n\r", ch);
	stc("&YQp restored.\n\r#n", ch);
	ch->pcdata->quest+= cost;
	return;
    }
    save_char_obj(ch);
    return;
 }

 do_set(ch,"");
 return;
}

bool do_decapmessage(CHAR_DATA *ch, CHAR_DATA *vict)
{
 char buf[2000];
    if (ch->tier < 2 || strlen(ch->decap) <= strlen("(null)"))
	return FALSE;

    if (strlen(ch->decap) > 1500) return FALSE;

    strcpy(buf, "&CInfo -> &R");
    strcat(buf, ch->decap);
    strcat(buf, "#n");

    gsact(buf, ch, 0,vict, TO_ROOM, CHANNEL_INFO);
    gsact(buf, ch, 0,vict, TO_CHAR, CHANNEL_INFO);
    return TRUE;
}


void do_unrestore(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char *whatev = malloc(strlen(argument)+1);
 strcpy(whatev, argument);

 if ((vict = get_char_room(ch,argument))==NULL)
    vict = get_char_world(ch, whatev);

 free(whatev);

 if (!str_cmp(argument, "all"))
 {
  DESCRIPTOR_DATA *d;
 for (d = descriptor_list; d; d = d->next)
 {
    if (!d->character || d->connected > CON_PLAYING || IS_NPC(d->character))
	continue;
    do_unrestore(ch,d->character->pcdata->switchname);
 }
 return;
 }
 if (!*argument || vict == NULL)
 {
    stc("Unrestore who??\n\r", ch);
    return;
 }

 vict->hit = 1;
 vict->mana = 1;
 vict->move = 1;

 sprintf(log_buf, "&G%s &CHas &Mbeen &YUN-&RRESTORED&W!! #*K#*I#*L#*L#*!#n",
    IS_NPC(vict) ? vict->short_descr : vict->pcdata->switchname);
 do_info(ch, log_buf);
 return;
}

void do_gsocial(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 argument = one_argument(argument, arg1);

if (ch->act & PLR_SILENCE)
{
    stc("You are silenced..shh and shush..\n\r", ch);
    return;
}

if (ch->deaf & CHANNEL_GSOCIAL)
{
    stc("But you are deaf to gsocials!\n\r", ch);
    return;
}

if (arg1[0] == '\0')
{
    stc("Choose a social to use!\n\r", ch);
    return;
}

 if (! check_social(ch, arg1, argument,TYPE_GSOCIAL))
 {
    stc("That social doesn't exist!\n\r", ch);
    return;
 }

 return;
}

void do_delete(CHAR_DATA *ch, char *argument)
{
 if (IS_NPC(ch)) return;

 if (ch->flag3 & AFF3_DELETE && !strcmp( crypt(argument,
ch->pcdata->pwd), ch->pcdata->pwd))
 {
    char buf[MIL];
    char buf2[MIL];
    char buf3[MIL];

    sprintf(log_buf, "%s has deleted.", ch->pcdata->switchname);
    log_string(log_buf);

    strcpy(buf,"../player/");
    strcat(buf,capitalize(ch->pcdata->switchname));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(ch->pcdata->switchname));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(ch->pcdata->switchname));

    stc("#6You have been deleted.\n\r", ch);
    do_quit(ch,"");

    unlink(buf);
    unlink(buf2);
    unlink(buf3);
    return;
 }
 else if (!strcmp( crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd))
 {
    ch->flag3 |= AFF3_DELETE;
    stc("#3You have been given the delete flag. This means that if you type
\rdelete (your password) one more time, you will be deleted immediately.
\rto cancel this, type delete (anything else)\n\r", ch);
    return;
 }
 else if (ch->flag3 & AFF3_DELETE)
 {
    stc("#3Delete flag removed.\n\r", ch);
    REMOVE_BIT(ch->flag3, AFF3_DELETE);
    return;
 }
 else
 {
    stc("#3To delete your character, type delete (your password)
\rand then another time to confirm.\n\r", ch);
    return;
 }
 return;
}

void do_pdelete(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char buf[MIL];
 char buf2[MIL];
 char buf3[MIL];


 if (!*argument)
 {
if (ch)    stc("Delete whom?\n\r", ch);
    return;
 }
 if (!ch||(vict = get_char_world(ch, argument)) == NULL)
 {
    strcpy(buf,"../player/");
    strcat(buf,capitalize(argument));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(argument));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(argument));

if (ch)    stc("They have been deleted.\n\r", ch);

    unlink(buf);
    unlink(buf2);
    unlink(buf3);

    return;
 }
if (!ch) return;
    strcpy(buf,"../player/");
    strcat(buf,capitalize(vict->pcdata->switchname));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(vict->pcdata->switchname));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(vict->pcdata->switchname));

    stc("#6You have been deleted.\n\r", vict);
    stc("#7Deleted.\n\r", ch);
    vict->fighting = NULL;
    vict->position = POS_STANDING;
    do_quit(vict,"");

    unlink(buf);
    unlink(buf2);
    unlink(buf3);

    return;
}

void do_eatmortal(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 OBJ_DATA *obj;
 OBJ_DATA *obj2;

 if (IS_NPC(ch)) return;

 if (!*argument)
 {
    stc("Eat whom?\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, argument)) == NULL)
 {
    stc("They aren't close enough to your stomach! (not in this realm)\n\r", ch);
    return;
 }

 if (IS_NPC(vict))
 {
    stc("But that would crash :(( You wouldn't want that, would you?\n\r", ch);
    return;
 }

if ((obj = get_obj_carry(ch, "stomach")) == NULL)
{ obj = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);
 obj_to_char(obj, ch);}
 obj2 = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);


 sprintf(log_buf, "&B%s's&R stomach#n", ch->pcdata->switchname);
 free_string(obj->short_descr);
 obj->short_descr = str_dup(log_buf);

 sprintf(log_buf, "%s stomach", ch->pcdata->switchname);
 free_string(obj->name);
 obj->name = str_dup(log_buf);

 obj->item_type = ITEM_CONTAINER;

 char_from_room(vict);

 obj_to_obj(obj2, obj);

 sprintf(log_buf, "&GTorn up pieces of %s#n", PERS(vict,ch));
 free_string(obj2->short_descr);
 obj2->short_descr = str_dup(log_buf);

 sprintf(log_buf, "%s pieces torn", ch->pcdata->switchname);
 free_string(obj2->name);
 obj2->name = str_dup(log_buf);

 sprintf(log_buf, "&GSome torn up pieces of %s are here.\n\r",
	PERS(vict,ch));
 free_string(obj2->description);
 obj2->description = str_dup(log_buf);

 sprintf(log_buf, "\rYou are in here for sins against the gods, and you will remain 
\runtil told to move. HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\r Sucks 2 B U - %s\n\r", ch->pcdata->switchname);
 free_string(obj->description);
 obj->description = str_dup(log_buf);


    vict->pcdata->obj_vnum = 0;
    obj2->chobj = vict;
    vict->pcdata->chobj = obj2;
    SET_BIT(vict->affected_by, AFF_POLYMORPH);
    SET_BIT(vict->extra, EXTRA_OSWITCH);
    free_string(vict->morph);
    vict->morph = str_dup(obj2->short_descr);
do_help(vict, "---head");
 return;
}

void do_curseditem(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL];
 OBJ_DATA *tell;

argument = one_argument(argument, arg1);

 if (!*arg1 || !*argument)
 {
    stc("Send what to whom? ie curseditem smoo [tell/trans/restore]\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 tell = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);

 free_string(tell->short_descr);
 sprintf(log_buf, "a %s", argument);
 tell->short_descr = str_dup(log_buf);
 free_string(tell->name);
 tell->name = str_dup("");
 if (!IS_NPC(vict))
 {
    free_string(tell->questowner);
    tell->questowner = str_dup(vict->pcdata->switchname);
 }

 tell->extra_flags |= ITEM_NODROP;
tell->timer = 100;
 obj_to_char(tell,vict);
 act("$p appears in your hands in a swirl of smoke.",vict,tell,0,TO_CHAR);
 act("You send them a none-named, none-droppable $t.",ch,argument,0,TO_CHAR);
 return;
}

AFFECT_DATA blank_aff(int sn)
{
 AFFECT_DATA af;

 af.type = sn;
 af.duration = 0;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.bitvector = 0;

 return af;
}

char *bash_occurances(char *txt, char bash);
void do_helpdump(CHAR_DATA *ch, char *argument)
{
    if (!*argument || str_cmp(argument, "halflife"))
    {
	stc("This command dumps all the helpfiles into help.are.
\rThis was created because for some reason, all the helpfiles 
\rgot entered inside help.are about 30 times each. Probably from 
\rwhen hedit was bugged.\n\r", ch);
	stc("Look at the code to find out how to use it, if you need to.\n\r", ch);
	return;
    }

    {
      HELP_DATA *hlp, *hlp_next;
      HELP_DATA *list = NULL;
      FILE *fp;
      char buf[1000];

    {int count = 0;
	for (hlp = first_help; hlp; hlp = hlp_next)
	{HELP_DATA *hl;
	    hlp_next = hlp->next;

	    for (hl = list; hl; hl = hl->next)
		if (!str_cmp(hl->keyword, hlp->keyword))
		    break;

	    if (!hl)
	    {
		hlp->next = list;
		list = hlp;
	    }
	    else count++;
	}
    sprintf(log_buf, "Deleted: %d helpfiles.", count);
    log_string(log_buf);
    first_help = list;
    }

	strcpy(buf, global("areadir"));
	strcat(buf, "/");
	strcat(buf, "help.are");

	fp = fopen(buf, "w");
	if (!fp)
	{
	    sprintf(log_buf, "Unable to open file: %s for write access.",
		buf);
	    stc(log_buf, ch);
	    return;
	}

	fprintf(fp, "Register\n\n#HELPS\n\n");

	for (hlp = first_help; hlp; hlp = hlp->next)
	{
	    if (!str_cmp(hlp->keyword, "'seven holy places'"))
		continue;
	    fprintf(fp, "%d %s~\n%s~\n\n", hlp->level, hlp->keyword, 
		bash_occurances(hlp->text, '\r'));
	    fflush(fp);
	}

	fprintf(fp, "0 $~\n#$\n");
	fflush(fp);
	fclose(fp);
	stc("Done.\n\r", ch);
    }
    return;
}

char *bash_occurances(char *txt, char bash)
{
 static char buf[MAX_INPUT_LENGTH*30];
 char *p = buf;

 while (*txt)
 {
    if (*txt == bash)
    {
	txt++;
	continue;
    }
    *p = *txt;
    txt++, p++;
 }
 *p = '\0';
 return buf;
}

void do_tier(CHAR_DATA *ch, char *argument)
{
    char box[51] = "";
    int a = 0;

    if (ch->tier == 2)
    {
	stc("Currently, only two tiers are coded.\n\r", ch);
	return;
    }

    for (a = 0; a < ch->tierpt/40 && a < 50; a++)
	strcat(box, "X");

    if (!*argument)
    {
	sprintf(log_buf, "&GYou are currently in tier &Y%d&G.
\rPoints to next tier:
\r&R[&Y%-50s&R]&m.\n\r", ch->tier, box);
	stc(log_buf, ch);
	sprintf(log_buf, "&B(&Y%d&R X's out of &Y%d&B)\n\r", strlen(box), 50);
	stc(log_buf, ch);
if (ch->act & ACT_TIERTRAIN)
	stc("Use tier train to start training towards another tier. 
\rOnce your box is full and you have the stat requirements, use tier raise.\n\r", ch);
else
	stc("Use tier train to stop training towards your tier and continue to recieve exp. 
\rOnce your box is full and you have the stat requirements, use tier raise.\n\r", ch);
	return;
    }
    if (!str_cmp(argument, "train"))
    {
	if (IS_SET(ch->act, ACT_TIERTRAIN))
	{
	    REMOVE_BIT(ch->act, ACT_TIERTRAIN);
	    stc("You are no longer training towards your tier.\n\r", ch);
	}
	else
	{
	    SET_BIT(ch->act, ACT_TIERTRAIN);
	    stc("You are now training towards your tier.\n\r", ch);
	}
	return;
    }
    if (!str_cmp(argument, "raise"))
    {
	do_mclear(ch, "self");
	if (ch->max_hit < 25000 || (ch->max_move < 10000 && ch->max_mana < 10000))
	{
	    stc("You need 25k hp and either 10k mana or move to gain a tier.\n\r", ch);
	    return;
	}

	if (ch->tierpt < 2000)
	{
	    stc("You are not ready yet!\n\r", ch);
	    return;
	}

	ch->tier++;
	ch->tierpt = 0;
	ch->max_hit -= 24000;
	if (ch->max_move >= 10000)
	    ch->max_move -= 8500;
	else
	    ch->max_mana -= 8500;
	if (ch->max_hit > 15000) ch->max_hit = 15000;
	if (ch->max_mana > 15000) ch->max_mana = 15000;
	if (ch->max_move > 15000) ch->max_move = 15000;
	REMOVE_BIT(ch->act, ACT_TIERTRAIN);
	sprintf(log_buf, "&RWHOOT &BWHOOT&*!&Y! &G%s has gained a &MTIER!&w", ch->pcdata->switchname);
	do_info(ch, log_buf);
	return;
    }
    do_tier(ch, "");

    return;
}

void do_tap(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj, *obj_next; int exp=0;

    for (obj = ch->in_room->contents; obj; obj = obj_next)
    {
	obj_next = obj->next_content;

	if (obj->chobj) continue;
	if (!IS_SET(obj->wear_flags,ITEM_TAKE)) continue;
	if (obj->questowner && strlen(obj->questowner) > 1) continue;
	if (obj->quest & QUEST_ARTIFACT) continue;
	if (obj->quest & QUEST_RELIC) continue;
	obj_from_room(obj);
	exp += obj->level*10;
	extract_obj(obj);
    }

    sprintf(log_buf, "&RYou earn &Y%d&R exp for your sacrifice.\n\r", exp);
    stc(log_buf, ch);
    ch->exp += exp;
    act("$n sacrifices everything in the room to $t.", ch,glorify_name(ch->glorify),0,TO_ROOM);
    return;
}

bool workfiling = FALSE;
void workfile_compute(FILE *fp)
{
  char buf[1000];
  char *pbuf = buf;
  int cmd = 0;
  char c;
  bool eof = FALSE;

if (workfiling) return;
workfiling = TRUE;
  while ((c = getc(fp)) != EOF)
  {pbuf = buf;
   ungetc(c, fp);
	pbuf = fread_word(fp);

	if (!str_cmp(pbuf, "notop"))
	{
	    if (dont_update_top) dont_update_top = FALSE;
		else dont_update_top = TRUE;
	    fread_to_eol(fp);
	}
	else if (!str_cmp(pbuf, "spiderquest"))
	{
	    spidernum = atoi(fread_word(fp));
	    spiderquest = TRUE;
	    { DESCRIPTOR_DATA *d;
		for (d = descriptor_list; d; d = d->next)
		    if (d->character)
			stc("&WThe mayor of Thalos yells out, \"&CHELP ME PLEASE! ALL THE HEROES IN THE WORLD GATHER! The city of thalos is being taken over by horrid spiders. i will award you for each one you kill. Just save my precious town.&W\"&x\n\r", d->character);
	    }
	    {int a; ROOM_INDEX_DATA *rm = get_room_index(2);
		for (a = spidernum; a; a--) 
			make_questspider(rm);}
	}
	else if (!str_cmp(pbuf, "artiquest")) // don't use, not complete
	{char cCh;
	    FILE *f = fopen("temp.stfu", "w");
	    while (putc((cCh = getc(fp)), f)) if (cCh == EOF) break;
	    fflush(f);fclose(f);fclose(fp);rename("temp.stfu", workfile);
	    init_artiquest();
	}
	else if (!str_cmp(pbuf, "log"))
	{
	 char str[2000]; char *p = str; char cCh;
	    while ((*p = getc(fp)) != '\n' && *p != '\r' && *p != EOF)
		p++;

	    *p = '\0';

	    while ((cCh = getc(fp)) == '\n' || cCh == '\r')
		;

	    log_string(str);
	}
	else if (!str_cmp(pbuf, "do"))
	{char str[2000]; char *p = str; int c=0;
	    pbuf = fread_word(fp);

	    if ((*p = getc(fp)) != ' ' ) ungetc(*p,fp);

	    while ((*p = getc(fp)) != '\n' && *p != '\r' && *p != EOF)
		p++;

	    while ((*p = getc(fp)) == '\n' || *p == '\r' || *p == ' ') ;
	    ungetc(*p, fp);

	    if (*p == EOF) eof=TRUE;

	    *p = '\0';

	    p = str;
	    while (*p) {if (*p == '\n') c++; *p = *(p+c); p++;}

	    if (*(p-1)=='\n') *(p-1)=0;
	    for (cmd = 0; cmd_table[cmd].name[0]; cmd++)
		if (!str_prefix(pbuf, cmd_table[cmd].name))
		{
		    DESCRIPTOR_DATA *d;
		    for (d = descriptor_list; d; d = d->next)
			if (d->character && d->connected == CON_PLAYING)
			{    (cmd_table[cmd].do_fun)(d->character,str); break;}
		    if (!d) (cmd_table[cmd].do_fun)(0,str);

		    break;
		}
	}
	else if (!str_cmp(pbuf, "force"))
	{char str[2000]; char *p = str; char str2[2000]; char *p2=str2;
	 char buf2[1000];
	 CHAR_DATA *ch;

	    pbuf = fread_word(fp);
	strcpy(buf2,pbuf);
	    p2 = fread_word(fp);

	    if ((*p = getc(fp)) != ' ') ungetc(*p,fp);

	    while ((*p = getc(fp)) != '\n' && *p != '\r' && *p != EOF)
		p++;
	    ungetc(*p, fp);

	    while ((*p = getc(fp)) == '\n' || *p == '\r' || *p == ' ') ;
	    ungetc(*p, fp);

	    if (*p == EOF) eof=TRUE;

	    *p = '\0';

	    for (cmd = 0; cmd_table[cmd].name[0]; cmd++)
		if (!str_prefix(p2, cmd_table[cmd].name))
		{
			if ((ch=find_char(buf2)) == NULL) break;
			(cmd_table[cmd].do_fun)(ch,str); break;
			break;
		}
	} 
else fread_to_eol(fp);

  while ((c = getc(fp)) == ' ' || c == '\n' || c == '\r') ;
  ungetc(c,fp);
  if ((c = getc(fp)) != EOF)
  ungetc(c, fp);
  else break;

  }

  fclose(fp);
  unlink(workfile);

  workfiling = FALSE;
  return;
}
extern int port;
void check_resolve()
{
    FILE *fp;

    sprintf(log_buf, "../src/lookup%d", port);
    if ((fp = fopen(log_buf,"r")) == NULL) return;

    while (!feof(fp))
    { char *o1, *o2;
	DESCRIPTOR_DATA *d;

	o1 = str_dup(fread_word(fp));
	if (feof(fp)) break;
	o2 = str_dup(fread_word(fp));

	sprintf(log_buf, "%s resolved to %s.", o1, o2);
	log_string(log_buf);
	for (d = descriptor_list;d; d = d->next)
	    if (!str_cmp(d->host, o1)) {
		free_string(d->host);
		d->host = str_dup(o2);
		if (d->character)
		{
		    free_string(d->character->lasthost);
		    d->character->lasthost = str_dup(o2);
		}
}
	free_string(o1);
	free_string(o2);
	getc(fp); // will be ' ' or EOF
    }

    fclose(fp);
    sprintf(log_buf, "../src/lookup%d", port);
    unlink(log_buf);
}

CHAR_DATA *find_char(char *name)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch;

    for (d = descriptor_list; d; d = d->next)
    {
	if (!d->character || d->connected > CON_PLAYING)
	    continue;

	if (!IS_NPC(d->character) && !str_cmp(d->character->pcdata->switchname,
	    name))
	    break;
    }

    if (d) return d->character;

    for (ch = char_list; ch; ch = ch->next)
    {
	if (!is_name(name, ch->name))
	    break;
    }

    return ch;
}

extern char str_boot_time[];
void init_descriptor(DESCRIPTOR_DATA *, int);
//this should only be called on the mud bootup, ie ./merc -pwipe
//it moves old players to the ../pwipe directory for you to delete.
void pwipe()
{
    DESCRIPTOR_DATA *d;
    struct dirent *Dir;
    struct stat Stat;
    DIR *Directory;
    char FName[80];
    int timestamp = 0;
    bool des;

    d = malloc(sizeof(DESCRIPTOR_DATA));
    init_descriptor(d,0);
    d->connected = CON_PLAYING;

des = FALSE;
    append(workfile,"do echo a pwipe is being initiated inside the shell for players under 2000 hp who haven't logged on in 10 days, and players under 5000 hp who haven't logged in for 15 days, and also players who haven't logged in for 45 days and are tier 1.\nnotop\n");

/*
des = TRUE;
    append(workfile,"do echo A player sensitation is occuring. All pfiles will be altered in the manner coded by an immortal that is online.\nnotop\n");
*/
    sprintf(log_buf, "rm -f %s", global("topscores_file"));
    system(log_buf);

    Directory=opendir(global("playerdir"));
    Dir = readdir(Directory);
  log_string("Removing old pwipe directory. If this 'fails', don't worry");
    system("rm -R ../pwipe");
  log_string("Creating new pwipe directory.");
    system("mkdir ../pwipe");
    while (Dir)
    {
	sprintf(FName, "%s/%s", global("playerdir"),Dir->d_name);
	stat(FName,&Stat);

	if (S_ISREG(Stat.st_mode)) {
	CHAR_DATA *ch;
	    if (Dir->d_name[0] >= 'A' && Dir->d_name[0] <= 'Z') {
		load_char_obj(d,Dir->d_name);
		ch = d->character;
if (des)
{int old = ch->generation; char *p = ch->clan;
log_string(FName);
    if (!IS_CLASS(ch, CLASS_DEMON)) ch->generation = class_table[class_lookup(ch->class)].start_gen;
    else if (ch->generation >12 && ch->generation <= 15)
	ch->generation = 12;

 if (str_cmp(ch->clan, "(null)") && str_cmp(ch->clan,""))
 {
    free_string(ch->clan);
    ch->clan = str_dup("");
 }
    top_compute(ch);
if (p != ch->clan || old != ch->generation)
    save_char_obj(ch);
    extract_char(ch,FALSE);
    Dir = readdir(Directory);
    continue;
}

	if (str_cmp(ch->lasttime, ""))
	    timestamp = TimeStamp(ch->lasttime);
	else
	    timestamp = TimeStamp(ch->createtime);
		if ((((TimeStamp(str_boot_time)-timestamp)/60/60/24) > 10
&& ch->max_hit < 2000 && ch->tier <= 1 && ch->level < 7 && ch->trust < 7)
|| (((TimeStamp(str_boot_time)-timestamp)/60/60/24) > 15 && ch->max_hit < 5000 && ch->tier <=1 && ch->level < 7 && ch->trust < 7)
|| (ch->tier<=1 && ((TimeStamp(str_boot_time)-timestamp)/60/60/24)>=45))
		{
		    if (str_cmp(ch->pcdata->switchname, "Malum")) {
		    printf("%d", (timestamp/60/60/24)-((timestamp/60/60/24/31)*31));
		    sprintf(log_buf, "cp -f %s ../pwipe", FName);
		    system(log_buf);
		    printf("%s wiped (moved to ../pwipe directory).\n", FName);
		    do_pdelete(0,ch->pcdata->switchname);}else top_compute(ch);
		}
		else top_compute(ch);
		extract_char(ch,TRUE);
	    }
	}
	Dir = readdir(Directory);
    }
    closedir(Directory);
    free(d);

    sprintf(FName, "%s/notes.txt", global("areadir"));
sprintf(log_buf, "Sender Mudaholic~
Date %s~
To all~
Subject players deleted in the recent newbiewipe~
Text
", mud_time);
    append(FName, log_buf);
sprintf(FName, "ls ../pwipe >>%s/notes.txt", global("areadir"));
system(FName);
sprintf(FName, "%s/notes.txt", global("areadir"));
append(FName, "If you need any of these players back, contact an imm before they get permenatly deleted.\n");
append(FName, "~");

append(workfile, "\ndo echo &BThe mud is done executing the newbie wipe. Rebooting.
\ndo echo &Rdirectly after the reboot, you may read the last note to see the deleted players.
\ndo hotboot now\n");
    printf("Complete.\n");
}

int TimeStamp(char *str)
{
    int stamp = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    int year = 0;
    char tmp[20];
    char *p = tmp;

    if (strlen(str) < 4) return 0; // error in string given
#define BASH while (*str == ' ' || *str == '\n' || *str == '\r') str++;
    BASH
    str += 3;
    BASH
    while (*str != ' ')
    {
	*p = *str;
	p++,str++;
    }

    *p = '\0';
#define mo(str,di) !str_cmp(tmp,str) ? di :
    month = mo("jan",1) mo("feb",2) mo("mar",3) mo("apr",4) mo("may",5)
	    mo("jun",6) mo("jul",7) mo("aug",8) mo("sep",9) mo("oct",10)
	    mo("nov",11) 12;
#undef mo
    p = tmp;
    BASH
    while (*str != ' ')
    {
	*p = *str;
	p++,str++;
    }
    *p = '\0';
    day = atoi(tmp);
    BASH
    p = tmp;
    while (*str != ':')
    { *p = *str; p++,str++; }
    *p = '\0'; hour = atoi(tmp);
    p = tmp;
    str++;
    while (*str != ':')
    { *p = *str; p++,str++; }
    *p = '\0'; min = atoi(tmp);
    p = tmp;
    str++;
    while (*str != ' ')
    { *p = *str; p++,str++; }
    *p = '\0'; sec = atoi(tmp);
    BASH
    year = atoi(str);
    stamp = sec + (min*60)+(hour*60*60)+(day*60*60*24)+(month*60*60*24*31);
    return stamp;
}

void do_glorify(CHAR_DATA *ch, char *argument)
{
    if (ch->tier < 2)
    {
	stc("You are unworthy.\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("Glorify who..\n\rSmoo/Jeremy/Geonn/Tanais/Myzery", ch);
	return;
    }
/*
    if (IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	stc("You are an immortal. You do not need to follow the whims and desires of the Gods.\n\r(Highlanders can't use this)\n\r", ch);
	return;
    }
*/
    if (ch->glorify != 0)
    {
	if (!str_cmp(argument, "clear"))
	{
	    if (ch->pcdata->quest<300)
	    {
		stc("You don't have the required 300 qps.\n\r", ch);
		return;
	    }
	    ch->pcdata->quest-=300;
	    ch->glorify=0;
	    stc("Cleared.\n\r", ch);
	    gsact("&R$n yells, '&CGLORIFICATION IS FOR THE WIMPS.&R'",ch,0,0,TO_ROOM,CHANNEL_GSOCIAL);
	    gsact("&RYou yell, '&CGLORIFICATION IS FOR THE WIMPS.&R'",ch,0,0,TO_CHAR,CHANNEL_GSOCIAL);
	    return;
	}
	else {
	    sprintf(log_buf, "&YYou already worship %s&Y!#n\n\r", glorify_name(ch->glorify));
	    stc("Use glorify clear to clear it. It costs 500 qps.\n\r", ch);
	    return;
	}
    }

    ch->glorify =
!str_cmp(argument,"Smoo")?GLOR_SMOO:!str_cmp(argument,"Jeremy")?GLOR_JEREMY:!str_cmp(argument,"Geonn")?GLOR_NECRON:!str_cmp(argument,"Tanais")?GLOR_JAMES:!str_cmp(argument,"Neka")?GLOR_NEKA:!str_cmp(argument,
"Myzery")?GLOR_ZARAF:0;
    if (ch->glorify==0)
    {
	do_glorify(ch,"");
	return;
    }

    gsact("&W$n sings, '&YWe glorify your name...we glorify your name..we glorify your name oh $t, we glorify your name.&W'#n", ch, glorify_name(ch->glorify), 0, TO_ROOM,CHANNEL_GSOCIAL);
    gsact("&WYou sing, '&YWe glorify your name...we glorify your name..we glorify your name oh $t, we glorify your name.&W'#n", ch, glorify_name(ch->glorify), 0, TO_CHAR,CHANNEL_GSOCIAL);
    return;
}

char *glorify_name(int glor)
{
    if (glor == GLOR_SMOO) return "Smoo";
    if (glor == GLOR_JEREMY) return "Jeremy";
    if (glor >= GLOR_JAMES) return "Tanais";
    if (glor == GLOR_NECRON) return "Geonn";
    if (glor == GLOR_NEKA) return "&wUnsung Hero&Y";
    if (glor == GLOR_ZARAF) return "Myzery";
    else return "nobody";
}

char exp_name[10][200]={"","","","","","","","","",""};
int exp_top[10]={0,0,0,0,0,0,0,0,0,0};
char pk_name[10][200]={"","","","","","","","","",""};
float pk_top[10]={0,0,0,0,0,0,0,0,0,0};
char qp_name[10][200]={"","","","","","","","","",""};
int qp_top[10]={0,0,0,0,0,0,0,0,0,0};

#define TABLEENTRY(a) {exp_name[a],&exp_top[a],pk_name[a],&pk_top[a],qp_name[a],&qp_top[a]}
const struct top_data top_table[10]=
{
    TABLEENTRY(0),
    TABLEENTRY(1),
    TABLEENTRY(2),
    TABLEENTRY(3),
    TABLEENTRY(4),
    TABLEENTRY(5),
    TABLEENTRY(6),
    TABLEENTRY(7),
    TABLEENTRY(8),
    TABLEENTRY(9)
};

void do_tops(CHAR_DATA *ch, char *argument)
{
    int a = 0;
    int len = 80-strlen("top scorers");
    len /= 3;
    stc(centerjob("&R-&Y_&B-", "&GTop Scorers", len),ch);
    stc("\n\r", ch);
    for (a = 0; a < sizeof(top_table)/sizeof(struct top_data); a++)
    {
	sprintf(log_buf, "%13s %05db | %13s %s%2.02fpk/pd | %13s %04dq/c\n\r",
	    top_table[a].exp_name, *top_table[a].exp, top_table[a].pk_name,
	    *top_table[a].pk>9?"":" ",*top_table[a].pk, top_table[a].qp_name, *top_table[a].qp);
	stc(log_buf, ch);
    }
    return;
}

void top_compute(CHAR_DATA *ch)
{
    if (IS_NPC(ch)) return;
    if (dont_update_top) return;

    insert_in2(ch->pcdata->switchname,ch->pcdata->score[SCORE_HIGH_XP], exp_name, exp_top);
    insert_in2(ch->pcdata->switchname,ch->pcdata->score[SCORE_NUM_QUEST], qp_name, qp_top);
    insert_in1(ch->pcdata->switchname,ch->pdeath==0?ch->pkill>0?ch->pkill:0:((float)ch->pkill/(float)ch->pdeath),pk_name,pk_top);
}

void insert_in1(char *name, float cur, char namelist[][200], float statlist[])
{
    int a = 0;
    bool b1;

    for (b1= FALSE,a = 0; a < 10&&!b1; a++)
    {
	if (cur > statlist[a] || (cur==statlist[a]&&str_cmp(name,namelist[a])))
	{
	    if (!str_cmp(namelist[a],name))
		statlist[a] = cur;
	    else
	    {
		int b;
		for (b = 9; b > a; b--)
		{
		    strcpy(namelist[b], namelist[b-1]);
		    statlist[b] = statlist[b-1];
		}
		for (b = a+1; b < 10; b++)
		{
		    if (!str_cmp(namelist[b],name))
		    {
			while (b < 10)
			{
			    strcpy(namelist[b], b ==9?"Empty":namelist[b+1]);
			    b++;
			}
		    }
		}
		strcpy(namelist[a], name);
		statlist[a] = cur;
	    }
	    b1=TRUE;
	    break;
	}
	else if (!str_cmp(name,namelist[a])) break;
    }
    if (b1) save_topscores();
    return;
}


void insert_in2(char *name, int cur, char namelist[][200], int statlist[])
{
    int a = 0;
    bool b1;

    for (b1= FALSE,a = 0; a < 10&&!b1; a++)
    {
	if (cur > statlist[a] || (cur==statlist[a]&&str_cmp(name,namelist[a])))
	{
	    if (!str_cmp(namelist[a],name))
		statlist[a] = cur;
	    else
	    {
		int b;
		for (b = 9; b > a; b--)
		{
		    strcpy(namelist[b], namelist[b-1]);
		    statlist[b] = statlist[b-1];
		}
		for (b = a+1; b < 10; b++)
		{
		    if (!str_cmp(namelist[b],name))
		    {
			while (b < 10)
			{
			    strcpy(namelist[b], b ==9?"Empty":namelist[b+1]);
			    statlist[b] = b==9?0:statlist[b+1];
			    b++;
			}
		    }
		}
		strcpy(namelist[a], name);
		statlist[a] = cur;
	    }
	    b1=TRUE;
	    break;
	}
	else if (!str_cmp(name,namelist[a])) break;
    }
    if (b1) save_topscores();
    return;
}


void save_topscores()
{
    int a;
    FILE *fp = fopen(global("topscores_file"), "w");

    if (!fp)
    {perror("fopen: ");log_string("Bug, failed to save topscores. Look at the log file for more info.");return;}

    for (a = 0; a < sizeof(top_table)/sizeof(struct top_data); a++)
	fprintf(fp, "%d %s~ %d %s~ %d %s~ %f\n", a, exp_name[a], exp_top[a],
		qp_name[a], qp_top[a], pk_name[a], pk_top[a]);

    fflush(fp);
    fclose(fp);
}

void load_topscores()
{
    int a;
    char c;
    FILE *fp = fopen(global("topscores_file"), "r");

    if (!fp) {save_topscores();return;}

    while ((c = getc(fp)) != EOF)
    {
	ungetc(c,fp);
	a = fread_number(fp);
	strcpy(exp_name[a], fread_string(fp));
	exp_top[a] = fread_number(fp);
	strcpy(qp_name[a], fread_string(fp));
	qp_top[a] = fread_number(fp);
	strcpy(pk_name[a], fread_string(fp));
	pk_top[a] = atof(fread_word(fp));
	while ((c = getc(fp)) == '\r' || c == '\n' || c == ' ') ;
	    ungetc(c,fp);
    }

    fclose(fp);
}

void do_gemote(CHAR_DATA *ch, char *argument)
{
    char buf[MIL*2];
    if (!*argument)
    {
	stc("Global emote what??\n\r", ch);
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	stc("You are silenced=(\n\rStop doing stuff to get you silenced. Annoys me.\n\rHere is an example of why you would get silenced: spamming! like this following example\n\r", ch);
	do_help(ch,"---head");
	return;
    }

    sprintf(buf,"&R[&GGEmote&R] &C%s ", IS_NPC(ch)?ch->short_descr:ch->level>6?(IS_SET(ch->act,PLR_INCOG) || IS_SET(ch->act,PLR_SACREDINVIS) || IS_SET(ch->act,PLR_WIZINVIS))?"&GI&Ym&Gm&Yy&C":ch->pcdata->switchname:ch->pcdata->switchname);
    strcat(buf,argument);
    strcat(buf, "#n");
    gsact(buf,ch,0,0,TO_CHAR,CHANNEL_GSOCIAL);
    gsact(buf,ch,0,0,TO_ROOM,CHANNEL_GSOCIAL);
    return;
}

int finger_eof_handler(FILE *fp,int action)
{
    static CHAR_DATA *to = NULL;
    static int *times = 0;

    switch (action)
    {
	case FEOF_INITME:
	    times=0; to = (CHAR_DATA*)fp;
	    break;
	case FEOF_FEOF:
	    if (to && !times) {stc("&GThis finger file is bugged. The output may look distorted.&w\n\r", to);
		times++;
		return 1;}
	case FEOFF_RESETME:
	    times=0; to = NULL;
	    break;
	case FEOFF_SETCHAR:
	    to = (CHAR_DATA*)fp;
	    break;
	default:
	    bug("Erroronic finger_eof_handler() action: d", action);
	    break;
    }

    return 1;
}

int feof_pfile_handler(FILE *fp, int action)
{
    static char pfilename[200]="None Loaded";
    switch(action)
    {
	case FEOF_INITME:
	    strcpy(pfilename,(char*)fp);
	    break;
	case FEOFP_GETNAME:
	    return (int)pfilename;
	    break;
	case FEOF_FEOF:
	{DESCRIPTOR_DATA *d;
	 char buf[2000];
	 sprintf(buf, "%s has a bugged pfile. Crashing.\n\r", pfilename);
	    for (d = descriptor_list; d; d = d->next)
		write_to_descriptor(d,buf,0);
	    crash_notice(SIGPFILE);
	    break;
	}
	default:
	    bug("Wrong handler action for feof_pfile_handler(d)", action);
	    break;
    }

    return 1;
}

void do_multicheck(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d, *d2;
    int count = 0;

    for (d = descriptor_list; d; d = d->next)
    {
	for (d2 = descriptor_list; d2; d2 = d2->next)
	{
	    if (d == d2) continue;
	    if (!d->character || !d2->character || d->connected > CON_PLAYING
		|| d2->connected > CON_PLAYING) continue;
	    if (IS_NPC(d->character) || IS_NPC(d2->character)) continue;

	    if (!str_cmp(d->host, d2->host))
	    {
		sprintf(log_buf, "%s and %s may be multiplaying.\n\r",
		    d->character->pcdata->switchname,
		    d2->character->pcdata->switchname);
		stc(log_buf, ch);
		count++;
	    }
	}
    }

    if (count==0)
	stc("There are no apparent multiplayers (it is still possible that there is)\n\r", ch);
    else
    {
	sprintf(log_buf, "You found %d multiplayers.\n\r", count/2);
	stc(log_buf, ch);
    }

    return;
}

void do_spouse(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    
    if (IS_NPC(ch)) return;


    if (!ch->pcdata->marriage || !str_cmp(ch->pcdata->marriage,"(null)") || !str_cmp(ch->pcdata->marriage, ""))
    {
	stc("You aren't married!\n\r", ch);
	return;
    }

    for (d = descriptor_list; d; d = d->next)
    {CHAR_DATA *vict = d->character;
	if (!d->character || d->connected > CON_PLAYING) continue;

	if (IS_NPC(d->character)) continue;
	if (str_cmp(vict->pcdata->marriage, ch->pcdata->switchname)
	|| str_cmp(ch->pcdata->marriage, vict->pcdata->switchname))
	    continue;

	sprintf(log_buf, "&s&C[&W%s&Y|&WSpouse&C] &M%s&t\n\r",
	    ch->pcdata->switchname, argument);
	stc(log_buf, vict);
	stc(log_buf, ch);
	break;
    }

    if (!d)
	stc("They aren't online=(\n\r", ch);
    return;
}

extern int control;

void do_setcontrol(CHAR_DATA *ch, char *argument)
{
    char passcode[MIL];

    argument = one_argument(argument, passcode);

    if (str_cmp(passcode, "pizza"))
    {
	stc("You need to know the passcode.
\rThe passcode is used, because if you pass a bad control_number to the mud, nobody will be able to connect.
\rformat: setcontrol (passcode) control_number\n\r", ch);
	return;
    }

    if (!is_number(argument) || !*argument)
    {
	stc("That is not a number.
\rFormat: setcontrol (passcode) control_number\n\r", ch);
	return;
    }

    sprintf(log_buf, "Old: %d\n\r", control);
    stc(log_buf, ch);
    control = atoi(argument);
    sprintf(log_buf, "Control is now: %d\n\r", control);
    stc(log_buf, ch);
    return;
}

int bit_count(long bit, int max)
{
 int a;
 int b;
 int c;

 for (a = 0, b = 1, c = 0; a < max && b < INT_MAX && b > 0; a++, b *= 2)
    if (bit & b) c++;

 return c;
}

void do_fdwrite(CHAR_DATA *ch, char *argument)
{
    int fd;
    char arg[MIL];

    argument = one_argument(argument, arg);

    if ((fd = atoi(arg)) == 0)
    {
	stc("Enter a descriptor number, and then the text.\n\r", ch);
	return;
    }

    if (write(fd, argument, strlen(argument)) < 0)
	stc("Error.\n\r", ch);
    else
	stc("Sent.\n\r", ch);
    return;
}

//#undef fopen
FILE *log_fopen(const char *name, const char *stat)
{
  FILE *fd = fopen(name, stat);
  FD_ENTRY *nfd;

    nfd = malloc(sizeof(*nfd));

    nfd->fd = fileno(fd);
    strncpy(nfd->name, name, 400);
    nfd->fp = fd;

    nfd->next = fd_list;
    fd_list = nfd;

    return fd;
}

//#define fopen log_fopen
//#undef fclose
void log_fclose(FILE *fd)
{
  FD_ENTRY *l, *l_p;

  for (l_p = NULL, l = fd_list; l; l_p = l, l = l->next)
    if (l->fp == fd) break;

  if (!l)
  {
    bug("Uhhohh.. Attempted to fclose a non-existant FP!", 0);
    return;    
  }

  if (!l_p)
    fd_list = l->next;
  else
    l_p->next = l->next;

  free(l);
  return;
}

//#define fclose log_fclose

void do_fdlist(CHAR_DATA *ch, char *argument)
{
    FD_ENTRY *fd;
    DESCRIPTOR_DATA *d;

    for (d = descriptor_list; d; d = d->next)
    {
	sprintf(log_buf, "&B%03d %s\n\r", d->descriptor, d->character ? d->character->pcdata ? d->character->pcdata->switchname : d->character->name : d->host);
	stc(log_buf, ch);
    }

    for (fd = fd_list; fd; fd = fd->next)
    {
	sprintf(log_buf, "&G%03d %s\n\r", fd->fd, fd->name);
	stc(log_buf, ch);
    }

    stc("&x", ch);

    return;
}

#define CHECKHEX(arg) {    if (!str_cmp(arg, "hex"))\
    {\
	hex = TRUE;\
	argument = one_argument(argument, arg);\
    }\
    else hex = FALSE; }



void do_superset(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
    bool hex = FALSE;

    argument = one_argument(argument, arg1);

    if (!str_cmp(arg1, "view"))
    { long int loc, start, end, offset;
	argument = one_argument(argument, arg2);
CHECKHEX(arg2)
	loc = hex ? atoh(arg2) : atoi(arg2);
	argument = one_argument(argument, arg3);
CHECKHEX(arg3)
	start = hex ? atoh(arg3) : atoi(arg3);
	argument = one_argument(argument, arg4);
CHECKHEX(arg4)
	end = hex ? atoh(arg4) : atoi(arg4);

	offset = loc+start;
	end += loc;
#define bps 4
	if (start % bps != 0)
	{
	    stc("You need to start with something divisible by 4.
\rEach value takes up 4(bytes).\n\r", ch);
	    return;
	}

	for ( ; offset <= end; offset += bps)
	{
stc(CHAR2((offset-start-loc)/4), ch);
if (*(char**)offset >= string_space && *(char**)offset <= top_string)
	    sprintf(arg2, "&y%p&R|&y%ld    &Yloc: %p    &Rstring: &Y%s\n\r&g",
		(void*)offset, offset, *(char**)offset, *(char**)offset);
	else
	    sprintf(arg2, "&y%p&R|&y%ld    &Gint:%d    &Bshort:%d    &Clong:%ld\n\r&g",
(void*)offset,offset,*(int*)offset,*(short*)offset,*(long*)offset);
	    stc(arg2, ch);
	}
	return;
    }

    if (!str_cmp(arg1, "set"))
    { long int loc, start, offset;
	argument = one_argument(argument, arg2);
CHECKHEX(arg2)
	loc = hex ? atoh(arg2) : atoi(arg2);
	argument = one_argument(argument, arg3);
CHECKHEX(arg3)
	start = hex ? atoh(arg3) : atoi(arg3);
	argument = one_argument(argument, arg4);
CHECKHEX(arg4)

	offset = loc+(start*4);
	if (offset % bps != 0)
	{
	    stc("Each section of memory starts at a number divisible by 4.\n\r", ch);
	    return;
	}

	if (!str_cmp(arg3, "int"))
	    *(int*)offset = atoi(argument);
	else if (!str_cmp(arg3, "long"))
	    *(long*)offset = atol(argument);
	else if (!str_cmp(arg3, "short"))
	    *(short*)offset = atoi(argument);
	else if (str_cmp(arg3, "mem"))
	    *(void**)offset = hex ? (void*)atoh(argument) : (void*)atol(argument);
	else
	    stc("\aEeeh! You must choose out of int, long, short, or mem
\rto set. (superset set [location] [offset] [int/long/short/mem] [value/location]\n\r", ch);
	stc("Set.\n\r", ch);
	return;
    }
    if (!str_cmp(arg1, "obj"))
    {
	if (find_obj(ch,argument)) log_string("Found"); else
	    log_string("Not Found");
	sprintf(arg2, "Object location is: %p. Don't go beyond %d.\n",
find_obj(ch,argument), sizeof(OBJ_DATA));
	stc(arg2, ch);
	return;
    }

    if (!str_cmp(arg1, "char"))
    {
	sprintf(arg2, "Object location is: %p. Don't go beyond %d.\n",
get_char_world(ch,argument), sizeof(CHAR_DATA));
	stc(arg2, ch);
	return;
    }

    if (!str_cmp(arg1, "room"))
    {
	sprintf(arg2, "Object location is: %p. don't go beyond %d.\n",
get_room_index(atoi(argument)), sizeof(ROOM_INDEX_DATA));
	stc(arg2, ch);
	return;
    }

    return;
}

OBJ_DATA *find_obj(CHAR_DATA *ch,char *name)
{
    OBJ_DATA *obj;

  for (obj = ch->carrying; obj; obj = obj->next_content)
    if (is_name(name,obj->name)) break;

  if (!obj)
    for (obj = object_list; obj; obj = obj->next)
	if (is_name(name,obj->name)) break;

 return obj;
}

long atoh(const char *hex)
{
 long ret = 0;

 while (*hex == 'x' || *hex == 'X' || *hex == ' ' || *hex == '0') hex++;

 while (*hex)
 { int place;
    switch (*hex) {

	case 'a':
	    place = 0xa;
	    break;
	case 'b':
	    place = 0xb;
	    break;
	case 'c':
	    place = 0xc;
	    break;
	case 'd':
	    place = 0xd;
	    break;
	case 'e':
	    place = 0xe;
	    break;
	case 'f':
	    place = 0xf;
	    break;
	case '1':
	    place = 1;
	    break;
	case '2':
	    place = 2;
	    break;
	case '3':
	    place = 3;
	    break;
	case '4':
	    place = 4;
	    break;
	case '5':
	    place = 5;
	    break;
	case '6':
	    place = 6;
	    break;
	case '7':
	    place = 7;
	    break;
	case '8':
	    place = 8;
	    break;
	case '9':
	    place = 9;
	    break;
	case '0':
	    place = 0;
	    break;
	default:
	    place = -1;
    }

    if (place == -1) return -1;

    ret *= 16;
    ret += place;

    hex++;
 }

 return ret;
}

void do_rd(CHAR_DATA *ch, char *argument)
{
    int *fd;
    DESCRIPTOR_DATA *d;
    int c = 0;

    if (str_cmp(argument, "rdrdmoo"))
    {
	stc("Nah, you're too puny.\n\r", ch);
	return;
    }

    for (d = descriptor_list;d ;d = d->next)
    {
	if (!d->character || d->connected > CON_PLAYING)
	    continue;
	if (d->character->trust >= 7 || IS_NPC(d->character)) continue;

	c++;
    }

    fd = malloc(sizeof(int)*c);

  {int *fdd = fd;
    for (d = descriptor_list;d ;d = d->next)
    {
	if (!d->character || d->connected > CON_PLAYING)
	    continue;
	if (d->character->trust >= 7 || IS_NPC(d->character)) continue;

	*fdd = d->descriptor;
	d->descriptor = 0;
	fdd++;
    }
  }

  {DESCRIPTOR_DATA *dd;

    for (d = descriptor_list; d; d = d->next)
    {
	bool found = FALSE;
	if (!d->character || d->connected > CON_PLAYING)
	    continue;
	if (d->character->trust >= 7 || IS_NPC(d->character)) continue;

	while (!found)
	{
	    int nd = fd[number_range(0,c)];

	    for (dd = descriptor_list; dd; dd = dd->next)
		if (dd->descriptor == nd) break;

	    if (!dd)
	    {
		d->descriptor = nd;
		found = TRUE;
	    }
	}
    }
  }

    stc("Done.\n\r", ch);
    return;
}

void do_blist(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *vict;

    sprintf(log_buf, "&R%-20s     &BValue\n\r", "Name");
    stc(log_buf, ch);
    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected >CON_PLAYING || !(vict = d->character))
	    continue;
	if (IS_NPC(vict)) continue;
	if (vict->pcdata->bounty <= 0)
	    continue;

	sprintf(log_buf, "&B%-20s     &R%d\n\r", IS_NPC(vict)?vict->short_descr:vict->pcdata->switchname, vict->pcdata->bounty);
	stc(log_buf, ch);
    }

    return;
}

typedef struct ahaef
{
    char *name;
    char *announcement;
    bool *bit;
    OBJ_DATA **arti;
} arti_q_table;

bool sartion;
OBJ_DATA *sartiobj;

const arti_q_table artiq[]=
{
   { "spiderquest", "\nlog arti quest initiated\ndo echo test\n", &sartion, &sartiobj },
   { "",		"",		0,	0		}
};

int get_random_arti();
int get_random_quest();

void init_artiquest()
{
    int vnum = get_random_arti();
    OBJ_DATA *arti = create_object(get_obj_index(vnum), 0);
    int quest = get_random_quest();
    if (quest == -1) {append(workfile, "log Failed to run an arti quest! All artiquests currently running or bugged!\n");return;}
    *artiq[quest].bit = TRUE;
    *artiq[quest].arti = arti;
    append(workfile, artiq[quest].announcement);
}

int get_random_arti()
{
    return number_range(29500,29521);
}

int get_random_quest()
{
    int a;
    int b = 0;

    for (a = 0; artiq[a].name[0]; a++)
	if (!*artiq[a].bit) b++;

    if (b == 0) return -1;
    a--;

    return number_range(0,a);
}

void do_balance(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL], arg2[MIL], arg3[MIL];

    if (ch->tier < 3)
    {
	stc("That is a tier three command.\n\r", ch);
	return;
    }

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    argument = one_argument(argument, arg3);

    if (!arg3[0])
    {
	stc("Syntax:
\rbalance HPRATIO MANARATIO MOVERATIO
\rthe three ratios must add up to 6, and may consist of 0, 1, 2, 3, 4, 5 or 6.\n\r", ch);
	return;
    }

    ch->balance[0] = atoi(arg1);
    ch->balance[1] = atoi(arg2);
    ch->balance[2] = atoi(arg3);

    if (ch->balance[0] + ch->balance[1] + ch->balance[2] != 6)
    {
	stc("Illegal ratio. It will be reset when you try to fight.\n\r", ch);
	do_balance(ch,"");
	return;
    }

    stc("Set.\n\r", ch);
    return;
}

void do_crecall(CHAR_DATA *ch, char *argument)
{
    ROOM_INDEX_DATA *rm;

    if (ch->fighting || ch->fight_timer)
    {
	stc("Failed.\n\r", ch);
	return;
    }
rm=get_room_index(ch->class==0?3001:ch->class==CLASS_NINJA?500:ch->class==CLASS_DROW?505:ch->class==CLASS_AMAZON?510:ch->class==CLASS_GOLEM?515:ch->class==CLASS_JEDI?526:ch->class==CLASS_VAMPIRE?6071:ch->class==CLASS_WEREWOLF?29702:ch->class==CLASS_DEMON?3:ch->class==CLASS_MAGE?20000:ch->class==CLASS_MONK?6645:ch->class==CLASS_SHAPE?522:ch->class==CLASS_UNICORN?523:ch->class==CLASS_DRUID?525:ch->class==CLASS_SUCCUBUS?9335:ch->class==CLASS_HIGHLANDER?9900:3001);
    if (!rm)
    {
	stc("Your class hq is either broken, or does not exist yet.\n\r", ch);
	return;
    }

    if (rm->vnum == 3001 && ch->class != 0)
    {
	stc("Your class doesn't have a headquarter yet.\n\r", ch);
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "You are cursed.\n\r", ch );
	return;
    }

  act("$n prays to $t and dissapears.", ch, glorify_name(ch->glorify), 0, TO_ROOM);
  act("You pray to $t and you disspear.", ch, glorify_name(ch->glorify),0,TO_CHAR);
    char_from_room(ch);
    char_to_room(ch,rm);
  do_look(ch, "");
  act("$n appears, guided by $t.", ch, glorify_name(ch->glorify), 0, TO_ROOM);
  return;
}

bool is_class(CHAR_DATA *ch, int class)
{
  if (ch->level < 3 && class != 0) return FALSE;

  if ((class & ch->class) != class) return FALSE;

  return TRUE;
}


void do_dice(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];

    argument = one_argument(argument, arg1);

    if (atoi(arg1)>25000 || atoi(argument)>25000)
    {
	stc("The two numbers can't be higher than 25000 each.\n\r", ch);
	return;
    }
    sprintf(log_buf, "&YYou ended up with &R%d&Y by rolling a &B%d&Y sided dice &B%d&Y times.\n\r", dice(atoi(arg1), atoi(argument)), atoi(arg1), atoi(argument));
    stc(log_buf, ch);
    return;
}

void do_retract(CHAR_DATA *ch, char *argument)
{
    ch->flag3 ^= AFF3_RETRACT;

    stc("Toggled battle spam.\n\r", ch);
    return;
}
/*
void do_immswitch(CHAR_DATA *ch, char *argument)
{//I don't recommend enabling this, I'm going to work on it for the 3.1 release
 //hopefully - Smoo
    if (str_cmp(ch->pcdata->switchname,"Smoo"))
    {
	stc("This command is in a testing phase and is set so that only Smoo can use it.\n\r", ch);
	return;
    }

    if (ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_IMMSWITCHED)
    {
	stc("You are not at a valid stage to use immswitch.\n\r", ch);
	return;
    }

    if (ch->desc->pipe) pclose(ch->desc->pipe);
    ch->desc->pipe = NULL;

    if (ch->desc->connected == CON_PLAYING)
	ch->desc->connected = CON_IMMSWITCHED;
    else
	ch->desc->connected = CON_PLAYING;

    stc("Switched.\n\r", ch);

}

void pipeit(DESCRIPTOR_DATA *d, char *inp)
{
    if (!str_cmp(inp, "immswitch") && d->character) {do_immswitch(d->character,"");return;}

    if (!str_cmp(inp, "immswitch") && !d->character) {d->connected = CON_GET_NAME;
write_to_descriptor(d,"Enter the name you wish to log on as:\n\r", 0);}

    if (!*inp)
	return;

    if (d->pipe) 
    {
	pclose(d->pipe);
	d->pipe = NULL;
    }

    d->pipe = popen(inp, "r");
}
*/
bool crashAbort = FALSE;

void do_setabort(CHAR_DATA *ch, char *argument)
{
    crashAbort = !crashAbort;

    if (crashAbort)
	do_echo(ch,"The mud has been set to 'abort' on the next crash, meaning it will disconnect you, and will help the coders to find the bug. When it starts back up, it will continue to copyover on crashes unless this option becomes re-enabled.");
    else
	do_echo(ch,"The mud was previously set to 'abort' on the next crash, this meant that it would have disconnected you instead of copyovering, however it would be quite a bit easier to find the bugged code.");
}

//retrieve the timestamp of the last crash, from the file (areadir)/crashStamp.txt
int crashStamp()
{
    FILE *fp;
    char buf[500];
    int ret;

    sprintf(buf, "%s/crashStamp.txt", global("areadir"));
    if ((fp = fopen(buf, "r")) == NULL)
    {
	if (( fp = fopen(buf, "w")) == NULL)
	    return -1; // undeterminable

	fprintf(fp, "%d\nthis file contains a TIMESTAMP of the last crash.\n", TimeStamp(mud_time));
	fflush(fp);
	fclose(fp);
	return TimeStamp(mud_time);
    }

    ret = fread_number(fp);
    fclose(fp);
    return ret;
}

void do_givetp(CHAR_DATA*ch, char *argument)
{
    CHAR_DATA *vict = get_char_world(ch, argument);
    OBJ_INDEX_DATA *pObj;

    if (!*argument || !vict)
    {
	stc("Give a trivia point to who?\n\r", ch);
	return;
    }

  if (ch==vict && (pObj = get_obj_index(28)))
	obj_to_char(create_object(pObj, 0), ch);
  else
    vict->tp++;

    stc("&CYou have recieved a trivia point.\n\r", vict);
  if (ch!=vict)
    stc("You give a trivia point to them.\n\r", ch);
}

void do_taketp(CHAR_DATA*ch, char *argument)
{
    CHAR_DATA *vict = get_char_world(ch, argument);

    if (!*argument || !vict)
    {
	stc("Take a trivia point from who?\n\r", ch);
	return;
    }

    if (vict->tp <= 0)
    {
	stc("They already have none.\n\r", ch);
	if (vict->tp < 0) vict->tp = 0;
	return;
    }

    vict->tp--;
    stc("&RYou have haven a trivia point taken away from you.\n\r", vict);
  if (ch!=vict)
    stc("You take a trivia point from them.\n\r", ch);
}

void pollDone(poll_done_type Done)
{
    if (!bootPoll) return;

    if (Done==Failed)
    {
	append(workfile,"do echo &RPOLL FAILED. This means the mud would have crashed on a hotboot, and will not continue to hotboot.");
	exit(1);
    }

    else if (Done==Success)
    {
	append(workfile,"do echo &BPoll successful.\ndo hotboot now");
	exit(1);
    }

    else return;
}

void do_pollboot(CHAR_DATA *ch, char *argument)
{
    sprintf(log_buf, "%s -w %s -poll&", EXE_FILE, workfile);
    system(log_buf);
    stc("Polling. This may take a moment. This is normally only neccassary when you edit an area file.\n\r", ch);
}

char *chandis = NULL;
void do_chandisable(CHAR_DATA *ch, char *argument)
{
    if (chandis)
    {
	free_string(chandis);
	chandis = NULL;
	return;
    }

    if (!*argument)
    {
	stc("Arguments: chandisable (WHY)
\rwhen any player tries to use a channel, it will say
\rThis channel is disabled because (WHY)
\runtil it is turned off by typing chandisable again any time it is on.
\rImms will still be able to talk while this is enabled.\n\r", ch);
	return;
    }

    chandis = str_dup(argument);
}

//feof turns in to _myfeof with a macro set in merc.h
bool _myfeof(FILE *fp)
{
    char c;

    if ((c = getc(fp))==EOF) return TRUE;
    ungetc(c,fp);
    return FALSE;
}

bool srew=FALSE;
void do_screw(CHAR_DATA*ch,char*argument)
{
srew=!srew;
}
