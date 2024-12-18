
#ifndef __NATIVE_EVENT_QUEUE_H__
#define __NATIVE_EVENT_QUEUE_H__

//

class NativeEvent;
class NativeEventQueueIntarface {
public:
	virtual void Dispatch(NativeEvent& event) = 0;
};
class NativeEvent {
public:
#if 0
	LRESULT Result;
	HWND HWnd;
	UINT Message;
	WPARAM WParam;
	LPARAM LParam;
#endif
 	unsigned int Message;
	intptr_t WParam;
	intptr_t LParam;
	NativeEventQueueIntarface * queue;

#if 0
	NativeEvent(){}
	NativeEvent( int mes ) : Result(0), HWnd(NULL), Message(mes), WParam(0), LParam(0) {}
#endif
	NativeEvent( int mes ) : /*Result(0), HWnd(NULL),*/ Message(mes), WParam(0), LParam(0) {}
	void SetQueue(NativeEventQueueIntarface * tmp_queue)
	{
		queue = tmp_queue;
	}
	void HandleEvent()
	{
		NativeEvent _this = *this;
		queue->Dispatch(_this);
		delete this;
	}
};

#if 0
class NativeEventQueueIntarface {
public:
	//
	virtual void HandlerDefault( class NativeEvent& event ) = 0;

	//
	virtual void Allocate() = 0;

	//
	virtual void Deallocate() = 0;

	virtual void Dispatch( class NativeEvent& event ) = 0;

	virtual void PostEvent( const NativeEvent& event ) = 0;
};
#endif
class NativeEventQueueImplement : public NativeEventQueueIntarface {
#if 0
	HWND window_handle_;
	WNDCLASSEX	wc_;
#endif

	int CreateUtilWindow();
#if 0
	static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
#endif

public:
	static tjs_uint32 native_event_queue_custom_event_type;
#if 0
	NativeEventQueueImplement() : window_handle_(NULL) {}
#endif
	NativeEventQueueImplement();

	//
	void HandlerDefault( NativeEvent& event ) {}

	//
	void Allocate() {}

	//
	void Deallocate() {}

	void PostEvent( const NativeEvent& event );

#if 0
	HWND GetOwner() { return window_handle_; }
#endif
	void Dispatch(NativeEvent& event) {}
};


template<typename T>
class NativeEventQueue : public NativeEventQueueImplement {
	void (T::*handler_)(NativeEvent&);
	T* owner_;

public:
	NativeEventQueue( T* owner, void (T::*Handler)(NativeEvent&) ) : owner_(owner), handler_(Handler) {}

	void Dispatch( NativeEvent &ev ) {
		(owner_->*handler_)(ev);
	}

	T* GetOwner() { return owner_; }
};

#endif // __NATIVE_EVENT_QUEUE_H__
