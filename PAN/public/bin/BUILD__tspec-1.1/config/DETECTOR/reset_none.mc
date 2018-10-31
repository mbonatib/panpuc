SET idle off
#sends the reset mode to the DSP waveforms
# first arg: 1 ==> do reset
# second arg:  1:H2rg global, 2: manual global, 3: line by line
memory manualcommand tim 0 0 none none none SRE
#sends the global mode to the device
REG SET NormalModeReg {GLOBAL 0x0} 
SET idle on
