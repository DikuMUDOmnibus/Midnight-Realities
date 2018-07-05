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

/* CLass table shit by smoo */
#define CLASS_DEMON      1
#define CLASS_MAGE       2
#define CLASS_WEREWOLF	 4
#define CLASS_VAMPIRE	 8
#define CLASS_HIGHLANDER 16
#define CLASS_DROW	 32
#define CLASS_MONK	 64
#define CLASS_NINJA	 128
#define CLASS_AMAZON	 256
#define CLASS_OBJECTOR   512
#define CLASS_HYDRA	 1024
#define CLASS_JEDI	 2048
#define CLASS_SHAPE	 4096
  #define CLASS_SHAPESHIFTER CLASS_SHAPE
#define CLASS_GOLEM	 8192
#define CLASS_DRUID	 16384
#define HYBRID		 32768 // defines a tier 3 class.
  #define HYB		 HYBRID
#define CLASS_UNICORN	 65536
#define CLASS_FLUFF	 131072
#define CLASS_WHATEV     262144
//524288
#define CLASS_MOOGLE     1048576
int class_lookup	( int class );
int class_lookupn	( char * class );

// HYBRIDS - Make sure you put in HYBRID as one of the classes
#define CLASS_KNIGHT    (CLASS_SHAPE|CLASS_DRUID|HYBRID)
#define CLASS_LICH	(CLASS_DROW|CLASS_MAGE|HYBRID)
#define CLASS_SAMURAI	(CLASS_NINJA|CLASS_MONK|HYBRID)
#define CLASS_SUCCUBUS	(CLASS_AMAZON|CLASS_DEMON|HYBRID)
#define CLASS_GARGOYLE	(CLASS_GOLEM|CLASS_MAGE|HYBRID)
#define CLASS_MOUNTED	(CLASS_HIGHLANDER|CLASS_UNICORN|HYBRID)
#define CLASS_GAROU	(CLASS_VAMPIRE|CLASS_WEREWOLF|HYBRID)
#define CLASS_SITH	(CLASS_GOLEM|CLASS_JEDI|HYBRID)


extern  const   struct  strctr_class_table    class_table [];
struct strctr_class_table
{
    long         class;
    char   *    class_name;
    int         start_trust;
    int         start_level;
    int		start_gen;
    int         bits; // BIT_PTEST_SAFE, etc
}; 

#define BIT_PTEST_NORMAL	1	//class in ptesting,not safe
#define BIT_PTEST_SAFE		2	//class in ptesting,safe from pk
#define BIT_PTEST_TOGGLE	4	//class in ptesting,players can
					//toggle if their safe or not
#define BIT_DONTCLASS		8	//unable to class /w class command
#define BIT_CLOSED		16	//unable to class /w autoclass
					//autoclass is in smoo.c
#define BIT_HIDDEN		32	//doesn't show in do_Class/autoclass
#define BIT_NOWHO		64	//class doesn't show on who list


#define IC_BIT_NUMB(class,bit)	(class_table[class_lookup(class)].bits & \
				bit)
#define IC_BIT_NAME(class,bit)	(class_table[class_lookupn(class)].bits &\
				bit)


#define IS_PTEST_SAFE(ch) (ch->flag3 & AFF3_PTEST_SAFE||IC_BIT_NUMB(ch->class, BIT_PTEST_SAFE))
#define SET_PTEST_SAFE(ch) (SET_BIT(ch->flag3, AFF3_PTEST_SAFE))
#define REM_PTEST_SAFE(ch) (REMOVE_BIT(ch->flag3, AFF3_PTEST_SAFE))
DECLARE_DO_FUN(do_ptesttoggle);
