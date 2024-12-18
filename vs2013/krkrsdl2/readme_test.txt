0.
run or debug method with vs2013, command line params:
krkrsdl2.exe data
(point to data folder with startup.tjs)
see https://github.com/krkrsdl2/kag3, last .otf font, kag3-00fd6bba4e2eb9f6ad25526ee715964a25ed4c3f.zip

1. 

#if ENABLE_DEBUG_BREAK
	__debugbreak();
#endif

2. 
		char *buf = (char *)malloc(size);
#if !defined(_MSC_VER)
		char *dir = getcwd(buf, size);
		while (dir == nullptr && buf != nullptr && errno == ERANGE)
		{
			size *= 2;
			buf = (char *)realloc(buf, size);
			dir = getcwd(buf, size);
		}
#else
---->		//char *dir = ".";
#if ENABLE_DEBUG_BREAK
		//__debugbreak();
#endif
		char *dir = getcwd(buf, size);
		while (dir == nullptr && buf != nullptr && errno == ERANGE)
		{
			size *= 2;
			buf = (char *)realloc(buf, size);
			dir = getcwd(buf, size);
		}
#endif

3.
TVPCannotSuggestGraphicExtension

GraphicsLoaderIntf.cpp
TVPLoadGraphic

if(i.IsNull())
{
	// not found
no webp plugin-->	TVPThrowExceptionMessage(TVPCannotSuggestGraphicExtension, name);
}
...
if(i.IsNull())
			{
				// not found
here--->				handler = NULL;
				break;
			}
...
handler is null--->if(handler)
{
...
not called here-->(handler->Load)


dllmain.cpp
void TVPLoadWEBP(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback, tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback, IStream_ *src, tjs_int keyidx, tTVPGraphicLoadMode mode)
not called here-->{


solve:
//#ifndef _WIN32
	if (!TVPLoadEmscriptenPreloadedData(handler->FormatData, (void*)&data, TVPLoadGraphic_SizeCallback,
		TVPLoadGraphic_ScanLineCallback, TVPLoadGraphic_MetaInfoPushCallback,
		name, keyidx, mode))
	{
here enter into TVPLoadWEBP -->		(handler->Load)(handler->FormatData, (void*)&data, TVPLoadGraphic_SizeCallback,
			TVPLoadGraphic_ScanLineCallback, TVPLoadGraphic_MetaInfoPushCallback,
			holder.Get(), keyidx, mode);
	}
//#endif










void tTVPGraphicHandlerType::Load( void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback, tTVPGraphicScanLineCallback scanlinecallback,
	tTVPMetaInfoPushCallback metainfopushcallback, tTJSBinaryStream *src, tjs_int32 keyidx, tTVPGraphicLoadMode mode)
{
	if( LoadHandler == NULL ) TVPThrowExceptionMessage(TVPUnknownGraphicFormat, TJS_W("unknown"));

---->#if 0// defined(_WIN32)
	if( IsPlugin )
	{
#if ENABLE_DEBUG_BREAK
		__debugbreak();
#endif
	}
#else











---->//TVPAddLog(TJS_W("(info) TVPGraphicType: ") + i.GetKey());












	case bmAlphaOnAddAlpha:
		// simple alpha on additive alpha
		if(opa == 255)
		{
			while(h--)
TVPAlphaBlend_a==NULL -->				TVPAlphaBlend_a((tjs_uint32*)dest, (tjs_uint32*)src, w),
				dest+=dpitch, src+=spitch;
		}

mod changed to this: (search (void)TVPAddLog)
				(TVPAlphaBlend_a != 0) ? 
					TVPAlphaBlend_a((tjs_uint32*)dest, (tjs_uint32*)src, w) : 
					(void)TVPAddLog("(notimp) TVPAlphaBlend_a not implemented"),

same mod:
#define TVP_BLEND_4(basename) /* blend for 4 types (normal, opacity, HDA, HDA opacity) */ \
	if(opa == 255)                                                            \
	{                                                                         \
		if(!hda)                                                              \
		{                                                                     \
			while(h--)                                                        \
				basename != 0 ? \
					basename((tjs_uint32*)dest, (tjs_uint32*)src, w): \
					(void)TVPAddLog("(notimp) " #basename " not implemented"), \
				dest+=dpitch, src+=spitch;    




-=========================


	TJS_METHOD_DEF(TJS_METHOD_RET_EMPTY, ~tTJSString, ()) { 
don't know why???--->		if (Ptr == (tTJSVariantString *)0x676f6c61) { 
			tTJSVariantString *temp = this->Ptr;
--->			OutputDebugString(L"hit 0x676f6c61\n"); 
			return; 
		} 
		if (Ptr) { 
			Ptr->Release(); 
		} 
	}

because this ???==>

ttstr TVPGetAppPath()
{
	static ttstr exepath1(TVPExtractStoragePath(TVPNormalizeStorageName(ExePath())));
	return exepath1;
}

mod 2 places:

ttstr TVPGetAppPath()
{
#if !defined(_MSC_VER)
	static ttstr exepath1(TVPExtractStoragePath(TVPNormalizeStorageName(ExePath())));
	return exepath1;
#else
	ttstr exepath1(TVPExtractStoragePath(TVPNormalizeStorageName(ExePath())));
	return exepath1;
#endif
}

tjs_string ExePath() {
#if 0
	tjs_char szFull[_MAX_PATH];
	::GetModuleFileName(NULL, szFull, sizeof(szFull) / sizeof(tjs_char));
	return tjs_string(szFull);
#endif
#if !defined(_MSC_VER)
	static tjs_string exepath2(TJS_W(""));
#else
	tjs_string exepath2(TJS_W(""));
#endif
























------------------------

Ctrl+c Mutex exception


//https://www.cnblogs.com/hookjc/p/13183741.html
#if defined(_MSC_VER)
BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	char mesg[128];
	switch(CEvent)
	{
	case CTRL_C_EVENT:
		MessageBoxA(NULL,
			"CTRL+C received!", "CEvent", MB_OK);
		break;

	case CTRL_BREAK_EVENT:
		MessageBoxA(NULL,
			"CTRL+BREAK received!", "CEvent", MB_OK);
		break;

	case CTRL_CLOSE_EVENT:
		MessageBoxA(NULL,
			"Program being closed!", "CEvent", MB_OK);
		break;

	case CTRL_LOGOFF_EVENT:
		MessageBoxA(NULL,
			"User is logging off!", "CEvent", MB_OK);
		break;

	case CTRL_SHUTDOWN_EVENT:
		MessageBoxA(NULL,
			"User is logging off!", "CEvent", MB_OK);
		break;
	}
	return TRUE;
}
#endif

#if defined(USE_SDL_MAIN)
extern "C" int SDL_main(int argc, char **argv)
#else
extern "C" int main(int argc, char **argv)
#endif
{
#if defined(_MSC_VER)
	if (SetConsoleCtrlHandler(
		(PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}
#endif

------------------

