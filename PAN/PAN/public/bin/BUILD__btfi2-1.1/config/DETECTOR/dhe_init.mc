#add here any initialization command

#loads sequence 0 ("ld 0")
memory load sequencer app 0

set image.extensions no
#set idle on
SET sampling singframe
set autoshutter on
set exptime 100
set shutdelay 120
memory manualcommand stm 0
set autoclear on
