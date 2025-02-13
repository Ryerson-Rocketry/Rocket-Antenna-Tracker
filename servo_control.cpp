#include <Servo.h>
#include <math.h>

Servo servoX;s
Servo servoY;

const int servoXPin = 9;
const int servoYPin = 10;

float rocketLat, rocketLon, rocketAlt;

// replace these with actual groundstaion coords of launch site
float gsLat = 43.6593;
float gsLon = 79.3804;
float gsAlt = 79.4232;

const float EARTH_RADIUS = 6371000; // meters

void setup(){
    servoX.attach(servoXPin);
    servoY.attach(servoYPin);

    Serial.begin(9600);
}

void read_data() {

    if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        parseCoordinates(data);
    

    // calculate the required angles for tracking
    float azimuth = calculateAzimuth(gsLat, gsLon, rocketLat, rocketLon);
    float elevation = calculateElevation(gsLat, gsLon, gsAlt, rocketLat, rocketLon, rocketAlt);
    
    // must convert to be within the servo range of motion
    int servoX_angle = constrain(map(azimuth, 0, 360, 0, 180), 0, 180);
    int servoY_angle = constrain(map(elevation, 0, 90, 0, 180), 0, 180);

    // change servo position in order to angle towards the rocket
    servoX.write(servoX_angle);
    servoY.write(servoY_angle);
    }
}

void parseCoordinates(String data) {
  // Parse the incoming data for rocket coordinates (latitude, longitude, altitude)
    int gsLatIndex = nthIndexOf(data, ',', 3) + 1;
    int gsLonIndex = nthIndexOf(data, ',', 4) + 1;
    int gsAltIndex = nthIndexOf(data, ',', 5) + 1;
    int trackerLatIndex = nthIndexOf(data, ',', 6) + 1;
    int trackerLonIndex = nthIndexOf(data, ',', 7) + 1;
    int trackerAltIndex = nthIndexOf(data, ',', 8) + 1;

    int nextComma;

    // lat
    nextComma = data.indexOf(',', trackerLatIndex);
    rocketLat = data.substring(trackerLatIndex, nextComma).toFloat();

    // lon
    trackerLonIndex = nextComma + 1;
    nextComma = data.indexOf(',', trackerLonIndex);
    rocketLon = data.substring(trackerLonIndex, nextComma).toFloat();

    // alt
    trackerAltIndex = nextComma + 1;
    nextComma = data.indexOf(',', trackerAltIndex);
    rocketAlt = data.substring(trackerAltIndex, nextComma).toFloat();

}

int nthIndexOf(String str, char ch, int n) {
    int index = -1;
    while (n-- > 0) {
    index = str.indexOf(ch, index + 1);
    if (index == -1) {
        break;
        }
    }
    return index;
}

float calculateAzimuth(float lat1, float lon1, float lat2, float lon2){

    // convert to radians
    lat1 = radians(lat1);
    lon1 = radians(lon1);
    lat2 = radians(lat2);
    lon2 = radians(lon2);

    float dLon = lon2 - lon1;
    float y = sin(dLon) * cos(lat1);
    float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    float azimuth = atan2(y, x);

    // conv to degrees

    azimuth = degrees(azimuth);
    if (azimuth < 0){
        azimuth += 360;
    }
    return azimuth;
}

float calculateElevation(float lat1, float lon1, float alt1, float lat2, float lon2, float alt2){

    // find distance between the two points
    float dLat = raidans(lat2- lat1);
    float dLon = radians(lon2 - lon1);

    float a = sin(dLat / 2) * sin(dLat / 2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float distance = EARTH_RADIUS * c;

    // now find angle
    float heightDifference = alt2 - alt1;
    float elevation = atan2(heightDifference, distance);

    // convert to degrees
    return degrees(elevation);
}