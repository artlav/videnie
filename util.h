//#######################################################################################//
// Orbiter and generic util H
// Made in 2007-2010 by Artlav
//#######################################################################################//
#ifndef _autil_H
#define _autil_H
//#######################################################################################//
#ifndef USE_ORBITER
//#######################################################################################//
const double PI   =3.14159265358979;
const double PI05 =1.57079632679490;
const double RAD  =PI/180.0;
const double GGRAV=6.67259e-11;
//#######################################################################################//
typedef struct{
 double x,y,z;
}VECTOR3;
typedef union{
 double data[9];
 struct{double m11,m12,m13,m21,m22,m23,m31,m32,m33;};
}MATRIX3;
typedef struct{
 float r,g,b,a;
} COLOUR4;
//#######################################################################################//
inline VECTOR3 operator+ (const VECTOR3 &a, const VECTOR3 &b){VECTOR3 c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;return c;}
inline VECTOR3 operator- (const VECTOR3 &a, const VECTOR3 &b){VECTOR3 c;c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;return c;}
inline VECTOR3 operator* (const VECTOR3 &a, const double f){VECTOR3 c;c.x=a.x*f;c.y=a.y*f;c.z=a.z*f;return c;}
inline VECTOR3 operator/ (const VECTOR3 &a,const double f){VECTOR3 c;c.x=a.x/f;c.y=a.y/f;c.z=a.z/f;return c;}
inline VECTOR3 &operator+= (VECTOR3 &a,const VECTOR3 &b){a.x+=b.x;a.y+=b.y;a.z+=b.z;return a;}
inline VECTOR3 &operator-= (VECTOR3 &a,const VECTOR3 &b){a.x-=b.x;a.y-=b.y;a.z-=b.z;return a;}
inline VECTOR3 &operator*= (VECTOR3 &a,const double f){a.x*=f;a.y*=f;a.z*=f;return a;}
inline VECTOR3 &operator/= (VECTOR3 &a,const double f){a.x/=f;a.y/=f;a.z/=f;return a;}
inline VECTOR3 operator- (const VECTOR3 &a){VECTOR3 c;c.x=-a.x;c.y=-a.y;c.z=-a.z;return c;}
//#######################################################################################//
#else
//#######################################################################################//
#define LINE 16
inline void print (HDC hDC,int i,int *j,LPCTSTR cad){TextOut(hDC,i,*j+=LINE,cad,sizeof(char)*strlen(cad));}
inline void printa(HDC hDC,int i,int *j,LPCTSTR cad){TextOut(hDC,i,*j+=10,cad,sizeof(char)*strlen(cad));}
inline void print (HDC hDC,int i,int  j,LPCTSTR cad){TextOut(hDC,i, j*LINE, cad,sizeof(char)*strlen(cad));}
inline void sprintDistance(char *s,double dist,bool isAU)
{
 if(dist>=1000000000L){
  dist/=1000000000L;
  if(isAU)sprintf(s,"%.2f Gm(%.3f AU)",dist,dist/150);
     else sprintf(s,"%.2f Gm",dist);
  return;
 }
 if(dist>=1000000L){
  dist/=1000000L;
  if(isAU)sprintf(s,"%.2f Mm(%.3f AU)",dist,dist/150000L);
     else sprintf(s,"%.2f Mm",dist);
  return;
 }
 if(dist>=1000L){
  dist/=1000L;
  if(isAU)sprintf(s,"%.2f Km(%.3f AU)",dist,dist/150000000L);
     else sprintf(s,"%.2f Km",dist);
  return;
 }
 sprintf(s,"%.1f m",dist);
}
//#######################################################################################//
#endif
//#######################################################################################//
//#######################################################################################//
//#######################################################################################//
inline double absd(double a){if(a<0)return -a; else return a;}
inline double sqr (double a){return a*a;}
inline double cub (double a){return a*a*a;}
inline double sgn(double a){if(a>=0)return 1; else return -1;}
inline double sacos(double a){return acos(a);}
inline double sasin(double a){return asin(a);}
inline double sqsrt(double a){return sqrt(a);}
inline double satan(double a){return atan(a);}
inline double satan2(double a,double b){return atan2(a,b);}
inline double slog(double a){return log(a);}
#ifndef WIN32
inline double max(double a,double b){if(a>=b)return a; else return b;}
inline int max(int a,int b){if(a>=b)return a; else return b;}
#endif
inline int sgn(int a){if(a>=0)return 1; else return -1;}
inline bool inrect(double x,double y,double xh,double yh,double xl,double yl){return ((x>=xh)&&(x<=xl)&&(y>=yh)&&(y<=yl));}
//#######################################################################################//
inline COLOUR4 _C4(float r,float g,float b,float a){COLOUR4 col={r,g,b,a}; return col;}
//#######################################################################################//
inline VECTOR3 tvec(double x,double y,double z)
{
 VECTOR3 vec={x,y,z};
 return vec;
}

inline MATRIX3 tmat(double m11, double m12, double m13,double m21, double m22, double m23,double m31, double m32, double m33)
{
 MATRIX3 mat={m11,m12,m13,  m21,m22,m23,  m31,m32,m33};
 return mat;
}

inline VECTOR3 lvmat(const MATRIX3 &A,const VECTOR3 &b)
{
 return tvec(
  A.m11*b.x + A.m12*b.y + A.m13*b.z,
  A.m21*b.x + A.m22*b.y + A.m23*b.z,
  A.m31*b.x + A.m32*b.y + A.m33*b.z);
}

inline VECTOR3 rvmat(const MATRIX3 &A, const VECTOR3 &b)
{
 return tvec(
  A.m11*b.x + A.m21*b.y + A.m31*b.z,
  A.m12*b.x + A.m22*b.y + A.m32*b.z,
  A.m13*b.x + A.m23*b.y + A.m33*b.z);
}

inline MATRIX3 mulm(const MATRIX3 &A, const MATRIX3 &B)
{
 MATRIX3 mat={
  A.m11*B.m11 + A.m12*B.m21 + A.m13*B.m31, A.m11*B.m12 + A.m12*B.m22 + A.m13*B.m32, A.m11*B.m13 + A.m12*B.m23 + A.m13*B.m33,
  A.m21*B.m11 + A.m22*B.m21 + A.m23*B.m31, A.m21*B.m12 + A.m22*B.m22 + A.m23*B.m32, A.m21*B.m13 + A.m22*B.m23 + A.m23*B.m33,
  A.m31*B.m11 + A.m32*B.m21 + A.m33*B.m31, A.m31*B.m12 + A.m32*B.m22 + A.m33*B.m32, A.m31*B.m13 + A.m32*B.m23 + A.m33*B.m33
 };
 return mat;
}

inline MATRIX3 transpm(const MATRIX3 &A)
{
 MATRIX3 mat={
  A.m11,A.m21,A.m31,
  A.m12,A.m22,A.m32,
  A.m13,A.m23,A.m33
 };
 return mat;
}
//#######################################################################################//
bool operator !=(VECTOR3 a,VECTOR3 b);
VECTOR3 operator -(VECTOR3 &a,VECTOR3 &b);
VECTOR3 operator +(VECTOR3 &a,VECTOR3 &b);
VECTOR3 operator /(VECTOR3 a,double b);
VECTOR3 operator *(VECTOR3 &a,double &b);

VECTOR3 vmulv(VECTOR3 v1,VECTOR3 v2);
double smulv(VECTOR3 v1,VECTOR3 v2);
VECTOR3 dmulv(VECTOR3 v1,VECTOR3 v2);

VECTOR3 vrotz(VECTOR3 v,double e);
VECTOR3 vroty(VECTOR3 v,double e);
VECTOR3 vrotx(VECTOR3 v,double e);
double  modv(VECTOR3 v);
double  modvs(VECTOR3 v);
VECTOR3 nrvec(VECTOR3 iv);
//#######################################################################################//
VECTOR3 vrec2sphv(VECTOR3 v);
double spherical_distance(VECTOR3 a,VECTOR3 b,double rad);
//#######################################################################################//
MATRIX3 epsmat(MATRIX3 a);
VECTOR3 tamat(MATRIX3 a);
MATRIX3 atmat(VECTOR3 a);
double getrtang(double pmx,double pmy,double wx,double wy);
MATRIX3 vecs2mat_orb(VECTOR3 f,VECTOR3 up);
//#######################################################################################//
double linterp(double x1,double y1,double x2,double y2,double x);
double calc_kd(double t,double i,double kp);
VECTOR3 get_pitchyawroll(VECTOR3 py,VECTOR3 r);
void Crt2Pol(VECTOR3 &pos,VECTOR3 &vel);
void KeplerFG(VECTOR3 &r,VECTOR3 &v,double T,double Mu,VECTOR3 &rOut,VECTOR3 &vOut);
//############################################################################//
void unprjxy(int wi,int he,MATRIX3 rotm,VECTOR3 tv,double apr,int &x,int &y);
double getob_apr(VECTOR3 v,double rad,VECTOR3 c,double apr,int hei);
double line2sph(VECTOR3 v1,VECTOR3 v2,VECTOR3 v3,double r,VECTOR3 &px);
//#######################################################################################//
#endif // !_autil_H
//#######################################################################################//
