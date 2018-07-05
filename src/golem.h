/*
    The following is a file from the Midnight Dreams source code.
    I do acknowledge that this is a diku, merc, godwar, LoW, and rogue
    mud, but I chose not to display their annoying header on every file
    and it is in the GNU license agreement that they do not have the right
    to demand anything, although I will keep in their credit helpfiles.
    I am not trying to steal anything from them, I acknowledge that this
    wouldn't be possible without them, and you may remove this yourself
    too.

    Anywho, this mud is the followup to midnight dreams v 1.0, and it will
    have a great ammount of modifications since bryantos the digit ran it.
    Have fun.
*/

/* yay its a smooclass */

extern char golem_ranks[11][20];

#define IS_GOLEM(xch) (!IS_NPC((xch)) && (xch)->level >= 3 && (xch)->class & CLASS_GOLEM)
#define IS_INFORM(xch,form) ((IS_GOLEM((xch)) && (xch)->pcdata->powers[0] & (form))) 
#define IS_STONE(xch)  						\
(IS_INFORM((xch),GOLEM_EMERALD) ||\
IS_INFORM((xch),GOLEM_GOLD) || IS_INFORM((xch),GOLEM_CRYSTAL) ||\
IS_INFORM((xch),GOLEM_RUBY) || IS_INFORM((xch),GOLEM_OBSIDIAN) ||\
IS_INFORM((xch),GOLEM_GRANITE) || IS_INFORM((xch),GOLEM_DIAMOND))
#define stone1(xch) (xch)->pcdata->powers[0]
#define NO_STONE(xch) ch->morphy = FALSE;ch->convert_to =0;\
stone1((xch)) &= ~(GOLEM_EMERALD |GOLEM_GOLD |GOLEM_CRYSTAL| GOLEM_RUBY| GOLEM_OBSIDIAN |GOLEM_GRANITE| GOLEM_DIAMOND);

#define GOLEM_EMERALD	2
#define GOLEM_GOLD	4
#define GOLEM_CRYSTAL	8
#define GOLEM_RUBY	16
#define GOLEM_OBSIDIAN  32
#define GOLEM_GRANITE	64
#define GOLEM_DIAMOND	128

#define GOLEM_WEAPON         256


#define CRYSTAL_MOVE1	1
#define CRYSTAL_MOVE2	2

#define OBSIDIAN_MOVE1  4
#define OBSIDIAN_MOVE2  8
#define OBSIDIAN_MOVE3 16
#define OBSIDIAN_MOVE4 32
#define OBSIDIAN_MOVE5 64

#define EMERALD_MOVE1 128
#define EMERALD_MOVE2 256

#define IN_RUBY       512

void crystal_change ( CHAR_DATA *ch);
void obsidian_change ( CHAR_DATA *ch);
void emerald_change ( CHAR_DATA *ch);

DECLARE_DO_FUN( do_rocktalk);

/* powers[2] */
#define IN_ASTRAL 1 // this is the room number the person is in, NOT a bit

#define SYMANTICS 3
#define TACTICALS 4
#define MAGITOS   5

void gold_change( CHAR_DATA * );
GRANITE_DATA *get_d(GRANITE_DATA *list, int count);
void astral_move(CHAR_DATA *ch, short dir);
char *get_astral_room(CHAR_DATA *ch);
