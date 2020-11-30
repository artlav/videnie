#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
//#######################################################################################//
// Orbiter and generic util C
// Made in 2007-2010 by Artlav
//#######################################################################################//
#include <stdio.h>
#include <math.h>
#ifdef USE_ORBITER
#include "orbitersdk.h"
#endif
#include "util.h"
//#######################################################################################//
bool operator !=(VECTOR3 a,VECTOR3 b) {if((a.x!=b.x)||(a.y!=b.y)||(a.z!=b.z)) return true; else return false;}
VECTOR3 operator -(VECTOR3 &a,VECTOR3 &b){return tvec(a.x-b.x,a.y-b.y,a.z-b.z);}
VECTOR3 operator +(VECTOR3 &a,VECTOR3 &b){return tvec(a.x+b.x,a.y+b.y,a.z+b.z);}
VECTOR3 operator /(VECTOR3 a,double b){return tvec(a.x/b,a.y/b,a.z/b);}
VECTOR3 operator *(VECTOR3 &a,double &b){return tvec(a.x*b,a.y*b,a.z*b);}
VECTOR3 vmulv(VECTOR3 v1,VECTOR3 v2){return tvec(v1.y*v2.z-v1.z*v2.y,-v1.x*v2.z+v1.z*v2.x,v1.x*v2.y-v1.y*v2.x);}
double  smulv(VECTOR3 v1,VECTOR3 v2){return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;}
VECTOR3 dmulv(VECTOR3 v1,VECTOR3 v2){return tvec(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z);}
double  modv(VECTOR3 v){return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
double  modvs(VECTOR3 v){return v.x*v.x+v.y*v.y+v.z*v.z;}
VECTOR3 vrotz(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return tvec(v.x*c-v.y*s,v.x*s+v.y*c,v.z);}
VECTOR3 vroty(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return tvec(v.x*c+v.z*s,v.y        ,-v.x*s+v.z*c);}
VECTOR3 vrotx(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return tvec(v.x        ,v.y*c-v.z*s, v.y*s+v.z*c);}
//#######################################################################################//
VECTOR3 nrvec(VECTOR3 iv)
{
 double md;
 VECTOR3 re;

 md=modv(iv);
 if (absd(md)<0.001) {re=iv; return re;}

 re.x=iv.x/md;
 re.y=iv.y/md;
 re.z=iv.z/md;
 return re;
}
//#######################################################################################//
VECTOR3 vrec2sphv(VECTOR3 v)
{
 VECTOR3 t;
 double a1,a2,h,m,n;

 a1=v.y/sqrt(sqr(v.x)+sqr(v.z));
 a2=v.z/v.x;
 h=sqrt(sqr(v.x)+sqr(v.y)+sqr(v.z));
 m=atan(a1);
 n=atan(a2);
 if((v.x>0)&&(v.z>0))n=n;
 if((v.x<0)&&(v.z>0))n=n+PI;
 if((v.x<0)&&(v.z<0))n=n-PI;
 if((v.x>0)&&(v.z<0))n=n;

 t.x=m;
 t.y=n;
 t.z=h;
 return t;
}
//#######################################################################################//
double spherical_distance(VECTOR3 a,VECTOR3 b,double rad)
{
 double d=sqr(sin((b.x-a.x)/2))+cos(a.x)*cos(b.x)*sqr(sin((b.y-a.y)/2));
 return 2*rad*atan2(sqrt(d),sqrt(1-d));
}
//#######################################################################################//
MATRIX3 emat={1,0,0,0,1,0,0,0,1};
//#######################################################################################//
MATRIX3 epsmat(MATRIX3 a)
{
 int i;
 MATRIX3 result;
 for(i=0;i<9;i++){result.data[i]=a.data[i];if(absd(a.data[i])<0.00001)result.data[i]=0;}
 return result;
}
//#######################################################################################//
VECTOR3 tamat(MATRIX3 a)
{
 VECTOR3 result;
 result.x=atan2(a.m23,a.m33);
 result.y=-asin(a.m13);
 result.z=atan2(a.m12,a.m11);
 return result;
}
//#######################################################################################//
double getrtang(double pmx,double pmy,double wx,double wy)
{
 double dx,dy,result;

 result=0;
 if((pmx==wx)&&(pmy==wy))result=0;
 if((pmx==wx)&&(pmy>wy))result=180;
 if((pmx==wx)&&(pmy<wy))result=0;
 if((pmx<wx)&&(pmy==wy))result=270;
 if((pmx>wx)&&(pmy==wy))result=90;
 if((pmx>wx)&&(pmy<wy)){
  dx=pmx-wx;
  dy=wy-pmy;
  result=atan(dx/dy)*180/PI;
 }
 if((pmx>wx)&&(pmy>wy)){
  dx=pmx-wx;
  dy=pmy-wy;
  result=90+atan(dy/dx)*180/PI;
 }
 if((pmx<wx)&&(pmy>wy)){
  dx=wx-pmx;
  dy=pmy-wy;
  result=180+atan(dx/dy)*180/PI;
 }
 if((pmx<wx)&&(pmy<wy)){
  dx=wx-pmx;
  dy=wy-pmy;
  result=270+atan(dy/dx)*180/PI;
 }
 return result;
}
//#######################################################################################//
MATRIX3 atmat(VECTOR3 a)
{
 double sinx,siny,sinz,cosx,cosy,cosz;
 MATRIX3 result;

 sinx=sin(a.x);cosx=cos(a.x);
 siny=sin(a.y);cosy=cos(a.y);
 sinz=sin(a.z);cosz=cos(a.z);

 result.data[0]=cosy*cosz;
 result.data[1]=cosy*sinz;
 result.data[2]=-siny;
 result.data[3]=cosz*sinx*siny-sinz*cosx;
 result.data[4]=sinx*siny*sinz+cosx*cosz;
 result.data[5]=sinx*cosy;
 result.data[6]=cosx*siny*cosz+sinz*sinx;
 result.data[7]=sinz*cosx*siny-sinx*cosz;
 result.data[8]=cosx*cosy;
 return result;
}
//#######################################################################################//
MATRIX3 vecs2mat_orb(VECTOR3 f,VECTOR3 up)
{
 VECTOR3 s,u;
 MATRIX3 m;

 f=nrvec(f);
 if((up.x==0)&&(up.y==0)&&(up.z==0))return emat;
 up=nrvec(up);

 s=vmulv(up,f);
 u=vmulv(f,s);

 m.data[0]=s.x;
 m.data[3]=s.y;
 m.data[6]=s.z;

 m.data[1]=u.x;
 m.data[4]=u.y;
 m.data[7]=u.z;

 m.data[2]=f.x;
 m.data[5]=f.y;
 m.data[8]=f.z;

 return m;
}
//#######################################################################################//
double linterp(double x1,double y1,double x2,double y2,double x){return y1+(y2-y1)*((x-x1)/(x2-x1));}
double calc_kd(double t,double i,double kp){return 2*sqrt(i*absd(kp)/t)*((kp>0)?1:-1);}
//#######################################################################################//
VECTOR3 get_pitchyawroll(VECTOR3 py,VECTOR3 r)
{
 VECTOR3 res,plv,pyw;
 double ac,sm;

 py=nrvec(py);
 res.x=-asin(py.y);
 plv=nrvec(py-(tvec(0,1,0)*smulv(py,tvec(0,1,0))));
 pyw=nrvec(tvec(0,0,1)-(tvec(0,1,0)*smulv(tvec(0,0,1),tvec(0,1,0))));

 sm=smulv(plv,pyw);
 if(absd(sm)>=1)ac=0; else ac=acos(sm);
 res.y=-sgn(smulv(py,tvec(1,0,0)))*ac;

 if(modv(r)<=0.001){res.z=0;return res;}
 r=nrvec(r);
 res.z=atan2(r.x,r.y);

 return res;
}
//#######################################################################################//
void Crt2Pol(VECTOR3 &pos,VECTOR3 &vel)
{
 //position in polar coordinates
 double r     =sqrt  (pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);
 double lng   =atan2 (pos.z, pos.x);
 double lat   =asin  (pos.y/r);
 //derivatives in polar coordinates
 double drdt  =(vel.x*pos.x+vel.y*pos.y+vel.z*pos.z)/r;
 double dlngdt=(vel.z*pos.x-pos.z*vel.x)/(pos.x*pos.x+pos.z*pos.z);
 double dlatdt=(vel.y*r-pos.y*drdt)/(r*sqrt(r*r-pos.y*pos.y));
 pos.x=r;
 pos.y=lng;
 pos.z=lat;
 vel.x=drdt;
 vel.y=dlngdt;
 vel.z=dlatdt;
}
//#######################################################################################//
//#######################################################################################//
typedef double (newt_fnc)(double,void*);
//#######################################################################################//
double newton(newt_fnc F,newt_fnc dFdx,double tgt_y,double x1,double epsilon,void* extra)
{
 double x=x1;
 double Fx=F(x,extra)-tgt_y;
 int iter=0;
 while(absd(Fx)>epsilon && iter<20){
  Fx=F(x,extra)-tgt_y;
  double Fpx=dFdx(x,extra);
  x=x-Fx/Fpx;
  iter++;
 }
 return x;
}
//#######################################################################################//
double C(double Z)
{
 if(Z>0.0) return (1.0-cos(sqrt(Z)))/Z;
 else{
  if(Z<0.0)return (1.0-cosh(sqrt(-Z)))/Z;
      else return 1.0/2.0;
 }
}
//#######################################################################################//
double S(double Z)
{
 if(Z>0.0) return (sqrt(Z)-sin(sqrt(Z)))/pow(Z,3.0/2.0);
 else{
  if(Z<0.0)return (sinh(sqrt(-Z))-sqrt(-Z))/pow(-Z,3.0/2.0);
      else return 1.0/6.0;
 }
}
//#######################################################################################//
double F(double X,double Alpha,double r0l){return 1.0-X*X*C(X*X*Alpha)/r0l;}
double GG(double X,double Alpha,double T){return T-X*X*X*S(X*X*Alpha);}
double FDot(double X,double Alpha,double R, double R0){double Z=X*X*Alpha;return X*(Z*(S(Z))-1)/(R0*R);}
double GDot(double X,double Alpha,double R){double Z=X*X*Alpha;return 1-X*X*(C(Z))/R;}
//#######################################################################################//
typedef struct{
 double Alpha;
 double R0dotV0;
 double r0l;
}KeplerExtra;
//#######################################################################################//
double KeplerF(double X,void* extra)
{
 KeplerExtra* KE=(KeplerExtra*)extra;
 double Z=X*X*KE->Alpha;
 return KE->R0dotV0*X*X*(C(Z))+(1.0-KE->r0l*KE->Alpha)*X*X*X*(S(Z))+KE->r0l*X;
}
//#######################################################################################//
double KeplerdFdx(double X,void* extra)
{
 KeplerExtra* KE=(KeplerExtra*)extra;
 double Z=X*X*KE->Alpha;
 return X*X*(C(Z))+KE->R0dotV0*X*(1-Z*(S(Z)))+KE->r0l*(1.0-Z*(C(Z)));
}
//#######################################################################################//
void KeplerCU(VECTOR3 &R0, VECTOR3 &V0, double T, VECTOR3 &R, VECTOR3 &V)
{
 //Algorithm for solving the Kepler problem: Given Mu, and state at some time
 //calculate state at some other time
 if(absd(T)<1e-7){
  R=R0;
  V=V0;
 }else{
  double r0l=modv(R0);
  double v0l=modv(V0);
  double R0dotV0=smulv(R0,V0);
  double Energy=v0l*v0l/2.0-1.0/r0l;
  double Alpha=-(2.0*Energy);
  double Xp1;
  if(Alpha<0){
   //hyperbolic
   double AA=(T>=0?1:-1)*sqrt(-1.0/Alpha);
   double BB=-2*T;
   double CC=R0dotV0;
   double DD=(T>=0?1:-1)*sqrt(-1.0/Alpha)*(1.0-r0l*Alpha);
   double EE=(CC+DD)/Alpha;
   double FF=log(BB/EE);
   Xp1=AA*FF;
  }else{
   //elliptical
   Xp1=T*Alpha;
  }
  //Iterate by newton to fine tune
  double X=Xp1;
  double Y=Xp1+1;
  KeplerExtra KE;
  KE.Alpha=Alpha;
  KE.R0dotV0=R0dotV0;
  KE.r0l=r0l;
  X=newton(KeplerF,KeplerdFdx,T,Xp1,1e-10,&KE);
  R=R0*F(X, Alpha, r0l)+V0*GG(X, Alpha, T);
  double rl=modv(R);
  V=R0*FDot(X, Alpha, rl, r0l)+V0*GDot(X, Alpha, rl);
 }
}
//#######################################################################################//
void KeplerFG(VECTOR3 &r,VECTOR3 &v,double T,double Mu,VECTOR3 &rOut,VECTOR3 &vOut)
{
 double DistY=6371010;
 double TimeX=sqrt(pow(DistY,3)/Mu);
 T=T/TimeX;
 VECTOR3 rCU=r*(1/DistY);
 VECTOR3 vCU=v*(TimeX/DistY);
 VECTOR3 rOutCU,vOutCU;
 KeplerCU(rCU,vCU,T,rOutCU,vOutCU);
 rOut=rOutCU*DistY;
 vOut=vOutCU*(DistY/TimeX);
}
//############################################################################//
#define max(a,b) (((a) > (b)) ? (a) : (b))
//############################################################################//
void unprjxy(int wi,int he,MATRIX3 rotm,VECTOR3 tv,double apr,int &x,int &y)
{
 double d,a,tan_pt;

 tv=rvmat(rotm,nrvec(tv));
 x=0;
 y=0;

 if(tv.z>0.0){
  tan_pt=sqrt(sqr(tv.x)+sqr(tv.y))/tv.z;
  d=(tan_pt/tan(apr))*(he/2);
  a=getrtang(tv.x,tv.y,0,0)/180*PI;
  x=(int)((wi/2)+d*sin(a));
  y=(int)((he/2)+d*cos(a));
 }
}
//############################################################################//
double getob_apr(VECTOR3 v,double rad,VECTOR3 c,double apr,int hei)
{
 double alt=max(1.0,modv(v-c)-rad);
 return rad*hei*0.5/(alt*tan(apr));
}
//############################################################################//
double line2sph(VECTOR3 v1,VECTOR3 v2,VECTOR3 v3,double r,VECTOR3 &px)
{
 double a,b,c,u1,u2,result;
 VECTOR3 p1,p2;

 a=sqr(v2.x-v1.x)+sqr(v2.y-v1.y)+sqr(v2.z-v1.z);
 b=2*((v2.x-v1.x)*(v1.x-v3.x)+(v2.y-v1.y)*(v1.y-v3.y)+(v2.z-v1.z)*(v1.z-v3.z));
 c=sqr(v3.x)+sqr(v3.y)+sqr(v3.z)+sqr(v1.x)+sqr(v1.y)+sqr(v1.z)-2*(v3.x*v1.x+v3.y*v1.y+v3.z*v1.z)-sqr(r);
 result=b*b-4*a*c;
 if(result>=0){
  u1=(-b+sqrt(result))/(2*a); u2=(-b-sqrt(result))/(2*a);
  p1.x=v1.x+u1*(v2.x-v1.x);
  p1.y=v1.y+u1*(v2.y-v1.y);
  p1.z=v1.z+u1*(v2.z-v1.z);
  p2.x=v1.x+u2*(v2.x-v1.x);
  p2.y=v1.y+u2*(v2.y-v1.y);
  p2.z=v1.z+u2*(v2.z-v1.z);
  px=p1+((p2-p1)*0.5);
 }
 return result;
}
//############################################################################//
