/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_TYPES_TIMER_H
#define VCL_TYPES_TIMER_H

#include <chrono>
#include <iostream>
#include <string>

namespace vcl {

/**
 * @brief The Timer class allows to instantiate simple Timer objects that can be
 * used everywhere.
 *
 * When a Timer object is create, by default the timer starts (you can change
 * this option using the bool argument of the Timer constructor). To each timer
 * can be associated a caption, and the printed/returned values are expressed in
 * seconds.
 */
class Timer
{
    std::string mCaption   = "Timer";
    bool        mIsStopped = false;

    std::chrono::high_resolution_clock::time_point mBegin, mEnd;

public:
    /**
     * @brief Creates a timer with the caption "Timer". If the given boolean is
     * true, the timer starts.
     *
     * @param[in] _start: if true, the timer will start when the object is
     * created.
     */
    Timer(bool _start = true)
    {
        if (_start)
            start();
    }

    /**
     * @brief Creates a timer with the given caption. If the given boolean is
     * true, the timer starts.
     *
     * @param[in] caption: the caption of the timer.
     * @param[in] _start: if true, the timer will start when the object is
     * created.
     */
    Timer(const char* caption, bool _start = true) : mCaption(caption)
    {
        if (_start)
            start();
    }

    /**
     * @brief Creates a timer with the given caption. If the given boolean is
     * true, the timer starts.
     *
     * @param[in] caption: the caption of the timer.
     * @param[in] _start: if true, the timer will start when the object is
     * created.
     */
    Timer(const std::string& caption, bool _start = true) : mCaption(caption)
    {
        if (_start)
            start();
    }

    /**
     * @brief Starts the timer.
     */
    void start() {
        mBegin = std::chrono::high_resolution_clock::now();
        mIsStopped = false;
    }

    /**
     * @brief Stops the timer and prints the time passed between the call of
     * start() and this member function. Printed time is expressed in seconds.
     */
    void stopAndPrint()
    {
        stop();
        print();
    }

    /**
     * @brief Stops the timer.
     */
    void stop()
    {
        mEnd       = std::chrono::high_resolution_clock::now();
        mIsStopped = true;
    }

    /**
     * @brief Prints the time passed between the call of start() and this member
     * function. Works also if the timer is not stopped. Printed time is
     * expressed in seconds.
     */
    void print() const
    {
        double secs = delay();
        int    mins = (int) secs / 60;
        if (mins == 0)
            std::cout << "[" << secs << " secs]\t" << mCaption << std::endl;
        else {
            secs      = secs - mins * 60;
            int hours = mins / 60;
            if (hours == 0)
                std::cout << "[" << mins << " mins; " << secs << " secs]\t"
                          << mCaption << std::endl;
            else {
                mins = mins - hours * 60;
                std::cout << "[" << hours << " hours; " << mins << " mins; "
                          << secs << " secs]\t" << mCaption << std::endl;
            }
        }
    }

    /**
     * @brief Returns the time passed between the call of start() and this
     * member function. Works also if the timer is not stopped. Returned time is
     * expressed in seconds.
     */
    double delay() const
    {
        double secs;
        if (mIsStopped) {
            secs =
                (double) (std::chrono::duration_cast<std::chrono::microseconds>(
                              mEnd - mBegin)
                              .count()) /
                1000000;
        }
        else {
            std::chrono::high_resolution_clock::time_point s =
                std::chrono::high_resolution_clock::now();
            secs =
                (double) (std::chrono::duration_cast<std::chrono::microseconds>(
                              s - mBegin)
                              .count()) /
                1000000;
        }
        return secs;
    }

    /**
     * @brief Sets the caption of the timer.
     * @param[in] caption: the new caption of the timer.
     */
    void setCaption(const std::string& caption) { mCaption = caption; }
};

} // namespace vcl

#endif // VCL_TYPES_TIMER_H
