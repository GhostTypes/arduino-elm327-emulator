<div align="center">
  <h1>Arduino ELM327 Emulator</h1>
  <p>Emulate an ELM327 OBD-II adapter over Bluetooth LE using Arduino UNO R4</p>
</div>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Arduino%20UNO%20R4-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/Protocol-ELM327-green?style=for-the-badge">
  <img src="https://img.shields.io/badge/Connection-Bluetooth%20LE-blue?style=for-the-badge">
</p>

---

<div align="center">
  <h2>Quick Start</h2>
</div>

<div align="center">

1. Flash `emulator.ino` to your Arduino UNO R4
2. Device advertises as "ELM327 Emulator"
3. Begin debugging your software

</div>

---

<div align="center">
  <h2>Features</h2>
</div>

<div align="center">
<table>
  <tr>
    <th>Feature</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>ELM327 Protocol Emulation</td>
    <td>Simulate command responses</td>
  </tr>
  <tr>
    <td>PID Request Simulation</td>
    <td>Engine RPM, speed, temperature, etc.</td>
  </tr>
  <tr>
    <td>DTC Simulation</td>
    <td>Simulate and clear diagnostic trouble codes</td>
  </tr>
</table>
</div>

---

<div align="center">
  <h2>Supported Commands</h2>
</div>

<div align="center">
<table>
  <tr>
    <th>Command</th>
    <th>Description</th>
    <th>Response</th>
  </tr>
  <tr>
    <td><code>ATZ</code></td>
    <td>Reset device</td>
    <td><code>ELM327 v2.2</code></td>
  </tr>
  <tr>
    <td><code>ATSP0</code></td>
    <td>Auto protocol detect</td>
    <td><code>SEARCHING...0</code></td>
  </tr>
  <tr>
    <td><code>0105</code></td>
    <td>Coolant temperature</td>
    <td>Simulated temp data</td>
  </tr>
  <tr>
    <td><code>010C</code></td>
    <td>Engine RPM</td>
    <td>Simulated RPM data</td>
  </tr>
  <tr>
    <td><code>010D</code></td>
    <td>Vehicle speed</td>
    <td>Simulated speed data</td>
  </tr>
  <tr>
    <td><code>03</code></td>
    <td>Read DTCs</td>
    <td>Simulated DTC codes</td>
  </tr>
  <tr>
    <td><code>04</code></td>
    <td>Clear DTCs</td>
    <td><code>OK</code></td>
  </tr>
  <tr>
    <td><code>0902</code></td>
    <td>Read VIN</td>
    <td>Vehicle ID data</td>
  </tr>
</table>
</div>
