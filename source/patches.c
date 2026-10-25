/*
 * In-memory binary fixes for gangsters.exe (v1.0.0.1, timestamp 0x3726cffd, image base 0x400000).
 * Every patch verifies the original bytes first, so it silently does nothing on a different build.
 */
#include <windows.h>
#include <string.h>
#include "mci_music.h"

#define EXE_BASE 0x400000

/*
 * Fix 1: sprite frame blit (0x53fb?? "draw frame N of a .spr file").
 * The routine reads frame N's header (w,h,x,y,...) and only validates w*h <= 0x1900. If N is out of range
 * (the character-appearance code can request frame == frame count) the header is pixel data; w == 0 makes the
 * copy loop `dec ecx; jne` run 2^32 times and read off the end of the stack. Reject non-positive w or h as
 * well, which takes the routine's normal "return 0" failure path.
 *   0x53fca9: cmp ecx,0x1900 ; jle 0x53fcbc      (8 bytes)   -> jmp stub ; nop nop nop
 *   0x53fcb1: xor eax,eax ... ret 0x18           (failure return)
 *   0x53fcbc: normal path
 */
__asm__(
    ".globl _sprite_check_stub\n"
    "_sprite_check_stub:\n"
    "  cmpw $0, -0x40(%ebp)\n"      /* w */
    "  jle 1f\n"
    "  cmpw $0, -0x3e(%ebp)\n"      /* h */
    "  jle 1f\n"
    "  cmpl $0x1900, %ecx\n"        /* ecx = w*h */
    "  jg 1f\n"
    "  pushl $0x53fcbc\n"
    "  ret\n"
    "1:\n"
    "  pushl $0x53fcb1\n"
    "  ret\n");
extern char sprite_check_stub[];

static int patch(DWORD va, const BYTE *expect, const BYTE *repl, size_t n, const char *what) {
    BYTE *p = (BYTE *)va;
    if (memcmp(p, expect, n)) {
        shim_log("patch '%s' skipped: bytes at %#lx do not match (different exe build?)", what, (unsigned long)va);
        return 0;
    }
    DWORD old;
    if (!VirtualProtect(p, n, PAGE_EXECUTE_READWRITE, &old)) return 0;
    memcpy(p, repl, n);
    VirtualProtect(p, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), p, n);
    shim_log("patch '%s' applied at %#lx", what, (unsigned long)va);
    return 1;
}

/*
 * Fix 2: owner-drawn button painter (0x642e40). It calls GetWindowRect(button) and uses the screen left/top as
 * DirectDraw surface coordinates for the button art, which is only right when the control sits at its design
 * position. We move/scale controls to match the upscaled picture, so route this one call through
 * fix_GetWindowRect_game_space(), which reports design-space coordinates.
 *   0x642ecd: call dword ptr [0x905e58]   (FF 15 58 5E 90 00)
 */
extern BOOL WINAPI fix_GetWindowRect_game_space(HWND, LPRECT);
static BOOL (WINAPI *g_gwr_game_space_ptr)(HWND, LPRECT) = fix_GetWindowRect_game_space;

/*
 * Fix 3: the same painter ends with a single TextOutA that draws the button label at screen resolution straight onto
 * whatever DC it was given. We render the labels ourselves (overlay popups), and that duplicate flashes onto the
 * screen whenever the game triggers the painter. Turn that one call into a no-op (the DrawTextA calls before it
 * only measure text).
 *   0x64386e: call dword ptr [0x905ac0]   (FF 15 C0 5A 90 00)   ; TextOutA
 */
static BOOL WINAPI noop_text_out(HDC dc, int x, int y, LPCSTR s, int n) { (void)dc; (void)x; (void)y; (void)s; (void)n; return TRUE; }
static BOOL (WINAPI *g_noop_text_out_ptr)(HDC, int, int, LPCSTR, int) = noop_text_out;

/* Debug: periodically dump the game's CD-player object (global pointer at 0x7c001c) to the log. */
static VOID CALLBACK cd_dump_tick(HWND w, UINT m, UINT_PTR id, DWORD now) {
    (void)w; (void)m; (void)id; (void)now;
    DWORD *pp = (DWORD *)0x7c001c;
    if (IsBadReadPtr(pp, 4) || !*pp || IsBadReadPtr((void *)*pp, 0x80)) { shim_log("cd object: g_cd=%08lx (null/unreadable)", IsBadReadPtr(pp, 4) ? 0 : *pp); return; }
    DWORD *o = (DWORD *)*pp;
    shim_log("cd object %p: curTrack(+8)=%lu tracks(+28)=%lu dev(+2c)=%lu opened(+34)=%lu playing(+38)=%lu paused(+3c)=%lu enabled(+40)=%lu req(+48)=%lu",
             (void *)o, o[2], o[0x28 / 4], o[0x2c / 4], o[0x34 / 4], o[0x38 / 4], o[0x3c / 4], o[0x40 / 4], o[0x48 / 4]);
}
void patches_debug_cd_dump(int on) { if (on) SetTimer(NULL, 0, 4000, cd_dump_tick); }

void apply_mouse_alignment_patch(void);
void apply_mouse_poll_patch(void);
void apply_exe_patches(void) {
    apply_mouse_poll_patch();
    apply_mouse_alignment_patch();
    if ((DWORD_PTR)GetModuleHandleA(NULL) != EXE_BASE) {
        shim_log("exe not at expected base, skipping binary patches");
        return;
    }
    {
        static const BYTE expect[8] = {0x81, 0xf9, 0x00, 0x19, 0x00, 0x00, 0x7e, 0x0b};
        BYTE repl[8] = {0xe9, 0, 0, 0, 0, 0x90, 0x90, 0x90};
        DWORD rel = (DWORD)(DWORD_PTR)sprite_check_stub - (0x53fca9 + 5);
        memcpy(repl + 1, &rel, 4);
        patch(0x53fca9, expect, repl, 8, "sprite frame bounds check");
    }
    {
        static const BYTE expect[6] = {0xff, 0x15, 0x58, 0x5e, 0x90, 0x00};
        BYTE repl[6] = {0xff, 0x15, 0, 0, 0, 0};
        DWORD slot = (DWORD)(DWORD_PTR)&g_gwr_game_space_ptr;
        memcpy(repl + 2, &slot, 4);
        patch(0x642ecd, expect, repl, 6, "button painter GetWindowRect");
    }
    {
        static const BYTE expect[6] = {0xff, 0x15, 0xc0, 0x5a, 0x90, 0x00};
        BYTE repl[6] = {0xff, 0x15, 0, 0, 0, 0};
        DWORD slot = (DWORD)(DWORD_PTR)&g_noop_text_out_ptr;
        memcpy(repl + 2, &slot, 4);
        patch(0x64386e, expect, repl, 6, "button painter TextOutA (labels are drawn by the overlay)");
    }
}

/* Align the legacy relative cursor with the real client position. The original
 * accumulates DirectInput deltas * 2, independently from the Windows pointer. */
extern HWND window_fix_game_root(void);
extern void *goc_real_user32(const char *name);
void sync_game_mouse(void *object, LONG *mouse_state) {
    HWND top = window_fix_game_root();
    if(!top || !IsWindow(top)) top=GetActiveWindow();
    if (top) top = GetAncestor(top, GA_ROOT);
    WINDOWINFO wi = {sizeof wi}; POINT p;
    static BOOL (WINAPI *cursor_pos)(LPPOINT);
    if(!cursor_pos) cursor_pos=(void *)goc_real_user32("GetCursorPos");
    if (!top || !cursor_pos || !GetWindowInfo(top, &wi) || !cursor_pos(&p)) return;
    int rw=wi.rcClient.right-wi.rcClient.left, rh=wi.rcClient.bottom-wi.rcClient.top;
    if(rw<=0 || rh<=0) return;
    LONG *x=(LONG *)((BYTE *)object+0xa38), *y=(LONG *)((BYTE *)object+0xa3c);
    LONG xmin=*(LONG *)((BYTE *)object+0xa48), xmax=*(LONG *)((BYTE *)object+0xa4c);
    LONG ymin=*(LONG *)((BYTE *)object+0xa50), ymax=*(LONG *)((BYTE *)object+0xa54);
    if(xmax<xmin || ymax<ymin || xmax-xmin>16384 || ymax-ymin>16384) return;
    *x=xmin+MulDiv(p.x-wi.rcClient.left,xmax-xmin+1,rw);
    *y=ymin+MulDiv(p.y-wi.rcClient.top,ymax-ymin+1,rh);
    if(*x<xmin)*x=xmin; if(*x>xmax)*x=xmax;
    if(*y<ymin)*y=ymin; if(*y>ymax)*y=ymax;
    static int last_buttons=-1; int buttons=((BYTE *)mouse_state)[12];
    if(buttons!=last_buttons){shim_log("mouse state x=%ld y=%ld left=%d",*x,*y,buttons);last_buttons=buttons;}
    mouse_state[0]=mouse_state[1]=0;
}
__asm__(
    ".globl _mouse_sync_stub\n"
    "_mouse_sync_stub:\n"
    "  pushfl\n"
    "  pushal\n"
    "  leal 48(%esp), %eax\n"
    "  pushl %eax\n"
    "  pushl %esi\n"
    "  call _sync_game_mouse\n"
    "  addl $8, %esp\n"
    "  popal\n"
    "  popfl\n"
    "  movl 12(%esp), %ecx\n"
    "  movl 0xa38(%esi), %eax\n"
    "  pushl $0x7657ae\n"
    "  ret\n");
extern char mouse_sync_stub[];
void apply_mouse_alignment_patch(void) {
    if((DWORD_PTR)GetModuleHandleA(NULL)!=EXE_BASE)return;
    static const BYTE expect[10]={0x8b,0x4c,0x24,0x0c,0x8b,0x86,0x38,0x0a,0x00,0x00};
    BYTE repl[10]={0xe9,0,0,0,0,0x90,0x90,0x90,0x90,0x90};
    DWORD rel=(DWORD)(DWORD_PTR)mouse_sync_stub-(0x7657a4+5);
    memcpy(repl+1,&rel,4);
    patch(0x7657a4,expect,repl,10,"absolute client mouse alignment");
}

/* Polling very short clicks must not lose an entire command. Keep native
 * device data when available and recover button transitions from Win32. */
HRESULT WINAPI compat_mouse_state(void *device, DWORD size, void *data) {
    typedef HRESULT (WINAPI *Poll)(void *,DWORD,void *);
    HRESULT hr=((Poll)((void **)(*(void ***)device))[9])(device,size,data);
    if(size<16 || !data)return hr;
    if(hr<0)memset(data,0,size);
    BYTE *b=(BYTE *)data;
    SHORT left=GetAsyncKeyState(VK_LBUTTON), right=GetAsyncKeyState(VK_RBUTTON), middle=GetAsyncKeyState(VK_MBUTTON);
    b[12]=(left & 0x8001)?128:0;
    b[13]=(right & 0x8001)?128:0;
    b[14]=(middle & 0x8001)?128:0;
    return 0;
}
void apply_mouse_poll_patch(void) {
    static const BYTE expect[5]={0x8b,0x1f,0xff,0x53,0x24};
    BYTE repl[5]={0xe8,0,0,0,0};
    DWORD rel=(DWORD)(DWORD_PTR)compat_mouse_state-(0x765773+5);
    memcpy(repl+1,&rel,4);
    patch(0x765773,expect,repl,5,"mouse button polling compatibility");
}
