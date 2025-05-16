#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

int main() {
    std::ofstream log("/tmp/logtimer.log", std::ios::app);
    int counter = 0;
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        log << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
            << " Count: " << counter++ << std::endl;
        log.flush();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
