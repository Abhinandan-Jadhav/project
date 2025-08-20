# Campus Emergency Help Button — README

## Overview

This project provides a **campus emergency alert system** with:
- A big red “Help” button (web frontend) for reporting accidents, harassment, theft, or emergencies.
- A C++ backend server that records alerts, simulates notifications to nearby registered users, and displays recent alerts in real time.

**Demo features:**
- Pressing the button sends an alert to the server.
- Nearby "responders" (mocked users) are notified via simulated email/SMS log output.
- Recent alerts show live in the browser.

***

## Structure

```
project/
  server/
    main.cpp
    httplib.h        # Single-header HTTP server
    notifier.h/.cpp  # Notification simulation
    storage.h/.cpp   # Alert storage (in-memory)
    utils.h/.cpp     # Helpers (ID generation)
    CMakeLists.txt
  web/
    index.html       # Main web page (Help button + alerts)
    app.js           # Frontend logic
    styles.css       # UI styling
  README.md
```

***

## Quick Start

### Requirements

- **C++17** or newer
- **CMake** (for building)
- **nlohmann/json.hpp** header (add to server/ if using full JSON features)
- Linux/macOS/Windows

### Build & Run

1. **Clone or copy the project.**

2. **Initialize build:**
   ```bash
   cd server
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run server:**
   ```bash
   ./server
   # Output: Server running at http://0.0.0.0:8080
   ```

4. **Open the web client:**
   - Use browser: [http://localhost:8080](http://localhost:8080)
   - You will see the Help button and recent alerts.

***

## Usage

### Reporting an Emergency

1. Select the type (“Accident”, “Harassment”, “Theft”, “Other”).
2. Optionally enter a description.
3. Click **HELP** (the red button).
4. Alerts appear in the recent list; notifications are “sent” to nearby registered users (output to console).

### Responders

- Simulated users are registered in the server code (`NearbyUser` list in `main.cpp`).
- When an alert is triggered, responders receive simulated email/SMS (server console log).

***

## Code Highlights

- **Frontend:** HTML/CSS/JS. Geolocation-enabled (if permitted), shows list of alerts.
- **Backend:** C++ using `httplib.h` for HTTP, `storage.h` for alerts, `notifier.h` for notification simulation.
- **API endpoints:**
  - `POST /api/panic` — Send alert.
  - `GET /api/alerts` — List recent alerts.

***

## Extending This Project

- **Add user authentication** (SSO/JWT) for secure alerting.
- **Notify real users** (SMTP/Twilio) instead of console simulation.
- **Persist alerts** (SQLite/PostgreSQL).
- **Add audit logs, geo-fencing, rate limits, HTTPS, etc.**

***

## Security Disclaimer

This demonstration project is for educational purposes.  
It is **not secure for production**; do not use it for actual emergency management without significant enhancements (auth, encryption, real-time reliability, and more).

***

## License

MIT (see LICENSE file, if provided).  
Third-party libraries (httplib.h, nlohmann/json.hpp) are separately licensed.

***

## Contact

Questions, issues, or feedback?  
Create an issue or email the project owner.

***

**Stay safe, and build responsibly!**