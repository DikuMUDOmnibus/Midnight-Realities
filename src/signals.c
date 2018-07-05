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

#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#define   __USE_GNU 1 // includes basename prototype from string.h
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include <sys/stat.h>
#include "merc.h"

//Guys, I, Smoo, wrote this from scratch. It took me a while so don't
//claim it as your own.

extern int control, port;
#define DD DESCRIPTOR_DATA
bool    write_to_descriptor     args( ( DD *desc, char *txt, int length )
);
void attempt_copyover( void );
void crash_notice( int );
void term_notice( int );

void init_signals( void )
{
  last_command = str_dup("");

    signal(SIGSTOP,	SIG_IGN		);
    signal(SIGPIPE,	SIG_IGN		);
    signal(SIGPFILE,	crash_notice	);
    signal(SIGSEGV,     crash_notice	);
    signal(SIGFPE,	crash_notice	);

return;
}

void crash_notice( int sig )
{
 FILE *fp;
 static bool first = TRUE;

if (first) {
 sprintf(log_buf, "%s/crashStamp.txt", global("areadir"));
 fp = fopen(log_buf, "w");
 if (fp)
 {
    fprintf(fp, "%d\nThis file contains a TIMESTAMP of the last crash, for computing how long it has been since the last crash on login.\n", TimeStamp(mud_time));
    fflush(fp);
    fclose(fp);
 }

 sprintf(log_buf, "%s/crash.txt", global("areadir"));
 fp =fopen(log_buf, "a");
 if (fp) {
 fprintf(fp, "Crash at %s system time.\n", mud_time);
 fprintf(fp, "Last pfile was: %s..%s\n", (char*)feof_pfile_handler(NULL,FEOFP_GETNAME), sig!=SIGPFILE ? "loaded properly" : "BUGGED");
 fprintf(fp, "Last command: %s <BY> %s <%s>\n",
last_user,last_command,command_finished ? "finished" : "BUGGED");


fflush(fp);
fclose(fp);}
}
 if ((!first) || (crashAbort && sig != SIGHUP)) 
 {
    char *wri;
    struct dirent *ent;
    DIR *dir;
    char **cmdArgs = NULL;

    if (!crashAbort)log_string("Crash/Cop is having an unexpected error. Breaking.");

    if (crashAbort)
	wri = str_dup("The mud is crashing. Creating a copy of the mud for debugging.\n\r");
    else
	wri = str_dup("The mud is crashing (unrecoverable.) Please reconnect.\n\r");
malloc(20000); // these memory leaks are incase the mud developes a loop inside here,
	       // which is unlikely, however it will break out once memory is too excessive.
	       // This memory is not carried over in to the next copy of the mud.
    dir = opendir("/proc/self/fd");

    while ((ent = readdir(dir)))
    {
	if (atoi(basename(ent->d_name)) <= 0) continue;
	write(atoi(basename(ent->d_name)), wri, sizeof(wri));
	if (atoi(basename(ent->d_name))>3) close(atoi(basename(ent->d_name)));
    }
    closedir(dir);
    if (crashAbort) {abort();exit(1);}

    if (globWFD) {fclose(globWFD);globWFD=NULL;}

    cmdArgs = AddStr(cmdArgs, "Realities/Respawn");
    cmdArgs = AddStr(cmdArgs, "-p");
    cmdArgs = AddStr(cmdArgs, CHAR2(port));

    if (spiderquest)
	cmdArgs = AddStr(cmdArgs, "-q");
    if (spiderquest)
	cmdArgs = AddStr(cmdArgs, CHAR2(spidernum));

    if (passlocked)
	cmdArgs = AddStr(cmdArgs, "-pass");
    if (passlocked)
	cmdArgs = AddStr(cmdArgs, passlock);

    cmdArgs = AddStr(cmdArgs, "-E");
    cmdArgs = AddStr(cmdArgs, CHAR2(xp_compute(0,0)));
    cmdArgs = AddStr(cmdArgs, "-w");
    cmdArgs = AddStr(cmdArgs, workfile);
    cmdArgs = AddStr(cmdArgs, "-b");
    cmdArgs = AddStr(cmdArgs, datafile);

    close(control);
    execv(EXE_FILE, cmdArgs);
    exit(1);
}
 first = FALSE;

{
 DESCRIPTOR_DATA *d;
 DESCRIPTOR_DATA *d_next;

// if (sig != SIGINT && sig != SIGPFILE)
if (sig != SIGPFILE)
attempt_copyover();

for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected <= CON_PLAYING) do_wake(d->character,
"");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected <= CON_PLAYING) do_call(d->character,
"all");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected <= CON_PLAYING)
save_char_obj(d->character);

 for (d = descriptor_list; d; d = d_next)
 {
  d_next = d->next;

     write_to_descriptor(d, "The Midnight Realities are about to crash. Hold on..", 0);

  close_socket(d);
} }
abort();
}

void attempt_copyover(void)
{
 char **cmdArgs = NULL;
 DESCRIPTOR_DATA *d,*d_next;
 FILE *fp;
	sprintf(log_buf, "../hotboot.dat");
	fp = fopen (log_buf, "w");
	if (!fp)
		return;

log_string("CRASHING..");
for (d = descriptor_list; d; d = d_next)
{
 CHAR_DATA *ch = d->original ? d->original : d->character;
 d_next = d->next;
 if (!ch || d->connected > CON_PLAYING)
 {write_to_descriptor(d,"We are crashing. Please reconnect.\n\r", 0);close_socket(d);continue;}
 save_char_obj(ch);
fprintf(fp, "%d %s %s\n", d->descriptor, d->character->pcdata->switchname, d->host);
 write_to_descriptor(d, "The Midnight Dreams are crashing! Attempting to copyover from the crash.\n\r", 0);
}
        fprintf (fp, "-1\n");
	fflush(fp);
        fclose (fp);

    cmdArgs = AddStr(cmdArgs, "Realities");
    cmdArgs = AddStr(cmdArgs, "-p");
    cmdArgs = AddStr(cmdArgs, CHAR2(port));

    cmdArgs = AddStr(cmdArgs, "-c");
    cmdArgs = AddStr(cmdArgs, CHAR2(control));

    if (spiderquest)
	cmdArgs = AddStr(cmdArgs, "-q");
    if (spiderquest)
	cmdArgs = AddStr(cmdArgs, CHAR2(spidernum));

    cmdArgs = AddStr(cmdArgs, "-E");
    cmdArgs = AddStr(cmdArgs, CHAR2(xp_compute(0,0)));
    cmdArgs = AddStr(cmdArgs, "-w");
    cmdArgs = AddStr(cmdArgs, workfile);
    cmdArgs = AddStr(cmdArgs, "-b");
    cmdArgs = AddStr(cmdArgs, datafile);

execv(EXE_FILE, cmdArgs);

for (d = descriptor_list; d; d = d->next)
 write_to_descriptor(d, "COPYOVER FAILED! Attempting NORMAL boot!\n\r", 0);

return;
}

void term_notice(int sig)
{
    DESCRIPTOR_DATA *d;

    for (d = descriptor_list;d ;d = d->next)
	write_to_descriptor(d,
"the server is sending the mud a signal to SHUT DOWN the mud. The mud may go down at any time, and it will be back up as soon as possible! Please be patient, this is out of the admins control.\n\r", 0);

    return;
}

char **AddStr(char **dbstr, char *add)
{
    int x = 0;
    char **dp;

    if (dbstr == NULL)
    {
	dbstr = malloc(sizeof(char*[1]));
	*dbstr = NULL;
	return AddStr(dbstr, add);
    }

    while (*(dbstr+x)) x++;

    dp = malloc(sizeof( char *[x+2] ));

    x = 0;
    while (*(dbstr+x))
    {
	*(dp+x) = *(dbstr+x);
	x++;
    }

    *(dp+x) = malloc(strlen(add)+1);
    strcpy(*(dp+x), add);

    *(dp+x+1) = NULL;

    free(dbstr);

    return dp;
}
