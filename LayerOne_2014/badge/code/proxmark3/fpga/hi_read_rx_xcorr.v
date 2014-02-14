//-----------------------------------------------------------------------------
//
// FPGA_MAJOR_MODE_HF_READER_RX_XCORR
//
// Jonathan Westhues, April 2006
//
// SPI Master Mode added by Arko, Feb 2014
//
//-----------------------------------------------------------------------------

module hi_read_rx_xcorr(
    pck0, ck_1356meg, ck_1356megb,
    pwr_lo, pwr_hi, pwr_oe1, pwr_oe2, pwr_oe3, pwr_oe4,
    adc_d, adc_clk,
    ssp_frame, ssp_din, ssp_dout, ssp_clk,
    cross_hi, cross_lo,
    dbg,
    xcorr_is_848, snoop, xcorr_quarter_freq
);
    input pck0, ck_1356meg, ck_1356megb;
    output pwr_lo, pwr_hi, pwr_oe1, pwr_oe2, pwr_oe3, pwr_oe4;
    input [7:0] adc_d;
    output adc_clk;
    input ssp_dout;
    output ssp_frame, ssp_din, ssp_clk;
    input cross_hi, cross_lo;
    output dbg;
    input xcorr_is_848, snoop, xcorr_quarter_freq;

// Carrier is steady on through this, unless we're snooping.
assign pwr_hi = ck_1356megb & (~snoop);
assign pwr_oe1 = 1'b0;
assign pwr_oe2 = 1'b0;
assign pwr_oe3 = 1'b0;
assign pwr_oe4 = 1'b0;

reg ssp_clk = 0;
reg ssp_din = 0;
reg ssp_frame = 0;

reg dbg = 0;

reg [3:0] spi_counter = 4'b1111;

reg fc_div_2 = 0;
always @(posedge ck_1356meg)
    fc_div_2 = ~fc_div_2;

reg fc_div_4 = 0;
always @(posedge fc_div_2)
    fc_div_4 = ~fc_div_4;

reg fc_div_8 = 0;
always @(posedge fc_div_4)
    fc_div_8 = ~fc_div_8;
	 
reg adc_clk = 0;

always @(xcorr_is_848 or xcorr_quarter_freq or ck_1356meg)
    if(~xcorr_quarter_freq)
    begin
	    if(xcorr_is_848)
	        // The subcarrier frequency is fc/16; we will sample at fc, so that 
	        // means the subcarrier is 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 1 1 ...
	        adc_clk <= ck_1356meg; // was before charlie's request to slow: adc_clk <= ck_1356meg;
	    else
	        // The subcarrier frequency is fc/32; we will sample at fc/2, and
	        // the subcarrier will look identical.
	        adc_clk <= fc_div_2;	// was before charlie's request to slow: adc_clk <= fc_div_2;
    end
    else
    begin
	    if(xcorr_is_848)
	        // The subcarrier frequency is fc/64
	        adc_clk <= fc_div_4;	// was before charlie's request to slow: adc_clk <= fc_div_4;
	    else
	        // The subcarrier frequency is fc/128
	        adc_clk <= fc_div_8;	// was before charlie's request to slow: adc_clk <= fc_div_8;
	end

// When we're a reader, we just need to do the BPSK demod; but when we're an
// eavesdropper, we also need to pick out the commands sent by the reader,
// using AM. Do this the same way that we do it for the simulated tag.
reg after_hysteresis = 0, after_hysteresis_prev = 0;
reg [11:0] has_been_low_for = 0;
always @(negedge adc_clk)
begin
    if(& adc_d[7:0]) after_hysteresis <= 1'b1;
    else if(~(| adc_d[7:0])) after_hysteresis <= 1'b0;

    if(after_hysteresis)
    begin
        has_been_low_for <= 7'b0;
    end
    else
    begin
        if(has_been_low_for == 12'd4095)
        begin
            has_been_low_for <= 12'd0;
            after_hysteresis <= 1'b1;
        end
        else
            has_been_low_for <= has_been_low_for + 1;
    end
end

// Let us report a correlation every 4 subcarrier cycles, or 4*16 samples,
// so we need a 6-bit counter.
reg [5:0] corr_i_cnt = 0;
reg [5:0] corr_q_cnt = 0;
// And a couple of registers in which to accumulate the correlations.
reg signed [15:0] corr_i_accum = 0;
reg signed [15:0] corr_q_accum = 0;
reg signed [7:0] corr_i_out = 0;
reg signed [7:0] corr_q_out = 0;

// ADC data appears on the rising edge, so sample it on the falling edge
always @(negedge adc_clk)
begin
    // These are the correlators: we correlate against in-phase and quadrature
    // versions of our reference signal, and keep the (signed) result to
    // send out later over the SSP.
    if(corr_i_cnt == 7'd63)
    begin
        if(snoop)
        begin
            corr_i_out <= 8'b01010101;  		// Originally: corr_i_out <= {corr_i_accum[12:6], after_hysteresis_prev};
            corr_q_out <= 8'b01010101;		// Originally: corr_q_out <= {corr_q_accum[12:6], after_hysteresis};
        end
        else
        begin
            // Only correlations need to be delivered.
            corr_i_out <= 8'b01010101;						// Originally: corr_i_out <= corr_i_accum[13:6];
            corr_q_out <= 8'b01010101;						// Originally: corr_q_out <= corr_q_accum[13:6];	
        end

        corr_i_accum <= adc_d;
        corr_q_accum <= adc_d;
        corr_q_cnt <= 4;
        corr_i_cnt <= 0;
    end
    else
    begin
        if(corr_i_cnt[3])
            corr_i_accum <= corr_i_accum - adc_d;
        else
            corr_i_accum <= corr_i_accum + adc_d;

        if(corr_q_cnt[3])
            corr_q_accum <= corr_q_accum - adc_d;
        else
            corr_q_accum <= corr_q_accum + adc_d;

        corr_i_cnt <= corr_i_cnt + 1;
        corr_q_cnt <= corr_q_cnt + 1;
    end

    // The logic in hi_simulate.v reports 4 samples per bit. We report two
    // (I, Q) pairs per bit, so we should do 2 samples per pair.
    if(corr_i_cnt == 6'd31)
        after_hysteresis_prev <= after_hysteresis;

    // Then the result from last time is serialized and send out to the ARM.
    // We get one report each cycle, and each report is 16 bits, so the
    // ssp_clk should be the adc_clk divided by 64/16 = 4.
	 
	 /* ORIGINAL PROXMARK SSP FPGA to ARM
    if(corr_i_cnt[1:0] == 2'b10)
        ssp_clk <= 1'b0;

    if(corr_i_cnt[1:0] == 2'b00)
    begin
        ssp_clk <= 1'b1;
        // Don't shift if we just loaded new data, obviously.
        if(corr_i_cnt != 7'd0)
        begin
            corr_i_out[7:0] <= {corr_i_out[6:0], corr_q_out[7]};
            corr_q_out[7:1] <= corr_q_out[6:0];
        end
    end

    if(corr_i_cnt[5:2] == 4'b0000 || corr_i_cnt[5:2] == 4'b1000)
        ssp_frame = 1'b1;
    else
        ssp_frame = 1'b0;
	 */
	 
	 // FOR DEBUG
	 // Set the debug to toggle at each 8bit frame
	 if(corr_i_cnt[5:2] == 4'b0000 || corr_i_cnt[5:2] == 4'b1000)
        dbg = 1'b1;
    else
        dbg = 1'b0;
		  
		  
	 // SPI Slave Out - FPGA (SPI Master) to ARM (SPI Slave)
	 // Once the 16bit frame is ready, enable the spi to clock it out
	 if(corr_i_cnt[5:2] == 4'b0000)
	 begin
		ssp_frame = 0;	
	 end
	 // Trick here is ending the spi enable at the right time
	 // Here we've simulated and measured the end to occur 
	 // when corr_i_cnt[5:2] = 4'b0100
	 if(corr_i_cnt[5:2] == 4'b0100)
	 begin
		ssp_frame = 1;
	 end
end

// SPI Slave Out - FPGA (SPI Master) to ARM (SPI Slave)
// This clocks the ssp_clk at the highest speed possible, 13.56Mhz
// only if the spi is enabled.
always @(ck_1356meg)
begin
	if(ssp_frame == 0)
	begin
		ssp_clk = ~ssp_clk;
	end
end

// SPI Slave Out - FPGA (SPI Master) to ARM (SPI Slave)
// If the spi data is ready (16bit frame of i & q adc data)
// then set the fpga as the master and send the data out
// over spi as fast as possible.
always @(posedge ssp_clk)
begin
	if(ssp_frame == 0)
	begin
		if(spi_counter > 4'b0111)
			begin
				ssp_din = corr_i_out[spi_counter[2:0]];
			end
		else
			begin
				ssp_din = corr_q_out[spi_counter[2:0]];
			end
		
		if(spi_counter > 4'b0000)
			begin
				spi_counter = spi_counter - 1;
			end
		else
			begin
				spi_counter = 4'b1111;
			end
	end
end
	 
//ORIGINAL PROXMARK SSP FPGA to ARM
//assign ssp_din = corr_i_out[7];
//assign dbg = corr_i_cnt[3];

// Unused.
assign pwr_lo = 1'b0;

endmodule
