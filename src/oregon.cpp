#include <SDL.h>
#include <oregon.h>


//#include <windows.h>


#include <keyboard.h>
#include <array>
#include "pic.h"
#include <cstdio>
#include <sdlmain.h>

// using namespace std;

int interval = 0;
int stage = 0;
int count = 0;
int travelState = 0; // 1 = landmark     2 = river      3 = river crossing
int lmCount = 0;

int disableFrames = 0;

Uint8 name = 0;
/*
0 = waiting for bios screen & autoexec.bat to run
1 = run menus
2 = wait for "Independence, Missouri" screen to load
3 = set rations and pace

*/

bool shift = false;

const KBD_KEYS menu[43] = {KBD_space, KBD_1, KBD_enter, KBD_1, KBD_enter, KBD_e, KBD_enter, KBD_enter, KBD_y, KBD_enter, KBD_2, KBD_enter, KBD_space, KBD_space, KBD_space, KBD_space, KBD_space, KBD_space, KBD_1, KBD_enter, KBD_8, KBD_enter, KBD_2, KBD_enter, KBD_2, KBD_0, KBD_0, KBD_0, KBD_enter, KBD_5, KBD_enter, KBD_2, KBD_enter, KBD_2, KBD_enter, KBD_2, KBD_enter, KBD_space, KBD_space };

const KBD_KEYS menu2[12] = { KBD_space, KBD_5, KBD_enter, KBD_3, KBD_enter, KBD_4, KBD_enter, KBD_3, KBD_enter, KBD_1, KBD_enter, KBD_space };

const KBD_KEYS river[5] = { KBD_n, KBD_enter, KBD_space, KBD_2, KBD_enter };

const KBD_KEYS landmark[3] = { KBD_n, KBD_enter, KBD_space };

const KBD_KEYS repair[3] = { KBD_y, KBD_enter, KBD_space };

const KBD_KEYS fork[6] = { KBD_n, KBD_enter, KBD_NONE, KBD_1, KBD_enter, KBD_space };

const KBD_KEYS forkfix[3] = { KBD_1, KBD_enter, KBD_space };

const KBD_KEYS fork2[5] = { KBD_n, KBD_enter, KBD_2, KBD_enter, KBD_space };

const KBD_KEYS fork3[7] = { KBD_n, KBD_enter, KBD_2, KBD_enter, KBD_y, KBD_enter, KBD_space };

bool release = false;
KBD_KEYS releaseKey;

void kbdTick() {
    if(release) {
        release = false;
        KEYBOARD_AddKey(releaseKey, false);
        if(shift) {
            KEYBOARD_AddKey(KBD_leftshift, false);
            shift = false;
        }
    }
    return;
}

void pressKey(KBD_KEYS key) {
    if(key == KBD_semicolon) {
        shift = true;
        KEYBOARD_AddKey(KBD_leftshift, true);
    }
    KEYBOARD_AddKey(key, true);
    releaseKey = key;
    release = true;

    return;
}

std::array<Uint8, 3> getPixel(int x, int y, SDL_Surface* surface) {
    if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8* row_start = (Uint8*)surface->pixels + y * surface->pitch;
    Uint8* pixel_ptr = row_start + x * surface->format->BytesPerPixel;

    Uint32 pixel_value = 0;
    memcpy(&pixel_value, pixel_ptr, surface->format->BytesPerPixel);

    Uint8 r, g, b;
    SDL_GetRGB(pixel_value, surface->format, &r, &g, &b);
    
    if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    std::array<Uint8, 3> rgb = { r,g,b };

    return rgb;
}

void setName(Uint8 goalName) {
    name = goalName;
    interval = 0;
    stage = 0;
    count = 0;
}

bool travel() {
    const std::array<Uint8, 3> black = { 0, 0, 0 };
    const std::array<Uint8, 3> white = { 255, 255, 255 };
    const std::array<Uint8, 3> gray = { 170, 170, 170 };
    const std::array<Uint8, 3> magenta = { 186, 0, 174 };
    const std::array<Uint8, 3> darkgreen = { 69, 113, 0 };
    const std::array<Uint8, 3> tan = { 146, 93, 52 };

    //OutputDebugStringA(std::to_string(lmCount).c_str());

    switch(travelState) {
    case 0:
        pressKey(KBD_space);

        //OutputDebugString(std::to_string(  getPixel(182, 161, sdl.surface) == white  ).c_str());

        if(disableFrames == 0
            && getPixel(73, 152, sdl.surface) == black
            && getPixel(101, 157, sdl.surface) == white
            && getPixel(182, 161, sdl.surface) == white
            && getPixel(312, 156, sdl.surface) == white) {

            lmCount++;

            interval = 0;
            count = 0;
            if(lmCount == 1 || lmCount == 2 || lmCount == 9 || lmCount == 12) {
                travelState = 1;
                //OutputDebugStringA(":|\n");
            }
            else if(lmCount == 8) {
                travelState = 3;
                //OutputDebugStringA(":)\n");
            }
            else if(lmCount == 14) {
                travelState = 5;
            }
            else if(lmCount == 15) {
                travelState = 6;
            }
            else {
                travelState = 2;
                //OutputDebugStringA(":(\n");
            }
        }
        if(disableFrames == 0
            && getPixel(17, 159, sdl.surface) == black
            && getPixel(94, 160, sdl.surface) == white
            && getPixel(133, 170, sdl.surface) == white
            && getPixel(191, 172, sdl.surface) == white) {
            travelState = 4;
            interval = 0;
            count = 0;
        }
        if(disableFrames == 0
            && getPixel(108, 174, sdl.surface) == black
            && getPixel(58, 148, sdl.surface) == white
            && getPixel(244, 160, sdl.surface) == white
            && getPixel(320, 257, sdl.surface) == white) {
            travelState = 7;
            interval = 0;
            count = 0;
        }
        if(disableFrames == 0
            && getPixel(638, 82, sdl.surface) == magenta
            && getPixel(294, 276, sdl.surface) == darkgreen
            && getPixel(352, 158, sdl.surface) == tan
            && getPixel(517, 334, sdl.surface) == white) {
            exit(0);
        }
        if(disableFrames == 0
            && getPixel(255, 85, sdl.surface) == gray
            && getPixel(344, 95, sdl.surface) == white
            ||
            getPixel(72, 195, sdl.surface) == white
            && getPixel(91, 384, sdl.surface) == white
            && getPixel(6, 97, sdl.surface) == white
            && getPixel(344, 324, sdl.surface) == black
            ) {
            exit(1);
        }
        if(disableFrames > 0) {
            disableFrames--;
        }
        break;
    case 1:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(river[count]);
            if(count == 5) {
                travelState = 0;
                disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 2:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(landmark[count]);
            if(count == 3) {
                travelState = 0;
                disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 3:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(fork[count]);
            if(count == 6) {
                travelState = 0;
                //disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 4:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(repair[count]);
            if(count == 3) {
                travelState = 0;
                disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 5:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(fork2[count]);
            if(count == 5) {
                travelState = 0;
                disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 6:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(fork3[count]);
            if(count == 5) {
                travelState = 0;
                disableFrames = 10;
            }
            else {
                count++;
            }
        }
        break;
    case 7:
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(forkfix[count]);
            if(count == 3) {
                travelState = 0;
                disableFrames = 10;
                lmCount++;
            }
            else {
                count++;
            }
        }
        break;
    }
    return false; //temp
}

void playerTick(Bitu /*val*/) {

    kbdTick();

    switch(name) {
    case 0:
        //OutputDebugStringA("part 1\n");
        /*  wait for BIOS and autoexec  */
        if(count == 15) {
            setName(1);
        }
        else {
            count++;
        }
        break;
    case 1:
        //OutputDebugStringA("part 2\n");
        /*  input starting menus  */
        interval++;
        if(interval >= 2 /* && menu[count] != KBD_NONE*/) {
            interval = 0;
            pressKey(menu[count]);
            if(count == 43) {
                setName(2);
            }
            else {
                count++;
            }
        }
        break;
    case 2:
        //OutputDebugStringA("part 3\n");
        /*  wait for independence missouri screen to load  */
        if(count == 15) {
            setName(3);
        }
        else {
            count++;
        }
        break;
    case 3:
        //OutputDebugStringA("part 4\n");
        /*  set rations and pace  */
        interval++;
        if(interval >= 2) {
            interval = 0;
            pressKey(menu2[count]);
            if(count == 12) {
                setName(4);
            }
            else {
                count++;
            }
        }
        break;
    case 4:
        travel();
        break;
    }

    PIC_AddEvent(playerTick, 50);
}

void oregonTrailPlayerInit() {
    PIC_AddEvent(playerTick, 5000);
}


/*

FOR REFRENCE

surface = sdl.surface

pressKey(menu[count]);
if(count == 51) {
    name = 2;
}
else {
    count++;
}

    if(chungus == 1) {
        KEYBOARD_AddKey(KBD_k, true);
        chungus = 0;
    }
    else {
        KEYBOARD_AddKey(KBD_p, true);
        chungus = 1;
    }



    if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    printf("BPP: %d, Pitch: %d\n", surface->format->BytesPerPixel, surface->pitch);
    printf("Surface size: %d x %d\n", surface->w, surface->h);

    int x = 10;
    int y = 10;

    Uint8* row_start = (Uint8*)surface->pixels + y * surface->pitch;
    Uint8* pixel_ptr = row_start + x * surface->format->BytesPerPixel;

    Uint32 pixel_value = 0;
    memcpy(&pixel_value, pixel_ptr, surface->format->BytesPerPixel);

    Uint8 r, g, b;
    SDL_GetRGB(pixel_value, surface->format, &r, &g, &b);

    char msg[200];
    sprintf(msg, "surface->w=%d, surface->h=%d, BPP=%d, pitch=%d, pixel_value=0x%X, r=%d, g=%d, b=%d\n",
        surface->w, surface->h, surface->format->BytesPerPixel, surface->pitch,
        pixel_value, r, g, b);
    OutputDebugStringA(msg);

    if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    //OutputDebugStringW(msg.c_str());
*/
