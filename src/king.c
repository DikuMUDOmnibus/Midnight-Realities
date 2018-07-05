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

/* Original kingdom code by...Smoo */
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

#define PkRatio(pk,pd) (pd==0?100:pk==0?0:(pk*100)/(pk+pd))
bool isnumber(const char*);

char DefaultKingdomNames[10][2][50] =
{
    {"King","Queen"},
    {"Prince","Princess"},
    {"Visier","Visier"},
    {"Knight","Temptress"},
    {"Councelor","Councless"},
    {"Chairman","Chairfem"},
    {"Bladesmith","Blade-worker"},
    {"Condemer","Condemer"},
    {"Tradesman","Tradeslady"},
    {"Peasant","Peasant"}
};

void do_kingedit(CHAR_DATA *ch, char *argument)
{
    KINGDOM_DATA nk;
    KINGDOM_DATA *pnk = NULL, *pnk_bef;
    char arg1[MIL];
    int a, b;

    if (IS_NPC(ch)) return;

    if (!*argument)
    {
	stc("Please enter create or delete followed by the kingdom name.\n\r", ch);
	stc("Or, set (kingdom name in quotes) king/queen []\n\r", ch);
	stc("Or, set (kingdom name in quotes) Gateway [vnum of the room to set as the gateway]\n\r", ch);
	return;
    }

    argument = one_argument(argument, arg1);

    if (!str_cmp(arg1, "set"))
    {int k = 0;
	argument = one_argument(argument, arg1);

	if ((pnk = find_kingdom(arg1)) == NULL)
	{
	    stc("That kingdom doesn't appear to exist.\n\r", ch);
	    return;
	}

	argument = one_argument(argument, arg1);
	if (!str_cmp(arg1, "gateway"))
	{
	    if (!*argument)
	    {
		stc("Set the clans Gateway room vnum to what?\n\r", ch);
		return;
	    }
	    pnk->Gateway = atoi(argument);
	    stc("Gateway set.\n\r", ch);
	    save_kingdoms();
	    return;
	}

	if (!str_cmp(arg1,"king"))
	    k = 1;
	else if (!str_cmp(arg1,"queen"))
	    k = 2;

	if (k == 0 || k > 2)
	{
	    stc("Currently, the only supported options are set king and set queen.\n\r", ch);
	    return;
	}

	if (!*argument)
	{
	    sprintf(log_buf, "set %s who?\n\r", k==1?"king":k==2?"queen":"error");
	    stc(log_buf, ch);
	    return;
	}

	free_string(k==1?pnk->King:pnk->Queen);
	(k==1?pnk->King:pnk->Queen) = str_dup(argument);
	sprintf(log_buf, "&G%s&G has a new %s&G! All hail %s&G %s&G!",
	    pnk->Name,k==1?pnk->Rank[0][0]:pnk->Rank[0][1],
	    k==1?pnk->Rank[0][0]:pnk->Rank[0][1], k==1?pnk->King:pnk->Queen);
	king_info(log_buf);
       {CHAR_DATA *v;
	if ((v=get_char_world(ch,k==1?pnk->King:pnk->Queen)) != NULL) v->krank = 0;}
	save_kingdoms();
	return;
    }
    if (!str_cmp(arg1, "create"))
    {
	if (!*argument)
	{
	    stc("Please enter the name of the kingdom to create.\n\r", ch);
	    return;
	}

	nk.Name = str_dup(argument);
	nk.King = str_dup(ch->pcdata->switchname);
	nk.Queen = str_dup(ch->pcdata->switchname);
	nk.Members = 1;
	nk.Pkills = 0;
	nk.Pdeaths = 0;
	nk.Humiliations = 0;
	nk.Pot = 0;
	nk.Skills = str_dup("");

	for (a = 0; a < 10; a++)
	    for (b = 0; b < 2; b++)
		nk.Rank[a][b] = str_dup(DefaultKingdomNames[a][b]);

	pnk = malloc(sizeof(KINGDOM_DATA));
	*pnk = nk;
	pnk->next = king_list;
	king_list = pnk;
	stc("Kingdom created.\n\r", ch);
	sprintf(log_buf, "&GThe kingdom '&Y%s&G' has been created.",
	    nk.Name);
	king_info(log_buf);
	save_kingdoms();
	free_string(ch->kingdom);
	ch->kingdom = str_dup(pnk->Name);
	return;
    }

    if (!str_cmp(arg1, "delete"))
    {
	char bsh[300];
	strcpy(bsh, bash_color(argument));
	for (pnk = king_list, pnk_bef=0; pnk; pnk_bef = pnk, pnk = pnk->next)
	    if (!str_cmp(bsh,bash_color(pnk->Name)))
		break;

	if (!pnk)
	{
	    stc("That kingdom doesn't exist.\n\r", ch);
	    return;
	}

	if (!pnk_bef) king_list = pnk->next;

	else pnk_bef->next = pnk->next;

	sprintf(log_buf, "&GThe '&Y%s&G' kingdom has been removed from the game.", pnk->Name);
	king_info(log_buf);

	free_string(pnk->Name);
	free_string(pnk->King);
	free_string(pnk->Queen);
	free_string(pnk->Skills);
	for (a = 0; a < 10; a++)
	    for (b = 0; b < 2; b++)
		free_string(pnk->Rank[a][b]);

	free(pnk);
	stc("Kingdom deleted.\n\r", ch);
	save_kingdoms();
	return;
    }

    do_kingedit(ch,"");
    return;
}

void do_klist(CHAR_DATA *ch, char *argument)
{
    KINGDOM_DATA *kp;
    int namelen = 24;

    sprintf(log_buf, "&R%-*s&B| &R%15s&B |&R %15s &B|&R Pk Ratio\n\r",
	namelen,"Name","King","Queen");
    stc(log_buf, ch);

    for (kp = king_list; kp; kp = kp->next)
    {
	sprintf(log_buf, "&B%-*s&R|&B %15s &R|&B %15s&R |&B %7d&x\n\r",
	    namelen+(strlen(kp->Name)-strlen(bash_color(kp->Name))),
	    kp->Name, kp->King, kp->Queen,
	    PkRatio(kp->Pkills,kp->Pdeaths));
	stc(log_buf, ch);
    }

    return;
}

void king_info(char *text)
{
    char *buf;
    char *pre = "&RKingdom Info &Y-&G> &C";
    DESCRIPTOR_DATA *d;

    buf = malloc(strlen(pre)+5+strlen(text));
    strcpy(buf, pre);
    strcat(buf, text);
    strcat(buf, "&x\n\r");
    for (d = descriptor_list; d; d = d->next)
    {
	if (!d->character || d->connected > CON_PLAYING)
	    continue;

	stc(buf, d->character);
    }

    free (buf);
}

KINGDOM_DATA *find_kingdom(const char *name)
{
    KINGDOM_DATA *pkd;
    char bsh[500];
				// it was extremely annoying to have
				// to type out the - [    ] -
if (!str_cmp(name,"asb")) return find_kingdom("- [ a s b ] -");

if (!name) return NULL;

if (!*name) return NULL;

    strcpy(bsh, bash_color(name));
    for (pkd = king_list; pkd; pkd = pkd->next)
	if (!str_cmp(bsh, bash_color(pkd->Name)))
	    break;

    return pkd;
}

bool kingdom_eof = FALSE;

int kingdom_eof_handler(FILE *fp, int action);
int kingdom_eof_handler(FILE *fp, int action)
{
    if (action == FEOF_FEOF) kingdom_eof = TRUE;

    return 1;
}

void load_kingdoms()
{
    KINGDOM_DATA nk;
    KINGDOM_DATA *pnk;
    FILE *fp;
    char c;
    int version, a, b;

    if ((fp = fopen(global("kingdom_datafile"), "r")) == NULL)
    {
	king_list = NULL;
	save_kingdoms();
	log_string("load_kingdoms: Kingdom datafile not found..Creating.");
	load_kingdoms();
	return;
    }

    king_list = NULL;
    kingdom_eof = FALSE;

    feof_handler = kingdom_eof_handler;
    version = fread_number(fp);
    if (kingdom_eof)
    {
	feof_handler = NULL;
	return;
    }

    switch (version)
    {
      case 1:
	while ((c = getc(fp)) != EOF)
	{
	    ungetc(c,fp);
	    nk.Name = fread_string(fp);
	    nk.King = fread_string(fp);
	    nk.Queen = fread_string(fp);
	    nk.Pot = fread_number(fp);
	    nk.Members = fread_number(fp);
	    nk.Pkills = fread_number(fp);
	    nk.Pdeaths = fread_number(fp);
	    nk.Humiliations = fread_number(fp);
	    for (a = 0; a < 10; a++)
		for (b = 0; b < 2; b++)
		    nk.Rank[a][b] = fread_string(fp);
	    pnk = malloc(sizeof(KINGDOM_DATA));
	    *pnk = nk;
	    pnk->next = king_list;
	    king_list = pnk;
	    while ((c = getc(fp)) == '\n' || c == '\r' || c == ' ') ;
		ungetc(c,fp);
	}
	break;
      case 2:
	while ((c = getc(fp)) != EOF)
	{
	    ungetc(c,fp);
	    nk.Name = fread_string(fp);
	    nk.King = fread_string(fp);
	    nk.Queen = fread_string(fp);
	    nk.Pot = fread_number(fp);
	    nk.Members = fread_number(fp);
	    nk.Pkills = fread_number(fp);
	    nk.Pdeaths = fread_number(fp);
	    nk.Humiliations = fread_number(fp);
	    nk.Gateway = fread_number(fp);
	    nk.Skills = str_dup("");
	    for (a = 0; a < 10; a++)
		for (b = 0; b < 2; b++)
		    nk.Rank[a][b] = fread_string(fp);
	    pnk = malloc(sizeof(KINGDOM_DATA));
	    *pnk = nk;
	    pnk->next = king_list;
	    king_list = pnk;
	    while ((c = getc(fp)) == '\n' || c == '\r' || c == ' ') ;
		ungetc(c,fp);
	}
	break;
      case 3: default:
	while ((c = getc(fp)) == ' ' || c == '\n' || c == '\r') ;
	if (c!=EOF)
	    ungetc(c,fp);
	while ((c = getc(fp)) != EOF)
	{
	    ungetc(c,fp);
	    nk.Name = fread_string(fp);
	    nk.King = fread_string(fp);
	    nk.Queen = fread_string(fp);
	    nk.Pot = fread_number(fp);
	    nk.Members = fread_number(fp);
	    nk.Pkills = fread_number(fp);
	    nk.Pdeaths = fread_number(fp);
	    nk.Humiliations = fread_number(fp);
	    nk.Gateway = fread_number(fp);
	    for (a = 0; a < 10; a++)
		for (b = 0; b < 2; b++)
		    nk.Rank[a][b] = fread_string(fp);
	    nk.Skills = fread_string(fp);
	    pnk = malloc(sizeof(KINGDOM_DATA));
	    *pnk = nk;
	    pnk->next = king_list;
	    king_list = pnk;
	    while ((c = getc(fp)) == '\n' || c == '\r' || c == ' ') ;
		ungetc(c,fp);
	}
	break;
    }

    feof_handler = NULL;
    fclose(fp);
    return;
}

void save_kingdoms()
{
    FILE *fp;
    int a, b;
    KINGDOM_DATA *pk;

    if ((fp = fopen(global("kingdom_datafile"), "w")) == NULL)
    {
	log_string("BUG! Unable to write to the kingdom data file!");
	log_string("Aborting the saving of kingdoms.");
	return;
    }

    fprintf(fp, "3\n"); // version

    for (pk = king_list; pk; pk = pk->next)
    {
	fprintf(fp,"%s~\n%s~\n%s~\n%d\n%d\n%d %d\n%d\n%d\n",pk->Name,
	    pk->King, pk->Queen, pk->Pot, pk->Members, pk->Pkills,
	    pk->Pdeaths, pk->Humiliations, pk->Gateway);

	for (a = 0; a < 10; a++)
	    for (b = 0; b < 2; b++)
		fprintf(fp, "%s~\n", pk->Rank[a][b]);
	fprintf(fp, "%s~\n", pk->Skills);
    }

    fflush(fp);
    fclose(fp);
    return;
}

void check_pkpd(CHAR_DATA *ch, int w)
{
    KINGDOM_DATA *k;
    if ((k = find_kingdom(ch->kingdom)) == NULL)
	return;

    if (w == CHECK_PK)
	k->Pkills++;
    else if (w == CHECK_PD)
	k->Pdeaths++;
    else
	return;
}

void do_introduce(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict = get_char_room(ch,argument);
    KINGDOM_DATA *k = find_kingdom(ch->kingdom);

    if (!k)
    {
	stc("You aren't in a kingdom!\n\r", ch);
	return;
    }

    if (str_cmp(k->King, ch->pcdata->switchname) &&
	str_cmp(k->Queen, ch->pcdata->switchname))
    {
	stc("You are not the King or Queen!\n\r", ch);
	return;
    }

    if (ch->in_room->vnum != k->Gateway)
    {
	stc("You need to be at the gateway to your kingdom for that!\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (!vict)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (find_kingdom(vict->kingdom) == find_kingdom(ch->kingdom))
    {
	stc("They are already in your kingdom.\n\r", ch);
	return;
    }

    if (find_kingdom(vict->kingdom))
    {
	stc("They are already in a kingdom\n\r", ch);
	stc("Ouch that hurt!\n\r", ch);
	ch->hit = -10;
	update_pos(ch);
	return;
    }

    free_string(vict->kingdom);
    vict->kingdom = str_dup(ch->kingdom);
    sprintf(log_buf, "&Y%s&G is now a member of &B%s&Y!!",
	vict->pcdata->switchname,	find_kingdom(ch->kingdom)->Name);
    king_info(log_buf);
    vict->krank = 9;
    find_kingdom(ch->kingdom)->Members++;

    act("$n inducts you in to $t.", ch, ch->kingdom, vict, TO_VICT);
    act("You induct $N in to $t.", ch, ch->kingdom, vict, TO_CHAR);
    act("$n inducts $N in to $t.", ch, ch->kingdom, vict, TO_NOTVICT);

    return;
}

void do_banish(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict = get_char_room(ch,argument);
    KINGDOM_DATA *k = find_kingdom(ch->kingdom);

    if (!k)
    {
	stc("You aren't in a kingdom!\n\r", ch);
	return;
    }

    if (str_cmp(k->King, ch->pcdata->switchname) &&
	str_cmp(k->Queen, ch->pcdata->switchname))
    {
	stc("You are not the King or Queen!\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (!vict)
    {
	stc("They aren't here!\n\r", ch);
	return;
    }

    if (find_kingdom(vict->kingdom) != find_kingdom(ch->kingdom))
    {
	stc("They are not in your kingdom!\n\r", ch);
	stc("Ouch that hurt!\n\r", ch);
	ch->hit = -10;
	update_pos(ch);
	return;
    }

    find_kingdom(ch->kingdom)->Members--;
    sprintf(log_buf, "&c%s&R has been outcasted from &B%s&Y!!",
	vict->pcdata->switchname,	find_kingdom(ch->kingdom)->Name);
    free_string(vict->kingdom);
    vict->kingdom = str_dup("");
    king_info(log_buf);

    act("$n outcasts you from $t.", ch, k->Name, vict, TO_VICT);
    act("You outcast $N from $t.", ch, k->Name, vict, TO_CHAR);
    act("$n outcasts $N from $t.", ch, k->Name, vict, TO_NOTVICT);

    return;
}

void do_druggies(CHAR_DATA *ch, char *argument)
{
    if (str_cmp(bash_color(ch->kingdom), "- [ a s b ] -") && ch->level < 7
	&& !IS_NPC(ch))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    talk_channel(ch, argument, CHANNEL_KINGDOM, "- [ a s b ] -");
}

void do_spiritlords(CHAR_DATA *ch, char *argument)
{
    if (str_cmp(bash_color(ch->kingdom), "spirit lords") && ch->level < 7
	&& !IS_NPC(ch))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    talk_channel(ch, argument, CHANNEL_KINGDOM, "spirit lords");
}

void do_eternity(CHAR_DATA *ch, char *argument)
{
    if (str_cmp(bash_color(ch->kingdom), "eternity") && ch->level < 7
	&& !IS_NPC(ch))
    {
	stc("Huh?\n\r", ch);
	return;
    }

    talk_channel(ch, argument, CHANNEL_KINGDOM, "eternity");
}

void do_setrank(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MIL];

    argument = one_argument(argument, arg);
    if (arg[0] == '\0' || !*argument)
    {
	stc("Set whos rank to what?
\rformat: setrank player number\n\r", ch);
	return;
    }

    if ((victim = get_char_world(ch,arg)) == NULL)
    {
	stc("They aren't here!\n\r", ch);
	do_setrank(ch,"");
    }

    if (IS_NPC(victim))
    {
	stc("Why would you do that on a mob!\n\r", ch);
	return;
    }

    victim->krank = atoi(argument);
    stc("Set.\n\r", ch);
    return;
}

void do_kcontrol(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    int a;
    KINGDOM_DATA *k;

    argument = one_argument(argument, arg1);

    if (!(k=find_kingdom(ch->kingdom)) || IS_NPC(ch) || ch->krank != 0)
    {
	stc("You are not the king or queen of a kingdom.\n\r", ch);
	return;
    }

    if (!arg1[0])
    {
	stc("Set what control variables of your kingdom?\n\r", ch);
	stc("kcontrol rankname (rank name) (new rank name)\n\r", ch);
	stc("kcontrol promote (player)\n\r", ch);
	stc("kcontrol demote (player)\n\r", ch);
	return;
    }

    if (!str_cmp(arg1, "rankname"))
    {
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	int b;

	argument = one_argument(argument, arg2);

	if (!arg2[0] || !argument || !*argument)
	{
	    stc("format: kcontrol rankname 'old rank name' 'new rank name'\n\r", ch);
	    return;
	}

	if (*argument == '\'')
	    argument++;

	strcpy(arg3, argument);

	if (*(argument+strlen(argument)-1)== '\'')
	    arg3[strlen(argument)-1] = '\0';

	strcpy(arg2, bash_color(arg2));

	for (a = 0;a < 10; a++)
	{
	    for (b = 0; b < 2; b++)
	    {
		if (!str_cmp(bash_color(k->Rank[a][b]), arg2))
		    break;
	    }
	    if (b < 2) break;
	}

	if (a == 10)
	{
	    sprintf(log_buf, "%s doesn't exist as a kingdom rank name for your kingdom.\n\r", arg2);
	    stc(log_buf, ch);
	    return;
	}

	if (strlen(bash_color(arg3)) > 30)
	{
	    stc("Too long=(\n\r", ch);
	    return;
	}

	sprintf(log_buf, "Changing &s%s&t to &s%s&t.\n\r", k->Rank[a][b],arg3);
	stc(log_buf, ch);

	free_string(k->Rank[a][b]);
	k->Rank[a][b] = str_dup(arg3);
	stc("Set.\n\r", ch);
	save_kingdoms();
	return;
    }
    if (!str_cmp(arg1, "promote"))
    {
	if ((victim = get_char_room(ch, argument)) == NULL)
	{
	    stc("They aren't here.\n\r", ch);
	    return;
	}

	if (find_kingdom(victim->kingdom) != k)
	{
	    stc("They aren't in your kingdom.\n\r", ch);
	    return;
	}

	if (victim->krank <= 1)
	{
	    stc("They are too high ranked to be promoted.\n\r", ch);
	    return;
	}

	victim->krank--;

	sprintf(log_buf, "&W%s&C has taken the rank of &M%s&C in &R%s&C.",
	    IS_NPC(victim)?victim->short_descr:victim->pcdata->switchname,
	    k->Rank[victim->krank][victim->sex==2?1:victim->sex==1?0:0],
	    k->Name);
	king_info(log_buf);
	stc("Ok!\n\r", ch);
	return;
    }

    if (!str_cmp(arg1, "demote"))
    {
	if ((victim = get_char_room(ch, argument)) == NULL)
	{
	    stc("They aren't here.\n\r", ch);
	    return;
	}

	if (find_kingdom(victim->kingdom) != k)
	{
	    stc("They aren't in your kingdom.\n\r", ch);
	    return;
	}

	if (victim->krank >= 9)
	{
	    stc("They are already at the lowest rank possible!\n\r", ch);
	    return;
	}

	victim->krank++;

	sprintf(log_buf, "&W%s&C has been demoted to the rank of &M%s&C in &R%s&C.",
	    IS_NPC(victim)?victim->short_descr:victim->pcdata->switchname,
	    k->Rank[victim->krank][victim->sex==2?1:victim->sex==1?0:0],
	    k->Name);
	king_info(log_buf);
	stc("Ok!\n\r", ch);
	return;
    }

    do_kcontrol(ch,"");
    return;
}

void do_kstat(CHAR_DATA *ch, char *argument)
{
    int a;
    KINGDOM_DATA *k;

    if ((k = find_kingdom(argument)) == NULL)
    {
	stc("That kingdom doesn't exist. Try the klist command.\n\r", ch);
	return;
    }

sprintf(log_buf,
"&C[&g%*s&C] &c-------------------------------------------------------------\n\r", 14+(strlen(k->Name)-strlen(bash_color(k->Name))),
k->Name); stc(log_buf, ch); 
sprintf(log_buf,
"----------------- &s&RKing&t ------------------------ &BQueen&t -----------------------\n\r"); stc(log_buf, ch); 
sprintf(log_buf,
"-------------&B%s&t-----------------&R%s&t--------------------\n\r",centerjob(" ",k->King,14-strlen(k->King)),
centerjob(" ",k->Queen,13-strlen(k->Queen))); stc(log_buf, ch); 
sprintf(log_buf, 
"-----------------------------------------------------------------------------\n\r"); stc(log_buf, ch); 
sprintf(log_buf,
"----- &YMale Ranks&t  ------- &GFemale Ranks&t ----- &RPks&t ---- &RPds&T --- &BHumiliations&t --\n\r"); stc(log_buf, ch); 
sprintf(log_buf,
"----&G%s&t----&Y%s&t----&B%-7d&t--&B%-7d&t--&G%-14d&t--\n\r", centerjob(" ",
k->Rank[0][0], 15-(strlen(k->Rank[0][0])-(strlen(k->Rank[0][0])-strlen(bash_color(k->Rank[0][0]))))),centerjob(" ",
k->Rank[0][1], 16-(strlen(k->Rank[0][1])-(strlen(k->Rank[0][1])-strlen(bash_color(k->Rank[0][1]))))),
k->Pkills, k->Pdeaths, k->Humiliations);
stc(log_buf, ch);
for (a = 1; a < 10; a++)
{
sprintf(log_buf,
"----&G%s&t----&Y%s&t--------------------------------------\n\r", centerjob(" ",
k->Rank[a][0], 15-(strlen(k->Rank[a][0])-(strlen(k->Rank[a][0])-strlen(bash_color(k->Rank[a][0]))))),centerjob(" ",
k->Rank[a][1], 16-(strlen(k->Rank[a][1])-(strlen(k->Rank[a][1])-strlen(bash_color(k->Rank[a][1]))))));
stc(log_buf, ch);
}
sprintf(log_buf,
"-----------------------------------------------------------------------------\n\r"); stc(log_buf, ch); 
sprintf(log_buf,
"--- &RKingdom Skills:&m%-56s&t--\n\r",k->Skills); stc(log_buf, ch); 
sprintf(log_buf,
"-----------------------------------------------------------------------------\n\r"); stc(log_buf, ch);

return;
}

//dark legion kingdom power
void do_kgate(CHAR_DATA *ch, char *argument)
{
    KINGDOM_DATA *k;
    CHAR_DATA *vict;

    if ((k = find_kingdom(ch->kingdom)) == NULL)
    {
	stc("You aren't in a kingdom.\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("Kingdom gate to who?\n\r", ch);
	return;
    }

    if ((vict = get_char_world(ch,argument)) == NULL)
    {
	stc("They aren't online.\n\r", ch);
	return;
    }

    if (k != find_kingdom(vict->kingdom))
    {
	stc("They aren't in your kingdom.\n\r", ch);
	return;
    }

    if (ch->in_room == vict->in_room)
    {
	stc("You are already there!\n\r", ch);
	return;
    }

    char_from_room(ch);
    char_to_room(ch,vict->in_room);
    do_look(ch,"");
    return;
}
/*
void do_humiliate(CHAR_DATA *ch, char *argument)
{
    KINGDOM_DATA *k = find_kingdom(ch->kingdom);
    CHAR_DATA *vict;
    KINGDOM_DATA *k2;

    if (!k)
    {
	stc("You aren't in a kingdom=(\n\r", ch);
	return;
    }

    if (!*argument)
    {
	stc("Humiliate who?\n\r", ch);
	return;
    }

    if ((vict = get_char_room(ch, argument)) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }

    if (IS_NPC(ch))
    {
	stc("Not on monsters!\n\r", ch);
	return;
    }

    if ((k2 = find_kingdom(vict->kingdom)) == NULL)
    {
	stc("They aren't in a kingdom.\n\r", ch);
	return;
    }

    
    if (k2 == k)
    {
	stc("Okay, fine. Humiliate your own kingdom! HEY! That was reverse psychology retard!\n\r", ch);
	return;
    }

    if (vict->hit > 0)
    {
	stc("They must be mortally wounded=(\n\r", ch);
	return;
    }

    k->
*/

void guard_check()
{
    CHAR_DATA *vch;
    KINGDOM_DATA *k;

    for (vch = char_list; vch; vch = vch->next)
    {
	if (!IS_NPC(vch)) continue;

	if (vch->spec_fun != spec_kingdom_guard && vch->spec_fun != spec_kingdom_healer)
	    continue;

	if (!(k = find_kingdom(vch->kingdom))) continue;

	if (!get_room_index(k->Gateway+1)) continue;

	if (!(vch->in_room && find_kingdom(vch->in_room->kingdom)!=k))
	    continue;

	act("&B$n says, \"&YFUck, you almost breached our kingdoms security for a moment. Losers.&B\".",
	    vch, 0, 0, TO_ROOM);
	act("&m$n waves bye to all the losers.", vch, 0, 0, TO_ROOM);
	char_from_room(vch);
	char_to_room(vch, get_room_index(k->Gateway+1));
	act("&R$n bows and informs all of a security breach.", vch, 0, 0, TO_ROOM);
	act("&W$n says, \"&GI WAS HERE!!! And then, I was THERE!!&W\".", vch, 0, 0, TO_ROOM);
    }

}
