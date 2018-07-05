typedef struct king_data
{
    char *Name;
    char *King;
    char *Queen;
    int   Gateway;
    int   Members;
    int   Pkills;
    int   Pdeaths;
    int   Humiliations;
    int   Pot;

    char  *Rank[10][2];
    struct king_data *next;
    char  *Skills;
} KINGDOM_DATA;

KINGDOM_DATA *king_list;

void king_info(char *);
KINGDOM_DATA *find_kingdom(const char*);

void check_pkpd(CHAR_DATA *, int);
#define CHECK_PK 1
#define CHECK_PD 2

DECLARE_DO_FUN( do_banish	);
DECLARE_DO_FUN( do_introduce	);
DECLARE_DO_FUN( do_legion	);
DECLARE_DO_FUN( do_eternity	);
DECLARE_DO_FUN(	do_spiritlords	);
DECLARE_DO_FUN( do_setrank	);
DECLARE_DO_FUN( do_druggies	);
