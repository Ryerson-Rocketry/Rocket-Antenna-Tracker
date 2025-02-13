import csv
import random
import pandas as pd
import time


def linear_value(start, step, index):
    return round(start + step * index, 5)

def generate_sample_data_from_csv(filename):
    rows = []
    data = pd.read_csv(filename)
    step_lat = 0.0001  # Increment for latitude
    step_lon = 0.0001  # Increment for longitude
    step_alt = 1  # Increment for altitude
    for index, row in enumerate(data.iterrows()):
        row = row[1]
        new_row = [
            row['TRACKER'],  # TRACKER
            row['DATE'],  # DATE
            row['TIME'],  # TIME
            linear_value(row['GS Lat'], step_lat, index),  # GS Lat
            linear_value(row['GS Lon'], step_lon, index),  # GS Lon
            linear_value(row['GS Alt asl'], step_alt, index),  # GS Alt asl
            linear_value(row['TRACKER Lat'], step_lat, index),  # TRACKER Lat
            linear_value(row['TRACKER Lon'], step_lon, index),  # TRACKER Lon
            linear_value(row['TRACKER Alt asl'], step_alt, index),  # TRACKER Alt asl
            random.randint(1, 3),  # FIX
            linear_value(row['HORZV'], 0.1, index),  # HORZV
            linear_value(row['VERTV'], 0.1, index),  # VERTV
            (row['HEAD'] + 5 * index) % 360,  # HEAD, increment heading
            row['FLAGS'],  # FLAGS
            random.randint(15, 25),  # #TOT
            random.randint(5, 10),  # >40
            random.randint(7, 12),  # >32
            random.randint(2, 5),  # >24
            random.randint(-100, -90),  # RSSI Down
            random.randint(-100, -90),  # RSSI Up
            linear_value(row['Horizontal range (ft)'], 5, index),  # Horizontal range (ft)
            linear_value(row['Alt AGL (ft)'], 1, index),  # Alt AGL (ft)
            linear_value(row['Total range (ft)'], 5, index),  # Total range (ft)
            linear_value(row['LoRa Max Range (kft)'], 1, index),  # LoRa Max Range (kft)
            row['LoRa Success Rate'],  # LoRa Success Rate
            linear_value(row['BATT'], 0.01, index),  # BATT
            linear_value(row['GS_batt_v'], 0.01, index),  # GS_batt_v
            random.randint(3000, 4000),  # Down Sent
            random.randint(200, 300),  # Down Rcvd
            random.randint(600, 700),  # Found Sent
            random.randint(200, 300),  # Up Sent
            random.randint(200, 300),  # Up Rccvd
            row['Found rocket ID'],  # Found rocket ID
            linear_value(row['Found rocket lat'], step_lat, index),  # Found rocket lat
            linear_value(row['Found rocket lon'], step_lon, index),  # Found rocket lon
            linear_value(row['Found rocket altitude'], step_alt, index),  # Found rocket altitude
            random.randint(1, 3),  # Found rocket fix
            linear_value(row['Found rocket batt_v'], 0.01, index),  # Found rocket batt_v
            random.randint(-100, -90),  # Found RSSI
            linear_value(row['GS_ASL_offset'], 0.1, index)  # GS_ASL_offset
        ]
        rows.append(new_row)
    return rows

def write_to_csv(filename, data):
    header = [
        "TRACKER", "DATE", "TIME", "GS Lat", "GS Lon", "GS Alt asl", "TRACKER Lat", "TRACKER Lon", "TRACKER Alt asl",
        "FIX", "HORZV", "VERTV", "HEAD", "FLAGS", "#TOT", ">40", ">32", ">24", "RSSI Down", "RSSI Up",
        "Horizontal range (ft)", "Alt AGL (ft)", "Total range (ft)", "LoRa Max Range (kft)", "LoRa Success Rate",
        "BATT", "GS_batt_v", "Down Sent", "Down Rcvd", "Found Sent", "Up Sent", "Up Rccvd", "Found rocket ID",
        "Found rocket lat", "Found rocket lon", "Found rocket altitude", "Found rocket fix", "Found rocket batt_v",
        "Found RSSI", "GS_ASL_offset"
    ]
    
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(data)

''' stream data for testing purposes
def stream_data(data, interval=1):
    for row in data:
        print(row)  # Simulate streaming by printing each row
        time.sleep(interval)  # Wait for the specified interval (in seconds) '''


input_filename = "/Sample data/MetRocketry_10-23-2024_18_36_18.csv"  
sample_data = generate_sample_data_from_csv(input_filename)  
write_to_csv("sample_gps_data.csv", sample_data)
print("Sample GPS data generated and written to sample_gps_data.csv")

    
# stream_data(sample_data, interval=1)
