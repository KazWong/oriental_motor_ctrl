//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.3.1 (lin64) Build 2489853 Tue Mar 26 04:18:30 MDT 2019
//Date        : Fri Apr 24 15:34:40 2020
//Host        : GL504GW running 64-bit Ubuntu 18.04.4 LTS
//Command     : generate_target cpu_wrapper.bd
//Design      : cpu_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module cpu_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    dac_iic_bus_scl_io,
    dac_iic_bus_sda_io,
    exio_miso,
    exio_mosi,
    exio_ncs,
    exio_sclk,
    gpio_rtl_0_tri_i,
    gpio_rtl_1_tri_i,
    gpio_rtl_2_tri_i,
    gpio_rtl_3_tri_i,
    zynq_can_rx,
    zynq_can_tx,
    zynq_emio_tri_io,
    sys_clk);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  inout dac_iic_bus_scl_io;
  inout dac_iic_bus_sda_io;
  input exio_miso;
  output exio_mosi;
  output exio_ncs;
  output exio_sclk;
  input [31:0]gpio_rtl_0_tri_i;
  input [31:0]gpio_rtl_1_tri_i;
  input [31:0]gpio_rtl_2_tri_i;
  input [31:0]gpio_rtl_3_tri_i;
  input zynq_can_rx;
  output zynq_can_tx;
  inout [20:0]zynq_emio_tri_io;
  input sys_clk;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire dac_iic_bus_scl_i;
  wire dac_iic_bus_scl_io;
  wire dac_iic_bus_scl_o;
  wire dac_iic_bus_scl_t;
  wire dac_iic_bus_sda_i;
  wire dac_iic_bus_sda_io;
  wire dac_iic_bus_sda_o;
  wire dac_iic_bus_sda_t;
  wire exio_miso;
  wire exio_mosi;
  wire exio_ncs;
  wire exio_sclk;
  wire [31:0]gpio_rtl_0;
  wire [31:0]gpio_rtl_1;
  wire [31:0]gpio_rtl_2;
  wire [31:0]gpio_rtl_3;
  wire [31:0]gpio_rtl_4;
  wire [31:0]gpio_rtl_5;
  wire [31:0]gpio_rtl_6;
  wire [31:0]gpio_rtl_7;
  wire zynq_can_rx;
  wire zynq_can_tx;
  wire [0:0]zynq_emio_tri_i_0;
  wire [1:1]zynq_emio_tri_i_1;
  wire [10:10]zynq_emio_tri_i_10;
  wire [11:11]zynq_emio_tri_i_11;
  wire [12:12]zynq_emio_tri_i_12;
  wire [13:13]zynq_emio_tri_i_13;
  wire [14:14]zynq_emio_tri_i_14;
  wire [15:15]zynq_emio_tri_i_15;
  wire [16:16]zynq_emio_tri_i_16;
  wire [17:17]zynq_emio_tri_i_17;
  wire [18:18]zynq_emio_tri_i_18;
  wire [19:19]zynq_emio_tri_i_19;
  wire [2:2]zynq_emio_tri_i_2;
  wire [20:20]zynq_emio_tri_i_20;
  wire [3:3]zynq_emio_tri_i_3;
  wire [4:4]zynq_emio_tri_i_4;
  wire [5:5]zynq_emio_tri_i_5;
  wire [6:6]zynq_emio_tri_i_6;
  wire [7:7]zynq_emio_tri_i_7;
  wire [8:8]zynq_emio_tri_i_8;
  wire [9:9]zynq_emio_tri_i_9;
  wire [0:0]zynq_emio_tri_io_0;
  wire [1:1]zynq_emio_tri_io_1;
  wire [10:10]zynq_emio_tri_io_10;
  wire [11:11]zynq_emio_tri_io_11;
  wire [12:12]zynq_emio_tri_io_12;
  wire [13:13]zynq_emio_tri_io_13;
  wire [14:14]zynq_emio_tri_io_14;
  wire [15:15]zynq_emio_tri_io_15;
  wire [16:16]zynq_emio_tri_io_16;
  wire [17:17]zynq_emio_tri_io_17;
  wire [18:18]zynq_emio_tri_io_18;
  wire [19:19]zynq_emio_tri_io_19;
  wire [2:2]zynq_emio_tri_io_2;
  wire [20:20]zynq_emio_tri_io_20;
  wire [3:3]zynq_emio_tri_io_3;
  wire [4:4]zynq_emio_tri_io_4;
  wire [5:5]zynq_emio_tri_io_5;
  wire [6:6]zynq_emio_tri_io_6;
  wire [7:7]zynq_emio_tri_io_7;
  wire [8:8]zynq_emio_tri_io_8;
  wire [9:9]zynq_emio_tri_io_9;
  wire [0:0]zynq_emio_tri_o_0;
  wire [1:1]zynq_emio_tri_o_1;
  wire [10:10]zynq_emio_tri_o_10;
  wire [11:11]zynq_emio_tri_o_11;
  wire [12:12]zynq_emio_tri_o_12;
  wire [13:13]zynq_emio_tri_o_13;
  wire [14:14]zynq_emio_tri_o_14;
  wire [15:15]zynq_emio_tri_o_15;
  wire [16:16]zynq_emio_tri_o_16;
  wire [17:17]zynq_emio_tri_o_17;
  wire [18:18]zynq_emio_tri_o_18;
  wire [19:19]zynq_emio_tri_o_19;
  wire [2:2]zynq_emio_tri_o_2;
  wire [20:20]zynq_emio_tri_o_20;
  wire [3:3]zynq_emio_tri_o_3;
  wire [4:4]zynq_emio_tri_o_4;
  wire [5:5]zynq_emio_tri_o_5;
  wire [6:6]zynq_emio_tri_o_6;
  wire [7:7]zynq_emio_tri_o_7;
  wire [8:8]zynq_emio_tri_o_8;
  wire [9:9]zynq_emio_tri_o_9;
  wire [0:0]zynq_emio_tri_t_0;
  wire [1:1]zynq_emio_tri_t_1;
  wire [10:10]zynq_emio_tri_t_10;
  wire [11:11]zynq_emio_tri_t_11;
  wire [12:12]zynq_emio_tri_t_12;
  wire [13:13]zynq_emio_tri_t_13;
  wire [14:14]zynq_emio_tri_t_14;
  wire [15:15]zynq_emio_tri_t_15;
  wire [16:16]zynq_emio_tri_t_16;
  wire [17:17]zynq_emio_tri_t_17;
  wire [18:18]zynq_emio_tri_t_18;
  wire [19:19]zynq_emio_tri_t_19;
  wire [2:2]zynq_emio_tri_t_2;
  wire [20:20]zynq_emio_tri_t_20;
  wire [3:3]zynq_emio_tri_t_3;
  wire [4:4]zynq_emio_tri_t_4;
  wire [5:5]zynq_emio_tri_t_5;
  wire [6:6]zynq_emio_tri_t_6;
  wire [7:7]zynq_emio_tri_t_7;
  wire [8:8]zynq_emio_tri_t_8;
  wire [9:9]zynq_emio_tri_t_9;

  cpu cpu_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .dac_iic_bus_scl_i(dac_iic_bus_scl_i),
        .dac_iic_bus_scl_o(dac_iic_bus_scl_o),
        .dac_iic_bus_scl_t(dac_iic_bus_scl_t),
        .dac_iic_bus_sda_i(dac_iic_bus_sda_i),
        .dac_iic_bus_sda_o(dac_iic_bus_sda_o),
        .dac_iic_bus_sda_t(dac_iic_bus_sda_t),
        .exio_miso(exio_miso),
        .exio_mosi(exio_mosi),
        .exio_ncs(exio_ncs),
        .exio_sclk(exio_sclk),
        .gpio_rtl_0_tri_i(gpio_rtl_0),
        .gpio_rtl_1_tri_i(gpio_rtl_1),
        .gpio_rtl_2_tri_i(gpio_rtl_2),
        .gpio_rtl_3_tri_i(gpio_rtl_3),
        .zynq_can_rx(zynq_can_rx),
        .zynq_can_tx(zynq_can_tx),
        .zynq_emio_tri_i({zynq_emio_tri_i_20,zynq_emio_tri_i_19,zynq_emio_tri_i_18,zynq_emio_tri_i_17,zynq_emio_tri_i_16,zynq_emio_tri_i_15,zynq_emio_tri_i_14,zynq_emio_tri_i_13,zynq_emio_tri_i_12,zynq_emio_tri_i_11,zynq_emio_tri_i_10,zynq_emio_tri_i_9,zynq_emio_tri_i_8,zynq_emio_tri_i_7,zynq_emio_tri_i_6,zynq_emio_tri_i_5,zynq_emio_tri_i_4,zynq_emio_tri_i_3,zynq_emio_tri_i_2,zynq_emio_tri_i_1,zynq_emio_tri_i_0}),
        .zynq_emio_tri_o({zynq_emio_tri_o_20,zynq_emio_tri_o_19,zynq_emio_tri_o_18,zynq_emio_tri_o_17,zynq_emio_tri_o_16,zynq_emio_tri_o_15,zynq_emio_tri_o_14,zynq_emio_tri_o_13,zynq_emio_tri_o_12,zynq_emio_tri_o_11,zynq_emio_tri_o_10,zynq_emio_tri_o_9,zynq_emio_tri_o_8,zynq_emio_tri_o_7,zynq_emio_tri_o_6,zynq_emio_tri_o_5,zynq_emio_tri_o_4,zynq_emio_tri_o_3,zynq_emio_tri_o_2,zynq_emio_tri_o_1,zynq_emio_tri_o_0}),
        .zynq_emio_tri_t({zynq_emio_tri_t_20,zynq_emio_tri_t_19,zynq_emio_tri_t_18,zynq_emio_tri_t_17,zynq_emio_tri_t_16,zynq_emio_tri_t_15,zynq_emio_tri_t_14,zynq_emio_tri_t_13,zynq_emio_tri_t_12,zynq_emio_tri_t_11,zynq_emio_tri_t_10,zynq_emio_tri_t_9,zynq_emio_tri_t_8,zynq_emio_tri_t_7,zynq_emio_tri_t_6,zynq_emio_tri_t_5,zynq_emio_tri_t_4,zynq_emio_tri_t_3,zynq_emio_tri_t_2,zynq_emio_tri_t_1,zynq_emio_tri_t_0}));
  IOBUF dac_iic_bus_scl_iobuf
       (.I(dac_iic_bus_scl_o),
        .IO(dac_iic_bus_scl_io),
        .O(dac_iic_bus_scl_i),
        .T(dac_iic_bus_scl_t));
  IOBUF dac_iic_bus_sda_iobuf
       (.I(dac_iic_bus_sda_o),
        .IO(dac_iic_bus_sda_io),
        .O(dac_iic_bus_sda_i),
        .T(dac_iic_bus_sda_t));
  IOBUF zynq_emio_tri_iobuf_0
       (.I(zynq_emio_tri_o_0),
        .IO(zynq_emio_tri_io[0]),
        .O(zynq_emio_tri_i_0),
        .T(zynq_emio_tri_t_0));
  IOBUF zynq_emio_tri_iobuf_1
       (.I(zynq_emio_tri_o_1),
        .IO(zynq_emio_tri_io[1]),
        .O(zynq_emio_tri_i_1),
        .T(zynq_emio_tri_t_1));
  IOBUF zynq_emio_tri_iobuf_10
       (.I(zynq_emio_tri_o_10),
        .IO(zynq_emio_tri_io[10]),
        .O(zynq_emio_tri_i_10),
        .T(zynq_emio_tri_t_10));
  IOBUF zynq_emio_tri_iobuf_11
       (.I(zynq_emio_tri_o_11),
        .IO(zynq_emio_tri_io[11]),
        .O(zynq_emio_tri_i_11),
        .T(zynq_emio_tri_t_11));
  IOBUF zynq_emio_tri_iobuf_12
       (.I(zynq_emio_tri_o_12),
        .IO(zynq_emio_tri_io[12]),
        .O(zynq_emio_tri_i_12),
        .T(zynq_emio_tri_t_12));
  IOBUF zynq_emio_tri_iobuf_13
       (.I(zynq_emio_tri_o_13),
        .IO(zynq_emio_tri_io[13]),
        .O(zynq_emio_tri_i_13),
        .T(zynq_emio_tri_t_13));
  IOBUF zynq_emio_tri_iobuf_14
       (.I(zynq_emio_tri_o_14),
        .IO(zynq_emio_tri_io[14]),
        .O(zynq_emio_tri_i_14),
        .T(zynq_emio_tri_t_14));
  IOBUF zynq_emio_tri_iobuf_15
       (.I(zynq_emio_tri_o_15),
        .IO(zynq_emio_tri_io[15]),
        .O(zynq_emio_tri_i_15),
        .T(zynq_emio_tri_t_15));
  IOBUF zynq_emio_tri_iobuf_16
       (.I(zynq_emio_tri_o_16),
        .IO(zynq_emio_tri_io[16]),
        .O(zynq_emio_tri_i_16),
        .T(zynq_emio_tri_t_16));
  IOBUF zynq_emio_tri_iobuf_17
       (.I(zynq_emio_tri_o_17),
        .IO(zynq_emio_tri_io[17]),
        .O(zynq_emio_tri_i_17),
        .T(zynq_emio_tri_t_17));
  IOBUF zynq_emio_tri_iobuf_18
       (.I(zynq_emio_tri_o_18),
        .IO(zynq_emio_tri_io[18]),
        .O(zynq_emio_tri_i_18),
        .T(zynq_emio_tri_t_18));
  IOBUF zynq_emio_tri_iobuf_19
       (.I(zynq_emio_tri_o_19),
        .IO(zynq_emio_tri_io[19]),
        .O(zynq_emio_tri_i_19),
        .T(zynq_emio_tri_t_19));
  IOBUF zynq_emio_tri_iobuf_2
       (.I(zynq_emio_tri_o_2),
        .IO(zynq_emio_tri_io[2]),
        .O(zynq_emio_tri_i_2),
        .T(zynq_emio_tri_t_2));
  IOBUF zynq_emio_tri_iobuf_20
       (.I(zynq_emio_tri_o_20),
        .IO(zynq_emio_tri_io[20]),
        .O(zynq_emio_tri_i_20),
        .T(zynq_emio_tri_t_20));
  IOBUF zynq_emio_tri_iobuf_3
       (.I(zynq_emio_tri_o_3),
        .IO(zynq_emio_tri_io[3]),
        .O(zynq_emio_tri_i_3),
        .T(zynq_emio_tri_t_3));
  IOBUF zynq_emio_tri_iobuf_4
       (.I(zynq_emio_tri_o_4),
        .IO(zynq_emio_tri_io[4]),
        .O(zynq_emio_tri_i_4),
        .T(zynq_emio_tri_t_4));
  IOBUF zynq_emio_tri_iobuf_5
       (.I(zynq_emio_tri_o_5),
        .IO(zynq_emio_tri_io[5]),
        .O(zynq_emio_tri_i_5),
        .T(zynq_emio_tri_t_5));
  IOBUF zynq_emio_tri_iobuf_6
       (.I(zynq_emio_tri_o_6),
        .IO(zynq_emio_tri_io[6]),
        .O(zynq_emio_tri_i_6),
        .T(zynq_emio_tri_t_6));
  IOBUF zynq_emio_tri_iobuf_7
       (.I(zynq_emio_tri_o_7),
        .IO(zynq_emio_tri_io[7]),
        .O(zynq_emio_tri_i_7),
        .T(zynq_emio_tri_t_7));
  IOBUF zynq_emio_tri_iobuf_8
       (.I(zynq_emio_tri_o_8),
        .IO(zynq_emio_tri_io[8]),
        .O(zynq_emio_tri_i_8),
        .T(zynq_emio_tri_t_8));
  IOBUF zynq_emio_tri_iobuf_9
       (.I(zynq_emio_tri_o_9),
        .IO(zynq_emio_tri_io[9]),
        .O(zynq_emio_tri_i_9),
        .T(zynq_emio_tri_t_9));
        
    //startstop_two_pulse A(.clk(sys_clk), .pulse(~zynq_emio_tri_i_11), .count(gpio_rtl_0));
    //startstop_two_pulse B(.clk(sys_clk), .pulse(~zynq_emio_tri_i_14), .count(gpio_rtl_1));
    //startstop_two_pulse C(.clk(sys_clk), .pulse(~zynq_emio_tri_i_17), .count(gpio_rtl_2));
    //startstop_two_pulse D(.clk(sys_clk), .pulse(~zynq_emio_tri_i_20), .count(gpio_rtl_3));
    one_pulse_rst A(.clk(sys_clk), .pulse(~zynq_emio_tri_i_11), .count(gpio_rtl_0));
    one_pulse_rst B(.clk(sys_clk), .pulse(~zynq_emio_tri_i_14), .count(gpio_rtl_1));
    one_pulse_rst C(.clk(sys_clk), .pulse(~zynq_emio_tri_i_17), .count(gpio_rtl_2));
    one_pulse_rst D(.clk(sys_clk), .pulse(~zynq_emio_tri_i_20), .count(gpio_rtl_3));
endmodule
