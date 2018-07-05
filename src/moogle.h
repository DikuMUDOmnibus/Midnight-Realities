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

#define IS_MOOGLE(ch) (ch->class & CLASS_MOOGLE && ch->level >= 3)


typedef void MOOG_FUN args( ( CHAR_DATA *, int, void * ) );

struct moogle_dance_stats
{
    char *name ;        int type,       lag;
    int hp_cost,        mn_cost ,       mv_cost;
    MOOG_FUN *run;	bool pk_check;
    int power,          level;
};
extern const struct moogle_dance_stats moogle_table[];

/* dance TYPES */
#define MOOG_IN_FIGHT		1
#define MOOG_IGNORE		2
#define MOOG_VIOLENCE		3
#define MOOG_GLOBAL_PERS	4


/* power array definitions */
#define MOOG_WIND_SONG		0
#define MOOG_FOREST_SUIT	1
#define MOOG_DESERT_ARIA	2
#define MOOG_LOVE_SONATA	3
#define MOOG_WATER_RONDO	4
#define MOOG_EARTH_BLUES	5
#define MOOG_DUSK_REQUIUM	6
#define MOOG_SNOWMAN_JAZZ	7
#define KUPPO_BITS		8
#define MOOG_WAIT		9
#define MOOG_NUTS	       10

/* definitions for ch->pcdata->powers[8] (KUPPO_BITS) */
#define KUPPO_SUN_BATH		1


int moogle_lookup(char *);
bool dance_checkup(CHAR_DATA *);

DECLARE_DO_FUN(do_dance);

