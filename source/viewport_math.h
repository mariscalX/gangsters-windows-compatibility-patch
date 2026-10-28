#ifndef GOC_VIEWPORT_MATH_H
#define GOC_VIEWPORT_MATH_H
/* Physical client -> logical surface. Outside letterboxing is not a map edge. */
static int viewport_point(int rw,int rh,int lw,int lh,int px,int py,int *x,int *y){
 if(rw<=0||rh<=0||lw<=0||lh<=0)return 0;
 int vw=rw,vh=rh;
 if((long long)rw*lh>(long long)rh*lw)vw=(int)((long long)rh*lw/lh);
 else vh=(int)((long long)rw*lh/lw);
 int ox=(rw-vw)/2,oy=(rh-vh)/2;
 if(vw<=0||vh<=0||px<ox||py<oy||px>=ox+vw||py>=oy+vh)return 0;
 *x=(int)((long long)(px-ox)*lw/vw);
 *y=(int)((long long)(py-oy)*lh/vh);
 return 1;
}
#endif
