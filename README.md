# spi_master
This repository contains SPI_master verilog code along with its testbench. The spi_master with single, dual and quad bus modes.

# Verilator simulation
 ``` 
 verilator -Wall --trace --cc --exe  sim_main.cpp spi_master.v 
cd obj_dir/

make -f Vspi_master.mk

