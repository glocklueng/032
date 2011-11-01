`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:21:26 10/29/2011 
// Design Name: 
// Module Name:    RomEmulatorCtrl 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------
    [ROM EMULATOR CONTROLLER]

    Copyright 2005 Ki

    This code may be used for non-commercial purposes only.
    The code is provided "AS IS" without warranty of any kind.
    Use this code at your own risk.
-----------------------------------------------------------------------------*/
`define     RE_RESET        3'h0
`define     RE_SET_ADDR     3'h1
`define     RE_INC_ADDR     3'h2
`define     RE_SET_DATA     3'h3
`define     RE_GET_DATA     3'h4
`define     RE_SET_MODE     3'h5
`define     RE_NOP          3'h6


module RomEmuCtrl(
    i_HCmd,
    i_HClk,
    i_HData,
    o_HData,

    i_TAddr,
    io_TData,
    i_nTOE,
    i_nTWE,

    o_RAddr,
    io_RData,
    o_nROE,
    o_nRWE,

    o_nReset
);

    input   [2:0]   i_HCmd;
    input           i_HClk;
    input   [3:0]   i_HData;
    output          o_HData;

    input   [15:0]  i_TAddr;
    inout   [7:0]   io_TData;
    input           i_nTOE;
    input           i_nTWE;

    output  [15:0]  o_RAddr;
    inout   [7:0]   io_RData;
    output          o_nROE;
    output          o_nRWE;

    output          o_nReset;

    reg     [15:0]  	r_Addr;
    reg     [7:0]   r_Data;
    reg             r_nOE;
    reg             r_nWE;
    reg             r_TargetMode;

    reg     [1:0]   r_AddrCount;
    reg     [3:0]   r_DataCount;

    always @(posedge i_HClk)
    begin
        case (i_HCmd)
            `RE_RESET:
            begin
                r_Addr <= 16'h0;
                r_Data <=  8'h0;
                r_TargetMode <= 1'h0;

                r_AddrCount <= 2'h0;
                r_DataCount <= 4'h0;

                r_nOE <= 1'h1;
                r_nWE <= 1'h1;
            end

            `RE_SET_ADDR:
            begin
                case (r_AddrCount)
                    0:  r_Addr[ 3: 0] <= i_HData;
                    1:  r_Addr[ 7: 4] <= i_HData;
                    2:  r_Addr[11: 8] <= i_HData;
                    3:  r_Addr[15:12] <= i_HData;
                endcase
                r_AddrCount <= r_AddrCount + 2'h1;
            end

            `RE_INC_ADDR:
            begin
                r_Addr <= r_Addr + 16'h1;
            end

            `RE_SET_DATA:
            begin
                case (r_DataCount)
                    0:  r_Data[3:0] <= i_HData;
                    1:
                    begin
                        r_Data[7:4] <= i_HData;
                        r_nWE <= 1'h0;
                    end
                    2:  r_nWE <= 1'h1;
                    default:    ; /* no change */
                endcase

                if (r_DataCount == 4'h2)
                    r_DataCount <= 4'h0;
                else
                    r_DataCount <= r_DataCount + 4'h1;
            end

            `RE_GET_DATA:
            begin
                case (r_DataCount)
                    0:  r_nOE <= 1'h0;
                    1:
                    begin
                        r_Data <= io_RData;
                        r_nOE <= 1'h1;
                    end
                    default:    r_Data <= { 1'b0, r_Data[7:1]};
                endcase

                if (r_DataCount == 4'h8)
                    r_DataCount <= 4'h0;
                else
                    r_DataCount <= r_DataCount + 4'h1;
            end

            `RE_SET_MODE:
            begin
                r_TargetMode <= i_HData[0];
            end

            default:    ;   /* no change */
        endcase
    end

    assign  o_RAddr  = r_TargetMode ? i_TAddr : r_Addr;
    assign  o_nROE   = r_TargetMode ? i_nTOE : r_nOE;
    assign  o_nRWE   = r_TargetMode ? i_nTWE : r_nWE;
    assign  io_RData = (!o_nRWE & o_nROE) ? (r_TargetMode ? io_TData : r_Data) : 8'hzz;
    assign  io_TData = (!i_nTOE & i_nTWE & r_TargetMode) ? io_RData : 8'hzz;
    assign  o_HData  = r_Data[0];
    assign  o_nReset = r_TargetMode;

endmodule
