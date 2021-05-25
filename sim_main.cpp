// DESCRIPTION: Verilator Example: Top level main for invoking model
//
// Copyright 2003-2014 by Wilson Snyder. This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.

#include <verilated.h>		// Defines common routines
#include "Vspi_master.h"		// From Verilating "top.v"
#if VM_TRACE
# include <verilated_vcd_c.h>	// Trace file format header
#endif

Vspi_master *top;			// Instantiation of module

vluint64_t main_time = 0;	// Current simulation time (64-bit unsigned)

double sc_time_stamp () {	// Called by $time in Verilog
    return main_time;		// Note does conversion to real, to match SystemC
}

int main(int argc, char **argv, char **env) {
    if (0 && argc && argv && env) {}	// Prevent unused variable warnings
    top = new Vspi_master;		// Create instance of module

    Verilated::commandArgs(argc, argv);
    Verilated::debug(0);

#if VM_TRACE			// If verilator was invoked with --trace
    Verilated::traceEverOn(true);	// Verilator must compute traced signals
    VL_PRINTF("Enabling waves...\n");
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 10);	// Trace 99 levels of hierarchy
    tfp->open ("vlt_dump.vcd");	// Open the dump file
#endif

    top->i_Rst_L = 1;		// Set some inputs
  
    top->i_Clk = 0;

    while (main_time < 60  && !Verilated::gotFinish()) {

	if ((main_time % 10) == 3) {	// Toggle clock
	    top->i_Clk = 1;
	}
	if ((main_time % 10) == 8) {
	    top->i_Clk = 0;
	}
	if (main_time > 10) {
	    top->i_Rst_L = 1;	// Deassert reset
	} else if (main_time > 1) {
	    top->i_Rst_L = 0;	// Assert reset
	}

	top->eval();		// Evaluate model
#if VM_TRACE
	if (tfp) tfp->dump (main_time);	// Create waveform trace for this timestamp
#endif

	// Read outputs

//	top->fastclk = !top->fastclk;
	main_time++;		// Time passes...
    }

    top->final();

#if VM_TRACE
    if (tfp) tfp->close();
#endif



    exit(0L);
}
