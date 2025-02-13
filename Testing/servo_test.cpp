#include <math.h>
#include <iostream>
#include <string>

// Rocket coordinates and ground station coordinates
float rocketLat, rocketLon, rocketAlt;
float gsLat = 32.94608;
float gsLon = -106.92175;
float gsAlt = 4546.1;

// Conversion factor
const float EARTH_RADIUS = 6371000;  // in meters
/*
void parseCoordinates(const std::string &data) {
    size_t gsLatIndex = nthIndexOf(data, ',', 3) + 1;
    size_t gsLonIndex = nthIndexOf(data, ',', 4) + 1;
    size_t gsAltIndex = nthIndexOf(data, ',', 5) + 1;
    size_t trackerLatIndex = nthIndexOf(data, ',', 6) + 1;
    size_t trackerLonIndex = nthIndexOf(data, ',', 7) + 1;
    size_t trackerAltIndex = nthIndexOf(data, ',', 8) + 1;

    size_t nextComma;

    // Extract tracker latitude
    nextComma = data.find(',', trackerLatIndex);
    rocketLat = std::stof(data.substr(trackerLatIndex, nextComma - trackerLatIndex));

    // Extract tracker longitude
    trackerLonIndex = nextComma + 1;
    nextComma = data.find(',', trackerLonIndex);
    rocketLon = std::stof(data.substr(trackerLonIndex, nextComma - trackerLonIndex));

    // Extract tracker altitude
    trackerAltIndex = nextComma + 1;
    nextComma = data.find(',', trackerAltIndex);
    rocketAlt = std::stof(data.substr(trackerAltIndex, nextComma - trackerAltIndex));
} */

void parseCoordinates(const std::string &data) {
    try {
        if (std::count(data.begin(), data.end(), ',') < 8) {
            throw std::runtime_error("Invalid data format");
        }
        
        size_t trackerLatIndex = nthIndexOf(data, ',', 6) + 1;
        size_t trackerLonIndex = nthIndexOf(data, ',', 7) + 1;
        size_t trackerAltIndex = nthIndexOf(data, ',', 8) + 1;

        rocketLat = std::stof(data.substr(trackerLatIndex, data.find(',', trackerLatIndex) - trackerLatIndex));
        rocketLon = std::stof(data.substr(trackerLonIndex, data.find(',', trackerLonIndex) - trackerLonIndex));
        rocketAlt = std::stof(data.substr(trackerAltIndex, data.find(',', trackerAltIndex) - trackerAltIndex));
    } catch (const std::exception &e) {
        std::cerr << "Error parsing coordinates: " << e.what() << std::endl;
    }
}


size_t nthIndexOf(const std::string &str, char ch, int n) {
    size_t index = -1;
    while (n-- > 0) {
        index = str.find(ch, index + 1);
        if (index == std::string::npos) {
            break;
        }
    }
    return index;
}

float calculateAzimuth(float lat1, float lon1, float lat2, float lon2) {
    // Convert degrees to radians
    lat1 = lat1 * M_PI / 180;
    lon1 = lon1 * M_PI / 180;
    lat2 = lat2 * M_PI / 180;
    lon2 = lon2 * M_PI / 180;

    float dLon = lon2 - lon1;
    float y = sin(dLon) * cos(lat2);
    float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    float azimuth = atan2(y, x);

    // Convert radians to degrees and normalize to 0-360
    azimuth = azimuth * 180 / M_PI;
    if (azimuth < 0) {
        azimuth += 360;
    }
    return azimuth;
}

float calculateElevation(float lat1, float lon1, float alt1, float lat2, float lon2, float alt2) {
    // Calculate the distance between the two points
    float dLat = (lat2 - lat1) * M_PI / 180;
    float dLon = (lon2 - lon1) * M_PI / 180;

    float a = sin(dLat / 2) * sin(dLat / 2) +
              cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) *
              sin(dLon / 2) * sin(dLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float distance = EARTH_RADIUS * c;

    // Calculate elevation angle
    float heightDifference = alt2 - alt1;
    float elevation = atan2(heightDifference, distance);

    // Convert radians to degrees
    return elevation * 180 / M_PI;
}

int main() {
    std::string data;
    while (true) {
        std::cout << "Enter telemetry data (format: TRACKER,DATE,TIME,GS Lat,GS Lon,GS Alt asl,TRACKER Lat,TRACKER Lon,TRACKER Alt asl,...): ";
        std::getline(std::cin, data);

        parseCoordinates(data);

        // Calculate angles for simulated servos
        float azimuth = calculateAzimuth(gsLat, gsLon, rocketLat, rocketLon);
        float elevation = calculateElevation(gsLat, gsLon, gsAlt, rocketLat, rocketLon, rocketAlt);

        // Output calculated angles
        std::cout << "Calculated Azimuth: " << azimuth << " degrees" << std::endl;
        std::cout << "Calculated Elevation: " << elevation << " degrees" << std::endl;
    }

    return 0;
}
