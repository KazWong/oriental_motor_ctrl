`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 21.04.2020 18:05:42
// Design Name: 
// Module Name: VAestimator
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

/*module ClockDivider(in_clk, out_clk, rst, N);
    input in_clk, rst;
    output out_clk;
    input [16:0] N;
    
    reg  [16:0] r_reg;
    wire [16:0] r_nxt;
    reg clk_track;
    
    initial begin
        r_reg <= 16'h0000;
        clk_track <= 1'b0;
    end
    
    always @ (posedge in_clk or posedge rst) begin
        if (rst) begin
            r_reg <= 16'h0000;
            clk_track <= 1'b0;
        end else begin 
            if (r_nxt == N) begin
                r_reg <= 16'h0000;
                clk_track <= ~clk_track;
            end else 
                r_reg <= r_nxt;
        end
    end
    
    assign r_nxt = r_reg+1;
    assign out_clk = ~clk_track;
endmodule

module DFlopFlip(D, clk, Q, NQ, rst);
    input clk, D, rst;
    output Q, NQ;
    
    reg q;
    
    initial begin
        q <= 1'b0;
    end
    
    always @ (posedge clk or posedge rst) begin
        if (rst) begin
            q <= 1'b0;
        end else begin
            q <= D;
        end
    end
    
    assign Q = q;
    assign NQ = ~q;
endmodule   

module Counter_8B(clk, rst, Q, NQ);
    input clk, rst;                                                                                                                                                                                                              
    output [31:0] Q, NQ;                                                                                                                                                                                   
                                                                                          
    wire [31:0] q;                                                                        
    wire [31:0] nq;                                                                       
                                                                                                                                                                         
                                                                                          
    DFlopFlip dff_00(.D(nq[0]), .clk(clk), .Q(q[0]), .NQ(nq[0]), .rst(rst));       
                                                                                          
    genvar i;                                                                             
    generate                                                                              
        for (i=1;i<32;i=i+1) begin : bit                                                  
            DFlopFlip dff_(.D(nq[i]), .clk(nq[i-1]), .Q(q[i]), .NQ(nq[i]), .rst(rst));   
        end                                                                               
    endgenerate
    
    assign Q = q;
    assign NQ = nq;                                                                           
endmodule                           

module Timer(clk, rst, O, t);
    input clk, rst;
    input [31:0] t;
    output O;
    
    wire [31:0] q;
    
    Counter_8B counter(.clk(clk), .Q(q), .rst(rst));
    assign O = (q>=t)? 1'b1:1'b0;
endmodule


module VAestimator( clk, pulse, count);
    input clk, pulse;
    output reg [31:0] count;
    
    reg rst;
    wire ms_clk, fixp_clk, ot_clk, zt_clk;
    wire start_out, module_out, timer_out, overtime_out, zerotime_out, stop_pulse;
    wire [31:0] fixp_count, fixt_count;
    
    initial begin
        rst <= 1'b0;
        count <= 32'h0000_0000;
    end
    
    always @ (posedge module_out or posedge zerotime_out) begin
        if (zerotime_out) begin
            count <= 32'h0000_0000;
        end else begin
            count <= {fixt_count, fixp_count};
        end
    end
    
    always @ (posedge stop_pulse) begin
        if (~start_out)
            rst <= 1'b0;
        else if (module_out)
            rst <= 1'b1;
    end
    
    //start stop
    DFlopFlip start(.D(1'b1), .clk(pulse), .Q(start_out), .NQ(), .rst(rst));
    DFlopFlip stop(.D(timer_out), .clk(stop_pulse), .Q(module_out), .NQ(), .rst(rst));
    
    //Update timer - 100Hz
    ClockDivider ms_clkdiv(.in_clk(clk), .out_clk(ms_clk), .rst(rst), .N(32'h61A8));
    Timer ms_timer(.clk(ms_clk), .rst(rst), .O(timer_out), .t(32'h9));
    
    //Overtime timer - 0.04s
    ClockDivider overtime_clkdiv(.in_clk(clk), .out_clk(ot_clk), .rst(~timer_out), .N(16'h61A8));
    Timer overtime_timer(.clk(ot_clk), .rst(~timer_out), .O(overtime_out), .t(32'h28));
    or u1(stop_pulse, pulse, overtime_out);
    
    //zero timer - 0.05s
    ClockDivider zerotime_clkdiv(.in_clk(clk), .out_clk(zt_clk), .rst(~rst), .N(16'h61A8));
    Timer zerotime_timer(.clk(zt_clk), .rst(~rst), .O(zerotime_out), .t(32'h32));
    
    //clk counter - 5000000Hz
    ClockDivider fixp_clkdiv(.in_clk(clk), .out_clk(fixp_clk), .rst(rst), .N(32'h5));
    Counter_8B fixP_counter(.clk(fixp_clk), .rst(rst), .Q(fixp_count));
    
    //pulse counter
    Counter_8B fixT_counter(.clk(pulse), .rst(rst), .Q(fixt_count));
endmodule*/
