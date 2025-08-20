#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <chrono>

struct Alert {
    std::string id;
    std::string type;        // "accident" | "harassment" | "theft" | "other"
    std::string description;
    double lat;
    double lon;
    std::chrono::system_clock::time_point ts;
};

class Storage {
public:
    void add_alert(Alert a);
    std::vector<Alert> recent_alerts(size_t max_count = 50);

private:
    std::vector<Alert> alerts_;
    std::mutex m_;
};
