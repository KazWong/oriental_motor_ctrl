`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05.02.2020 16:19:20
// Design Name: 
// Module Name: differentiator
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module dff(D, clk, Q, NQ, rst);
    input clk;
    input D;
    input rst;
    output reg Q, NQ;
    
    initial begin
        Q <= 1'b1;
        NQ <= 1'b0;
    end
    
    always @(posedge clk or negedge rst) begin
        if (~rst) begin
            Q <= 1'b0;
            NQ <= 1'b1;
        end else begin
            Q <= D; 
            NQ <= ~D;     
        end
    end
endmodule

module dff_4B(D, clk, Q, NQ, rst);
    input clk;
    input [31:0] D;
    input rst;
    output reg [31:0] Q, NQ;
    
    initial begin
        Q <= 32'hFFFF_FFFF;
        NQ <= 32'h0000_0000;
    end
    
    always @(posedge clk) begin
        if (~rst) begin
            Q <= 32'h0000_0000;
            NQ <= 32'hFFFF_FFFF;
        end else begin
            Q <= D;
            NQ <= ~D;
        end
    end
endmodule

module differentiator(clk, multi, speed_out, vt, at, jt);
    input clk;
    input [31:0] multi;
    input speed_out;
    output [31:0] vt, at, jt;
    
    reg [31:0] t_count;
    reg rst_clk, update_clk, latch;
    
    wire [31:0] q;
    wire [31:0] nq;
    
    initial begin
        t_count <= 32'h0;
        update_clk <= 1'b0;
        rst_clk <= 1'b0;
        latch <= 1'b1;
    end
    
    always @ (posedge clk) begin
        t_count <= t_count + 32'h1;
        if (~rst_clk) begin
            rst_clk <= 1'b1;
        end
        if (~update_clk) begin
            update_clk <= 1'b0;
        end
        if (t_count >= 32'h1E8480) begin
            t_count <= 32'h0;
            rst_clk <= ~rst_clk;
        end
        if (t_count >= 32'h1E847F) begin
            update_clk <= ~update_clk;
        end
    end


    dff dff_00(.D(nq[0] ), .clk(speed_out), .Q(q[0] ), .NQ(nq[0] ), .rst(rst_clk));

    genvar i;
    generate
        for (i=1;i<32;i=i+1) begin : bit
            dff dff_(.D(nq[i]), .clk(   nq[i-1]), .Q(q[i]), .NQ(nq[i]), .rst(rst_clk));
        end
    endgenerate 
    
    dff_4B dff_out(.D(q), .clk(update_clk), .Q(vt), .rst(latch));
    
endmodule
