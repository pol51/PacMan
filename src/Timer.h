#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
  protected:
    int _startTicks = 0;
    unsigned int _fps = 30;
    bool _started = false;

  public:
    /*!
    * @brief ctor
    */
    Timer(const unsigned int fps): _fps(fps) {}

    /*!
    * @brief Start the timer
    */
    void Start();

    /*!
    * @brief Stop the timer
    */
    void Stop()
    {
      _started = false;
    }

    /*!
    * @brief Is FPS Reached
    */
    bool IsFPSReached() const;
};

#endif
