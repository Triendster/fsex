#ifndef CONFIG_H
#define CONFIG_H


#define FSEX_VP_MODE_LIST 0
#define FSEX_VP_MODE_GRID 1

// To get a vim-like feel
#define FSEX_NAV_MODE_UP 107 
#define FSEX_NAV_MODE_DOWN 106 
#define FSEX_NAV_MODE_LEFT 104
#define FSEX_NAV_MODE_RIGHT 108
#define FSEX_NAV_MODE_ENTER 10

const unsigned char fsex_vp_mode = FSEX_VP_MODE_LIST;

const char fsex_vp_bg_color[8][7] = 
{
    "000000",
    "ffffff",
    "ff0000",
    "00ff00",
    "0000ff",
    "ffff00",
    "ff00ff",
    "00ffff",
};

#endif
