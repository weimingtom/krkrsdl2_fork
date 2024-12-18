//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Universal Storage System
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include "MsgIntf.h"

#include "StorageImpl.h"
#include "WindowImpl.h"
#include "SysInitIntf.h"
#include "DebugIntf.h"
#include "Random.h"
#include "XP3Archive.h"
#include "Random.h"

#include "Application.h"
#include "StringUtil.h"
#include "FilePathUtil.h"
#include "TickCount.h"

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

#if defined(__vita__)
#include <psp2/io/devctl.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#include <android/configuration.h>
#include <android/asset_manager_jni.h>
#include "AndroidAssetManager.h"
#endif

#if defined(_MSC_VER)
#include <direct.h> //_mkdir
#include <sys/types.h>
#include <sys/stat.h> //stat
#define  S_ISDIR(model) ((model) & _S_IFDIR)
#define  S_ISREG(model) ((model) & _S_IFREG)
#define strcasecmp stricmp

//https://www.cnblogs.com/lanxuezaipiao/p/3420025.html
#define DT_REG 1

typedef struct _dirdesc {
	int     dd_fd;      /** file descriptor associated with directory */
	long    dd_loc;     /** offset in current buffer */
	long    dd_size;    /** amount of data returned by getdirentries */
	char    *dd_buf;    /** data buffer */
	int     dd_len;     /** size of data buffer */
	long    dd_seek;    /** magic cookie returned by getdirentries */
} DIR;

# define __dirfd(dp)    ((dp)->dd_fd)

DIR *opendir(const char *);
struct dirent *readdir(DIR *);
void rewinddir(DIR *);
int closedir(DIR *);

#include <sys/types.h>

struct dirent
{
	long d_ino;              /* inode number*/
	off_t d_off;             /* offset to this dirent*/
	DWORD d_reclen; /* length of this d_name*/
	unsigned char d_type;    /* the type of d_name*/
	char d_name[1];          /* file name (null-terminated)*/
};


static HANDLE hFind;

DIR *opendir(const char *name)
{
	DIR *dir;
	WIN32_FIND_DATAA FindData;
	char namebuf[512];

	sprintf(namebuf, "%s\\*.*", name);

	hFind = FindFirstFileA(namebuf, &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
#if ENABLE_DEBUG_BREAK
		//_debugbreak();
		//printf("FindFirstFile failed (%d)\n", GetLastError());
#endif
		return 0;
	}

	dir = (DIR *)malloc(sizeof(DIR));
	if (!dir)
	{
#if ENABLE_DEBUG_BREAK
		//_debugbreak();
		//printf("DIR memory allocate fail\n");
#endif
		return 0;
	}

	memset(dir, 0, sizeof(DIR));
	dir->dd_fd = 0;   // simulate return  

	return dir;
}

struct dirent *readdir(DIR *d)
{
	int i;
	static struct dirent dirent;
	BOOL bf;
	WIN32_FIND_DATAA FileData;
	if (!d)
	{
		return 0;
	}

	bf = FindNextFileA(hFind, &FileData);
	//fail or end  
	if (!bf)
	{
		return 0;
	}

	for (i = 0; i < 256; i++)
	{
		dirent.d_name[i] = FileData.cFileName[i];
		if (FileData.cFileName[i] == '\0') break;
	}
	dirent.d_reclen = i;
	dirent.d_reclen = FileData.nFileSizeLow;

	//check there is file or directory  
	if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		dirent.d_type = 2;
	}
	else
	{
		dirent.d_type = 1;
	}


	return (&dirent);
}

int closedir(DIR *d)
{
	if (!d) return -1;
	hFind = 0;
	free(d);
	return 0;
}

#endif

//---------------------------------------------------------------------------
// tTVPFileMedia
//---------------------------------------------------------------------------
class tTVPFileMedia : public iTVPStorageMedia
{
	tjs_uint RefCount;

public:
	tTVPFileMedia()
	{
		RefCount = 1;
	}
	~tTVPFileMedia()
	{
#if defined(__ANDROID__)
		AndroidAssetManager_Destroy_AssetManager();
#endif
	}

	void TJS_INTF_METHOD AddRef() { RefCount ++; }
	void TJS_INTF_METHOD Release()
	{
		if(RefCount == 1)
			delete this;
		else
			RefCount --;
	}

	void TJS_INTF_METHOD GetName(ttstr &name) { name = TJS_W("file"); }

	void TJS_INTF_METHOD NormalizeDomainName(ttstr &name);
	void TJS_INTF_METHOD NormalizePathName(ttstr &name);
	bool TJS_INTF_METHOD CheckExistentStorage(const ttstr &name);
	tTJSBinaryStream * TJS_INTF_METHOD Open(const ttstr & name, tjs_uint32 flags);
	void TJS_INTF_METHOD GetListAt(const ttstr &name, iTVPStorageLister *lister);
	void TJS_INTF_METHOD GetLocallyAccessibleName(ttstr &name);

public:
	void TJS_INTF_METHOD GetLocalName(ttstr &name);
};
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPFileMedia::NormalizeDomainName(ttstr &name)
{
	// normalize domain name
	// make all characters small
#ifdef KRKRZ_CASE_INSENSITIVE
	tjs_char *p = name.Independ();
	while(*p)
	{
		if(*p >= TJS_W('A') && *p <= TJS_W('Z'))
			*p += TJS_W('a') - TJS_W('A');
		p++;
	}
#endif
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPFileMedia::NormalizePathName(ttstr &name)
{
	// 非Windows環境では大文字小文字区別する実装の方が良いか？
	// normalize path name
	// make all characters small
#ifdef KRKRZ_CASE_INSENSITIVE
	tjs_char *p = name.Independ();
	while(*p)
	{
		if(*p >= TJS_W('A') && *p <= TJS_W('Z'))
			*p += TJS_W('a') - TJS_W('A');
		p++;
	}
#endif
}
//---------------------------------------------------------------------------
bool TJS_INTF_METHOD tTVPFileMedia::CheckExistentStorage(const ttstr &name)
{
	if(name.IsEmpty()) return false;

	ttstr _name(name);
	GetLocalName(_name);

	return TVPCheckExistentLocalFile(_name);
}
//---------------------------------------------------------------------------
tTJSBinaryStream * TJS_INTF_METHOD tTVPFileMedia::Open(const ttstr & name, tjs_uint32 flags)
{
	// open storage named "name".
	// currently only local/network(by OS) storage systems are supported.
	if(name.IsEmpty())
		TVPThrowExceptionMessage(TVPCannotOpenStorage, TJS_W("\"\""));

	ttstr origname = name;
	ttstr _name(name);
	GetLocalName(_name);

	return new tTVPLocalFileStream(origname, _name, flags);
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPFileMedia::GetListAt(const ttstr &_name, iTVPStorageLister *lister)
{
	ttstr name(_name);
	GetLocalName(name);
	tjs_string wname(name.AsStdString());
	std::string nname;
	if( TVPUtf16ToUtf8(nname, wname) ) {
#if defined(__ANDROID__)
		AAssetManager *asset_manager = AndroidAssetManager_Get_AssetManager();
		AAssetDir* android_dr;
#endif
#if 0//defined(_MSC_VER)


		not_imp();











#else

#if defined(__vita__)
		SceUID dr;
		if( ( dr = sceIoDopen(nname.c_str()) ) >= 0 )
#else
		DIR* dr;
		if( ( dr = opendir(nname.c_str()) ) != nullptr )
#endif
		{
#if defined(__vita__)
			SceIoDirent entry;
			while( sceIoDread( dr, &entry ) > 0 )
#else
			struct dirent* entry;
			while( ( entry = readdir( dr ) ) != nullptr )
#endif
			{
#if defined(__vita__)
				if (SCE_S_ISREG(entry.d_stat.st_mode))
#else
				if( entry->d_type == DT_REG )
#endif
				{
					tjs_char fname[256];
#if defined(__vita__)
					tjs_int count = TVPUtf8ToWideCharString( entry.d_name, fname );
#else
					tjs_int count = TVPUtf8ToWideCharString( entry->d_name, fname );
#endif
#if defined(_MSC_VER)
#if ENABLE_DEBUG_BREAK
					__debugbreak();
#endif
					if (count > 0) {
						fname[count] = TJS_W('\0');
					}
					else {
						fname[0] = TJS_W('\0');
					}
#else
					fname[count] = TJS_W('\0');
#endif
					ttstr file(fname);
#ifdef KRKRZ_CASE_INSENSITIVE
					tjs_char *p = file.Independ();
					while(*p) {
						// make all characters small
						if(*p >= TJS_W('A') && *p <= TJS_W('Z'))
							*p += TJS_W('a') - TJS_W('A');
						p++;
					}
#endif
					lister->Add(file);
				}
				// entry->d_type == DT_UNKNOWN
			}
#if defined(__vita__)
			sceIoDclose( dr );
#else
			closedir( dr );
#endif
		}
#if defined(__ANDROID__)
		// Skip the leading slash.
		else if ( asset_manager != NULL && nname.length() > 0 && nname[0] == '/' && (AndroidAssetManager_Check_Directory_Existent(asset_manager, nname.c_str() + 1)) && (android_dr = AAssetManager_openDir( asset_manager, nname.c_str() + 1 )) )
		{
			const char* filename = nullptr;
			do {
				filename = AAssetDir_getNextFileName( android_dr );
				if( filename ) {
					tjs_char fname[256];
					tjs_int count = TVPUtf8ToWideCharString( filename, fname );
					fname[count] = TJS_W('\0');
					ttstr file( fname );
#ifdef KRKRZ_CASE_INSENSITIVE
					tjs_char *p = file.Independ();
					while(*p) {
						// make all characters small
						if(*p >= TJS_W('A') && *p <= TJS_W('Z'))
							*p += TJS_W('a') - TJS_W('A');
						p++;
					}
#endif
					lister->Add( file );
				}
			} while( filename );
			AAssetDir_close( android_dr );
		}
#endif
#endif /*_MSC_VER*/
	}
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPFileMedia::GetLocallyAccessibleName(ttstr &name)
{
	tjs_string wname(name.AsStdString());
	std::string nname;
	if (!TVPUtf16ToUtf8(nname, wname))
	{
		name.Clear();
		return;
	}

#if 0
	ttstr newname;

	const tjs_char *ptr = name.c_str();
	if( *ptr == TJS_W('.') ) ptr++;
	while( (*ptr == TJS_W('/') || *ptr == TJS_W('\\')) && (ptr[1] == TJS_W('/') || ptr[1] == TJS_W('\\')) ) ptr++;
	newname = ttstr(ptr);
	// change path delimiter to '/'
	if (!newname.IsEmpty()) {
		tjs_char *pp = newname.Independ();
		while(*pp)
		{
			if(*pp == TJS_W('\\')) *pp = TJS_W('/');
			pp++;
		}
	}
	name = newname;
#endif

#if defined(__ANDROID__)
	AAssetManager *asset_manager = AndroidAssetManager_Get_AssetManager();
#endif

	std::string nnewname;
	const char *ptr = nname.c_str();
	tjs_int path_entries = 0;
	std::string domain_name;

	while (*ptr)
	{
		const char *ptr_end = ptr;
		while (*ptr_end && (*ptr_end != '/' && *ptr_end != '\\'))
		{
			ptr_end += 1;
		}
		if (ptr_end == ptr)
		{
			break;
		}
		const char *ptr_cur = ptr;
		std::string nwalker(ptr, ptr_end - ptr);
		while (*ptr_end && (*ptr_end == '/' || *ptr_end == '\\'))
		{
			ptr_end += 1;
		}
		ptr = ptr_end;
		if (path_entries == 0)
		{
			domain_name = nwalker;
		}
		path_entries += 1;
		if (nwalker == "." || (path_entries == 1 && nwalker == "?"))
		{
			continue;
		}

#if 0//defined(_MSC_VER)




		not_imp();














#else
		DIR *dirp;
#if defined(__ANDROID__)
		AAssetDir* android_dr;
#endif
		struct dirent *direntp;
		if ((path_entries != 2) || (path_entries == 2 && domain_name != "?"))
		{
			nnewname += "/";
		}
		else
		{
			nnewname += "./";
		}

		if ((dirp = opendir(nnewname.c_str())))
		{
			bool found = false;
			bool is_directory = false;
			while (found == false && (direntp = readdir(dirp)) != nullptr)
			{
				if (!strcasecmp(nwalker.c_str(), direntp->d_name))
				{
					nnewname += direntp->d_name;
					found = true;
					break;
				}
			}
			closedir(dirp);
			if (!found)
			{
				nnewname += ptr_cur;
				break;
			}
		}
#if defined(__ANDROID__)
		// Skip the leading slash.
		else if ( asset_manager != NULL && nnewname.length() > 0 && nnewname[0] == '/' && (AndroidAssetManager_Check_Directory_Existent(asset_manager, nnewname.c_str() + 1)) && (android_dr = AAssetManager_openDir( asset_manager, nnewname.c_str() + 1 )) )
		{
			const char* filename = nullptr;
			bool found = false;
			bool is_directory = false;
			while (found == false && (filename = AAssetDir_getNextFileName( android_dr )) != nullptr)
			{
				if (!strcasecmp(nwalker.c_str(), filename))
				{
					nnewname += filename;
					found = true;
					break;
				}
			}
			AAssetDir_close( android_dr );
			if (!found)
			{
				nnewname += ptr_cur;
				break;
			}
		}
#endif
		else
		{
			if (errno == EPERM || errno == EACCES) // Most likely inside the iOS sandbox, so just append the name
			{
				nnewname += nwalker;
			}
			else if (errno == ENOENT || errno == ENOTDIR)
			{
				nnewname += ptr_cur;
				break;
			}
			else // Other error, probably can't access anyway
			{
				nnewname += ptr_cur;
				break;
			}
		}
#endif /*_MSC_VER*/
	}

	if (path_entries == 1 && domain_name == "?")
	{
		name = ttstr(TJS_W("./"));
		return;
	}

	tjs_string wnewname;
	if (!TVPUtf8ToUtf16(wnewname, nnewname))
	{
		name.Clear();
		return;
	}

	name = ttstr(wnewname);
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPFileMedia::GetLocalName(ttstr &name)
{
	ttstr tmp = name;
	GetLocallyAccessibleName(tmp);
	if(tmp.IsEmpty()) TVPThrowExceptionMessage(TVPCannotGetLocalName, name);
	name = tmp;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
iTVPStorageMedia * TVPCreateFileMedia()
{
	return new tTVPFileMedia;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// TVPPreNormalizeStorageName
//---------------------------------------------------------------------------
void TVPPreNormalizeStorageName(ttstr &name)
{
	// if the name is an OS's native expression, change it according with the
	// TVP storage system naming rule.
	tjs_int namelen = name.GetLen();
	if(namelen == 0) return;

	tjs_char lastchar = name.GetLastChar();

#if defined(__SWITCH__) || defined(__vita__)
	// HACK for Switch and Vita: colon in filesystem causes a conflict
	if ((TJS_strstr(name.c_str(), TJS_W("file:")) == nullptr) && (TJS_strchr(name.c_str(), TJS_W(':')) != nullptr))
	{
		ttstr newname(TJS_W("file://?/"));
		newname += name;
		name = newname;
	}
#endif

	if (namelen >= 1)
	{
		if (name[0] == TJS_W('/'))
		{
			// POSIX absolute path
			name = ttstr(TJS_W("file://./")) + name;
			return;
		}
	}
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TVPGetTemporaryName
//---------------------------------------------------------------------------
static tjs_int TVPTempUniqueNum = 0;
static tTJSCriticalSection TVPTempUniqueNumCS;
static ttstr TVPTempPath;
bool TVPTempPathInit = false;
static tjs_int TVPProcessID;
ttstr TVPGetTemporaryName()
{
	tjs_int num;

	{
		tTJSCriticalSectionHolder holder(TVPTempUniqueNumCS);

		if(!TVPTempPathInit)
		{
			//tjs_char tmp[MAX_PATH+1];
			//TVPUtf8ToWideCharString( Application->GetCachePath()->c_str(), static_cast<tjs_char*>(tmp) );
#if 0
			TVPTempPath = ttstr( Application->GetCachePath()->c_str() );
#endif
#if defined(__SWITCH__)
			TVPTempPath = ttstr( "sdmc:/tmp/" );
#elif defined(__vita__)
			TVPTempPath = ttstr( "./tmp/" );
#else
			TVPTempPath = ttstr( "/tmp/" );
#endif

			if(TVPTempPath.GetLastChar() != TJS_W('/')) TVPTempPath += TJS_W("/");
			TVPProcessID = static_cast<tjs_int>( getpid() );
			TVPTempUniqueNum = static_cast<tjs_int>( TVPGetRoughTickCount32() );
			TVPTempPathInit = true;
		}
		num = TVPTempUniqueNum ++;
	}

	unsigned char buf[16];
	TVPGetRandomBits128(buf);
	tjs_char random[128];
	TJS_snprintf(random, sizeof(random)/sizeof(tjs_char), TJS_W("%02x%02x%02x%02x%02x%02x"),
		buf[0], buf[1], buf[2], buf[3],
		buf[4], buf[5]);

	return TVPTempPath + TJS_W("krkr_") + ttstr(random) +
		TJS_W("_") + ttstr(num) + TJS_W("_") + ttstr(TVPProcessID);
}
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
// TVPRemoveFile
//---------------------------------------------------------------------------
bool TVPRemoveFile(const ttstr &name)
{
	std::string filename;
	if( TVPUtf16ToUtf8( filename, name.AsStdString() ) ) {
#if defined(__vita__)
		bool res = 0 == sceIoRemove(filename.c_str());
#else
		bool res = 0 == remove(filename.c_str());
#endif
		if (res)
		{
			Application->SyncSavedata();
		}
		return res;
	} else {
		return false;
	}

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPRemoveFolder
//---------------------------------------------------------------------------
bool TVPRemoveFolder(const ttstr &name)
{
	std::string filename;
	if( TVPUtf16ToUtf8( filename, name.AsStdString() ) ) {
#if defined(__vita__)
		bool res = 0==sceIoRmdir(filename.c_str());
#else
		bool res = 0==rmdir(filename.c_str());
#endif
		if (res)
		{
			Application->SyncSavedata();
		}
		return res;
	} else {
		return false;
	}
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TVPGetAppPath
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TVPOpenStream
//---------------------------------------------------------------------------
tTJSBinaryStream * TVPOpenStream(const ttstr & _name, tjs_uint32 flags)
{
	// open storage named "name".
	// currently only local/network(by OS) storage systems are supported.
	if(_name.IsEmpty())
		TVPThrowExceptionMessage(TVPCannotOpenStorage, TJS_W("\"\""));

	ttstr origname = _name;
	ttstr name(_name);
	TVPGetLocalName(name);

	return new tTVPLocalFileStream(origname, name, flags);
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// TVPCheckExistantLocalFile
//---------------------------------------------------------------------------
bool TVPCheckExistentLocalFile(const ttstr &name)
{
	std::string filename;
	if( TVPUtf16ToUtf8( filename, name.AsStdString() ) ) {
#if defined(__vita__)
		SceIoStat st;
		if( sceIoGetstat( filename.c_str(), &st) >= 0)
			if( SCE_S_ISREG(st.st_mode) )
#else
		struct stat st;
		if( stat( filename.c_str(), &st) == 0)
			if( S_ISREG(st.st_mode) )
#endif
				return true;
#if defined(__ANDROID__)
		AAssetManager *asset_manager = AndroidAssetManager_Get_AssetManager();
		// Skip the leading slash.
		if (asset_manager != NULL && filename.length() > 0 && filename[0] == '/')
		{
			AAsset* asset = AAssetManager_open( asset_manager, filename.c_str() + 1, AASSET_MODE_UNKNOWN);
			bool result = asset != NULL;
			if ( result )
			{
				AAsset_close( asset );
				return result;
			}
		}
#endif
	}
	return false;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TVPCheckExistantLocalFolder
//---------------------------------------------------------------------------
bool TVPCheckExistentLocalFolder(const ttstr &name)
{
	std::string filename;
	if( TVPUtf16ToUtf8( filename, name.AsStdString() ) ) {
#if defined(__vita__)
		SceIoStat st;
		if( sceIoGetstat( filename.c_str(), &st) >= 0)
			if( SCE_S_ISDIR(st.st_mode) )
#else
		struct stat st;
		if( stat( filename.c_str(), &st) == 0)
			if( S_ISDIR(st.st_mode) )
#endif
				return true;
#if defined(__ANDROID__)
		AAssetManager *asset_manager = AndroidAssetManager_Get_AssetManager();
		// Skip the leading slash.
		if (asset_manager != NULL && filename.length() > 0 && filename[0] == '/')
		{
			AAssetDir* asset = AAssetManager_openDir( asset_manager, filename.c_str() + 1 );
			bool result = AndroidAssetManager_Check_Directory_Existent(asset_manager, filename.c_str() + 1);
			if ( result )
			{
				return result;
			}
		}
#endif
	}
	return false;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// TVPOpenArchive
//---------------------------------------------------------------------------
tTVPArchive * TVPOpenArchive(const ttstr & name)
{
	return new tTVPXP3Archive(name);
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TVPLocalExtrectFilePath
//---------------------------------------------------------------------------
ttstr TVPLocalExtractFilePath(const ttstr & name)
{
	// this extracts given name's path under local filename rule
	const tjs_char *p = name.c_str();
	tjs_int i = name.GetLen() -1;
	for(; i >= 0; i--)
	{
		if(p[i] == TJS_W(':') || p[i] == TJS_W('/') ||
			p[i] == TJS_W('\\'))
			break;
	}
	return ttstr(p, i + 1);
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TVPCreateFolders
//---------------------------------------------------------------------------
static bool _TVPCreateFolders(const ttstr &folder)
{
	// create directories along with "folder"
	if(folder.IsEmpty()) return true;

	if(TVPCheckExistentLocalFolder(folder))
		return true; // already created

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if(p[i] == TJS_W(':')) return true;

	while(i >= 0 && (p[i] == TJS_W('/') || p[i] == TJS_W('\\'))) i--;

	if(i >= 0 && p[i] == TJS_W(':')) return true;

	for(; i >= 0; i--)
	{
		if(p[i] == TJS_W(':') || p[i] == TJS_W('/') ||
			p[i] == TJS_W('\\'))
			break;
	}

	ttstr parent(p, i + 1);

	if(!_TVPCreateFolders(parent)) return false;

	std::string filename;
	int res = -1;
	if( TVPUtf16ToUtf8( filename, folder.AsStdString() ) ) {
#if defined(__vita__)
		res = sceIoMkdir( filename.c_str(), 0777 );
#elif defined(_MSC_VER)
		res = _mkdir(filename.c_str());
#else
		res = mkdir( filename.c_str(), 0777 );
#endif
	}
	return 0 == res;
}

bool TVPCreateFolders(const ttstr &folder)
{
	if(folder.IsEmpty()) return true;

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if(p[i] == TJS_W(':')) return true;

	if(p[i] == TJS_W('/') || p[i] == TJS_W('\\')) i--;

	return _TVPCreateFolders(ttstr(p, i+1));
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// tTVPLocalFileStream
//---------------------------------------------------------------------------
tTVPLocalFileStream::tTVPLocalFileStream(const ttstr &origname,
	const ttstr &localname, tjs_uint32 flag)
{
	tjs_uint32 access = flag & TJS_BS_ACCESS_MASK;
	io_handle = NULL;
	written = false;
	const char* mode = "rb";
	switch(access)
	{
	case TJS_BS_READ:
		mode = "rb";		break;
	case TJS_BS_WRITE:
		mode = "wb";		break;
	case TJS_BS_APPEND:
		mode = "ab";		break;
	case TJS_BS_UPDATE:
		mode = "rb+";		break;
	}

	tjs_int trycount = 0;
	std::string filename;
	TVPUtf16ToUtf8( filename, localname.AsStdString() );

retry:
	io_handle = SDL_RWFromFile(filename.c_str(), mode);
	if(io_handle == nullptr)
	{
		if(trycount == 0 && access == TJS_BS_WRITE)
		{
			trycount++;

			// retry after creating the folder
			if(TVPCreateFolders(TVPLocalExtractFilePath(localname)))
				goto retry;
		}
#ifdef __ANDROID__
		// Special case for Android and reading the asset filesystem from AAssetManager
		if (trycount == 0 && access == TJS_BS_READ && filename.length() > 0 && filename[0] == '/')
		{
			trycount++;

			// retry after removing the leading slash
			filename.erase(0, 1);
			goto retry;
		}
#endif
		TVPThrowExceptionMessage(TVPCannotOpenStorage, origname);
	}

	if (access == TJS_BS_APPEND) // move the file pointer to last
	{
		SetEndOfStorage();
	}

	// push current tick as an environment noise
	tjs_uint32 tick = TVPGetRoughTickCount32();
	TVPPushEnvironNoise(&tick, sizeof(tick));
}
//---------------------------------------------------------------------------
tTVPLocalFileStream::~tTVPLocalFileStream()
{
	if (io_handle != nullptr)
	{
		SDL_RWclose(io_handle);
	}

	// push current tick as an environment noise
	// (timing information from file accesses may be good noises)
	tjs_uint32 tick = TVPGetRoughTickCount32();
	TVPPushEnvironNoise(&tick, sizeof(tick));

	if (written)
	{
		Application->SyncSavedata();
	}
}
//---------------------------------------------------------------------------
tjs_uint64 TJS_INTF_METHOD tTVPLocalFileStream::Seek(tjs_int64 offset, tjs_int whence)
{
	int dwmm;
	switch(whence)
	{
	case TJS_BS_SEEK_SET:	dwmm = RW_SEEK_SET;	break;
	case TJS_BS_SEEK_CUR:	dwmm = RW_SEEK_CUR;	break;
	case TJS_BS_SEEK_END:	dwmm = RW_SEEK_END;	break;
	default:				dwmm = RW_SEEK_SET;	break; // may be enough
	}

	Sint64 low = SDL_RWseek(io_handle, offset, dwmm);
	if (low < 0)
	{
		TVPThrowExceptionMessage(TVPSeekError);
	}
	return (tjs_uint64)low;
}
//---------------------------------------------------------------------------
tjs_uint TJS_INTF_METHOD tTVPLocalFileStream::Read(void *buffer, tjs_uint read_size)
{
	size_t ret = SDL_RWread(io_handle, buffer, 1, read_size);
	return (tjs_uint)ret;
}
//---------------------------------------------------------------------------
tjs_uint TJS_INTF_METHOD tTVPLocalFileStream::Write(const void *buffer, tjs_uint write_size)
{
	written = true;
	size_t ret = SDL_RWwrite(io_handle, buffer, 1, write_size);
	return (tjs_uint)ret;
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLocalFileStream::SetEndOfStorage()
{
	Seek(0, TJS_BS_SEEK_END);
}
//---------------------------------------------------------------------------
tjs_uint64 TJS_INTF_METHOD tTVPLocalFileStream::GetSize()
{
#if 0
	tjs_uint64 oldpos = Seek(0, TJS_BS_SEEK_CUR);
	tjs_uint64 retpos = Seek(0, TJS_BS_SEEK_END);
	Seek(oldpos, TJS_BS_SEEK_SET);
	return retpos;
#endif
	Sint64 low = SDL_RWsize(io_handle);
	if (low < 0)
	{
		TVPThrowExceptionMessage(TVPSeekError);
	}
	return (tjs_uint64)low;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// tTVPPluginHolder
//---------------------------------------------------------------------------
tTVPPluginHolder::tTVPPluginHolder(const ttstr &aname)
: LocalTempStorageHolder(nullptr)
{
#if 0
	// /data/data/(パッケージ名)/lib/
	tjs_string sopath = tjs_string(TJS_W("file://")) + tjs_string(Application->GetSoPath()) + aname.AsStdString();
	//tjs_string sopath = tjs_string(TJS_W("/data/data/")) + tjs_string(Application->GetPackageName()) + tjs_string(TJS_W("/lib/")) + aname.AsStdString();
	ttstr place( sopath.c_str() );
	LocalTempStorageHolder = new tTVPLocalTempStorageHolder(place);
#endif
	LocalTempStorageHolder = NULL;

	// search in TVP storage system
	ttstr place(TVPGetPlacedPath(aname));
	if(!place.IsEmpty())
	{
		LocalTempStorageHolder = new tTVPLocalTempStorageHolder(place);
	}
}
//---------------------------------------------------------------------------
tTVPPluginHolder::~tTVPPluginHolder()
{
	if(LocalTempStorageHolder)
	{
		delete LocalTempStorageHolder;
	}
}
//---------------------------------------------------------------------------
const ttstr & tTVPPluginHolder::GetLocalName() const
{
	if(LocalTempStorageHolder) return LocalTempStorageHolder->GetLocalName();
	return LocalPath;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPCreateNativeClass_Storages
//---------------------------------------------------------------------------
tTJSNativeClass * TVPCreateNativeClass_Storages()
{
	tTJSNC_Storages *cls = new tTJSNC_Storages();


	// setup some platform-specific members
//----------------------------------------------------------------------

//-- methods

//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/searchCD)
{
	return TJS_E_NOTIMPL;
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(/*object to register*/cls,
	/*func. name*/searchCD)
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/getLocalName)
{
	if(numparams < 1) return TJS_E_BADPARAMCOUNT;

	if(result)
	{
		ttstr str(TVPNormalizeStorageName(*param[0]));
		TVPGetLocalName(str);
		*result = str;
	}

	return TJS_S_OK;
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(/*object to register*/cls,
	/*func. name*/getLocalName)
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/selectFile)
{
#if 1
	return TJS_E_NOTIMPL;
#else
	if(numparams < 1) return TJS_E_BADPARAMCOUNT;

	iTJSDispatch2 * dsp =  param[0]->AsObjectNoAddRef();

	bool res = TVPSelectFile(dsp);

	if(result) *result = (tjs_int)res;

	return TJS_S_OK;
#endif
}
TJS_END_NATIVE_STATIC_METHOD_DECL_OUTER(/*object to register*/cls,
	/*func. name*/selectFile)
//----------------------------------------------------------------------


	return cls;

}
//---------------------------------------------------------------------------

