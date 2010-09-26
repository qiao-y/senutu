#pragma once
#ifndef _IDECODER_H
#define _IDECODER_H

#include "aerror.h"
#include "XAudio2Manip.h"

//This struct defines the tag. e.g. ID3, APE tag, WMA tag...
class TAG
{
public :
	string Title;
	string Artist;
	string Album;
	string Year;
	string Comment;
	string Genre;
	string TotalTime;
	TAG() {}
	TAG(string title, string artist, string album, string year, string comment, string genre,string totalTime):
			Title(title),Artist(artist),Album(album),Comment(comment),Genre(genre), TotalTime(totalTime)  {}
};



//-----------------------------------------------------------------------------
// Name: class IDecoder
// Desc: the base class of all decoders
//-----------------------------------------------------------------------------
class IDecoder{
public:
    IDecoder() :m_pwfx(NULL),m_bIsPlaying(false),m_bIsPause(false),m_bIsStop(true) {}

	virtual ~IDecoder() {SAFE_DELETE(m_pwfx);}

    virtual ARESULT Open( LPWSTR strFileName) = 0;
    virtual ARESULT Play();
    virtual ARESULT Pause();
    virtual ARESULT Stop() = 0;//the file is still open, use |close()| to release the file;
    virtual ARESULT Sync(int minBufTime=500,int MaxBufTime=1000,int BufferSpan=DEFAULT_BUFFER_SPAN)=0;
    //time unit is millisecond
    
	virtual int GetFullTime() = 0; //return the full time, in ms
	virtual string GetFullTimeSerialized() { return MilliSecondToString(GetFullTime()) ;} //return the full time, hh:mm:ss.ms
	
	virtual ARESULT SetCurTime(int time) = 0;
	
	virtual int GetCurTime() = 0; //return the current time, in ms
	virtual string GetCurTimeSeralized(){ return MilliSecondToString(GetCurTime());} //return the current time, hh:mm:ss.ms

	virtual bool isPlaying() {return m_bIsPlaying;}
	virtual bool isPause() {return m_bIsPause;}
    virtual ARESULT Close() = 0;//release the file

	virtual float GetVolume();
	virtual ARESULT SetVolume(float theVolume);
    
	virtual TAG GetTag() {
		return TAG();
	}


protected:
    WAVEFORMATEX * m_pwfx;       //pointer to waveformatex structure
    
    //see Xaudio2Manip.h for detail
    XAudio2Manip XACtrl;     //an instance of XAudio2Manip

	//playing status
    volatile bool m_bIsPlaying;
    volatile bool m_bIsPause;
    volatile bool m_bIsStop;

private:
	//helper function to change the display format of time 
	string MilliSecondToString(int ms);


};



#endif //_IDECODER_H