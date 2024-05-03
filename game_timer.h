

struct game_timer
{
    real64 SecondsPerCount;
    real64 DeltaTime;

    int64 BaseTime;
    int64 PausedTime;
    int64 StopTime;
    int64 PrevTime;
    int64 CurrTime;

    bool Stopped;

};