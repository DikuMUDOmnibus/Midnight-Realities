/* Original by Smoo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "gamble.h"

GAMBLE_DATA *gamble_first = NULL;

void do_gamble(CHAR_DATA *ch, char *argument)
{
    GAMBLE_DATA *gamb;
    char arg[4][MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
    int a;

    if (IS_NPC(ch)) return;

    for (a = 0; a < 4; a++)
	argument = one_argument(argument, arg[a]);

    if (!arg[0][0])
    {
	stc(
"You may use the following commands:
\rGamble join          name          to join game (name)
\rGamble start         name   time   to start game (name)
\rGamble bid           game   bid    to bid on your current game
\rGamble hit           game          to ask the dealer to let you hit
\rGamble stand         game          to ask the dealer to let you stand
\r", ch);

	return;
    }

    if (!str_cmp(arg[0], "stand"))
	stand(ch);
    else
    if (!str_cmp(arg[0], "hit"))
	hitme(ch);
    else
    if (!str_cmp(arg[0], "bid"))
    {
	int bid;

	for (gamb = gamble_first; gamb; gamb = gamb->next)
	{ 
	    for (a = 0; a < MAX_GAMBLE; a++)
		if (gamb->p[a] == ch) break;

	    if (a < MAX_GAMBLE) break;
	}

	if (!gamb)
	{
	    stc("You are not in a game!\n\r", ch);
	    return;
	}

	if (gamb->timer > -1)
	{
	    stc("Wait for the dealer to start the game!\n\r", ch);
	    return;
	}

	if (gamb->stage[a] != PLACE_BET)
	{
	    stc("You have already placed your bet!\n\r", ch);
	    return;
	}

	if (((bid = atoi(arg[1])) < 0 || bid > 300) && ch->trust < 7)
	{
	    stc("You may place a bid of 0 to 300 (0 if you want to play for fun:)\n\r", ch);
	    return;
	}

	if (ch->pcdata->quest < bid && ch->trust < 7)
	{
	    stc("You don't have that much quest points to bid!:P sucks to be you.\n\r", ch);
	    return;
	}

	gamb->wager[a] = bid;
	gamb->stage[a] = DEAL_FIRST;

	sprintf(buf, "&G%s has bid &Y%d &Gon his game.\n\r", ch->name, bid);
	same_game_channel(gamb->name, buf);
	return;
    }
    else
    if (!str_cmp(arg[0], "start"))
    {
	int length, b;
	char name[MAX_INPUT_LENGTH];

	if (!arg[1][0])
	{
	    stc("Enter the name of the game you wish to start.\n\r", ch);
	    return;
	}

	strcpy(name, bash_color(arg[1]));

	for (gamb = gamble_first; gamb; gamb = gamb->next)
	{
	    for (a = 0; a < MAX_GAMBLE; a++)
		if (gamb->p[a] == ch) break;

	    if (gamb->p[a] == ch) break;
	}

	if (gamb)
	{
	    stc("You can only be in one game at a time.\n\r", ch);
	    return;
	}

	for (gamb = gamble_first; gamb; gamb = gamb->next)
	    if (!str_cmp(bash_color(gamb->name), name)) break;

	if (gamb)
	{
	    stc("There is already a game by that name.\n\r", ch);
	    return;
	}

	if (strlen(bash_color(arg[1])) > 15)
	{
	    stc("Game names are limited to 15 characters, not including color.\n\r", ch);
	    return;
	}

	if (!arg[2][0])
	{
	    stc("Please choose an amount of time until the game begins.\n\r", ch);
	    return;
	}

	if ((length = (atoi(arg[2])-1)) < 0 || length > 7)
	{
	    stc("Please choose an amount of ticks inbetween 1 and 7 until the game starts.\n\r", ch);
	    return;
	}

	gamb = malloc(sizeof(GAMBLE_DATA));
	for (a = 0; a < MAX_GAMBLE; a++)
	{
	    gamb->wager[a] = 0;
	    gamb->stage[a] = 0;
	    gamb->p[a] = 0;
	    gamb->broke[a] = FALSE;

	    for (b = 0; b < 80; b++)
	    {
		gamb->card[a][b] = 0;
		if (a == 0) gamb->dealer[b] = 0;
	    }
	}

	gamb->game_starter = ch;
	gamb->p[0] = ch;
	gamb->timer = length;
	gamb->name = str_dup(arg[1]);

	gamb->next = gamble_first;
	gamble_first = gamb;

	sprintf(buf, "&r%s &mhas started a game of &bblackjack!&m Type gamble join &c%s&m to enter the game. &Y%d&m tick%s until the game starts.",
	    ch->name, gamb->name, gamb->timer+1, gamb->timer==0?"":"s");
	gamble_channel(buf);
    }
    else
    if (!str_cmp(arg[0], "join"))
    {
	char name[MAX_INPUT_LENGTH];

	if (!arg[1][0])
	{
	    stc("Join which game?\n\r", ch);
	    return;
	}

	strcpy(name, bash_color(arg[1]));

	for (gamb = gamble_first; gamb; gamb = gamb->next)
	{
	    for (a = 0; a < MAX_GAMBLE; a++)
		if (gamb->p[a] == ch) break;

	    if (gamb->p[a] == ch) break;
	}

	if (gamb)
	{
	    stc("You can only be in one game at a time.\n\r", ch);
	    return;
	}

	for (gamb = gamble_first; gamb; gamb = gamb->next)
	    if (!str_cmp(bash_color(gamb->name), name)) break;

	if (!gamb)
	{
	    stc("There is no game with that name.\n\r", ch);
	    return;
	}

	if (gamb->timer == -1)
	{
	    stc("That game is already closed.\n\r", ch);
	    return;
	}

	if (gamb->p[MAX_GAMBLE-1] != 0)
	{
	    sprintf(buf, "There are already %d players in that game:)\n\r", MAX_GAMBLE);
	    stc(buf, ch);
	    return;
	}

	for (a = 0; a < MAX_GAMBLE; a++)
	    if (!gamb->p[a]) break;

	gamb->p[a] = ch;
	gamb->wager[a] = 0;
	gamb->stage[a] = 0;
	stc("You join the game!\n\r", ch);
	sprintf(buf, "&G%s&R has joined &Y%s&R!", ch->name, gamb->name);
	same_game_channel(gamb->name, buf);
    }
    else
    {
	do_gamble(ch,"");
	return;
    }
}

void gamble_channel(char *string)
{
    char *buf;
    char prepend[] = "&RGamble:-> ";
    DESCRIPTOR_DATA *d;

    buf = malloc(strlen(string)+strlen(prepend)+5);

    strcpy(buf, prepend);
    strcat(buf, string);
    strcat(buf, "\n\r&x");

    for (d = descriptor_list; d; d = d->next)
    {
	if (!d->character || d->connected > CON_PLAYING)
	    continue;

	if (d->character->deaf & CHANNEL_GAMBLE) continue;

	stc(buf, d->character);
    }

    free(buf);
    return;
}


void gamble_start(GAMBLE_DATA *gamb)
{
    char buf[MIL];
    sprintf(buf, "&YThe game &W%s&Y has started.",
	gamb->name);
    gamble_channel(buf);
}

void gamble_update()
{
    GAMBLE_DATA *gamb, *g_n;

    for (gamb = gamble_first ;gamb ;gamb = g_n)
    {
	char buf[MIL];
	g_n = gamb->next;

	if (gamb->timer > -1) continue;
	if (count_dcards(gamb, 1) == 0)
	{
	    gamb->dealer[0] = number_range(1,4)*1000+number_range(2,14);
	    sprintf(buf, "&RThe dealer has dealt %s a %s&R and a &Y##&R.",
		number_range(1,2)==1?"himself":"herself", convert_card(gamb->dealer[0]));
	    same_game_channel(gamb->name, buf);
	}

	dealer_update(gamb);
    }

    for (gamb = gamble_first; gamb ;gamb = g_n)
    {
	int a, count = 0;
	char buf[MIL];

	g_n = gamb->next;
	if (gamb->timer > -1) continue;

	for (a = 0; a <= MAX_GAMBLE; a++)
	{
	    if (gamb->p[a] == NULL) continue;
	    if (gamb->stage[a] == PLACE_BET) count++;
	}

	for (a = 0; a < MAX_GAMBLE; a++)
	{
	    if (gamb->stage[a] >= GAMB_STANDING) continue;

	    if (count_cards(gamb, a, 1) > 21 && !has_jack(gamb, a))
		bust(gamb, a, count_cards(gamb, a, 1));
	    if (has_jack(gamb, a) && count_cards(gamb, a, 2) > 21)
		bust(gamb, a, count_cards(gamb, a, 2));

	    switch(gamb->stage[a])
	    {
		case DEAL_FIRST:
		    if (count > 0) return;

		    gamb->card[a][0] = (number_range(1,4)*1000) + number_range(2,14);
		    gamb->card[a][1] = (number_range(1,4)*1000) + number_range(2,14);
		    sprintf(buf, "&YYou were dealt a &r%s&Y and a &r%s&Y",
			convert_card(gamb->card[a][0]), convert_card(gamb->card[a][1]));
		    stc(buf, gamb->p[a]);
		    gamb->stage[a]++;
		    break;
	    }
	}
    }
}

char *convert_card(int card)
{
    int b;
    char buf[MIL];
    char *buff = 0;
    char *cname = 0;
    static int scount=0;
    static char sbuf[36][80];

    b=card/1000;

    if (b==1)
	buff = "&RD";
    else if (b==2)
	buff = "&RH";
    else if (b==3)
	buff = "&cS";
    else if (b==4)
	buff = "&cC";

    b = card - (b*1000);

    if (b < 11 && b > 1)
	sprintf(buf, "&s%s&Y%-5d&t", buff, b);
    else 
    {
	switch(b)
	{
	    case 11:
		cname = "&GJack ";
		break;
	    case 12:
		cname = "&GQueen";
		break;
	    case 13:
		cname = "&GKing ";
		break;
	    case 14:
		cname = "&GAce  ";
		break;
	}

	sprintf(buf, "&s%s%s&t", buff, cname);
    }

    if (scount > 35)
	scount = 0;

    strcpy(sbuf[scount], buf);
    scount++;
    return sbuf[scount-1];
}

void same_game_channel(char *name, char *string)
{
    char buf[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    GAMBLE_DATA *gamb;

    for (gamb = gamble_first ;gamb; gamb = gamb->next)
	if (!str_cmp(name, gamb->name)) break;

    if (!gamb)
    {
	bug("same_game_channel: game not found.", 0);
	return;
    }

    sprintf(buf, "&s&R%s:-> %s\n\r&t", name, string);
    for (d = descriptor_list; d; d = d->next)
    {int a;
	if (!d->character || d->connected > CON_PLAYING)
	    continue;

	for (a = 0; a < MAX_GAMBLE; a++)
	    if (gamb->p[a] == d->character) {stc(buf, d->character); break;}
    }
}

void gamble_prompt(CHAR_DATA *ch	)
{
    GAMBLE_DATA *gamb;
    char buf[MIL];
    int a, b;

    if (!ch) return;

    for (gamb = gamble_first; gamb; gamb = gamb->next)
    {
	for (a = 0; a < MAX_GAMBLE; a++)
	{
	    if (gamb->p[a] != ch) continue;

	    stc("&M", ch);
	    stc(gamb->name, ch);
	    stc("    ", ch);

	    if (gamb->timer > -1)
	    {
		sprintf(buf, "&R%d ticks left...\n\r", gamb->timer+1);
		stc(buf, ch);
		continue;
	    }

	    if (gamb->stage[a] == BUSTED) {stc("&gYou are &RBUSTED!&x\n\r", ch); continue;}

	    for (b = 0; b <= 16; b++)
	    {
		if (gamb->card[a][b] > 0)
		{
		    stc("  ", ch);
		    stc(convert_card(gamb->card[a][b]), ch);
		}
	    }

	    if (gamb->timer < 0)
	    {
		sprintf(buf, "    &cCount: %d", count_cards(gamb,a,1));
		stc(buf, ch);

		if (has_jack(gamb,a))
		{
		    sprintf(buf, "  &gOR  &c%d", count_cards(gamb,a,2));
		    stc(buf, ch);
		}

		if (gamb->stage[a] == GAMB_STANDING)
		    stc(" - &GSTANDING", ch);

		if (dhas_jack(gamb))
		    sprintf(buf, "     &RVs &Y%d &yor &Y%d", count_dcards(gamb, 1), count_dcards(gamb, 2));
		else
		    sprintf(buf, "     &RVs &Y%d", count_dcards(gamb, 1));

		stc(buf, ch);
	    }

	    stc("\n\r", ch);
	}
    }
}

int count_cards(GAMBLE_DATA *gamb, int a, int type)
{
    int b, c, count = 0;

    if (gamb->p[a] == 0) return 0;

    if (type == 1)
    {
	for (b = 0; b <= 20; b++)
	{
	    c = gamb->card[a][b] - ((gamb->card[a][b]/1000)*1000);
	    if (c > 10 && c != 14) count += 10;
	    else if (c == 14) count += 11;
	    else count += c;
	}

	return count;
    }

    for (b = 0; b <= 20; b++)
    {
	c = gamb->card[a][b] - ((gamb->card[a][b]/1000)*1000);
	if (c  > 10 && c != 14) count += 10;
	else if (c == 14) count += 1;
	else count += c;
    }

    return count;
}

bool has_jack(GAMBLE_DATA *gamb, int a)
{
    int b;

    for (b = 0; b <= 20; b++)
	if (gamb->card[a][b] - ((gamb->card[a][b]/1000)*1000) == 14)
	    return TRUE;

    return FALSE;
}

bool dhas_jack(GAMBLE_DATA *gamb)
{
    int b;

    for (b = 0; b <= 20; b++)
	if (gamb->dealer[b] - ((gamb->dealer[b]/1000)*1000) == 14)
	    return TRUE;

    return FALSE;
}

void bust(GAMBLE_DATA *gamb, int a, int count)
{
    char buf[MIL];

    sprintf(buf, "&g%s&y has busted. bid of {&g%d&y deducted. (Total of &c%d&y)",
	gamb->p[a]->pcdata->switchname, gamb->wager[a], count);
    same_game_channel(gamb->name, buf);

    gamb->p[a]->pcdata->quest -= gamb->wager[a];

    sprintf(buf, "&yYou have busted! %d qp deducted!\n\r", gamb->wager[a]);
    stc(buf, gamb->p[a]);

    gamb->wager[a] = 0;
    gamb->stage[a] = BUSTED;
}

void hitme(CHAR_DATA *ch)
{
    GAMBLE_DATA *gamb;
    int a, b;
    char buf[MIL];

    for (gamb = gamble_first; gamb; gamb = gamb->next)
    {
	for (a = 0; a < MAX_GAMBLE; a++)
	    if (gamb->p[a] == ch) break;

	if (gamb->p[a] == ch) break;
    }

    if (!gamb)
    {
	stc("You aren't playing a game!\n\r", ch);
	return;
    }

    if (gamb->timer > -1)
    {
	stc("The game hasn't even begun!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == PLACE_BET)
    {
	stc("You need to place a bet first!\n\r", ch);
	return;
    }

    for (b = 0; b < MAX_GAMBLE; b++)
	if (gamb->p[a] && gamb->stage[a] == PLACE_BET) break;

    if (gamb->p[a] && gamb->stage[a] == PLACE_BET)
    {
	stc("Other people still need to place a bet!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == GAMB_STANDING)
    {
	stc("You have already chosen to stand.\n\r", ch);
	return;
    }

    if (gamb->stage[a] == BUSTED)
    {
	stc("You are Busted!\n\r", ch);
	return;
    }

    sprintf(buf, "&R%s&Y says enthusiastically, \"&GHIT ME!&Y\".", gamb->p[a]->pcdata->switchname);
    same_game_channel(gamb->name, buf);

    for (b = 0; b < 21; b++)
	if (gamb->card[a][b] == 0) break;

    gamb->card[a][b] = (number_range(1,4)*1000) + number_range(2,14);

    sprintf(buf, "&YYou were dealt a &r%s&Y&x\n\r",
	convert_card(gamb->card[a][b]));
    stc(buf, ch);

    return;
}

void stand(CHAR_DATA *ch)
{
    GAMBLE_DATA *gamb;
    int a, b;
    char buf[MIL];

    for (gamb = gamble_first; gamb; gamb = gamb->next)
    {
	for (a = 0; a < MAX_GAMBLE; a++)
	    if (gamb->p[a] == ch) break;

	if (gamb->p[a] == ch) break;
    }

    if (!gamb)
    {
	stc("You aren't playing a game!\n\r", ch);
	return;
    }

    if (gamb->timer > -1)
    {
	stc("The game hasn't even begun!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == PLACE_BET)
    {
	stc("You need to place a bet first!\n\r", ch);
	return;
    }

    for (b = 0; b < MAX_GAMBLE; b++)
	if (gamb->p[a] && gamb->stage[a] == PLACE_BET) break;

    if (gamb->p[a] && gamb->stage[a] == PLACE_BET)
    {
	stc("Other people still need to place a bet!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == 1)
    {
	stc("The dealer is not ready!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == BUSTED)
    {
	stc("You are busted!\n\r", ch);
	return;
    }

    if (gamb->stage[a] == GAMB_STANDING)
    {
	stc("You are already standing!\n\r", ch);
	return;
    }

    gamb->stage[a] = GAMB_STANDING;
    if (has_jack(gamb,a) && count_cards(gamb,a,1) > count_cards(gamb,a,2)
    &&count_cards(gamb,a,1)<= 21) b = 1;
    else if (has_jack(gamb,a)) b = 2;
    else b = 1;

    if (b == 1)
	sprintf(buf, "&c%s stands on a hard %d.&x", gamb->p[a]->pcdata->switchname, count_cards(gamb,a,b));
    else
	sprintf(buf, "&c%s stands on a soft %d.&x", gamb->p[a]->pcdata->switchname, count_cards(gamb,a,b));

    same_game_channel(gamb->name, buf);
    return;
}

int count_dcards( GAMBLE_DATA *gamb, int type)
{
    int b, c, count = 0;

    if (!gamb) return 0;

    if (type == 1)
    {
	for (b = 0; b <= 20; b++)
	{
	    c = gamb->dealer[b] - ((gamb->dealer[b]/1000)*1000);
	    if (c > 10 && c != 14) count += 10;
	    else if (c == 14) count += 11;
	    else count += c;
	}

	return count;
    }

    for (b = 0; b <= 20; b++)
    {
	int c = gamb->dealer[b]-((gamb->dealer[b]/1000)*1000);
	if (c > 10 && c != 14) count += 10;
	else if (c == 14) count += 1;
	else count += c;
    }

    return count;
}

void dealer_update(GAMBLE_DATA *gamb)
{
    int a, c;
    char buf[MIL];

    for (a = 0; a < MAX_GAMBLE; a++)
    {
	if (gamb->p[a] == 0) continue;
	if (gamb->stage[a] < GAMB_STANDING) return;
    }

    for (a = 0; a < 21; a++)
	if (gamb->dealer[a] == 0) break;

    if (dhas_jack(gamb)) c = count_dcards(gamb,2);
    else c = count_dcards(gamb,1);

    if (c > 21) {end_game(gamb, "busted"); return;}
    if (c >= 17) {end_game(gamb, "standing"); return;}

    gamb->dealer[a] = (number_range(1,4)*1000)+number_range(2,14);
    sprintf(buf, "&YThe dealer raises a %s and places it in front of him",
	convert_card(gamb->dealer[a]));

    same_game_channel(gamb->name, buf);
}

void end_game(GAMBLE_DATA *gamb, char *type)
{
    char buf[MIL];
    int a, c, d;

    if (!str_cmp(type, "busted"))
    {
	sprintf(buf, "&gThe dealer has &Rbusted&g.");
	same_game_channel(gamb->name, buf);

	for (a = 0; a < MAX_GAMBLE; a++)
	{
	    if (gamb->p[a] == NULL) continue;
	    if (gamb->stage[a] == BUSTED) continue;
	    winner(gamb, a);
	}

    }
    if (!str_cmp(type, "standing"))
    {
	for (a = 0; a < MAX_GAMBLE; a++)
	{
	    if (gamb->p[a] == NULL) continue;
	    if (gamb->stage[a] == BUSTED) continue;

	    if (has_jack(gamb,a))
	    {
		if ((c = count_cards(gamb,a,1)) > 21)
		    c = count_cards(gamb,a,2);
	    }
	    else c = count_cards(gamb,a,1);

	    if (dhas_jack(gamb))
	    {
		if ((d = count_dcards(gamb,1)) > 21)
		    d = count_dcards(gamb,2);
	    }
	    else d = count_dcards(gamb,1);

	    if (d > c) loser(gamb,a);
	    else if (d == c) tied(gamb,a);
	    else winner(gamb,a);
	}
    }

    close_game(gamb);
}


void loser(GAMBLE_DATA *gamb, int a)
{
    char buf[MIL];

    if (gamb->p[a] == NULL) return;

    sprintf(buf, "&y%s is a loser!", gamb->p[a]->name);
    same_game_channel(gamb->name, buf);

    sprintf(buf, "&RYou have lost. %d qps deducted.\n\r", gamb->wager[a]);
    stc(buf, gamb->p[a]);
    gamb->p[a]->pcdata->quest -= gamb->wager[a];
    save_char_obj(gamb->p[a]);
}

void winner(GAMBLE_DATA *gamb, int a)
{
    char buf[MIL];

    if (gamb->p[a] == NULL) return;

    sprintf(buf, "&M%s has won %d qps!", gamb->p[a]->name, gamb->wager[a]);
    same_game_channel(gamb->name, buf);

    sprintf(buf, "&GYou have won! %d qps awarded.\n\r", gamb->wager[a]);
    stc(buf, gamb->p[a]);
    gamb->p[a]->pcdata->quest += gamb->wager[a];
    save_char_obj(gamb->p[a]);
}


void tied(GAMBLE_DATA *gamb, int a)
{
    char buf[MIL];

    if (gamb->p[a] == NULL) return;

    sprintf(buf, "&M%s has tied with the dealer!", gamb->p[a]->name);
    same_game_channel(gamb->name, buf);

    sprintf(buf, "&wYou have tied with the dealer. No qps awarded/deducted.\n\r");
    stc(buf, gamb->p[a]);
    save_char_obj(gamb->p[a]);
}


void close_game(GAMBLE_DATA *gamb)
{
    int a;
    char buf[MIL];

    if (!gamb) return;

    sprintf(buf, "The game %s has ended.", gamb->name);
    gamble_channel(buf);

    gamb->game_starter = NULL;
    for (a = 0; a < MAX_GAMBLE; a++)
	gamb->p[a] = NULL;

    if (gamble_first == gamb)
	gamble_first = gamble_first->next;

    else
    {
	GAMBLE_DATA *pre;

	for (pre = gamble_first; pre->next != gamb; pre = pre->next)
	    ;

	pre->next = gamb->next;
    }

    free_string(gamb->name);
    free(gamb);

    return;
}

void do_break_game(CHAR_DATA *ch, char *argument)
{
    GAMBLE_DATA *gamb;
    int a;
    char buf[MIL];

    for (gamb = gamble_first; gamb; gamb = gamb->next)
    {
	for (a = 0; a < MAX_GAMBLE; a++)
	    if (gamb->p[a] == ch) break;

	if (gamb->p[a] == ch) break;
    }

    if (!gamb)
    {
	stc("You aren't in a game!\n\r", ch);
	return;
    }

    gamb->broke[a] = TRUE;

   for (a = 0; a < MAX_GAMBLE; a++)
   {
	if (gamb->p[a] == NULL) continue;
	if (gamb->p[a]->desc == NULL) continue;
	if (gamb->broke[a] == FALSE) break;
   }

   if (a == MAX_GAMBLE)
   {
	gamble_channel("GAME DECLARED BROKEN.");
	close_game(gamb);
	return;
   }

    sprintf(buf, "%s has declared the game BROKEN. If all other players that have a link type break_game or breakgame, the game will be gone.", ch->name);
    same_game_channel(gamb->name, buf);
    return;
}
