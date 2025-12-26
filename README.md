# Context-Aware Tiered Smart Waste Management System
**A virtual IoT architecture for scalable urban waste collection**

---

## Overview
Urban waste collection becomes inefficient when trucks follow static schedules: bins in high-usage areas overflow early, while bins in low-usage areas are collected unnecessarily. This project proposes a **context-aware, tiered smart waste bin network** that monitors fill levels, publishes telemetry, flags near-full conditions, and feeds a **priority-aware routing engine** to support optimized collection decisions.

**Core idea:** not every bin requires the same sensing capability, sampling rate, or communication cost. Bins are deployed in **tiers** (high-, medium-, and low-priority zones), with sensing, reporting, and power behavior adapted to the operational importance of each location.

---

## Key Design Goals
- **Scalable:** supports 100+ bins across multiple urban zones with bounded network load.
- **Reliable:** robust against sensor noise, transient faults, and intermittent connectivity.
- **Power-aware:** battery-first bin design with tier-specific duty cycling.
- **Cost-feasible:** tier-based hardware trade-offs to maximize return on deployment cost.

---

## System Architecture (End-to-End)
**Bin Node (Edge) → Gateway (LPWAN) → Backend (MQTT/HTTP) → Dashboard + Routing Engine**

<p align="center">
  <img src="software/software_architecture/data_flow_diagram.jpeg" width="600">
</p>
<p align="center"><em>Figure 1: End-to-end system architecture and data flow</em></p>

### Edge Node (Bin)
Typical components (tier-dependent):
- **Sensors:** ultrasonic sensor for fill-level estimation; optional load cell or tilt sensor in high-priority bins for validation and misuse detection.
- **MCU:** low-power LPWAN-enabled microcontroller; processing kept minimal and deterministic.
- **Communication:** LPWAN (LoRa-based) for long-range, low-power uplinks; reporting behavior varies by tier.

### Gateway
- Aggregates uplinks from multiple bin nodes using a star topology.
- Bridges **LPWAN → IP**, forwarding data to the backend via **MQTT** (preferred) or HTTP.
- Buffers data during temporary backhaul outages to avoid data loss.

### Backend + Dashboard
- Data ingestion service and state store (latest bin state, health, and history).
- Priority and routing logic operating on aggregated bin state.
- Dashboard for municipal operations: map view, alerts, bin health, and collection recommendations.

---

## Data Flow
1. **Sense:** bin node samples sensors based on its assigned tier.
2. **Edge validation:** basic sanity checks and temporal validation reduce false readings.
3. **Publish:** telemetry (periodic) and alerts (event-driven) are transmitted to the gateway.
4. **Ingest:** gateway forwards messages to the backend via **MQTT** (or HTTP where required).
5. **Persist + compute:** backend updates bin state and recalculates collection priorities.
6. **Visualize:** dashboard presents fill levels, alerts, and recommended actions.

---

## Route Optimization (Tier-Aware Priority)
Bins are ranked using a **priority score** derived from:
- Fill level and rate of change
- Deployment tier (critical zones weighted higher)
- Time since last collection
- Confidence in sensor data (down-weighting noisy or uncertain readings)
- Vehicle capacity and zone constraints

**Output:** a ranked set of bins and zone-aligned route suggestions for collection vehicles.

---

## Power Model (Tiered Duty Cycling)
Tiering enables explicit control over power consumption:
- **High-priority bins:** shorter sensing intervals and faster alerting for critical locations.
- **Medium-priority bins:** balanced periodic sensing with event-driven escalation.
- **Low-priority bins:** long sleep intervals, sparse reporting, and opportunistic collection.

Power optimization techniques include:
- Deep sleep with RTC-based wake-up
- Sensor power gating
- Event-driven communication to minimize radio on-time
- Local smoothing to suppress transient false alerts

---

## Hardware Architecture
The internal hardware structure of a smart waste bin node is shown below.

<p align="center">
  <img src="hardware/hardware_architecture/block_diagram.jpeg" width="500">
</p>
<p align="center"><em>Figure 2: Smart bin hardware block diagram</em></p>

---

## Pin Mapping
The logical pin-level interface between the MCU and peripherals is illustrated below.

<p align="center">
  <img src="hardware/pin_diagram/bin_node_pin_mapping.jpeg" width="500">
</p>
<p align="center"><em>Figure 3: MCU pin mapping for sensors and peripherals</em></p>

---

## Scalability & Network Considerations
Suggested topology:
- **Star topology:** bin nodes communicate directly with one or more gateways; gateways forward data upstream to the backend.

Protocol notes:
- **MQTT** is recommended for telemetry and alerts due to its lightweight publish–subscribe model and support for quality-of-service levels.
- **HTTP** may be used for simple integrations or low-frequency reporting, but is less efficient for large-scale, event-driven device telemetry.

---

## Repository Structure

```text
WM-Aishwarya_Joshi-NMIT/
│
├── README.md
│
├── software/
│   ├── sensor_code/
│   │   └── ultrasonic_fill_level.c
│   │
│   └── software_architecture/
│       └── data_flow_diagram.jpeg
│
└── hardware/
    ├── hardware_architecture/
    │   └── block_diagram.jpeg
    │
    └── pin_diagram/
        └── bin_node_pin_mapping.jpeg 
```
## Author Details

**Name:** Aishwarya Joshi  
**Email:** aishwarya17.joshi@gmail.com  
**Institution:** Nitte Meenakshi Institute of Technology  
**Program:** B.E. in Electronics and Communication Engineering
