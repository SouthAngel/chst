local os = require"os"
local io = require"io"
-- local iinfo = dofile"init"
local hroot=[[V:\pipeline\plugins\maya\mtoa]]
local appVersion=pfg["maya_"]
local pluginVersion=pfg["maya"]["mtoa"]

local pluginRoot=string.format("%s\\Mtoa-%s-%s",hroot,pluginVersion,appVersion)
if(not cc.isdir(pluginRoot))then cc.appendFile("error","Not found mtoa in "..pluginRoot) end
local pluginHID=cc.hashcs(pluginRoot)

local docRoot=os.getenv("USERPROFILE").."\\Documents"
local pluginLRoot=docRoot.."\\chst\\"..pluginHID

cc.copydir(pluginRoot,pluginLRoot)
local mmd=cc.readFileLines(pluginLRoot.."\\mtoa.mod")
mmd[1] = string.sub(mmd[1],1,11)..string.gsub(pluginLRoot,"\\","/").."/maya"..appVersion
cc.writeFileLines("mtoa.mod",mmd)
cc.copyfile("mtoa.mod",docRoot.."\\maya\\"..appVersion.."\\modules\\mtoa.mod")