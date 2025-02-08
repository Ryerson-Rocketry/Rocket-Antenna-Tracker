import math
import socket

# Ground station coordinates
gs_lat = 32.94608
gs_lon = -106.92175
gs_alt = 4546.1

# Earth's radius in meters
EARTH_RADIUS = 6371000

def calculate_azimuth(lat1, lon1, lat2, lon2):
    # Convert degrees to radians
    lat1 = math.radians(lat1)
    lon1 = math.radians(lon1)
    lat2 = math.radians(lat2)
    lon2 = math.radians(lon2)

    d_lon = lon2 - lon1
    y = math.sin(d_lon) * math.cos(lat2)
    x = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(d_lon)
    azimuth = math.atan2(y, x)

    # Convert radians to degrees and normalize to 0-360
    azimuth = math.degrees(azimuth)
    if azimuth < 0:
        azimuth += 360
    return azimuth

def calculate_elevation(lat1, lon1, alt1, lat2, lon2, alt2):
    # Calculate the distance between the two points
    d_lat = math.radians(lat2 - lat1)
    d_lon = math.radians(lon2 - lon1)

    a = math.sin(d_lat / 2) ** 2 + math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) * math.sin(d_lon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    distance = EARTH_RADIUS * c

    # Calculate elevation angle
    height_difference = alt2 - alt1
    elevation = math.atan2(height_difference, distance)

    # Convert radians to degrees
    return math.degrees(elevation)

'''if __name__ == "__main__":
    while True:
        data = input("Enter rocket coordinates (latitude,longitude,altitude): ")
        rocket_lat, rocket_lon, rocket_alt = map(float, data.split(','))

        azimuth = calculate_azimuth(gs_lat, gs_lon, rocket_lat, rocket_lon)
        elevation = calculate_elevation(gs_lat, gs_lon, gs_alt, rocket_lat, rocket_lon, rocket_alt)

        print(f"Calculated Azimuth: {azimuth:.2f} degrees")
        print(f"Calculated Elevation: {elevation:.2f} degrees")'''

if __name__ == "__main__":
    # Set up the UDP server
    udp_ip = "127.0.0.1"
    udp_port = 5005

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((udp_ip, udp_port))

    print(f"Listening for data on {udp_ip}:{udp_port}...")

    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size of 1024 bytes
        if not data:
            continue

        try:
            print("listening")
            rocket_lat, rocket_lon, rocket_alt = map(float, data.decode('utf-8').split(','))
            azimuth = calculate_azimuth(gs_lat, gs_lon, rocket_lat, rocket_lon)
            elevation = calculate_elevation(gs_lat, gs_lon, gs_alt, rocket_lat, rocket_lon, rocket_alt)

            print(f"Received from {addr}:")
            print(f"Calculated Azimuth: {azimuth:.2f} degrees")
            print(f"Calculated Elevation: {elevation:.2f} degrees")
        except ValueError:
            print("Invalid data format. Expected latitude,longitude,altitude.")
