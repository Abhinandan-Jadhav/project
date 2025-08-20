#include "storage.h"
#include <algorithm>

void Storage::add_alert(Alert a) {
    std::lock_guard<std::mutex> lock(m_);
    alerts_.push_back(std::move(a));
    if (alerts_.size() > 500) {
        alerts_.erase(alerts_.begin(), alerts_.begin() + (alerts_.size() - 500));
    }
}

std::vector<Alert> Storage::recent_alerts(size_t max_count) {
    std::lock_guard<std::mutex> lock(m_);
    std::vector<Alert> out = alerts_;
    std::sort(out.begin(), out.end(), [](const Alert& a, const Alert& b){
        return a.ts > b.ts;
    });
    if (out.size() > max_count) out.resize(max_count);
    return out;
}
