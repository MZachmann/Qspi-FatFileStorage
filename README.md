# Qspi-FatFileStorage
Simple implementation of QSPI fat file storage for Nordic nrf52 (nrf52840) chips.

## Summary
This is very simple sample code that implements the full FatFs library with a QSPI chip on a nrf52. A few lines of it originally came from Nordic example code.

This appeares to requires exclusive use of the QSPI chip.

#### Notes
SerialParams.c crudely adds support for the Particle Xenon GD25Q32C QSPI chip.

## Requirements
* User must add QSPI chip parameters in sdk_config.h. See Nordic examples.
* A serial device instance (ASeries) is used to print diagnostics.
* Uses common String implementation


