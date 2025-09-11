local os = require"os"
-- local iinfo = dofile"init"
local hroot=[[V:\pipeline\plugins\houdini\htoa]]
local appVersion=pfg["houdini_"]
local pluginVersion=pfg["houdini"]["htoa"]

local pluginRoot=string.format("%s\\htoa-%s_houdini-%s",hroot,pluginVersion,appVersion)
local pluginHID=cc.hashcs(pluginRoot)

local docRoot=os.getenv("USERPROFILE").."\\Documents"
local pluginLRoot=docRoot.."\\chst\\"..pluginHID

local envm = cc.readFileKV"env"
cc.appendKeyPath(envm,"HOUDINI_PACKAGE_DIR",string.gsub(pluginLRoot,"\\","/"),"&")
cc.writeFileKV("env",envm)
cc.copydir(pluginRoot,pluginLRoot)
-- cc.appendFile("envstack","HOUDINI_PACKAGE_PATH\n"..pluginLRoot.."\n")
cc.appendFile("message","htoa end\n")