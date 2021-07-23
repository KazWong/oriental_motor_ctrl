# 
# Synthesis run script generated by Vivado
# 

set TIME_start [clock seconds] 
proc create_report { reportName command } {
  set status "."
  append status $reportName ".fail"
  if { [file exists $status] } {
    eval file delete [glob $status]
  }
  send_msg_id runtcl-4 info "Executing : $command"
  set retval [eval catch { $command } msg]
  if { $retval != 0 } {
    set fp [open $status w]
    close $fp
    send_msg_id runtcl-5 warning "$msg"
  }
}
create_project -in_memory -part xc7z020clg484-2

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.cache/wt [current_project]
set_property parent.project_path /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.xpr [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_FIFO XPM_MEMORY} [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_output_repo /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
read_verilog -library xil_defaultlib {
  /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/new/combine_Vestimator.v
  /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/hdl/cpu_wrapper.v
}
add_files /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/cpu.bd
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_processing_system7_0_0/cpu_processing_system7_0_0.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_0_0/cpu_axi_gpio_0_0_board.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_0_0/cpu_axi_gpio_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_0_0/cpu_axi_gpio_0_0.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_1_0/cpu_axi_gpio_1_0_board.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_1_0/cpu_axi_gpio_1_0_ooc.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_axi_gpio_1_0/cpu_axi_gpio_1_0.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_rst_ps7_0_50M_0/cpu_rst_ps7_0_50M_0_board.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_rst_ps7_0_50M_0/cpu_rst_ps7_0_50M_0.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_rst_ps7_0_50M_0/cpu_rst_ps7_0_50M_0_ooc.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_xbar_0/cpu_xbar_0_ooc.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/ip/cpu_auto_pc_0/cpu_auto_pc_0_ooc.xdc]
set_property used_in_implementation false [get_files -all /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/sources_1/bd/cpu/cpu_ooc.xdc]

# Mark all dcp files as not used in implementation to prevent them from being
# stitched into the results of this synthesis run. Any black boxes in the
# design are intentionally left as such for best results. Dcp files will be
# stitched into the design at a later time, either when this synthesis run is
# opened, or when it is stitched into a dependent implementation run.
foreach dcp [get_files -quiet -all -filter file_type=="Design\ Checkpoint"] {
  set_property used_in_implementation false $dcp
}
read_xdc /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/constrs_1/new/top.xdc
set_property used_in_implementation false [get_files /home/whwong/Codes/xilinx/vivado/zynq_motor_basic_test.xpr/zynq_motor_ctrl/zynq_motor_ctrl.srcs/constrs_1/new/top.xdc]

read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]
set_param ips.enableIPCacheLiteLoad 1
close [open __synthesis_is_running__ w]

synth_design -top cpu_wrapper -part xc7z020clg484-2


# disable binary constraint mode for synth run checkpoints
set_param constraints.enableBinaryConstraints false
write_checkpoint -force -noxdef cpu_wrapper.dcp
create_report "synth_1_synth_report_utilization_0" "report_utilization -file cpu_wrapper_utilization_synth.rpt -pb cpu_wrapper_utilization_synth.pb"
file delete __synthesis_is_running__
close [open __synthesis_is_complete__ w]