
#ifndef BASIC_DRAW_DEVICE_H
#define BASIC_DRAW_DEVICE_H

#include "DrawDevice.h"
#if 0
#include <d3d9.h>
#endif

class TVPSDLBitmapCompletion;

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
class tTVPBasicDrawDevice : public tTVPDrawDevice
{
	typedef tTVPDrawDevice inherited;

	TVPSDLBitmapCompletion* bitmap_completion;

#if 0
	void* TargetWindow;
	bool IsMainWindow;
	bool DrawUpdateRectangle;
	bool BackBufferDirty;

	void*				Direct3D;
	void*		Direct3DDevice;
	void*		Texture;
	int	D3dPP;
	int			DispMode;

	UINT	CurrentMonitor;
	void*	TextureBuffer; //
	long	TexturePitch; //

	tjs_uint TextureWidth; //
	tjs_uint TextureHeight; //

	bool ShouldShow; //

	tjs_uint VsyncInterval;
#endif

public:
	tTVPBasicDrawDevice(); //

private:
	~tTVPBasicDrawDevice(); //

	void InvalidateAll();

#if 0
	UINT GetMonitorNumber( HWND window );

	bool IsTargetWindowActive() const;

	bool GetDirect3D9Device();
	HRESULT InitializeDirect3DState();
	HRESULT DecideD3DPresentParameters();

	bool CreateD3DDevice();
#endif
	void DestroyD3DDevice();

#if 0
	bool CreateTexture();
	void DestroyTexture();
#endif

	void TryRecreateWhenDeviceLost();
#if 0
	void ErrorToLog( HRESULT hr );
	void CheckMonitorMoved();
#endif

public:
#if 0
	void SetToRecreateDrawer() { DestroyD3DDevice(); }
#endif

public:
#if 0
	void EnsureDevice();
#endif

//
	virtual void TJS_INTF_METHOD AddLayerManager(iTVPLayerManager * manager);

//
	virtual void TJS_INTF_METHOD SetTargetWindow(HWND wnd, bool is_main);
	virtual void TJS_INTF_METHOD SetDestRectangle(const tTVPRect & rect);
	virtual void TJS_INTF_METHOD NotifyLayerResize(iTVPLayerManager * manager);

//
	virtual void TJS_INTF_METHOD Show();
#if 0
	virtual bool TJS_INTF_METHOD WaitForVBlank( tjs_int* in_vblank, tjs_int* delayed );
#endif

//
	virtual void TJS_INTF_METHOD StartBitmapCompletion(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD NotifyBitmapCompleted(iTVPLayerManager * manager,
		tjs_int x, tjs_int y, const void * bits, const class BitmapInfomation * bitmapinfo,
		const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity);
	virtual void TJS_INTF_METHOD EndBitmapCompletion(iTVPLayerManager * manager);

//
	virtual void TJS_INTF_METHOD SetShowUpdateRect(bool b);

//
	virtual bool TJS_INTF_METHOD SwitchToFullScreen( HWND window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color, bool changeresolution );
	virtual void TJS_INTF_METHOD RevertFromFullScreen( HWND window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color );

};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTJSNI_BasicDrawDevice
//---------------------------------------------------------------------------
class tTJSNI_BasicDrawDevice :
	public tTJSNativeInstance
{
	typedef tTJSNativeInstance inherited;

	tTVPBasicDrawDevice * Device;

public:
	tTJSNI_BasicDrawDevice();
	~tTJSNI_BasicDrawDevice();
	tjs_error TJS_INTF_METHOD
		Construct(tjs_int numparams, tTJSVariant **param,
			iTJSDispatch2 *tjs_obj);
	void TJS_INTF_METHOD Invalidate();

public:
	tTVPBasicDrawDevice * GetDevice() const { return Device; }

};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTJSNC_BasicDrawDevice
//---------------------------------------------------------------------------
class tTJSNC_BasicDrawDevice : public tTJSNativeClass
{
public:
	tTJSNC_BasicDrawDevice();

	static tjs_uint32 ClassID;

private:
	iTJSNativeInstance *CreateNativeInstance();
};
//---------------------------------------------------------------------------


#endif // BASIC_DRAW_DEVICE_H