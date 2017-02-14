# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Matthew\Documents\PSoC Creator\Master_v2\Master_2.cydsn\Master_2.cyprj
# Date: Fri, 10 Feb 2017 21:07:11 GMT
#set_units -time ns
create_clock -name {ADC_intClock(FFB)} -period 166.66666666666666 -waveform {0 83.3333333333333} [list [get_pins {ClockBlock/ff_div_6}]]
create_clock -name {Clock_1(FFB)} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ff_div_7}]]
create_clock -name {I2C_1_SCBCLK(FFB)} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/ff_div_2}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyLFCLK} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {ADC_intClock} -source [get_pins {ClockBlock/hfclk}] -edges {1 9 17} [list]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 48001 96001} [list]
create_generated_clock -name {I2C_1_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 7 13} [list]


# Component constraints for C:\Users\Matthew\Documents\PSoC Creator\Master_v2\Master_2.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Matthew\Documents\PSoC Creator\Master_v2\Master_2.cydsn\Master_2.cyprj
# Date: Fri, 10 Feb 2017 21:07:08 GMT
