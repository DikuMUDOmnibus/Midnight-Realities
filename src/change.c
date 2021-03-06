#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "merc.h"

void save_changes args ( ( ) );
CHANGE_DATA *change_first;

void do_changes(CHAR_DATA *ch, char *argument)
{
 int count = 1, a = 0;
 char buf[MSL*4];
 CHANGE_DATA *change = NULL;

 if (!str_prefix("view",argument) || !str_prefix("read",argument))
 {
   argument = one_argument(argument, log_buf);

   if (*argument != '\0')
   {
	a = atoi(argument);
	for (change = change_first; change; change = change->next)
	    if (change->number == a) break;

	if (!change)
	{stc("That change doesn't exist!\n\r",ch); return;}

	ch->changeview = change;
    }
else    ch->changeview = change_first;
    if (ch->changeview)
    {
	stc(ch->changeview->text,ch);
    }
    else
	stc("There are no changes listed.\n\r",ch);
    return;
 }
 if (!str_cmp(argument, "next"))
 {
    if (!ch->changeview)
    {
	stc("You aren't viewing a change at the moment..\n\r",ch);
	return;
    }
    ch->changeview = ch->changeview->next;
    if (ch->changeview)
    {
	stc(ch->changeview->text,ch);
    }
    if (!ch->changeview || !ch->changeview->next)
	stc("You are at the end of the change list.\n\r",ch);
    return;
 }
 if (!str_cmp(argument, "list"))
 {
    for (change = change_first; change; change = change->next)
    {
	sprintf(log_buf, "%-3d][%d] %s\n\r",change->number, change->count, change->title);
	stc(log_buf,ch);
    }
    return;
 }

 if (IS_NPC(ch) || ch->trust < 10 || *argument == '\0')
 {
    stc("You must choose out of \"change list\" \"change view\" or \"change next\"\n\r",ch);
    return;
 }

 for (change = change_first; change && change->next; change = change->next) count++;

 if (!change_first) count = 0;

 if (!change || change->count > 7 || !str_cmp(argument, "new"))
 {
  CHANGE_DATA *cnew;
a = count;
    cnew = alloc_perm(sizeof(*cnew));
    cnew->next = NULL;
 if (change)
    change->next = cnew;
    cnew->text = str_dup("");
    sprintf(log_buf, "Change started on %s.", ctime(&current_time));
    log_buf[strlen(log_buf)-2] = '\0';
    cnew->title = str_dup(log_buf);
strcat(log_buf,"\n\r");
    cnew->text = str_dup(log_buf);
    cnew->count = 0;

    if (!change_first)
	change_first = cnew;
    cnew->number = count;
 change = cnew;
 }

if (!str_cmp(argument, "new"))
{stc("New change file created.\n\r", ch);return;}

 sprintf(buf, "%s%s: %s\n\r",change->text, ch->name, argument);
 
 free_string(change->text);
 change->text = str_dup(buf);
 change->count++;
 save_changes();

 stc("Change appended.\n\r",ch);
 do_echo(ch,"&WChange appended. Check out the 'change' command for details.");
 return;
}

void save_changes()
{
 FILE *fp;
 CHANGE_DATA *change;

 if ((fp = fopen("../area/changes.txt","w")) == NULL)
    return;

 for (change = change_first; change; change = change->next)
    fprintf(fp, "%d %d %s~ %s~\n",change->number,change->count,change->text,
change->title);

 fprintf(fp,"END\n");

 fflush(fp);
 fclose(fp);

 return;
}

void load_changes()
{
 FILE *fp;
 CHANGE_DATA *change, *cnew;
 char *word;
 if ((fp = fopen("../area/changes.txt","r")) == NULL)
    return;

 while (str_cmp((word = fread_word(fp)), "END"))
 {
    cnew = alloc_perm(sizeof(*cnew));
    cnew->number = atoi(word);
    cnew->count = fread_number(fp);
    cnew->text = fread_string(fp);
    cnew->title = fread_string(fp);
  for (change = change_first; change && change->next; change = change->next)
    ;
  if (!change)
    change_first = cnew;
  else
    change->next = cnew;
 }

 fclose(fp);

 return;
}
