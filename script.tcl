############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project find_smallest_channel
set_top find_smallest_channel
add_files find_smallest_channel/find_smallest_channel/find_smallest_channel.cpp
add_files find_smallest_channel/find_smallest_channel/find_smallest_channel.h
add_files -tb find_smallest_channel/find_smallest_channel/find_smallest_channel_test.cpp
open_solution "find_smallest_channel" -flow_target vivado
set_part {xcvu11p-flga2577-1-e}
create_clock -period 10 -name default
#source "./find_smallest_channel/find_smallest_channel/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
