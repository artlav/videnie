//############################################################################//
#define _CRT_SECURE_NO_DEPRECATE
//############################################################################//
#define STRICT
#define ORBITER_MODULE
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "orbitersdk.h"
#include "util.h"
#include "grhook.h"
//############################################################################//
#include "kost_shape.h"
#include "kost_elements.h"
#include "planettree.h"
//############################################################################//
int pks[256+3];
//############################################################################//
int mnu_on=0;
int lines_on=1;
int my_orbit=1;
int all_orbits=1;
int planet_orbits=1;
int allow_internal=0;
int show_landed=0;
int hide_behind=1;
int pan_mode=0;
int draw_tiny=0;
int shape_prec=200;
int tiny_cutoff=100000;
int no_msg=0;
int pan_control_mode=0;
int shutdown_mode=0;
//############################################################################//
int mx,dx=0,my,dy=0;
int thehei=1000;
char **excludes=NULL;
int ex_cnt=0;
VECTOR3 pan_off={0,0,0};
kostOrbitShape shape;
VESSEL *camvessel=NULL;
OBJHANDLE myvessel=0;
HPEN green_pen,red_pen,yellow_pen,blue_pen,orange_pen;
//############################################################################//
int nsh()
{
 if (pks[VK_CONTROL]) return 0;
 if (pks[VK_SHIFT])   return 0;
 if (pks[VK_MENU])    return 0;
 return 1;
}
//############################################################################//
void write_cfg()
{
 FILE *fp;
 if ((fp=fopen("orbitlines.cfg","w"))==NULL) {
 }else{
  fprintf(fp,"toggle=%d\n",lines_on);
  fprintf(fp,"precision=%d\n",shape_prec);
  fprintf(fp,"tiny_cutoff=%d\n",tiny_cutoff);
  fprintf(fp,"my_orbit=%d\n",my_orbit);
  fprintf(fp,"all_orbits=%d\n",all_orbits);
  fprintf(fp,"planet_orbits=%d\n",planet_orbits);
  fprintf(fp,"allow_internal=%d\n",allow_internal);
  fprintf(fp,"show_landed=%d\n",show_landed);
  fprintf(fp,"hide_behind=%d\n",hide_behind);
  fprintf(fp,"draw_tiny=%d\n",draw_tiny);
  fprintf(fp,"no_anoying_messages=%d\n",no_msg);
  fprintf(fp,"pan_control_mode=%d\n",pan_control_mode);
  for (int i=0;i<ex_cnt;i++) fprintf(fp,"exclude_object=%s\n",excludes[i]);
  fclose(fp);
 }
}
//############################################################################//
void add_exclude(char *name)
{
 excludes=(char**)realloc(excludes,(ex_cnt+1)*sizeof(char*));
 excludes[ex_cnt]=(char*)malloc(strlen(name)+1);
 strcpy(excludes[ex_cnt],name);
 ex_cnt++;
}
//############################################################################//
void keydown(int key)
{
 pks[key]=1;

 if ((key>=48)&&(key<=59)&&(mnu_on)) {
  if (key==48+0) lines_on=1-lines_on;
  if (key==48+1) my_orbit=1-my_orbit;
  if (key==48+2) all_orbits=1-all_orbits;
  if (key==48+3) planet_orbits=1-planet_orbits;
  if (key==48+4) allow_internal=1-allow_internal;
  if (key==48+5) show_landed=1-show_landed;
  if (key==48+6) hide_behind=1-hide_behind;
  if (key==48+7) pan_mode=1-pan_mode;
  if (key==48+8) draw_tiny=1-draw_tiny;
  if (key==48+9) pan_control_mode=1-pan_control_mode;
  write_cfg();
 }
 if ((pks[VK_MENU])&&(key==73)) mnu_on=1;else mnu_on=0;
}
//############################################################################//
void keyup(int key){pks[key]=0;}
//############################################################################//
void msevent(int evt,int x,int y)
{
 if (evt==WM_MBUTTONDOWN) pks[255+2]=1;
 if (evt==WM_MBUTTONUP)   pks[255+2]=0;
 if (evt==WM_LBUTTONDOWN) pks[255+0]=1;
 if (evt==WM_LBUTTONUP)   pks[255+0]=0;
 if (evt==WM_RBUTTONDOWN) pks[255+1]=1;
 if (evt==WM_RBUTTONUP)   pks[255+1]=0;
 if (evt==WM_MOUSEMOVE) {
  if (pan_control_mode) {
   if (pks[255+0]) {
    dx+=x-mx;
    dy+=y-my;
   }
  }else{
   if (pks[255+2]) {
    dx+=x-mx;
    dy+=y-my;
   }
  }
  mx=x;
  my=y;
 }
}
//############################################################################//
#define wrtxt32(x,y,txt,col) sprintf(cbuf,txt);len=strlen(cbuf);SetTextColor(hDC,col);TextOut(hDC,x,y,cbuf,len);
#define wrstr32(x,y,str,col)                   len=strlen(str); SetTextColor(hDC,col);TextOut(hDC,x,y,str ,len);
//############################################################################//
void draw_traj(HDC hDC,int wid,int hei,OBJHANDLE obj,OBJHANDLE ref,int color)
{
 char cbuf[255];
 int x,y,px,py;
 unsigned int i;
 MATRIX3 rotm;
 double apr,dst;
 VECTOR3 vp,cp,pp,vv,pv,vvvv;
 double mu,rad;
 kostElements elements;
 kostStateVector state;
 kostOrbitParam params;
 HPEN defcol;

 oapiGetObjectName(obj,cbuf,255);
 for (int j=0;j<ex_cnt;j++) if (!strcmp(cbuf,excludes[j])) return;


 if (color==0) defcol=green_pen;
 if (color==1) defcol=red_pen;
 if (color==2) defcol=yellow_pen;
 if (color==3) defcol=blue_pen;
 if (color==4) defcol=orange_pen;

 oapiGetGlobalPos(obj,&vp);
 oapiGetGlobalVel(obj,&vv);

 oapiGetGlobalPos(ref,&pp);
 oapiGetGlobalVel(ref,&pv);
 mu=GGRAV*(oapiGetMass(obj)+oapiGetMass(ref));
 rad=oapiGetSize(ref);

 oapiCameraGlobalPos(&cp);
 oapiCameraRotationMatrix(&rotm);
 apr=oapiCameraAperture();

 state.pos=vp-pp;
 state.vel=vv-pv;
 kostStateVector2Elements(mu,&state,&elements,&params);
 if (params.ApD>0)if(getob_apr(vp,params.ApD,cp,apr,hei)<10) return;
 if (getob_apr(vp,oapiGetSize(obj),cp,apr,hei)>hei/10) return;
 kostElements2Shape(&elements,&shape);

 for (i=1;i<shape.numPoints;i++) {
  if (hide_behind) {
   dst=line2sph(cp-pp,shape.points[i],_V(0,0,0),rad,vvvv);
   if ((dst>0)&&(modv(shape.points[i]+pp-cp)>modv(vvvv+pp-cp))&&(smulv(cp-pp,(cp-pp)-shape.points[i])>0)) continue;
  }
  unprjxy(wid,hei,rotm,shape.points[i-1]+pp-cp,apr,px,py);
  unprjxy(wid,hei,rotm,shape.points[i]+pp-cp,apr,x,y);
  if ((x==0)||(y==0)||(px==0)||(py==0)) continue;
  if (modv(shape.points[i])<rad) SelectObject(hDC,red_pen);else SelectObject(hDC,defcol);
  MoveToEx(hDC,px,py,NULL);
  LineTo(hDC,x,y);
 }

 if (hide_behind) {
  dst=line2sph(cp-pp,vp-pp,_V(0,0,0),rad,vvvv);
  if ((dst>0)&&(modv(vp-cp)>modv(vvvv+pp-cp))&&(smulv(cp-pp,(cp-pp)-(vp-pp))>0)) return;
 }
 unprjxy(wid,hei,rotm,vp-cp,apr,x,y);

 MoveToEx(hDC,x-10,y,NULL);LineTo(hDC,x-2,y);
 MoveToEx(hDC,x+10,y,NULL);LineTo(hDC,x+2,y);
 MoveToEx(hDC,x,y-10,NULL);LineTo(hDC,x,y-2);
 MoveToEx(hDC,x,y+10,NULL);LineTo(hDC,x,y+2);

 if (color==0) SetTextColor(hDC,0x00FF00);
 if (color==1) SetTextColor(hDC,0x0000FF);
 if (color==2) SetTextColor(hDC,0x00FFFF);
 if (color==3) SetTextColor(hDC,0xFF8080);
 if (color==4) SetTextColor(hDC,0x0080FF);
 TextOut(hDC,x+2,y-15,cbuf,strlen(cbuf));
}
//############################################################################//
void draw(HDC hDC,int wid,int hei)
{
 char cbuf[255];
 int len;
 OBJHANDLE ref,obj;
 VESSEL *v;
 int c,i;

 thehei=hei;

 if (lines_on) {
  if ((!oapiCameraInternal())||allow_internal) {

   if (my_orbit) {
    v=oapiGetFocusInterface();
    if (camvessel) v=oapiGetVesselInterface(myvessel);
    if ((show_landed)||(!v->GroundContact())) draw_traj(hDC,wid,hei,v->GetHandle(),v->GetGravityRef(),0);
   }

   if (all_orbits) {
    c=oapiGetVesselCount();
    if (c!=0) for (i=0;i<c;i++) {
     obj=oapiGetVesselByIndex(i);
     v=oapiGetVesselInterface(obj);
     if (v==oapiGetFocusInterface()) continue;
     if (v->GetHandle()==myvessel) continue;
     if (v->GroundContact()) if (!show_landed) continue;
     ref=v->GetGravityRef();
     draw_traj(hDC,wid,hei,obj,ref,3);
    }
   }

   if (planet_orbits) {
    c=oapiGetGbodyCount();
    for (i=0;i<c;i++) {
     obj=oapiGetGbodyByIndex(i);
     if (oapiGetObjectType(obj)==OBJTP_STAR) continue;
     if (!draw_tiny)if(oapiGetSize(obj)<tiny_cutoff) continue;
     ref=getCentralBody(obj);
     if ((obj!=0)&&(ref!=0)) {
      if (oapiGetObjectType(ref)==OBJTP_STAR) draw_traj(hDC,wid,hei,obj,ref,2);else draw_traj(hDC,wid,hei,obj,ref,4);
     }
    }
   }
  }
 }
 if (mnu_on) {
  if (lines_on)        {wrtxt32(5, 55,"Videnie ON (0 to toggle)",0x00C8FF);}else{wrtxt32(5, 55,"Videnie OFF (0 to toggle)",0x00C8FF);}
  if (my_orbit)        {wrtxt32(5, 75,"My orbit ON (1)"         ,0x00C8FF);}else{wrtxt32(5, 75,"My orbit OFF (1)"         ,0x00C8FF);}
  if (all_orbits)      {wrtxt32(5, 95,"Vessel orbits ON (2)"    ,0x00C8FF);}else{wrtxt32(5, 95,"Vessel orbits OFF (2)"    ,0x00C8FF);}
  if (planet_orbits)   {wrtxt32(5,115,"Planet orbits ON (3)"    ,0x00C8FF);}else{wrtxt32(5,115,"Planet orbits OFF (3)"    ,0x00C8FF);}
  if (allow_internal)  {wrtxt32(5,135,"Internal view shows (4)" ,0x00C8FF);}else{wrtxt32(5,135,"Internal view clean (4)"  ,0x00C8FF);}
  if (show_landed)     {wrtxt32(5,155,"Landed shown (5)"        ,0x00C8FF);}else{wrtxt32(5,155,"Landed not shown (5)"     ,0x00C8FF);}
  if (hide_behind)     {wrtxt32(5,175,"Hide behind planets (6)" ,0x00C8FF);}else{wrtxt32(5,175,"Show behind planets (6)"  ,0x00C8FF);}
  if (pan_mode)        {wrtxt32(5,195,"Panning mode ON (7)"     ,0x00C8FF);}else{wrtxt32(5,195,"Panning mode OFF (7)"     ,0x00C8FF);}
  if (draw_tiny)       {wrtxt32(5,215,"Tiny worlds drawn (8)"   ,0x00C8FF);}else{wrtxt32(5,215,"Tiny worlds ignored (8)"  ,0x00C8FF);}
  if (pan_control_mode){wrtxt32(5,235,"Panning control LMB (9)" ,0x00C8FF);}else{wrtxt32(5,235,"Panning control wheel (9)",0x00C8FF);}
 }
 if (!no_msg){
  if (pan_mode) {wrtxt32(wid/2-150,10,"Panning mode, do not save before disabling, could cause crashes",0x00C8FF);}
  if (!shutdown_mode) {
   wrtxt32(wid/2-150,30,"Orbiter shutdown mode is set to DEALLOCATE, BUGS GUARANTEED on restarts",0x00C8FF);
   wrtxt32(wid/2-150,50,"(Set no_anoying_messages to 1 or fix mode to disable this mesage)",0x00C8FF);
  }
 }
}
//############################################################################//
DLLCLBK void InitModule (HINSTANCE hDLL)
{
 char buf[255],val[255];
 FILE *fp;
 int i;

 if (grhlib==NULL) {
  load_grhook("modules\\grhook.dll",(DWORD)keydown,(DWORD)keyup,(DWORD)msevent,(DWORD)draw);
  if ((fp=fopen("orbitlines.cfg","r"))==NULL) {
  }else{
   i=0;
   while(!feof(fp)&&i++<100){
    fscanf(fp,"%[^=]=%[^=^\n]\n",buf,val);
    if (!strcmp(buf,"toggle"))              lines_on=atoi(val);
    if (!strcmp(buf,"precisionn"))          shape_prec=atoi(val);
    if (!strcmp(buf,"tiny_cutoff"))         tiny_cutoff=atoi(val);
    if (!strcmp(buf,"my_orbit"))            my_orbit=atoi(val);
    if (!strcmp(buf,"all_orbits"))          all_orbits=atoi(val);
    if (!strcmp(buf,"planet_orbits"))       planet_orbits=atoi(val);
    if (!strcmp(buf,"allow_internal"))      allow_internal=atoi(val);
    if (!strcmp(buf,"show_landed"))         show_landed=atoi(val);
    if (!strcmp(buf,"hide_behind"))         hide_behind=atoi(val);
    if (!strcmp(buf,"draw_tiny"))           draw_tiny=atoi(val);
    if (!strcmp(buf,"no_anoying_messages")) no_msg=atoi(val);
    if (!strcmp(buf,"exclude_object"))      add_exclude(val);
    if (!strcmp(buf,"pan_control_mode"))    pan_control_mode=atoi(val);
   }
   fclose(fp);
  }

  if ((fp=fopen("orbiter.cfg","r"))==NULL) {
   shutdown_mode=0;
  }else{
   while (!feof(fp)) {
    fscanf(fp,"%s\n",buf);
    if (!strcmp(buf,"ShutdownMode")) shutdown_mode=1;
   }
   fclose(fp);
  }
  if ((fp=fopen("Config\\Vessels\\camtgt.cfg","w"))==NULL) {
  }else{
   //fprintf(fp,"ClassName=camtgt\n");
   //fprintf(fp,"Module=plugins\\videnie\n");
   fprintf(fp,"ClassName=camtgt\n");
   fprintf(fp,"Size=10.0\n");
   fprintf(fp,"EnableFocus=TRUE\n");
   fprintf(fp,"Mass=2500\n");

   fclose(fp);
  }

  green_pen=CreatePen(PS_SOLID,1,RGB(0,255,0));
  red_pen=CreatePen(PS_SOLID,1,RGB(255,0,0));
  yellow_pen=CreatePen(PS_SOLID,1,RGB(255,255,0));
  orange_pen=CreatePen(PS_SOLID,1,RGB(255,128,0));
  blue_pen=CreatePen(PS_SOLID,1,RGB(128,128,255));
  shape.numPoints=shape_prec;
  shape.points=new kostVector3[shape.numPoints];
 }
}
//############################################################################//
DLLCLBK void ExitModule(HINSTANCE hDLL){FreeLibrary(grhlib);grhlib=0;}
//############################################################################//
void putcamtgt()
{
 VESSELSTATUS2 vs;

 vs.version=2;
 vs.flag=0;
 oapiGetFocusInterface()->GetStatusEx(&vs);
 camvessel=oapiGetVesselInterface(oapiCreateVesselEx("Target","camtgt",&vs));
 camvessel->SetSize(oapiGetFocusInterface()->GetSize());
 myvessel=oapiSetFocusObject(camvessel->GetHandle());
}
//############################################################################//
DLLCLBK void opcOpenRenderViewport(HWND hRenderWnd,DWORD width,DWORD height,BOOL fullscreen)
{
 if (!grhlib) return;
 initPlanetTree();
}
//############################################################################//
DLLCLBK void opcPreStep(double simt,double simdt,double mjd)
{
 int c,i;
 if (!grhlib) return;
 if (oapiCameraInternal()) pan_mode=0;

 if (pan_mode) {
  if (camvessel==NULL) {
   pan_off=_V(0,0,0);
   c=oapiGetVesselCount();
   if (c!=0) for (i=0;i<c;i++) {
    OBJHANDLE va=oapiGetVesselByIndex(i);
    VESSEL *vai=oapiGetVesselInterface(va);
    if (vai->GetClassName()!=NULL) if (!strcmp(vai->GetClassName(),"camtgt")) {
     camvessel=vai;
     myvessel=oapiGetFocusObject();
     break;
    }
   }
   if (camvessel==NULL) putcamtgt();
  }
  if (camvessel==NULL) {pan_mode=0;return;}
  if (camvessel->GetHandle()==myvessel) {
   myvessel=NULL;
   c=oapiGetVesselCount();
   if (c!=0) for (i=0;i<c;i++) {
    OBJHANDLE va=oapiGetVesselByIndex(i);
    VESSEL *vai=oapiGetVesselInterface(va);
    if (vai->GetClassName()!=NULL) if (strcmp(vai->GetClassName(),"camtgt")) {
     myvessel=va;
     break;
    }
   }
  }
  if (myvessel==NULL) {pan_mode=0;return;}

  MATRIX3 rmat;
  oapiCameraRotationMatrix(&rmat);


  double scale=oapiCameraTargetDist()/thehei;
  pan_off+=lvmat(rmat,_V(-dx,dy,0))*scale;
  VESSELSTATUS2 vs;
  vs.version=2;
  vs.flag=0;
  oapiGetVesselInterface(myvessel)->GetStatusEx(&vs);
  vs.rpos+=pan_off;
  camvessel->DefSetStateEx(&vs);
 }else{
  pan_off=_V(0,0,0);
  if (camvessel) {
   oapiDeleteVessel(camvessel->GetHandle(),myvessel);
   camvessel=NULL;
   myvessel=0;
  }
 }
 dx=0;
 dy=0;
}
//############################################################################//
