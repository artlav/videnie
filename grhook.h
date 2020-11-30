//############################################################################//
void (__stdcall *goint)(DWORD,DWORD,DWORD,DWORD);
HMODULE grhlib=NULL;
//############################################################################//
int load_grhook(const char *name,DWORD ikeydown,DWORD ikeyup,DWORD imsevent,DWORD idraw)
{
 grhlib=LoadLibrary(name);
 if(grhlib){
  goint=(void(__stdcall *)(DWORD,DWORD,DWORD,DWORD))GetProcAddress(grhlib,"_goint@16");
  if(goint){
   goint(ikeydown,ikeyup,imsevent,idraw);
  }else{MessageBox(NULL,"Cannot initialize video link","Videnie: Error",MB_OK);FreeLibrary(grhlib);}
 }else MessageBox(NULL,"Cannot load grhook.dll","Videnie: Error",MB_OK);
 if(!grhlib)return 0;
 return 1;
}
//############################################################################//
