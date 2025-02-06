import socket
import pandas as pd
import time

# UDP Configuration
UDP_IP = "127.0.0.1"  # Update this if needed
UDP_PORT = 5005       # Ensure this matches the receiver's port

# Set up a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def stream_previous_launch_data(filename, interval=1):
    """ Reads past launch data and streams it in real time. """
    data = pd.read_csv(filename)
    
    for index, row in data.iterrows():
        # Extract only the necessary columns for tracking
        rocket_lat = row['Found rocket lat']
        rocket_lon = row['Found rocket lon']
        rocket_alt = row['Found rocket altitude']

        # Format the message
        message = f"{rocket_lat},{rocket_lon},{rocket_alt}"
        sock.sendto(message.encode(), (UDP_IP, UDP_PORT))
        
        print(f"Sent: {message}")
        time.sleep(interval)  # Simulate real-time delay

# Update with your actual CSV file path
previous_launch_file = "Sample data/MetRocketry_10-23-2024_18_36_18.csv"

# Start the streaming
stream_previous_launch_data(previous_launch_file, interval=1)
