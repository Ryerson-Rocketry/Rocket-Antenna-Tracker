#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

constexpr float EARTH_RADIUS = 6371000.0f;  // Earth's radius in meters
constexpr float DEG_TO_RAD = M_PI / 180.0f; // Precomputed conversion factor

// Rocket and ground station coordinates
float rocketLat, rocketLon, rocketAlt;
constexpr float gsLat = 32.94608;
constexpr float gsLon = -106.92175;
constexpr float gsAlt = 4546.1;

// Function to parse a CSV string into latitude, longitude, altitude
bool parseCoordinates(const std::string& data) {
    std::istringstream ss(data);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 9) {
        std::cerr << "Error: Malformed data\n";
        return false;
    }

    try {
        rocketLat = std::stof(tokens[6]);  // TRACKER Lat
        rocketLon = std::stof(tokens[7]);  // TRACKER Lon
        rocketAlt = std::stof(tokens[8]);  // TRACKER Alt
    } catch (const std::exception& e) {
        std::cerr << "Error parsing coordinates: " << e.what() << std::endl;
        return false;
    }
    return true;
}

// Optimized azimuth calculation
inline float calculateAzimuth(float lat1, float lon1, float lat2, float lon2) {
    lat1 *= DEG_TO_RAD;
    lon1 *= DEG_TO_RAD;
    lat2 *= DEG_TO_RAD;
    lon2 *= DEG_TO_RAD;

    float dLon = lon2 - lon1;
    float y = std::sin(dLon) * std::cos(lat2);
    float x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dLon);
    float azimuth = std::atan2(y, x) * (180.0f / M_PI);

    return (azimuth < 0) ? azimuth + 360.0f : azimuth;
}

// Optimized elevation calculation
inline float calculateElevation(float lat1, float lon1, float alt1, float lat2, float lon2, float alt2) {
    lat1 *= DEG_TO_RAD;
    lon1 *= DEG_TO_RAD;
    lat2 *= DEG_TO_RAD;
    lon2 *= DEG_TO_RAD;

    float dLat = lat2 - lat1;
    float dLon = lon2 - lon1;

    float a = std::sin(dLat / 2) * std::sin(dLat / 2) +
              std::cos(lat1) * std::cos(lat2) *
              std::sin(dLon / 2) * std::sin(dLon / 2);

    float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    float distance = EARTH_RADIUS * c;

    return std::atan2(alt2 - alt1, distance) * (180.0f / M_PI);
}

int main() {
    std::string input;
    std::cout << "Enter CSV data: ";
    
    while (std::getline(std::cin, input)) {
        if (!parseCoordinates(input)) continue;

        float azimuth = calculateAzimuth(gsLat, gsLon, rocketLat, rocketLon);
        float elevation = calculateElevation(gsLat, gsLon, gsAlt, rocketLat, rocketLon, rocketAlt);

        std::cout << "Azimuth: " << azimuth << "° | Elevation: " << elevation << "°\n";
    }
    return 0;
}
