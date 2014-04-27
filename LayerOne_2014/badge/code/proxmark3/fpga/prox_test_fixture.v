`timescale 1ms / 1us

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:52:59 02/08/2014
// Design Name:   fpga
// Module Name:   C:/Users/arko/Desktop/layerone2014/badge/code/proxmark3/fpga/prox_test_fixture.v
// Project Name:  Proxmark3
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: fpga
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module prox_test_fixture;

	// Inputs
	reg spcki;
	reg mosi;
	reg ncs;
	reg pck0i;
	reg ck_1356meg;
	reg ck_1356megb;
	reg [7:0] adc_d;
	reg ssp_dout;
	reg cross_hi;
	reg cross_lo;

	// Outputs
	wire miso;
	wire pwr_lo;
	wire pwr_hi;
	wire pwr_oe1;
	wire pwr_oe2;
	wire pwr_oe3;
	wire pwr_oe4;
	wire adc_clk;
	wire adc_noe;
	wire ssp_frame;
	wire ssp_din;
	wire ssp_clk;
	wire dbg;

	// Instantiate the Unit Under Test (UUT)
	fpga uut (
		.spcki(spcki), 
		.miso(miso), 
		.mosi(mosi), 
		.ncs(ncs), 
		.pck0i(pck0i), 
		.ck_1356meg(ck_1356meg), 
		.ck_1356megb(ck_1356megb), 
		.pwr_lo(pwr_lo), 
		.pwr_hi(pwr_hi), 
		.pwr_oe1(pwr_oe1), 
		.pwr_oe2(pwr_oe2), 
		.pwr_oe3(pwr_oe3), 
		.pwr_oe4(pwr_oe4), 
		.adc_d(adc_d), 
		.adc_clk(adc_clk), 
		.adc_noe(adc_noe), 
		.ssp_frame(ssp_frame), 
		.ssp_din(ssp_din), 
		.ssp_dout(ssp_dout), 
		.ssp_clk(ssp_clk), 
		.cross_hi(cross_hi), 
		.cross_lo(cross_lo), 
		.dbg(dbg)
	);

	initial begin
		// Initialize Inputs
		spcki = 0;
		mosi = 0;
		ncs = 1;
		pck0i = 0;
		ck_1356meg = 0;
		ck_1356megb = 0;
		adc_d = 8'b10101010;
		ssp_dout = 0;
		cross_hi = 0;
		cross_lo = 0;
		
		// Wait 100 ns for global reset to finish
		#100;
    
		// Load Major Mode
		begin
			#10 ncs = 0;			// Pull select down to start
			#10 spcki = ~spcki;	// From low to high
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 spcki = ~spcki;
			#10 ncs = 1;			// Pull select down to start
			#10 ncs = 0;			// Pull select down to start
		end
		
		#60
		
		// Load Major Mode
		begin
			#10 ncs = 0;			// Pull select down to start
			#10 spcki = ~spcki;	// From low to high
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 0;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;	
			#10 mosi = 1;
			#10 spcki = ~spcki;	
			#10 spcki = ~spcki;
			#10 spcki = ~spcki;
			#10 ncs = 1;			// Pull select down to start
			#10 ncs = 0;			// Pull select down to start
		end
		
		// Simulate clock at 20ns period (not 13.56mhz)
		end
	
   always
	begin
		#1 ck_1356meg = ~ck_1356meg; ck_1356megb = ck_1356meg; pck0i = ck_1356meg;
   end
endmodule
