import socket
import pandas as pd
import time

# UDP Configuration
UDP_IP = "127.0.0.1"  # Ensure this matches your receiver's IP
UDP_PORT = 5005       # Ensure this matches your receiver's port

# Set up a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def linear_value(start, step, index):
    return round(start + step * index, 5)

def generate_sample_data_from_csv(filename):
    data = pd.read_csv(filename)
    step_lat = 0.0001
    step_lon = 0.0001
    step_alt = 1

    for index, row in data.iterrows():
        rocket_lat = linear_value(row['Found rocket lat'], step_lat, index)
        rocket_lon = linear_value(row['Found rocket lon'], step_lon, index)
        rocket_alt = linear_value(row['Found rocket altitude'], step_alt, index)

        message = f"{rocket_lat},{rocket_lon},{rocket_alt}"
        sock.sendto(message.encode(), (UDP_IP, UDP_PORT))
        print(f"Sent: {message}")

        time.sleep(1)  # Simulate real-time transmission

# Specify the actual CSV file path
input_filename = "sample_gps_data.csv"

# Start the UDP data stream
generate_sample_data_from_csv(input_filename)
