#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <Windows.h>
#include "lua.hpp"

namespace lfn{
static int startappBa(lua_State *L){
    ::STARTUPINFOW sinfo{};
    sinfo.cb=sizeof(sinfo);
    ::PROCESS_INFORMATION pinfo{};
    WCHAR cmda[] = L"cmd.exe /C startapp.bat";
    ::CreateProcessW(0,cmda,0,0,false,CREATE_NEW_CONSOLE,0,0,&sinfo,&pinfo);
    return 0;
}
static int rdid(lua_State *L){
    void *ar = &L;
    std::stringstream ss;
    ss << ar;
    lua_pushstring(L, ss.str().c_str());
    return 1;
}

static int copyfile(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    const char *br = lua_tolstring(L,2,0);
    static std::error_code ec;
    auto ppath=std::filesystem::path(br).parent_path();
    if (!std::filesystem::is_directory(ppath)){
        std::filesystem::create_directories(ppath,ec);
    }
    int tt=std::filesystem::copy_file(ar,br,std::filesystem::copy_options::overwrite_existing,ec)?1:2;
    lua_pushnumber(L,tt);
    return 1;
}

static int copydir(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    const char *br = lua_tolstring(L,2,0);
    if (std::filesystem::is_directory(br)){
        lua_pushnumber(L,0);
    }else{
        static std::error_code ec;
        auto ppath=std::filesystem::path(br).parent_path();
        if (!std::filesystem::is_directory(ppath)){
            std::filesystem::create_directories(ppath,ec);
        }
        std::filesystem::copy(ar,br,std::filesystem::copy_options::recursive,ec);
        lua_pushnumber(L,ec.value()==0?1:2);
    }
    return 1;
}

static int isfile(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    lua_pushboolean(L,std::filesystem::is_regular_file(ar));
    return 1;
}

static int isdir(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    lua_pushboolean(L,std::filesystem::is_directory(ar));
    return 1;
}

static int hashcs(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    std::size_t h1 = std::hash<std::string>{}(ar);
    std::stringstream ss;
    ss << h1;
    lua_pushstring(L, ss.str().c_str());
    return 1;
}

static int testa(lua_State *L){
    const char *ar = lua_tolstring(L,1,0);
    std::size_t h1 = std::hash<std::string>{}(ar);
    std::cout <<ar<<" h "<<h1<<std::endl;
    return 0;
}
}
static const struct luaL_Reg cc[]={
    {"testa",lfn::testa},
    {"hashcs",lfn::hashcs},
    {"rdid",lfn::rdid},
    {"isfile",lfn::isfile},
    {"isdir",lfn::isdir},
    {"copyfile",lfn::copyfile},
    {"copydir",lfn::copydir},
    {"startappBa",lfn::startappBa},
    {0,0},
};

int main(int, char**){
    const std::string root = [](){ auto tmpc = std::getenv("CHST_ROOT"); std::string tmp(tmpc?tmpc:"."); return tmp;}();
    auto L = luaL_newstate();
    int retL=-1;
    luaL_openlibs(L);
    lua_newtable(L);
    for (const luaL_Reg &ap : cc)
    {
        if (ap.name==0)break;
        lua_pushstring(L,ap.name);
        lua_pushcclosure(L,ap.func,0);
        lua_settable(L,-3);
    }
    lua_pushstring(L,"root");
    lua_pushstring(L,root.c_str());
    lua_settable(L,-3);
    lua_setglobal(L,"cc");
    retL = luaL_dofile(L, (root+"/setup/cut").c_str());
    auto printTrack=[&L](){
        luaL_traceback(L,L,0,1);
        auto tsz = lua_gettop(L);
        for (int i = 0; i < tsz; i++)
        {
            std::cerr<<lua_tostring(L,tsz-i)<<std::endl;;
        }
    };
    if (retL!=0){
        std::cout<<"load core util fail\n";
        printTrack();
    }
    retL = luaL_dofile(L, (root+"/setup/setup").c_str());
    if (retL!=0){
        std::cout<<"dofile setup fail\n";
        printTrack();
    }
    lua_close(L);
    return 0;
}
