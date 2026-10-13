// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_TIMER_H
#define VCL_BASE_TIMER_H

#include <chrono>
#include <iostream>
#include <print>
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
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    std::string mCaption   = "Timer";
    bool        mIsStopped = false;
    TimePoint   mBegin, mEnd;

public:
    /**
     * @brief Creates a timer with the given caption. If the given boolean is
     * true, the timer starts.
     *
     * @param[in] caption: the caption of the timer.
     * @param[in] start: if true, the timer will start when the object is
     * created.
     */
    explicit Timer(std::string caption = "Timer", bool start = true) :
            mCaption(std::move(caption))
    {
        if (start)
            this->start();
    }

    /**
     * @brief Creates a timer with the caption "Timer". If the given boolean is
     * true, the timer starts.
     *
     * @param[in] start: if true, the timer will start when the object is
     * created.
     */
    explicit Timer(bool start) : Timer(std::string("Timer"), start) {}

    /**
     * @brief Returns the caption of the timer.
     *
     * @return Reference to the caption of the timer.
     */
    decltype(auto) caption(this auto&& self) { return (self.mCaption); }

    /**
     * @brief Starts the timer.
     */
    void start()
    {
        mBegin     = Clock::now();
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
        mEnd       = Clock::now();
        mIsStopped = true;
    }

    /**
     * @brief Returns the time passed between the call of start() and this
     * member function. Works also if the timer is not stopped. Returned time is
     * expressed in seconds.
     */
    double delay() const
    {
        const auto end = mIsStopped ? mEnd : Clock::now();
        return std::chrono::duration<double>(end - mBegin).count();
    }

    /**
     * @brief Returns a string representation of the timer's elapsed time and
     * caption.
     *
     * The format of the returned string is:
     * - If less than 1 minute: "[X.XXX secs]    caption"
     * - If less than 1 hour: "[Y mins; X.XXX secs]    caption"
     * - If 1 hour or more: "[Z hours; Y mins; X.XXX secs]    caption"
     *
     * @return A formatted string representing the timer's elapsed time and
     * caption.
     */
    std::string toString() const
    {
        double secs = delay();
        int    mins = static_cast<int>(secs) / 60;
        if (mins == 0) {
            return std::format("[{:.3f} secs]\t{}", secs, mCaption);
        }
        secs -= mins * 60;
        int hours = mins / 60;
        if (hours == 0) {
            return std::format(
                "[{} mins; {:.3f} secs]\t{}", mins, secs, mCaption);
        }
        mins -= hours * 60;
        return std::format(
            "[{} hours; {} mins; {:.3f} secs]\t{}",
            hours,
            mins,
            secs,
            mCaption);
    }

    /**
     * @brief Prints the time passed between the call of start() and this member
     * function. Works also if the timer is not stopped. Printed time is
     * expressed in seconds.
     */
    void print() const { std::println("{}", toString()); }

    /**
     * @brief Sets the caption of the timer.
     * @param[in] caption: the new caption of the timer.
     */
    void setCaption(const std::string& caption) { mCaption = caption; }
};

} // namespace vcl

#endif // VCL_BASE_TIMER_H
