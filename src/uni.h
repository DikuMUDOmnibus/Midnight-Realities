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

DECLARE_DO_FUN(do_raise);
DECLARE_DO_FUN(do_sing);
int countinspires(CHAR_DATA *);
void inspire(CHAR_DATA *);
void uni_heal(CHAR_DATA *, CHAR_DATA *);

#define IS_UNI(ch) (ch->class & CLASS_UNICORN && ch->level >= 3)

// ch->pcdata->powers[]
#define UNI_SONG	0
#define UNI_WAIT	1
#define UNI_DISC_HEAL	2
#define UNI_SIZE	3

// ch->pcdata->powers[0] || ch->pcdata->powers[UNI_SONG]
#define SONG_HEAL	1
#define SONG_INSPIRE	2
#define SONG_QUELL	3
