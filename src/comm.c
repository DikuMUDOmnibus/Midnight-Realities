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

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "merc.h"

void connection_stuff args( ( DESCRIPTOR_DATA *, CHAR_DATA * ) );

void init_descriptor	args( (DESCRIPTOR_DATA *dnew, int desc) );


int wtdsync = 1;
char *wtdbuf=0;

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>	/* for RLIMIT_NOFILE */
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif


#define str_des str_dup(argument)
#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
/* mercpoint: commented out to enable compiling
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );

int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
 kavirpoint: put this in for nin site.
int	read		args( ( int fd, char *buf, int nbyte ) );

int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
 kavirpoint: put this in for nin site.
int	write		args( ( int fd, char *buf, int nbyte ) );
*/
#endif
bool 	bugfind;

void telopt_check(DESCRIPTOR_DATA *d, char *in);
#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif



/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
char		    str_boot_time[MAX_INPUT_LENGTH];
char		    crypt_pwd[MAX_INPUT_LENGTH];
time_t		    current_time;	/* Time of this pulse		*/
int		    arena;
/* Colour scale char list - Calamar */

int oldcon;
char passcrypt[]="smooXmas";

char *scale[SCALE_COLS] = {
	L_RED,
	L_BLUE,
	L_GREEN,
	L_YELLOW
};
#define DD DESCRIPTOR_DATA
/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( DD *desc, char *txt, int length )
);
bool	write_to_descriptor2	args( ( int desc, char *txt, int length )
);
#endif

#if defined(unix)
void	game_loop_unix		args( ( ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( DD *desc, char *txt, int length )
);
bool	write_to_descriptor2	args( ( int desc, char *txt, int length )
);
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_kickoff		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	bust_a_prompt		args( ( DESCRIPTOR_DATA *d ) );
void     bust_a_header          args((DESCRIPTOR_DATA *d));

/* For Copyover */

int port, control;
bool HotBoot = FALSE;
bool bootPoll = FALSE;

void parse_arguments(char **);
//THESE ARGUMENTS ARE NOT CASE SENSATIVE. -a is the same as -A. If you have it set to handle
//"-a" AND "-A" it will only handle the first in the if block.
void parse_arguments(char **arg)
{
  strcpy(workfile, "../work");
  unlink(workfile);
  spidernum = 0;
  spiderquest = FALSE;

  while (*arg)
  {
    if (!str_cmp(*arg, "-E"))
    {extern int xpbonusaa;
	arg++;
	xpbonusaa = atoi(*arg);
    } else
    if (!str_cmp(*arg, "-p"))
    {
	arg++;
	port = atoi(*arg);
    } else

    if (!str_cmp(*arg, "-q"))
    {
	arg++;
	spiderquest = TRUE;
	spidernum = atoi(*arg);
    } else

    if (!str_cmp(*arg, "-A"))
    {
	crashAbort = TRUE;
    } else

    if (!str_cmp(*arg, "-w"))
    {
	arg++;
	strcpy(workfile, *arg);
	unlink(workfile);
    } else

    if (!str_cmp(*arg, "-b"))
    { FILE *fp;
	arg++;
	strcpy(datafile, *arg);
	if ((fp = fopen(datafile, "r")) == NULL)
	{
	    sprintf(log_buf, "Unable to open boot file '%s'. Defaulting to 'boot' in root directory.", datafile);

	    bug(log_buf, 0);
	    strcpy(datafile, "../boot");
	}
	else
	    fclose(fp);
    } else

    if (!str_cmp(*arg, "-poll"))
    {
	bootPoll = TRUE;
    } else

    if (!str_cmp(*arg, "-pwipe"))
    {
	printf("Initiating pwipe sequences. Booting up the mud.\n");
	pwipeit = TRUE;
    } else

    if (!str_cmp(*arg, "-pass"))
    {
	printf("Password locked.");
	passlocked = TRUE;
	arg++;
	strcpy(passlock, *arg);
    } else

    if (!str_cmp(*arg, "-c"))
    {
	arg++;
	HotBoot = TRUE;
	control = atoi(*arg);
    }
  arg++;
  }
}

int main( int argc, char **argv )
{
    struct timeval now_time;

    strcpy(datafile, "../boot");
triviaon = FALSE;
    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        { 
        struct  rlimit rlp;
        (void)getrlimit(RLIMIT_NOFILE, &rlp);
        rlp.rlim_cur=min(rlp.rlim_max,FD_SETSIZE);
        (void)setrlimit(RLIMIT_NOFILE, &rlp);
	}
#endif
    /*
     * Init time and encryption.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );
  if (pwipeit) str_boot_time[strlen(str_boot_time)-1] = 0;
    strcpy( crypt_pwd, "Don't bother." );
port = 9000;
pwipeit=FALSE;
parse_arguments(argv);
pfile = FALSE;
immspar = FALSE;
if (bootPoll){
	append(workfile, "do echo &s&RWarning: &BPolling for a hotboot. &R(&GThis means the mud is checking to make sure it will boot up properly before it continues to hotboot&R)&t");
	sleep(1); // I know, pathetic, but at least it'll look like it's doing some work:P sleep for 2 seconds
}
balancing = FALSE; /* knocks off weapon flags when TRUE, for immspar */

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /* Allocate initiating byte for the write to descriptor sync */
    wtdbuf = malloc(1);

    /*
     * Get the port number.
     */
    if ( !HotBoot && port <= 1024 )
    {
	fprintf( stderr, "Port number must be above 1024.\n" );
	exit( 1 );
    }

if (!HotBoot && !pwipeit && !bootPoll)
control = init_socket(port);

    /*
     * Run the game.
     */
oldcon = control;

if (HotBoot)
hotboot_recover(1);

#if defined(macintosh) || defined(MSDOS)
    boot_db( );

if (HotBoot)
hotboot_recover(2);

    log_string( "God Wars Ready" );
    game_loop_mac_msdos( );
#endif

#if defined(unix)

    boot_db( );
if (bootPoll) {    log_string( "God Wars Ready" );pollDone(Success);}
init_signals();
    sprintf( log_buf, "God Wars up on port %d.", port );
    log_string( log_buf );

if (HotBoot)
hotboot_recover(2);

if (passlocked && !HotBoot)
{char npl[200];
    strcpy(npl, crypt(passlock,passcrypt));
    strcpy(passlock,npl);
    do_hotboot(NULL,"now");
}

if (!pwipeit)
    game_loop_unix( );

    arena = FIGHT_OPEN;

    close( control );
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close( fd );
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
	close( fd );
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
	close( fd );
	exit( 1 );
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= malloc( dcon.outsize );
    dcon.next		= descriptor_list;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }
	    telopt_check(d, d->inbuf);
	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
if (IS_CLASS(d->character, CLASS_DRUID) && !IS_WARP(d->character, REWARD_SLOW) && d->character->wait > 0)
    --d->character->wait;
if (d->character->trust < 7)
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		if (d->watcher)
{
sprintf(log_buf, "<<%d/%s
\r%s>>\n\r", d->descriptor, (d->original) ?
d->original->pcdata->switchname :
d->character ? d->character->pcdata->switchname : "NotConnected",
d->incomm);
if (d->connected != CON_GET_OLD_PASSWORD && d->connected != CON_GET_NEW_PASSWORD
&& d->connected != CON_CONFIRM_NEW_PASSWORD)
write_to_buffer(d->watcher, log_buf, strlen(log_buf));
}
		d->fcommand	= TRUE;
		stop_idling( d->character );

		switch( d->connected )
		{
	     	        default:
 			nanny( d, d->incomm );
			break;
		   case CON_PLAYING:
if (d->character <= 0xff)
{write_to_descriptor("You are bugged.\n\r", 0);d->character=0;d->connected=CON_GETNAME;return;}
			interpret( d->character, d->incomm );
			break;
		   case CON_EDITING:
			edit_buffer( d->character, d->incomm );
			break;
		  }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL )
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    telopt_check(&dcon,&dcon->inbuf);

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)

void excessive_cpu(int blx)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) )
	{if (vch->desc)
	    write_to_descriptor(vch->desc, "&BOH NO! &YTHE MUD has &Wfrozen!&B attempting to get out of it.\n\r", 0);
	}
    }
    raise(SIGSEGV);
}

void game_loop_unix( )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;
strcpy( mud_time, ctime( &current_time) );
mud_time[strlen(mud_time)-1] = '\0';

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

if (control == 0)
{ log_string("Uhhohh. Control == 0. Downtime aborted.");
if (oldcon)
    control = oldcon;
else
    control = init_socket(port);
}

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
/* kavirpoint
	maxdesc	= control * 2;
*/
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    telopt_check(d,d->inbuf);

	    if ( d->character != NULL && d->character->wait2 > 0)
		--d->character->wait2;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
if (IS_CLASS(d->character, CLASS_DRUID) && !IS_WARP(d->character, REWARD_SLOW) && d->character->wait > 0)
    --d->character->wait;
if (d->character->trust < 7)
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		if (d->watcher)
{
sprintf(log_buf, "<<%d/%s
\r%s>>\n\r", d->descriptor, (d->original) ?
d->original->pcdata->switchname :
d->character ? d->character->pcdata->switchname : "NotConnected",
d->incomm);
if (d->connected != CON_GET_OLD_PASSWORD && d->connected !=
CON_GET_NEW_PASSWORD
&& d->connected != CON_CONFIRM_NEW_PASSWORD)
write_to_buffer(d->watcher, log_buf, strlen(log_buf));
}
		d->fcommand	= TRUE;
		stop_idling( d->character );

		switch( d->connected )
			  {
			   default:
 				nanny( d, d->incomm );
				break;

			   case CON_PLAYING:
				interpret( d->character, d->incomm );
				break;
			   case CON_EDITING:
				edit_buffer( d->character, d->incomm );
				break;
			  }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}
check_resolve();
{FILE *fp = fopen(workfile, "r");
	if (fp)
	{
	    globWFD = fp;
	    workfile_compute(fp); // fclose and unlink is in workfile_compute()
	    globWFD = NULL; // globWFD is so that if it crashes before the fclose in workfile_compute
			    // or a copyover is called in a workfile, the fp will close.
	}
}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
strcpy( mud_time, ctime( &current_time) );
mud_time[strlen(mud_time)-1] = '\0';
    }

    return;
}
#endif



#if defined(unix)
void new_descriptor( )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    struct sockaddr_in sock;
    int desc =0;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {static DESCRIPTOR_DATA dn;
	dnew		= alloc_perm( sizeof(*dnew) );
	*dnew = dn;
    }
    else
    {static DESCRIPTOR_DATA dn;
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
	*dnew = dn;
    }

    init_descriptor(dnew, desc);

    size = sizeof(sock);
    if (dnew->host) free_string(dnew->host);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, buf ) ||
!str_prefix(pban->name, buf) || !str_suffix(pban->name, buf) ||
!str_prefix(pban->name, buf))
	{
	    write_to_descriptor2( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close( desc );
	    free( dnew->outbuf);
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

	sprintf( log_buf, "<<%d>>Sock.sinaddr:  %s", desc, buf );
	log_string( log_buf );
	sprintf(log_buf, "../hlook %s ../src/lookup%d&", buf, port);
	system(log_buf);

	dnew->host = str_dup( buf );
    }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) ||
!str_prefix(pban->name, dnew->host) || !str_suffix(pban->name, buf) ||
!str_prefix(pban->name, dnew->host))
	{
	    write_to_descriptor2( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close( desc );
	    free_string( dnew->host );
	    free( dnew->outbuf);
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

        if ( !str_suffix( "brentwood.ny.da.uu.net", dnew->host)
	|| !str_suffix("mgec.EDnet.NS.CA", dnew->host))
	
        {
            write_to_descriptor2( desc,
                "Your site has been banned from this Mud for repeated abuse.\n\r", 0 );
            close( desc );
            free_string( dnew->host );
            free( dnew->outbuf);
            dnew->next          = descriptor_free;
            descriptor_free     = dnew;
            return;
        }
    
        if ( !str_suffix( "DOL.STATE.VT.US", dnew->host) && (dnew->character != NULL))
	{
            write_to_descriptor2( desc,
                "Your site has been newbie banned from this Mud.\n\r", 0 );
            close( desc );
            free_string( dnew->host );
            free( dnew->outbuf);
            dnew->next          = descriptor_free;
            descriptor_free     = dnew;
            return;
        }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    if (!passlocked)
    /*
     * Send the greeting.
     */
    {int timediff = TimeStamp(mud_time)-crashStamp();
	char bigbuf[2000];
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );

	sprintf(bigbuf, "&YWe have not crashed in %d hours, %d minutes and %d seconds.
\r&b$$hello|&w", (timediff/(60*60)), (((timediff/60)*60)-((timediff/(60*60))*60*60))/60, timediff-((timediff/60)*60));
	write_to_buffer(dnew, bigbuf, 0);
    }
    else
    {
	write_to_buffer(dnew, "The mud is currently locked by a password: ", 0);
	dnew->connected = CON_PASSLOCK;
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
DESCRIPTOR_DATA *d = dclose;
    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

if (d->watcher)
{
sprintf(log_buf, "<<%d/%s
\rHas Left the Game.>>\n\r", d->descriptor, d->original ?
d->original->pcdata->switchname : d->character ?
d->character->pcdata->switchname : "NotPlaying");
write_to_buffer(d->watcher, log_buf, strlen(log_buf));
d->watcher->watching = NULL;
d->watcher = NULL;
}
if (d->watching)
{
    d->watching->watcher = NULL;
    d->watching = NULL;
}
    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && 
	(dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING) &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& dclose->connected == CON_PLAYING
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	if ( dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )
	{
	    if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
	    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	            if ( IS_SET(ch->act,PLR_CHALLENGED) && arena == FIGHT_START )
               do_decline(ch,ch->challenger->name);
            if ( IS_SET(ch->act,PLR_CHALLENGER) && arena == FIGHT_START )
            {
               char buf[MAX_STRING_LENGTH];

               REMOVE_BIT(ch->act,PLR_CHALLENGER);
               REMOVE_BIT(ch->challenged->act,PLR_CHALLENGED);
               ch->challenged->challenger = NULL;
               ch->challenged = NULL;
               arena = FIGHT_OPEN;
     sprintf(buf, "[Arena] %s has lost $s link. Arena is OPEN.", ch->name);
 {
 DESCRIPTOR_DATA *d;
 for ( d = descriptor_list; d; d = d->next )
  
     if( d->connected == CON_PLAYING || d->connected == CON_EDITING)
     {
        send_to_char( buf, d->character );
     }
  }
            }
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host );

    /* RT socket leak fix */
    free( dclose->outbuf);

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}

/* For a better kickoff message :) KaVir */
void close_socket2( DESCRIPTOR_DATA *dclose, bool kickoff )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && 
       (dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )&&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& ( dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING)
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING
		|| dclose->connected == CON_EDITING )
	{
	    if (kickoff)
		act( "$n doubles over in agony and $s eyes roll up into $s head.", ch, NULL, NULL, TO_ROOM );
 	    save_char_obj( ch );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }
    close( dclose->descriptor );
    free_string( dclose->host );
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	if (d != NULL && d->character != NULL)
	    sprintf( log_buf, "%s input overflow!", d->character->lasthost );
	else
	    sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );

	write_to_descriptor( d,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK )
	{   telopt_check(d,d->inbuf);
	    break;}
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 35 ) //for global expantions.
	{
	    write_to_descriptor( d, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 40 )
	    {
		if (d != NULL && d->character != NULL)
		    sprintf( log_buf, "%s input spamming!", d->character->lasthost );
		else
		    sprintf( log_buf, "%s input spamming!", d->host );
//		log_string( log_buf );
		write_to_descriptor( d,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;

	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

		
           
	gamble_prompt(ch);

	if (IS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( IS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];
	    int per;

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%lu", srew?rand():ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, EXP(ch,1000));
/*
	        sprintf( buf, "[%s exp] <?hp ?m ?mv> ",exp_str );
*/
	        sprintf( buf, "<[%sX] [?H ?M ?V]> ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING && !ch->newfighting)
	    {
	         per = ch->fighting->hit * 100 / ch->fighting->max_hit;
	         if (per >= 100)sprintf(cond, "[   &CPerfect#n   ]");
	    else if (per >= 75) sprintf(cond, "[    &YGood#n     ]");
	    else if (per >= 50) sprintf(cond, "[    &GFair#n     ]");
	    else if (per >= 25) sprintf(cond, "[    &BPoor#n     ]");
	    else if (per >= 0)  sprintf(cond, "[    &RAwful#n    ]");
	    else                sprintf(cond, "[   &RMorted#n    ]");
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf( buf, "<%s [%sH %sM %sV]> ", cond, hit_str, mana_str, move_str );
	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "%lu", srew?rand():ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, EXP(ch,1000));
/*
	        sprintf( buf, "[%s exp] <%shp %sm %smv> ",exp_str, hit_str, mana_str, move_str );
	        sprintf( buf, "<[%sX] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
*/
	        sprintf( buf, "<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, "&w", 0);
	    write_to_buffer( d, buf, 0 );
	}

   if (IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_WIZINVIS))
   {
	sprintf(log_buf, "&Y[&CW&Y] &x");
	write_to_buffer(d, log_buf, 0);
   }
   if (IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_INCOG))
   {
	sprintf(log_buf, "&Y[&CI&Y] &x");
	write_to_buffer(d, log_buf, 0);
   }

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, " ", 0 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    wtdsync=0;
    if ( !write_to_descriptor( d, d->outbuf, d->outtop ) )
    {
	if (wtdsync)
	{
	    strncpy(d->outbuf, wtdbuf, d->outsize);
	    d->outtop = wtdsync>d->outsize?d->outsize:wtdsync;
	    return TRUE;
	}
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }

}

/*
 * Append onto an output buffer.
 */
void write_to_buffer(DD *d, const char *text, int length)
{
    char ccode,*tb;
    char *txt;
    int i,j;
    int collen=20;
    bool ansi;
if (immspar) return;
/*
    if (d != NULL && d->as != NULL) d = d->as;
*/

if (!d) return;
if (d->watcher)
    write_to_buffer(d->watcher, text, length);

    /*
     * Find length in case caller didn't.
     */
    if (length<=0)
	length = strlen(text);

    if (d->character == NULL ||
	(d->connected != CON_PLAYING && d->connected != CON_EDITING))
	ansi = FALSE;
    else ansi = (IS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]    = '\n';
	d->outbuf[1]    = '\r';
	d->outtop       = 2;
    }

    /*
     * Expand d->outbuf for ansi info
     */
txt = str_dup(text); // free_string below.


//MUST NOT return before free_string or there will be a memory leak!
{ char *p = txt;
while (*p) {if (*p == '#') collen+= 10;else collen++;p++;}
}
tb = malloc(collen);
    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
	if (txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N')
	{
	    if (txt[++i] == 'B')
	    {
		ccode = '3';
		i++;
	    }
	    else ccode = '3';
	if (txt[i] == '*')
	{ switch(number_range(0,7))
	  {
	    case 0: txt[i] = '0'; break;
	    case 1: txt[i] = '1'; break;
	    case 2: txt[i] = '2'; break;
	    case 3: txt[i] = '3'; break;
	    case 4: txt[i] = '4'; break;
	    case 5: txt[i] = '5'; break;
	    case 6: txt[i] = '6'; break;
	    case 7: txt[i] = '7'; break;
	  }
	}
	    switch (txt[i])
	    {
		default : break;
		case '#': tb[j++] = '#'; break;
		case '-': tb[j++] = '~'; break;
		case '+': tb[j++] = '%'; break;
		case 'I':
		case 'b': tb[j++] = '&'; tb[j++] = 'W';
		case 'N':
		case 'n':
			tb[j++] = '&'; tb[j++] = 'x';
		    break;
		case '0':
			tb[j++]='&'; tb[j++]='D';
		    break;
		case '1': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='R';
		    break;
		case '2': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='G';
		    break;
		case '3': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='Y';
		    break;
		case '4': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='B';
		    break;
		case '5': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='M';
		    break;
		case '6': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='C';
		    break;
		case '7': if (!ansi) continue;
			tb[j++]='&'; tb[j++]='W';
		    break;
	    }
	}
	else tb[j++] = txt[i];
    }

    tb[j]   = '\0';

    length = j;

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	outbuf      = malloc( d->outsize+collen+1 );
	strncpy( outbuf, d->outbuf, d->outtop );
	free( d->outbuf);
	d->outbuf   = outbuf;
	d->outsize += collen+1;
    }

free_string(txt);
    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, tb );
    d->outtop += length;
    free(tb); // the mud MUST NOT return before this.
    return;
}

int replace_occurance(char *buf, char *find, char *replace)
{
    int count = 0;

    while (*buf)
    {
	count++;

	if (!str_prefix(find, buf))
	{
	    count--;
	    while (*buf && *replace)
	    {
		*buf = *replace == '\a' ? '\0' : *replace;
		buf++;
		replace++;
		count++;
	    }
	    return count;
	}
	buf++;
    }
    return -1;
}

void expand_aliases(char*,char*);
int count_aliases(char*);
bool write_to_descriptor_color( DD*, char *, int);
bool write_to_descriptor_real( DD*, const char *, int);
bool write_to_descriptor( DD *d, char *txt, int length)
{
 bool ret;
 char *buf=NULL;
 int nlen = count_aliases(txt);
 buf = malloc(sizeof(char[nlen+1]));

 expand_aliases(buf, txt);
 ret = write_to_descriptor_color(d, buf, nlen);
 free(buf);
 return ret;
}

void cpy_inter(char*,char*,char);
int count_aliases(char *txt)
{
    char *newbuf;
    int c = 0;
int len = strlen(txt);
newbuf = malloc(len);

    while (*txt)
    {
	if (*txt == '$' && *(txt+1) == '$')
	{
	    txt += 2;
	    cpy_inter(newbuf, txt, '|');

	    if (str_cmp((global(newbuf)), "(null)"))
	    {
		c += strlen(global(newbuf));
		while (*txt != '|')
		    txt++;
		txt++;
	    }
	    else c += 2;
	}
	else {c++;
	txt++;}
    }

free(newbuf);
    return c;
}


void expand_aliases(char *newb, char *txt)
{
    char *name; int len = strlen(txt);
name = malloc(len);
    while (*txt)
    {
	if (*txt == '$' && *(txt+1) == '$')
	{
	    txt += 2;
	    cpy_inter(name, txt, '|');
	    if (str_cmp(global(name), "(null)"))
	    {char *glob = global(name);
		*newb='\0';strcat(newb, glob); newb += strlen(glob); while (*txt != '|') txt++;txt++;
	    } else {
		*newb = '$'; newb++; *newb = '$'; newb++;
	    }
	}
	else {*newb = *txt;newb++;txt++;}
    }
    *newb = '\0';
free(name);
    return;
}

void cpy_inter(char *newb, char *oldb, char inter)
{
    while (*oldb)
    {
	if (*oldb == inter)
	    break;

	*newb = *oldb;
	oldb++, newb++;
    }

    *newb = '\0';

    return;
}

char *color(char);
int count_colors(char *);
void expand_colors(char *,char *);
bool write_to_descriptor_color(DD *d, char *txt, int length)
{
 bool ret;
 char *buf=NULL;
 int nlen = count_colors(txt);

 buf = malloc(sizeof(char[nlen+1]));
 expand_colors(buf, txt);
 ret = write_to_descriptor_real(d, buf, nlen);
 free(buf);
 return ret;
}

int count_colors(char *txt)
{
    int c = 0;

    color('w'); // give it a standard to go by, if this is removed there will be crashes from &s and &t.

    while (*txt)
    {
	if (*txt == '&')
	{
	    c += strlen(color(*(txt+1)));
	    txt++;
	}
	else c++;
	txt++;
    }

    return c;
}

void expand_colors(char *newb, char *txt)
{

    color('w'); // give it a standard to go by, if this is removed there will be crashes from &s and &t

    while (*txt)
    {
	if (*txt == '&')
	{
	    *newb = 0; strcat(newb, color(*(txt+1)));
	    newb += strlen(color(*(txt+1)));
	    txt += 2;
	    continue;
	}
	else {*newb = *txt; newb++;}
	txt++;
    }
    *newb = 0;

    return;
}

/* don't use this with sprintf or other format crap.. use &(code) in the 
string instead or it will royally fuck up*/
char *color(char c)
{
    static char current[20]="", last[20]="", stored[20]="";
    static char code[20];

    if (c == '*')
 {int rand = number_range(1,15);
c =
rand == 1 ? 'b' : rand == 2 ? 'c' : rand == 3 ? 'g' :
rand == 4 ? 'm' : rand == 5 ? 'r' : rand == 6 ? 'w' :
rand == 7 ? 'y' : rand == 8 ? 'B' : rand == 9 ? 'C' :
rand == 10 ? 'G' : rand == 11 ? 'M' : rand == 12 ? 'R' 
: rand == 13 ? 'W' : rand == 14 ? 'Y' : 'D';}
strcpy(code, "");

    switch (c)
    {
	default: case 'x':
	    sprintf(code, "%c[1;07m%c[0m", 27, 27);
	    sprintf(last, current);
	    break;
	case 'b':
	    sprintf(code, global("Col:Blue"));
	    sprintf(last, current);
	    break;

	case 'c':
	    sprintf(code, global("Col:Cyan"));
	    sprintf(last, current);
	    break;
	case 'g':
	    sprintf(code, global("Col:Green"));
	    sprintf(last, current);
	    break;
	case 'm':
	    sprintf(code, global("Col:Magenta"));
	    sprintf(last, current);
	    break;
	case 'r':
	    sprintf(code, global("Col:Red"));
	    sprintf(last, current);
	    break;
	case 'w':
	    sprintf(code, global("Col:White"));
	    sprintf(last, current);
	    break;
	case 'y':
	    sprintf(code, global("Col:Orange"));
	    sprintf(last, current);
	    break;
	case 'B':
	    sprintf(code, global("Col:B:Blue"));
	    sprintf(last, current);
	    break;
	case 'C':
	    sprintf(code, global("Col:B:Cyan"));
	    sprintf(last, current);
	    break;
	case 'G':
	    sprintf(code, global("Col:B:Green"));
	    sprintf(last, current);
	    break;
	case 'M':
	    sprintf(code, global("Col:B:Magenta"));
	    sprintf(last, current);
	    break;
	case 'R':
	    sprintf(code, global("Col:B:Red"));
	    sprintf(last, current);
	    break;
	case 'W':
	    sprintf(code, global("Col:B:White"));
	    sprintf(last, current);
	    break;
	case 'Y':
	    sprintf(code, global("Col:Yellow"));
	    sprintf(last, current);
	    break;
	case 'D':
	    sprintf(code, global("Col:Grey"));
	    sprintf(last, current);
	    break;
	case 'l':
	    sprintf(code, last);
	    sprintf(last, current);
	    break;
	case ' ':
	    strcpy(code, "& ");
	    break;
	case '&': case 0:
	    strcpy(code, "&");
	    break;
	case 's':
	    sprintf(stored, current);
	    break;
	case 't':
	    sprintf(code, stored);
	    break;
    }
if (c != '{' && c != 's' && c != 't')
    sprintf(current, code);

 return code;
}
	

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor_real( DD *d, const char *txt, int length )
{int desc = d->descriptor;
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif
//    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    {
	switch(errno)
	{
	    default: perror("Write_to_descriptor");
		return FALSE;
	    case EAGAIN:
		free(wtdbuf);
		wtdsync=length-iStart;
		if (wtdsync<0) wtdsync=0;
		wtdsync++; // extra bit for null-terminator
		wtdbuf = malloc(wtdsync);  // extra bit for null-terminator
		strncpy(wtdbuf,txt+iStart,wtdsync);
		return FALSE; // it will handle this in bool process_output
	}   }
    } 

    return TRUE;
}

bool write_to_descriptor2( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif
    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    BAN_DATA *pban;
    char buf[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    char *strtime;
    bool fOld = FALSE;

    while ( isspace(*argument) )
	argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected d.", d->connected );
	close_socket( d );
	return;

    case CON_PASSLOCK:
	if (str_cmp(crypt(argument, passcrypt), passlock) && passlocked)
	{
	    write_to_descriptor(d,"Wrong password.\n\rRemember for next time you try, this is NOT your character's password, this is the muds password.\n\r", 0);
	    close_socket(d);
	    return;
	}
	
    /*
     * Send the greeting.
     */
    {int timediff = TimeStamp(mud_time)-crashStamp();
	char bigbuf[2000];
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( d, help_greeting+1, 0 );
	else
	    write_to_buffer( d, help_greeting  , 0 );

	sprintf(bigbuf, "&YWe have not crashed in %d hours, %d minutes and %d seconds.
\r&b$$hello|&w", (timediff/(60*60)), (((timediff/60)*60)-((timediff/(60*60))*60*60))/60, timediff-((timediff/60)*60));
	write_to_buffer(d, bigbuf, 0);
    }

	d->connected = CON_GET_NAME;
	break;


    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	sprintf(kav,"<<%d>>%s trying to connect.", d->descriptor, argument);
	log_string( kav );

	fOld = load_char_short( d, argument );
	ch   = d->character;
/*	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}
*/
	if ( IS_SET(ch->act, PLR_DENY) )
	{
	    sprintf( log_buf, "<<%d>>Denying access to %s.",
d->descriptor, argument/*, ch->lasthost*/ );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    if ( wizlock && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    ch = d->character;
	    if (ch && !str_cmp(ch->pcdata->pwd,""))
	    {
		write_to_buffer(d, "There is a great security problem with your character, anybody can log it on by typing in anything
\rfor the password. I can not allow you to log on. To gain access back to your character, you must contact an owner or coder, and be
\rable to prove that you are the same person (for example having the same IP address, however if you do not, this will not disqualify
\ryou from getting your character back.
\rType in another name to log in as:", 0);
		extract_char(ch, TRUE);
		d->character = NULL;
		return;
	    }

	    write_to_buffer( d, "Please enter password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
	    sprintf( buf, "You want %s engraved on your tombstone (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;

	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif


	if ( ch == NULL || (!IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( ch == NULL || (IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, GET_PC_NAME(ch), TRUE ) )
	    return;

	if ( check_playing( d, GET_PC_NAME(ch) ) )
	    return;
/*
	if ( check_kickoff( d, ch->name, TRUE ) )
	    return;

	** Avoid nasty duplication bug - KaVir */
	if (ch->level > 1)
	{
	    sprintf(kav,ch->pcdata->switchname);
	    free_char(d->character);
	    d->character = NULL;
	    fOld = load_char_obj( d, kav );
	    ch   = d->character;
	}

	if ( !IS_EXTRA(ch,EXTRA_NEWPASS) && strlen(argument) > 1) {
		sprintf(kav,"%s %s",argument,argument);
		do_password(ch,kav);}

	if (ch->lasthost != NULL) free_string(ch->lasthost);
	if (ch->desc != NULL && ch->desc->host != NULL)
	{
	    if (!str_cmp(ch->pcdata->switchname, "Goliath"))
		ch->lasthost = str_dup("localhost");
	    else ch->lasthost = str_dup(ch->desc->host);
	}
	else
	    ch->lasthost = str_dup("(unknown)");
	strtime = ctime( &current_time );
	strtime[strlen(strtime)-1] = '\0';
	free_string(ch->lasttime);
	ch->lasttime = str_dup( strtime );
	sprintf( log_buf, "<<%d>>%s@%s has connected.",d->descriptor,ch->name, ch->lasthost );
	log_string( log_buf );
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, ch->lasthost ) ||
!str_prefix(pban->name, ch->lasthost) || !str_suffix(pban->name,
ch->lasthost))
	{
	    write_to_descriptor2( d->descriptor,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close_socket(d);
	    return;
	}
    }

	/* In case we have level 4+ players from another merc mud, or 
	 * players who have somehow got file access and changed their pfiles.
	 */
	if ( ch->level > 3 && ch->trust == 0)
	    ch->level = 3;
	else
	{
	    if ( ch->level > MAX_LEVEL )
		ch->level = MAX_LEVEL;
	    if ( ch->trust > MAX_LEVEL)
		ch->trust = MAX_LEVEL;
	    /* To temporarily grant higher powers...
	    if ( ch->trust > ch->level)
		ch->trust = ch->level;
	    */
	}
	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}

	if (ch->level >= 7)
	{
	    stc("Would you like to log on INVISIBLY or VISIBLY? [I/V]: ", ch);
	    d->connected = CON_DO_YOU_WANT_TO_LOG_ON_INVISIBLY;
	    return;
	}
	connection_stuff(d, ch);
	break;
    case CON_DO_YOU_WANT_TO_LOG_ON_INVISIBLY:
	if (LOWER(*argument) == 'i')
	{
	    SET_BIT(ch->act, PLR_WIZINVIS);
	    stc("logging on invisibly.\n\r", ch);
	    do_help(ch, "imotd");
connection_stuff(d, ch);
d->connected = CON_PLAYING;
//	    d->connected = CON_READ_MOTD;
	    return;
	}
	else if (LOWER(*argument) == 'v')
	{
	    REMOVE_BIT(ch->act, PLR_WIZINVIS);
	    stc("Logging on visibly.\n\r", ch);
	    do_help(ch, "imotd");
connection_stuff(d, ch);
d->connected = CON_PLAYING;
//	    d->connected = CON_READ_MOTD;
	    return;
	}
	else stc("WOULD YOU LIKE TO log on INVISIBLY or VISIBLY? [i/v]: ", ch);
	return;
    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

    	pwdnew = crypt(argument, ch->name);

	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );

	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	      case 'm': case 'M': ch->sex = SEX_MALE;    break;
	      case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	      default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
/*	 write_to_buffer( d, echo_on_str, 0 );

	write_to_buffer( d, "You may choose a class now, or have someone induct you later.\n\r", 0);
	write_to_buffer( d, "You Can change Classes By DETOXING.\n\r\n\r", 0);
	write_to_buffer( d, "	(W)erewolf    - hairy, clawwed beast\n\r", 0);
	write_to_buffer( d, "   (S)hapeshifter- Special Metamorphing Humanoids\n\r", 0);
	write_to_buffer( d, "	(V)ampire     - bloodthirsty creatures of the night\n\r", 0);
	write_to_buffer( d, "	(D)emon       - the soulless damned of hell\n\r", 0);
	write_to_buffer( d, "	D(r)ow        - powerful dark elves\n\r", 0);
	write_to_buffer( d, "   J(e)di        - Masters of The force, Deadly Swordsmen\n\r", 0);
	write_to_buffer( d, "	(M)onk        - Servants of God.\n\r", 0);
	write_to_buffer( d, "   Amazon Is for Females, Slaves talk to an Immortal\n\r", 0);
	write_to_buffer( d, "   Magi Seek out Grand Sorcerers!\n\r", 0);
	write_to_buffer( d, "   Nin(j)a       - Masters of there surroundings\n\r", 0);
	write_to_buffer( d, "	(N)one!	      - look for a class later\n\r\n\r", 0);
		  write_to_buffer( d, "What is your class (W/S/V/D/R/E/M/J/N)? ", 0 );
		  write_to_buffer( d, "There are other classes that will
be in soon\n\r", 0);*/
/*	d->connected = CON_GET_NEW_CLASS;
		  break;

	 case CON_GET_NEW_CLASS:*/
/*
		  switch ( argument[0] )
		  {
	case 'w': case 'W':
	ch->class = CLASS_WEREWOLF;
	ch->trust = 0;
	ch->level = 2;
	break;

	case 's': case 'S':
	ch->class = CLASS_SHAPE;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'e': case 'E':
	ch->class = CLASS_JEDI;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'v': case 'V':
	ch->class = CLASS_VAMPIRE;
	ch->generation = 13;
	ch->beast = 30;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'd': case 'D':
	ch->class = CLASS_DEMON;
	ch->generation = 1;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'r': case 'R':
	ch->class = CLASS_DROW;
	ch->generation = 3;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'm': case 'M':
	ch->class = CLASS_MONK;
	ch->generation = 3;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'j': case 'J':
	ch->class = CLASS_NINJA;
	ch->generation = 3;
	ch->trust = 0;
	ch->level = 2;
	break;
	
	case 'n': case 'N': 
	ch->class = 0;
	ch->trust = 0;
	ch->level = 2;
	break;

		  default:
				write_to_buffer( d, "That's not a class.\n\rWhat IS your class (W/S/V/D/R/H/E/M/J/N)? ", 0);
				return;
		  }*/


	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "Out of curiosity, have you ever played this mud before? [Y/N] ", 0);
	d->connected = CON_SURVEY;
	break;

    case CON_SURVEY:
	switch (LOWER(*argument))
	{
	    default:
		write_to_buffer(d, "This survey helps us to determine how well we are doing with attracting new players, have you ever played this mud before? [Y/N] ", 0);
		break;
	    case 'y':
		log_string("Old player being created.");
	write_to_buffer( d, "Does your terminal support ANSI (y/n)? ", 0);
	d->connected = CON_GET_NEW_ANSI;
		break;
	    case 'n':
		log_string("New player being created.");
	write_to_buffer( d, "Does your terminal support ANSI (y/n)? ", 0);
	d->connected = CON_GET_NEW_ANSI;
		break;
	}

	break;

    case CON_GET_NEW_ANSI:
	switch ( argument[0] )
	{
	case 'y': case 'Y': SET_BIT(ch->act,PLR_ANSI);    break;
	case 'n': case 'N': break;
	default:
	    write_to_buffer( d, "Does your terminal support ANSI? (y/n)? ", 0 );
	    return;
	}

	ch->pcdata->perm_str=number_range(10,16);
        ch->pcdata->perm_int=number_range(10,16);
        ch->pcdata->perm_wis=number_range(10,16);
        ch->pcdata->perm_dex=number_range(10,16);
	ch->pcdata->perm_con=number_range(10,16);
	sprintf( log_buf, "<<%d>>%s@%s new player.",d->descriptor, ch->name, d->host );
	log_string( log_buf );
	write_to_buffer( d, "\n\r", 2 );
	do_help( ch, "motd" );
	d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
	connection_stuff( d, ch );
	break;
    }
    return;
}


/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{//leo is here because it's a mobs name, and a player wanted to use it
 if (!str_cmp(name,"leo")) return TRUE;
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto immortal self someone gaia immy mudaholic xero" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) /*&& *pc != '#'*/)
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch) && !IS_EXTRA(ch, EXTRA_SWITCH)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character), GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
	    	if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "<<%d>>%s@%s reconnected.", d->descriptor, ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Kick off old connection.  KaVir.
 */
bool check_kickoff( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character),GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "You take over your body, which was already in use.\n\r", ch );
		act( "...$n's body has been taken over by another spirit!", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "<<%d>>%s@%s kicking off old link.", d->descriptor, ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold != NULL; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? GET_PC_NAME(dold->original) : GET_PC_NAME(dold->character) ) )
	{
	    char	buf [MAX_STRING_LENGTH];
	    if (dold->character->in_room == NULL) continue;
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    send_to_char("This body has been taken over!\n\r",dold->character);
	    d->connected = CON_PLAYING;
	    d->character = dold->character;
	    d->character->desc = d;
	    send_to_char( "You take over your body, which was already in use.\n\r", d->character );
	    act( "$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_ROOM );
	    act( "...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM );
	    dold->character=NULL;
	    
	    sprintf(buf,"<<%d>>Kicking off old connection %s@%s", d->descriptor, d->character->name,d->host);
	    log_string(buf);
	    close_socket(dold);	/*Slam the old connection into the ether*/
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   (ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_EDITING)
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}


void stc( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, 0 );
    return;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - strlen(bash_color(txt))) / 2;
    for (pos = 0; pos < len; pos++)
    {
	buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",
ch); }

void divide2_to_char(CHAR_DATA *ch)
{
send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",
ch); }

void divide3_to_char(CHAR_DATA *ch)
{
send_to_char("===============================================================================\r\n",ch);
}

void divide4_to_char(CHAR_DATA *ch)
{
send_to_char("&B-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C***&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-\r\n",ch);
}

void divide5_to_char(CHAR_DATA *ch)
{
cent_to_char("&B-=[&C***********&B]=-------------=[&C***********&B]=-#n",ch);
}

void divide6_to_char(CHAR_DATA *ch)
{
cent_to_char("&B-    -   -  - - -&C- ---====&W*&C====--- -&B- - -  -   -    -",ch); }

void banner_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#b&B-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=#n[                               ]&B=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-#n"); 
	wdth = (31 - ln) / 2 + 20;
}
else
{
	sprintf(buf,
"#b&B-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=#n[                ]&B=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-=[&C**&B]=-#n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 22] = txt[loop];
cent_to_char(buf,ch);
}

void banner2_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "&B    -   -  - - -&C- ---===&W                               &C===--- -&B- - -  -   -\r\n"); 
	wdth = (31 - ln) / 2 + 24;
}
else
{
	sprintf(buf, "&B     -    -   -  - - -&C- ---====&W                &C====--- -&B- - -  -   -    -\r\n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 6] = txt[loop];
send_to_char(buf,ch);
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    CHAR_DATA *wizard;
    CHAR_DATA *familiar;

    if ( ch->desc == NULL && IS_NPC(ch) && (wizard = ch->wizard) != NULL )
    {
	if (!IS_NPC(wizard) && (familiar = wizard->pcdata->familiar) != NULL 
	    && familiar == ch && ch->in_room != wizard->in_room)
	{
	    send_to_char("[ ",wizard);
	    if ( txt != NULL && wizard->desc != NULL )
		write_to_buffer( wizard->desc, txt, strlen(txt) );
	}
    }

    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, 0 );
    return;
}



/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;

    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;

    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		/* bug( "Act: missing arg2 for code d.", *str );*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'N': i = PERS( vch, to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
	write_to_buffer( to->desc, "#n", 2);

	if (is_fam) to = to_old;

    }
    return;
}

void act2( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;

	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {

		/*bug( "Act: missing arg2 for code d.", *str );*/

		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  i = " "; break;
		case 'n': if ( ch != NULL ) i = PERS( ch,  to  );
			  else i = " ";
			  break;
		case 'N': if ( vch != NULL ) i = PERS( vch,  to  );
			  else i = " ";
			  break;
		case 'e': if ( ch != NULL ) i=he_she  [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'E': if (vch != NULL ) i=he_she  [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'm': if ( ch != NULL ) i=him_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'M': if (vch != NULL ) i=him_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 's': if ( ch != NULL ) i=his_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'S': if (vch != NULL ) i=his_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'p':
		    if (obj1 != NULL)
		    {
			i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;

		case 'P':
		    if (obj2 != NULL)
		    {
			i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );

	if (is_fam) to = to_old;

    }
    return;
}



void kavitem( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char kav[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;
    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok) continue;
    	}
	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
		i = "";
	    else
	    {
		switch ( *str )
		{
		default:  i = "";					break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'o':
		    if (obj1 != NULL) sprintf(kav,"%s's",obj1->short_descr);
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "your" : kav)
			    : "something's";
		    break;

		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}
void bust_a_header(DESCRIPTOR_DATA *d)
{
char class[MAX_STRING_LENGTH];
char class2[MAX_STRING_LENGTH];
char header[MAX_STRING_LENGTH];
char header1[MAX_STRING_LENGTH];
char blanklin[MAX_STRING_LENGTH];
CHAR_DATA *ch;
char cls[MAX_STRING_LENGTH];
sprintf(cls," ");

ch=d->character;
if (ch==NULL) return;

ADD_COLOUR(ch,cls,NORMAL);

if (IS_CLASS(ch,CLASS_VAMPIRE)) sprintf(class,"Vampire");
else if (IS_CLASS(ch,CLASS_DEMON)) sprintf(class,"Demon");
else if (IS_CLASS(ch,CLASS_WEREWOLF)) sprintf(class,"Werewolf");
else sprintf(class,"Classless"); 
sprintf(class2,"%s the %s",ch->name,class);
sprintf(blanklin," ");
sprintf(header1,"%-30s Align:%-4d",class2,ch->alignment);
sprintf(header,
"\0337\033[1;1H\033[1;44m\033[1;37m-79%s%s\0338",header1,cls);
send_to_char(header,ch);
return;
}
/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
         CHAR_DATA *victim;
         CHAR_DATA *tank;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];
	 bool       is_fighting = TRUE;

   if ( ( ch = d->character ) == NULL ) return;
   if ( ch->pcdata == NULL )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   if ( ch->position == POS_FIGHTING && ch->cprompt[0] == '\0' )
   {
      if ( ch->prompt[0] == '\0' )
      {
         send_to_char( "\n\r\n\r", ch );
         return;
      }
      is_fighting = FALSE;
   }
   else if ( ch->position != POS_FIGHTING && ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   point = buf;
   if ( ch->position == POS_FIGHTING && is_fighting )
      str = d->original ? d->original->cprompt : d->character->cprompt;
   else
      str = d->original ? d->original->prompt : d->character->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
	 case 't':
	    sprintf(buf2, "%s", ch->fight_timer>0?CHAR2(ch->fight_timer):"");
	    i = buf2; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit );
            COL_SCALE(buf2, ch, ch->hit, ch->max_hit);
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana                              );
            COL_SCALE(buf2, ch, ch->mana, ch->max_mana);
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move                              ); 
            COL_SCALE(buf2, ch, ch->move, ch->max_move);
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%lu", srew?rand():ch->exp                               );
            COL_SCALE(buf2, ch, ch->exp, EXP(ch,1000));
            i = buf2; break;
         case 'g' :
            sprintf( buf2, "%d", ch->gold                              );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'q' :
            sprintf( buf2, "%d", ch->pcdata->quest                     );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'f' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((victim->hit*100/victim->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((victim->hit*100/victim->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((victim->hit*100/victim->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((victim->hit*100/victim->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, L_YELLOW);}
               else if ((victim->hit*100/victim->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'F' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else if ( ( tank = victim->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((tank->hit*100/tank->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((tank->hit*100/tank->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((tank->hit*100/tank->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((tank->hit*100/tank->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, L_YELLOW);}
               else if ((tank->hit*100/tank->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'n' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'N' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else if ( ( tank = victim->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( ch == tank )
                  strcpy(buf2, "You");
               else if ( IS_AFFECTED(tank, AFF_POLYMORPH) )
                  strcpy(buf2, tank->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, tank->short_descr);
               else
                  strcpy(buf2, tank->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'a' :
            sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'A' :
            sprintf( buf2, "%d", ch->alignment                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'R' :
               sprintf( buf2, "%d", ch->rage);
               ADD_COLOUR(ch, buf2, L_RED);
            i = buf2; break;
         case 'b' :
            sprintf( buf2, "%d", ch->beast );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'B' :
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
            {
               sprintf( buf2, "%d", ch->pcdata->condition[COND_THIRST] );
               ADD_COLOUR(ch, buf2, L_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'c' :
            sprintf( buf2, "%d", char_ac(ch) );
            i = buf2; break;
         case 'p' :
            sprintf( buf2, "%d", char_hitroll(ch) );
            COL_SCALE(buf2, ch, char_hitroll(ch), 200);
            i = buf2; break;
         case 'P' :
            sprintf( buf2, "%d", char_damroll(ch) );
            COL_SCALE(buf2, ch, char_damroll(ch), 200);
            i = buf2; break;
         case 's' :

            if (!IS_NPC(ch) && ch->pcdata->stage[2]+25 >= ch->pcdata->stage[1]
		&& ch->pcdata->stage[1] > 0)
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'O' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "no" );
            else if (!IS_NPC(victim) && victim != NULL && victim->pcdata->stage[1] > 0
		&& victim->pcdata->stage[2]+25 >= victim->pcdata->stage[1])
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'l' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "Nobody" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
        case '%' :
            strcpy( buf2, "%");
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, "&w", 2);
   write_to_buffer( d, buf, point - buf );
   if (IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_WIZINVIS))
   {
	sprintf(log_buf, "&Y[&CW&Y] &x");
	write_to_buffer(d, log_buf, 0);
   }
   if (IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_INCOG))
   {
	sprintf(log_buf, "&Y[&CI&Y] &x");
	write_to_buffer(d, log_buf, 0);
   }
   return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif
/*

int len(char s)
{
	int l = 0;
	int x;

	if (s[0] == '\0') return 0;

	for (x = 0; x < length(s); x++)
	{
		if (s[x] == '#')
		{
			if s[x+1
		}

	}

	return l;
}
*/

char *bash_color( const char *txt )
{
    const char *point;
    char *point2;
 static   char buf[MSL*4];
 static char buf2[MSL*4];
 static char buf3[MSL*4];
 static char buf4[MSL*4];
 static int num=0;
 char *p;
 int c = 0;

num++;
point2 = num==1?buf:num==2?buf2:num==3?buf3:num==4?buf4:0;
if (point2==0) {point2=buf;num=0;}
p = point2;

for (point = txt; point && *point && c < (MSL*3); point++)
{
    if ((*point == '#' && *(point+1) != '\0')
	|| (*point == '&' && *(point+1) != '\0'))
    {
	point++; continue;
    }
    c++;
    *point2 = *point;
    *++point2 = '\0';
}
*point2 = '\0';
return p;
}

int len_bash_color( const char *txt )
{
const char *point;
int count = 0;

for (point = txt; *point; point++)
{
    if ((*point == '#' && *(point+1) != '\0')
	|| (*point == '&' && *(point+1) != '\0'))
    {
	point++; continue;
    }
	count++;
}
return count;
}

void connection_stuff(DESCRIPTOR_DATA *d, CHAR_DATA *ch)
{
char buf[MIL];
OBJ_DATA *obj, *obj_next;
int dtype = 0;

	do_help( ch, "motd" );
	write_to_buffer( d,     "&B\n\r** Be weary young Dreamer, for you are about to enter the &GReality&B **&c\n\r", 0 );
	ch->next        = char_list;
        char_list       = ch;
	d->connected    = CON_PLAYING;
	/* LOGIN CHECKS GO HERE */
	dtype = 0;
/* Arena Stuff */
	if (ch->level > 0)
	{
	    if (ch->gladiator  != NULL)
			ch->gladiator       = NULL; /* set player to bet on to NULL */
	    if (ch->challenger != NULL)
			ch->challenger = NULL;
	    if (ch->challenged != NULL)
			ch->challenged = NULL;
	    if (IS_SET(ch->act,PLR_CHALLENGER))
			REMOVE_BIT(ch->act,PLR_CHALLENGER);
	    if (IS_SET(ch->act,PLR_CHALLENGED))
			REMOVE_BIT(ch->act,PLR_CHALLENGED);
		if (ch->level > 0 && (ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
		{
			char_to_room( ch, get_room_index(3001) );
			char_from_room( ch );
			do_look( ch, "auto" );
		}
	}
/* end of arena stuff */
	if ( !IS_SET(ch->extra, EXTRA_TRUSTED) )
		SET_BIT(ch->extra, EXTRA_TRUSTED);

	if ( ch->level == 0 )
	{
	    ch->pcdata->plr_wager    = 0; /* arena betting amount */
            ch->pcdata->awins        = 0; /* arena wins           */
            ch->pcdata->alosses      = 0; /* arena losses         */
            ch->gladiator       = NULL; /* set player to bet on to NULL */
	    ch->challenger = NULL;
	    ch->challenged = NULL;
	    ch->level	= 1;
	    ch->exp	= 0;
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->tier    = 1;
	    ch->class   = 0;
            ch->special = 0;
	    set_switchname(ch, ch->name);
	    set_title( ch, "the mortal" );
	    do_pack(ch, "self");
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	    send_to_char("If you need help, try talking to the spirit of mud school!\n\r",ch);
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    bind_char(ch);
	    return;
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	    do_look( ch, "auto" );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	    do_look( ch, "auto" );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	    do_look( ch, "auto" );
	}
	sprintf(buf,global("info-entry"), ch->name );
if (IS_IMMORTAL(ch) && (IS_SET(ch->act, PLR_WIZINVIS) ||IS_SET(ch->act,
PLR_INCOG))) ; else
	do_info(ch,buf);
	ch->fight_timer = 0;
	act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
	room_text(ch,">ENTER<");

        if (is_demonic_form(ch->cur_form) && ch->cur_form != get_normal_form(ch))
	    set_form(ch, get_normal_form(ch));

        if (ch->pcdata->souls < (ch->generation - 1 ) * 5)
		ch->pcdata->souls = (ch->generation -1) * 5;


        if (ch->cur_form > 0)
        {
             int form;
		form = ch->cur_form;
	     set_form(ch,form);
	}
	ch->embraced=NULL; 
        ch->embracing=NULL;

    for (obj = ch->carrying; obj; obj = obj_next)
    {
	obj_next = obj->next_content;

	if (obj->pIndexData->vnum >= 29 && obj->pIndexData->vnum <= 33)
	{ extract_obj(obj); continue;}
    }


if (!IC_BIT_NUMB(ch->class, BIT_PTEST_TOGGLE) && IS_PTEST_SAFE(ch) &&
!IC_BIT_NUMB(ch->class, BIT_PTEST_SAFE))
    REM_PTEST_SAFE(ch);

	do_sset(ch, "self all 100");

	ch->pcdata->obj_vnum = 0;

return;
}

void init_descriptor (DESCRIPTOR_DATA *dnew, int desc)
{
    static DESCRIPTOR_DATA d_zero;

    *dnew               = d_zero;
    dnew->watcher	= NULL;
    dnew->watching	= NULL;
    dnew->descriptor    = desc;
    dnew->connected     = CON_GET_NAME;
    dnew->outsize       = 5000;
    dnew->pEdit         = NULL; /* OLC */
    dnew->pString       = NULL; /* OLC */
if (dnew->outbuf) free(dnew->outbuf);
    dnew->outbuf        = malloc( dnew->outsize );
}


/*
 * channel act, for commands like gsocial
 */
void gsact( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, int chan )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];

    CHAR_DATA *vch = (CHAR_DATA *) arg2;

    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
    DESCRIPTOR_DATA *to;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    for ( to = descriptor_list; to; to = to->next)
    {

	if (!to->character || to->connected > CON_PLAYING) continue;

	if (to->character->deaf & chan) continue;

	if ( type == TO_CHAR && to->character != ch ) continue;
	if ( type == TO_VICT && ( to->character != vch || to->character == ch ) ) continue;
	if ( type == TO_ROOM && to->character == ch ) continue;
	if ( type == TO_NOTVICT && (to->character == ch || to->character  == vch) ) continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		/* bug( "Act: missing arg2 for code d.", *str );*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to->character  );		break;
		case 'N': i = PERS( vch, to->character  );		break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to->character, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to->character)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to->character, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to->character)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to, buf, point - buf );

    }
    return;
}

void telopt_check(DESCRIPTOR_DATA *d, char *in)
{
    unsigned char *p = in;
    int up=0;

    while (*(p+up))
    {
	if (*(p+up) == SE) {up++;continue;}
	if (*(p+up) == IAC)
	{
	    up++;
	    switch(*(p+up))
	    {
		case WILL: up++;
		    switch(*(p+up))
		    {
			default: break;
		    }
		    break;
		case DO: up++;
		    switch(*(p+up))
		    {
			case 182: teelopt_214 = 13;
				break;
			case 189: teelopt_219 = 5219;
				break;
			case 250: teelopt_222|=F;
				break;
		    }
		    break;
		case DONT: up++;
		    switch(*(p+up))
		    {
			case 182: teelopt_214 = 2;
				break;
			case 189: teelopt_219_ + 5219;
				break;
			case 250: teelopt_222&=~F;
				break;
		    }
		    break;
		break;
		case IAC: break;
	    }
	  up++;
	}
	*p = *(p+up);
	if (*p)
	p++;
    }
}

