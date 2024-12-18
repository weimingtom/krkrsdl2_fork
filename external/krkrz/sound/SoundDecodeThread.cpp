//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Sound Decode Thead for QueueSoundBuffer
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include "MsgIntf.h"
#include "SoundDecodeThread.h"
#include "QueueSoundBufferImpl.h"
#include "SoundSamples.h"

//---------------------------------------------------------------------------
static const tTVPThreadPriority TVPDecodeThreadHighPriority = ttpHigher;
//---------------------------------------------------------------------------
tTVPSoundDecodeThread::tTVPSoundDecodeThread( tTJSNI_QueueSoundBuffer * owner )
 : Owner(owner), DecodedSamples(0) {
	StartTread();
}
//---------------------------------------------------------------------------
tTVPSoundDecodeThread::~tTVPSoundDecodeThread() {
	Terminate();
	ClearQueue();
	Event.Set();
	WaitFor();
}
//---------------------------------------------------------------------------
void tTVPSoundDecodeThread::Execute(void) {
	SetPriority(TVPDecodeThreadHighPriority);
	while( !GetTerminated() ) {
		tjs_uint32 count = 0;
		{
			tTJSCriticalSectionHolder cs_holder(OneLoopCS);
			count = Samples.size();
			if( count ) {
				//
				auto itr = Samples.begin();
				tTVPSoundSamplesBuffer* buf = *itr;
				buf->Decode();
				buf->SetDecodePosition( DecodedSamples );
				DecodedSamples += buf->GetInSamples();

				//
				Owner->PushPlayStream( buf );
				Samples.erase( itr );
				count = Samples.size();
			}
		}

		if( GetTerminated() ) break;

		if( count == 0 ) {
			// buffer is empty; sleep infinite
			Event.WaitFor(0);
		} else {
			// buffer is not full; sleep shorter
			//
			//
			std::this_thread::yield();
		}
	}
}
//---------------------------------------------------------------------------
void tTVPSoundDecodeThread::Interrupt() {
	ClearQueue();
}
//---------------------------------------------------------------------------
void tTVPSoundDecodeThread::StartDecoding( tjs_int64 predecoded ) {
	DecodedSamples = predecoded;
	Event.Set();
}
//---------------------------------------------------------------------------
void tTVPSoundDecodeThread::PushSamplesBuffer( tTVPSoundSamplesBuffer* buf ) {
	//
	tTJSCriticalSectionHolder cs_holder(OneLoopCS);
	Samples.push_back(buf);
	Event.Set();
}
//---------------------------------------------------------------------------
void tTVPSoundDecodeThread::ClearQueue() {
	//
	tTJSCriticalSectionHolder cs_holder(OneLoopCS);
	Samples.clear();
}
//---------------------------------------------------------------------------
