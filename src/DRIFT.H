#ifndef __DRIFT_H     /* prevent multiple includes */
#define __DRIFT_H

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/* Borland C compat for Open Watcom */
#ifndef random
#define random(n)   (rand() % (n))
#endif
#ifndef randomize
#define randomize() srand((unsigned)time(NULL))
#endif
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include "resource.h"
#include "llist.h"

#define RGB(a,b,c) (a << 16 | b << 8 | c)

#define  IDM_PLAY       101
#define  IDM_QUIT       102
#define  IDM_PAUZE      103

#define  IDVK_ESCAPE    104

#define  IDM_HIGHDET    201
#define  IDM_MEDIUMDET  202
#define  IDM_LOWDET     203
#define  IDM_SAVEONEXIT 204
#define  IDM_EXIT	      199
#define  IDM_DEBUG      666
#define  IDM_HELP	      901
#define  IDM_ABOUT	    999

#define IDD_HISCORE     300   // the hiscore string ID

#define IDM_LANG_EN     301
#define IDM_LANG_ES     302
#define IDM_LANG_NL     303
#define IDM_LANG_DE     304
#define IDM_LANG_FR     305
#define IDM_LANG_IT     306

/* Submenu item IDs — needed to locate submenu HWNDs at runtime */
#define IDM_SUBMENU_GAME      1000
#define IDM_SUBMENU_OPTIONS   1001
#define IDM_SUBMENU_HELP      1002
#define IDM_SUBMENU_DETAIL    1010
#define IDM_SUBMENU_LANGUAGE  1011

/* Language codes */
#define LANG_EN  0
#define LANG_ES  1
#define LANG_NL  2
#define LANG_DE  3
#define LANG_FR  4
#define LANG_IT  5

/* String table indices */
#define STR_GAME          0
#define STR_PLAY          1
#define STR_PAUZE         2
#define STR_QUITGAME      3
#define STR_EXIT          4
#define STR_OPTIONS       5
#define STR_DETAIL        6
#define STR_DETAIL_HIGH   7
#define STR_DETAIL_MEDIUM 8
#define STR_DETAIL_LOW    9
#define STR_LANGUAGE     10
#define STR_SAVEONEXIT   11
#define STR_HELP         12
#define STR_ABOUT        13
#define STR_SAT_MADNESS  14
#define STR_POLYGUNZZ    15
#define STR_ASTER_FACTORY 16
#define STR_BLACK_HOLE   17
#define STR_BIGSPACE     18
#define STR_GAME_OVER    19
#define STR_LEVEL_FMT    20
#define STR_LEVEL_CLR    21
#define STR_SCORE_FMT    22
#define STR_HISCORE_TITLE 23
#define STR_HISCORE_HDR  24
#define STR_DRIFT_TITLE  25
#define STR_DRIFT_BY     26
#define STR_KEYS         27
#define STR_KEY_UP       28
#define STR_KEY_LEFT     29
#define STR_KEY_RIGHT    30
#define STR_KEY_SPACE    31
#define NUM_STRINGS      32

extern int current_lang;
extern const char *lang_strings[6][NUM_STRINGS];
#define tr(id) ((char*)lang_strings[current_lang][(id)])

void set_language(HWND hMenu, int lang);

#define GONIO_PRECISION 1024L //precission of gonio tables

#define THRUSTPOWER 2  // magnitude of engine thrust
#define MAXTHRUST   28 // maximum speed of ship

#define MAXSHOT          8        // frequency of shots in frames

// drawtypes
#define DT_POLY     0
#define DT_LINE     1
#define DT_PIXEL    2
// drift custom message
#define DM_CHECKSCORE          (WM_USER+1)
// objekt types
#define OT_SHIP		      1
#define OT_BIGASTER	    10
#define OT_MEDIUMASTER  11
#define OT_SMALLASTER   12
#define OT_THRUST	      20
#define OT_SHOT		      30
// things that shoot at you
#define OT_SAT          40
#define OT_ANGRYSAT     41  // more dangerous sat
#define OT_MYSTSHIP     42  // mysteryship: creates new asteroids
#define OT_ENEMY1       43  // MK1 enemy fighter (1 shot)
#define OT_ENEMY2       44  // MK2 enemy fighter (2 shot , fast turn)
// power ups
#define OT_2SHOT        50
#define OT_LONGSHOT     51
#define OT_REARSHOT     52
#define OT_SHIELD       53
#define OT_FASTSHOT		54
// misc
#define OT_LOGO		      900
#define OT_LINE         901
#define OT_SHIPSHIELD   902
#define OT_DUMMY        999

#define GAME_PLAYING  1
#define GAME_HISCORE  2
#define GAME_INTRO    3

// objekt structure
typedef struct OBJEKT_s
				{
				int ID;                   // type of objekt
				POINTL current;            // the current coordinates
				POINTL old;                // the previous coordinates
        int angle;                // the angle of rotation
				int thrust_angle;         // angle of thrustvector
        float thrust_speed;       // length of thrustvector
        int draw_type;            // 0 = polyline, 1 = line, 2 = pixel
        int zoom;                 // zoomfactor (only for fancy stuff)
				int rot_speed;            // rotation speed in degrees (only for autorotating obj)
				LONG color; //COLORREF color;           // color
				int life;                 // number of frames that objekt "lives" 0=explicit kill
        POINTL *shape;             // the actual data (maximum 15 lines)
				int nbr_points;           // the number of POINTs
        int notnew;
        POINTL *real;              // the real shape after transformation
        } OBJEKT;

struct HISCORE
			 {
       char name[10];
       UINT score;
			 UINT level;
       };

// function declarations from objekt.c

// objekt manipulation functions
OBJEKT* init_objekt (int, int, int, int, float ,int ,int ,LONG , POINTL *, int);
void free_objekt (OBJEKT *);
void add_list (OBJEKT *, LLIST **);
void delete_list (OBJEKT *, LLIST **);
void process_list (HPS);
void process_list_zoom (HPS, int);
void free_all (void);
void free_list (LLIST *);
void init_objekts (void);
void translate_objekt (HPS, OBJEKT *);
void translate_zoom_objekt (HPS, OBJEKT *);
void init_logo (void);
void free_demo (void);
void move_objekt (OBJEKT *);
void thrust_objekt (OBJEKT *, int );
int collision (POINTL , OBJEKT* );
void explode_asteroid (POINTL , int );
void insert_message (int, int, char*, ...);
void objekt_seek (OBJEKT *objekt, int x, int y, int turnratio);

// process the effekts list only (for hiscore table)
void process_effektslist (HPS);
void fast_translate_objekt (HPS, OBJEKT*);

// gonio functions
void generate_tables(void);
int i_cos (int, int);
int i_sin (int, int);
int i_atan (int, int);

// function declarations from drift.c
int collision_trian (POINTL check_p, POINTL c1, POINTL c2, POINTL c3);
int line_sign (POINTL , POINTL , POINTL );

void intro (HPS);
void game (HPS);
void init_game (void);
void hiscore (HPS);
int to_deg (float rads);
float to_rad (int degrees);
int collision_trian (POINTL, POINTL, POINTL, POINTL);
int line_sign (POINTL, POINTL, POINTL);
int sign (long );
void insert_explosion (POINTL, int);
void insert_shipexplosion (POINTL , int );

void check_keys (void);
int edge (void);

// setting stuff
void load_settings(void);
void save_settings(void);

// hiscore stuff
void reset_hiscores(void);
void load_hiscores(void);
void save_hiscores(void);
int check_hiscores(MPARAM);

void debugdump (void );

// function declarations from common.c
void randomize_asteroids(POINTL *aster_data, int dim, int radius);

#endif  /* __DRIFT_H */
