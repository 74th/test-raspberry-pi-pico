#!/bin/bash
set -xe
tinygo build -target=pico -o=main.uf2 main.go
picotool load ./main.uf2
picotool reboot