2-Axis Tracking Antenna Prototype

This project is part of MetRocketry's ongoing efforts to enhance ground station capabilities for tracking our rockets in real-time. Our antenna system is designed to automatically track the predicted trajectory of a rocket, utilizing machine learning for precise predictions and a robust mechanical setup for accurate tracking.

Overview:

    - This project aims to create a prototype for a 2-axis antenna tracking system capable of following a rocket's flight path using real-time telemetry data (GPS coordinates (x,y,z)). The system improves communications by ensuring that the directional antenna remains pointed at the rocket during its ascent and descent, allowing for more stable data transfer. minimal signal loss as well as accurate recovery.


Features:

    - Automatic Trajectory Tracking: Utilizes a machine learning-based trajectory prediction model to track the rocket's flight path even in case of telemetry dropouts.

    - 2-Axis Movement: The prototype features azimuth (horizontal) and elevation (vertical) controls, allowing the antenna to follow the rocket's movements seamlessly.

    - Real-Time Control: Interfaces with a ground control station to receive telemetry data and adjust the antenna accordingly.



System Architecture:

    The system consists of the following main components:

        - Mechanical Setup: A motorized antenna mount with two degrees of freedom for azimuth and elevation adjustments.

        - Control Software: Code running on a microcontroller (arduino) that receives trajectory data and adjusts the motors to track the rocket.

        - Telemetry Integration: Real-time telemetry is sent to the control system, which calculates the optimal pointing angles for the antenna.

        - GUI for Ground Station: A simple graphical user interface (GUI) that visualizes the rocket's trajectory and allows manual overrides.
