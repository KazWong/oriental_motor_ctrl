-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.3.1 (lin64) Build 2489853 Tue Mar 26 04:18:30 MDT 2019
-- Date        : Fri Apr 24 15:35:17 2020
-- Host        : GL504GW running 64-bit Ubuntu 18.04.4 LTS
-- Command     : write_vhdl -force -mode synth_stub
--               /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_rst_ps7_0_50M_0/cpu_rst_ps7_0_50M_0_stub.vhdl
-- Design      : cpu_rst_ps7_0_50M_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg484-2
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity cpu_rst_ps7_0_50M_0 is
  Port ( 
    slowest_sync_clk : in STD_LOGIC;
    ext_reset_in : in STD_LOGIC;
    aux_reset_in : in STD_LOGIC;
    mb_debug_sys_rst : in STD_LOGIC;
    dcm_locked : in STD_LOGIC;
    mb_reset : out STD_LOGIC;
    bus_struct_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 )
  );

end cpu_rst_ps7_0_50M_0;

architecture stub of cpu_rst_ps7_0_50M_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "slowest_sync_clk,ext_reset_in,aux_reset_in,mb_debug_sys_rst,dcm_locked,mb_reset,bus_struct_reset[0:0],peripheral_reset[0:0],interconnect_aresetn[0:0],peripheral_aresetn[0:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "proc_sys_reset,Vivado 2018.3.1";
begin
end;
