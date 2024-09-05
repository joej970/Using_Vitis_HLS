############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project find_smallest_channel
set_top find_smallest_channel_c_like
add_files find_smallest_channel/find_smallest_channel/find_smallest_channel.cpp
add_files find_smallest_channel/find_smallest_channel/find_smallest_channel.h
add_files -tb find_smallest_channel/find_smallest_channel/find_smallest_channel_test.cpp
open_solution "find_smallest_channel" -flow_target vivado
set_part {xcvu11p-flga2577-1-e}
create_clock -period 4 -name default
config_export -description experiment -display_name extract_min -library min -output C:/DATA/Faks/trost_HLS/find_smallest_channel/find_smallest_channel -taxonomy no -vendor zan -version 1.0
#source "./find_smallest_channel/find_smallest_channel/directives.tcl"
csim_design
csynth_design
cosim_design -trace_level port
export_design -rtl verilog -format ip_catalog -output C:/DATA/Faks/trost_HLS/find_smallest_channel/find_smallest_channel
