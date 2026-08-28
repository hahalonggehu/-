
// ������
// ������Ϊ��Ӿ�����д��
// ��Ȩ���ޣ����������κη�ʽʹ�ô���


#include <stdio.h>
#include <time.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include "jymain.h"
#include "sdlfun.h"
#include "charset.h"
#include "mainmap.h"

// ȫ�̱���
SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;
SDL_Texture* g_Texture = NULL;
SDL_Texture* g_TextureShow = NULL;
SDL_Texture* g_TextureTmp = NULL;

SDL_Surface* g_Surface = NULL;          // ��Ϸʹ�õ���Ƶ����
Uint32 g_MaskColor32 = 0xff706020;      // ͸��ɫ

int g_Rotate = 0;                       //��Ļ�Ƿ���ת
int g_ScreenW = 800;                    // ��Ļ����
int g_ScreenH = 600;
int g_ScreenBpp = 16;                   // ��Ļɫ��
int g_FullScreen = 0;
int g_EnableSound = 1;                  // �������� 0 �ر� 1 ��
int g_MusicVolume = 32;                 // ����������С
int g_SoundVolume = 32;                 // ��Ч������С

int g_XScale = 18;                      //��ͼx,y����һ���С
int g_YScale = 9;

//������ͼ����ʱxy������Ҫ����Ƶ���������֤����ȫ����ʾ
int g_MMapAddX;
int g_MMapAddY;
int g_SMapAddX;
int g_SMapAddY;
int g_WMapAddX;
int g_WMapAddY;

int g_MAXCacheNum = 1000;               //��󻺴�����
int g_LoadFullS = 1;                    //�Ƿ�ȫ������S�ļ�
int g_LoadMMapType = 0;                 //�Ƿ�ȫ������M�ļ�
int g_LoadMMapScope = 0;
int g_PreLoadPicGrp = 1;                //�Ƿ�Ԥ�ȼ�����ͼ�ļ���grp
int IsDebug = 0;                        //�Ƿ�򿪸����ļ�
char JYMain_Lua[255];                   //lua������
int g_MP3 = 0;                          //�Ƿ��MP3
char g_MidSF2[255];                     //��ɫ���Ӧ���ļ�
float g_Zoom = 1;                       //ͼƬ�Ŵ�

#ifdef __SWITCH__
char* JY_CurrentPath = "sdmc:/switch/jysdl/";
#elif defined(_WIN32)
char* JY_CurrentPath = "./";
#else
char* JY_CurrentPath = "/sdcard/JYLDCR/";
#endif

lua_State* pL_main = NULL;

void* g_Tinypot;

//�����lua�ӿں�����
const struct luaL_Reg jylib[] =
{
    {"Debug", HAPI_Debug},

    {"GetKey", HAPI_GetKey},
    {"GetKeyState", HAPI_GetKeyState},
    {"EnableKeyRepeat", HAPI_EnableKeyRepeat},

    {"Delay", HAPI_Delay},
    {"GetTime", HAPI_GetTime},

    {"CharSet", HAPI_CharSet},
    {"DrawStr", HAPI_DrawStr},
	{"GetDrawStrSize", HAPI_GetDrawStrSize},

    {"SetClip", HAPI_SetClip},
    {"FillColor", HAPI_FillColor},
    {"Background", HAPI_Background},
    {"DrawRect", HAPI_DrawRect},

    {"ShowSurface", HAPI_ShowSurface},
    {"ShowSlow", HAPI_ShowSlow},

    {"PicInit", HAPI_PicInit},
    {"PicGetXY", HAPI_GetPicXY},
    {"PicLoadCache", HAPI_LoadPic},
    {"PicLoadFile", HAPI_PicLoadFile},

    {"FullScreen", HAPI_FullScreen},

    {"LoadPicture", HAPI_LoadPicture},

    {"PlayMIDI", HAPI_PlayMIDI},
    {"PlayWAV", HAPI_PlayWAV},
    {"PlayMPEG", HAPI_PlayMPEG},

    {"LoadMMap", HAPI_LoadMMap},
    {"DrawMMap", HAPI_DrawMMap},
    {"GetMMap", HAPI_GetMMap},
    {"UnloadMMap", HAPI_UnloadMMap},

    {"LoadSMap", HAPI_LoadSMap},
    {"SaveSMap", HAPI_SaveSMap},
    {"GetS", HAPI_GetS},
    {"SetS", HAPI_SetS},
    {"GetD", HAPI_GetD},
    {"SetD", HAPI_SetD},
    {"DrawSMap", HAPI_DrawSMap},

    {"LoadWarMap", HAPI_LoadWarMap},
    {"GetWarMap", HAPI_GetWarMap},
    {"SetWarMap", HAPI_SetWarMap},
    {"CleanWarMap", HAPI_CleanWarMap},

    {"DrawWarMap", HAPI_DrawWarMap},
    {"SaveSur", HAPI_SaveSur},
    {"LoadSur", HAPI_LoadSur},
    {"FreeSur", HAPI_FreeSur},
    {"GetScreenW", HAPI_ScreenWidth},
    {"GetScreenH", HAPI_ScreenHeight},
    {"LoadPNGPath", HAPI_LoadPNGPath},
    {"LoadPNG", HAPI_LoadPNG},
    {"GetPNGXY", HAPI_GetPNGXY},
    {NULL, NULL}
};



const struct luaL_Reg bytelib[] =
{
    {"create", Byte_create},
    {"loadfile", Byte_loadfile},
    {"loadfilefromzip", Byte_loadfilefromzip},
    {"savefile", Byte_savefile},
    {"unzip", Byte_unzip},
    {"zip", Byte_zip},
    {"get16", Byte_get16},
    {"set16", Byte_set16},
    {"getu16", Byte_getu16},
    {"setu16", Byte_setu16},
    {"get32", Byte_get32},
    {"set32", Byte_set32},
    {"getstr", Byte_getstr},
    {"setstr", Byte_setstr},
    {NULL, NULL}
};

static const struct luaL_Reg configLib[] = {

    { "GetPath", Config_GetPath },
    { NULL, NULL }
};

void GetModes(int *width, int *height)
{
    char buf[10];
    FILE *fp = fopen(_("resolution.txt"), "r");

    if (!fp) {
        JY_Error("GetModes: cannot open resolution.txt");
        return;
    }

    //��
    memset(buf, 0, 10);
    fgets(buf, 10, fp);
    *width = atoi(buf);

    //��
    memset(buf, 0, 10);
    fgets(buf, 10, fp);
    *height = atoi(buf);

    JY_Debug("GetModes: width=%d, height=%d", *width, *height);

    fclose(fp);
}

// ������
int main(int argc, char* argv[])
{
    //lua_State* pL_main;

#ifdef __SWITCH__
    fsdevMountSdmc();
#endif

    remove(DEBUG_FILE);
    remove(ERROR_FILE);    //����stderr������ļ�

    pL_main = luaL_newstate();
    luaL_openlibs(pL_main);

    JY_Debug("Lua_Config();");
    Lua_Config(pL_main, _(CONFIG_FILE));        //��ȡlua�����ļ������ò���


    JY_Debug("InitSDL();");
    InitSDL();           //��ʼ��SDL

    JY_Debug("InitGame();");
    InitGame();          //��ʼ����Ϸ����

    JY_Debug("LoadMB();");
    LoadMB(_(HZMB_FILE));  //���غ����ַ���ת�����

    JY_Debug("Lua_Main();");
    Lua_Main(pL_main);          //����Lua����������ʼ��Ϸ

    JY_Debug("ExitGame();");
    ExitGame();       //�ͷ���Ϸ����

    JY_Debug("ExitSDL();");
    ExitSDL();        //�˳�SDL

    JY_Debug("main() end;");

    //�ر�lua
    lua_close(pL_main);

    return 0;
}


//Lua������
int Lua_Main(lua_State* pL_main)
{
    int result = 0;

    //��ʼ��lua

    //ע��lua����
    lua_newtable(pL_main);
    luaL_setfuncs(pL_main, jylib, 0);
    lua_pushvalue(pL_main, -1);
    lua_setglobal(pL_main, "lib");

    lua_newtable(pL_main);
    luaL_setfuncs(pL_main, bytelib, 1);
    lua_pushvalue(pL_main, -1);
    lua_setglobal(pL_main, "Byte");

    //����lua�ļ�
    result = luaL_loadfile(pL_main, JYMain_Lua);
    switch (result)
    {
    case LUA_ERRSYNTAX:
        JY_Error("load lua file %s error: syntax error!\n", JYMain_Lua);
        break;
    case LUA_ERRMEM:
        JY_Error("load lua file %s error: memory allocation error!\n", JYMain_Lua);
        break;
    case LUA_ERRFILE:
        JY_Error("load lua file %s error: can not open file!\n", JYMain_Lua);
        break;
    }

    result = lua_pcall(pL_main, 0, LUA_MULTRET, 0);

    //����lua��������JY_Main
    lua_getglobal(pL_main, "JY_Main");
    result = lua_pcall(pL_main, 0, 0, 0);

    return 0;
}


//Lua��ȡ������Ϣ
int Lua_Config(lua_State* pL, const char* filename)
{
    int result = 0;

    //����lua�����ļ�
    result = luaL_loadfile(pL, filename);
    switch (result)
    {
    case LUA_ERRSYNTAX:
        fprintf(stderr, "load lua file %s error: syntax error!\n", filename);
        break;
    case LUA_ERRMEM:
        fprintf(stderr, "load lua file %s error: memory allocation error!\n", filename);
        break;
    case LUA_ERRFILE:
        fprintf(stderr, "load lua file %s error: can not open file!\n", filename);
        break;
    }

    result = lua_pcall(pL, 0, LUA_MULTRET, 0);

    lua_getglobal(pL, "CONFIG");            //��ȡconfig�����ֵ
    if (getfield(pL, "Width") != 0)
    {
        g_ScreenW = getfield(pL, "Width");
    }
    if (getfield(pL, "Height") != 0)
    {
        g_ScreenH = getfield(pL, "Height");
    }
    g_ScreenBpp = getfield(pL, "bpp");
    g_FullScreen = getfield(pL, "FullScreen");
    g_XScale = getfield(pL, "XScale");
    g_YScale = getfield(pL, "YScale");
    g_XScale = 18;
    g_YScale = 9;
    g_EnableSound = getfield(pL, "EnableSound");
    IsDebug = getfield(pL, "Debug");
    g_MMapAddX = getfield(pL, "MMapAddX");
    g_MMapAddY = getfield(pL, "MMapAddY");
    g_SMapAddX = getfield(pL, "SMapAddX");
    g_SMapAddY = getfield(pL, "SMapAddY");
    g_WMapAddX = getfield(pL, "WMapAddX");
    g_WMapAddY = getfield(pL, "WMapAddY");
    g_SoundVolume = getfield(pL, "SoundVolume");
    g_MusicVolume = getfield(pL, "MusicVolume");

    g_MAXCacheNum = getfield(pL, "MAXCacheNum");
    g_LoadFullS = getfield(pL, "LoadFullS");
    g_MP3 = getfield(pL, "MP3");

    g_Zoom = (float)(getfield(pL, "Zoom") / 100.0);

    getfieldstr(pL, "MidSF2", g_MidSF2);

    getfieldstr(pL, "JYMain_Lua", JYMain_Lua);

    return 0;
}

//��ȡlua���е�����
int getfield(lua_State* pL, const char* key)
{
    int result;
    lua_getfield(pL, -1, key);
    result = (int)lua_tonumber(pL, -1);
    lua_pop(pL, 1);
    return result;
}

//��ȡlua���е��ַ���
int getfieldstr(lua_State* pL, const char* key, char* str)
{
    const char* tmp;
    lua_getfield(pL, -1, key);
    tmp = (const char*)lua_tostring(pL, -1);
    if (tmp) { strcpy(str, tmp); }
    lua_pop(pL, 1);
    return 0;
}

//����Ϊ����ͨ�ú���

// ���Ժ���
// �����debug.txt��
int JY_Debug(const char* fmt, ...)
{
    time_t t;
    FILE* fp;
    struct tm* newtime;
    va_list argptr;
#ifndef _DEBUG
    if (IsDebug == 0)
    {
        return 0;
    }
#endif
    char string[1024];
    // concatenate all the arguments in one string
    va_start(argptr, fmt);
    vsnprintf(string, sizeof(string), fmt, argptr);
    va_end(argptr);
    time(&t);
    newtime = localtime(&t);
#ifdef _DEBUG
    fprintf(stderr, "%02d:%02d:%02d %s\n", newtime->tm_hour, newtime->tm_min, newtime->tm_sec, string);
#else
    fp = fopen(DEBUG_FILE, "a+t");
    fprintf(fp, "%02d:%02d:%02d %s\n", newtime->tm_hour, newtime->tm_min, newtime->tm_sec, string);
    fclose(fp);
#endif
    return 0;
}

// ���Ժ���
// �����error.txt��
int JY_Error(const char* fmt, ...)
{
    //�޾Ʋ������������error��Ϣ
#ifdef _DEBUG
    time_t t;
    FILE* fp;
    struct tm* newtime;

    va_list argptr;
    char string[1024];

    va_start(argptr, fmt);
    vsnprintf(string, sizeof(string), fmt, argptr);
    va_end(argptr);
    //fp=fopen(ERROR_FILE,"a+t");
    time(&t);
    newtime = localtime(&t);
    fprintf(stderr, "%02d:%02d:%02d %s\n", newtime->tm_hour, newtime->tm_min, newtime->tm_sec, string);
    //fflush(fp);
#endif
    return 0;
}

// ����x��С
int limitX(int x, int xmin, int xmax)
{
    if (x > xmax)
    {
        x = xmax;
    }
    if (x < xmin)
    {
        x = xmin;
    }
    return x;
}

// �����ļ����ȣ���Ϊ0�����ļ����ܲ�����
int FileLength(const char* filename)
{
    FILE*   f;
    int ll;
    if ((f = fopen(filename, "rb")) == NULL)
    {
        return 0;            // �ļ������ڣ�����
    }
    fseek(f, 0, SEEK_END);
    ll = ftell(f);    //����õ���len�����ļ��ĳ�����
    fclose(f);
    return ll;
}

char* va(const char* format, ...)
{
    static char string[256];
    va_list     argptr;

    va_start(argptr, format);
    vsnprintf(string, 256, format, argptr);
    va_end(argptr);

    return string;
}
