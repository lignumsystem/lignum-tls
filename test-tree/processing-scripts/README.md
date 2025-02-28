# Data processing scripts

This folder contains scripts and tools to convert TLS tree geometry from into a
format that can be used with `lignum-system`. There are additional scripts for processing environemental and physiological data.

## Tree geometry workflow

The main steps for reading TLS data to LIGNUM are 1.QSM generation, 2. Leaf generation, and 3. Reading data to LIGNUM. Branch geometry is first converted into QSMs with [TreeQSM](https://github.com/InverseTampere/TreeQSM) and leaf geometry is generated with [QSM-FaNNI](https://github.com/InverseTampere/qsm-fanni-matlab). These data can then be converted into `.xml` files with `QSMReader`.

```mermaid
---
config:
  look: neo
  theme: default
---
flowchart TD
  A[Branch PC] --> X[TreeQSM] --> |qsm2csv.py| C{QSMReader}
  B[Leaf PC] --> Y[FANNI] --> |fanni2csv.py| C{QSMReader}
  C --> |generate_xml.sh| D{LIGNUM}
  