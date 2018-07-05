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

#if defined(macintosh)

#include <types.h>

#else

#include <sys/types.h>

#endif

#include <ctype.h>

#include <stdio.h>

#include <stdlib.h>

#include <dirent.h>

#include <string.h>

#include <time.h>

#include "merc.h"

#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

#if !defined(macintosh)

extern	int	_filbuf		args( (FILE *) );

#endif
int first_repop = TRUE;
/*

 * Globals.

 */

HELP_DATA *		help_first;

HELP_DATA *		help_last;



SHOP_DATA *		shop_first;

SHOP_DATA *		shop_last;


bool nodamcap = FALSE;

CHAR_DATA *		char_free;

EXTRA_DESCR_DATA *	extra_descr_free;

NOTE_DATA *		note_free;

OBJ_DATA *		obj_free;

PC_DATA *		pcdata_free;
DECLARE_SPEC_FUN(spec_kingdom_guard);
void load_changes();//do_changes command data

char			bug_buf		[2*MAX_INPUT_LENGTH];

CHAR_DATA *		char_list;

char *			help_greeting;

char			log_buf		[2*MAX_INPUT_LENGTH];

KILL_DATA		kill_table	[MAX_LEVEL];

NOTE_DATA *		note_list;

OBJ_DATA *		object_list;

TIME_INFO_DATA		time_info;

WEATHER_DATA		weather_info;

sh_int			gsn_shiroken;
sh_int			gsn_thorns;
sh_int			gsn_backstab;
sh_int			gsn_circle;
sh_int                  gsn_shred;
/*mecha*/
sh_int			gsn_burst;

sh_int                  gsn_quills;

sh_int 			gsn_cheapshot;

sh_int                  gsn_spit;

sh_int			gsn_venomtong;

sh_int			gsn_spiketail;

sh_int			gsn_badbreath;

sh_int			gsn_magma;

sh_int			gsn_shards;

sh_int			gsn_hide;

sh_int			gsn_peek;

sh_int			gsn_pick_lock;

sh_int			gsn_sneak;

sh_int			gsn_steal;

sh_int			gsn_spiderform;

sh_int			gsn_garotte;

sh_int			gsn_disarm;

sh_int			gsn_tendrils;

sh_int			gsn_berserk;

sh_int			gsn_punch;

sh_int			gsn_elbow;

sh_int			gsn_headbutt;

sh_int			gsn_inferno;
sh_int			gsn_blinky;
sh_int			gsn_fangs;

sh_int			gsn_buffet;

sh_int			gsn_sweep;

sh_int			gsn_knee;

sh_int                  gsn_rfangs;

sh_int			gsn_kick;

sh_int			gsn_hurl;

sh_int			gsn_rescue;

sh_int			gsn_track;

sh_int			gsn_polymorph;

sh_int			gsn_web;

sh_int			gsn_drowfire;

sh_int                  gsn_infirmity;

sh_int                  gsn_spew;

sh_int			gsn_blindness;

sh_int			gsn_charm_person;

sh_int			gsn_curse;

sh_int			gsn_invis;

sh_int			gsn_mass_invis;

sh_int			gsn_poison;

sh_int			gsn_sleep;

sh_int			gsn_haste;

sh_int			gsn_darkness;

sh_int			gsn_paradox;

GLOB_DATA	*	global_list = NULL;





char datafile[1000];
char workfile[1000];

void stripfilename(char*,char*);

/*

 * Locals.

 */

MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];

OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];

ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];

char *			string_hash		[MAX_KEY_HASH];



AREA_DATA *		area_first;

AREA_DATA *		area_last;


CONFIG_DATA *		config_info;
ROOM_INDEX_DATA *	room_list;

HELP_DATA *		first_help;
HELP_DATA *		last_help;



char *			string_space;

char *			top_string;

char			str_empty	[1];



int			top_affect;

int			top_area;

int			top_rt;

int			top_ed;

int			top_exit;

int			top_help;

int			top_mob_index;

int			top_obj_index;

int			top_reset;

int			top_room;

int			top_shop;







/*

 * Memory management.

 * Increase MAX_STRING if you have too.

 * Tune the others only if you understand what you're doing.



#define			MAX_STRING	1048576



 */



#define			MAX_STRING	2097152

#define			MAX_PERM_BLOCK	131072

#define			MAX_MEM_LIST	11



void *			rgFreeList	[MAX_MEM_LIST];

const int		rgSizeList	[MAX_MEM_LIST]	=

{

    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64

};



int			nAllocString;

int			sAllocString;

int			nAllocPerm;

int			sAllocPerm;







/*

 * Semi-locals.

 */

bool			fBootDb;

FILE *			fpArea;

char			strArea[MAX_INPUT_LENGTH];







/*

 * Local booting procedures.

 */

void	init_mm		args( ( void ) );

void	read_data_recurse_dir (const char *);

void	load_area	args( ( FILE *fp ) );

void    load_helps      args( ( FILE *fp ) );
void	load_mobiles	args( ( FILE *fp ) );

void	load_objects	args( ( FILE *fp ) );

void	load_resets	args( ( FILE *fp ) );

void	load_rooms	args( ( FILE *fp ) );

void	load_shops	args( ( FILE *fp ) );

void	load_specials	args( ( FILE *fp ) );

void	load_notes	args( ( void ) );

void	read_datafile	args( ( const char * ) );

void	fix_exits	args( ( void ) );
void	check_resets	args( ( void ) );
void	check_shops	args( ( void ) );


void	reset_area	args( ( AREA_DATA * pArea ) );







/*

 * Big mama top level function.

 */

void boot_db( void )

{
xpdiv=1;
    /*

     * Init some data space stuff.

     */

    {

	if ( ( string_space = calloc( 1, MAX_STRING ) ) == NULL )

	{

	    bug( "Boot_db: can't alloc d string space.", MAX_STRING );
	    pollDone(Failed);
	    exit( 1 );

	}

	top_string	= string_space;

	fBootDb		= TRUE;

    }

    /*

     * Init random number generator.

     */

    {

	init_mm( );

    }

    load_bootfile();

    /*

     * Set time and weather.

     */

    {

	long lhour, lday, lmonth;



	lhour		= (current_time - 650336715)

			/ (PULSE_TICK / PULSE_PER_SECOND);

	time_info.hour	= lhour %  24;
	
	lday		= lhour  / 24;

	time_info.day	= lday  %  35;

	lmonth		= lday   / 35;

	time_info.month	= lmonth % 17;

	time_info.year	= lmonth / 17;

	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;

	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;

	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;

	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;

	else                            weather_info.sunlight = SUN_DARK;



	weather_info.change	= 0;

	weather_info.mmhg	= 960;

	if ( time_info.month >= 7 && time_info.month <=12 )

	    weather_info.mmhg += number_range( 1, 50 );

	else

	    weather_info.mmhg += number_range( 1, 80 );



	     if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;

	else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;

	else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;

	else                                  weather_info.sky = SKY_CLOUDLESS;



    }



    /*

     * Assign gsn's for skills which have them.

     */

    {

	int sn;


	    last_skill = 0;

	for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name; sn++ )

	{

	    if ( skill_table[sn].pgsn != NULL )
		*skill_table[sn].pgsn = sn;

	    last_skill++;

	}

    }



    /*

     * Read in all the area files.

     */

    {

	DIR *dir;
	struct dirent *fil;
	char cCh;

	dir = opendir(global("areadir"));
	if (!dir)
	{
	    sprintf(log_buf,
		"Unable to open 'areadir' directory: %s. Exiting", 
		global("areadir"));
	    log_string(log_buf);
	    pollDone(Failed);
	    exit(1);
	}

	for ( fil = readdir(dir); fil; fil = readdir(dir))
	{

	    if (str_suffix(".are", fil->d_name)) continue;

	    strcpy( strArea, global("areadir"));
	    strcat( strArea, "/");
	    strcat( strArea, fil->d_name );

		if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )

		{

		    perror( strArea );

		    pollDone(Failed);
		    exit( 1 );

		}

	    while ((cCh = getc(fpArea)) == '\n' || cCh == '\r' || cCh == ' ')
		;

	    ungetc(cCh, fpArea);

	    if (cCh == EOF)    {fclose(fpArea);continue;}

	    if (str_cmp(fread_word(fpArea), "register")) {fclose(fpArea);continue;}

	    log_string(strArea);

	    for ( ; ; )

	    {

		char *word;



		if ( fread_letter( fpArea ) != '#' )

		{

		    bug( "Boot_db: # not found.", 0 );

		    pollDone(Failed);
		    exit( 1 );

		}



		word = fread_word( fpArea );

		     if ( word[0] == '$'               )                 break;

		else if ( !str_cmp( word, "AREA"     ) ) load_area    (fpArea);

		else if ( !str_cmp( word, "HELPS"    ) ) load_helps   (fpArea);

		else if ( !str_cmp( word, "MOBILES"  ) ) load_mobiles (fpArea);

		else if ( !str_cmp( word, "OBJECTS"  ) ) load_objects (fpArea);

		else if ( !str_cmp( word, "RESETS"   ) ) load_resets  (fpArea);

		else if ( !str_cmp( word, "ROOMS"    ) ) load_rooms   (fpArea);

		else if ( !str_cmp( word, "SHOPS"    ) ) load_shops   (fpArea);

		else if ( !str_cmp( word, "SPECIALS" ) ) load_specials(fpArea);

		else

		{

		    bug( "Boot_db: bad section name.", 0 );

		    pollDone(Failed);
		    exit( 1 );

		}

	    }



	    if ( fpArea != stdin )

		fclose( fpArea );

	    fpArea = NULL;

	}

    closedir(dir);
    }


	fBootDb	= FALSE;

    /*

     * Fix up exits.

     * Declare db booting over.

     * Reset all areas once.

     * Load up the notes file.

     */

    {
log_string("exits");
	fix_exits( );
log_string("exits done");

log_string("resets");
	check_resets();
log_string("resets done");

log_string("shops");
	check_shops();
log_string("shops done");


	area_update( );
log_string("bans");
	load_bans();
log_string("bans done");

log_string("notes");
	load_notes( );
log_string("notes done");
log_string("disabled");
	load_disabled();
log_string("disabled done");
log_string("topscores");
    if (!pwipeit)
	load_topscores();
log_string("topscores done");
log_string("init_history");
	init_history();
log_string("init_history done");
log_string("load_kingdoms");
	load_kingdoms();
log_string("load_kingdoms done");
    }



    /* 

     *  Rotain's Clan Table Read Settings

     */

 

/*    clan_table_read();

    artifact_table_read();*/
log_string("classconfig");
load_class_configs();
log_string("classconfig done");
log_string("changes data");
load_changes();
log_string("changes data done");
    if (pwipeit) {
        pwipe();
        printf("Pwiped. Delete the files in ../pwipe to complete.\n");
        exit(1);}
    return;

}







/*

 * Snarf an 'area' header line.

 */

void load_area( FILE *fp )

{

    AREA_DATA *pArea;



    pArea		= alloc_perm( sizeof(*pArea) );

    pArea->reset_first	= NULL;

    pArea->reset_last	= NULL;

    pArea->name		= fread_string( fp );

    pArea->age		= 15;

    pArea->nplayer	= 0;



    if ( area_first == NULL )

	area_first = pArea;

    if ( area_last  != NULL )

	area_last->next = pArea;

    area_last	= pArea;

    pArea->next	= NULL;



    top_area++;

    return;

}









/*
 * Returns an uppercase string.
 */
char *strupper( const char *str )
{
    static char strup[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strup[i] = UPPER(str[i]);
    strup[i] = '\0';
    return strup;
}


/*
 * Adds a help page to the list if it is not a duplicate of an existing page.
 * Page is insert-sorted by keyword.			-Thoric
 * (The reason for sorting is to keep do_hlist looking nice)
 */
void add_help( HELP_DATA *pHelp )
{
    HELP_DATA *tHelp;
/* Not in use, I'm guessing this function is not working.
    char buf[MAX_STRING_LENGTH];
*/
    int match;

    for ( tHelp = first_help; tHelp; tHelp = tHelp->next )
/*
	if ( pHelp->level == tHelp->level
	&&  !strcmp(pHelp->keyword, tHelp->keyword) )
	{
	    sprintf(buf, "Duplicate %s. Deleting Help.\n\r",pHelp->keyword);
	    bug(buf,0);
	    STRFREE( pHelp->text );
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    return;
	}
	else
*/
	if ( (match=strcmp(pHelp->keyword[0]=='\'' ? pHelp->keyword+1 : pHelp->keyword,
			   tHelp->keyword[0]=='\'' ? tHelp->keyword+1 : tHelp->keyword)) < 0
	||   (match == 0 && pHelp->level > tHelp->level) )
	{
	    if ( !tHelp->prev )
		first_help	  = pHelp;
	    else
		tHelp->prev->next = pHelp;
	    pHelp->prev		  = tHelp->prev;
	    pHelp->next		  = tHelp;
	    tHelp->prev		  = pHelp;
	    break;
	}

    if ( !tHelp )
	LINK( pHelp, first_help, last_help, next, prev );

    top_help++;
}


/*
 * Load a help section.
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;
    for ( ; ; )
    {
	CREATE( pHelp, HELP_DATA, 1 );
	pHelp->level	= fread_number( fp );
	pHelp->keyword	= fread_string( fp );
	if ( pHelp->keyword[0] == '$' )
	    break;
	pHelp->text	= fread_string( fp );
	if ( pHelp->keyword[0] == '\0' )
	{
	    STRFREE( pHelp->text );
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    continue;
	}

	if ( !str_cmp( pHelp->keyword, "greeting" ) )
	    help_greeting = pHelp->text;
	add_help( pHelp );
    }
    return;
}


/*

 * Snarf a mob section.

 */

void load_mobiles( FILE *fp )

{

    MOB_INDEX_DATA *pMobIndex;



    for ( ; ; )

    {

	int vnum;

	char letter;

	int iHash;



	letter				= fread_letter( fp );

	if ( letter != '#' )

	{

	    bug( "Load_mobiles: # not found.", 0 );
	    pollDone(Failed);

	    exit( 1 );

	}



	vnum				= fread_number( fp );

	if ( vnum == 0 )

	    break;



	fBootDb = FALSE;

	if ( get_mob_index( vnum ) != NULL )

	{

	    bug( "Load_mobiles: vnum d duplicated.", vnum );

	    pollDone(Failed);
	    exit( 1 );

	}

	fBootDb = TRUE;



	pMobIndex			= alloc_perm( sizeof(*pMobIndex) );

	pMobIndex->vnum			= vnum;

	pMobIndex->kingdom		= str_dup("");

	pMobIndex->player_name		= fread_string( fp );

	pMobIndex->short_descr		= fread_string( fp );

	pMobIndex->long_descr		= fread_string( fp );

	pMobIndex->description		= fread_string( fp );



	pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);

	pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);



	pMobIndex->act			= fread_number( fp ) | ACT_IS_NPC;

	pMobIndex->affected_by		= fread_number( fp );

	pMobIndex->itemaffect		= 0;

	pMobIndex->pShop		= NULL;

	pMobIndex->alignment		= fread_number( fp );

	letter				= fread_letter( fp );

	pMobIndex->level		= number_fuzzy( fread_number( fp ) );



	/*

	 * The unused stuff is for imps who want to use the old-style

	 * stats-in-files method.

	 */

	pMobIndex->hitroll		= fread_number( fp );	/* Unused */

	pMobIndex->ac			= fread_number( fp );	/* Unused */

	pMobIndex->hitnodice		= fread_number( fp );	/* Unused */

	/* 'd'		*/		  fread_letter( fp );	/* Unused */

	pMobIndex->hitsizedice		= fread_number( fp );	/* Unused */

	/* '+'		*/		  fread_letter( fp );	/* Unused */

	pMobIndex->hitplus		= fread_number( fp );	/* Unused */

	pMobIndex->damnodice		= fread_number( fp );	/* Unused */

	/* 'd'		*/		  fread_letter( fp );	/* Unused */

	pMobIndex->damsizedice		= fread_number( fp );	/* Unused */

	/* '+'		*/		  fread_letter( fp );	/* Unused */

	pMobIndex->damplus		= fread_number( fp );	/* Unused */

	pMobIndex->gold			= fread_number( fp );	/* Unused */

	/* xp can't be used! */		  fread_number( fp );	/* Unused */

	/* position	*/		  fread_number( fp );	/* Unused */

	/* start pos	*/		  fread_number( fp );	/* Unused */



	/*

	 * Back to meaningful values.

	 */

	pMobIndex->sex			= fread_number( fp );



	if ( letter != 'S' )

	{

	    bug( "Load_mobiles: vnum d non-S.", vnum );
	    pollDone(Failed);

	    exit( 1 );

	}
pMobIndex->hp = 0;pMobIndex->mv = 0;pMobIndex->mn = 0;
letter = 0;
while (letter != '#')
{
while ((letter = getc(fp)) == '\n' || letter == '\r' || letter==' ') ;
if (letter=='#')
{    ungetc(letter,fp);break;}
else if (letter=='x' || letter=='X')
{pMobIndex->hp = fread_number(fp);pMobIndex->mn = fread_number(fp);pMobIndex->mv = fread_number(fp);
}
else if (letter == 'K' || letter == 'k')
{
    free_string(pMobIndex->kingdom);
    pMobIndex->kingdom = fread_string(fp);
}
else {sprintf(log_buf, "||%c||", letter);log_string(log_buf);	    pollDone(Failed);
exit(1);}
}
	iHash			= vnum % MAX_KEY_HASH;

	pMobIndex->next		= mob_index_hash[iHash];

	mob_index_hash[iHash]	= pMobIndex;

	top_mob_index++;

	kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;

    }



    return;

}







/*

 * Snarf an obj section.

 */

void load_objects( FILE *fp )

{

    OBJ_INDEX_DATA *pObjIndex;



    for ( ; ; )

    {

	int vnum;

	char letter;

	int iHash;



	letter				= fread_letter( fp );

	if ( letter != '#' )

	{

	    bug( "Load_objects: # not found.", 0 );

	    pollDone(Failed);
	    exit( 1 );

	}



	vnum				= fread_number( fp );

	if ( vnum == 0 )

	    break;



	fBootDb = FALSE;

	if ( get_obj_index( vnum ) != NULL )

	{

	    bug( "Load_objects: vnum d duplicated.", vnum );
	    pollDone(Failed);

	    exit( 1 );

	}

	fBootDb = TRUE;



	pObjIndex			= alloc_perm( sizeof(*pObjIndex) );

	pObjIndex->vnum			= vnum;

	pObjIndex->name			= fread_string( fp );

	pObjIndex->short_descr		= fread_string( fp );

	pObjIndex->description		= fread_string( fp );

	/* Action description */	  fread_string( fp );



	pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);

	pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);


	pObjIndex->area		= area_last;

	pObjIndex->item_type		= fread_number( fp );

	pObjIndex->extra_flags		= fread_number( fp );

	pObjIndex->wear_flags		= fread_number( fp );

	pObjIndex->value[0]		= fread_number( fp );

	pObjIndex->value[1]		= fread_number( fp );

	pObjIndex->value[2]		= fread_number( fp );

	pObjIndex->value[3]		= fread_number( fp );

	pObjIndex->weight		= fread_number( fp );

	pObjIndex->cost			= fread_number( fp );	/* Unused */

	pObjIndex->affected		= NULL;

	pObjIndex->extra_descr		= NULL;

	pObjIndex->chpoweron		= NULL;

	pObjIndex->chpoweroff		= NULL;

	pObjIndex->chpoweruse		= NULL;

	pObjIndex->victpoweron		= NULL;

	pObjIndex->victpoweroff		= NULL;

	pObjIndex->victpoweruse		= NULL;

	pObjIndex->spectype		= 0;

	pObjIndex->specpower		= 0;

	/* Cost per day */		  fread_number( fp );

/*

	if ( pObjIndex->item_type == ITEM_POTION )

	    SET_BIT(pObjIndex->extra_flags, ITEM_NODROP);

*/

	for ( ; ; )

	{

	    char letter;



	    letter = fread_letter( fp );



	    if ( letter == 'A' )

	    {

		AFFECT_DATA *paf;



		paf			= alloc_perm( sizeof(*paf) );

		paf->type		= -1;

		paf->duration		= -1;

		paf->location		= fread_number( fp );

		paf->modifier		= fread_number( fp );

		paf->bitvector		= 0;

		paf->next		= pObjIndex->affected;

		pObjIndex->affected	= paf;

		top_affect++;

	    }
	    else if (LOWER(letter)=='x')
		pObjIndex->ClassBit = fread_number(fp);

	    else if ( letter == 'E' )

	    {

		EXTRA_DESCR_DATA *ed;



		ed			= alloc_perm( sizeof(*ed) );

		ed->keyword		= fread_string( fp );

		ed->description		= fread_string( fp );

		ed->next		= pObjIndex->extra_descr;

		pObjIndex->extra_descr	= ed;

		top_ed++;

	    }



	    else if ( letter == 'Q' )

	    {

		pObjIndex->chpoweron	= fread_string( fp );

		pObjIndex->chpoweroff	= fread_string( fp );

		pObjIndex->chpoweruse	= fread_string( fp );

		pObjIndex->victpoweron	= fread_string( fp );

		pObjIndex->victpoweroff	= fread_string( fp );

		pObjIndex->victpoweruse	= fread_string( fp );

		pObjIndex->spectype	= fread_number( fp );

		pObjIndex->specpower	= fread_number( fp );

	    }



	    else

	    {

		ungetc( letter, fp );

		break;

	    }

	}



	/*

	 * Translate spell "slot numbers" to internal "skill numbers."

	 */

	switch ( pObjIndex->item_type )

	{

	case ITEM_PILL:

	case ITEM_POTION:

	case ITEM_SCROLL:

	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );

	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );

	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );

	    break;



	case ITEM_STAFF:

	case ITEM_WAND:

	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );

	    break;

	}



	iHash			= vnum % MAX_KEY_HASH;

	pObjIndex->next		= obj_index_hash[iHash];

	obj_index_hash[iHash]	= pObjIndex;

	top_obj_index++;

    }



    return;

}







/*

 * Snarf a reset section.

 */

void load_resets( FILE *fp )

{

    RESET_DATA *pReset;



    if ( area_last == NULL )

    {

	bug( "Load_resets: no #AREA seen yet.", 0 );
	    pollDone(Failed);

	exit( 1 );

    }



    for ( ; ; )

    {

	char letter;



	if ( ( letter = fread_letter( fp ) ) == 'S' )

	    break;



	if ( letter == '*' )

	{

	    fread_to_eol( fp );

	    continue;

	}



	pReset		= alloc_perm( sizeof(*pReset) );

	pReset->command	= letter;

	/* if_flag */	  fread_number( fp );

	pReset->arg1	= fread_number( fp );

	pReset->arg2	= fread_number( fp );
if (pReset->command == 'M' && !str_suffix("desolution.are", strArea))
   if (pReset->arg2 > 1) pReset->arg2 *=2;
	pReset->arg3	= (letter == 'G' || letter == 'R')

			    ? 0 : fread_number( fp );

			  fread_to_eol( fp );

if (letter=='M')
{
    if (pReset->arg1>=11310&&pReset->arg1<11315)
	pReset->arg2 *= 5;
}

	if ( area_last->reset_first == NULL )

	    area_last->reset_first	= pReset;

	if ( area_last->reset_last  != NULL )

	    area_last->reset_last->next	= pReset;

	    

	area_last->reset_last	= pReset;

	pReset->next		= NULL;

	top_reset++;

    }



    return;

}


void check_resets(void)
{
  RESET_DATA *pReset;
  AREA_DATA *are;


  for (are = area_first; are; are = are->next)
    for (pReset = are->reset_first; pReset; pReset = pReset->next)
    {
	ROOM_INDEX_DATA *pRoomIndex;
	EXIT_DATA *pexit;

	/*

	 * Validate parameters.

	 * We're calling the index functions for the side effect.

	 */

	switch ( pReset->command )

	{

	default:

	    bug( "Load_resets: bad command 'c'.", pReset->command );

	    pollDone(Failed);
	    exit( 1 );

	    break;



	case 'M':
	    get_mob_index  ( pReset->arg1 );

	    get_room_index ( pReset->arg3 );
	    break;



	case 'O':

	    get_obj_index  ( pReset->arg1 );

	    get_room_index ( pReset->arg3 );

	    break;



	case 'P':

	    get_obj_index  ( pReset->arg1 );

	    get_obj_index ( pReset->arg3 );

	    break;



	case 'G':

	case 'E':

	    get_obj_index  ( pReset->arg1 );
	    break;



	case 'D':

	    pRoomIndex = get_room_index( pReset->arg1 );


	    if ( pReset->arg2 < 0

	    ||   pReset->arg2 > 5

	    || ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL

	    || !IS_SET( pexit->exit_info, EX_ISDOOR ) )

	    {

		bug( "Load_resets: 'D': exit d not door.", pReset->arg2 );

	    pollDone(Failed);
		exit( 1 );

	    }



	    if ( pReset->arg3 < 0 || pReset->arg3 > 2 )

	    {

		bug( "Load_resets: 'D': bad 'locks': d.", pReset->arg3 );

	    pollDone(Failed);
		exit( 1 );

	    }



	    break;



	case 'R':

	    pRoomIndex		= get_room_index( pReset->arg1 );



	    if ( pReset->arg2 < 0 || pReset->arg2 > 6 )

	    {

		bug( "Load_resets: 'R': bad exit d.", pReset->arg2 );

	    pollDone(Failed);
		exit( 1 );

	    }



	    break;

	}
    }


}




/*

 * Snarf a room section.

 */

void load_rooms( FILE *fp )

{

    ROOM_INDEX_DATA *pRoomIndex;



    if ( area_last == NULL )

    {

	bug( "Load_resets: no #AREA seen yet.", 0 );

	    pollDone(Failed);
	exit( 1 );

    }



    for ( ; ; )

    {

	int vnum;

	char letter;

	int door;

	int iHash;



	letter				= fread_letter( fp );

	if ( letter != '#' )

	{

	    bug( "Load_rooms: # not found.", 0 );

	    pollDone(Failed);
	    exit( 1 );

	}



	vnum				= fread_number( fp );

	if ( vnum == 0 )

	    break;



	fBootDb = FALSE;

	if ( get_room_index( vnum ) != NULL )

	{

	    bug( "Load_rooms: vnum d duplicated.", vnum );

	    pollDone(Failed);
	    exit( 1 );

	}

	fBootDb = TRUE;



	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );

	pRoomIndex->people		= NULL;

	pRoomIndex->contents		= NULL;

	pRoomIndex->kingdom		= str_dup("");
	pRoomIndex->first_person	= NULL;
	pRoomIndex->last_person		= NULL;
	pRoomIndex->first_content	= NULL;
	pRoomIndex->last_content	= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->first_extradesc	= NULL;
	pRoomIndex->last_extradesc	= NULL;
	pRoomIndex->area		= area_last;
pRoomIndex->disabled = str_dup("");
pRoomIndex->disabletick = 0;
	pRoomIndex->vnum		= vnum;

	pRoomIndex->name		= fread_string( fp );

	pRoomIndex->description		= fread_string( fp );

	/* Area number */		  fread_number( fp );

	pRoomIndex->room_flags		= fread_number( fp );

	pRoomIndex->sector_type		= fread_number( fp );

	pRoomIndex->light		= 0;

	pRoomIndex->blood		= 0;

	pRoomIndex->roomtext		= NULL;

	pRoomIndex->first_exit		= NULL;
	pRoomIndex->last_exit		= NULL;
	for ( door = 0; door <= 4; door++ )

	{

	    pRoomIndex->track[door] 	= str_dup( "" );

	    pRoomIndex->track_dir[door] = 0;
	}
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;


	for ( ; ; )

	{

	    letter = fread_letter( fp );



	    if ( letter == 'S' )

		break;

	    if (letter == 'K')
	    {
		free_string(pRoomIndex->kingdom);
		pRoomIndex->kingdom = fread_string(fp);
	    }


	 else   if ( letter == 'D' )

	    {

		EXIT_DATA *pexit;

		int locks;



		door = fread_number( fp );

		if ( door < 0 || door > 5 )

		{

		    bug( "Fread_rooms: vnum d has bad door number.", vnum );

	    pollDone(Failed);
		    exit( 1 );

		}



		pexit			= alloc_perm( sizeof(*pexit) );

		pexit->description	= fread_string( fp );

		pexit->keyword		= fread_string( fp );

		pexit->exit_info	= 0;

		locks			= fread_number( fp );

		pexit->key		= fread_number( fp );

		pexit->vnum		= fread_number( fp );



		switch ( locks )

		{

		case 1: pexit->exit_info = EX_ISDOOR;                break;

		case 2: pexit->exit_info = EX_ISDOOR | EX_PICKPROOF; break;

		}



		pRoomIndex->exit[door]	= pexit;

		top_exit++;

	    }

	    else if ( letter == 'E' )

	    {

		EXTRA_DESCR_DATA *ed;



		ed			= alloc_perm( sizeof(*ed) );

		ed->keyword		= fread_string( fp );

		ed->description		= fread_string( fp );

		ed->next		= pRoomIndex->extra_descr;

		pRoomIndex->extra_descr	= ed;

		top_ed++;

	    }

	    else if ( letter == 'T' )

	    {

		ROOMTEXT_DATA *rt;



		rt			= alloc_perm( sizeof(*rt) );

		rt->input		= fread_string( fp );

		rt->output		= fread_string( fp );

		rt->choutput		= fread_string( fp );

		rt->name		= fread_string( fp );

		rt->type		= fread_number( fp );

		rt->power		= fread_number( fp );

		rt->mob			= fread_number( fp );

		rt->next		= pRoomIndex->roomtext;

		pRoomIndex->roomtext	= rt;

		top_rt++;

	    }

	    else

	    {

		bug( "Load_rooms: vnum d has flag not 'DES'.", vnum );

	    pollDone(Failed);
		exit( 1 );

	    }

	}



	iHash			= vnum % MAX_KEY_HASH;

	pRoomIndex->next	= room_index_hash[iHash];

	room_index_hash[iHash]	= pRoomIndex;

	pRoomIndex->next_room	= room_list;
	room_list		= pRoomIndex;
	top_room++;

    }



    return;

}







/*

 * Snarf a shop section.

 */

void load_shops( FILE *fp )

{

    SHOP_DATA *pShop;



    for ( ; ; )

    {

	int iTrade;



	pShop			= alloc_perm( sizeof(*pShop) );

	pShop->keeper		= fread_number( fp );

	if ( pShop->keeper == 0 )

	    break;

	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )

	    pShop->buy_type[iTrade]	= fread_number( fp );

	pShop->profit_buy	= fread_number( fp );

	pShop->profit_sell	= fread_number( fp );

	pShop->open_hour	= fread_number( fp );

	pShop->close_hour	= fread_number( fp );

				  fread_to_eol( fp );



	if ( shop_first == NULL )

	    shop_first = pShop;

	if ( shop_last  != NULL )

	    shop_last->next = pShop;



	shop_last	= pShop;

	pShop->next	= NULL;

	top_shop++;

    }



    return;

}


void check_shops(void)
{
  SHOP_DATA *pShop;

  for (pShop = shop_first; pShop; pShop = pShop->next)
 {
	MOB_INDEX_DATA *pMobIndex;
	pMobIndex		= get_mob_index( pShop->keeper );

	pMobIndex->pShop	= pShop;
 }
}



/*

 * Snarf spec proc declarations.

 */

void load_specials( FILE *fp )

{
char *t;
    for ( ; ; )

    {

	MOB_INDEX_DATA *pMobIndex;

	char letter;



	switch ( letter = fread_letter( fp ) )

	{

	default:

	    bug( "Load_specials: letter 'c' not *MS.", letter );

	    pollDone(Failed);
	    exit( 1 );



	case 'S':

	    return;



	case '*':

	    break;



	case 'M':

	    pMobIndex		= get_mob_index	( fread_number ( fp ) );

	    pMobIndex->spec_fun	= spec_lookup	( (t = fread_word   ( fp ) ));

	    if ( pMobIndex->spec_fun == 0 )

	    {

		sprintf(log_buf, "Load_specials: 'M': vnum %d. %s",
		pMobIndex->vnum, t);
		bug(log_buf, 0);

	    pollDone(Failed);
		exit( 1 );

	    }

	    break;

	}



	fread_to_eol( fp );

    }

}







/*

 * Snarf notes file.

 */

void load_notes( void )

{

    FILE *fp;

    NOTE_DATA *pnotelast;



    if ( ( fp = fopen( NOTE_FILE, "r" ) ) == NULL )

	return;



    pnotelast = NULL;

    for ( ; ; )

    {

	NOTE_DATA *pnote;

	char letter;



	do

	{

	    letter = getc( fp );

	    if ( feof(fp) )

	    {

		fclose( fp );

		return;

	    }

	}

	while ( isspace(letter) );

	ungetc( letter, fp );



	pnote		= alloc_perm( sizeof(*pnote) );



	if ( str_cmp( fread_word( fp ), "sender" ) )

	    break;

	pnote->sender	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "date" ) )

	    break;

	pnote->date	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "to" ) )

	    break;

	pnote->to_list	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "subject" ) )

	    break;

	pnote->subject	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "text" ) )

	    break;

	pnote->text	= fread_string( fp );



	/* Fix from the Themoog from Xania */

	pnote->next	= NULL;



	if ( note_list == NULL )

	    note_list		= pnote;

	else

	    pnotelast->next	= pnote;



	pnotelast	= pnote;

    }



    strcpy( strArea, NOTE_FILE );

    fpArea = fp;

    bug( "Load_notes: bad key word.", 0 );

	    pollDone(Failed);
    exit( 1 );

    return;

}







/*

 * Translate all room exits from virtual to real.

 * Has to be done after all rooms are read in.

 * Check for bad reverse exits.

 */

void fix_exits( void )

{

    extern const sh_int rev_dir [];

    char buf[MAX_STRING_LENGTH];

    ROOM_INDEX_DATA *pRoomIndex;

    ROOM_INDEX_DATA *to_room;

    EXIT_DATA *pexit;

    EXIT_DATA *pexit_rev;

    int iHash;

    int door;



    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )

    {

	for ( pRoomIndex  = room_index_hash[iHash];

	      pRoomIndex != NULL;

	      pRoomIndex  = pRoomIndex->next )

	{

	    bool fexit;



	    fexit = FALSE;

	    for ( door = 0; door <= 5; door++ )

	    {

		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )

		{

		    fexit = TRUE;

		    if ( pexit->vnum <= 0 )

			pexit->to_room = NULL;

		    else

			pexit->to_room = get_room_index( pexit->vnum );

		}

	    }



	    if ( !fexit )

		SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );

	}

    }



    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )

    {

	for ( pRoomIndex  = room_index_hash[iHash];

	      pRoomIndex != NULL;

	      pRoomIndex  = pRoomIndex->next )

	{

	    for ( door = 0; door <= 5; door++ )

	    {

		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL

		&&   ( to_room   = pexit->to_room               ) != NULL

		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL

		&&   pexit_rev->to_room != pRoomIndex )

		{

		    sprintf( buf, "Fix_exits: %d:%d -> %d:%d -> %d.",

			pRoomIndex->vnum, door,

			to_room->vnum,    rev_dir[door],

			(pexit_rev->to_room == NULL)

			    ? 0 : pexit_rev->to_room->vnum );

/*		    bug( buf, 0 ); */

		}

	    }

	}

    }



    return;

}







/*

 * Repopulate areas periodically.

 */

void area_update( void )


{

    AREA_DATA *pArea;

/* take out below for slow, normal area resets */
for (pArea = area_first; pArea; pArea = pArea->next)
    reset_area(pArea);

first_repop = FALSE;

return;
/* take out the above for normal area resets */


    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )

    {

	CHAR_DATA *pch;



	if ( ++pArea->age < 3 )

	    continue;



	/*

	 * Check for PC's.

	 */

	if ( pArea->nplayer > 0 && pArea->age == 15 - 1 )

	{

	    for ( pch = char_list; pch != NULL; pch = pch->next )

	    {

		if ( !IS_NPC(pch)

		&&   IS_AWAKE(pch)

		&&   pch->in_room != NULL

		&&   pch->in_room->area == pArea )

		{

		    send_to_char( "You hear an agonised scream in the distance.\n\r",

			pch );

		}



	    }

	}



	/*

	 * Check age and reset.

	 * Note: Mud School resets every 3 minutes (not 15).

	 */

	if ( pArea->nplayer == 0 || pArea->age >= 15 )

	{

	    ROOM_INDEX_DATA *pRoomIndex;



	    reset_area( pArea );

	    pArea->age = number_range( 0, 3 );

	    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );

	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )

		pArea->age = 15 - 3;

	}

    }



    return;

}







/*

 * Reset one area.

 */

void reset_area( AREA_DATA *pArea )

{

    RESET_DATA *pReset;

    CHAR_DATA *mob;

    bool last;

    int level;



    mob 	= NULL;

    last	= TRUE;

    level	= 0;

    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )

    {

	ROOM_INDEX_DATA *pRoomIndex;

	MOB_INDEX_DATA *pMobIndex;

	OBJ_INDEX_DATA *pObjIndex;

	OBJ_INDEX_DATA *pObjToIndex;

	EXIT_DATA *pexit;

	OBJ_DATA *obj;

	OBJ_DATA *obj_to;



	switch ( pReset->command )

	{

	default:

	    bug( "Reset_area: bad command c.", pReset->command );

	    break;



	case 'M':

	    if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'M': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum d.", pReset->arg3 );

		continue;

	    }



	    level = URANGE( 0, pMobIndex->level - 2, LEVEL_HERO );

	    if ( pMobIndex->count >= pReset->arg2 )

	    {

		last = FALSE;

		break;

	    }

	    if (pMobIndex->spec_fun == spec_kingdom_guard && (number_range(1,9)!=1) && !first_repop)
		continue;

	    mob = create_mobile( pMobIndex );



	    /*

	     * Check for pet shop.

	     */

	    {

		ROOM_INDEX_DATA *pRoomIndexPrev;

		pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );

		if ( pRoomIndexPrev != NULL

		&&   IS_SET(pRoomIndexPrev->room_flags, ROOM_PET_SHOP) )

		    SET_BIT(mob->act, ACT_PET);

	    }



	    if ( room_is_dark( pRoomIndex ) )

		SET_BIT(mob->affected_by, AFF_INFRARED);



	    char_to_room( mob, pRoomIndex );

	    level = URANGE( 0, mob->level - 2, LEVEL_HERO );

	    last  = TRUE;

	    break;



	case 'O':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'O': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum d.", pReset->arg3 );

		continue;

	    }



	    if ( pArea->nplayer > 0

	    ||   count_obj_list( pObjIndex, pRoomIndex->contents ) > 0 )

	    {

		last = FALSE;

		break;

	    }



	    obj       = create_object( pObjIndex, number_range( 1,50 ) );

	    obj->cost = 0;

	    obj_to_room( obj, pRoomIndex );

	    last = TRUE;

	    break;



	case 'P':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'P': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pObjToIndex = get_obj_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'P': bad vnum d.", pReset->arg3 );

		continue;

	    }



	    if ( pArea->nplayer > 0

	    || ( obj_to = get_obj_type( pObjToIndex ) ) == NULL

	    ||   obj_to->in_room == NULL

	    ||   count_obj_list( pObjIndex, obj_to->contains ) > 0 )

	    {

		last = FALSE;

		break;

	    }

	    

	    obj = create_object( pObjIndex, number_range( 1,50 ) );

	    obj_to_obj( obj, obj_to );

	    last = TRUE;

	    break;



	case 'G':

	case 'E':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'E' or 'G': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    if ( !last )

		break;



	    if ( mob == NULL )

	    {

		bug( "Reset_area: 'E' or 'G': null mob for vnum d.",

		    pReset->arg1 );

		last = FALSE;

		break;

	    }



	    if ( mob->pIndexData->pShop != NULL )

	    {

		int olevel;



		switch ( pObjIndex->item_type )

		{

		default:		olevel = 0;                      break;

		case ITEM_PILL:		olevel = number_range(  0, 10 ); break;

		case ITEM_POTION:	olevel = number_range(  0, 10 ); break;

		case ITEM_SCROLL:	olevel = number_range(  5, 15 ); break;

		case ITEM_WAND:		olevel = number_range( 10, 20 ); break;

		case ITEM_STAFF:	olevel = number_range( 15, 25 ); break;

		case ITEM_ARMOR:	olevel = number_range(  5, 15 ); break;

		case ITEM_WEAPON:	olevel = number_range(  5, 15 ); break;

		}



		obj = create_object( pObjIndex, olevel );

		SET_BIT( obj->extra_flags, ITEM_INVENTORY );

	    }

	    else

	    {

		obj = create_object( pObjIndex, number_range( 1,50 ) );

	    }

	    obj_to_char( obj, mob );

	    if ( pReset->command == 'E' )

		equip_char( mob, obj, pReset->arg3 );

	    last = TRUE;

	    break;



	case 'D':

	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'D': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )

		break;



	    switch ( pReset->arg3 )

	    {

	    case 0:

		REMOVE_BIT( pexit->exit_info, EX_CLOSED );

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );

		break;



	    case 1:

		SET_BIT(    pexit->exit_info, EX_CLOSED );

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );

		break;



	    case 2:

		SET_BIT(    pexit->exit_info, EX_CLOSED );

		SET_BIT(    pexit->exit_info, EX_LOCKED );

		break;

	    }



	    last = TRUE;

	    break;



	case 'R':

	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum d.", pReset->arg1 );

		continue;

	    }



	    {

		int d0;

		int d1;



		for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )

		{

		    d1                   = number_range( d0, pReset->arg2-1 );

		    pexit                = pRoomIndex->exit[d0];

		    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];

		    pRoomIndex->exit[d1] = pexit;

		}

	    }

	    break;

	}

    }



    return;

}







/*

 * Create an instance of a mobile.

 */

CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )

{

    CHAR_DATA *mob;

    int tempvalue;



    if ( pMobIndex == NULL )

    {

	bug( "Create_mobile: NULL pMobIndex.", 0 );

	append("../area/crash.txt", "The below crash is caused by a null pMobIndex in create_mobile if it happend at the time ");
	append("../area/crash.txt", ctime(&current_time));
	return NULL;

    }



    if ( char_free == NULL )

    {

	mob		= alloc_perm( sizeof(*mob) );

    }

    else

    {

	mob		= char_free;

	char_free	= char_free->next;

    }



    clear_char( mob );

    mob->pIndexData	= pMobIndex;


    mob->hunting	= str_dup( "" );

    mob->lord		= str_dup( "" );

    mob->clan		= str_dup( "" );

    mob->morph		= str_dup( "" );

    mob->createtime	= str_dup( "" );

    mob->lasttime	= str_dup( "" );

    mob->lasthost	= str_dup( "" );

    mob->immpower = str_dup(""); /* bog */

    mob->powertype	= str_dup( "" );

    mob->poweraction	= str_dup( "" );

    mob->pload		= str_dup( "" );

    mob->prompt		= str_dup( "" );

    mob->cprompt	= str_dup( "" );



    mob->name		= pMobIndex->player_name;

    mob->short_descr	= pMobIndex->short_descr;

    mob->long_descr	= pMobIndex->long_descr;

    mob->description	= pMobIndex->description;

    free_string(mob->kingdom);
    mob->kingdom = str_dup(pMobIndex->kingdom);


    mob->spec_fun	= pMobIndex->spec_fun;



    mob->home		= 3001;

    mob->form		= 32767;

    mob->level		= number_fuzzy( pMobIndex->level );
for (tempvalue = 1; tempvalue <= STANCE_WOLF; tempvalue++)
    mob->stance[tempvalue] = mob->level;
mob->stance[0] = -1;
if (pMobIndex->vnum >= 11300 && pMobIndex->vnum < 11336)
    mob->level *= 2.2;
if (pMobIndex->level >= 9200 && pMobIndex->vnum <= 9299)
    mob->level *= 3.5;
if (pMobIndex->level >= 9400 && pMobIndex->vnum <= 9450)
    mob->level *= 5;

    mob->act		= pMobIndex->act;

    mob->affected_by	= pMobIndex->affected_by;

    mob->alignment	= pMobIndex->alignment;

    mob->sex		= pMobIndex->sex;



    mob->armor		= interpolate( mob->level, 100, -100 );



    tempvalue		= mob->level * 12 + number_range(

				mob->level * mob->level / 4,

				mob->level * mob->level );

tempvalue/=1.8;
    if (tempvalue > 1000000) mob->max_hit = 1000000;

    else mob->max_hit	= tempvalue;

    if (mob->level >= 85)
	mob->max_hit *= 3;

if (pMobIndex->hp > 0) mob->max_hit = pMobIndex->hp;
if (pMobIndex->mn > 0) mob->max_mana = pMobIndex->mn;
if (pMobIndex->mv > 0) mob->max_move = pMobIndex->mv;


    mob->hit		= mob->max_hit;



    mob->hitroll	= mob->level;

    mob->damroll	= mob->level;



    /*

     * Insert in list.

     */

    mob->next		= char_list;

    char_list		= mob;

    pMobIndex->count++;

    return mob;

}





    int objid = 1;


/*

 * Create an instance of an object.

 */

OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )

{

    static OBJ_DATA obj_zero;

    OBJ_DATA *obj;


    if ( pObjIndex == NULL )

    {

	bug( "Create_object: NULL pObjIndex.", 0 );

	append("../area/crash.txt", "The below crash is caused by a null pObjIndex in create_object if it happend at the time ");
	append("../area/crash.txt", ctime(&current_time));
	return NULL;

    }



    if ( obj_free == NULL )

    {

	obj		= alloc_perm( sizeof(*obj) );

    }

    else

    {

	obj		= obj_free;

	obj_free	= obj_free->next;

    }

    obj->miscbit[0] = 0; obj->miscbit[1] = 0;

    *obj		= obj_zero;

    obj->pIndexData	= pObjIndex;

    obj->in_room	= NULL;

    obj->level		= level;

    obj->wear_loc	= -1;

    obj->ClassBit	= pObjIndex->ClassBit;

    obj->name		= pObjIndex->name;

    obj->short_descr	= pObjIndex->short_descr;

    obj->description	= pObjIndex->description;



    if (pObjIndex->chpoweron != NULL)

    {

	obj->chpoweron    = pObjIndex->chpoweron;

	obj->chpoweroff   = pObjIndex->chpoweroff;

	obj->chpoweruse   = pObjIndex->chpoweruse;

	obj->victpoweron  = pObjIndex->victpoweron;

	obj->victpoweroff = pObjIndex->victpoweroff;

	obj->victpoweruse = pObjIndex->victpoweruse;

	obj->spectype     = pObjIndex->spectype;

	obj->specpower    = pObjIndex->specpower;

    }

    else

    {

	obj->chpoweron    = str_dup( "(null)" );

	obj->chpoweroff   = str_dup( "(null)" );

	obj->chpoweruse   = str_dup( "(null)" );

	obj->victpoweron  = str_dup( "(null)" );

	obj->victpoweroff = str_dup( "(null)" );

	obj->victpoweruse = str_dup( "(null)" );

	obj->spectype     = 0;

	obj->specpower    = 0;

    }

    obj->questmaker     = str_dup( "" );

    obj->questowner     = str_dup( "" );



    obj->chobj		= NULL;



    obj->quest		= 0;

    obj->points		= 0;



    obj->item_type	= pObjIndex->item_type;

    obj->extra_flags	= pObjIndex->extra_flags;

    obj->wear_flags	= pObjIndex->wear_flags;

    obj->value[0]	= pObjIndex->value[0];

    obj->value[1]	= pObjIndex->value[1];

    obj->value[2]	= pObjIndex->value[2];

    obj->value[3]	= pObjIndex->value[3];

    obj->weight		= pObjIndex->weight;

    obj->cost		= number_fuzzy( 10 )

			* number_fuzzy( level ) * number_fuzzy( level );

 

	if (obj->pIndexData->vnum >= 29600 && obj->pIndexData->vnum <= 29625)

	{

	obj->condition = 100;

	obj->toughness = 100;

	SET_BIT(obj->spectype,SITEM_DROW);

	SET_BIT(obj->quest,QUEST_RELIC);

	} else {



    if (obj->pIndexData->vnum >=  29500 && obj->pIndexData->vnum <= 29599)

    {

	SET_BIT(obj->quest, QUEST_ARTIFACT);

    	obj->condition	= 100;

    	obj->toughness	= 100;

    	obj->resistance	= 1;

	obj->level	= 60;

	obj->cost	= 1000000;

    }

    else if ((obj->pIndexData->vnum >= 29666 

             && obj->pIndexData->vnum <= 29679

	     && !IS_SET(obj->spectype, SITEM_PDEMONIC)) ||
obj->pIndexData->vnum == 29713||obj->pIndexData->vnum==29714)

	{

    SET_BIT(obj->spectype, SITEM_PDEMONIC);

    SET_BIT(obj->quest, QUEST_RELIC);

        obj->condition  = 100;

        obj->toughness  = 100;

        obj->resistance = 1; 

	}

    else if (obj->pIndexData->vnum >= 27650 && obj->pIndexData->vnum <=
29712)

    {

	SET_BIT(obj->quest, QUEST_RELIC);

    	obj->condition	= 100;

    	obj->toughness	= 100;

    	obj->resistance	= 1;

    }

    else

    {

    	obj->condition	= 100;

    	obj->toughness	= 5;

    	obj->resistance	= 25;

    }

}

    if (obj->ClassBit >0)
	SET_BIT(obj->quest, QUEST_RELIC);
    /*

     * Mess with object properties.

     */

    switch ( obj->item_type )

    {

    default:

	bug( "Read_object: vnum d bad type.", pObjIndex->vnum );

	break;



    case ITEM_LIGHT:

    case ITEM_TREASURE:

    case ITEM_FURNITURE:

    case ITEM_TRASH:

    case ITEM_CONTAINER:

    case ITEM_DRINK_CON:

    case ITEM_KEY:

    case ITEM_FOOD:

    case ITEM_BOAT:

    case ITEM_CORPSE_NPC:

    case ITEM_CORPSE_PC:

    case ITEM_FOUNTAIN:

    case ITEM_PORTAL:

    case ITEM_EGG:

    case ITEM_VOODOO:

    case ITEM_STAKE:

    case ITEM_MISSILE:

    case ITEM_AMMO:

    case ITEM_QUEST:

    case ITEM_QUESTCARD:

    case ITEM_QUESTMACHINE:

    case ITEM_SYMBOL:

    case ITEM_BOOK:

    case ITEM_PAGE:

    case ITEM_TOOL:

    case ITEM_COPPER:

    case ITEM_IRON:

    case ITEM_STEEL:

    case ITEM_ADAMANTITE:

    case ITEM_GEMSTONE:

    case ITEM_HILT:

	break;



    case ITEM_SCROLL:

	obj->value[0]	= number_fuzzy( obj->value[0] );

	break;



    case ITEM_WAND:

    case ITEM_STAFF:

	obj->value[0]	= number_fuzzy( obj->value[0] );

	obj->value[1]	= number_fuzzy( obj->value[1] );

	obj->value[2]	= obj->value[1];

	break;



    case ITEM_WEAPON:

	if (!IS_SET(obj->quest, QUEST_ARTIFACT)

	    && !IS_SET(obj->quest, QUEST_RELIC))

	{
	    if (pObjIndex->vnum <= 11413 && pObjIndex->vnum >= 11300) ;
	else {

	    obj->value[1]	= number_range( 1, 10 );

	    obj->value[2]	= number_range((obj->value[1]+1),(obj->value[1]*2));
	}
	}

	break;



    case ITEM_ARMOR:

	if (!IS_SET(obj->quest, QUEST_ARTIFACT)

	    && !IS_SET(obj->quest, QUEST_RELIC))

	    obj->value[0]	= number_range( 5, 15 );

	break;



    case ITEM_POTION:

    case ITEM_PILL:

	obj->value[0]	= number_fuzzy( number_fuzzy( obj->value[0] ) );

	break;



    case ITEM_MONEY:

	obj->value[0]	= obj->cost;

	break;

    }



    obj->next		= object_list;

    object_list		= obj;

    pObjIndex->count++;


    if (objid == 0) objid = 1;
    obj->ObjId = objid;
    objid++;

    return obj;

}







/*

 * Clear a new character.

 */

void clear_char( CHAR_DATA *ch )

{

    static CHAR_DATA ch_zero;



    *ch				= ch_zero;

    ch->name			= &str_empty[0];

    ch->short_descr		= &str_empty[0];

    ch->long_descr		= &str_empty[0];

    ch->description		= &str_empty[0];

    ch->lord			= &str_empty[0];

    ch->clan			= &str_empty[0];

    ch->morph			= &str_empty[0];

    ch->createtime		= &str_empty[0];

    ch->lasthost		= &str_empty[0];

    ch->lasttime		= &str_empty[0];

    ch->powertype		= &str_empty[0];

    ch->poweraction		= &str_empty[0];

    ch->pload			= &str_empty[0];

    ch->prompt			= &str_empty[0];

    ch->cprompt			= &str_empty[0];

    ch->hunting			= &str_empty[0];



    ch->logon			= current_time;

    ch->armor			= 100;

    ch->position		= POS_STANDING;

    ch->practice		= 0;

    ch->hit			= 1000;

    ch->max_hit			= 1000;

    ch->mana			= 1500;

    ch->max_mana		= 1500;

    ch->move			= 1500;

    ch->max_move		= 1500;

    ch->master			= NULL;

    ch->leader			= NULL;

    ch->fighting		= NULL;

    ch->mount			= NULL;

    ch->wizard			= NULL;

    ch->paradox[0]              = 0;        

    ch->paradox[1]              = 0;        

    ch->paradox[2]              = 0;        

    ch->damcap[0]		= 1000;

    ch->damcap[1]		= 0;

    return;

}







/*

 * Free a character.

 */

void free_char( CHAR_DATA *ch )

{

    OBJ_DATA *obj;

    OBJ_DATA *obj_next;

    AFFECT_DATA *paf;

    AFFECT_DATA *paf_next;



    for ( obj = ch->carrying; obj != NULL; obj = obj_next )

    {

	obj_next = obj->next_content;

	extract_obj( obj );

    }



    for ( paf = ch->affected; paf != NULL; paf = paf_next )

    {

	paf_next = paf->next;

	affect_remove( ch, paf );

    }

if (ch->influx)
   granite_struct(ch->influx, sFreeList);
ch->influx = NULL;

    free_string( ch->name		);

    free_string( ch->short_descr	);

    free_string( ch->long_descr		);

    free_string( ch->description	);

    free_string( ch->lord 		);

    free_string( ch->clan 		);

    free_string( ch->morph 		);

    free_string( ch->createtime 	);

    free_string( ch->lasttime 		);

    free_string( ch->lasthost 		);

    free_string( ch->powertype 		);

    free_string( ch->poweraction 	);

    free_string( ch->decap		);

    free_string( ch->pload 		);

    free_string( ch->prompt 		);

    free_string( ch->cprompt 		);

    free_string( ch->hunting		);



    if ( ch->pcdata != NULL )

    {

	free_string( ch->pcdata->pwd		);

	free_string( ch->pcdata->bamfin		);

	free_string( ch->pcdata->bamfout	);

	free_string( ch->pcdata->title		);

	free_string( ch->pcdata->conception	);

	free_string( ch->pcdata->parents	);

	free_string( ch->pcdata->cparents	);

	free_string( ch->pcdata->marriage	);

	free_string( ch->pcdata->email		);

	ch->pcdata->next = pcdata_free;

	pcdata_free      = ch->pcdata;

    }



    ch->next	     = char_free;

    char_free	     = ch;

    return;

}







/*

 * Get an extra description from a list.

 */

char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )

{

    for ( ; ed != NULL; ed = ed->next )

    {

	if ( is_name( name, ed->keyword ) )

	    return ed->description;

    }

    return NULL;

}







/*

 * Translates mob virtual number to its mob index struct.

 * Hash table lookup.

 */

MOB_INDEX_DATA *get_mob_index( int vnum )

{

    MOB_INDEX_DATA *pMobIndex;



    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];

	  pMobIndex != NULL;

	  pMobIndex  = pMobIndex->next )

    {

	if ( pMobIndex->vnum == vnum )

	    return pMobIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_mob_index: bad vnum d.", vnum );

	    pollDone(Failed);
	exit( 1 );

    }



    return NULL;

}







/*

 * Translates mob virtual number to its obj index struct.

 * Hash table lookup.

 */

OBJ_INDEX_DATA *get_obj_index( int vnum )

{

    OBJ_INDEX_DATA *pObjIndex;



    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];

	  pObjIndex != NULL;

	  pObjIndex  = pObjIndex->next )

    {

	if ( pObjIndex->vnum == vnum )

	    return pObjIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_obj_index: bad vnum d.", vnum );

	    pollDone(Failed);
	exit( 1 );

    }



    return NULL;

}







/*

 * Translates mob virtual number to its room index struct.

 * Hash table lookup.

 */

ROOM_INDEX_DATA *get_room_index( int vnum )

{

    ROOM_INDEX_DATA *pRoomIndex;



    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];

	  pRoomIndex != NULL;

	  pRoomIndex  = pRoomIndex->next )

    {

	if ( pRoomIndex->vnum == vnum )

	    return pRoomIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_room_index: bad vnum d.", vnum );

	    pollDone(Failed);
	exit( 1 );

    }



    return NULL;

}







/*

 * Read a letter from a file.

 */

char fread_letter( FILE *fp )

{

    char c;



    do

    {

	c = getc( fp );

    }

    while ( isspace(c) );



    return c;

}







/*

 * Read a number from a file.

 */

int fread_number( FILE *fp )

{

    int number;

    bool sign;

    char c;



    do

    {

	c = getc( fp );

    }

    while ( isspace(c) );



    number = 0;



    sign   = FALSE;

    if ( c == '+' )

    {

	c = getc( fp );

    }

    else if ( c == '-' )

    {

	sign = TRUE;

	c = getc( fp );

    }



    if ( !isdigit(c) )

    {

    if (!feof_handler)
    {
	bug( "Fread_number: bad format.", 0 );
	if (fBootDb)
	{
	    pollDone(Failed);
	exit(1);}
	else {append("../area/crash.txt","The following crash is caused by raise(SIGPFE) in fread_number .. however it is not a floating point error.\n");raise(SIGFPE);}
    }
    else
	if (feof_handler(fp,FEOF_FEOF)<0) {append("../area/crash.txt","feof_handler returned -1.\n");	    pollDone(Failed);exit(1);}
	return 0;
    }



    while ( isdigit(c) )

    {

	number = number * 10 + c - '0';

	c      = getc( fp );

    }



    if ( sign )

	number = 0 - number;


    if ( c == '&' ) REMOVE_BIT(number, fread_number(fp));

    else if ( c == '|' )

	SET_BIT(number, fread_number(fp));

    else if ( c != ' ' )

	ungetc( c, fp );



    return number;

}







/*

 * Read and allocate space for a string from a file.

 * These strings are read-only and shared.

 * Strings are hashed:

 *   each string prepended with hash pointer to prev string,

 *   hash code is simply the string length.

 * This function takes 40 to 50 of boot-up time.

 */

char *fread_string( FILE *fp )

{

    char *plast;

    char c;



    plast = top_string + sizeof(char *);

    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )

    {

    if (!feof_handler)
    {
	bug( "Fread_string: MAX_STRING d exceeded.", MAX_STRING );
	if (fBootDb)
	{	    pollDone(Failed);exit(1);}
	else {append("../area/crash.txt","The following crash was caused by fread_string while the MAX_STRING constant was exceeded.\n"); raise(SIGSEGV);}
    }
    else
	if (feof_handler(fp,FEOF_FEOF)<0) {append("../area/crash.txt","The following crash was caused when the feof_handler in fread_string failed because MAX_STRING has been exceeded.\n");exit(1);}
	return "";
    }



    /*

     * Skip blanks.

     * Read first char.

     */

    do

    {

	c = getc( fp );

    }

    while ( isspace(c) );



    if ( ( *plast++ = c ) == '~' )

	return &str_empty[0];



    for ( ;; )

    {

	/*

	 * Back off the char type lookup,

	 *   it was too dirty for portability.

	 *   -- Furey

	 */

	switch ( *plast = getc( fp ) )

	{

	default:

	    plast++;

	    break;



	case EOF:

    if (!feof_handler)
    {
	bug( "Fread_string: EOF.", 0 );
	if (fBootDb)
	{	    pollDone(Failed);exit(1);}
	else {append("../area/crash.txt","The following crash is caused by raise(SIGSEGV) in fread_string while it had an EOF.\n");raise(SIGSEGV);}
    }
    else
	if (feof_handler(fp,FEOF_FEOF)<0) {append("../area/crash.txt","feof_handler returned -1 in fread_string.\n");exit(1);}
	return "";
	    break;



	case '\n':

	    plast++;

	    *plast++ = '\r';

	    break;



	case '\r':

	    break;



	case '~':

	    plast++;

	    {

		union

		{

		    char *	pc;

		    char	rgc[sizeof(char *)];

		} u1;

		int ic;

		int iHash;

		char *pHash;

		char *pHashPrev;

		char *pString;



		plast[-1] = '\0';

		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );

		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )

		{

		    for ( ic = 0; ic < sizeof(char *); ic++ )

			u1.rgc[ic] = pHash[ic];

		    pHashPrev = u1.pc;

		    pHash    += sizeof(char *);



		    if ( top_string[sizeof(char *)] == pHash[0]

		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )

			return pHash;

		}



		if ( fBootDb )

		{

		    pString		= top_string;

		    top_string		= plast;

		    u1.pc		= string_hash[iHash];

		    for ( ic = 0; ic < sizeof(char *); ic++ )

			pString[ic] = u1.rgc[ic];

		    string_hash[iHash]	= pString;



		    nAllocString += 1;

		    sAllocString += top_string - pString;

		    return pString + sizeof(char *);

		}

		else

		{

		    return str_dup( top_string + sizeof(char *) );

		}

	    }

	}

    }

}







/*

 * Read to end of line (for comments).

 */

char *fread_to_eol( FILE *fp )

{
    static char buf[MSL];

    char c;

strcpy(buf,"");

    do

    {
	c = getc( fp );
	if (c == EOF) break;
if (strlen(buf) < MSL-8) {
sprintf(log_buf, "%s%c", buf, c);
strcpy(buf,log_buf);}
    }

    while ( c != '\n' && c != '\r' );



    do

    {

	c = getc( fp );
	if (c == EOF) break;

    }

    while ( c == '\n' || c == '\r' );



    ungetc( c, fp );

    return buf;

}







/*

 * Read one word (into static buffer).

 */

char *fread_word( FILE *fp )

{

    static char word[MAX_INPUT_LENGTH];

    char *pword;

    char cEnd;



    do

    {

	cEnd = getc( fp );

    }

    while ( isspace( cEnd ) );



    if ( cEnd == '\'' || cEnd == '"' )

    {

	pword   = word;

    }

    else

    {

	word[0] = cEnd;

	pword   = word+1;

	cEnd    = ' ';

    }



    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )

    {

	*pword = getc( fp );

	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )

	{

	    if ( cEnd == ' ' )

		ungetc( *pword, fp );

	    *pword = '\0';

	    return word;

	}

    }


    if (!feof_handler)
    {
	bug( "Fread_word: EOF", 0 );
	if (fBootDb)
	{	    pollDone(Failed);exit(1);}
	else {append("../area/crash.txt","The following crash is from fread_word having an EOF error.\n");raise(SIGSEGV);}
    }
    else
	if (feof_handler(fp,FEOF_FEOF)<0) {append("../area/crash.txt","The following crash was caused when the feof_handler returned a negative number on an EOF event from fread_word.\n");exit(1);}

    return "";

}







/*

 * Allocate some ordinary memory,

 *   with the expectation of freeing it someday.

 */

void *alloc_mem( int sMem )

{

    void *pMem;

    int iList;

//return malloc(sMem);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )

    {

	if ( sMem <= rgSizeList[iList] )

	    break;

    }



    if ( iList == MAX_MEM_LIST )

    {

	bug( "Alloc_mem: size d too large.", sMem );

if (pfile) raise(SIGPFILE); else
{	    pollDone(Failed);
	exit( 1 );}

    }



    if ( rgFreeList[iList] == NULL )

    {

	pMem		  = alloc_perm( rgSizeList[iList] );

    }

    else

    {

	pMem              = rgFreeList[iList];

	rgFreeList[iList] = * ((void **) rgFreeList[iList]);

    }



    return pMem;

}

void free_mem( void *pMem, int sMem )
{
    int iList;

/*free(pMem);
return;
*/
    for (iList = 0; iList < MAX_MEM_LIST; iList++)
    {
	if ( sMem < rgSizeList[iList] )
	    break;
    }

    if (iList == MAX_MEM_LIST)
    {
	bug("Free_mem: size d too large.", sMem);
	raise(SIGSEGV);
    }

    *((void**)pMem) = rgFreeList[iList];
    rgFreeList[iList] = pMem;

    return;
}

/*

 * Allocate some permanent memory.

 * Permanent memory is never freed,

 *   pointers into it may be copied safely.

 */

void *alloc_perm( int sMem )

{

    static char *pMemPerm;

    static int iMemPerm;

    void *pMem;

//return malloc(sMem);

    while ( sMem % sizeof(long) != 0 )

	sMem++;

    if ( sMem > MAX_PERM_BLOCK )

    {

	bug( "Alloc_perm: d too large.", sMem );

	append("../area/crash.txt","There is a memory leak. Alloc_perm has a size too large error..crashing.(for the following crash)\n");
	raise(SIGSEGV);

    }



    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )

    {

	iMemPerm = 0;

	if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )

	{

	    perror( "Alloc_perm" );

	    append("../area/crash.txt","Error in alloc_perm..view the logs at the date ");
	    append("../area/crash.txt",mud_time);
	    append("../area/crash.txt"," for the error message.(the following crash is the resault\n");
	    raise(SIGSEGV);

	}

    }



    pMem        = pMemPerm + iMemPerm;

    iMemPerm   += sMem;

    nAllocPerm += 1;

    sAllocPerm += sMem;

    return pMem;

}







/*

 * Duplicate a string into dynamic memory.

 * Fread_strings are read-only and shared.

 */

char *str_dup( const char *str )

{

    char *str_new;



    if ( str[0] == '\0' )

	return &str_empty[0];



    if ( str >= string_space && str < top_string )

	return (char *) str;



    str_new = malloc( strlen(str) + 1 );

    strcpy( str_new, str );

    return str_new;

}







/*

 * Free a string.

 * Null is legal here to simplify callers.

 * Read-only shared strings are not touched.

 */

void free_string( char *pstr )

{

    if ( pstr == NULL

    ||   pstr == &str_empty[0]

    || ( pstr >= string_space && pstr < top_string ) )

	return;



    free( pstr);

    return;

}








/*
 * Creat a new room (for online building)			-Thoric
 */
ROOM_INDEX_DATA *make_room( int vnum )
{
	ROOM_INDEX_DATA *pRoomIndex;
	int	iHash;
	int     door;

	CREATE( pRoomIndex, ROOM_INDEX_DATA, 1 );
	pRoomIndex->people	        = NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->area		= NULL;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= STRALLOC("Floating in a void");
	pRoomIndex->description		= STRALLOC("");
	pRoomIndex->room_flags		= ROOM_PROTOTYPE;
	pRoomIndex->sector_type		= 1;
	pRoomIndex->light		= 0;
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;

	return pRoomIndex;
}

/*
 * Create a new INDEX object (for online building)		-Thoric
 * Option to clone an existing index object.
 */
OBJ_INDEX_DATA *make_object( int vnum, int cvnum, char *name )
{
	OBJ_INDEX_DATA *pObjIndex, *cObjIndex;
	char buf[MAX_STRING_LENGTH];
	int	iHash;

	if ( cvnum > 0 )
	  cObjIndex = get_obj_index( cvnum );
	else
	  cObjIndex = NULL;
	CREATE( pObjIndex, OBJ_INDEX_DATA, 1 );
	pObjIndex->vnum			= vnum;
	pObjIndex->name			= STRALLOC( name );
	pObjIndex->affected		= NULL;
	pObjIndex->extra_descr		= NULL;
	if ( !cObjIndex )
	{
	  sprintf( buf, "A newly created %s", name );
	  pObjIndex->short_descr	= STRALLOC( buf  );
	  sprintf( buf, "Some god dropped a newly created %s here.", name );
	  pObjIndex->description	= STRALLOC( buf );
	  pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);
	  pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);
	  pObjIndex->item_type		= ITEM_TRASH;
	  pObjIndex->extra_flags	= ITEM_PROTOTYPE;
	  pObjIndex->wear_flags		= 0;
	  pObjIndex->value[0]		= 0;
	  pObjIndex->value[1]		= 0;
	  pObjIndex->value[2]		= 0;
	  pObjIndex->value[3]		= 0;
	  pObjIndex->weight		= 1;
	  pObjIndex->cost		= 0;
	}
	else
	{
/* None of these are in use, guessing this function is incomplete.
	  EXTRA_DESCR_DATA *ed,  *ced;
	  AFFECT_DATA	   *paf, *cpaf;
*/

	  pObjIndex->short_descr	= QUICKLINK( cObjIndex->short_descr );
	  pObjIndex->description	= QUICKLINK( cObjIndex->description );
	  pObjIndex->item_type		= cObjIndex->item_type;
	  pObjIndex->extra_flags	= cObjIndex->extra_flags
	  				| ITEM_PROTOTYPE;
	  pObjIndex->wear_flags		= cObjIndex->wear_flags;
	  pObjIndex->value[0]		= cObjIndex->value[0];
	  pObjIndex->value[1]		= cObjIndex->value[1];
	  pObjIndex->value[2]		= cObjIndex->value[2];
	  pObjIndex->value[3]		= cObjIndex->value[3];
	  pObjIndex->weight		= cObjIndex->weight;
	  pObjIndex->cost		= cObjIndex->cost;
/*
	  for ( ced = cObjIndex->first_extradesc; ced; ced = ced->next )
	  {
		CREATE( ed, EXTRA_DESCR_DATA, 1 );
		ed->keyword		= QUICKLINK( ced->keyword );
		ed->description		= QUICKLINK( ced->description );
		LINK( ed, pObjIndex->first_extradesc, pObjIndex->last_extradesc,
			  next, prev );
		top_ed++;
	  }
	  for ( cpaf = cObjIndex->first_affect; cpaf; cpaf = cpaf->next )
	  {
		CREATE( paf, AFFECT_DATA, 1 );
		paf->type		= cpaf->type;
		paf->duration		= cpaf->duration;
		paf->location		= cpaf->location;
		paf->modifier		= cpaf->modifier;
		paf->bitvector		= cpaf->bitvector;
		LINK( paf, pObjIndex->first_affect, pObjIndex->last_affect, next, prev );
		top_affect++;
	  }
*/
	}
	pObjIndex->count		= 0;
	iHash				= vnum % MAX_KEY_HASH;
	pObjIndex->next			= obj_index_hash[iHash];
	obj_index_hash[iHash]		= pObjIndex;
	top_obj_index++;

	return pObjIndex;
}

/*
 * Create a new INDEX mobile (for online building)		-Thoric
 * Option to clone an existing index mobile.
 */
MOB_INDEX_DATA *make_mobile( sh_int vnum, sh_int cvnum, char *name )
{
	MOB_INDEX_DATA *pMobIndex, *cMobIndex;
	char buf[MAX_STRING_LENGTH];
	int	iHash;

	if ( cvnum > 0 )
	  cMobIndex = get_mob_index( cvnum );
	else
	  cMobIndex = NULL;
	CREATE( pMobIndex, MOB_INDEX_DATA, 1 );
	pMobIndex->vnum			= vnum;
	pMobIndex->count		= 0;
	pMobIndex->killed		= 0;
	pMobIndex->player_name		= STRALLOC( name );
	if ( !cMobIndex )
	{
	  sprintf( buf, "A newly created %s", name );
	  pMobIndex->short_descr	= STRALLOC( buf  );
	  sprintf( buf, "Some god abandoned a newly created %s here.\n\r", name );
	  pMobIndex->long_descr		= STRALLOC( buf );
	  pMobIndex->description	= STRALLOC( "" );
	  pMobIndex->short_descr[0]	= LOWER(pMobIndex->short_descr[0]);
	  pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	  pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);
	  pMobIndex->act		= ACT_IS_NPC | ACT_PROTOTYPE;
	  pMobIndex->affected_by	= 0;
	  pMobIndex->spec_fun		= NULL;
	  pMobIndex->alignment		= 0;
	  pMobIndex->level		= 1;
	  pMobIndex->ac			= 0;
	  pMobIndex->gold		= 0;
	  pMobIndex->sex		= 0;
	}
	else
	{
	  pMobIndex->short_descr	= QUICKLINK( cMobIndex->short_descr );
	  pMobIndex->long_descr		= QUICKLINK( cMobIndex->long_descr );
	  pMobIndex->description	= QUICKLINK( cMobIndex->description );
	  pMobIndex->act		= cMobIndex->act | ACT_PROTOTYPE;
	  pMobIndex->affected_by	= cMobIndex->affected_by;
	  pMobIndex->spec_fun		= cMobIndex->spec_fun;
	  pMobIndex->alignment		= cMobIndex->alignment;
	  pMobIndex->level		= cMobIndex->level;
	  pMobIndex->ac			= cMobIndex->ac;
	  pMobIndex->gold		= cMobIndex->gold;
	  pMobIndex->sex		= cMobIndex->sex;
	}
	iHash				= vnum % MAX_KEY_HASH;
	pMobIndex->next			= mob_index_hash[iHash];
	mob_index_hash[iHash]		= pMobIndex;
	top_mob_index++;

	return pMobIndex;
}

/*
 * Creates a simple exit with no fields filled but rvnum and optionally
 * to_room and vnum.						-Thoric
 * Exits are inserted into the linked list based on vdir.
 */

EXIT_DATA *make_exit( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, sh_int door )
{
	EXIT_DATA *pexit, *texit;
	bool broke;

	CREATE( pexit, EXIT_DATA, 1 );
	pexit->vdir		= door;
	pexit->rvnum		= pRoomIndex->vnum;
	pexit->to_room		= to_room;
	if ( to_room )
	{
	    pexit->vnum = to_room->vnum;
	    texit = get_exit_to( to_room, rev_dir[door], pRoomIndex->vnum );
	    if ( texit )
	    {
		texit->rexit = pexit;
		pexit->rexit = texit;
	    }
	}
	broke = FALSE;
	for ( texit = pRoomIndex->first_exit; texit; texit = texit->next )
	   if ( door < texit->vdir )
	   {
	     broke = TRUE;
	     break;
	   }
	if ( !pRoomIndex->first_exit )
	  pRoomIndex->first_exit	= pexit;
	else
	{

	  if ( broke && texit )
	  {
	    if ( !texit->prev )
	      pRoomIndex->first_exit	= pexit;
	    else
	      texit->prev->next		= pexit;
	    pexit->prev			= texit->prev;
	    pexit->next			= texit;
	    texit->prev			= pexit;
	    top_exit++;
	    return pexit;
	  }
	  pRoomIndex->last_exit->next	= pexit;
	}
	pexit->next			= NULL;
	pexit->prev			= pRoomIndex->last_exit;
	pRoomIndex->last_exit		= pexit;
	top_exit++;
	fix_exits();
	return pexit;
}


void do_areas( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    AREA_DATA *pArea1;

    AREA_DATA *pArea2;

    int iArea;

    int iAreaHalf;

    WAIT_STATE(ch,10);

    iAreaHalf = (top_area + 1) / 2;

    pArea1    = area_first;

    pArea2    = area_first;

    for ( iArea = 0; iArea < iAreaHalf; iArea++ )

	pArea2 = pArea2->next;



    

   for ( iArea = 0; iArea < iAreaHalf; iArea++ )

    {

	sprintf( buf, "%-39s%-39s\n\r",

	    pArea1->name, (pArea2 != NULL) ? pArea2->name : "" );

	send_to_char( buf, ch );

	pArea1 = pArea1->next;

	if ( pArea2 != NULL )

	    pArea2 = pArea2->next;

    }



    return;

}







void do_memory( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );

    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );

    sprintf( buf, "RmTxt   %5d\n\r", top_rt        ); send_to_char( buf, ch );

    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );

    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );

    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );

    sprintf( buf, "Mobs    %5d\n\r", top_mob_index ); send_to_char( buf, ch );

    sprintf( buf, "Objs    %5d\n\r", top_obj_index ); send_to_char( buf, ch );

    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );

    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );

    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );



    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",

	nAllocString, sAllocString, MAX_STRING );

    send_to_char( buf, ch );



    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",

	nAllocPerm, sAllocPerm );

    send_to_char( buf, ch );



    return;

}







/*

 * Stick a little fuzz on a number.

 */

int number_fuzzy( int number )

{
    switch (number_range(1,3))
    {
	case 1: number--;
	break;
	case 2: number++;
	break;
	case 3: break;
    }


    return UMAX( 1, number );

}

int number_fuzzy2( int number )

{

    switch ( number_bits( 2 ) )

    {

    case 0:  number -= 1; break;

    case 3:  number += 1; break;

    }




 return number;
}







/*

 * Generate a random number.

 */

int number_range( int from, int to )

{

    int power;

    int number;



    if ( ( to = to - from + 1 ) <= 1 )

	return from;



    for ( power = 2; power < to; power <<= 1 )

	;



    while ( ( number = number_mm( ) & (power - 1) ) >= to )

	;



    return from + number;

}







/*

 * Generate a percentile roll.

 */

int number_percent( void )

{

    int percent;



    while ( ( percent = number_mm( ) & (128-1) ) > 99 )

	;



    return 1 + percent;

}







/*

 * Generate a random door.

 */

int number_door( void )

{

    int door;



    while ( ( door = number_mm( ) & (8-1) ) > 5 )

	;



    return door;

}







int number_bits( int width )

{

    return number_mm( ) & ( ( 1 << width ) - 1 );

}







/*

 * I've gotten too many bad reports on OS-supplied random number generators.

 * This is the Mitchell-Moore algorithm from Knuth Volume II.

 * Best to leave the constants alone unless you've read Knuth.

 * -- Furey

 */

static	int	rgiState[2+55];



void init_mm( )

{

    int *piState;

    int iState;



    piState	= &rgiState[2];



    piState[-2]	= 55 - 55;

    piState[-1]	= 55 - 24;



    piState[0]	= ((int) current_time) & ((1 << 30) - 1);

    piState[1]	= 1;

    for ( iState = 2; iState < 55; iState++ )

    {

	piState[iState] = (piState[iState-1] + piState[iState-2])

			& ((1 << 30) - 1);

    }

    return;

}







int number_mm( void )

{

    int *piState;

    int iState1;

    int iState2;

    int iRand;



    piState		= &rgiState[2];

    iState1	 	= piState[-2];

    iState2	 	= piState[-1];

    iRand	 	= (piState[iState1] + piState[iState2])

			& ((1 << 30) - 1);

    piState[iState1]	= iRand;

    if ( ++iState1 == 55 )

	iState1 = 0;

    if ( ++iState2 == 55 )

	iState2 = 0;

    piState[-2]		= iState1;

    piState[-1]		= iState2;

    return iRand >> 6;

}







/*

 * Roll some dice.

 */

int dice( int number, int size )

{

    int idice;

    int sum;



    switch ( size )

    {

    case 0: return 0;

    case 1: return number;

    }



    for ( idice = 0, sum = 0; idice < number; idice++ )

	sum += number_range( 1, size );



    return sum;

}







/*

 * Simple linear interpolation.

 */

int interpolate( int level, int value_00, int value_32 )

{

    return value_00 + level * (value_32 - value_00) / 32;

}







/*

 * Removes the tildes from a string.

 * Used for player-entered strings that go into disk files.

 */

void smash_tilde( char *str )

{

    for ( ; *str != '\0'; str++ )

    {

	if ( *str == '~' )

	    *str = '-';

    }



    return;

}







/*

 * Compare strings, case insensitive.

 * Return TRUE if different

 *   (compatibility with historical functions).

 */

bool str_cmp( const char *astr, const char *bstr )

{

    if ( astr == NULL )

    {

//	bug( "Str_cmp: null astr.", 0 );

	return TRUE;

    }



    if ( bstr == NULL )

    {

//	bug( "Str_cmp: null bstr.", 0 );

	return TRUE;

    }



    for ( ; *astr || *bstr; astr++, bstr++ )

    {

	if ( LOWER(*astr) != LOWER(*bstr) )

	    return TRUE;

    }



    return FALSE;

}







/*

 * Compare strings, case insensitive, for prefix matching.

 * Return TRUE if astr not a prefix of bstr

 *   (compatibility with historical functions).

 */

bool str_prefix( const char *astr, const char *bstr )

{

    if ( astr == NULL )

    {

	bug( "Strn_cmp: null astr.", 0 );

	return TRUE;

    }



    if ( bstr == NULL )

    {

	bug( "Strn_cmp: null bstr.", 0 );

	return TRUE;

    }



    for ( ; *astr; astr++, bstr++ )

    {

	if ( LOWER(*astr) != LOWER(*bstr) )

	    return TRUE;

    }



    return FALSE;

}







/*

 * Compare strings, case insensitive, for match anywhere.

 * Returns TRUE is astr not part of bstr.

 *   (compatibility with historical functions).

 */

bool str_infix( const char *astr, const char *bstr )

{

    int sstr1;

    int sstr2;

    int ichar;

    char c0;



    if ( ( c0 = LOWER(astr[0]) ) == '\0' )

	return FALSE;



    sstr1 = strlen(astr);

    sstr2 = strlen(bstr);



    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )

    {

	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )

	    return FALSE;

    }



    return TRUE;

}







/*

 * Compare strings, case insensitive, for suffix matching.

 * Return TRUE if astr not a suffix of bstr

 *   (compatibility with historical functions).

 */

bool str_suffix( const char *astr, const char *bstr )

{

    int sstr1;

    int sstr2;



    sstr1 = strlen(astr);

    sstr2 = strlen(bstr);

    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )

	return FALSE;

    else

	return TRUE;

}







/*

 * Returns an initial-capped string.

 */

char *capitalize( const char *str )

{

    static char strcap[MAX_STRING_LENGTH];

    int i;



    for ( i = 0; str[i] != '\0'; i++ )

	strcap[i] = LOWER(str[i]);

    strcap[i] = '\0';

    strcap[0] = UPPER(strcap[0]);

    return strcap;

}







/*

 * Append a string to a file.

 */

void append_file( CHAR_DATA *ch, char *file, char *str )

{

    FILE *fp;



    if ( IS_NPC(ch) || str[0] == '\0' )

	return;




    if ( ( fp = fopen( file, "a" ) ) == NULL )

    {

	perror( file );

	send_to_char( "Could not open the file!\n\r", ch );

    }

    else

    {

	fprintf( fp, "[%5d] %s: %s\n",

	    ch->in_room ? ch->in_room->vnum : 0, ch->name, str );

	fclose( fp );

    }




    return;

}


void append(char *filename, char *txt)
{
    FILE *fp = fopen(filename, "a");
    fprintf(fp,txt);
    fflush(fp);
    fclose(fp);
}




/*

 * Reports a bug.

 */

void bug( const char *str, int param )

{

    char buf[MAX_STRING_LENGTH];

//    FILE *fp;



    if ( fpArea != NULL )

    {

	int iLine;

	int iChar;



	if ( fpArea == stdin )

	{

	    iLine = 0;

	}

	else

	{

	    iChar = ftell( fpArea );

	    fseek( fpArea, 0, 0 );

	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )

	    {

		while ( getc( fpArea ) != '\n' )

		    ;

	    }

	    fseek( fpArea, iChar, 0 );

	}



	sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );

//	log_string( buf );

/*

	if ( ( fp = fopen( "shutdown.txt", "a" ) ) != NULL )

	{

	    fprintf( fp, "[*****] %s\n", buf );

	    fclose( fp );

	}

*/
    }



    strcpy( buf, "[*****] BUG: " );

    sprintf( buf + strlen(buf), str, param );

    log_string( buf );




    return;

}







/*

 * Writes a string to the log.

 */

void log_string( const char *str )

{

    char *strtime;

    char logout [MAX_STRING_LENGTH];


if (pwipeit==TRUE) return;

    strtime                    = ctime( &current_time );

    strtime[strlen(strtime)-1] = '\0';

    fprintf( stderr, "%s :: %s\n", strtime, str );

    strcpy ( logout, str );

    logchan( logout );

    return;

}







/*

 * This function is here to aid in debugging.

 * If the last expression in a function is another function call,

 *   gcc likes to generate a JMP instead of a CALL.

 * This is called "tail chaining."

 * It hoses the debugger call stack for that call.

 * So I make this the last call in certain critical functions,

 *   where I really need the call stack to be right for debugging!

 *

 * If you don't understand this, then LEAVE IT ALONE.

 * Don't remove any calls to tail_chain anywhere.

 *

 * -- Furey

 */

void tail_chain( void )

{

    return;

}

void read_datafile(const char *name)
{
    FILE *fp;
    char c;

    if ((fp = fopen(name, "r")) == NULL)
	return;

    while ((c = getc(fp)) == '\n' || c == '\r' || c == ' ')
	;

    ungetc(c, fp);

    if (str_cmp(fread_word(fp), "register"))
    {
	sprintf(log_buf, "Skipping data file \"%s\".", name);
	log_string(log_buf);
	fclose(fp);
	return;
    }

    sprintf(log_buf, "Loading data file \"%s\".", name);
    log_string(log_buf);

    while (!feof(fp))
    {
	char *str = fread_word(fp);

	if (*str == '#')
	{
	    fread_to_eol(fp);
	    continue;
	}

	if (!str_cmp(str, "global"))
	{GLOB_DATA *glob;
	    str = fread_word(fp);
	    glob = alloc_perm(sizeof(*glob));
	    glob->name = str_dup(str);
	    glob->value = str_dup(fread_word(fp));
	    glob->next = global_list;
	    global_list = glob;
	    fread_to_eol(fp);
	    continue;
	}

	if (!str_cmp(str, "globalext"))
	{GLOB_DATA *glob;
	    str = fread_word(fp);
	    glob = alloc_perm(sizeof(*glob));
	    glob->name = str_dup(str);
	    glob->value = fread_string(fp);
	    glob->next = global_list;
	    global_list = glob;
	    fread_to_eol(fp);
	    continue;
	}

	if (!str_cmp(str, "recurse"))
	{
	    DIR *dir;
	    char *dirname = fread_word(fp);

	    if ((dir = opendir(dirname)) == NULL)
	    {
		sprintf(log_buf, "Directory: %s does not exist for recursing.",
		dirname);
		bug(log_buf, 0);
		fread_to_eol(fp);
		continue;
	    }
	    else closedir(dir);

	    fread_to_eol(fp);

	    read_data_recurse_dir(dirname);
	    continue;
	}
    }
    fclose(fp);
}

void read_data_recurse_dir(const char *directory)
{
    DIR *dir;
    struct dirent *fli;
    char start[300];
    struct stat statbuf;

    getcwd(start, 300);

    dir = opendir(directory);
    chdir(directory);
    if (!dir) return;

    while ((fli = readdir(dir)) != NULL)
    {
	if (!str_cmp(fli->d_name, ".") || !str_cmp(fli->d_name,".."))
	    continue;

	if (stat(fli->d_name, &statbuf))
	    continue;

	if (statbuf.st_mode & S_IFDIR)
	    read_data_recurse_dir(fli->d_name);
	else
	    read_datafile(fli->d_name);
    }

    closedir(dir);
    chdir(start);
    return;
}


bool has_slash(char*);
void stripfilename(char *path, char *name)
{int len;
 char *p = path;
    if ((len=strlen(path)) < 2) return;
    strcpy(name,"");
    while (has_slash(path+1))
	path++;

    strcpy(name, path+1);
    *(path+1) = '\0';
    path = p;
{char buf[1000];//back fuck, I know this is really messy but it's hard for me to fix
    strcpy(buf, path);
    strcpy(path, name);
    strcpy(name, buf);
}
}

bool has_slash(char *str)
{
    while (*str)
	if (*str == '/') return TRUE;
	else str++;

    return FALSE;
}

char *global(const char *name)
{
 GLOB_DATA *glob;

 for (glob = global_list; glob; glob = glob->next)
    if (!str_cmp(glob->name, name)) break;

 return glob ? glob->value : "(null)";
}

void load_bootfile()
    {
	char name[1000];
	char dire[1000];
	char cwd[1000];
	FILE *fp;

	getcwd(cwd,1000);
	strcpy(name,datafile);
	stripfilename(name,dire);
	chdir(dire);
	fp = fopen(name, "r");
	if (!fp)
	{
	    sprintf(log_buf, "Invalid data file! :%s", name);
	    log_string(log_buf);
	}
	else if (fp)
	{
	    fclose(fp);
	    read_datafile(name);
	}
	chdir(cwd);
    }


