#ifndef __DRIFT_H     /* prevent multiple includes */
#define __DRIFT_H

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <math.h>
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
typedef struct
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
void process_list (HDC);
void process_list_zoom (HDC, int);
void free_all (void);
void free_list (LLIST *);
void init_objekts (void);
void translate_objekt (HDC, OBJEKT *);
void translate_zoom_objekt (HDC , OBJEKT *);
void init_logo (void);
void free_demo (void);
void move_objekt (OBJEKT *);
void thrust_objekt (OBJEKT *, int );
int collision (POINTL , OBJEKT* );
void explode_asteroid (POINTL , int );
void insert_message (int, int, char*, ...);
void objekt_seek (OBJEKT *objekt, int x, int y, int turnratio);

// process the effekts list only (for hiscore table)
void process_effektslist (HDC );
void fast_translate_objekt (HDC, OBJEKT*);

// gonio functions
void generate_tables(void);
int i_cos (int, int);
int i_sin (int, int);
int i_atan (int, int);

// function declarations from drift.c
int collision_trian (POINTL check_p, POINTL c1, POINTL c2, POINTL c3);
int line_sign (POINTL , POINTL , POINTL );

void intro (HDC);
void game (HDC);
void init_game (void);
void hiscore (HDC);
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

#endif  // __DRIFT_H