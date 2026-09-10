#define INCL_BASE
#define INCL_WIN
#define INCL_GPI
#define INCL_DOSPROCESS
#define INCL_WININPUT

#include "Drift.h"

/* BLDLEVEL string — read by bldlevel.exe on OS/2 */
static const char bldlevel[] =
    "@#Dirk Vandenheuvel:1.03#@##1## 09 Sep 2026 12:00:00      "
    "ARCAOS:::0::::@@Drift for OS/2 - Space shoot em up game\r\n\x1a";

int current_lang = LANG_EN;

const char *lang_strings[6][NUM_STRINGS] = {
    { /* LANG_EN */
        "~Game", "~Play", "~Pauze", "~Quit game", "E~xit  Ctrl+X",
        "~Options", "~Detail", "~High", "~Medium", "~Low",
        "~Language", "~Save settings on exit", "~Help", "~About Drift...",
        "sattelite madness", "Go Polygunzz", "Asteroid factory",
        "black hole zone", "wow bigspace", "game over",
        "level %02d", "level %02d clear", "score %05d",
        "D R I F T  F O R  O S / 2  H I S C O R E S",
        "Rank    Name          Score  Level",
        "DRIFT WARPSPEED FOR OS/2 WARP",
        "By Dirk Vandenheuvel /c/ 1995",
        "Keys",
        "UP                     Thrust",
        "LEFT                Turn left",
        "RIGHT              Turn right",
        "SPACE            Fire missile"
    },
    { /* LANG_ES */
        "~Juego", "~Jugar", "~Pausar", "~Salir del juego", "E~xit  Ctrl+X",
        "~Opciones", "~Detalle", "~Alto", "~Medio", "~Bajo",
        "~Idioma", "~Guardar ajustes al salir", "~Ayuda", "~Acerca de Drift...",
        "locura satelital", "Vamos Polygunzz", "Fabrica de asteroides",
        "zona de agujero negro", "wow gran espacio", "juego terminado",
        "nivel %02d", "nivel %02d despejado", "puntaje %05d",
        "D R I F T  P A R A  O S / 2  P U N T U A C I O N E S",
        "Pos.    Nombre        Puntaje  Nivel",
        "DRIFT WARPSPEED PARA OS/2 WARP",
        "Por Dirk Vandenheuvel /c/ 1995",
        "Teclas",
        "ARRIBA               Empuje",
        "IZQUIERDA         Girar izq.",
        "DERECHA            Girar der.",
        "ESPACIO              Disparar"
    },
    { /* LANG_NL */
        "~Spel", "~Spelen", "~Pauze", "Spel ~verlaten", "~Afsluiten  Ctrl+X",
        "~Opties", "~Detail", "~Hoog", "~Middel", "~Laag",
        "~Taal", "~Instellingen opslaan bij afsluiten", "~Help", "~Over Drift...",
        "satelliet waanzin", "Ga Polygunzz", "Asteroide fabriek",
        "zwart gat zone", "wow grote ruimte", "spel voorbij",
        "niveau %02d", "niveau %02d vrij", "score %05d",
        "D R I F T  V O O R  O S / 2  H O G E  S C O R E S",
        "Rang    Naam          Score  Niveau",
        "DRIFT WARPSPEED VOOR OS/2 WARP",
        "Door Dirk Vandenheuvel /c/ 1995",
        "Toetsen",
        "OMHOOG              Stuwkracht",
        "LINKS              Links draaien",
        "RECHTS           Rechts draaien",
        "SPATIE               Vuur raket"
    },
    { /* LANG_DE */
        "~Spiel", "~Spielen", "~Pause", "Spiel ~beenden", "~Beenden  Ctrl+X",
        "~Optionen", "~Detail", "~Hoch", "~Mittel", "~Niedrig",
        "~Sprache", "~Einstellungen beim Beenden speichern", "~Hilfe", "~Ueber Drift...",
        "Satelliten-Wahnsinn", "Los Polygunzz", "Asteroiden-Fabrik",
        "Schwarzes-Loch-Zone", "wow grosser Raum", "Spiel vorbei",
        "Ebene %02d", "Ebene %02d geschafft", "Punkte %05d",
        "D R I F T  F U E R  O S / 2  H I G H S C O R E S",
        "Rang    Name          Punkte  Ebene",
        "DRIFT WARPSPEED FUER OS/2 WARP",
        "Von Dirk Vandenheuvel /c/ 1995",
        "Tasten",
        "HOCH                      Schub",
        "LINKS               Links drehen",
        "RECHTS            Rechts drehen",
        "LEERTASTE               Feuer"
    },
    { /* LANG_FR */
        "~Jeu", "~Jouer", "~Pause", "~Quitter le jeu", "~Exit  Ctrl+X",
        "~Options", "~Detail", "~Haut", "~Moyen", "~Bas",
        "~Langue", "~Sauvegarder les reglages a la sortie", "~Aide", "~A propos de Drift...",
        "folie des satellites", "Allez les Polygunzz", "Usine a asteroides",
        "zone de trou noir", "wow grand espace", "partie terminee",
        "niveau %02d", "niveau %02d efface", "score %05d",
        "D R I F T  P O U R  O S / 2  H A U T S  S C O R E S",
        "Rang    Nom           Score  Niveau",
        "DRIFT WARPSPEED POUR OS/2 WARP",
        "Par Dirk Vandenheuvel /c/ 1995",
        "Touches",
        "HAUT                    Poussee",
        "GAUCHE            Tourner gauche",
        "DROITE             Tourner droite",
        "ESPACE              Tirer missile"
    },
    { /* LANG_IT */
        "~Gioco", "~Gioca", "~Pausa", "~Esci dal gioco", "Es~ci  Ctrl+X",
        "~Opzioni", "~Dettaglio", "~Alta", "~Media", "~Bassa",
        "~Lingua", "~Salva impostazioni all'uscita", "~Aiuto", "~Informazioni su Drift...",
        "follia satellitare", "Forza Polygunzz", "Fabbrica di asteroidi",
        "zona buco nero", "wow grande spazio", "partita finita",
        "livello %02d", "livello %02d superato", "punteggio %05d",
        "D R I F T  P E R  O S / 2  P U N T E G G I  M A S S I M I",
        "Pos.    Nome          Punti  Livello",
        "DRIFT WARPSPEED PER OS/2 WARP",
        "Di Dirk Vandenheuvel /c/ 1995",
        "Tasti",
        "SU                          Spinta",
        "SINISTRA          Svolta sinistra",
        "DESTRA               Svolta destra",
        "SPAZIO              Lancia missile"
    }
};

static HWND get_submenu(HWND hMnu, USHORT id)
{
    MENUITEM mi;
    memset(&mi, 0, sizeof(mi));
    if ((BOOL)WinSendMsg(hMnu, MM_QUERYITEM, MPFROM2SHORT(id, FALSE), MPFROMP(&mi)))
        return mi.hwndSubMenu;
    return NULLHANDLE;
}

static void menu_set_text(HWND hMnu, USHORT id, const char *text)
{
    WinSendMsg(hMnu, MM_SETITEMTEXT, MPFROMSHORT(id), MPFROMP((PSZ)text));
}

void set_language(HWND hMenu, int lang)
{
    HWND hGame, hOptions, hDetail, hHelp;

    current_lang = lang;

    hGame    = get_submenu(hMenu, IDM_SUBMENU_GAME);
    hOptions = get_submenu(hMenu, IDM_SUBMENU_OPTIONS);
    hHelp    = get_submenu(hMenu, IDM_SUBMENU_HELP);

    menu_set_text(hMenu, IDM_SUBMENU_GAME,    tr(STR_GAME));
    menu_set_text(hMenu, IDM_SUBMENU_OPTIONS, tr(STR_OPTIONS));
    menu_set_text(hMenu, IDM_SUBMENU_HELP,    tr(STR_HELP));

    if (hGame) {
        menu_set_text(hGame, IDM_PLAY,  tr(STR_PLAY));
        menu_set_text(hGame, IDM_PAUZE, tr(STR_PAUZE));
        menu_set_text(hGame, IDM_QUIT,  tr(STR_QUITGAME));
        menu_set_text(hGame, IDM_EXIT,  tr(STR_EXIT));
    }
    if (hOptions) {
        hDetail = get_submenu(hOptions, IDM_SUBMENU_DETAIL);
        menu_set_text(hOptions, IDM_SUBMENU_DETAIL,   tr(STR_DETAIL));
        menu_set_text(hOptions, IDM_SUBMENU_LANGUAGE, tr(STR_LANGUAGE));
        menu_set_text(hOptions, IDM_SAVEONEXIT,       tr(STR_SAVEONEXIT));
        if (hDetail) {
            menu_set_text(hDetail, IDM_HIGHDET,   tr(STR_DETAIL_HIGH));
            menu_set_text(hDetail, IDM_MEDIUMDET, tr(STR_DETAIL_MEDIUM));
            menu_set_text(hDetail, IDM_LOWDET,    tr(STR_DETAIL_LOW));
        }
    }
    if (hHelp)
        menu_set_text(hHelp, IDM_ABOUT, tr(STR_ABOUT));

    WinCheckMenuItem(hMenu, IDM_LANG_EN, lang == LANG_EN);
    WinCheckMenuItem(hMenu, IDM_LANG_ES, lang == LANG_ES);
    WinCheckMenuItem(hMenu, IDM_LANG_NL, lang == LANG_NL);
    WinCheckMenuItem(hMenu, IDM_LANG_DE, lang == LANG_DE);
    WinCheckMenuItem(hMenu, IDM_LANG_FR, lang == LANG_FR);
    WinCheckMenuItem(hMenu, IDM_LANG_IT, lang == LANG_IT);
}

#define ID_TIMER	1
ULONG idTimer;

CHAR szClientClass [] = "DriftOS2";

// Callback function standard definition
MRESULT EXPENTRY _export ClientWndProc(HWND,ULONG,MPARAM,MPARAM);
MRESULT EXPENTRY _export AboutDiaProc(HWND,ULONG,MPARAM,MPARAM);
MRESULT EXPENTRY _export EnterHiScore(HWND,ULONG,MPARAM,MPARAM);

// games vars
int input_enabled;
UINT game_score;
int game_status = GAME_INTRO;            // status can be; playing, demo or intro
int game_duration = 200;                 // duration of intro and demo in frames
int game_level;
int game_shooting = 0;                   // frequency of shots
int nbr_asteroids = 0;
int game_bigex = 36;
int game_mediumex = 18;
int game_smallex = 9;
int maxthrustpixels = 9;
int game_maxshots = MAXSHOT;
int game_over;
int game_pauze=FALSE;
int game_blackhole;       // is it a black hole level?
int game_bigspace;        // is it a bigspace level? (should combine gametypes)
int game_shots;           // number of shots
int game_turning;         // turning speed of ship
int game_shotlife;        // life of ships missiles
int game_shieldduration;  // duration of shield
int game_rearshot;
int game_life;

struct HISCORE HiTable[10];

int end_count;

// all the necessary llists
LLIST *start      = NULL;
LLIST *effekts    = NULL;
LLIST *enemies    = NULL;
LLIST *shots      = NULL;
LLIST *extra      = NULL;

char glb_name[10];     // hiscore name
//HPEN hClearPen;

HWND hwndClient;

//FILE *debug;

char szProgName[]="DriftOS2";

int detaillevel = IDM_HIGHDET;
int saveonexit = TRUE;

extern OBJEKT* ship;

extern POINTL asteroiddata[];
extern POINTL mediumasteroid1[];
extern POINTL mediumasteroid2[];
extern POINTL smallasteroid[];
extern POINTL pixeldata[];
extern POINTL shotdata[];
extern POINTL satdata[];
extern POINTL linedata[];
extern POINTL shielddata[];
extern POINTL mysterydata[];
extern POINTL enemy1data[];
extern POINTL enemy2data[];

// demo objekts
extern OBJEKT *D;
extern OBJEKT *R;
extern OBJEKT *I;
extern OBJEKT *F;
extern OBJEKT *T;

HAB	hab;
HMQ	hmq;
HWND	hwndMain=0L, hwndClient;
QMSG	qmsg;

int main(void)
{

static ULONG flFrameFlags = 	FCF_TITLEBAR 			|
															FCF_SYSMENU 			|
															FCF_SIZEBORDER		|
                              FCF_MINMAX 				|
                              FCF_MENU 					|
															FCF_SHELLPOSITION |
                              FCF_ACCELTABLE    |
                              FCF_TASKLIST;

hab = WinInitialize(0L);
hmq = WinCreateMsgQueue (hab,0L);

WinRegisterClass (hab, (PSZ) szProgName, (PFNWP) ClientWndProc, CS_SIZEREDRAW|CS_CLIPCHILDREN,0L);

hwndMain = WinCreateStdWindow(HWND_DESKTOP, 0L, &flFrameFlags,
																szClientClass, "Drift Warpspeed", 0L,
                                0L,ID_RESOURCE,
																(PHWND) &hwndClient);
{
    LONG cxScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    LONG cyScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
    LONG winW = (cxScreen >= 1024L) ? 1024L : cxScreen;
    LONG winH = (cyScreen >= 768L)  ? 768L  : cyScreen;
    LONG x    = (cxScreen - winW) / 2;
    LONG y    = (cyScreen - winH) / 2;
    WinSetWindowPos(hwndMain, HWND_TOP, x, y, winW, winH,
                    SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW);
}
// generate gonio tables
generate_tables();

idTimer = WinStartTimer(hab,hwndClient,ID_TIMER,31);

while (WinGetMsg(hab,&qmsg,NULLHANDLE,0,0))
	{
	WinDispatchMsg(hab,&qmsg);
	}

save_hiscores();
if (saveonexit)
  save_settings();
free_all();
free_demo();
WinStopTimer(hab,hwndClient,idTimer);

//WinDestroyWindow(hwndMain);
WinDestroyMsgQueue(hmq);
WinTerminate(hab);
return (0);
}

// exported extern aboutdialogbox callback routine (doet dialogbox stuff)
MRESULT EXPENTRY AboutDiaProc(HWND hdlg,ULONG messg,MPARAM mp1,MPARAM mp2)
{
switch (messg)
	{
	case WM_COMMAND: switch (COMMANDMSG(&messg)->cmd)
									 {
									 case DID_OK: WinDismissDlg (hdlg, TRUE);
																return (0);
									 }
									 break;
	}
return(WinDefDlgProc(hdlg,messg,mp1,mp2));
}

MRESULT EXPENTRY EnterHiScore(HWND hdlg,ULONG messg,MPARAM mp1,MPARAM mp2)
{
switch (messg)
	{
	case WM_COMMAND: switch (COMMANDMSG(&messg)->cmd)
										{
                    case WM_INITDLG:
                                  WinSendDlgItemMsg(hdlg,IDD_HISCORE,EM_SETTEXTLIMIT,(MPARAM)9,NULL);
                                  break;
										case DID_OK:  WinQueryDlgItemText(hdlg,IDD_HISCORE,sizeof(glb_name),glb_name);
                                  WinDismissDlg (hdlg, TRUE);
																	return (0);
										}
										break;
			 }
return(WinDefDlgProc(hdlg,messg,mp1,mp2));}

// exported extern windows callback routine (doet alle belangrijke stuff)
MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG messg,MPARAM mp1,MPARAM mp2)
{
HPS         hps;
static HWND hMenu;
RECTL       rc;
static INT xView, yView;
int count,zoom,tel;
float tmpangle;
BOOL stat;
SIZEL sizel;
POINTL ptl;
switch (messg)
			 {
			 case DM_CHECKSCORE:
					count = check_hiscores(mp1);
					if (count != 10)
						{
						WinDlgBox(HWND_DESKTOP,hwnd,EnterHiScore,NULLHANDLE,DRIFT_HISCORE,NULL);
						if (glb_name[0] == 0)
							strcpy(HiTable[count].name,"Cthulhu");
						else
							strcpy(HiTable[count].name,glb_name);
						}
						break;
		 case WM_SIZE:
						xView = SHORT1FROMMP(mp2);
						yView = SHORT2FROMMP(mp2);
						break;
		 case WM_CREATE:
						init_logo();
						load_hiscores();
						// initialize the settings
            //hMenu=WinQueryWindow(hwnd,QW_PARENT);
						hMenu=WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_MENU);
						// uncheck detaillevel
						stat = WinCheckMenuItem(hMenu,detaillevel,FALSE);
            // uncheck save on exit
            stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,FALSE);
						load_settings();
						// check to correct menu item detaillevel
						stat = WinCheckMenuItem(hMenu,detaillevel,TRUE);
            if (saveonexit)
            	stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,TRUE);
						switch (detaillevel)
							{
							case IDM_HIGHDET:
											game_bigex = 36;
											game_mediumex = 18;
											game_smallex = 9;
											maxthrustpixels = 9;
											break;
							case IDM_MEDIUMDET:
											game_bigex = 18;
											game_mediumex = 18;
											game_smallex = 9;
											maxthrustpixels = 5;
											break;
							case IDM_LOWDET:
											game_bigex = 9;
											game_mediumex = 9;
											game_smallex = 9;
											maxthrustpixels = 3;
											break;
							}
						if (saveonexit == TRUE)
								stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,TRUE);
						else
								stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,FALSE);
						set_language(hMenu, current_lang);
						break;
		 case WM_TIMER:
						hps = WinGetPS(hwnd) ;
            sizel.cx = 2000 ;
            sizel.cy = 2000 ;
            GpiSetPS (hps, &sizel, PU_PELS) ;

						GpiQueryPageViewport(hps,&rc);
						rc.xLeft   = 0;//xView / 2 ;
            rc.xRight  = xView ;
            rc.yBottom = 0;//yView / 2 ;
            rc.yTop    = yView;

            GpiSetPageViewport (hps, &rc) ;

						 switch (game_status)
							{
							case GAME_INTRO :
												stat = WinEnableMenuItem(hMenu,IDM_PLAY,TRUE);
												stat = WinEnableMenuItem(hMenu,IDM_QUIT,FALSE);
												stat = WinEnableMenuItem(hMenu,IDM_PAUZE,FALSE);
												zoom = (game_duration - 100)/3;
												if (zoom < 5)
													zoom = 5;
												D->angle += 10;
												D->zoom = zoom;
												R->angle += 10;
												R->zoom = zoom;
												I->angle += 10;
												I->zoom = zoom;
												F->angle += 10;
												F->zoom = zoom;
												T->angle += 10;
												T->zoom = zoom;
												game_duration --;
												if (game_duration == 0)
													{
                          game_status = GAME_HISCORE;
                          game_duration = 200;                          if (random(2) == 0)
                            {
                            // insert the hiscore table
													  insert_message(1800,0,"%s",tr(STR_HISCORE_TITLE));
													  insert_message(1600,0,"%s",tr(STR_HISCORE_HDR));
													  for (tel = 0; tel < 10; tel++)
														  insert_message(1460-(tel*100),0,"%2d     %-12s %5d    %2d",tel+1,HiTable[tel].name,HiTable[tel].score,HiTable[tel].level);
                            }
                          else
                            {
                            // insert help message
                            insert_message(1800,0,"%s",tr(STR_DRIFT_TITLE));
                            insert_message(1600,0,"%s",tr(STR_DRIFT_BY));
                            insert_message(1400,0,"%s",tr(STR_KEYS));
                            insert_message(1200,0,"%s",tr(STR_KEY_UP));
                            insert_message(1100,0,"%s",tr(STR_KEY_LEFT));
                            insert_message(1000,0,"%s",tr(STR_KEY_RIGHT));
                            insert_message(900,0,"%s",tr(STR_KEY_SPACE));
                            }
                          WinInvalidateRect(hwnd,NULL,TRUE);
													}
												else
													intro(hps);
												break;
							case GAME_HISCORE:
												game_duration --;
												if (game_duration == 0)
													{
													// free the effekts list
													free_all();
													game_status = GAME_INTRO;
													game_duration = 200;
													WinInvalidateRect(hwnd,NULL,TRUE);
													}
												break;
							case GAME_PLAYING: // play
												check_keys();
												if (game_shooting)
													game_shooting--;
												game(hps);
												break;
							}
               WinReleasePS(hps);
               break;
		 case WM_SYSCOMMAND:
							/*switch(mp1)
								{
								case SC_MINIMIZE:
											if (!game_pauze)
												SendMessage(hwnd,WM_COMMAND,IDM_PAUZE,0);
											break;
											}
								return(DefWindowProc(hwnd,messg,mp1,mp2));*/
								break;
		 case WM_COMMAND:
								switch (COMMANDMSG(&messg)->cmd)
									{
									case IDM_SAVEONEXIT:
															if (saveonexit == TRUE) // already checked
																{
																stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,FALSE);
																saveonexit = FALSE;
																save_settings(); // save the "do not save on exit" setting
																}
															else
																{
																stat = WinCheckMenuItem(hMenu,IDM_SAVEONEXIT,TRUE);
																saveonexit = TRUE;
																}
															break;
									case IDM_HIGHDET : // set to high
															game_bigex = 36;
															game_mediumex = 18;
															game_smallex = 9;
															maxthrustpixels = 9;
															stat = WinCheckMenuItem(hMenu,detaillevel,FALSE);
															detaillevel=SHORT1FROMMP(mp1);
															stat = WinCheckMenuItem(hMenu,detaillevel,TRUE);
															break;
									case IDM_MEDIUMDET:
															game_bigex = 18;
															game_mediumex = 18;
															game_smallex = 9;
															maxthrustpixels = 5;
															stat = WinCheckMenuItem(hMenu,detaillevel,FALSE);
															detaillevel=SHORT1FROMMP(mp1);
															stat = WinCheckMenuItem(hMenu,detaillevel,TRUE);
															break;
									case IDM_LOWDET:
															game_bigex = 9;
															game_mediumex = 9;
															game_smallex = 9;
															maxthrustpixels = 3;
															stat = WinCheckMenuItem(hMenu,detaillevel,FALSE);
															detaillevel=SHORT1FROMMP(mp1);
															stat = WinCheckMenuItem(hMenu,detaillevel,TRUE);
															break;
									case IDM_ABOUT:
															WinDlgBox(HWND_DESKTOP,hwnd,AboutDiaProc,NULLHANDLE,IDD_ABOUT,NULL);
															break;
									case IDM_HELP :
															//WinHelp (hwnd,"drift.hlp",HELP_INDEX,0);
															break;
									case IDVK_ESCAPE: // escape key pressed
															//SendMessage (hwnd, WM_SYSCOMMAND,SC_MINIMIZE,0);
															break;
									case IDM_PAUZE:
															if (game_status != GAME_PLAYING)
																break;
															if (game_pauze != TRUE)
																{
                                WinStopTimer(hab,hwndClient,idTimer);
                                stat = WinCheckMenuItem(hMenu,IDM_PAUZE,TRUE);
																game_pauze = TRUE;
																}
															else
																{
																game_pauze = FALSE;
																// gets a timer
																idTimer = WinStartTimer(hab,hwndClient,ID_TIMER,30);
																stat = WinCheckMenuItem(hMenu,IDM_PAUZE,FALSE);
																WinInvalidateRect(hwnd, NULL, TRUE);
																}
															break;
									case IDM_QUIT:
															stat = WinEnableMenuItem(hMenu,IDM_PLAY,TRUE);
															stat = WinEnableMenuItem(hMenu,IDM_QUIT,FALSE);
															stat = WinEnableMenuItem(hMenu,IDM_PAUZE,FALSE);
															free_all();
															game_status = GAME_INTRO;
															if (game_pauze)
																{
																game_pauze = FALSE;
																//while (!SetTimer(hwnd,ID_TIMER,10,NULL))
																//if (IDCANCEL == MessageBox (hwnd, "Cannot get timer handle!", szProgName,MB_ICONEXCLAMATION | MB_RETRYCANCEL))
																//exit(0); // add a better shutdown here
																//CheckMenuItem(hmenu,IDM_PAUZE,MF_UNCHECKED);
																}
															game_duration = 200;
															WinInvalidateRect(hwnd, NULL, TRUE);
															break;
									case IDM_PLAY:
															free_all();
															stat = WinEnableMenuItem(hMenu,IDM_PLAY,FALSE);
															stat = WinEnableMenuItem(hMenu,IDM_QUIT,TRUE);
															stat = WinEnableMenuItem(hMenu,IDM_PAUZE,TRUE);
															init_objekts();
															game_level = 1;
															// init powerups to default
															game_pauze = FALSE;
															game_maxshots = MAXSHOT;
															game_shots = 1;
															game_turning = 10;
															game_shotlife = 20;
															game_rearshot = FALSE;
															game_shieldduration = 0;
															init_game();
															game_status = GAME_PLAYING;
															game_score = 0;
															game_duration = 25;
                              game_life = 3;
															WinInvalidateRect(hwnd, NULL, TRUE);
															break;
									case IDM_EXIT:
																WinPostMsg(hwnd, WM_QUIT, 0, 0);
																break;
									case IDM_LANG_EN: set_language(hMenu, LANG_EN); break;
									case IDM_LANG_ES: set_language(hMenu, LANG_ES); break;
									case IDM_LANG_NL: set_language(hMenu, LANG_NL); break;
									case IDM_LANG_DE: set_language(hMenu, LANG_DE); break;
									case IDM_LANG_FR: set_language(hMenu, LANG_FR); break;
									case IDM_LANG_IT: set_language(hMenu, LANG_IT); break;
									default:    break;
									}
									break;
			 case WM_PAINT:
									hps = WinBeginPaint(hwnd,NULLHANDLE,&rc);
									WinQueryWindowRect(hps,&rc);
            			WinFillRect(hps,&rc,CLR_BLACK);
                  sizel.cx = 2000 ;
                  sizel.cy = 2000 ;
            			GpiSetPS (hps, &sizel, PU_PELS) ;

									GpiQueryPageViewport(hps,&rc);
									rc.xLeft   = 0;//xView / 2 ;
            			rc.xRight  = xView ;
            			rc.yBottom = 0;//yView / 2 ;
            			rc.yTop    = yView;

            			GpiSetPageViewport (hps, &rc) ;
                  if (game_status == GAME_HISCORE)
                    hiscore(hps);
                  if (game_status == GAME_PLAYING && game_pauze)
										game(hps);
									WinEndPaint(hps);
									break;
        default:   return(WinDefWindowProc(hwnd,messg,mp1,mp2));
			 }
return(MRESULT)0L;
}

void init_game (void)
{
int count,tmprnd,special_rand;
game_over = FALSE;
game_blackhole = FALSE;
game_bigspace = FALSE;
end_count = 150;
nbr_asteroids = 0;
randomize();
input_enabled = TRUE;
randomize();
special_rand = random(20);
add_list(ship,&start);
if (game_shieldduration > 3)
	 add_list(init_objekt(OT_SHIPSHIELD,ship->current.x,ship->current.y,ship->angle,0,DT_POLY,game_shieldduration, CLR_WHITE,shielddata,4),&effekts);
// insert sats
if (special_rand == 0)  // sattelite madness
		{
		tmprnd = 10;
		insert_message(1600,0,"%s",tr(STR_SAT_MADNESS));
	 }
else
	 {
		tmprnd = game_level-5+random(7);  // number of sats
		if (tmprnd > 4)
			tmprnd = 4;
		}
if (tmprnd > 0)
	 for (count = 0; count < tmprnd; count++)
		{
		add_list(init_objekt(OT_SAT,random(2000),random(1200)-600,random(360),3,DT_POLY,0,CLR_CYAN,satdata,8),&start);
		nbr_asteroids++;
		}
if (special_rand == 2)
	 {
	 insert_message(1600,0,"%s",tr(STR_POLYGUNZZ));
	 for (count = 0; count < 4; count++)
		 add_list(init_objekt(OT_ENEMY1,edge(),random(2000),0,3,DT_POLY,0,CLR_BLUE,enemy1data,5),&start);
	 for (count = 0; count < 4; count++)
			 add_list(init_objekt(OT_ENEMY2,random(2000),edge(),0,4,DT_POLY,0,CLR_CYAN,enemy2data,7),&start);
	 nbr_asteroids+=8;
	 }
if (special_rand == 3)
	 {
	 insert_message(1600,0,"%s",tr(STR_ASTER_FACTORY));
	 for (count=0; count < 4; count++)
			add_list(init_objekt(OT_MYSTSHIP,edge(),random(2000),random(360),random(8)+2,DT_POLY,0,CLR_BROWN,mysterydata,8),&start);
	 nbr_asteroids+=4;
	 }
if (special_rand > 4 || special_rand == 1 || special_rand == 4) // no special level except black hole zone
	 {
	// inserts level+1 number of asteroids in llist
	for (count = 0; count < game_level+1; count++)
		{
		randomize_asteroids (asteroiddata , 10, 100);
		add_list(init_objekt(OT_BIGASTER,random(1200)-600,random(1200)-600,random(360),random(8)+2,DT_POLY,0,CLR_GREEN,asteroiddata,10),&start);
		nbr_asteroids++;
		}
	 }
if (special_rand == 1)
	 {
	 game_blackhole = TRUE;
	 insert_message(1600,0,"%s",tr(STR_BLACK_HOLE));
	 }
if (special_rand == 4)
  {
  game_bigspace = TRUE;
  insert_message(1600,0,"%s",tr(STR_BIGSPACE));
  }
// insert start message
insert_message(800,0,tr(STR_LEVEL_FMT),game_level);
}

void game (HPS hps)
{
int zoom, count;
POINTL at;
if (game_duration != 0)           // zooming intro of the game
	 {
	 zoom = game_duration / 2;
	 if (zoom == 0)
			zoom = 1;
	 process_list_zoom(hps,zoom);
	 game_duration--;
	 }
else
		{
	 process_list(hps);
		if (random(2500) == 0 && !game_over && nbr_asteroids>5)  // yep the mystery ship arrives
			 {
				add_list(init_objekt(OT_MYSTSHIP,edge(),edge(),random(360),random(8)+4,DT_POLY,0,CLR_BROWN,mysterydata,8),&start);
				nbr_asteroids++;
			 }
		if (random(1000) < game_level && !game_over && nbr_asteroids > 5) // enemy1 has arrived
			 {
			 add_list(init_objekt(OT_ENEMY1,random(2000),edge(),0,4,DT_POLY,0,CLR_BLUE,enemy1data,5),&start);
			 nbr_asteroids++;
			 }
		if (random(1500) == 0 && !game_over && nbr_asteroids > 5) // enemy2 has arrived
			 {
			 add_list(init_objekt(OT_ENEMY2,edge(),random(2000),0,4,DT_POLY,0,CLR_CYAN,enemy2data,7),&start);
			 nbr_asteroids++;
		 }
		if (game_shieldduration > 0)
			 game_shieldduration--;
		if (game_shieldduration < 0)
			 game_shieldduration = 0;
		if (game_over)
			 {
			 input_enabled = FALSE;
			 // game over... the end
			 if (end_count == 150)
					{
					insert_message(1200,0,"%s",tr(STR_GAME_OVER));
					insert_message(800,0,tr(STR_SCORE_FMT),game_score);
					WinPostMsg(hwndClient,DM_CHECKSCORE,(MPARAM)game_score,(MPARAM)0); // check for hiscore
					}
		 if (end_count == 0)
			 {
					//terminate all
					free_all();
					game_status = GAME_INTRO;
					game_duration = 200;
					WinInvalidateRect(hwndClient, NULL, TRUE);
					return;
					}
			 end_count--;
			 }
		if (nbr_asteroids == 0) // end of game
			 {
			 // do some end stuff
			 if (input_enabled)
			 {
					for (count = 0; count < 10; count++)
						 {
						 at.x = random(2000);
						 at.y = random(2000);
						 insert_explosion(at,10);
						 }
					insert_message(1200,0,tr(STR_LEVEL_CLR),game_level);
					insert_message(800,0,tr(STR_SCORE_FMT),game_score);
					}
			 input_enabled = FALSE;
			 if ((llist_items == 1) || (llist_items == 2 && game_shieldduration != 0))       // real end of game
					{
					delete_list(ship,&start);
					if (llist_items != 0)
					free_all();       // frees the shield
					ship->current.x = 1000;
					ship->current.y = 1000;
					ship->thrust_speed = 0;
					game_level++;
					init_game();
					game_duration = 25;
					WinInvalidateRect(hwndClient, NULL, TRUE);
					}
			 }
		}
}

void hiscore (HPS hps)
{
// draw hiscore
process_effektslist(hps);
}

void intro (HPS hps)
{
D->current.y = 1000;
R->current.y = 1000;
I->current.y = 1000;
F->current.y = 1000;
T->current.y = 1000;
translate_zoom_objekt(hps,D);
translate_zoom_objekt(hps,R);
translate_zoom_objekt(hps,I);
translate_zoom_objekt(hps,F);
translate_zoom_objekt(hps,T);
}

int collision_trian (POINTL check_p, POINTL c1, POINTL c2, POINTL c3)
{
if (line_sign (c1, c2, c3) != line_sign(c1, c2, check_p))
	return (FALSE);
if (line_sign (c2, c3, c1) != line_sign(c2, c3, check_p))
	return(FALSE);
if (line_sign (c1, c3, c2) != line_sign(c1, c3, check_p))
	return(FALSE);
return (TRUE);
}

int line_sign (POINTL p1, POINTL p2, POINTL check)
{
long tmp1,tmp2,tmp3,tmp4,result;
tmp1 = (check.y - p1.y);
tmp2 = (p2.x-p1.x);
tmp3 = (p2.y -p1.y);
tmp4 = (check.x-p1.x);
result = (tmp1*tmp2)-(tmp3*tmp4);
if (result >= 0)
	 return(1);
else
		return(-1);
}

void check_keys (void)
{
LONG state;
LONG key;
int count;
int tmpangle;
if (game_status != GAME_PLAYING || input_enabled == FALSE)
	 return;
// this routine checks the keys async.
key = 0x63; //VK_LEFT;
state = WinGetPhysKeyState (HWND_DESKTOP,key);
if (state & 0x8000)  // key is down
	  ship->angle += game_turning;
key = 0x64; // VK_RIGHT
state = WinGetPhysKeyState (HWND_DESKTOP,key);
if (state & 0x8000)
	 ship->angle -= game_turning;
key = 0x61; // VK_UP
state = WinGetPhysKeyState (HWND_DESKTOP,key); // increase thrust
if (state & 0x8000)
	{
	 thrust_objekt(ship,THRUSTPOWER);
	 // now insert some thrust pixels in the OBJEKT llist
	 for (count = 0; count < maxthrustpixels; count++)
			 {
			 tmpangle = (ship->angle)+180+random(40)-20;
			 add_list(init_objekt(OT_THRUST,ship->current.x,ship->current.y, tmpangle, random(10)+MAXTHRUST, DT_PIXEL, random(4)+3, CLR_WHITE, pixeldata, 1),&effekts);
		 }
	}
key = 0x39; // VK_SPACE
state = WinGetPhysKeyState (HWND_DESKTOP,key); //shoot
if (state & 0x8000)
	{
	if (game_shooting == 0)  // ok you can shoot
		{
		if (game_shots == 1)
			{
				 add_list(init_objekt(OT_SHOT,ship->real[0].x,ship->real[0].y, ship -> angle, MAXTHRUST+12, DT_LINE, game_shotlife,CLR_PINK, shotdata, 2),&start);
				 thrust_objekt(ship,-THRUSTPOWER);
				 }
			else
          {
          add_list(init_objekt(OT_SHOT,ship->real[0].x,ship->real[0].y, ship -> angle-3, MAXTHRUST+12, DT_LINE, game_shotlife,CLR_PINK, shotdata, 2),&start);
          add_list(init_objekt(OT_SHOT,ship->real[0].x,ship->real[0].y, ship -> angle+3, MAXTHRUST+12, DT_LINE, game_shotlife,CLR_PINK, shotdata, 2),&start);
          thrust_objekt(ship,-THRUSTPOWER*2);
          }
      if (game_rearshot)
          {
          add_list(init_objekt(OT_SHOT,ship->real[2].x,ship->real[2].y, ship -> angle+180, MAXTHRUST+12, DT_LINE, game_shotlife,CLR_PINK, shotdata, 2),&start);
          thrust_objekt(ship,THRUSTPOWER);
          }
      game_shooting = game_maxshots;
      }
   }
}

void insert_explosion (POINTL at, int nbr_pixels)
{
int tmpangle;
int count;
for (count = 0; count < nbr_pixels; count++)
	{
  tmpangle = random(360);
  add_list(init_objekt(OT_THRUST,at.x,at.y, tmpangle, random(10)+5, DT_PIXEL, 15, CLR_WHITE, pixeldata, 1),&effekts);
	}
}

void insert_shipexplosion (POINTL at, int nbr_lines)
{
int tmpangle;
int count;

for (count = 0; count < nbr_lines; count++)
  {
  tmpangle = random(360);
	add_list(init_objekt(OT_LINE,at.x,at.y, tmpangle, random(12)+5, DT_POLY, 30, CLR_YELLOW, linedata, 2),&effekts);
  }
}

void reset_hiscores (void)
{
strcpy(HiTable[0].name,"Dizzy");
HiTable[0].score = 2000;
HiTable[0].level = 4;
strcpy(HiTable[1].name,"HAL9000");
HiTable[1].score = 1000;
HiTable[1].level = 4;
strcpy(HiTable[2].name,"Sonja");
HiTable[2].score = 900;
HiTable[2].level = 4;
strcpy(HiTable[3].name,"Fil");
HiTable[3].score = 800;
HiTable[3].level = 3;
strcpy(HiTable[4].name,"Chicky");
HiTable[4].score = 700;
HiTable[4].level = 3;
strcpy(HiTable[5].name,"BJ");
HiTable[5].score = 600;
HiTable[5].level = 3;
strcpy(HiTable[6].name,"Carl");
HiTable[6].score = 500;
HiTable[6].level = 3;
strcpy(HiTable[7].name,"Foob");
HiTable[7].score = 400;
HiTable[7].level = 3;
strcpy(HiTable[8].name,"Fluf");
HiTable[8].score = 300;
HiTable[8].level = 3;
strcpy(HiTable[9].name,"Font");
HiTable[9].score = 100;
HiTable[9].level = 3;
}

void load_settings (void)
{
FILE *CfgFile;
if ((CfgFile = fopen("Driftos2.cfg","rb")) == NULL)
   return; // no cfg file found
fread(&saveonexit,sizeof(int),1,CfgFile);
fread(&detaillevel,sizeof(int),1,CfgFile);
fread(&current_lang,sizeof(int),1,CfgFile); /* may fail on old cfg - ignore */
if (current_lang < LANG_EN || current_lang > LANG_IT) current_lang = LANG_EN;
fclose(CfgFile);
}

void save_settings (void)
{
FILE *CfgFile;
if ((CfgFile = fopen("Driftos2.cfg","wb")) == NULL)
   return; // serious error just do not save
fwrite(&saveonexit,sizeof(int),1,CfgFile);
fwrite(&detaillevel,sizeof(int),1,CfgFile);
fwrite(&current_lang,sizeof(int),1,CfgFile);
fclose(CfgFile);
}

void load_hiscores (void)
{
int tel;
FILE *HiFileL;
if ((HiFileL = fopen("Hiscore.bin","rb")) == NULL)
   {
   // no hiscore file found
   reset_hiscores();
   save_hiscores();
   return;
   }
for (tel = 0; tel < 10; tel++)
    fread(&HiTable[tel],sizeof(struct HISCORE),1,HiFileL);
fclose(HiFileL);
}

void save_hiscores (void)
{
int tel;
FILE *HiFileS;
if ((HiFileS = fopen("Hiscore.bin","wb")) == NULL)
   {
   // serious error do something later
   return;
   }
for (tel = 0; tel < 10; tel++)
    fwrite(&HiTable[tel],sizeof(struct HISCORE),1,HiFileS);
fclose(HiFileS);
}

int check_hiscores (MPARAM mp_score )
{
int tel, tel2;
int end_score = (int) mp_score;
// sees if player has a hiscore
for (tel = 0; tel < 10; tel++)
		if (end_score > HiTable[tel].score)
			 {
			 for (tel2 = 9; tel2 > tel; tel2--)
					 HiTable[tel2] = HiTable[tel2-1];
			 HiTable[tel].score = end_score;
			 HiTable[tel].level = game_level;
			 return (tel);
			 }
return (10);
}

/*void debugdump (void)
{

LLIST *index;
fprintf(debug,"Debug Dump\r\n");
fprintf(debug,"llist_items:      %d\r\n",llist_items );
fprintf(debug,"Shieldduration:   %d\r\n",game_shieldduration);
fprintf(debug,"\r\n");
// process the llist
index = start;  
while (index != NULL)
      {
      fprintf(debug,"OBJEKT ID: %d\r\n",index -> objekt -> ID);
      fprintf(debug,"Life:      %d\r\n",index -> objekt -> life);
      fprintf(debug,"NotNew:    %d\r\n",index -> objekt -> notnew);
      fprintf(debug,"X:  %d,Y:  %d\r\n",index -> objekt -> current.x, index -> objekt -> current.y);
      fprintf(debug,"Color:     %d\r\n",index -> objekt -> color);
      fprintf(debug,"+++++++++++++++\r\n");
			index = index -> next;
      }
}  */

int edge (void)
{
// returns 0 or 2000
if (random(2))
   return(0);
return(2000);
}

