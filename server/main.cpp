#include "httplib.h"    // single-header server
#include "storage.h"
#include "notifier.h"
#include "utils.h"

#include <nlohmann/json.hpp> // Optional: if you want to bundle header-only JSON; else basic parsing.
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using json = nlohmann::json;

static std::string iso8601(std::chrono::system_clock::time_point tp) {
    auto t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return buf;
}

int main() {
    // Initialize mock nearby registered users
    std::vector<NearbyUser> users = {
        {"Security Desk A", "securityA@campus.edu", "+91-9876543210", 15.8497, 74.4977},
        {"Patrol Team B",   "patrolB@campus.edu",   "+91-9876500000", 15.8490, 74.4985},
        {"Warden C",        "wardenC@campus.edu",   "+91-9876511111", 15.8502, 74.4969}
    };

    Storage storage;
    Notifier notifier(users);

    httplib::Server svr;

    // Serve static files
    svr.Get("/", [&](const httplib::Request&, httplib::Response& res) {
        std::ifstream f("../web/index.html");
        std::stringstream buf;
        buf << f.rdbuf();
        res.set_content(buf.str(), "text/html");
    });

    svr.Get("/app.js", [&](const httplib::Request&, httplib::Response& res) {
        std::ifstream f("../web/app.js");
        std::stringstream buf;
        buf << f.rdbuf();
        res.set_content(buf.str(), "application/javascript");
    });

    svr.Get("/styles.css", [&](const httplib::Request&, httplib::Response& res) {
        std::ifstream f("../web/styles.css");
        std::stringstream buf;
        buf << f.rdbuf();
        res.set_content(buf.str(), "text/css");
    });

    // API: GET recent alerts
    svr.Get("/api/alerts", [&](const httplib::Request& req, httplib::Response& res) {
        auto list = storage.recent_alerts(50);
        json arr = json::array();
        for (const auto& a : list) {
            arr.push_back({
                {"id", a.id},
                {"type", a.type},
                {"description", a.description},
                {"lat", a.lat},
                {"lon", a.lon},
                {"timestamp", iso8601(a.ts)}
            });
        }
        res.set_content(arr.dump(), "application/json");
    });

    // API: POST panic
    // Body JSON: { "type": "accident|harassment|theft|other", "description": "...", "lat": 15.85, "lon": 74.49 }
    svr.Post("/api/panic", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto j = json::parse(req.body);

            std::string type = j.value("type", "other");
            std::string description = j.value("description", "Emergency");
            double lat = j.value("lat", 0.0);
            double lon = j.value("lon", 0.0);

            Alert a;
            a.id = gen_id();
            a.type = type;
            a.description = description;
            a.lat = lat;
            a.lon = lon;
            a.ts = std::chrono::system_clock::now();

            storage.add_alert(a);
            notifier.notify(type, description, lat, lon);

            json resp = {
                {"status", "ok"},
                {"id", a.id}
            };
            res.set_content(resp.dump(), "application/json");
        } catch (const std::exception& e) {
            json err = { {"status", "error"}, {"message", e.what()} };
            res.status = 400;
            res.set_content(err.dump(), "application/json");
        }
    });

    // CORS for simplicity (adjust origin for production)
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Headers", "Content-Type"},
        {"Access-Control-Allow-Methods", "GET,POST,OPTIONS"}
    });
    svr.Options(R"(\.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    const char* host = "0.0.0.0";
    int port = 8080;
    printf("Server running at http://%s:%d\n", host, port);
    svr.listen(host, port);
    return 0;
}
