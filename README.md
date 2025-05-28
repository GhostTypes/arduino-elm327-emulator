# 🚗 Arduino ELM327 Emulator

> Emulate an ELM327 OBD-II adapter over Bluetooth LE using Arduino UNO R4


## 🚀 Quick Start

1. Flash `emulator.ino` to your Arduino UNO R4
2. Device advertises as "ELM327 Emulator"
3. Begin debugging your software

## ✨ Features

- 🔧 **ELM327 Protocol Emulation** - Simulate command responses
- 📊 **PID Request Simulation** - Engine RPM, speed, temperature, etc.
- 🚨 **DTC Simulation** - Simulate and clear diagnostic trouble codes

## 📡 Supported Commands

| Command | Description | Response |
|---------|-------------|----------|
| `ATZ` | 🔄 Reset device | `ELM327 v2.2` |
| `ATSP0` | 🔍 Auto protocol detect | `SEARCHING...0` |
| `0105` | 🌡️ Coolant temperature | Simulated temp data |
| `010C` | ⚡ Engine RPM | Simulated RPM data |
| `010D` | 🏎️ Vehicle speed | Simulated speed data |
| `03` | 🚨 Read DTCs | Simulated DTC codes |
| `04` | 🧹 Clear DTCs | `OK` |
| `0902` | 📋 Read VIN | Vehicle ID data |
