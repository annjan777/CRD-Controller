# CRD-Controller

Welcome to the CRD-Controller project! This repository contains code for a Django web application and Raspberry Pi scripts to facilitate two-way communication between AWS, a Raspberry Pi, and an ESP32 microcontroller. This project is designed to manage and control remote devices through a web interface, leveraging the power of AWS services for robust and scalable communication.

## Table of Contents

- [Introduction](#introduction)
- [Architecture](#architecture)
- [Features](#features)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Setup](#setup)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

## Introduction

CRD-Controller is a comprehensive solution for remote device management. It integrates a Django-based web application for user interaction, a Raspberry Pi for on-site control, and an ESP32 microcontroller for device communication. The system utilizes AWS EC2 for hosting and MQTT for message brokering.

## Architecture

![CRD-Controller Architecture](/architecture.jpg)

1. **Django Web Application**: Provides a user-friendly interface to monitor and control devices.
2. **Raspberry Pi**: Acts as a bridge between the web application and ESP32, handling data transmission.
3. **ESP32**: Connected devices that receive commands and send back data.
4. **AWS EC2**: Hosts the Django application and MQTT broker for reliable communication.

## Features

- Real-time device monitoring and control
- Two-way communication between Raspberry Pi, ESP32, and AWS
- Secure and scalable architecture
- User-friendly web interface

## Installation

### Prerequisites

- AWS Account
- Raspberry Pi with Raspbian OS
- ESP32 microcontroller
- Python 3.8+
- Django
- Mosquitto MQTT broker
- NodeRed
- Paho-MQTT
- AWS IoT
- AWS EC2 Instance

### Setup

#### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/CRD-Controller.git
cd CRD-Controller
