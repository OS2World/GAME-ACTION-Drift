/****************************************************************************
 * KEYMAP.C - keyboard, mouse and joystick control of the ship, and the     *
 *            "Key Mapping..." dialog (notebook with three tabs).           *
 *                                                                          *
 *  Every action (turn left, turn right, thrust, fire) can be driven by a   *
 *  key, by the mouse and by a joystick at the same time.  The mappings are *
 *  stored in the settings file by km_save() / km_load().                   *
 ****************************************************************************/
#define INCL_BASE
#define INCL_WIN
#define INCL_GPI
#define INCL_DOSPROCESS
#define INCL_WININPUT

#include "drift.h"
#include "joyos2.h"

/* the game state this module needs to know about (DRIFT.C) */
extern int game_status;
extern int game_pauze;

/* ------------------------------------------------------------------ data */

/* keyboard: the physical scan code is polled, vk/chr are only for display */
KMKEY km_keys[KM_ACTIONS] = {
    { 0x63, VK_LEFT,  0 },          /* turn left  */
    { 0x64, VK_RIGHT, 0 },          /* turn right */
    { 0x61, VK_UP,    0 },          /* thrust     */
    { 0x39, VK_SPACE, 0 }           /* fire       */
};

/* mouse: 0=none, 1-4=pointer moved up/down/left/right, 5-7=left/right/middle button */
#define MOUSE_UP      1
#define MOUSE_DOWN    2
#define MOUSE_LEFT    3
#define MOUSE_RIGHT   4
#define MOUSE_BTN1    5
#define MOUSE_BTN3    7
int km_mouse_on = FALSE;
int km_mouse[KM_ACTIONS] = { MOUSE_LEFT, MOUSE_RIGHT, MOUSE_UP, MOUSE_BTN1 };

/* joystick: 0=none, 1-4=button, 5-8=axis left/right/up/down */
#define JOY_AXIS_L  5
#define JOY_AXIS_R  6
#define JOY_AXIS_U  7
#define JOY_AXIS_D  8
int km_joy_sel = 0;                 /* 0=off, 1=stick A, 2=stick B */
int km_joy[KM_ACTIONS] = { JOY_AXIS_L, JOY_AXIS_R, JOY_AXIS_U, 1 };

static HFILE             hGame = 0;             /* GAME$ driver            */
static GAME_2DPOS_STRUCT joyCalA = { 0, 0 };    /* centre of stick A       */
static GAME_2DPOS_STRUCT joyCalB = { 0, 0 };    /* centre of stick B       */
static BOOL              bJoyAvail[2];          /* stick A / B reported    */

/* sampled once per game tick by km_tick() */
static int  st_joy[KM_ACTIONS];
static int  st_mouse[KM_ACTIONS];
static int  mouse_dir[5];                       /* indexed by MOUSE_UP..   */
static int  mouse_btn[3];
static BOOL bPointerHidden = FALSE;

static const USHORT usButBit[4] = { JOY_BUT1_BIT, JOY_BUT2_BIT, JOY_BUT3_BIT, JOY_BUT4_BIT };

/* --------------------------------------------------------------- display */

static const struct { SHORT code; const char *name; } vkNames[] = {
    { 0x01, "Button1" }, { 0x02, "Button2" }, { 0x03, "Button3" },
    { 0x04, "Break" },   { 0x05, "Backspace" }, { 0x06, "Tab" },
    { 0x07, "Backtab" }, { 0x08, "Return" },  { 0x09, "Shift" },
    { 0x0A, "Ctrl" },    { 0x0B, "Alt" },     { 0x0C, "Altgraf" },
    { 0x0D, "Pause" },   { 0x0E, "Caps Lock" }, { 0x0F, "Esc" },
    { 0x10, "Space" },   { 0x11, "Page Up" }, { 0x12, "Page Down" },
    { 0x13, "End" },     { 0x14, "Home" },    { 0x15, "Left" },
    { 0x16, "Up" },      { 0x17, "Right" },   { 0x18, "Down" },
    { 0x19, "PrintScrn" }, { 0x1A, "Insert" }, { 0x1B, "Delete" },
    { 0x1C, "ScrlLock" }, { 0x1D, "Num Lock" }, { 0x1E, "Enter" },
    { 0x1F, "SysRq" },   { 0x20, "F1" },      { 0x21, "F2" },
    { 0x22, "F3" },      { 0x23, "F4" },      { 0x24, "F5" },
    { 0x25, "F6" },      { 0x26, "F7" },      { 0x27, "F8" },
    { 0x28, "F9" },      { 0x29, "F10" },     { 0x30, "F11" },
    { 0x31, "F12" }
};
#define NVKNAMES (sizeof(vkNames) / sizeof(vkNames[0]))

void km_keyname(const KMKEY *k, char *out)
{
    unsigned i;

    if (k->vk) {
        for (i = 0; i < NVKNAMES; i++)
            if (vkNames[i].code == k->vk) {
                strcpy(out, vkNames[i].name);
                return;
            }
    }
    if (k->chr > 32 && k->chr < 127) {
        out[0] = (char)toupper(k->chr);
        out[1] = 0;
        return;
    }
    if (k->scan)
        sprintf(out, "Key %02X", k->scan);
    else
        strcpy(out, "None");
}

static const char *MouseLabel(int n)
{
    static const char * const s[] = {
        "None", "Move Up", "Move Down", "Move Left", "Move Right",
        "Left Button", "Right Button", "Middle Button"
    };
    return (n >= 0 && n <= MOUSE_BTN3) ? s[n] : "?";
}

static const char *JoyLabel(int n)
{
    static const char * const s[] = {
        "None", "Btn 1", "Btn 2", "Btn 3", "Btn 4",
        "Axis L", "Axis R", "Axis U", "Axis D"
    };
    return (n >= 0 && n <= 8) ? s[n] : "?";
}

/* "~Key Mapping..." -> "Key Mapping" */
static void plain_text(const char *in, char *out)
{
    char *start = out;

    while (*in) {
        if (*in != '~')
            *out++ = *in;
        in++;
    }
    *out = 0;
    while (out > start && *(out - 1) == '.')
        *--out = 0;
}

static const int actStr[KM_ACTIONS] = { STR_ACT_LEFT, STR_ACT_RIGHT, STR_ACT_THRUST, STR_ACT_FIRE };

/* ---------------------------------------------------------------- joystick */

void km_init(void)
{
    APIRET            rc;
    ULONG             action, dataLen;
    GAME_PARM_STRUCT  parms;
    GAME_CALIB_STRUCT calib;

    rc = DosOpen(GAMEPDDNAME, &hGame, &action, 0, FILE_READONLY, FILE_OPEN,
                 OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE, NULL);
    if (rc != NO_ERROR)
        hGame = 0;
    else {
        dataLen = sizeof(parms);
        rc = DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_PARMS,
                         NULL, 0, NULL, &parms, dataLen, &dataLen);
        if (rc != NO_ERROR) { DosClose(hGame); hGame = 0; }
    }
    if (hGame != 0) {
        bJoyAvail[0] = (parms.useA & GAME_USE_BOTH_NEWMASK) != 0;
        bJoyAvail[1] = (parms.useB & GAME_USE_BOTH_NEWMASK) != 0;
        dataLen = sizeof(calib);
        if (DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_CALIB,
                        NULL, 0, NULL, &calib, dataLen, &dataLen) == NO_ERROR) {
            joyCalA.x = calib.Ax.centre;  joyCalA.y = calib.Ay.centre;
            joyCalB.x = calib.Bx.centre;  joyCalB.y = calib.By.centre;
        }
    }
    /* a saved joystick choice only holds if the driver reports that stick */
    if (km_joy_sel == 1 && !bJoyAvail[0]) km_joy_sel = 0;
    if (km_joy_sel == 2 && !bJoyAvail[1]) km_joy_sel = 0;
    if (!hGame) km_joy_sel = 0;
}

void km_pointer_show(void)
{
    if (bPointerHidden) {
        WinShowPointer(HWND_DESKTOP, TRUE);
        bPointerHidden = FALSE;
    }
}

void km_done(void)
{
    km_pointer_show();
    if (hGame) {
        DosClose(hGame);
        hGame = 0;
    }
}

/* Read the stick; fills in the pressed state of the four actions */
static void joy_sample(void)
{
    GAME_STATUS_STRUCT s;
    ULONG              len = sizeof(s);
    GAME_2DPOS_STRUCT *jd, *cal;
    SHORT              tolX, tolY, dX, dY;
    BOOL               axL, axR, axU, axD;
    int                a, c;

    for (a = 0; a < KM_ACTIONS; a++)
        st_joy[a] = 0;
    if (!km_joy_sel || !hGame)
        return;
    if (DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_STATUS,
                    NULL, 0, NULL, &s, len, &len) != NO_ERROR)
        return;
    jd   = (km_joy_sel == 1) ? &s.curdata.A : &s.curdata.B;
    cal  = (km_joy_sel == 1) ? &joyCalA : &joyCalB;
    tolX = (SHORT)(abs((int)cal->x) / 10 + 1);
    tolY = (SHORT)(abs((int)cal->y) / 10 + 1);
    dX   = (SHORT)(jd->x - cal->x);
    dY   = (SHORT)(jd->y - cal->y);
    axL = dX < -tolX;  axR = dX > tolX;
    axU = dY < -tolY;  axD = dY > tolY;
    for (a = 0; a < KM_ACTIONS; a++) {
        c = km_joy[a];
        if (c >= 1 && c <= 4)
            st_joy[a] = (s.curdata.butMask & usButBit[c - 1]) == 0;
        else if (c == JOY_AXIS_L) st_joy[a] = axL;
        else if (c == JOY_AXIS_R) st_joy[a] = axR;
        else if (c == JOY_AXIS_U) st_joy[a] = axU;
        else if (c == JOY_AXIS_D) st_joy[a] = axD;
    }
}

/* ------------------------------------------------------------------ mouse */

void km_mouse_button(int btn, int down)
{
    if (btn >= 0 && btn < 3)
        mouse_btn[btn] = down;
}

/* Called for every WM_TIMER tick: samples mouse and joystick */
void km_tick(HWND hwnd)
{
    BOOL  active;
    int   a, c;
    RECTL rcl;
    POINTL ctr, ptl;

    joy_sample();

    active = km_mouse_on && game_status == GAME_PLAYING && !game_pauze &&
             WinQueryFocus(HWND_DESKTOP) == hwnd;
    for (a = 0; a < 5; a++)
        mouse_dir[a] = 0;
    for (a = 0; a < KM_ACTIONS; a++)
        st_mouse[a] = 0;

    if (!active) {
        km_pointer_show();
        mouse_btn[0] = mouse_btn[1] = mouse_btn[2] = 0;
        return;
    }
    WinQueryWindowRect(hwnd, &rcl);
    ctr.x = (rcl.xRight - rcl.xLeft) / 2;
    ctr.y = (rcl.yTop - rcl.yBottom) / 2;
    WinMapWindowPoints(hwnd, HWND_DESKTOP, &ctr, 1);
    if (!bPointerHidden) {
        WinShowPointer(HWND_DESKTOP, FALSE);
        bPointerHidden = TRUE;
        WinSetPointerPos(HWND_DESKTOP, ctr.x, ctr.y);
    }
    WinQueryPointerPos(HWND_DESKTOP, &ptl);
    mouse_dir[MOUSE_UP]    = (ptl.y - ctr.y > 0);
    mouse_dir[MOUSE_DOWN]  = (ptl.y - ctr.y < 0);
    mouse_dir[MOUSE_LEFT]  = (ctr.x - ptl.x > 0);
    mouse_dir[MOUSE_RIGHT] = (ctr.x - ptl.x < 0);
    WinSetPointerPos(HWND_DESKTOP, ctr.x, ctr.y);   /* re-centre every tick */

    for (a = 0; a < KM_ACTIONS; a++) {
        c = km_mouse[a];
        if (c >= MOUSE_UP && c <= MOUSE_RIGHT)
            st_mouse[a] = mouse_dir[c];
        else if (c >= MOUSE_BTN1 && c <= MOUSE_BTN3)
            st_mouse[a] = mouse_btn[c - MOUSE_BTN1];
    }
}

/* Mouse buttons are used by the game right now (not by the window frame) */
int km_mouse_on_game(void)
{
    return km_mouse_on && game_status == GAME_PLAYING;
}

/* Is the action asked for by the keyboard, the mouse or the joystick? */
int km_action_down(int act)
{
    if (km_keys[act].scan &&
        (WinGetPhysKeyState(HWND_DESKTOP, km_keys[act].scan) & 0x8000))
        return 1;
    return st_joy[act] || st_mouse[act];
}

/* "UP   Thrust" style line for the intro legend */
void km_legend(int act, char *out)
{
    char name[16];

    km_keyname(&km_keys[act], name);
    sprintf(out, "%-10s %s", name, tr(actStr[act]));
}

/* --------------------------------------------------------------- settings */

typedef struct {
    int ver;
    int scan[KM_ACTIONS], vk[KM_ACTIONS], chr[KM_ACTIONS];
    int mouse_on, mouse[KM_ACTIONS];
    int joy_sel, joy[KM_ACTIONS];
} KMCFG;

void km_save(FILE *f)
{
    KMCFG c;
    int   i;

    c.ver = 1;
    for (i = 0; i < KM_ACTIONS; i++) {
        c.scan[i]  = km_keys[i].scan;
        c.vk[i]    = km_keys[i].vk;
        c.chr[i]   = km_keys[i].chr;
        c.mouse[i] = km_mouse[i];
        c.joy[i]   = km_joy[i];
    }
    c.mouse_on = km_mouse_on;
    c.joy_sel  = km_joy_sel;
    fwrite(&c, sizeof(c), 1, f);
}

/* Older settings files stop before the mappings: nothing is changed then */
void km_load(FILE *f)
{
    KMCFG c;
    int   i, ok = 1;

    if (fread(&c, sizeof(c), 1, f) != 1 || c.ver != 1)
        return;
    for (i = 0; i < KM_ACTIONS; i++) {
        if (c.scan[i] < 0 || c.scan[i] > 255 || c.vk[i] < 0 || c.vk[i] > 255 ||
            c.chr[i] < 0 || c.chr[i] > 255 ||
            c.mouse[i] < 0 || c.mouse[i] > MOUSE_BTN3 ||
            c.joy[i] < 0 || c.joy[i] > 8)
            ok = 0;
    }
    if (c.joy_sel < 0 || c.joy_sel > 2)
        ok = 0;
    if (!ok)
        return;
    for (i = 0; i < KM_ACTIONS; i++) {
        km_keys[i].scan = c.scan[i];
        km_keys[i].vk   = c.vk[i];
        km_keys[i].chr  = c.chr[i];
        km_mouse[i]     = c.mouse[i];
        km_joy[i]       = c.joy[i];
    }
    km_mouse_on = c.mouse_on ? TRUE : FALSE;
    km_joy_sel  = c.joy_sel;
}

/* ============================================================== dialogs */

#define WM_KM_INPUT   (WM_USER + 1)

typedef struct _KMIN {
    USHORT type;                    /* 0 = key down, 1 = mouse button down */
    USHORT fs;                      /* KC_* flags of the key               */
    UCHAR  vkey;                    /* virtual key                         */
    UCHAR  scan;                    /* scan code                           */
    USHORT chr;                     /* character                           */
    USHORT btn;                     /* 0..2 = left, right, middle button   */
} KMIN;

static PFNWP pfnEntryOrig;

/* Keys and mouse buttons pressed in a mapping box go to the owning page */
static MRESULT EXPENTRY KmEntryProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HWND   hwndOwner = WinQueryWindow(hwnd, QW_OWNER);
    USHORT id        = WinQueryWindowUShort(hwnd, QWS_ID);
    KMIN   ki;

    switch (msg) {
    case WM_CHAR: {
        USHORT fs = CHARMSG(&msg)->fs;
        USHORT vk = CHARMSG(&msg)->vkey;
        /* Tab/Esc/F1 and Alt combinations keep their dialog meaning */
        if (fs & KC_ALT)
            break;
        if ((fs & KC_VIRTUALKEY) &&
            (vk == VK_TAB || vk == VK_BACKTAB || vk == VK_ESC || vk == VK_F1))
            break;
        if (!(fs & KC_KEYUP)) {
            ki.type = 0;
            ki.fs   = fs;
            ki.vkey = (UCHAR)vk;
            ki.scan = CHARMSG(&msg)->scancode;
            ki.chr  = CHARMSG(&msg)->chr;
            ki.btn  = 0;
            WinSendMsg(hwndOwner, WM_KM_INPUT, MPFROMSHORT(id), MPFROMP(&ki));
        }
        return (MRESULT)TRUE;
    }
    case WM_BUTTON1DOWN:
    case WM_BUTTON2DOWN:
    case WM_BUTTON3DOWN:
        ki.type = 1;
        ki.fs   = 0;
        ki.vkey = 0;
        ki.scan = 0;
        ki.chr  = 0;
        ki.btn  = (msg == WM_BUTTON1DOWN) ? 0 : (msg == WM_BUTTON2DOWN) ? 1 : 2;
        if (WinSendMsg(hwndOwner, WM_KM_INPUT, MPFROMSHORT(id), MPFROMP(&ki)))
            return (MRESULT)TRUE;
        break;
    }
    return pfnEntryOrig(hwnd, msg, mp1, mp2);
}

static VOID KmSubclass(HWND hwndPage, USHORT id)
{
    HWND  hwndE = WinWindowFromID(hwndPage, id);
    PFNWP p;

    if (hwndE == NULLHANDLE)
        return;
    p = WinSubclassWindow(hwndE, KmEntryProc);
    if (p != NULL && pfnEntryOrig == NULL)
        pfnEntryOrig = p;
}

/* Enable or disable every child of hwnd except the one with excludeId */
static VOID EnableChildren(HWND hwnd, USHORT excludeId, BOOL en)
{
    HENUM  he = WinBeginEnumWindows(hwnd);
    HWND   hc;
    while ((hc = WinGetNextWindow(he)) != NULLHANDLE) {
        if (WinQueryWindowUShort(hc, QWS_ID) != excludeId)
            WinEnableWindow(hc, en);
    }
    WinEndEnumWindows(he);
}

/* ------------------------------------------------------------ keyboard tab */

static KMKEY kmKeyTmp[KM_ACTIONS];

static MRESULT EXPENTRY KmKeyboardProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    int  i;
    char name[16];

    switch (msg) {
    case WM_INITDLG:
        for (i = 0; i < KM_ACTIONS; i++) {
            WinSetDlgItemText(hwnd, IDC_KM_KL1 + i, (PSZ)tr(actStr[i]));
            kmKeyTmp[i] = km_keys[i];
            km_keyname(&kmKeyTmp[i], name);
            WinSetDlgItemText(hwnd, IDC_KM_KBD1 + i, (PSZ)name);
            KmSubclass(hwnd, (USHORT)(IDC_KM_KBD1 + i));
        }
        return (MRESULT)FALSE;

    case WM_KM_INPUT: {
        KMIN *ki = (KMIN *)PVOIDFROMMP(mp2);
        INT   n  = (INT)SHORT1FROMMP(mp1) - IDC_KM_KBD1;

        if (n < 0 || n >= KM_ACTIONS || ki->type != 0)
            return (MRESULT)FALSE;          /* mouse click: just take focus */
        if (ki->vkey == VK_SHIFT || ki->vkey == VK_CTRL || ki->vkey == VK_ALT ||
            ki->scan == 0)
            return (MRESULT)TRUE;           /* modifier or unknown key */
        kmKeyTmp[n].scan = ki->scan;
        kmKeyTmp[n].vk   = 0;
        kmKeyTmp[n].chr  = 0;
        if ((ki->fs & KC_VIRTUALKEY) && ki->vkey >= 1 && ki->vkey <= 0x31)
            kmKeyTmp[n].vk = ki->vkey;
        else if ((ki->fs & KC_CHAR) && ki->chr > 32 && ki->chr < 127)
            kmKeyTmp[n].chr = ki->chr;
        km_keyname(&kmKeyTmp[n], name);
        WinSetDlgItemText(hwnd, SHORT1FROMMP(mp1), (PSZ)name);
        return (MRESULT)TRUE;
    }

    case WM_COMMAND:
        if (SHORT1FROMMP(mp1) == DID_OK)
            for (i = 0; i < KM_ACTIONS; i++)
                km_keys[i] = kmKeyTmp[i];
        return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* --------------------------------------------------------------- mouse tab */

static MRESULT EXPENTRY KmMouseProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static INT iTmpMouse[KM_ACTIONS];
    int i;

    switch (msg) {
    case WM_INITDLG: {
        BOOL bEn = km_mouse_on;
        WinSendDlgItemMsg(hwnd, IDC_KM_MOUSE_EN, BM_SETCHECK,
                          MPFROMSHORT(bEn ? 1 : 0), MPVOID);
        for (i = 0; i < KM_ACTIONS; i++) {
            iTmpMouse[i] = km_mouse[i];
            WinSetDlgItemText(hwnd, IDC_KM_ML1 + i, (PSZ)tr(actStr[i]));
            WinSetDlgItemText(hwnd, IDC_KM_MBTN1 + i, (PSZ)MouseLabel(iTmpMouse[i]));
            KmSubclass(hwnd, (USHORT)(IDC_KM_MBTN1 + i));
        }
        EnableChildren(hwnd, IDC_KM_MOUSE_EN, bEn);
        return (MRESULT)FALSE;
    }

    case WM_KM_INPUT: {
        KMIN *ki = (KMIN *)PVOIDFROMMP(mp2);
        INT   a  = (INT)SHORT1FROMMP(mp1) - IDC_KM_MBTN1;
        INT   code = -1;

        if (a < 0 || a >= KM_ACTIONS)
            return (MRESULT)FALSE;
        if (ki->type == 1) {
            /* the first click only moves the focus into the box */
            if (WinQueryFocus(HWND_DESKTOP) !=
                WinWindowFromID(hwnd, SHORT1FROMMP(mp1)))
                return (MRESULT)FALSE;
            code = MOUSE_BTN1 + (INT)ki->btn;
        } else if (ki->fs & KC_VIRTUALKEY) {
            switch (ki->vkey) {
            case VK_UP:        code = MOUSE_UP;    break;
            case VK_DOWN:      code = MOUSE_DOWN;  break;
            case VK_LEFT:      code = MOUSE_LEFT;  break;
            case VK_RIGHT:     code = MOUSE_RIGHT; break;
            case VK_BACKSPACE:
            case VK_DELETE:    code = 0;           break;
            }
        }
        if (code >= 0) {
            iTmpMouse[a] = code;
            WinSetDlgItemText(hwnd, SHORT1FROMMP(mp1), (PSZ)MouseLabel(code));
        }
        return (MRESULT)TRUE;
    }

    case WM_CONTROL:
        if (SHORT1FROMMP(mp1) == IDC_KM_MOUSE_EN &&
            SHORT2FROMMP(mp1) == BN_CLICKED) {
            BOOL bEn = (BOOL)(SHORT)(ULONG)WinSendDlgItemMsg(hwnd,
                            IDC_KM_MOUSE_EN, BM_QUERYCHECK, MPVOID, MPVOID);
            EnableChildren(hwnd, IDC_KM_MOUSE_EN, bEn);
        }
        return 0;

    case WM_COMMAND:
        if (SHORT1FROMMP(mp1) == DID_OK) {
            km_mouse_on = (BOOL)(SHORT)(ULONG)WinSendDlgItemMsg(hwnd, IDC_KM_MOUSE_EN,
                                                  BM_QUERYCHECK, MPVOID, MPVOID);
            for (i = 0; i < KM_ACTIONS; i++)
                km_mouse[i] = iTmpMouse[i];
        }
        return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* ------------------------------------------------------------ joystick tab */

/* Calibrate and stick choices follow what the driver reports */
static VOID KmCalEnable(HWND hwnd)
{
    WinEnableWindow(WinWindowFromID(hwnd, IDC_KM_CAL),  hGame && bJoyAvail[0]);
    WinEnableWindow(WinWindowFromID(hwnd, IDC_KM_CALB), hGame && bJoyAvail[1]);
    if (!hGame)
        WinEnableWindow(WinWindowFromID(hwnd, IDC_KM_JOY_EN), FALSE);
    if (!bJoyAvail[0])
        WinEnableWindow(WinWindowFromID(hwnd, IDC_KM_JOY_A), FALSE);
    if (!bJoyAvail[1])
        WinEnableWindow(WinWindowFromID(hwnd, IDC_KM_JOY_B), FALSE);
}

/* Centre the stick, then read its position as the neutral point */
static VOID CalibrateStick(HWND hwnd, BOOL isA)
{
    CHAR szMsg[64]; CHAR szTitle[32];

    if (!hGame)
        return;
    sprintf(szMsg,  "Center Joystick %c, then click OK.", isA ? 'A' : 'B');
    sprintf(szTitle, "Calibrate Joystick %c", isA ? 'A' : 'B');
    if (WinMessageBox(HWND_DESKTOP, hwnd, szMsg, szTitle, 0,
                      MB_OKCANCEL | MB_INFORMATION) == MBID_OK) {
        GAME_STATUS_STRUCT joyCal;
        ULONG calLen = sizeof(joyCal);
        if (DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_STATUS,
                        NULL, 0, NULL, &joyCal, calLen, &calLen) == NO_ERROR) {
            if (isA) {
                joyCalA.x = joyCal.curdata.A.x;
                joyCalA.y = joyCal.curdata.A.y;
            } else {
                joyCalB.x = joyCal.curdata.B.x;
                joyCalB.y = joyCal.curdata.B.y;
            }
        }
    }
}

static MRESULT EXPENTRY KmJoystickProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static int    iTmpJoy;
    static int    iTmpAct[KM_ACTIONS];
    static int    iSelAct;
    static USHORT usPrevButs;
    static BOOL   bPAxL, bPAxR, bPAxU, bPAxD;
    int  i;

    switch (msg) {
    case WM_INITDLG:
        iTmpJoy = km_joy_sel;
        for (i = 0; i < KM_ACTIONS; i++)
            iTmpAct[i] = km_joy[i];
        iSelAct    = -1;
        usPrevButs = 0xFFFF;
        bPAxL = bPAxR = bPAxU = bPAxD = FALSE;
        /* prime the axis state so a stick held off-centre does not assign */
        if (hGame) {
            GAME_STATUS_STRUCT s0;
            ULONG len0 = sizeof(s0);
            if (DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_STATUS,
                            NULL, 0, NULL, &s0, len0, &len0) == NO_ERROR) {
                GAME_2DPOS_STRUCT *jd0  = (iTmpJoy != 2) ? &s0.curdata.A : &s0.curdata.B;
                GAME_2DPOS_STRUCT *cal0 = (iTmpJoy != 2) ? &joyCalA : &joyCalB;
                SHORT tx = (SHORT)(abs((int)cal0->x) / 10 + 1);
                SHORT ty = (SHORT)(abs((int)cal0->y) / 10 + 1);
                SHORT dx = (SHORT)(jd0->x - cal0->x);
                SHORT dy = (SHORT)(jd0->y - cal0->y);
                bPAxL = dx < -tx;  bPAxR = dx >  tx;
                bPAxU = dy < -ty;  bPAxD = dy >  ty;
            }
        }
        WinSendDlgItemMsg(hwnd, IDC_KM_JOY_EN, BM_SETCHECK,
                          MPFROMSHORT(iTmpJoy != 0), MPVOID);
        WinSendDlgItemMsg(hwnd, (iTmpJoy == 2) ? IDC_KM_JOY_B : IDC_KM_JOY_A,
                          BM_SETCHECK, MPFROMSHORT(1), MPVOID);
        for (i = 0; i < KM_ACTIONS; i++) {
            WinSetDlgItemText(hwnd, IDC_KM_JL1 + i, (PSZ)tr(actStr[i]));
            WinSetDlgItemText(hwnd, IDC_KM_JBTN1 + i, (PSZ)JoyLabel(iTmpAct[i]));
            KmSubclass(hwnd, (USHORT)(IDC_KM_JBTN1 + i));
        }
        EnableChildren(hwnd, IDC_KM_JOY_EN, iTmpJoy != 0);
        KmCalEnable(hwnd);
        WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, 1, 50);
        return (MRESULT)FALSE;

    case WM_TIMER:
        /* poll the stick: a new button press or deflection goes to the focused box */
        if (hGame && iTmpJoy != 0) {
            GAME_STATUS_STRUCT s;
            ULONG len = sizeof(s);
            if (DosDevIOCtl(hGame, IOCTL_CAT_USER, GAME_GET_STATUS,
                            NULL, 0, NULL, &s, len, &len) == NO_ERROR) {
                GAME_2DPOS_STRUCT *jd  = (iTmpJoy == 1) ? &s.curdata.A : &s.curdata.B;
                GAME_2DPOS_STRUCT *cal = (iTmpJoy == 1) ? &joyCalA : &joyCalB;
                SHORT tolX = (SHORT)(abs((int)cal->x) / 10 + 1);
                SHORT tolY = (SHORT)(abs((int)cal->y) / 10 + 1);
                SHORT dX = (SHORT)(jd->x - cal->x);
                SHORT dY = (SHORT)(jd->y - cal->y);
                USHORT curButs = s.curdata.butMask;
                USHORT pressed = usPrevButs & ~curButs;
                BOOL axL = dX < -tolX, axR = dX > tolX;
                BOOL axU = dY < -tolY, axD = dY >  tolY;
                INT  newAct = 0;
                if (pressed) {
                    for (i = 0; i < 4; i++)
                        if (pressed & usButBit[i]) { newAct = i + 1; break; }
                }
                if (!newAct) {
                    if (axL && !bPAxL) newAct = JOY_AXIS_L;
                    else if (axR && !bPAxR) newAct = JOY_AXIS_R;
                    else if (axU && !bPAxU) newAct = JOY_AXIS_U;
                    else if (axD && !bPAxD) newAct = JOY_AXIS_D;
                }
                if (newAct && iSelAct >= 0) {
                    iTmpAct[iSelAct] = newAct;
                    WinSetDlgItemText(hwnd, IDC_KM_JBTN1 + iSelAct, (PSZ)JoyLabel(newAct));
                }
                usPrevButs = curButs;
                bPAxL = axL; bPAxR = axR; bPAxU = axU; bPAxD = axD;
            }
        }
        return 0;

    case WM_KM_INPUT: {
        KMIN *ki = (KMIN *)PVOIDFROMMP(mp2);
        INT   r  = (INT)SHORT1FROMMP(mp1) - IDC_KM_JBTN1;
        if (ki->type != 0)
            return (MRESULT)FALSE;          /* mouse click: just take focus */
        if (r >= 0 && r < KM_ACTIONS && (ki->fs & KC_VIRTUALKEY) &&
            (ki->vkey == VK_BACKSPACE || ki->vkey == VK_DELETE)) {
            iTmpAct[r] = 0;
            WinSetDlgItemText(hwnd, (USHORT)(IDC_KM_JBTN1 + r), (PSZ)JoyLabel(0));
        }
        return (MRESULT)TRUE;
    }

    case WM_CONTROL:
        if (SHORT1FROMMP(mp1) >= IDC_KM_JBTN1 && SHORT1FROMMP(mp1) < IDC_KM_JBTN1 + KM_ACTIONS) {
            if (SHORT2FROMMP(mp1) == EN_SETFOCUS)
                iSelAct = SHORT1FROMMP(mp1) - IDC_KM_JBTN1;
            else if (SHORT2FROMMP(mp1) == EN_KILLFOCUS)
                iSelAct = -1;
            return 0;
        }
        if (SHORT2FROMMP(mp1) == BN_CLICKED) {
            switch (SHORT1FROMMP(mp1)) {
            case IDC_KM_JOY_EN: {
                BOOL en = (BOOL)(SHORT)(ULONG)WinSendDlgItemMsg(hwnd,
                              IDC_KM_JOY_EN, BM_QUERYCHECK, MPVOID, MPVOID);
                if (!en) {
                    iTmpJoy = 0;
                } else {
                    BOOL isB = (BOOL)(SHORT)(ULONG)WinSendDlgItemMsg(hwnd,
                                   IDC_KM_JOY_B, BM_QUERYCHECK, MPVOID, MPVOID);
                    iTmpJoy = isB ? 2 : 1;
                }
                EnableChildren(hwnd, IDC_KM_JOY_EN, en);
                KmCalEnable(hwnd);
                break;
            }
            case IDC_KM_JOY_A: if (iTmpJoy != 0) iTmpJoy = 1; break;
            case IDC_KM_JOY_B: if (iTmpJoy != 0) iTmpJoy = 2; break;
            }
        }
        return 0;

    case WM_COMMAND:
        switch (SHORT1FROMMP(mp1)) {
        case DID_OK:
            km_joy_sel = iTmpJoy;
            for (i = 0; i < KM_ACTIONS; i++)
                km_joy[i] = iTmpAct[i];
            break;
        case IDC_KM_CAL:
            CalibrateStick(hwnd, TRUE);
            break;
        case IDC_KM_CALB:
            CalibrateStick(hwnd, FALSE);
            break;
        }
        return 0;

    case WM_DESTROY:
        WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, 1);
        return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* ------------------------------------------------------------- main dialog */

static HWND  hwndNB, hwndKbd, hwndMouse, hwndJoy;
static ULONG ulKbdPage, ulMousePage, ulJoyPage;

static MRESULT EXPENTRY KeyMappingDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

    switch (msg) {
    case WM_INITDLG: {
        char title[64];
        SWP  swpFr;

        /* a dialog window is the frame itself: the title bar belongs to hwnd */
        plain_text(tr(STR_KEYMAPPING), title);
        WinSetWindowText(hwnd, (PSZ)title);
        WinQueryWindowPos(hwnd, &swpFr);
        WinSetWindowPos(hwnd, HWND_TOP,
            (WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN) - swpFr.cx) / 2,
            (WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - swpFr.cy) / 2,
            0, 0, SWP_MOVE | SWP_ACTIVATE);

        hwndNB = WinCreateWindow(hwnd, WC_NOTEBOOK, "",
                     BKS_TABBEDDIALOG | BKS_BACKPAGESBR | WS_VISIBLE | WS_TABSTOP,
                     0, 0, 1, 1, hwnd, HWND_TOP, IDC_KM_NOTEBOOK, NULL, NULL);
        if (hwndNB == NULLHANDLE) {
            WinDismissDlg(hwnd, FALSE);
            return (MRESULT)FALSE;
        }
        {
            SWP swpBtn, swpDlg;
            WinQueryWindowPos(hwnd, &swpDlg);
            WinQueryWindowPos(WinWindowFromID(hwnd, DID_OK), &swpBtn);
            WinSetWindowPos(hwndNB, HWND_TOP,
                4, swpBtn.y + swpBtn.cy + 4,
                swpDlg.cx - 8,
                swpDlg.cy - (swpBtn.y + swpBtn.cy + 4) - 4 -
                    WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR),
                SWP_MOVE | SWP_SIZE);
        }

        /* the strip of the page area the page window does not cover has the
           dialog colour (it was white) */
        WinSendMsg(hwndNB, BKM_SETNOTEBOOKCOLORS,
                   MPFROMLONG(WinQuerySysColor(HWND_DESKTOP, SYSCLR_DIALOGBACKGROUND, 0)),
                   MPFROMSHORT(BKA_BACKGROUNDPAGECOLOR));

        ulKbdPage = (ULONG)WinSendMsg(hwndNB, BKM_INSERTPAGE, NULL,
                    MPFROM2SHORT(BKA_AUTOPAGESIZE | BKA_MAJOR, BKA_LAST));
        WinSendMsg(hwndNB, BKM_SETTABTEXT, MPFROMLONG(ulKbdPage), MPFROMP("Keyboard"));
        ulMousePage = (ULONG)WinSendMsg(hwndNB, BKM_INSERTPAGE, NULL,
                      MPFROM2SHORT(BKA_AUTOPAGESIZE | BKA_MAJOR, BKA_LAST));
        WinSendMsg(hwndNB, BKM_SETTABTEXT, MPFROMLONG(ulMousePage), MPFROMP("Mouse"));
        ulJoyPage = (ULONG)WinSendMsg(hwndNB, BKM_INSERTPAGE, NULL,
                    MPFROM2SHORT(BKA_AUTOPAGESIZE | BKA_MAJOR, BKA_LAST));
        WinSendMsg(hwndNB, BKM_SETTABTEXT, MPFROMLONG(ulJoyPage), MPFROMP("Joystick"));

        hwndKbd = WinLoadDlg(hwndNB, hwndNB, KmKeyboardProc, NULLHANDLE, IDD_KM_KEYBOARD, NULL);
        WinSendMsg(hwndNB, BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulKbdPage), MPFROMHWND(hwndKbd));
        hwndMouse = WinLoadDlg(hwndNB, hwndNB, KmMouseProc, NULLHANDLE, IDD_KM_MOUSE, NULL);
        WinSendMsg(hwndNB, BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulMousePage), MPFROMHWND(hwndMouse));
        hwndJoy = WinLoadDlg(hwndNB, hwndNB, KmJoystickProc, NULLHANDLE, IDD_KM_JOYSTICK, NULL);
        WinSendMsg(hwndNB, BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulJoyPage), MPFROMHWND(hwndJoy));

        WinSendMsg(hwndNB, BKM_TURNTOPAGE, MPFROMLONG(ulKbdPage), MPVOID);
        return (MRESULT)FALSE;
    }

    case WM_SIZE:
        if (hwndNB) {
            SWP swpBtn, swpDlg;
            WinQueryWindowPos(hwnd, &swpDlg);
            WinQueryWindowPos(WinWindowFromID(hwnd, DID_OK), &swpBtn);
            WinSetWindowPos(hwndNB, HWND_TOP,
                4, swpBtn.y + swpBtn.cy + 4,
                swpDlg.cx - 8,
                swpDlg.cy - (swpBtn.y + swpBtn.cy + 4) - 4 -
                    WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR),
                SWP_MOVE | SWP_SIZE);
        }
        break;

    case WM_COMMAND:
        switch (SHORT1FROMMP(mp1)) {
        case DID_OK:
            WinSendMsg(hwndKbd,   WM_COMMAND, MPFROMSHORT(DID_OK), MPVOID);
            WinSendMsg(hwndMouse, WM_COMMAND, MPFROMSHORT(DID_OK), MPVOID);
            WinSendMsg(hwndJoy,   WM_COMMAND, MPFROMSHORT(DID_OK), MPVOID);
            WinDismissDlg(hwnd, TRUE);
            break;
        case DID_CANCEL:
            WinDismissDlg(hwnd, FALSE);
            break;
        }
        return 0;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

void km_dialog(HWND owner)
{
    km_pointer_show();
    hwndNB = NULLHANDLE;                    /* no stale handle for WM_SIZE */
    WinDlgBox(HWND_DESKTOP, owner, KeyMappingDlgProc, NULLHANDLE, IDD_KEYMAPPING, NULL);
}
