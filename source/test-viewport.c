#include <assert.h>
#include <stdio.h>
#include "viewport_math.h"
int main(void){
 int x,y,n=0;
 int sizes[][2]={{640,480},{1024,768},{1280,960},{1920,1080},{1366,768},{1000,1000}};
 for(int i=0;i<6;i++){
  int rw=sizes[i][0],rh=sizes[i][1];
  assert(viewport_point(rw,rh,1024,768,rw/2,rh/2,&x,&y));
  assert(x>=511&&x<=513&&y>=383&&y<=385);n++;
  assert(!viewport_point(rw,rh,1024,768,-1,rh/2,&x,&y));n++;
  assert(!viewport_point(rw,rh,1024,768,rw,rh/2,&x,&y));n++;
 }
 assert(!viewport_point(1920,1080,1024,768,239,540,&x,&y));n++;
 assert(viewport_point(1920,1080,1024,768,240,540,&x,&y)&&x==0);n++;
 assert(!viewport_point(1920,1080,1024,768,1680,540,&x,&y));n++;
 assert(!viewport_point(0,0,1024,768,0,0,&x,&y));n++;
 printf("PASS %d viewport cases: native/resized centers, letterboxing and outside points\n",n);
}

