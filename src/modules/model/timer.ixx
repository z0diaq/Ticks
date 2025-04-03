/*
 * Ticks - Timers, Events and GUI
 * Copyright (C) 2025 Piotr Tkaczyk
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
export module model.timer;

import <chrono>;
import <string>;
import <functional>;
import <optional>;
import <iomanip>;
import <sstream>;

/**
 * @brief A functional timer implementation with immutable state
 */
export class Timer {
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using Duration = std::chrono::seconds;
    using Callback = std::function<void()>;
    
    // Constructor
    explicit Timer(int durationSeconds, 
                  std::optional<Callback> onCompleteCallback = std::nullopt);
    
    // Start the timer
    void start();
    
    // Stop the timer
    void stop();
    
    // Reset the timer
    void reset();
    
    // Update the timer - to be called periodically
    void update();
    
    // Get time remaining as string (MM:SS)
    [[nodiscard]] std::string getRemainingTimeString() const;
    
    // Get time remaining in seconds
    [[nodiscard]] int getRemainingSeconds() const;
    
    // Get estimated time of completion
    [[nodiscard]] TimePoint getETA() const;
    
    // Check if timer is running
    [[nodiscard]] bool isRunning() const noexcept;
    
    // Check if timer is completed
    [[nodiscard]] bool isCompleted() const noexcept;
    
    // Functional setter for duration
    [[nodiscard]] Timer withDuration(int newDurationSeconds) const;
    
private:
    Duration totalDuration_;
    Duration remainingDuration_;
    TimePoint startTime_;
    TimePoint endTime_;
    bool isRunning_;
    bool isCompleted_;
    std::optional<Callback> onCompleteCallback_;
};

// Implementation
Timer::Timer(int durationSeconds, std::optional<Callback> onCompleteCallback)
    : totalDuration_(std::chrono::seconds(durationSeconds)),
      remainingDuration_(totalDuration_),
      isRunning_(false),
      isCompleted_(false),
      onCompleteCallback_(std::move(onCompleteCallback)) {}

void Timer::start() {
    if (!isRunning_ && !isCompleted_) {
        isRunning_ = true;
        startTime_ = std::chrono::system_clock::now();
        endTime_ = startTime_ + remainingDuration_;
    }
}

void Timer::stop() {
    if (isRunning_) {
        isRunning_ = false;
        auto now = std::chrono::system_clock::now();
        remainingDuration_ = std::chrono::duration_cast<Duration>(endTime_ - now);
        if (remainingDuration_.count() <= 0) {
            remainingDuration_ = Duration(0);
            isCompleted_ = true;
            if (onCompleteCallback_) {
                (*onCompleteCallback_)();
            }
        }
    }
}

void Timer::reset() {
    isRunning_ = false;
    isCompleted_ = false;
    remainingDuration_ = totalDuration_;
}

void Timer::update() {
    if (isRunning_ && !isCompleted_) {
        auto now = std::chrono::system_clock::now();
        if (now >= endTime_) {
            remainingDuration_ = Duration(0);
            isCompleted_ = true;
            isRunning_ = false;
            if (onCompleteCallback_) {
                (*onCompleteCallback_)();
            }
        } else {
            remainingDuration_ = std::chrono::duration_cast<Duration>(endTime_ - now);
        }
    }
}

std::string Timer::getRemainingTimeString() const {
    auto seconds = getRemainingSeconds();
    auto minutes = seconds / 60;
    seconds %= 60;
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

int Timer::getRemainingSeconds() const {
    return static_cast<int>(remainingDuration_.count());
}

Timer::TimePoint Timer::getETA() const {
    if (isRunning_) {
        return endTime_;
    }
    return std::chrono::system_clock::now() + remainingDuration_;
}

bool Timer::isRunning() const noexcept {
    return isRunning_;
}

bool Timer::isCompleted() const noexcept {
    return isCompleted_;
}

Timer Timer::withDuration(int newDurationSeconds) const {
    return Timer(newDurationSeconds, onCompleteCallback_);
}