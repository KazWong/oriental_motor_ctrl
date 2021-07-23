`timescale 1ns / 100ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.05.2020 11:02:23
// Design Name: 
// Module Name: combine_Vestimator
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


module ClockDivider_2N(in_clk, out_clk, rst, N);
    input in_clk, rst;
    output out_clk;
    input [15:0] N;
    
    reg  [15:0] r_reg;
    wire [15:0] r_nxt;
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

module ClockDivider_5(in_clk, out_clk, rst);
    input in_clk, rst;
    output out_clk;
 
    wire d2, d1, d0, q2bar, q1bar, q0bar;
    reg q2, q1, q0, q2temp;
 
    initial begin
        q2 = 1'b0;
        q1 = 1'b0;
        q0 = 1'b0;
    end
 
    assign q2bar = ~q2;
    assign q1bar = ~q1;
    assign q0bar = ~q0;
 
    assign d0 = q2bar & q0bar;
    assign d1 = (q1&q0bar) | (q1bar & q0);
    assign d2 = q1 & q0;
    
    //q0
    always @(posedge in_clk or posedge rst) begin
        if (rst) begin
            q0 <= 1'b0;
            q1 <= 1'b0;
            q2 <= 1'b0;
        end else begin
            q0 <= d0;
            q1 <= d1;
            q2 <= d2;
        end
    end
    
    always @(negedge in_clk or posedge rst) begin
        if (rst)
            q2temp <= 1'b0;
        else
            q2temp <= q1;
    end
    
    assign out_clk = q1 | q2temp;
endmodule

module ClockDivider_2( in_clk, out_clk, rst);
    input in_clk, rst;
    output reg out_clk;

    always @ (posedge in_clk or posedge rst) begin
        if (rst)
            out_clk <= 1'b0;
        else
            out_clk <= ~out_clk;	
    end
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

module Counter_20b(clk, Q, rst);
    input clk, rst;                                                                                                                                                                                                              
    output [31:0] Q;                                                                                                                                                                                   
                                                                                          
    wire [31:0] q;                                                                        
    wire [31:0] nq;                                                                       
                                                                                                                                                                         
                                                                                          
    DFlopFlip dff_00(.D(nq[0]), .clk(clk), .Q(q[0]), .NQ(nq[0]), .rst(rst));       
                                                                                          
    genvar i;                                                                             
    generate                                                                              
        for (i=1;i<20;i=i+1) begin : bit                                                  
            DFlopFlip dff_(.D(nq[i]), .clk(nq[i-1]), .Q(q[i]), .NQ(nq[i]), .rst(rst));   
        end                                                                               
    endgenerate
    
    assign Q = q;                                                                          
endmodule        

module Counter_8b(clk, Q, rst);
    input clk, rst;                                                                                                                                                                                                              
    output [31:0] Q;                                                                                                                                                                                   
                                                                                          
    wire [31:0] q;                                                                        
    wire [31:0] nq;                                                                       
                                                                                                                                                                         
                                                                                          
    DFlopFlip dff_00(.D(nq[0]), .clk(clk), .Q(q[0]), .NQ(nq[0]), .rst(rst));       
                                                                                          
    genvar i;                                                                             
    generate                                                                              
        for (i=1;i<8;i=i+1) begin : bit                                                  
            DFlopFlip dff_(.D(nq[i]), .clk(nq[i-1]), .Q(q[i]), .NQ(nq[i]), .rst(rst));   
        end                                                                               
    endgenerate
    
    assign Q = q;                                                                          
endmodule                    

module Timer(clk, O, rst, t);
    input clk, rst;
    input [7:0] t;
    output O;
    
    wire [7:0] q;
    
    Counter_8b counter(.clk(clk), .Q(q), .rst(rst));
    assign O = (q>=t)? 1'b1:1'b0;
endmodule


module startstop_two_pulse( clk, pulse, count);
    input clk, pulse;
    output reg [31:0] count;
    
    reg rst;
    wire ms_clk, fixp_clk, ot_clk, zt_clk;
    wire start_out, module_out, timer_out, overtime_out, zerotime_out, stop_pulse;
    wire [18:0] fixp_count;
    wire [12:0] fixt_count;
    
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
    ClockDivider_2N ms_clkdiv(.in_clk(clk), .out_clk(ms_clk), .rst(rst), .N(16'h61A8));
    Timer ms_timer(.clk(ms_clk), .rst(rst), .O(timer_out), .t(8'h9));
    
    //Overtime timer - 0.04s
    ClockDivider_2N overtime_clkdiv(.in_clk(clk), .out_clk(ot_clk), .rst(~timer_out), .N(16'h61A8));
    Timer overtime_timer(.clk(ot_clk), .rst(~timer_out), .O(overtime_out), .t(8'h27));
    or u1(stop_pulse, pulse, overtime_out);
    
    //zero timer - 0.026s
    ClockDivider_2N zerotime_clkdiv(.in_clk(clk), .out_clk(zt_clk), .rst(~rst), .N(16'h61A8));
    Timer zerotime_timer(.clk(zt_clk), .rst(~rst), .O(zerotime_out), .t(8'h19));
    
    //clk counter - 10000000Hz
    ClockDivider_5 fixp_clkdiv(.in_clk(clk), .out_clk(fixp_clk), .rst(rst));
    Counter_20b fixP_counter(.clk(fixp_clk), .rst(rst), .Q(fixp_count));
    
    //pulse counter
    Counter_20b fixT_counter(.clk(pulse), .rst(rst), .Q(fixt_count));
endmodule


module one_pulse_rst( clk, pulse, count);

    input clk, pulse;
    output reg [31:0] count;
    
    wire ms_clk, fixp_clk, ot_clk;
    wire rst, timer_out, overtime_out, stop_pulse;
    wire [18:0] fixp_count;
    wire [12:0] fixt_count;
    
    initial begin
        count <= 32'h0000_0000;
    end
    
    always @ (posedge rst) begin
        count <= {fixt_count, fixp_count};
    end
    
    //rst trigger
    and u2(rst, stop_pulse, timer_out);
    
    //Update timer - 100Hz
    ClockDivider_2N ms_clkdiv(.in_clk(clk), .out_clk(ms_clk), .rst(rst), .N(16'h61A8));
    Timer ms_timer(.clk(ms_clk), .rst(rst), .O(timer_out), .t(8'h9));
    
    //Overtime timer - 0.04s
    ClockDivider_2N overtime_clkdiv(.in_clk(clk), .out_clk(ot_clk), .rst(~timer_out), .N(16'h61A8));
    Timer overtime_timer(.clk(ot_clk), .rst(~timer_out), .O(overtime_out), .t(8'h27));
    or u1(stop_pulse, pulse, overtime_out);
    
    //clk counter - 10000000Hz
    ClockDivider_5 fixp_clkdiv(.in_clk(clk), .out_clk(fixp_clk), .rst(rst));
    Counter_20b fixP_counter(.clk(fixp_clk), .rst(rst), .Q(fixp_count));
    
    //pulse counter
    Counter_20b fixT_counter(.clk(~pulse), .rst(rst), .Q(fixt_count));
endmodule
