#!/bin/bash

#
# Uncomment to generate individual packets
#
# vrtpktgen cpp signal_data.yaml
# vrtpktgen cpp standard_context.yaml
# vrtpktgen cpp version_context.yaml

vrtpktgen cpp --project --dir difi_v1.1 difiv1.1.yaml
