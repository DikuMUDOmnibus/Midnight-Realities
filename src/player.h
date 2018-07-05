
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

/* Special Bits */







#define SPC_CHAMPION	1 /* PLR_CHAMPION 4 */



#define SPC_DEMON_LORD	2 /* OLD_DEMON Flag */



#define SPC_WOLFMAN	4 /* old PLR_WOLFMAN */



#define SPC_PRINCE      8 /* old EXTRA_PRINCE */



#define SPC_SIRE	16 /* Old EXTRA_SIRE */



#define SPC_ANARCH      32 /* old extra_anarch */



#define SPC_INCONNU     64 /* old extra_inconnu */



#define SPC_DROW_WAR	128



#define SPC_DROW_MAG	256



#define SPC_DROW_CLE	512



#define SPC_ROGUE       1024

#define SPC_JEDI_FSHIELD	2048
#define SPC_JEDI_LEVITATE	4096
#define SPC_JEDI_QUICKEN	8192
#define SPC_GOLEMBIND	       16384
/* Class Bits */
/* PUT CLASS DEFINITIONS IN class.h AND class.c */

/* 



 * Bits For Highlanders



 */



#define HPOWER_WPNSKILL 0







/* 



 * Bits For Mages



 */



#define MPOWER_RUNE0	0



#define MPOWER_RUNE1	1



#define MPOWER_RUNE2	2



#define MPOWER_RUNE3	3



#define MPOWER_RUNE4	4



/*start jedi section*/

#define JPOWER_BODY			0
#define JPOWER_MIND			1
#define JPOWER_SPIRIT		2
#define JPOWER_TECHNIQUE	3
#define JAPP_TAKEN			4
#define JAPP_MASTERED		5
#define JEDI_FOCUS			6
#define JEDI_STATE			7
#define JEDI_FOCUS_MAX		8
#define	JEDI_STATE_MAX		9
#define JEDI_COMBO_CURR		11
#define JEDI_TP_COMBO		12
/*end jedi secton*/





#define NPOWER_SORA 1



#define NPOWER_CHIKYU 2



#define NPOWER_NINGENNO 3
#define NPOWER_HAMASHAKA 4
#define NPOWER_BITS 5


#define NBIT_SHIELD 1
#define NBIT_REDIRECT 2
#define NBIT_ROUND 4
#define nbit(ch) (ch)->pcdata->powers[NPOWER_BITS]



/*



 * Bits for Werewolves 



 */



/* Stats */







#define UNI_GEN        	0 /* vampgen */



#define UNI_AFF		1 /* vampaff */



#define UNI_CURRENT	2 /* vamppass */



#define UNI_RAGE	3 /* ch->wolf = how big rage they are in*/



#define UNI_FORM0      	4/* wolfform[0] */



#define UNI_FORM1      	5 /* wolfform[1] */







#define	WOLF_POLYAFF	6





/* 
totems were taken out,these define mean nothing but leave them in for compatibility. the pcdata_>powers structure is taken up by the WWPOW_line of defines
 * Powers 

 * Totems for werewolves.

 */



#define WPOWER_MANTIS	      0



#define WPOWER_BEAR	      1



#define WPOWER_LYNX	      2



#define WPOWER_BOAR	      3



#define WPOWER_OWL	      4



#define WPOWER_SPIDER	      5



#define WPOWER_WOLF	      6



#define WPOWER_HAWK	      7



#define WPOWER_RAPTOR         8



#define WPOWER_LUNA           9



#define WPOWER_SILVER	     10



#define WPOWER_PAIN          11



#define WPOWER_CONGREGATION  12



#define VPOWER_ANIMAL         1



#define VPOWER_AUSPEX         2



#define VPOWER_CELERITY       3 



#define VPOWER_DOMINATE       4



#define VPOWER_FORTITUDE      5



#define VPOWER_OBFUSCATE      6



#define VPOWER_OBTENEBRATION  7



#define VPOWER_POTENCE        8



#define VPOWER_PRESENCE       9



#define VPOWER_PROTEAN       10



#define VPOWER_QUIETUS       11



#define VPOWER_SERPENTIS     12



#define VPOWER_THAU          13

#define VPOWER_VICISSITUDE   14

#define VPOWER_DAIM          15

#define VPOWER_NECROMANCY    16

#define VPOWER_THANATOSIS    17

#define VPOWER_MELPOMINEE    18

#define VPOWER_OBJ_VNUM      19






#define DROW_POWER	8



#define DROW_TOTAL	9



#define DROW_MAGIC	11





/*



 * Bits for Demonic Champions.



 */







/*Stats*/







#define DEMON_CURRENT		      8	/* POWER_CURRENT 0*/



#define DEMON_TOTAL		      9	/* POWER_TOTAL   1*/



#define DEMON_POWER		     10 /* TOTAL ARMOUR BOUNS */



#define DEMON_PPOWER		     11 /* ARMOUR BONUS FROM PLAYERS */



/*Powers*/



#define DPOWER_FLAGS		      0 /* C_POWERS  1 */



#define DPOWER_CURRENT		      1 /* C_CURRENT 2 */



#define DPOWER_HEAD		      2 /* C_HEAD    3 */



#define DPOWER_TAIL		      3 /* C_TAIL    4 */



#define DPOWER_OBJ_VNUM		      4 /* DISC[10]  5 */



#define VPOWER_CURRENT		     18







#define DEM_UNFOLDED		      1







#define DEM_FANGS		      1



#define DEM_CLAWS		      2



#define DEM_DAMN		      3



#define DEM_HORNS		      4



#define DEM_HOOVES		      8



#define DEM_EYES		     16



#define DEM_WINGS		     32



#define DEM_MIGHT		     64



#define DEM_TOUGH		    128



#define DEM_SPEED		    256



#define DEM_TRAVEL		    512



#define DEM_SCRY		   1024



#define DEM_SHADOWSIGHT		   2048



/* Object Powers */



#define DEM_MOVE		   4096   /* 1 Can roll as an object */



#define DEM_LEAP		   8192   /* 2 Can leap out of someone's hands */



#define DEM_MAGIC		  16384   /* 4 Can cast spells as an object */



#define DEM_LIFESPAN		  32768   /* 8 Can change lifespan */







#define DEM_EXOSKELETON		     



#define DEM_HEAD		 131072



#define DEM_TAIL		 262144



#define DEM_SHIELD		 524288



#define DEM_TRUESIGHT		1048576







#define HEAD_NORMAL		      0



#define HEAD_WOLF		      1



#define HEAD_EAGLE		      2



#define HEAD_LION		      4



#define HEAD_SNAKE		      8







#define TAIL_NONE		      0



#define TAIL_POISONOUS		      1



#define TAIL_SCORPION		      2







/* Drow Powers */



#define DPOWER_DROWFIRE		1

#define DPOWER_DARKNESS		2

#define DPOWER_LIGHTS		4

#define DPOWER_DROWSIGHT	8

#define DPOWER_LEVITATION	16

#define DPOWER_DROWSHIELD	32

#define DPOWER_DROWPOISON	64

#define DPOWER_SHADOWWALK	128

#define DPOWER_GAROTTE		256

#define DPOWER_ARMS		512

#define DPOWER_DROWHATE		1024

#define DPOWER_SPIDERFORM	2048

#define DPOWER_WEB		4096

#define DPOWER_DGAROTTE		8192

#define DPOWER_CIRCLE		16384





/*new demon powers*/



#define DEM_CONE		1



#define DEM_STRENGTH		2



#define DEM_FORM		3



#define DEM_STAKE		4



#define DEM_SPEEDY		5



#define DEM_TRUE		6



/* Monk Power Bit */



#define PMONK			0
/* Amazon power bit, i hope! */
#define PAMAZON			0
#define AMA_COMBO		1


/* shapeshifter powers..smirk */
#define dopplevnum		0
#define spectoggle		1
#define doppleweapon		2
#define warcry			3
#define autodecap		4
#define beeping			5
#define slaiyth			6
#define IS_SLAIYTH(ch) (IS_CLASS(ch, CLASS_SHAPE) && !IS_NPC(ch) && (ch)->pcdata->powers[slaiyth] != 0)
#define SET_SLAIYTH(ch) ((ch)->pcdata->powers[slaiyth] = 1)
#define REM_SLAIYTH(ch) ((ch)->pcdata->powers[slaiyth] = 0)
#define REMOVE_SLAIYTH REM_SLAIYTH

// druid powers
#define DRUID_POWER		0
// damn that was a lot of powers..hurt my fingers typing them out

/*Druid Rewards(Warps)*/
#define REWARD_SILVERARMOR	1 	//toughness
#define REWARD_SOFTBODY		2 	//weaker
#define REWARD_SWIFTNESS	4 	//speed
#define REWARD_SLOW		8 	//duh
#define REWARD_INFEST		16	//infest spell casted on all in room
#define REWARD_MAGICAL		32	//improved spellcasting ability
#define REWARD_PEACEFUL		64	//reduced damage and reduced combat
#define REWARD_DEADLYSTAFF	128	//improved ability with shillelagh wpn
#define REWARD_VULNERABLE	256	//ouch
#define REWARD_RESTRICTED	512	//druid powers weakened
#define REWARD_SPACE		1024	//a USELESS warp
#define REWARD_IRONLEGS		2048	//a celestial path type ability
#define REWARD_WEAPONSMASTER	4096	//weapon levels to 250
#define REWARD_RICH		8192	//less druid points
#define REWARD_CLUMSY		16384	//decreased combat ability
#define REWARD_UNALIGN		32768	//Increased damage, lowered resist
#define REWARD_THORNS		65536	//extra thorn attack /round
#define REWARD_WARTS		131072	//scares people!
#define REWARD_HELPFUL		262144	//casts dragon/angel breath on peeps
					//in the same room (constantly)
#define REWARD_WELFARE		524288	//double druid points
#define REWARD_CHEF		1048576 //improved berry
#define REWARD_REJECT		2097152 //no communication with other druids
#define REWARD_LONELY		4194304 //spy leaves you where you scry

#define IS_WARP(ch, bit) (((ch)->warp & (bit)))

/*Werewolf defines for ->pcdata->powers */
#define WWPOW_ADRENORUSH 0
