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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>	
#include "merc.h"
#define DD DESCRIPTOR_DATA
void init_descriptor    args( (DESCRIPTOR_DATA *dnew, int desc) );
bool    write_to_descriptor     args( ( DD * desc, char *txt, int length ));
bool    process_output          args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
bool write_to_descriptor2	args( ( int desc, char *txt, int length));

extern int port, control;

//iRecon was created for use specifically with the original Midnight Realities, to switch inbetween two codebases on the
//test port. It is not intended for public use, however may be adapted.

void do_hotboot( CHAR_DATA *ch, char *argument )
{
 FILE *fp;
 DESCRIPTOR_DATA *d, *d_next;
 CHAR_DATA *v;
 bool iRecon = FALSE, res = FALSE;
 char **cmdArgs=NULL;

 if (!str_cmp(argument, "reconstruct")) iRecon = TRUE;
 if (!str_cmp(argument, "reset")) {iRecon = TRUE;res = TRUE;argument = one_argument(argument,argument);}
 for (v = char_list; v; v = v->next)
    if ((v->fighting && !IS_NPC(v->fighting) && !IS_NPC(v))||v->pnote)
	break;

 if (v && str_cmp(argument, "now") && !iRecon && ch)
 {
    if (v->pnote)
    {
	sprintf(log_buf, "%s is writing a note right now! Are you sure you want to hotboot? (type hotboot now to continue)\n\r", IS_NPC(v)?v->short_descr:v->pcdata->switchname);
	stc(log_buf, ch);
    }
    else
	stc("There is a PK fight going on right now! Type checkpk to see when
\rit is over, and who is pking!\n\rtyped hotboot NOW to hotboot now
\rbut please don't do this unless necassary, we WANT more pk!heh\n\r", ch);
    return;
 }

 if (v && !iRecon && ch)
 {
    stc("Okay....but there IS a pk fight going on you know, and Pk should
\rbe encouraged, not killed.\n\r", ch);
    do_checkpk(ch,"");
 }

if (iRecon)
 sprintf(log_buf, "/home/smoo/dragon/area/copyover.tmp");
else
 sprintf(log_buf, "../hotboot.dat");
 if ((fp = fopen(log_buf, "w")) == NULL)
 {
if (ch)    stc("Hotboot failed. ID 001\n", ch);
    log_string("Hotboot failed..");
    return;
 }

 for (d = descriptor_list; d; d = d->next)
 {CHAR_DATA *vch;if ((vch = d->character) == NULL)
   continue;
  if (d->connected > CON_PLAYING) continue;
    d->character->fighting = NULL;
    d->character->position = POS_STANDING;
    smooregen(vch,60000,60000,60000);
if (iRecon && !res)
    stc("&WRECONSTRUCTIVE SURGURY
\r&GAutomatic wake, unfight, call all, and save.#n\n\r", vch);
else
    stc("&RHotbooting
\r&GAutomatic wake, unfight, call all, and save.#n\n\r", vch);
}


for (d = descriptor_list; d; d = d->next)
{
    if (d->character && d->connected <= CON_PLAYING) do_wake(d->character, "");
}
for (d = descriptor_list; d; d = d->next)
{
    if (d->character && d->connected <= CON_PLAYING)
{
    do_call(d->character, "all");
stc("Saved.\n\r",d->character);/*the real save is below with no message */
}
}

 for (d = descriptor_list; d; d = d_next)
 {
    d_next = d->next;
    if (!d->character || d->connected > CON_PLAYING)
    {
	write_to_descriptor(d, "We are rebooting. Please reconnect.\n", 0);
	close_socket(d);
	continue;
    }
    process_output(d, FALSE);
    save_char_obj(d->character);
    fprintf(fp, "%d %s %s\n", d->descriptor, d->character->pcdata->switchname,d->host);
 }

 fprintf(fp, "-1\n");
 fflush(fp);
 fclose(fp);

if (iRecon && !res)
chdir("/home/smoo/dragon/src");
else
chdir("~/MR2/src");

#define PAIR(add1,add2) cmdArgs=AddStr(cmdArgs,add1);cmdArgs=AddStr(cmdArgs,add2);

if (globWFD) fclose(globWFD); globWFD = NULL;
if (iRecon && !res)
 execl("/home/smoo/dragon/src/dragon", "iReconDragon", CHAR2(port), "iReconstruct", CHAR2(control), NULL); 

cmdArgs = AddStr(cmdArgs,"Realities");
PAIR("-p",CHAR2(port));
PAIR("-c",CHAR2(control));
if (crashAbort)
cmdArgs = AddStr(cmdArgs,"-A");
if (iRecon && res)
PAIR("-b", !str_cmp(argument,"")?"../boot":argument);
PAIR("-w", workfile);
if (spiderquest)
PAIR("-q", CHAR2(spidernum));
PAIR("-E", CHAR2(xp_compute(0,0)));
if (passlocked)
PAIR("-pass", passlock);

execv(EXE_FILE, cmdArgs);

 for (d = descriptor_list; d; d = d->next)
    stc("&RHotboot Failed.\n\r", d->character);

 return;
}

void hotboot_recover( int stage)
{
 DESCRIPTOR_DATA *d;
 FILE *fp;
 char name[100];
 char host[100];
 int desc;
 bool fOld;
 char hbootFILE[MSL];

 log_string("Hotboot recovery started..");
 sprintf(hbootFILE, "../hotboot.dat");
 if ((fp = fopen(hbootFILE, "r")) == NULL)
 {
    log_string("Hotboot recovery FAILED.");
    exit(1);
 }
 for (; ;)
 {

    fscanf( fp, "%d %s %s", &desc, name, host);

    if (desc == -1)
	break;
  if (stage==1)
  {
    d = alloc_perm(sizeof(*d));
    d->descriptor = desc;

    if (!write_to_descriptor(d, "\n\rRestoring from hotboot..\n\r", 0))
    {
	close(desc);
	continue;
    }

    init_descriptor(d, desc);

    d->host = str_dup(host);
    d->next = descriptor_list;
    descriptor_list = d;
    d->connected = CON_COPYOVER_RECOVER;

  continue;
  }
  for (d = descriptor_list;d;d = d->next)
    if (d->descriptor == desc) break;

  if (!d) {close(desc);log_string("Awww we had to close a descriptor in hotboot, it got lost=(");continue;}


    if ((fOld = load_char_obj(d, name)) == FALSE)
    {
	write_to_descriptor2(desc, "\n\rByebye. You wern't saved properly:)\n\r", 0);
	close_socket(d);
	continue;
    }

    stc("&RH#b&Rot&Bboo&Gt #bRecovery S&Duccessful.#n\n\r", d->character);
    if (!d->character->in_room)
	d->character->in_room = get_room_index(ROOM_VNUM_ALTAR);

    d->character->next = char_list;
    char_list = d->character;

    char_to_room(d->character, d->character->in_room);
    d->connected = CON_PLAYING;
    do_look(d->character, "auto");

    act("&D$n fades back into the realm.", d->character, 0, 0, TO_ROOM);
 }

 fclose(fp);
if (stage==2)
 unlink(hbootFILE);
 return;
}
