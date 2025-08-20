#pragma once
#include <string>
#include <vector>
#include <iostream>

struct NearbyUser {
    std::string name;
    std::string email;
    std::string phone;
    double lat;
    double lon;
};

class Notifier {
public:
    Notifier(std::vector<NearbyUser> users) : users_(std::move(users)) {}

    void notify(const std::string& type,
                const std::string& description,
                double lat, double lon) {
        // In production, filter by geofence/radius here.
        for (const auto& u : users_) {
            // Simulate email
            std::cout << "[EMAIL] To: " << u.email
                      << " | Subject: Campus Emergency (" << type << ")\n"
                      << "Message: " << description
                      << " | Location: (" << lat << ", " << lon << ")\n\n";
            // Simulate SMS
            std::cout << "[SMS] To: " << u.phone
                      << " | " << type << " | " << description
                      << " @ (" << lat << ", " << lon << ")\n\n";
        }
    }

private:
    std::vector<NearbyUser> users_;
};
