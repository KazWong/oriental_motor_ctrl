


set_property IOSTANDARD LVCMOS33 [get_ports exio_ncs]
set_property IOSTANDARD LVCMOS33 [get_ports exio_miso]
set_property IOSTANDARD LVCMOS33 [get_ports exio_mosi]
set_property IOSTANDARD LVCMOS33 [get_ports exio_sclk]

set_property PACKAGE_PIN Y10 [get_ports exio_ncs]
set_property PACKAGE_PIN Y11 [get_ports exio_sclk]
set_property PACKAGE_PIN W11 [get_ports exio_miso]
set_property PACKAGE_PIN W10 [get_ports exio_mosi]

set_property IOSTANDARD LVCMOS33 [get_ports dac_iic_bus_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports dac_iic_bus_sda_io]
set_property PACKAGE_PIN V12 [get_ports dac_iic_bus_scl_io]
set_property PACKAGE_PIN W12 [get_ports dac_iic_bus_sda_io]

# The spi reset control pins
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[0]}]
set_property PACKAGE_PIN U4 [get_ports {zynq_emio_tri_io[0]}]

################################################################
# The motor forward and backward pins go after the SPI nrst pin
################################################################
# start from motor driver signal set A (indicated in schematic)
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[2]}]
# forward
set_property PACKAGE_PIN AA7 [get_ports {zynq_emio_tri_io[1]}]
# backward
set_property PACKAGE_PIN AA6 [get_ports {zynq_emio_tri_io[2]}]
################################################################

# start from motor driver signal set B (indicated in schematic)
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[4]}]
# forward
set_property PACKAGE_PIN V8 [get_ports {zynq_emio_tri_io[3]}]
# backward
set_property PACKAGE_PIN W8 [get_ports {zynq_emio_tri_io[4]}]
################################################################

# start from motor driver signal set C (indicated in schematic)
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[6]}]
# forward
set_property PACKAGE_PIN U10 [get_ports {zynq_emio_tri_io[5]}]
# backward
set_property PACKAGE_PIN U9 [get_ports {zynq_emio_tri_io[6]}]
################################################################

# start from motor driver signal set D (indicated in schematic)
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[8]}]
# forward
set_property PACKAGE_PIN U11 [get_ports {zynq_emio_tri_io[7]}]
# backward
set_property PACKAGE_PIN U12 [get_ports {zynq_emio_tri_io[8]}]
################################################################

###########################
# pin 9 => ARSCH - A
# pin 10 => WSCH - A
# pin 11 => PSCH - A
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[11]}]
set_property PACKAGE_PIN AB4 [get_ports {zynq_emio_tri_io[9]}]
set_property PACKAGE_PIN AB5 [get_ports {zynq_emio_tri_io[10]}]
set_property PACKAGE_PIN AA11 [get_ports {zynq_emio_tri_io[11]}]
###########################

###########################
# pin 12 => ARSCH - B
# pin 13 => WSCH - B
# pin 14 => PSCH - B
set_property PACKAGE_PIN AB1 [get_ports {zynq_emio_tri_io[12]}]
set_property PACKAGE_PIN AB2 [get_ports {zynq_emio_tri_io[13]}]
set_property PACKAGE_PIN AB11 [get_ports {zynq_emio_tri_io[14]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[12]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[13]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[14]}]
###########################

###########################
# pin 15 => ARSCH - C
# pin 16 => WSCH - C
# pin 17 => PSCH - C
# this is the missing routing in the pcb (mod-wire from IC8 to pin AA8)
set_property PACKAGE_PIN AA8 [get_ports {zynq_emio_tri_io[15]}]
set_property PACKAGE_PIN U6 [get_ports {zynq_emio_tri_io[16]}]
set_property PACKAGE_PIN R6 [get_ports {zynq_emio_tri_io[17]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[15]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[16]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[17]}]
###########################

###########################
# pin 18 => ARSCH - D
# pin 19 => WSCH - D
# pin 20 => PSCH - D
set_property PACKAGE_PIN T4 [get_ports {zynq_emio_tri_io[18]}]
set_property PACKAGE_PIN U5 [get_ports {zynq_emio_tri_io[19]}]
set_property PACKAGE_PIN T6 [get_ports {zynq_emio_tri_io[20]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[18]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[19]}]
set_property IOSTANDARD LVCMOS33 [get_ports {zynq_emio_tri_io[20]}]
###########################

###########################
# if CAN BUS IC = VP232
# no EN and RS pin
# if CAN BUS IC = VP234
# EN and RS pin is needed
###########################
set_property IOSTANDARD LVCMOS33 [get_ports zynq_can_rx]
set_property IOSTANDARD LVCMOS33 [get_ports zynq_can_tx]
set_property PACKAGE_PIN AB9 [get_ports zynq_can_tx]
set_property PACKAGE_PIN Y4 [get_ports zynq_can_rx]
###########################

set_property PACKAGE_PIN Y9 [get_ports sys_clk]
set_property IOSTANDARD LVCMOS33 [get_ports sys_clk]