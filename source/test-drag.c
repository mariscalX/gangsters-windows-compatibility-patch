/* Regression for the legacy 630/470 cursor wrap. Test the actual replacement. */
#include "patches.c"
#include <assert.h>
#include <stdio.h>
void shim_log(const char *fmt, ...) {(void)fmt;}
int main(void) {
    BYTE object[0x200]={0};
    int *anchor=(int *)(object+0x98), mapx=0,mapy=0;
    absolute_drag_anchor(object,800,600);
    for(int i=0;i<10000;i++) {
        mapx+=800-anchor[0];mapy+=600-anchor[1];
        absolute_drag_anchor(object,800,600);
    }
    assert(mapx==0 && mapy==0);
    for(int i=1;i<=20;i++) {
        mapx+=800+i-anchor[0];mapy+=600+i-anchor[1];
        absolute_drag_anchor(object,800+i,600+i);
    }
    assert(mapx==20 && mapy==20);
    puts("PASS: stationary RMB has zero accumulated pan; 20px motion stays 20px beyond legacy limits");
}
HWND window_fix_game_root(void){return NULL;}
void *goc_real_user32(const char *name){return NULL;}
BOOL WINAPI fix_GetWindowRect_game_space(HWND h,LPRECT r){return FALSE;}
