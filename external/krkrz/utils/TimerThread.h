//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Timer Base Class :
//---------------------------------------------------------------------------
#ifndef TimerBaseH
#define TimerBaseH

#include "NativeEventQueue.h"
#include "ThreadIntf.h"

//---------------------------------------------------------------------------
// tTVPTimerBase
//---------------------------------------------------------------------------
class tTVPTimerBase
{
	friend class tTVPTimerThread;

private:
	tjs_uint64 Interval;
	tjs_uint64 NextTick;
	tjs_int PendingCount;
	bool Enabled;

public:
	tTVPTimerBase();
	virtual ~tTVPTimerBase(){}

	//
	void InternalSetEnabled(bool b) { Enabled = b; }
	void SetEnabled(bool b);
	bool GetEnabled() const { return Enabled; }

	//
	void InternalSetInterval(tjs_uint64 n) { Interval = n; }
	virtual void SetInterval(tjs_uint64 n);
	virtual tjs_uint64 GetInterval() const { return Interval; }

	//
	tjs_uint64 GetNextTick() const { return NextTick; }
	void SetNextTick(tjs_uint64 n)  { NextTick = n; }

	void ZeroPendingCount() { PendingCount = 0; }

	//
	void Trigger(tjs_uint n);

	//
	void FirePendingEventsAndClear();

protected:
	//
	virtual void Fire(tjs_uint n) = 0;

	//
	virtual void CancelEvents() = 0;
};
//---------------------------------------------------------------------------
// tTVPTimerThread
//---------------------------------------------------------------------------
class tTVPTimerThread : public tTVPThread
{
	// thread for triggering punctual event.
	// normal Windows timer cannot call the timer callback routine at
	// too short interval ( roughly less than 50ms ).

	std::vector<tTVPTimerBase *> List;
	std::vector<tTVPTimerBase *> Pending; // timer object which has pending events
	std::vector<tTVPTimerBase *> ProcWork;
	bool PendingEventsAvailable;
	tTVPThreadEvent Event;
	
	NativeEventQueue<tTVPTimerThread> EventQueue;

public:

	tTJSCriticalSection TVPTimerCS;

	tTVPTimerThread();
	~tTVPTimerThread();

protected:
	void Execute();

private:
	void Proc( NativeEvent& event );

	void AddItem(tTVPTimerBase * item);
	bool RemoveItem(tTVPTimerBase *item);
	void RemoveFromPendingItem(tTVPTimerBase *item);
	void RegisterToPendingItem(tTVPTimerBase *item);
#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
	void InternalTrigger();
#endif

public:
	void SetEnabled(tTVPTimerBase *item, bool enabled); // managed by this class
	void SetInterval(tTVPTimerBase *item, tjs_uint64 interval); // managed by this class

public:
	static void Init();
	static void Uninit();

	static void Add(tTVPTimerBase * item);
	static void Remove(tTVPTimerBase *item);

	static void RemoveFromPending(tTVPTimerBase *item);
	static void RegisterToPending(tTVPTimerBase *item);

#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
	static void Trigger();
#endif
};
//---------------------------------------------------------------------------
#endif
