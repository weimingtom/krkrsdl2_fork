//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// "Plugins" class implementation / Service for plug-ins
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include <algorithm>
#include <functional>
#include "ScriptMgnIntf.h"
#include "PluginImpl.h"
#include "StorageImpl.h"
//#include "GraphicsLoaderImpl.h"

#include "MsgImpl.h"
#include "SysInitIntf.h"

#include "tjsHashSearch.h"
#include "EventIntf.h"
#include "TransIntf.h"
#include "tjsArray.h"
#include "tjsDictionary.h"
#include "DebugIntf.h"
#ifdef KRKRSDL2_ENABLE_PLUGINS
#include "FuncStubs.h"
#endif
#include "tjs.h"

#include "FilePathUtil.h"
#include "Application.h"
#include "SysInitImpl.h"
#include <set>


//---------------------------------------------------------------------------
// export table
//---------------------------------------------------------------------------
#ifdef KRKRSDL2_ENABLE_PLUGINS
static tTJSHashTable<ttstr, void *> TVPExportFuncs;
static bool TVPExportFuncsInit = false;
#endif
void TVPAddExportFunction(const char *name, void *ptr)
{
#ifdef KRKRSDL2_ENABLE_PLUGINS
	TVPExportFuncs.Add(name, ptr);
#endif
}
void TVPAddExportFunction(const tjs_char *name, void *ptr)
{
#ifdef KRKRSDL2_ENABLE_PLUGINS
	TVPExportFuncs.Add(name, ptr);
#endif
}
#ifdef KRKRSDL2_ENABLE_PLUGINS
static void TVPInitExportFuncs()
{
	if(TVPExportFuncsInit) return;
	TVPExportFuncsInit = true;


	// Export functions
	TVPExportFunctions();
}
//---------------------------------------------------------------------------
struct tTVPFunctionExporter : iTVPFunctionExporter
{
	bool TJS_INTF_METHOD QueryFunctions(const tjs_char **name, void **function,
		tjs_uint count);
	bool TJS_INTF_METHOD QueryFunctionsByNarrowString(const char **name,
		void **function, tjs_uint count);
} static TVPFunctionExporter;
//---------------------------------------------------------------------------
bool TJS_INTF_METHOD tTVPFunctionExporter::QueryFunctions(const tjs_char **name, void **function,
		tjs_uint count)
{
	// retrieve function table by given name table.
	// return false if any function is missing.
	bool ret = true;
	ttstr tname;
	for(tjs_uint i = 0; i<count; i++)
	{
		tname = name[i];
		void ** ptr = TVPExportFuncs.Find(tname);
		if(ptr)
			function[i] = *ptr;
		else
			function[i] = NULL, ret= false;
	}
	return ret;
}
//---------------------------------------------------------------------------
bool TJS_INTF_METHOD tTVPFunctionExporter::QueryFunctionsByNarrowString(
	const char **name, void **function, tjs_uint count)
{
	// retrieve function table by given name table.
	// return false if any function is missing.
	bool ret = true;
	ttstr tname;
	for(tjs_uint i = 0; i<count; i++)
	{
		tname = name[i];
		void ** ptr = TVPExportFuncs.Find(tname);
		if(ptr)
			function[i] = *ptr;
		else
			function[i] = NULL, ret= false;
	}
	return ret;
}
//---------------------------------------------------------------------------
extern "C" iTVPFunctionExporter * TVPGetFunctionExporter()
{
	// for external applications
	TVPInitExportFuncs();
    return &TVPFunctionExporter;
}
//---------------------------------------------------------------------------
#endif


//---------------------------------------------------------------------------
void TVPThrowPluginUnboundFunctionError(const char *funcname)
{
	TVPThrowExceptionMessage(TVPPluginUnboundFunctionError, funcname);
}
//---------------------------------------------------------------------------
void TVPThrowPluginUnboundFunctionError(const tjs_char *funcname)
{
	TVPThrowExceptionMessage(TVPPluginUnboundFunctionError, funcname);
}
//---------------------------------------------------------------------------



#ifdef KRKRSDL2_ENABLE_PLUGINS
//---------------------------------------------------------------------------
// Plug-ins management
//---------------------------------------------------------------------------
struct tTVPPlugin
{
	ttstr Name;
	void *Instance = nullptr;

	tTVPPluginHolder *Holder = nullptr;

	tTVPV2LinkProc V2Link = nullptr;
	tTVPV2UnlinkProc V2Unlink = nullptr;

	tTVPPlugin(const ttstr & name);
	~tTVPPlugin();

	bool Uninit();
};
//---------------------------------------------------------------------------
tTVPPlugin::tTVPPlugin(const ttstr & name) : Name(name)
{
	Holder = new tTVPPluginHolder(name);
	std::string filename;
	if( TVPUtf16ToUtf8( filename, Holder->GetLocalName().AsStdString() ) )
	{
		if( TVPCheckExistentLocalFile(Holder->GetLocalName()) ) {
			Instance = SDL_LoadObject(filename.c_str());
		}
	}
	if(!Instance)
	{
		delete Holder;
		TVPThrowExceptionMessage(TVPCannotLoadPlugin, name);
	}

	try
	{
		// retrieve each functions
		V2Link = (tTVPV2LinkProc)SDL_LoadFunction(Instance, "V2Link");

		V2Unlink = (tTVPV2UnlinkProc)SDL_LoadFunction(Instance, "V2Unlink");

		// link
		if(V2Link)
		{
			V2Link(TVPGetFunctionExporter());
		}
	}
	catch(...)
	{
		SDL_UnloadObject(Instance);
		delete Holder;
		throw;
	}
}
//---------------------------------------------------------------------------
tTVPPlugin::~tTVPPlugin()
{
}
//---------------------------------------------------------------------------
bool tTVPPlugin::Uninit()
{
	tTJS *tjs = TVPGetScriptEngine();
	if(tjs) tjs->DoGarbageCollection(); // to release unused objects

	if(V2Unlink)
	{
 		if(TJS_FAILED(V2Unlink())) return false;
	}

	SDL_UnloadObject(Instance);
	delete Holder;
	return true;
}
#endif
//---------------------------------------------------------------------------
#if 0
tjs_int TVPGetAutoLoadPluginCount() { return TVPAutoLoadPluginCount; }
#else
tjs_int TVPGetAutoLoadPluginCount() { return 0; }
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#ifdef KRKRSDL2_ENABLE_PLUGINS
bool TVPPluginUnloadedAtSystemExit = false;
typedef std::vector<tTVPPlugin*> tTVPPluginVectorType;
struct tTVPPluginVectorStruc
{
	tTVPPluginVectorType Vector;
} static TVPPluginVector;
static void TVPDestroyPluginVector(void)
{
	// state all plugins are to be released
	TVPPluginUnloadedAtSystemExit = true;

	// delete all objects
	tTVPPluginVectorType::iterator i;
	while(TVPPluginVector.Vector.size())
	{
		i = TVPPluginVector.Vector.end() - 1;
		try
		{
			(*i)->Uninit();
			delete *i;
		}
		catch(...)
		{
		}
		TVPPluginVector.Vector.pop_back();
	}
}
tTVPAtExit TVPDestroyPluginVectorAtExit
	(TVP_ATEXIT_PRI_RELEASE, TVPDestroyPluginVector);
#endif
//---------------------------------------------------------------------------
static bool TVPPluginLoading = false;
void TVPLoadPlugin(const ttstr & name)
{
#ifdef KRKRSDL2_ENABLE_PLUGINS
	// load plugin
	if(TVPPluginLoading)
		TVPThrowExceptionMessage(TVPCannnotLinkPluginWhilePluginLinking);
			// linking plugin while other plugin is linking, is prohibited
			// by data security reason.

	// check whether the same plugin was already loaded
	tTVPPluginVectorType::iterator i;
	for(i = TVPPluginVector.Vector.begin();
		i != TVPPluginVector.Vector.end(); i++)
	{
		if((*i)->Name == name) return;
	}

	tTVPPlugin * p;

	try
	{
		TVPPluginLoading = true;
		p = new tTVPPlugin(name);
		TVPPluginLoading = false;
	}
	catch(...)
	{
		TVPPluginLoading = false;
		throw;
	}

	TVPPluginVector.Vector.push_back(p);
#endif
}
//---------------------------------------------------------------------------
bool TVPUnloadPlugin(const ttstr & name)
{
	// unload plugin

#ifdef KRKRSDL2_ENABLE_PLUGINS
	tTVPPluginVectorType::iterator i;
	for(i = TVPPluginVector.Vector.begin();
		i != TVPPluginVector.Vector.end(); i++)
	{
		if((*i)->Name == name)
		{
			if(!(*i)->Uninit()) return false;
			delete *i;
			TVPPluginVector.Vector.erase(i);
			return true;
		}
	}
	TVPThrowExceptionMessage(TVPNotLoadedPlugin, name);
	return false;
#else
	return true;
#endif
}
//---------------------------------------------------------------------------





#if 0
//---------------------------------------------------------------------------
// plug-in autoload support
//---------------------------------------------------------------------------
struct tTVPFoundPlugin
{
	tjs_string Path;
	tjs_string Name;
	bool operator < (const tTVPFoundPlugin &rhs) const { return Name < rhs.Name; }
};
static tjs_int TVPAutoLoadPluginCount = 0;
static void TVPSearchPluginsAt(std::vector<tTVPFoundPlugin> &list, tjs_string folder)
{
	WIN32_FIND_DATA ffd;
	HANDLE handle = ::FindFirstFile((folder + TJS_W("*.tpm")).c_str(), &ffd);
	if(handle != INVALID_HANDLE_VALUE)
	{
		BOOL cont;
		do
		{
			if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				tTVPFoundPlugin fp;
				fp.Path = folder;
				fp.Name = ffd.cFileName;
				list.push_back(fp);
			}
			cont = FindNextFile(handle, &ffd);
		} while(cont);
		FindClose(handle);
	}
}
#endif

void TVPLoadPluigins(void)
{
#if 0
	// This function searches plugins which have an extension of ".tpm"
	// in the default path: 
	//    1. a folder which holds kirikiri executable
	//    2. "plugin" folder of it
	// Plugin load order is to be decided using its name;
	// aaa.tpm is to be loaded before aab.tpm (sorted by ASCII order)

	// search plugins from path: (exepath), (exepath)\system, (exepath)\plugin
	std::vector<tTVPFoundPlugin> list;

	tjs_string exepath = IncludeTrailingBackslash(ExtractFileDir(ExePath()));

	TVPSearchPluginsAt(list, exepath);
	TVPSearchPluginsAt(list, exepath + TJS_W("system\\"));
#ifdef TJS_64BIT_OS
	TVPSearchPluginsAt(list, exepath + TJS_W("plugin64\\"));
#else
	TVPSearchPluginsAt(list, exepath + TJS_W("plugin\\"));
#endif

	// sort by filename
	std::sort(list.begin(), list.end());

	// load each plugin
	TVPAutoLoadPluginCount = (tjs_int)list.size();
	for(std::vector<tTVPFoundPlugin>::iterator i = list.begin();
		i != list.end();
		i++)
	{
		TVPAddImportantLog(ttstr(TJS_W("(info) Loading ")) + ttstr(i->Name));
		TVPLoadPlugin(i->Path + i->Name);
	}
#endif
}





//---------------------------------------------------------------------------
// some service functions for plugin
//---------------------------------------------------------------------------
#include "zlib/zlib.h"
int ZLIB_uncompress(unsigned char *dest, unsigned long *destlen,
	const unsigned char *source, unsigned long sourcelen)
{
	return uncompress(dest, destlen, source, sourcelen);
}
//---------------------------------------------------------------------------
int ZLIB_compress(unsigned char *dest, unsigned long *destlen,
	const unsigned char *source, unsigned long sourcelen)
{
	return compress(dest, destlen, source, sourcelen);
}
//---------------------------------------------------------------------------
int ZLIB_compress2(unsigned char *dest, unsigned long *destlen,
	const unsigned char *source, unsigned long sourcelen, int level)
{
	return compress2(dest, destlen, source, sourcelen, level);
}
//---------------------------------------------------------------------------
#include "md5.h"
static char TVP_assert_md5_state_t_size[
	 (sizeof(TVP_md5_state_t) >= sizeof(md5_state_t))];
	// if this errors, sizeof(TVP_md5_state_t) is not equal to sizeof(md5_state_t).
	// sizeof(TVP_md5_state_t) must be equal to sizeof(md5_state_t).
//---------------------------------------------------------------------------
void TVP_md5_init(TVP_md5_state_t *pms)
{
	md5_init((md5_state_t*)pms);
}
//---------------------------------------------------------------------------
void TVP_md5_append(TVP_md5_state_t *pms, const tjs_uint8 *data, int nbytes)
{
	md5_append((md5_state_t*)pms, (const md5_byte_t*)data, nbytes);
}
//---------------------------------------------------------------------------
void TVP_md5_finish(TVP_md5_state_t *pms, tjs_uint8 *digest)
{
	md5_finish((md5_state_t*)pms, digest);
}
//---------------------------------------------------------------------------
#if 0
HWND TVPGetApplicationWindowHandle()
{
	return Application->GetHandle();
}
#endif
//---------------------------------------------------------------------------
void TVPProcessApplicationMessages()
{
#if 0
	Application->ProcessMessages();
#endif
}
//---------------------------------------------------------------------------
void TVPHandleApplicationMessage()
{
#if 0
	Application->HandleMessage();
#endif
}
//---------------------------------------------------------------------------
bool TVPRegisterGlobalObject(const tjs_char *name, iTJSDispatch2 * dsp)
{
	// register given object to global object
	tTJSVariant val(dsp);
	iTJSDispatch2 *global = TVPGetScriptDispatch();
	tjs_error er;
	try
	{
		er = global->PropSet(TJS_MEMBERENSURE, name, NULL, &val, global);
	}
	catch(...)
	{
		global->Release();
		return false;
	}
	global->Release();
	return TJS_SUCCEEDED(er);
}
//---------------------------------------------------------------------------
bool TVPRemoveGlobalObject(const tjs_char *name)
{
	// remove registration of global object
	iTJSDispatch2 *global = TVPGetScriptDispatch();
	if(!global) return false;
	tjs_error er;
	try
	{
		er = global->DeleteMember(0, name, NULL, global);
	}
	catch(...)
	{
		global->Release();
		return false;
	}
	global->Release();
	return TJS_SUCCEEDED(er);
}
//---------------------------------------------------------------------------
void TVPDoTryBlock(
	tTVPTryBlockFunction tryblock,
	tTVPCatchBlockFunction catchblock,
	tTVPFinallyBlockFunction finallyblock,
	void *data)
{
	try
	{
		tryblock(data);
	}
	catch(const eTJS & e)
	{
		if(finallyblock) finallyblock(data);
		tTVPExceptionDesc desc;
		desc.type = TJS_W("eTJS");
		desc.message = e.GetMessage();
		if(catchblock(data, desc)) throw;
		return;
	}
	catch(...)
	{
		if(finallyblock) finallyblock(data);
		tTVPExceptionDesc desc;
		desc.type = TJS_W("unknown");
		if(catchblock(data, desc)) throw;
		return;
	}
	if(finallyblock) finallyblock(data);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPGetFileVersionOf
//---------------------------------------------------------------------------
#if 0
bool TVPGetFileVersionOf(const tjs_char* module_filename, tjs_int &major, tjs_int &minor, tjs_int &release, tjs_int &build)
{
	// retrieve file version
	major = minor = release = build = 0;

	VS_FIXEDFILEINFO *FixedFileInfo;
	BYTE *VersionInfo;
	bool got = false;

	UINT dum;
	DWORD dum2;

	tjs_char* filename = new tjs_char[TJS_strlen(module_filename) + 1];
	try
	{
		TJS_strcpy(filename, module_filename);

		DWORD size = ::GetFileVersionInfoSize (filename, &dum2);
		if(size)
		{
			VersionInfo = new BYTE[size + 2];
			try
			{
				if(::GetFileVersionInfo(filename, 0, size, (void*)VersionInfo))
				{
					if(::VerQueryValue((void*)VersionInfo, TJS_W("\\"), (void**)(&FixedFileInfo),
						&dum))
					{
						major   = FixedFileInfo->dwFileVersionMS >> 16;
						minor   = FixedFileInfo->dwFileVersionMS & 0xffff;
						release = FixedFileInfo->dwFileVersionLS >> 16;
						build   = FixedFileInfo->dwFileVersionLS & 0xffff;
						got = true;
					}
				}
			}
			catch(...)
			{
				delete [] VersionInfo;
				throw;
			}
			delete [] VersionInfo;
		}
	}
	catch(...)
	{
		delete [] filename;
		throw;
	}

	delete [] filename;

	return got;
}
//---------------------------------------------------------------------------
#endif



//---------------------------------------------------------------------------
// TVPCreateNativeClass_Plugins
//---------------------------------------------------------------------------
tTJSNativeClass * TVPCreateNativeClass_Plugins()
{
	tTJSNC_Plugins *cls = new tTJSNC_Plugins();


	// setup some platform-specific members
//---------------------------------------------------------------------------

//-- methods

//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/link)
{
	if(numparams < 1) return TJS_E_BADPARAMCOUNT;

	ttstr name = *param[0];

	TVPLoadPlugin(name);

	return TJS_S_OK;
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(/*object to register*/cls,
	/*func. name*/link)
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/unlink)
{
	if(numparams < 1) return TJS_E_BADPARAMCOUNT;

	ttstr name = *param[0];

	bool res = TVPUnloadPlugin(name);

	if(result) *result = (tjs_int)res;

	return TJS_S_OK;
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(/*object to register*/cls,
	/*func. name*/unlink)
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(getList)
{
	iTJSDispatch2 * array = TJSCreateArrayObject();
	try
	{
#ifdef KRKRSDL2_ENABLE_PLUGINS
		tTVPPluginVectorType::iterator i;
		tjs_int idx = 0;
		for(i = TVPPluginVector.Vector.begin(); i != TVPPluginVector.Vector.end(); i++)
		{
			tTJSVariant val = (*i)->Name.c_str();
			array->PropSetByNum(TJS_MEMBERENSURE, idx++, &val, array);
		}
#endif

		if (result) *result = tTJSVariant(array, array);
	}
	catch(...)
	{
		array->Release();
		throw;
	}
	array->Release();
	return TJS_S_OK;
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(cls, getList)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
	return cls;
}
//---------------------------------------------------------------------------




