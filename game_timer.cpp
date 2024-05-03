

#include "game_timer.h"

internal void
ResetTimer(game_timer *GameTimer)
{
    int64 CurrTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);

    GameTimer->BaseTime = CurrTime;
    GameTimer->PrevTime = CurrTime;
    GameTimer->StopTime = 0;
    GameTimer->Stopped = false;
}

internal void
GameTimerTick(game_timer *GameTimer)
{
    if(GameTimer->Stopped)
    {
        GameTimer->DeltaTime = 0.0;
        return;
    }

    // get the time this frame
    int64 CurrTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
    GameTimer->CurrTime = CurrTime;

    // time diff between this frame and the previous
    GameTimer->DeltaTime = (GameTimer->CurrTime - GameTimer->PrevTime) * 
        GameTimer->SecondsPerCount;
    
    // preprare for next frame
    GameTimer->PrevTime = CurrTime;

    // force nonnegative
    if(GameTimer->DeltaTime < 0.0)
    {
        GameTimer->DeltaTime = 0.0;
    }

}

internal void
GameTimerStart(game_timer *GameTimer)
{
    int64 StartTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime   

    if(GameTimer->Stopped)
    {
        GameTimer->PausedTime += (StartTime - GameTimer->StopTime);

        GameTimer->PrevTime = StartTime;
        GameTimer->StopTime = 0;
        GameTimer->Stopped = false;
    }
}

internal void
GameTimerStop(game_timer *GameTimer)
{
    if(!GameTimer->Stopped)
    {
         int64 CurrTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);

        GameTimer->StopTime = CurrTime;
        GameTimer->Stopped = true;
    }
}

internal real32
GameTimerTotalTime(game_timer *GameTimer)
{
    	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

    if(GameTimer->Stopped)
    {
        return (real32)(((GameTimer->StopTime-GameTimer->PausedTime)- GameTimer->BaseTime) *
            GameTimer->SecondsPerCount);
    }
    
	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
    else
    {
        return (real32)(((GameTimer->CurrTime-GameTimer->PausedTime)-GameTimer->BaseTime) *
            GameTimer->SecondsPerCount);
    }

}