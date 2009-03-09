#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
  protected:
    int _startTicks;
    unsigned int _fps;
    bool _started;

  public:
    /*!
    * @brief ctor
    */
    Timer(const unsigned int fps): _startTicks(0), _fps(fps), _started(false) {}

    /*!
    * @brief dtor
    */
    ~Timer() {};

    /*!
    * @brief Start the timer
    */
    void Start();

    /*!
    * @brief Stop the timer
    */
    void Stop() { _started = false; }

    /*!
    * @brief Is FPS Reached
    */
    bool IsFPSReached() const;
};

#endif
