//---------------------------------------------------------------------------
/*
	Risa [りさ]      alias 吉里吉里3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
// @brief Win32 GDI 経由でのFreeType Face
//---------------------------------------------------------------------------
#ifndef _NATIVEFREETYPEFACE_H_
#define _NATIVEFREETYPEFACE_H_

#include "tvpfontstruc.h"
#include "FreeTypeFace.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4819)
#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief		Win32 GDI 経由でのFreeType Face クラス
//---------------------------------------------------------------------------
class tNativeFreeTypeFace : public tBaseFreeTypeFace
{
protected:
	tjs_string FaceName;	//!< Face名 = フォント名
	FT_Face Face;	//!< FreeType face オブジェクト

private:
#if 0
	HDC DC;			//!< デバイスコンテキスト
	HFONT OldFont;	//!< デバイスコンテキストに元々登録されていた古いフォント
#endif
	bool IsTTC;		//!< TTC(TrueTypeCollection)ファイルを扱っている場合に真
	FT_StreamRec Stream;
#if 0
	TEXTMETRIC TextMetric;
#endif

public:
	tNativeFreeTypeFace(const tjs_string &fontname, tjs_uint32 options);
	virtual ~tNativeFreeTypeFace();

	virtual FT_Face GetFTFace() const;
	virtual void GetFaceNameList(std::vector<tjs_string> & dest) const; 

	bool GetIsTTC() const { return IsTTC; }
	tjs_char GetDefaultChar() const;

private:
	void Clear();
	static unsigned long IoFunc(
			FT_Stream stream,
			unsigned long   offset,
			unsigned char*  buffer,
			unsigned long   count );
	static void CloseFunc( FT_Stream  stream );

	bool OpenFaceByIndex(int index);

};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------




#endif /*_NATIVEFREETYPEFACE_H_*/
