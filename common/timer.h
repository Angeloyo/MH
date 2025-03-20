#pragma once
#include <chrono>
#include <string>

/**
 * A simple timer class to measure execution time.
 */
class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    bool running;

public:
    /**
     * Constructor: creates the timer in stopped state.
     */
    Timer() : running(false) {}

    /**
     * Starts the timer.
     */
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    /**
     * Stops the timer.
     */
    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

    /**
     * Returns the elapsed time in seconds.
     * 
     * @return Elapsed time in seconds
     */
    double elapsed() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        std::chrono::duration<double> diff = end - start_time;
        return diff.count();
    }

    /**
     * Resets the timer to its initial state.
     */
    void reset() {
        running = false;
    }
}; 