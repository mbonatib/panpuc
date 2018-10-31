Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 1



1      
2                                    PAGE    132                               ; Printronix page width - 132 columns
3      
4                          ; Include the boot and header files so addressing is easy
5                                    INCLUDE "timboot.asm"
6                          ;  This file is used to generate boot DSP code for the Gen III 250 MHz fiber
7                          ;       optic timing board = ARC22 using a DSP56303 as its main processor.
8      
9                          ; Various addressing control registers
10        FFFFFB           BCR       EQU     $FFFFFB                           ; Bus Control Register
11        FFFFF9           AAR0      EQU     $FFFFF9                           ; Address Attribute Register, channel 0
12        FFFFF8           AAR1      EQU     $FFFFF8                           ; Address Attribute Register, channel 1
13        FFFFF7           AAR2      EQU     $FFFFF7                           ; Address Attribute Register, channel 2
14        FFFFF6           AAR3      EQU     $FFFFF6                           ; Address Attribute Register, channel 3
15        FFFFFD           PCTL      EQU     $FFFFFD                           ; PLL control register
16        FFFFFE           IPRP      EQU     $FFFFFE                           ; Interrupt Priority register - Peripheral
17        FFFFFF           IPRC      EQU     $FFFFFF                           ; Interrupt Priority register - Core
18     
19                         ; Port E is the Synchronous Communications Interface (SCI) port
20        FFFF9F           PCRE      EQU     $FFFF9F                           ; Port Control Register
21        FFFF9E           PRRE      EQU     $FFFF9E                           ; Port Direction Register
22        FFFF9D           PDRE      EQU     $FFFF9D                           ; Port Data Register
23        FFFF9C           SCR       EQU     $FFFF9C                           ; SCI Control Register
24        FFFF9B           SCCR      EQU     $FFFF9B                           ; SCI Clock Control Register
25     
26        FFFF9A           SRXH      EQU     $FFFF9A                           ; SCI Receive Data Register, High byte
27        FFFF99           SRXM      EQU     $FFFF99                           ; SCI Receive Data Register, Middle byte
28        FFFF98           SRXL      EQU     $FFFF98                           ; SCI Receive Data Register, Low byte
29     
30        FFFF97           STXH      EQU     $FFFF97                           ; SCI Transmit Data register, High byte
31        FFFF96           STXM      EQU     $FFFF96                           ; SCI Transmit Data register, Middle byte
32        FFFF95           STXL      EQU     $FFFF95                           ; SCI Transmit Data register, Low byte
33     
34        FFFF94           STXA      EQU     $FFFF94                           ; SCI Transmit Address Register
35        FFFF93           SSR       EQU     $FFFF93                           ; SCI Status Register
36     
37        000009           SCITE     EQU     9                                 ; X:SCR bit set to enable the SCI transmitter
38        000008           SCIRE     EQU     8                                 ; X:SCR bit set to enable the SCI receiver
39        000000           TRNE      EQU     0                                 ; This is set in X:SSR when the transmitter
40                                                                             ;  shift and data registers are both empty
41        000001           TDRE      EQU     1                                 ; This is set in X:SSR when the transmitter
42                                                                             ;  data register is empty
43        000002           RDRF      EQU     2                                 ; X:SSR bit set when receiver register is full
44        00000F           SELSCI    EQU     15                                ; 1 for SCI to backplane, 0 to front connector
45     
46     
47                         ; ESSI Flags
48        000006           TDE       EQU     6                                 ; Set when transmitter data register is empty
49        000007           RDF       EQU     7                                 ; Set when receiver is full of data
50        000010           TE        EQU     16                                ; Transmitter enable
51     
52                         ; Phase Locked Loop initialization
53        050003           PLL_INIT  EQU     $050003                           ; PLL = 25 MHz x 2 = 100 MHz
54     
55                         ; Port B general purpose I/O
56        FFFFC4           HPCR      EQU     $FFFFC4                           ; Control register (bits 1-6 cleared for GPIO)
57        FFFFC9           HDR       EQU     $FFFFC9                           ; Data register
58        FFFFC8           HDDR      EQU     $FFFFC8                           ; Data Direction Register bits (=1 for output)
59     
60                         ; Port C is Enhanced Synchronous Serial Port 0 = ESSI0
61        FFFFBF           PCRC      EQU     $FFFFBF                           ; Port C Control Register
62        FFFFBE           PRRC      EQU     $FFFFBE                           ; Port C Data direction Register
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 2



63        FFFFBD           PDRC      EQU     $FFFFBD                           ; Port C GPIO Data Register
64        FFFFBC           TX00      EQU     $FFFFBC                           ; Transmit Data Register #0
65        FFFFB8           RX0       EQU     $FFFFB8                           ; Receive data register
66        FFFFB7           SSISR0    EQU     $FFFFB7                           ; Status Register
67        FFFFB6           CRB0      EQU     $FFFFB6                           ; Control Register B
68        FFFFB5           CRA0      EQU     $FFFFB5                           ; Control Register A
69     
70                         ; Port D is Enhanced Synchronous Serial Port 1 = ESSI1
71        FFFFAF           PCRD      EQU     $FFFFAF                           ; Port D Control Register
72        FFFFAE           PRRD      EQU     $FFFFAE                           ; Port D Data direction Register
73        FFFFAD           PDRD      EQU     $FFFFAD                           ; Port D GPIO Data Register
74        FFFFAC           TX10      EQU     $FFFFAC                           ; Transmit Data Register 0
75        FFFFA7           SSISR1    EQU     $FFFFA7                           ; Status Register
76        FFFFA6           CRB1      EQU     $FFFFA6                           ; Control Register B
77        FFFFA5           CRA1      EQU     $FFFFA5                           ; Control Register A
78     
79                         ; Timer module addresses
80        FFFF8F           TCSR0     EQU     $FFFF8F                           ; Timer control and status register
81        FFFF8E           TLR0      EQU     $FFFF8E                           ; Timer load register = 0
82        FFFF8D           TCPR0     EQU     $FFFF8D                           ; Timer compare register = exposure time
83        FFFF8C           TCR0      EQU     $FFFF8C                           ; Timer count register = elapsed time
84        FFFF83           TPLR      EQU     $FFFF83                           ; Timer prescaler load register => milliseconds
85        FFFF82           TPCR      EQU     $FFFF82                           ; Timer prescaler count register
86        000000           TIM_BIT   EQU     0                                 ; Set to enable the timer
87        000009           TRM       EQU     9                                 ; Set to enable the timer preloading
88        000015           TCF       EQU     21                                ; Set when timer counter = compare register
89     
90                         ; Board specific addresses and constants
91        FFFFF1           RDFO      EQU     $FFFFF1                           ; Read incoming fiber optic data byte
92        FFFFF2           WRFO      EQU     $FFFFF2                           ; Write fiber optic data replies
93        FFFFF3           WRSS      EQU     $FFFFF3                           ; Write switch state
94        FFFFF5           WRLATCH   EQU     $FFFFF5                           ; Write to a latch
95        010000           RDAD      EQU     $010000                           ; Read A/D values into the DSP
96        000009           EF        EQU     9                                 ; Serial receiver empty flag
97     
98                         ; DSP port A bit equates
99        000000           PWROK     EQU     0                                 ; Power control board says power is OK
100       000001           LED1      EQU     1                                 ; Control one of two LEDs
101       000002           LVEN      EQU     2                                 ; Low voltage power enable
102       000003           HVEN      EQU     3                                 ; High voltage power enable
103       00000E           SSFHF     EQU     14                                ; Switch state FIFO half full flag
104       00000A           EXT_IN0   EQU     10                                ; External digital I/O to the timing board
105       00000B           EXT_IN1   EQU     11
106       00000C           EXT_OUT0  EQU     12
107       00000D           EXT_OUT1  EQU     13
108    
109                        ; Port D equate
110       000000           SLAVE     EQU     0                                 ; Set if master by not installing a jumper
111       000001           SSFEF     EQU     1                                 ; Switch state FIFO empty flag
112    
113                        ; Other equates
114       000002           WRENA     EQU     2                                 ; Enable writing to the EEPROM
115    
116                        ; Latch U25 bit equates
117       000000           CDAC      EQU     0                                 ; Clear the analog board DACs
118       000002           ENCK      EQU     2                                 ; Enable the clock outputs
119       000004           SHUTTER   EQU     4                                 ; Control the shutter
120       000005           TIM_U_RST EQU     5                                 ; Reset the utility board
121    
122                        ; Software status bits, defined at X:<STATUS = X:0
123       000000           ST_RCV    EQU     0                                 ; Set to indicate word is from SCI = utility board
124       000002           IDLMODE   EQU     2                                 ; Set if need to idle after readout
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 3



125       000003           ST_SHUT   EQU     3                                 ; Set to indicate shutter is closed, clear for open
126       000004           ST_RDC    EQU     4                                 ; Set if executing 'RDC' command - reading out
127       000005           SPLIT_S   EQU     5                                 ; Set if split serial
128       000006           SPLIT_P   EQU     6                                 ; Set if split parallel
129       000007           MPP       EQU     7                                 ; Set if parallels are in MPP mode
130       000008           NOT_CLR   EQU     8                                 ; Set if not to clear CCD before exposure
131       00000A           TST_IMG   EQU     10                                ; Set if controller is to generate a test image
132       00000B           SHUT      EQU     11                                ; Set if opening shutter at beginning of exposure
133       00000C           ST_DITH   EQU     12                                ; Set if to dither during exposure
134       00000D           ST_SYNC   EQU     13                                ; Set if starting exposure on SYNC = high signal
135       00000E           ST_CNRD   EQU     14                                ; Set if in continous readout mode
136       00000F           ST_DIRTY  EQU     15                                ; Set if waveform tables need to be updated
137       000010           ST_SA     EQU     16                                ; Set if in subarray readout mode
138       000011           ST_CDS    EQU     17                                ; Set if in correlated double sample radout mode
139    
140                        ; Address for the table containing the incoming SCI words
141       000400           SCI_TABLE EQU     $400
142    
143    
144                        ; Specify controller configuration bits of the X:STATUS word
145                        ;   to describe the software capabilities of this application file
146                        ; The bit is set (=1) if the capability is supported by the controller
147    
148    
149                                COMMENT *
150    
151                        BIT #'s         FUNCTION
152                        2,1,0           Video Processor
153                                                000     ARC41, CCD Rev. 3
154                                                001     CCD Gen I
155                                                010     ARC42, dual readout CCD
156                                                011     ARC44, 4-readout IR coadder
157                                                100     ARC45. dual readout CCD
158                                                101     ARC46 = 8-channel IR
159                                                110     ARC48 = 8 channel CCD
160                                                111     ARC47 = 4-channel CCD
161    
162                        4,3             Timing Board
163                                                00      ARC20, Rev. 4, Gen II
164                                                01      Gen I
165                                                10      ARC22, Gen III, 250 MHz
166    
167                        6,5             Utility Board
168                                                00      No utility board
169                                                01      ARC50
170    
171                        7               Shutter
172                                                0       No shutter support
173                                                1       Yes shutter support
174    
175                        9,8             Temperature readout
176                                                00      No temperature readout
177                                                01      Polynomial Diode calibration
178                                                10      Linear temperature sensor calibration
179    
180                        10              Subarray readout
181                                                0       Not supported
182                                                1       Yes supported
183    
184                        11              Binning
185                                                0       Not supported
186                                                1       Yes supported
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 4



187    
188                        12              Split-Serial readout
189                                                0       Not supported
190                                                1       Yes supported
191    
192                        13              Split-Parallel readout
193                                                0       Not supported
194                                                1       Yes supported
195    
196                        14              MPP = Inverted parallel clocks
197                                                0       Not supported
198                                                1       Yes supported
199    
200                        16,15           Clock Driver Board
201                                                00      ARC30 or ARC31
202                                                01      ARC32, CCD and IR
203                                                11      No clock driver board (Gen I)
204    
205                        19,18,17        Special implementations
206                                                000     Somewhere else
207                                                001     Mount Laguna Observatory
208                                                010     NGST Aladdin
209                                                xxx     Other
210                                *
211    
212                        CCDVIDREV3B
213       000000                     EQU     $000000                           ; CCD Video Processor Rev. 3
214       000000           ARC41     EQU     $000000
215       000001           VIDGENI   EQU     $000001                           ; CCD Video Processor Gen I
216       000002           IRREV4    EQU     $000002                           ; IR Video Processor Rev. 4
217       000002           ARC42     EQU     $000002
218       000003           COADDER   EQU     $000003                           ; IR Coadder
219       000003           ARC44     EQU     $000003
220       000004           CCDVIDREV5 EQU    $000004                           ; Differential input CCD video Rev. 5
221       000004           ARC45     EQU     $000004
222       000005           ARC46     EQU     $000005                           ; 8-channel IR video board
223       000006           ARC48     EQU     $000006                           ; 8-channel CCD video board
224       000007           ARC47     EQU     $000007                           ; 4-channel CCD video board
225       000000           TIMREV4   EQU     $000000                           ; Timing Revision 4 = 50 MHz
226       000000           ARC20     EQU     $000000
227       000008           TIMGENI   EQU     $000008                           ; Timing Gen I = 40 MHz
228       000010           TIMREV5   EQU     $000010                           ; Timing Revision 5 = 250 MHz
229       000010           ARC22     EQU     $000010
230       008000           ARC32     EQU     $008000                           ; CCD & IR clock driver board
231       000020           UTILREV3  EQU     $000020                           ; Utility Rev. 3 supported
232       000020           ARC50     EQU     $000020
233       000080           SHUTTER_CC EQU    $000080                           ; Shutter supported
234       000100           TEMP_POLY EQU     $000100                           ; Polynomial calibration
235                        TEMP_LINEAR
236       000200                     EQU     $000200                           ; Linear calibration
237       000400           SUBARRAY  EQU     $000400                           ; Subarray readout supported
238       000800           BINNING   EQU     $000800                           ; Binning supported
239                        SPLIT_SERIAL
240       001000                     EQU     $001000                           ; Split serial supported
241                        SPLIT_PARALLEL
242       002000                     EQU     $002000                           ; Split parallel supported
243       004000           MPP_CC    EQU     $004000                           ; Inverted clocks supported
244       018000           CLKDRVGENI EQU    $018000                           ; No clock driver board - Gen I
245       020000           MLO       EQU     $020000                           ; Set if Mount Laguna Observatory
246       040000           NGST      EQU     $040000                           ; NGST Aladdin implementation
247       100000           CONT_RD   EQU     $100000                           ; Continuous readout implemented
248       060000           TWO_XMTR  EQU     $060000                           ; Two fiber optic transmitters on ARC-22
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 5



249    
250                        ; Special address for two words for the DSP to bootstrap code from the EEPROM
251                                  IF      @SCP("HOST","ROM")
258                                  ENDIF
259    
260                                  IF      @SCP("HOST","HOST")
261       P:000000 P:000000                   ORG     P:0,P:0
262       P:000000 P:000000 0C0190            JMP     <INIT
263       P:000001 P:000001 000000            NOP
264                                           ENDIF
265    
266                                 ;  This ISR receives serial words a byte at a time over the asynchronous
267                                 ;    serial link (SCI) and squashes them into a single 24-bit word
268       P:000002 P:000002 602400  SCI_RCV   MOVE              R0,X:<SAVE_R0           ; Save R0
269       P:000003 P:000003 052139            MOVEC             SR,X:<SAVE_SR           ; Save Status Register
270       P:000004 P:000004 60A700            MOVE              X:<SCI_R0,R0            ; Restore R0 = pointer to SCI receive regist
er
271       P:000005 P:000005 542300            MOVE              A1,X:<SAVE_A1           ; Save A1
272       P:000006 P:000006 452200            MOVE              X1,X:<SAVE_X1           ; Save X1
273       P:000007 P:000007 54A600            MOVE              X:<SCI_A1,A1            ; Get SRX value of accumulator contents
274       P:000008 P:000008 45E000            MOVE              X:(R0),X1               ; Get the SCI byte
275       P:000009 P:000009 0AD041            BCLR    #1,R0                             ; Test for the address being $FFF6 = last by
te
276       P:00000A P:00000A 000000            NOP
277       P:00000B P:00000B 000000            NOP
278       P:00000C P:00000C 000000            NOP
279       P:00000D P:00000D 205862            OR      X1,A      (R0)+                   ; Add the byte into the 24-bit word
280       P:00000E P:00000E 0E0013            JCC     <MID_BYT                          ; Not the last byte => only restore register
s
281       P:00000F P:00000F 545C00  END_BYT   MOVE              A1,X:(R4)+              ; Put the 24-bit word into the SCI buffer
282       P:000010 P:000010 60F400            MOVE              #SRXL,R0                ; Re-establish first address of SCI interfac
e
                            FFFF98
283       P:000012 P:000012 2C0000            MOVE              #0,A1                   ; For zeroing out SCI_A1
284       P:000013 P:000013 602700  MID_BYT   MOVE              R0,X:<SCI_R0            ; Save the SCI receiver address
285       P:000014 P:000014 542600            MOVE              A1,X:<SCI_A1            ; Save A1 for next interrupt
286       P:000015 P:000015 05A139            MOVEC             X:<SAVE_SR,SR           ; Restore Status Register
287       P:000016 P:000016 54A300            MOVE              X:<SAVE_A1,A1           ; Restore A1
288       P:000017 P:000017 45A200            MOVE              X:<SAVE_X1,X1           ; Restore X1
289       P:000018 P:000018 60A400            MOVE              X:<SAVE_R0,R0           ; Restore R0
290       P:000019 P:000019 000004            RTI                                       ; Return from interrupt service
291    
292                                 ; Clear error condition and interrupt on SCI receiver
293       P:00001A P:00001A 077013  CLR_ERR   MOVEP             X:SSR,X:RCV_ERR         ; Read SCI status register
                            000025
294       P:00001C P:00001C 077018            MOVEP             X:SRXL,X:RCV_ERR        ; This clears any error
                            000025
295       P:00001E P:00001E 000004            RTI
296    
297       P:00001F P:00001F                   DC      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
298       P:000030 P:000030                   DC      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
299       P:000040 P:000040                   DC      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
300    
301                                 ; Tune the table so the following instruction is at P:$50 exactly.
302       P:000050 P:000050 0D0002            JSR     SCI_RCV                           ; SCI receive data interrupt
303       P:000051 P:000051 000000            NOP
304       P:000052 P:000052 0D001A            JSR     CLR_ERR                           ; SCI receive error interrupt
305       P:000053 P:000053 000000            NOP
306    
307                                 ; *******************  Command Processing  ******************
308    
309                                 ; Read the header and check it for self-consistency
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 6



310       P:000054 P:000054 609F00  START     MOVE              X:<IDL_ADR,R0
311       P:000055 P:000055 018FA0            JSET    #TIM_BIT,X:TCSR0,EXPOSING         ; If exposing go check the timer
                            000333
312       P:000057 P:000057 0A00A4            JSET    #ST_RDC,X:<STATUS,CONTINUE_READING
                            100000
313       P:000059 P:000059 0AE080            JMP     (R0)
314    
315       P:00005A P:00005A 330700  TST_RCV   MOVE              #<COM_BUF,R3
316       P:00005B P:00005B 0D00A5            JSR     <GET_RCV
317       P:00005C P:00005C 0E005B            JCC     *-1
318    
319                                 ; Check the header and read all the remaining words in the command
320       P:00005D P:00005D 0C00FF  PRC_RCV   JMP     <CHK_HDR                          ; Update HEADER and NWORDS
321       P:00005E P:00005E 578600  PR_RCV    MOVE              X:<NWORDS,B             ; Read this many words total in the command
322       P:00005F P:00005F 000000            NOP
323       P:000060 P:000060 01418C            SUB     #1,B                              ; We've already read the header
324       P:000061 P:000061 000000            NOP
325       P:000062 P:000062 06CF00            DO      B,RD_COM
                            00006A
326       P:000064 P:000064 205B00            MOVE              (R3)+                   ; Increment past what's been read already
327       P:000065 P:000065 0B0080  GET_WRD   JSCLR   #ST_RCV,X:STATUS,CHK_FO
                            0000A9
328       P:000067 P:000067 0B00A0            JSSET   #ST_RCV,X:STATUS,CHK_SCI
                            0000D5
329       P:000069 P:000069 0E0065            JCC     <GET_WRD
330       P:00006A P:00006A 000000            NOP
331       P:00006B P:00006B 330700  RD_COM    MOVE              #<COM_BUF,R3            ; Restore R3 = beginning of the command
332    
333                                 ; Is this command for the timing board?
334       P:00006C P:00006C 448500            MOVE              X:<HEADER,X0
335       P:00006D P:00006D 579B00            MOVE              X:<DMASK,B
336       P:00006E P:00006E 459A4E            AND     X0,B      X:<TIM_DRB,X1           ; Extract destination byte
337       P:00006F P:00006F 20006D            CMP     X1,B                              ; Does header = timing board number?
338       P:000070 P:000070 0EA080            JEQ     <COMMAND                          ; Yes, process it here
339       P:000071 P:000071 0E909D            JLT     <FO_XMT                           ; Send it to fiber optic transmitter
340    
341                                 ; Transmit the command to the utility board over the SCI port
342       P:000072 P:000072 060600            DO      X:<NWORDS,DON_XMT                 ; Transmit NWORDS
                            00007E
343       P:000074 P:000074 60F400            MOVE              #STXL,R0                ; SCI first byte address
                            FFFF95
344       P:000076 P:000076 44DB00            MOVE              X:(R3)+,X0              ; Get the 24-bit word to transmit
345       P:000077 P:000077 060380            DO      #3,SCI_SPT
                            00007D
346       P:000079 P:000079 019381            JCLR    #TDRE,X:SSR,*                     ; Continue ONLY if SCI XMT is empty
                            000079
347       P:00007B P:00007B 445800            MOVE              X0,X:(R0)+              ; Write to SCI, byte pointer + 1
348       P:00007C P:00007C 000000            NOP                                       ; Delay for the status flag to be set
349       P:00007D P:00007D 000000            NOP
350                                 SCI_SPT
351       P:00007E P:00007E 000000            NOP
352                                 DON_XMT
353       P:00007F P:00007F 0C0054            JMP     <START
354    
355                                 ; Process the receiver entry - is it in the command table ?
356       P:000080 P:000080 0203DF  COMMAND   MOVE              X:(R3+1),B              ; Get the command
357       P:000081 P:000081 205B00            MOVE              (R3)+
358       P:000082 P:000082 205B00            MOVE              (R3)+                   ; Point R3 to the first argument
359       P:000083 P:000083 302800            MOVE              #<COM_TBL,R0            ; Get the command table starting address
360       P:000084 P:000084 062E80            DO      #NUM_COM,END_COM                  ; Loop over the command table
                            00008B
361       P:000086 P:000086 47D800            MOVE              X:(R0)+,Y1              ; Get the command table entry
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 7



362       P:000087 P:000087 62E07D            CMP     Y1,B      X:(R0),R2               ; Does receiver = table entries address?
363       P:000088 P:000088 0E208B            JNE     <NOT_COM                          ; No, keep looping
364       P:000089 P:000089 00008C            ENDDO                                     ; Restore the DO loop system registers
365       P:00008A P:00008A 0AE280            JMP     (R2)                              ; Jump execution to the command
366       P:00008B P:00008B 205800  NOT_COM   MOVE              (R0)+                   ; Increment the register past the table addr
ess
367                                 END_COM
368       P:00008C P:00008C 0C008D            JMP     <ERROR                            ; The command is not in the table
369    
370                                 ; It's not in the command table - send an error message
371       P:00008D P:00008D 479D00  ERROR     MOVE              X:<ERR,Y1               ; Send the message - there was an error
372       P:00008E P:00008E 0C0090            JMP     <FINISH1                          ; This protects against unknown commands
373    
374                                 ; Send a reply packet - header and reply
375       P:00008F P:00008F 479800  FINISH    MOVE              X:<DONE,Y1              ; Send 'DON' as the reply
376       P:000090 P:000090 578500  FINISH1   MOVE              X:<HEADER,B             ; Get header of incoming command
377       P:000091 P:000091 469C00            MOVE              X:<SMASK,Y0             ; This was the source byte, and is to
378       P:000092 P:000092 330700            MOVE              #<COM_BUF,R3            ;     become the destination byte
379       P:000093 P:000093 46935E            AND     Y0,B      X:<TWO,Y0
380       P:000094 P:000094 0C1ED1            LSR     #8,B                              ; Shift right eight bytes, add it to the
381       P:000095 P:000095 460600            MOVE              Y0,X:<NWORDS            ;     header, and put 2 as the number
382       P:000096 P:000096 469958            ADD     Y0,B      X:<SBRD,Y0              ;     of words in the string
383       P:000097 P:000097 200058            ADD     Y0,B                              ; Add source board's header, set Y1 for abov
e
384       P:000098 P:000098 000000            NOP
385       P:000099 P:000099 575B00            MOVE              B,X:(R3)+               ; Put the new header on the transmitter stac
k
386       P:00009A P:00009A 475B00            MOVE              Y1,X:(R3)+              ; Put the argument on the transmitter stack
387       P:00009B P:00009B 570500            MOVE              B,X:<HEADER
388       P:00009C P:00009C 0C006B            JMP     <RD_COM                           ; Decide where to send the reply, and do it
389    
390                                 ; Transmit words to the host computer over the fiber optics link
391       P:00009D P:00009D 63F400  FO_XMT    MOVE              #COM_BUF,R3
                            000007
392       P:00009F P:00009F 060600            DO      X:<NWORDS,DON_FFO                 ; Transmit all the words in the command
                            0000A3
393       P:0000A1 P:0000A1 57DB00            MOVE              X:(R3)+,B
394       P:0000A2 P:0000A2 0D00EB            JSR     <XMT_WRD
395       P:0000A3 P:0000A3 000000            NOP
396       P:0000A4 P:0000A4 0C0054  DON_FFO   JMP     <START
397    
398                                 ; Check for commands from the fiber optic FIFO and the utility board (SCI)
399       P:0000A5 P:0000A5 0D00A9  GET_RCV   JSR     <CHK_FO                           ; Check for fiber optic command from FIFO
400       P:0000A6 P:0000A6 0E80A8            JCS     <RCV_RTS                          ; If there's a command, check the header
401       P:0000A7 P:0000A7 0D00D5            JSR     <CHK_SCI                          ; Check for an SCI command
402       P:0000A8 P:0000A8 00000C  RCV_RTS   RTS
403    
404                                 ; Because of FIFO metastability require that EF be stable for two tests
405       P:0000A9 P:0000A9 0A8989  CHK_FO    JCLR    #EF,X:HDR,TST2                    ; EF = Low,  Low  => CLR SR, return
                            0000AC
406       P:0000AB P:0000AB 0C00AF            JMP     <TST3                             ;      High, Low  => try again
407       P:0000AC P:0000AC 0A8989  TST2      JCLR    #EF,X:HDR,CLR_CC                  ;      Low,  High => try again
                            0000D1
408       P:0000AE P:0000AE 0C00A9            JMP     <CHK_FO                           ;      High, High => read FIFO
409       P:0000AF P:0000AF 0A8989  TST3      JCLR    #EF,X:HDR,CHK_FO
                            0000A9
410    
411       P:0000B1 P:0000B1 08F4BB            MOVEP             #$028FE2,X:BCR          ; Slow down RDFO access
                            028FE2
412       P:0000B3 P:0000B3 000000            NOP
413       P:0000B4 P:0000B4 000000            NOP
414       P:0000B5 P:0000B5 5FF000            MOVE                          Y:RDFO,B
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 8



                            FFFFF1
415       P:0000B7 P:0000B7 2B0000            MOVE              #0,B2
416       P:0000B8 P:0000B8 0140CE            AND     #$FF,B
                            0000FF
417       P:0000BA P:0000BA 0140CD            CMP     #>$AC,B                           ; It must be $AC to be a valid word
                            0000AC
418       P:0000BC P:0000BC 0E20D1            JNE     <CLR_CC
419       P:0000BD P:0000BD 4EF000            MOVE                          Y:RDFO,Y0   ; Read the MS byte
                            FFFFF1
420       P:0000BF P:0000BF 0C1951            INSERT  #$008010,Y0,B
                            008010
421       P:0000C1 P:0000C1 4EF000            MOVE                          Y:RDFO,Y0   ; Read the middle byte
                            FFFFF1
422       P:0000C3 P:0000C3 0C1951            INSERT  #$008008,Y0,B
                            008008
423       P:0000C5 P:0000C5 4EF000            MOVE                          Y:RDFO,Y0   ; Read the LS byte
                            FFFFF1
424       P:0000C7 P:0000C7 0C1951            INSERT  #$008000,Y0,B
                            008000
425       P:0000C9 P:0000C9 000000            NOP
426       P:0000CA P:0000CA 516300            MOVE              B0,X:(R3)               ; Put the word into COM_BUF
427       P:0000CB P:0000CB 0A0000            BCLR    #ST_RCV,X:<STATUS                 ; Its a command from the host computer
428       P:0000CC P:0000CC 000000  SET_CC    NOP
429       P:0000CD P:0000CD 0AF960            BSET    #0,SR                             ; Valid word => SR carry bit = 1
430       P:0000CE P:0000CE 08F4BB            MOVEP             #$028FE1,X:BCR          ; Restore RDFO access
                            028FE1
431       P:0000D0 P:0000D0 00000C            RTS
432       P:0000D1 P:0000D1 0AF940  CLR_CC    BCLR    #0,SR                             ; Not valid word => SR carry bit = 0
433       P:0000D2 P:0000D2 08F4BB            MOVEP             #$028FE1,X:BCR          ; Restore RDFO access
                            028FE1
434       P:0000D4 P:0000D4 00000C            RTS
435    
436                                 ; Test the SCI (= synchronous communications interface) for new words
437       P:0000D5 P:0000D5 44F000  CHK_SCI   MOVE              X:(SCI_TABLE+33),X0
                            000421
438       P:0000D7 P:0000D7 228E00            MOVE              R4,A
439       P:0000D8 P:0000D8 209000            MOVE              X0,R0
440       P:0000D9 P:0000D9 200045            CMP     X0,A
441       P:0000DA P:0000DA 0EA0D1            JEQ     <CLR_CC                           ; There is no new SCI word
442       P:0000DB P:0000DB 44D800            MOVE              X:(R0)+,X0
443       P:0000DC P:0000DC 446300            MOVE              X0,X:(R3)
444       P:0000DD P:0000DD 220E00            MOVE              R0,A
445       P:0000DE P:0000DE 0140C5            CMP     #(SCI_TABLE+32),A                 ; Wrap it around the circular
                            000420
446       P:0000E0 P:0000E0 0EA0E4            JEQ     <INIT_PROCESSED_SCI               ;   buffer boundary
447       P:0000E1 P:0000E1 547000            MOVE              A1,X:(SCI_TABLE+33)
                            000421
448       P:0000E3 P:0000E3 0C00E9            JMP     <SCI_END
449                                 INIT_PROCESSED_SCI
450       P:0000E4 P:0000E4 56F400            MOVE              #SCI_TABLE,A
                            000400
451       P:0000E6 P:0000E6 000000            NOP
452       P:0000E7 P:0000E7 567000            MOVE              A,X:(SCI_TABLE+33)
                            000421
453       P:0000E9 P:0000E9 0A0020  SCI_END   BSET    #ST_RCV,X:<STATUS                 ; Its a utility board (SCI) word
454       P:0000EA P:0000EA 0C00CC            JMP     <SET_CC
455    
456                                 ; Transmit the word in B1 to the host computer over the fiber optic data link
457                                 XMT_WRD
458       P:0000EB P:0000EB 08F4BB            MOVEP             #$028FE2,X:BCR          ; Slow down RDFO access
                            028FE2
459       P:0000ED P:0000ED 60F400            MOVE              #FO_HDR+1,R0
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 9



                            000002
460       P:0000EF P:0000EF 060380            DO      #3,XMT_WRD1
                            0000F3
461       P:0000F1 P:0000F1 0C1D91            ASL     #8,B,B
462       P:0000F2 P:0000F2 000000            NOP
463       P:0000F3 P:0000F3 535800            MOVE              B2,X:(R0)+
464                                 XMT_WRD1
465       P:0000F4 P:0000F4 60F400            MOVE              #FO_HDR,R0
                            000001
466       P:0000F6 P:0000F6 61F400            MOVE              #WRFO,R1
                            FFFFF2
467       P:0000F8 P:0000F8 060480            DO      #4,XMT_WRD2
                            0000FB
468       P:0000FA P:0000FA 46D800            MOVE              X:(R0)+,Y0              ; Should be MOVEP  X:(R0)+,Y:WRFO
469       P:0000FB P:0000FB 4E6100            MOVE                          Y0,Y:(R1)
470                                 XMT_WRD2
471       P:0000FC P:0000FC 08F4BB            MOVEP             #$028FE1,X:BCR          ; Restore RDFO access
                            028FE1
472       P:0000FE P:0000FE 00000C            RTS
473    
474                                 ; Check the command or reply header in X:(R3) for self-consistency
475       P:0000FF P:0000FF 46E300  CHK_HDR   MOVE              X:(R3),Y0
476       P:000100 P:000100 579600            MOVE              X:<MASK1,B              ; Test for S.LE.3 and D.LE.3 and N.LE.7
477       P:000101 P:000101 20005E            AND     Y0,B
478       P:000102 P:000102 0E208D            JNE     <ERROR                            ; Test failed
479       P:000103 P:000103 579700            MOVE              X:<MASK2,B              ; Test for either S.NE.0 or D.NE.0
480       P:000104 P:000104 20005E            AND     Y0,B
481       P:000105 P:000105 0EA08D            JEQ     <ERROR                            ; Test failed
482       P:000106 P:000106 579500            MOVE              X:<SEVEN,B
483       P:000107 P:000107 20005E            AND     Y0,B                              ; Extract NWORDS, must be > 0
484       P:000108 P:000108 0EA08D            JEQ     <ERROR
485       P:000109 P:000109 44E300            MOVE              X:(R3),X0
486       P:00010A P:00010A 440500            MOVE              X0,X:<HEADER            ; Its a correct header
487       P:00010B P:00010B 550600            MOVE              B1,X:<NWORDS            ; Number of words in the command
488       P:00010C P:00010C 0C005E            JMP     <PR_RCV
489    
490                                 ;  *****************  Boot Commands  *******************
491    
492                                 ; Test Data Link - simply return value received after 'TDL'
493       P:00010D P:00010D 47DB00  TDL       MOVE              X:(R3)+,Y1              ; Get the data value
494       P:00010E P:00010E 0C0090            JMP     <FINISH1                          ; Return from executing TDL command
495    
496                                 ; Read DSP or EEPROM memory ('RDM' address): read memory, reply with value
497       P:00010F P:00010F 47DB00  RDMEM     MOVE              X:(R3)+,Y1
498       P:000110 P:000110 20EF00            MOVE              Y1,B
499       P:000111 P:000111 0140CE            AND     #$0FFFFF,B                        ; Bits 23-20 need to be zeroed
                            0FFFFF
500       P:000113 P:000113 21B000            MOVE              B1,R0                   ; Need the address in an address register
501       P:000114 P:000114 20EF00            MOVE              Y1,B
502       P:000115 P:000115 000000            NOP
503       P:000116 P:000116 0ACF14            JCLR    #20,B,RDX                         ; Test address bit for Program memory
                            00011A
504       P:000118 P:000118 07E087            MOVE              P:(R0),Y1               ; Read from Program Memory
505       P:000119 P:000119 0C0090            JMP     <FINISH1                          ; Send out a header with the value
506       P:00011A P:00011A 0ACF15  RDX       JCLR    #21,B,RDY                         ; Test address bit for X: memory
                            00011E
507       P:00011C P:00011C 47E000            MOVE              X:(R0),Y1               ; Write to X data memory
508       P:00011D P:00011D 0C0090            JMP     <FINISH1                          ; Send out a header with the value
509       P:00011E P:00011E 0ACF16  RDY       JCLR    #22,B,RDR                         ; Test address bit for Y: memory
                            000122
510       P:000120 P:000120 4FE000            MOVE                          Y:(R0),Y1   ; Read from Y data memory
511       P:000121 P:000121 0C0090            JMP     <FINISH1                          ; Send out a header with the value
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 10



512       P:000122 P:000122 0ACF17  RDR       JCLR    #23,B,ERROR                       ; Test address bit for read from EEPROM memo
ry
                            00008D
513       P:000124 P:000124 479400            MOVE              X:<THREE,Y1             ; Convert to word address to a byte address
514       P:000125 P:000125 220600            MOVE              R0,Y0                   ; Get 16-bit address in a data register
515       P:000126 P:000126 2000B8            MPY     Y0,Y1,B                           ; Multiply
516       P:000127 P:000127 20002A            ASR     B                                 ; Eliminate zero fill of fractional multiply
517       P:000128 P:000128 213000            MOVE              B0,R0                   ; Need to address memory
518       P:000129 P:000129 0AD06F            BSET    #15,R0                            ; Set bit so its in EEPROM space
519       P:00012A P:00012A 0D0178            JSR     <RD_WORD                          ; Read word from EEPROM
520       P:00012B P:00012B 21A700            MOVE              B1,Y1                   ; FINISH1 transmits Y1 as its reply
521       P:00012C P:00012C 0C0090            JMP     <FINISH1
522    
523                                 ; Program WRMEM ('WRM' address datum): write to memory, reply 'DON'.
524       P:00012D P:00012D 47DB00  WRMEM     MOVE              X:(R3)+,Y1              ; Get the address to be written to
525       P:00012E P:00012E 20EF00            MOVE              Y1,B
526       P:00012F P:00012F 0140CE            AND     #$0FFFFF,B                        ; Bits 23-20 need to be zeroed
                            0FFFFF
527       P:000131 P:000131 21B000            MOVE              B1,R0                   ; Need the address in an address register
528       P:000132 P:000132 20EF00            MOVE              Y1,B
529       P:000133 P:000133 46DB00            MOVE              X:(R3)+,Y0              ; Get datum into Y0 so MOVE works easily
530       P:000134 P:000134 0ACF14            JCLR    #20,B,WRX                         ; Test address bit for Program memory
                            000138
531       P:000136 P:000136 076086            MOVE              Y0,P:(R0)               ; Write to Program memory
532       P:000137 P:000137 0C008F            JMP     <FINISH
533       P:000138 P:000138 0ACF15  WRX       JCLR    #21,B,WRY                         ; Test address bit for X: memory
                            00013C
534       P:00013A P:00013A 466000            MOVE              Y0,X:(R0)               ; Write to X: memory
535       P:00013B P:00013B 0C008F            JMP     <FINISH
536       P:00013C P:00013C 0ACF16  WRY       JCLR    #22,B,WRR                         ; Test address bit for Y: memory
                            000140
537       P:00013E P:00013E 4E6000            MOVE                          Y0,Y:(R0)   ; Write to Y: memory
538       P:00013F P:00013F 0C008F            JMP     <FINISH
539       P:000140 P:000140 0ACF17  WRR       JCLR    #23,B,ERROR                       ; Test address bit for write to EEPROM
                            00008D
540       P:000142 P:000142 013D02            BCLR    #WRENA,X:PDRC                     ; WR_ENA* = 0 to enable EEPROM writing
541       P:000143 P:000143 460E00            MOVE              Y0,X:<SV_A1             ; Save the datum to be written
542       P:000144 P:000144 479400            MOVE              X:<THREE,Y1             ; Convert word address to a byte address
543       P:000145 P:000145 220600            MOVE              R0,Y0                   ; Get 16-bit address in a data register
544       P:000146 P:000146 2000B8            MPY     Y1,Y0,B                           ; Multiply
545       P:000147 P:000147 20002A            ASR     B                                 ; Eliminate zero fill of fractional multiply
546       P:000148 P:000148 213000            MOVE              B0,R0                   ; Need to address memory
547       P:000149 P:000149 0AD06F            BSET    #15,R0                            ; Set bit so its in EEPROM space
548       P:00014A P:00014A 558E00            MOVE              X:<SV_A1,B1             ; Get the datum to be written
549       P:00014B P:00014B 060380            DO      #3,L1WRR                          ; Loop over three bytes of the word
                            000154
550       P:00014D P:00014D 07588D            MOVE              B1,P:(R0)+              ; Write each EEPROM byte
551       P:00014E P:00014E 0C1C91            ASR     #8,B,B
552       P:00014F P:00014F 469E00            MOVE              X:<C100K,Y0             ; Move right one byte, enter delay = 1 msec
553       P:000150 P:000150 06C600            DO      Y0,L2WRR                          ; Delay by 12 milliseconds for EEPROM write
                            000153
554       P:000152 P:000152 060CA0            REP     #12                               ; Assume 100 MHz DSP56303
555       P:000153 P:000153 000000            NOP
556                                 L2WRR
557       P:000154 P:000154 000000            NOP                                       ; DO loop nesting restriction
558                                 L1WRR
559       P:000155 P:000155 013D22            BSET    #WRENA,X:PDRC                     ; WR_ENA* = 1 to disable EEPROM writing
560       P:000156 P:000156 0C008F            JMP     <FINISH
561    
562                                 ; Load application code from P: memory into its proper locations
563       P:000157 P:000157 47DB00  LDAPPL    MOVE              X:(R3)+,Y1              ; Application number, not used yet
564       P:000158 P:000158 0D015A            JSR     <LOAD_APPLICATION
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 11



565       P:000159 P:000159 0C008F            JMP     <FINISH
566    
567                                 LOAD_APPLICATION
568       P:00015A P:00015A 60F400            MOVE              #$8000,R0               ; Starting EEPROM address
                            008000
569       P:00015C P:00015C 0D0178            JSR     <RD_WORD                          ; Number of words in boot code
570       P:00015D P:00015D 21A600            MOVE              B1,Y0
571       P:00015E P:00015E 479400            MOVE              X:<THREE,Y1
572       P:00015F P:00015F 2000B8            MPY     Y0,Y1,B
573       P:000160 P:000160 20002A            ASR     B
574       P:000161 P:000161 213000            MOVE              B0,R0                   ; EEPROM address of start of P: application
575       P:000162 P:000162 0AD06F            BSET    #15,R0                            ; To access EEPROM
576       P:000163 P:000163 0D0178            JSR     <RD_WORD                          ; Read number of words in application P:
577       P:000164 P:000164 61F400            MOVE              #(X_BOOT_START+1),R1    ; End of boot P: code that needs keeping
                            00022B
578       P:000166 P:000166 06CD00            DO      B1,RD_APPL_P
                            000169
579       P:000168 P:000168 0D0178            JSR     <RD_WORD
580       P:000169 P:000169 07598D            MOVE              B1,P:(R1)+
581                                 RD_APPL_P
582       P:00016A P:00016A 0D0178            JSR     <RD_WORD                          ; Read number of words in application X:
583       P:00016B P:00016B 61F400            MOVE              #END_COMMAND_TABLE,R1
                            000036
584       P:00016D P:00016D 06CD00            DO      B1,RD_APPL_X
                            000170
585       P:00016F P:00016F 0D0178            JSR     <RD_WORD
586       P:000170 P:000170 555900            MOVE              B1,X:(R1)+
587                                 RD_APPL_X
588       P:000171 P:000171 0D0178            JSR     <RD_WORD                          ; Read number of words in application Y:
589       P:000172 P:000172 310100            MOVE              #1,R1                   ; There is no Y: memory in the boot code
590       P:000173 P:000173 06CD00            DO      B1,RD_APPL_Y
                            000176
591       P:000175 P:000175 0D0178            JSR     <RD_WORD
592       P:000176 P:000176 5D5900            MOVE                          B1,Y:(R1)+
593                                 RD_APPL_Y
594       P:000177 P:000177 00000C            RTS
595    
596                                 ; Read one word from EEPROM location R0 into accumulator B1
597       P:000178 P:000178 060380  RD_WORD   DO      #3,L_RDBYTE
                            00017B
598       P:00017A P:00017A 07D88B            MOVE              P:(R0)+,B2
599       P:00017B P:00017B 0C1C91            ASR     #8,B,B
600                                 L_RDBYTE
601       P:00017C P:00017C 00000C            RTS
602    
603                                 ; Come to here on a 'STP' command so 'DON' can be sent
604                                 STOP_IDLE_CLOCKING
605       P:00017D P:00017D 305A00            MOVE              #<TST_RCV,R0            ; Execution address when idle => when not
606       P:00017E P:00017E 601F00            MOVE              R0,X:<IDL_ADR           ;   processing commands or reading out
607       P:00017F P:00017F 0A0002            BCLR    #IDLMODE,X:<STATUS                ; Don't idle after readout
608       P:000180 P:000180 0C008F            JMP     <FINISH
609    
610                                 ; Routines executed after the DSP boots and initializes
611       P:000181 P:000181 305A00  STARTUP   MOVE              #<TST_RCV,R0            ; Execution address when idle => when not
612       P:000182 P:000182 601F00            MOVE              R0,X:<IDL_ADR           ;   processing commands or reading out
613       P:000183 P:000183 44F400            MOVE              #50000,X0               ; Delay by 500 milliseconds
                            00C350
614       P:000185 P:000185 06C400            DO      X0,L_DELAY
                            000188
615       P:000187 P:000187 06E8A3            REP     #1000
616       P:000188 P:000188 000000            NOP
617                                 L_DELAY
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 12



618       P:000189 P:000189 57F400            MOVE              #$020002,B              ; Normal reply after booting is 'SYR'
                            020002
619       P:00018B P:00018B 0D00EB            JSR     <XMT_WRD
620       P:00018C P:00018C 57F400            MOVE              #'SYR',B
                            535952
621       P:00018E P:00018E 0D00EB            JSR     <XMT_WRD
622    
623       P:00018F P:00018F 0C0054            JMP     <START                            ; Start normal command processing
624    
625                                 ; *******************  DSP  INITIALIZATION  CODE  **********************
626                                 ; This code initializes the DSP right after booting, and is overwritten
627                                 ;   by application code
628       P:000190 P:000190 08F4BD  INIT      MOVEP             #PLL_INIT,X:PCTL        ; Initialize PLL to 100 MHz
                            050003
629       P:000192 P:000192 000000            NOP
630    
631                                 ; Set operation mode register OMR to normal expanded
632       P:000193 P:000193 0500BA            MOVEC             #$0000,OMR              ; Operating Mode Register = Normal Expanded
633       P:000194 P:000194 0500BB            MOVEC             #0,SP                   ; Reset the Stack Pointer SP
634    
635                                 ; Program the AA = address attribute pins
636       P:000195 P:000195 08F4B9            MOVEP             #$FFFC21,X:AAR0         ; Y = $FFF000 to $FFFFFF asserts commands
                            FFFC21
637       P:000197 P:000197 08F4B8            MOVEP             #$008909,X:AAR1         ; P = $008000 to $00FFFF accesses the EEPROM
                            008909
638       P:000199 P:000199 08F4B7            MOVEP             #$010C11,X:AAR2         ; X = $010000 to $010FFF reads A/D values
                            010C11
639       P:00019B P:00019B 08F4B6            MOVEP             #$080621,X:AAR3         ; Y = $080000 to $0BFFFF R/W from SRAM
                            080621
640    
641       P:00019D P:00019D 0A0F00            BCLR    #CDAC,X:<LATCH                    ; Enable clearing of DACs
642       P:00019E P:00019E 0A0F02            BCLR    #ENCK,X:<LATCH                    ; Disable clock and DAC output switches
643       P:00019F P:00019F 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Execute these two operations
                            00000F
644    
645                                 ; Program the DRAM memory access and addressing
646       P:0001A1 P:0001A1 08F4BB            MOVEP             #$028FE1,X:BCR          ; Bus Control Register
                            028FE1
647    
648                                 ; Program the Host port B for parallel I/O
649       P:0001A3 P:0001A3 08F484            MOVEP             #>1,X:HPCR              ; All pins enabled as GPIO
                            000001
650       P:0001A5 P:0001A5 08F489            MOVEP             #$810C,X:HDR
                            00810C
651       P:0001A7 P:0001A7 08F488            MOVEP             #$B10E,X:HDDR           ; Data Direction Register
                            00B10E
652                                                                                     ;  (1 for Output, 0 for Input)
653    
654                                 ; Port B conversion from software bits to schematic labels
655                                 ;       PB0 = PWROK             PB08 = PRSFIFO*
656                                 ;       PB1 = LED1              PB09 = EF*
657                                 ;       PB2 = LVEN              PB10 = EXT-IN0
658                                 ;       PB3 = HVEN              PB11 = EXT-IN1
659                                 ;       PB4 = STATUS0           PB12 = EXT-OUT0
660                                 ;       PB5 = STATUS1           PB13 = EXT-OUT1
661                                 ;       PB6 = STATUS2           PB14 = SSFHF*
662                                 ;       PB7 = STATUS3           PB15 = SELSCI
663    
664                                 ; Program the serial port ESSI0 = Port C for serial communication with
665                                 ;   the utility board
666       P:0001A9 P:0001A9 07F43F            MOVEP             #>0,X:PCRC              ; Software reset of ESSI0
                            000000
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 13



667       P:0001AB P:0001AB 07F435            MOVEP             #$180809,X:CRA0         ; Divide 100 MHz by 20 to get 5.0 MHz
                            180809
668                                                                                     ; DC[4:0] = 0 for non-network operation
669                                                                                     ; WL0-WL2 = 3 for 24-bit data words
670                                                                                     ; SSC1 = 0 for SC1 not used
671       P:0001AD P:0001AD 07F436            MOVEP             #$020020,X:CRB0         ; SCKD = 1 for internally generated clock
                            020020
672                                                                                     ; SCD2 = 0 so frame sync SC2 is an output
673                                                                                     ; SHFD = 0 for MSB shifted first
674                                                                                     ; FSL = 0, frame sync length not used
675                                                                                     ; CKP = 0 for rising clock edge transitions
676                                                                                     ; SYN = 0 for asynchronous
677                                                                                     ; TE0 = 1 to enable transmitter #0
678                                                                                     ; MOD = 0 for normal, non-networked mode
679                                                                                     ; TE0 = 0 to NOT enable transmitter #0 yet
680                                                                                     ; RE = 1 to enable receiver
681       P:0001AF P:0001AF 07F43F            MOVEP             #%111001,X:PCRC         ; Control Register (0 for GPIO, 1 for ESSI)
                            000039
682       P:0001B1 P:0001B1 07F43E            MOVEP             #%000110,X:PRRC         ; Data Direction Register (0 for In, 1 for O
ut)
                            000006
683       P:0001B3 P:0001B3 07F43D            MOVEP             #%000100,X:PDRC         ; Data Register - WR_ENA* = 1
                            000004
684    
685                                 ; Port C version = Analog boards
686                                 ;       MOVEP   #$000809,X:CRA0 ; Divide 100 MHz by 20 to get 5.0 MHz
687                                 ;       MOVEP   #$000030,X:CRB0 ; SCKD = 1 for internally generated clock
688                                 ;       MOVEP   #%100000,X:PCRC ; Control Register (0 for GPIO, 1 for ESSI)
689                                 ;       MOVEP   #%000100,X:PRRC ; Data Direction Register (0 for In, 1 for Out)
690                                 ;       MOVEP   #%000000,X:PDRC ; Data Register: 'not used' = 0 outputs
691    
692       P:0001B5 P:0001B5 07F43C            MOVEP             #0,X:TX00               ; Initialize the transmitter to zero
                            000000
693       P:0001B7 P:0001B7 000000            NOP
694       P:0001B8 P:0001B8 000000            NOP
695       P:0001B9 P:0001B9 013630            BSET    #TE,X:CRB0                        ; Enable the SSI transmitter
696    
697                                 ; Conversion from software bits to schematic labels for Port C
698                                 ;       PC0 = SC00 = UTL-T-SCK
699                                 ;       PC1 = SC01 = 2_XMT = SYNC on prototype
700                                 ;       PC2 = SC02 = WR_ENA*
701                                 ;       PC3 = SCK0 = TIM-U-SCK
702                                 ;       PC4 = SRD0 = UTL-T-STD
703                                 ;       PC5 = STD0 = TIM-U-STD
704    
705                                 ; Program the serial port ESSI1 = Port D for serial transmission to
706                                 ;   the analog boards and two parallel I/O input pins
707       P:0001BA P:0001BA 07F42F            MOVEP             #>0,X:PCRD              ; Software reset of ESSI0
                            000000
708       P:0001BC P:0001BC 07F425            MOVEP             #$000809,X:CRA1         ; Divide 100 MHz by 20 to get 5.0 MHz
                            000809
709                                                                                     ; DC[4:0] = 0
710                                                                                     ; WL[2:0] = ALC = 0 for 8-bit data words
711                                                                                     ; SSC1 = 0 for SC1 not used
712       P:0001BE P:0001BE 07F426            MOVEP             #$000030,X:CRB1         ; SCKD = 1 for internally generated clock
                            000030
713                                                                                     ; SCD2 = 1 so frame sync SC2 is an output
714                                                                                     ; SHFD = 0 for MSB shifted first
715                                                                                     ; CKP = 0 for rising clock edge transitions
716                                                                                     ; TE0 = 0 to NOT enable transmitter #0 yet
717                                                                                     ; MOD = 0 so its not networked mode
718       P:0001C0 P:0001C0 07F42F            MOVEP             #%100000,X:PCRD         ; Control Register (0 for GPIO, 1 for ESSI)
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 14



                            000020
719                                                                                     ; PD3 = SCK1, PD5 = STD1 for ESSI
720       P:0001C2 P:0001C2 07F42E            MOVEP             #%000100,X:PRRD         ; Data Direction Register (0 for In, 1 for O
ut)
                            000004
721       P:0001C4 P:0001C4 07F42D            MOVEP             #%000100,X:PDRD         ; Data Register: 'not used' = 0 outputs
                            000004
722       P:0001C6 P:0001C6 07F42C            MOVEP             #0,X:TX10               ; Initialize the transmitter to zero
                            000000
723       P:0001C8 P:0001C8 000000            NOP
724       P:0001C9 P:0001C9 000000            NOP
725       P:0001CA P:0001CA 012630            BSET    #TE,X:CRB1                        ; Enable the SSI transmitter
726    
727                                 ; Conversion from software bits to schematic labels for Port D
728                                 ; PD0 = SC10 = 2_XMT_? input
729                                 ; PD1 = SC11 = SSFEF* input
730                                 ; PD2 = SC12 = PWR_EN
731                                 ; PD3 = SCK1 = TIM-A-SCK
732                                 ; PD4 = SRD1 = PWRRST
733                                 ; PD5 = STD1 = TIM-A-STD
734    
735                                 ; Program the SCI port to communicate with the utility board
736       P:0001CB P:0001CB 07F41C            MOVEP             #$0B04,X:SCR            ; SCI programming: 11-bit asynchronous
                            000B04
737                                                                                     ;   protocol (1 start, 8 data, 1 even parity
,
738                                                                                     ;   1 stop); LSB before MSB; enable receiver
739                                                                                     ;   and its interrupts; transmitter interrup
ts
740                                                                                     ;   disabled.
741       P:0001CD P:0001CD 07F41B            MOVEP             #$0003,X:SCCR           ; SCI clock: utility board data rate =
                            000003
742                                                                                     ;   (390,625 kbits/sec); internal clock.
743       P:0001CF P:0001CF 07F41F            MOVEP             #%011,X:PCRE            ; Port Control Register = RXD, TXD enabled
                            000003
744       P:0001D1 P:0001D1 07F41E            MOVEP             #%000,X:PRRE            ; Port Direction Register (0 = Input)
                            000000
745    
746                                 ;       PE0 = RXD
747                                 ;       PE1 = TXD
748                                 ;       PE2 = SCLK
749    
750                                 ; Program one of the three timers as an exposure timer
751       P:0001D3 P:0001D3 07F403            MOVEP             #$C34F,X:TPLR           ; Prescaler to generate millisecond timer,
                            00C34F
752                                                                                     ;  counting from the system clock / 2 = 50 M
Hz
753       P:0001D5 P:0001D5 07F40F            MOVEP             #$208200,X:TCSR0        ; Clear timer complete bit and enable presca
ler
                            208200
754       P:0001D7 P:0001D7 07F40E            MOVEP             #0,X:TLR0               ; Timer load register
                            000000
755    
756                                 ; Enable interrupts for the SCI port only
757       P:0001D9 P:0001D9 08F4BF            MOVEP             #$000000,X:IPRC         ; No interrupts allowed
                            000000
758       P:0001DB P:0001DB 08F4BE            MOVEP             #>$80,X:IPRP            ; Enable SCI interrupt only, IPR = 1
                            000080
759       P:0001DD P:0001DD 00FCB8            ANDI    #$FC,MR                           ; Unmask all interrupt levels
760    
761                                 ; Initialize the fiber optic serial receiver circuitry
762       P:0001DE P:0001DE 061480            DO      #20,L_FO_INIT
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 15



                            0001E3
763       P:0001E0 P:0001E0 5FF000            MOVE                          Y:RDFO,B
                            FFFFF1
764       P:0001E2 P:0001E2 0605A0            REP     #5
765       P:0001E3 P:0001E3 000000            NOP
766                                 L_FO_INIT
767    
768                                 ; Pulse PRSFIFO* low to revive the CMDRST* instruction and reset the FIFO
769       P:0001E4 P:0001E4 44F400            MOVE              #1000000,X0             ; Delay by 10 milliseconds
                            0F4240
770       P:0001E6 P:0001E6 06C400            DO      X0,*+3
                            0001E8
771       P:0001E8 P:0001E8 000000            NOP
772       P:0001E9 P:0001E9 0A8908            BCLR    #8,X:HDR
773       P:0001EA P:0001EA 0614A0            REP     #20
774       P:0001EB P:0001EB 000000            NOP
775       P:0001EC P:0001EC 0A8928            BSET    #8,X:HDR
776    
777                                 ; Reset the utility board
778       P:0001ED P:0001ED 0A0F05            BCLR    #5,X:<LATCH
779       P:0001EE P:0001EE 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Clear reset utility board bit
                            00000F
780       P:0001F0 P:0001F0 06C8A0            REP     #200                              ; Delay by RESET* low time
781       P:0001F1 P:0001F1 000000            NOP
782       P:0001F2 P:0001F2 0A0F25            BSET    #5,X:<LATCH
783       P:0001F3 P:0001F3 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Clear reset utility board bit
                            00000F
784       P:0001F5 P:0001F5 56F400            MOVE              #200000,A               ; Delay 2 msec for utility boot
                            030D40
785       P:0001F7 P:0001F7 06CE00            DO      A,*+3
                            0001F9
786       P:0001F9 P:0001F9 000000            NOP
787    
788                                 ; Put all the analog switch inputs to low so they draw minimum current
789       P:0001FA P:0001FA 012F23            BSET    #3,X:PCRD                         ; Turn the serial clock on
790       P:0001FB P:0001FB 56F400            MOVE              #$0C3000,A              ; Value of integrate speed and gain switches
                            0C3000
791       P:0001FD P:0001FD 20001B            CLR     B
792       P:0001FE P:0001FE 241000            MOVE              #$100000,X0             ; Increment over board numbers for DAC write
s
793       P:0001FF P:0001FF 45F400            MOVE              #$001000,X1             ; Increment over board numbers for WRSS writ
es
                            001000
794       P:000201 P:000201 060F80            DO      #15,L_ANALOG                      ; Fifteen video processor boards maximum
                            000209
795       P:000203 P:000203 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
796       P:000204 P:000204 200040            ADD     X0,A
797       P:000205 P:000205 5F7000            MOVE                          B,Y:WRSS    ; This is for the fast analog switches
                            FFFFF3
798       P:000207 P:000207 0620A3            REP     #800                              ; Delay for the serial data transmission
799       P:000208 P:000208 000000            NOP
800       P:000209 P:000209 200068            ADD     X1,B                              ; Increment the video and clock driver numbe
rs
801                                 L_ANALOG
802       P:00020A P:00020A 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
803       P:00020B P:00020B 0C0223            JMP     <SKIP
804    
805                                 ; Transmit contents of accumulator A1 over the synchronous serial transmitter
806                                 XMIT_A_WORD
807       P:00020C P:00020C 07F42C            MOVEP             #0,X:TX10               ; This helps, don't know why
                            000000
808       P:00020E P:00020E 547000            MOVE              A1,X:SV_A1
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 16



                            00000E
809       P:000210 P:000210 000000            NOP
810       P:000211 P:000211 01A786            JCLR    #TDE,X:SSISR1,*                   ; Start bit
                            000211
811       P:000213 P:000213 07F42C            MOVEP             #$010000,X:TX10
                            010000
812       P:000215 P:000215 060380            DO      #3,L_X
                            00021B
813       P:000217 P:000217 01A786            JCLR    #TDE,X:SSISR1,*                   ; Three data bytes
                            000217
814       P:000219 P:000219 04CCCC            MOVEP             A1,X:TX10
815       P:00021A P:00021A 0C1E90            LSL     #8,A
816       P:00021B P:00021B 000000            NOP
817                                 L_X
818       P:00021C P:00021C 01A786            JCLR    #TDE,X:SSISR1,*                   ; Zeroes to bring transmitter low
                            00021C
819       P:00021E P:00021E 07F42C            MOVEP             #0,X:TX10
                            000000
820       P:000220 P:000220 54F000            MOVE              X:SV_A1,A1
                            00000E
821       P:000222 P:000222 00000C            RTS
822    
823                                 SKIP
824    
825                                 ; Set up the circular SCI buffer, 32 words in size
826       P:000223 P:000223 64F400            MOVE              #SCI_TABLE,R4
                            000400
827       P:000225 P:000225 051FA4            MOVE              #31,M4
828       P:000226 P:000226 647000            MOVE              R4,X:(SCI_TABLE+33)
                            000421
829    
830                                           IF      @SCP("HOST","ROM")
838                                           ENDIF
839    
840       P:000228 P:000228 44F400            MOVE              #>$AC,X0
                            0000AC
841       P:00022A P:00022A 440100            MOVE              X0,X:<FO_HDR
842    
843       P:00022B P:00022B 0C0181            JMP     <STARTUP
844    
845                                 ;  ****************  X: Memory tables  ********************
846    
847                                 ; Define the address in P: space where the table of constants begins
848    
849                                  X_BOOT_START
850       00022A                              EQU     @LCV(L)-2
851    
852                                           IF      @SCP("HOST","ROM")
854                                           ENDIF
855                                           IF      @SCP("HOST","HOST")
856       X:000000 X:000000                   ORG     X:0,X:0
857                                           ENDIF
858    
859                                 ; Special storage area - initialization constants and scratch space
860       X:000000 X:000000         STATUS    DC      $100004                           ; Controller status bits
861    
862       000001                    FO_HDR    EQU     STATUS+1                          ; Fiber optic write bytes
863       000005                    HEADER    EQU     FO_HDR+4                          ; Command header
864       000006                    NWORDS    EQU     HEADER+1                          ; Number of words in the command
865       000007                    COM_BUF   EQU     NWORDS+1                          ; Command buffer
866       00000E                    SV_A1     EQU     COM_BUF+7                         ; Save accumulator A1
867    
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timboot.asm  Page 17



868                                           IF      @SCP("HOST","ROM")
873                                           ENDIF
874    
875                                           IF      @SCP("HOST","HOST")
876       X:00000F X:00000F                   ORG     X:$F,X:$F
877                                           ENDIF
878    
879                                 ; Parameter table in P: space to be copied into X: space during
880                                 ;   initialization, and is copied from ROM by the DSP boot
881       X:00000F X:00000F         LATCH     DC      $32                               ; Starting value in latch chip U25
882                                  EXPOSURE_TIME
883       X:000010 X:000010                   DC      0                                 ; Exposure time in milliseconds
884                                  ELAPSED_TIME
885       X:000011 X:000011                   DC      0                                 ; Time elapsed so far in the exposure
886       X:000012 X:000012         ONE       DC      1                                 ; One
887       X:000013 X:000013         TWO       DC      2                                 ; Two
888       X:000014 X:000014         THREE     DC      3                                 ; Three
889       X:000015 X:000015         SEVEN     DC      7                                 ; Seven
890       X:000016 X:000016         MASK1     DC      $FCFCF8                           ; Mask for checking header
891       X:000017 X:000017         MASK2     DC      $030300                           ; Mask for checking header
892       X:000018 X:000018         DONE      DC      'DON'                             ; Standard reply
893       X:000019 X:000019         SBRD      DC      $020000                           ; Source Identification number
894       X:00001A X:00001A         TIM_DRB   DC      $000200                           ; Destination = timing board number
895       X:00001B X:00001B         DMASK     DC      $00FF00                           ; Mask to get destination board number
896       X:00001C X:00001C         SMASK     DC      $FF0000                           ; Mask to get source board number
897       X:00001D X:00001D         ERR       DC      'ERR'                             ; An error occurred
898       X:00001E X:00001E         C100K     DC      100000                            ; Delay for WRROM = 1 millisec
899       X:00001F X:00001F         IDL_ADR   DC      TST_RCV                           ; Address of idling routine
900       X:000020 X:000020         EXP_ADR   DC      0                                 ; Jump to this address during exposures
901    
902                                 ; Places for saving register values
903       X:000021 X:000021         SAVE_SR   DC      0                                 ; Status Register
904       X:000022 X:000022         SAVE_X1   DC      0
905       X:000023 X:000023         SAVE_A1   DC      0
906       X:000024 X:000024         SAVE_R0   DC      0
907       X:000025 X:000025         RCV_ERR   DC      0
908       X:000026 X:000026         SCI_A1    DC      0                                 ; Contents of accumulator A1 in RCV ISR
909       X:000027 X:000027         SCI_R0    DC      SRXL
910    
911                                 ; Command table
912       000028                    COM_TBL_R EQU     @LCV(R)
913       X:000028 X:000028         COM_TBL   DC      'TDL',TDL                         ; Test Data Link
914       X:00002A X:00002A                   DC      'RDM',RDMEM                       ; Read from DSP or EEPROM memory
915       X:00002C X:00002C                   DC      'WRM',WRMEM                       ; Write to DSP memory
916       X:00002E X:00002E                   DC      'LDA',LDAPPL                      ; Load application from EEPROM to DSP
917       X:000030 X:000030                   DC      'STP',STOP_IDLE_CLOCKING
918       X:000032 X:000032                   DC      'DON',START                       ; Nothing special
919       X:000034 X:000034                   DC      'ERR',START                       ; Nothing special
920    
921                                  END_COMMAND_TABLE
922       000036                              EQU     @LCV(R)
923    
924                                 ; The table at SCI_TABLE is for words received from the utility board, written by
925                                 ;   the interrupt service routine SCI_RCV. Note that it is 32 words long,
926                                 ;   hard coded, and the 33rd location contains the pointer to words that have
927                                 ;   been processed by moving them from the SCI_TABLE to the COM_BUF.
928    
929                                           IF      @SCP("HOST","ROM")
931                                           ENDIF
932    
933       000036                    END_ADR   EQU     @LCV(L)                           ; End address of P: code written to ROM
934    
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 18



935       P:00022C P:00022C                   ORG     P:,P:
936    
937       168415                    CC        EQU     ARC22+ARC32+ARC46+TWO_XMTR+SUBARRAY+CONT_RD
938    
939                                 ; Put number of words of application in P: for loading application from EEPROM
940       P:00022C P:00022C                   DC      TIMBOOT_X_MEMORY-@LCV(L)-1
941    
942       000013                    ST_RDM    EQU     19                                ; Set if reading video channels one-by-one
943                                  ST_RST_MODE
944       000014                              EQU     20                                ; Set if array needs resetting in continuous
 readout mode
945       000015                    ST_WM     EQU     21                                ; Set if in native H2RG windowing mode
946       000016                    ST_RST    EQU     22                                ; Set if resetting the array
947       000017                    ST_XMT2   EQU     23                                ; Set if transmitting over two fiber optic l
inks
948       000003                    RD_MODE   EQU     3
949    
950                                 ;******************  Read out the H2RG array  **************************
951                                 ; Check for a command once per frame. Only the ABORT command should be issued.
952                                 RD_ARRAY
953                                 ;       BSET    #ST_RDC,X:<STATUS                       ; Set status to reading out
954                                 ;       JSR     <WAIT_TO_FINISH_CLOCKING
955                                 ;       MOVE    #RESET_FIFOS,A                          ; Reset ARC-46 image data FIFOs
956                                 ;       JSR     <WR_BIAS
957    
958                                 ; Exercise the second fiber optic transmitter if needed
959                                 ;       JCLR    #ST_XMT2,X:STATUS,NO2XMT
960                                 ;       BSET    #1,X:PDRC                               ; Transmit image data on second fo xmtr
961                                 ;NO2XMT JSET    #TST_IMG,X:STATUS,SYNTHETIC_IMAGE
962       P:00022D P:00022D 0A0024            BSET    #ST_RDC,X:<STATUS                 ; Set status to reading out
963       P:00022E P:00022E 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
964       P:00022F P:00022F 0D03DD            JSR     <PCI_READ_IMAGE                   ; Tell the PCI card the number of pixels to 
expect
965       P:000230 P:000230 0D023E            JSR     <CLOCK_H2RG                       ; Clock out the array normally
966       P:000231 P:000231 000000            NOP
967                                 ; This is code for continuous readout - check if more frames are needed
968                                 ;       MOVE    Y:<N_FRAMES,A                   ; Are we in continuous readout mode?
969                                 ;       CMP     #1,A
970                                 ;       JLE     <RDA_END
971       P:000232 P:000232 0A0004            BCLR    #ST_RDC,X:<STATUS                 ; Set status to not reading out
972       P:000233 P:000233 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
973       P:000234 P:000234 0D034A            JSR     <INC_FRAME_COUNT
974       P:000235 P:000235 000000            NOP
975       P:000236 P:000236 00000C            RTS
976    
977                                 ; Check for a command once. Only the ABORT command should be issued.
978                                 ;       MOVE    #COM_BUF,R3
979                                 ;       JSR     <GET_RCV                        ; Was a command received?
980                                 ;       JCC     <NEXT_FRAME                     ; If no, get the next frame
981                                 ;       JMP     <PRC_RCV                        ; If yes, go process it
982    
983                                 ; Restore the controller to non-image data transfer and idling if necessary
984       P:000237 P:000237 60F400  RDA_END   MOVE              #CONT_RST,R0            ; Continuously read array in idle mode
                            0002C3
985       P:000239 P:000239 601F00            MOVE              R0,X:<IDL_ADR
986       P:00023A P:00023A 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
987       P:00023B P:00023B 0A0004            BCLR    #ST_RDC,X:<STATUS                 ; Set status to not reading out
988       P:00023C P:00023C 013D01            BCLR    #1,X:PDRC                         ; Restore single transmitter mode
989                                 ;        RTS
990       P:00023D P:00023D 0C0054            JMP     <START
991    
992                                 ;*********  Clock out the H2RG array  *************
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 19



993                                 CLOCK_H2RG
994       P:00023E P:00023E 60F400            MOVE              #FRAME_INIT,R0          ; Initialize the frame for readout
                            000032
995       P:000240 P:000240 0D03EB            JSR     <CLOCK
996    
997                                 ; Skip over the unread rows, if in sub-array, non-windowing mode
998                                 ;       JCLR    #ST_SA,X:STATUS,L_RSKIP
999                                 ;       JSET    #ST_WM,X:STATUS,L_RSKIP
1000                                ;       DO      Y:<SA_STARTROW,L_RSKIP
1001                                ;       MOVE    #SKIP_ROW,R0
1002                                ;       JSR     <CLOCK
1003                                ;       NOP
1004                                L_RSKIP
1005   
1006                                ; Read the entire frame, clocking each row
1007      P:000241 P:000241 060440            DO      Y:<NROWS_CLOCK,L_FRAME
                            000259
1008      P:000243 P:000243 60F400            MOVE              #CLOCK_ROW,R0           ; Clock each row
                            000036
1009      P:000245 P:000245 0D03EB            JSR     <CLOCK
1010   
1011      P:000246 P:000246 060640            DO      Y:<READ_DELAY,R_DELAY             ; Delay by READ_DELAY microseconds
                            00024B
1012      P:000248 P:000248 60F400            MOVE              #ONE_MICROSEC_DELAY,R0
                            00006F
1013      P:00024A P:00024A 0D03EB            JSR     <CLOCK
1014      P:00024B P:00024B 000000            NOP
1015                                R_DELAY
1016   
1017                                ; Skip over the unread columns, if needed
1018                                ;       JCLR    #ST_SA,X:STATUS,L_CSKIP
1019                                ;       JSET    #ST_WM,X:STATUS,L_CSKIP
1020                                ;       DO      Y:<SA_NCOLS_CK,L_CSKIP
1021                                ;       MOVE    #SKIP_COL,R0
1022                                ;       JSR     <CLOCK
1023                                ;       NOP
1024                                L_CSKIP
1025   
1026                                ; H2RG requires 2 HCLK pulses before the first real pixel in each row
1027      P:00024C P:00024C 60F400            MOVE              #FIRST_HCLKS,R0
                            000066
1028      P:00024E P:00024E 0D03EB            JSR     <CLOCK
1029   
1030                                ; Finally, clock each row, read each pixel and transmit the A/D data
1031      P:00024F P:00024F 060340            DO      Y:<NCOLS_CLOCK,L_COLS
                            000254
1032      P:000251 P:000251 60F400            MOVE              #CLK_COL,R0
                            000048
1033      P:000253 P:000253 0D03EB            JSR     <CLOCK                            ; Clock and read each column
1034      P:000254 P:000254 000000            NOP
1035      P:000255 P:000255 000000  L_COLS    NOP
1036   
1037      P:000256 P:000256 60F400            MOVE              #LAST_HCLKS,R0
                            00006B
1038      P:000258 P:000258 0D03EB            JSR     <CLOCK
1039      P:000259 P:000259 000000            NOP
1040                                L_FRAME
1041      P:00025A P:00025A 60F400            MOVE              #OVERCLOCK_ROW,R0       ; Extra row clock at the end of frame
                            00003C
1042      P:00025C P:00025C 0D03EB            JSR     <CLOCK
1043      P:00025D P:00025D 00000C            RTS
1044   
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 20



1045   
1046                                ;*********  Line by Line reset with xmit  *************
1047                                CLOCK_RESET_LINE_BY_LINE_XMIT
1048      P:00025E P:00025E 60F400            MOVE              #FRAME_INIT,R0          ; Initialize the frame for readout
                            000032
1049      P:000260 P:000260 0D03EB            JSR     <CLOCK
1050                                ; Read the entire frame, clocking each row
1051      P:000261 P:000261 060440            DO      Y:<NROWS_CLOCK,L_FRAME_RES
                            000279
1052      P:000263 P:000263 60F400            MOVE              #CLOCK_ROW_RESET,R0     ; Clock and reset each row
                            000039
1053      P:000265 P:000265 0D03EB            JSR     <CLOCK
1054   
1055      P:000266 P:000266 060640            DO      Y:<READ_DELAY,R_DELAY_RES         ; Delay by READ_DELAY microseconds
                            00026B
1056      P:000268 P:000268 60F400            MOVE              #ONE_MICROSEC_DELAY,R0
                            00006F
1057      P:00026A P:00026A 0D03EB            JSR     <CLOCK
1058      P:00026B P:00026B 000000            NOP
1059                                R_DELAY_RES
1060   
1061                                ; H2RG requires 2 HCLK pulses before the first real pixel in each row
1062      P:00026C P:00026C 60F400            MOVE              #FIRST_HCLKS,R0
                            000066
1063      P:00026E P:00026E 0D03EB            JSR     <CLOCK
1064   
1065                                ; Finally, clock each row, read each pixel and transmit the A/D data
1066      P:00026F P:00026F 060340            DO      Y:<NCOLS_CLOCK,L_COLS_RES
                            000274
1067      P:000271 P:000271 60F400            MOVE              #CLK_COL,R0
                            000048
1068      P:000273 P:000273 0D03EB            JSR     <CLOCK                            ; Clock and read each column
1069      P:000274 P:000274 000000            NOP
1070                                L_COLS_RES
1071      P:000275 P:000275 000000            NOP
1072   
1073      P:000276 P:000276 60F400            MOVE              #LAST_HCLKS,R0
                            00006B
1074      P:000278 P:000278 0D03EB            JSR     <CLOCK
1075      P:000279 P:000279 000000            NOP
1076                                L_FRAME_RES
1077      P:00027A P:00027A 60F400            MOVE              #OVERCLOCK_ROW,R0       ; Extra row clock at the end of frame
                            00003C
1078      P:00027C P:00027C 0D03EB            JSR     <CLOCK
1079      P:00027D P:00027D 00000C            RTS
1080   
1081   
1082                                ;*********  Line by Line reset with no xmit, for manual global reset  *************
1083                                CLOCK_RESET_LINE_BY_LINE_NO_XMIT
1084      P:00027E P:00027E 60F400            MOVE              #FRAME_INIT,R0          ; Initialize the frame for readout
                            000032
1085      P:000280 P:000280 0D03EB            JSR     <CLOCK
1086                                ; Read the entire frame, clocking each row
1087      P:000281 P:000281 060440            DO      Y:<NROWS_CLOCK,L_FRAME_RES_NOX
                            000299
1088      P:000283 P:000283 60F400            MOVE              #CLOCK_ROW_RESET,R0     ; Clock and reset each row
                            000039
1089      P:000285 P:000285 0D03EB            JSR     <CLOCK
1090   
1091      P:000286 P:000286 060640            DO      Y:<READ_DELAY,R_DELAY_RES_NOX     ; Delay by READ_DELAY microseconds
                            00028B
1092      P:000288 P:000288 60F400            MOVE              #ONE_MICROSEC_DELAY,R0
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 21



                            00006F
1093      P:00028A P:00028A 0D03EB            JSR     <CLOCK
1094      P:00028B P:00028B 000000            NOP
1095                                R_DELAY_RES_NOX
1096   
1097                                ; H2RG requires 2 HCLK pulses before the first real pixel in each row
1098      P:00028C P:00028C 60F400            MOVE              #FIRST_HCLKS,R0
                            000066
1099      P:00028E P:00028E 0D03EB            JSR     <CLOCK
1100   
1101                                ; Finally, clock each row, read each pixel and transmit the A/D data
1102      P:00028F P:00028F 060340            DO      Y:<NCOLS_CLOCK,L_COLS_RES_NOX
                            000294
1103      P:000291 P:000291 60F400            MOVE              #CLK_COL_NO_XMIT,R0
                            000056
1104      P:000293 P:000293 0D03EB            JSR     <CLOCK                            ; Clock and read each column
1105      P:000294 P:000294 000000            NOP
1106                                L_COLS_RES_NOX
1107      P:000295 P:000295 000000            NOP
1108   
1109      P:000296 P:000296 60F400            MOVE              #LAST_HCLKS,R0
                            00006B
1110      P:000298 P:000298 0D03EB            JSR     <CLOCK
1111      P:000299 P:000299 000000            NOP
1112                                L_FRAME_RES_NOX
1113      P:00029A P:00029A 60F400            MOVE              #OVERCLOCK_ROW,R0       ; Extra row clock at the end of frame
                            00003C
1114      P:00029C P:00029C 0D03EB            JSR     <CLOCK
1115      P:00029D P:00029D 00000C            RTS
1116   
1117   
1118                                ;******************************************************************************
1119                                ; 1 H2RG Global reset (bad!) (2) DSP global reset (3) Line by Line reset
1120                                RESET_ARRAY
1121      P:00029E P:00029E 0A0036            BSET    #ST_RST,X:<STATUS
1122      P:00029F P:00029F 5E9500            MOVE                          Y:<RSTMODE,A
1123      P:0002A0 P:0002A0 014085            CMP     #0,A
1124      P:0002A1 P:0002A1 0AF0AA            JEQ     BACK_NO_RESET
                            0002B5
1125      P:0002A3 P:0002A3 014185            CMP     #1,A
1126      P:0002A4 P:0002A4 0AF0AA            JEQ     DETGBL
                            0002BB
1127      P:0002A6 P:0002A6 014285            CMP     #2,A
1128      P:0002A7 P:0002A7 0AF0AA            JEQ     MANGBL
                            0002B7
1129      P:0002A9 P:0002A9 000000            NOP
1130                                                                                    ;line by line reset and xmit
1131      P:0002AA P:0002AA 0A0024            BSET    #ST_RDC,X:<STATUS                 ; Set status to reading out
1132      P:0002AB P:0002AB 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
1133      P:0002AC P:0002AC 0D03DD            JSR     <PCI_READ_IMAGE                   ;
1134      P:0002AD P:0002AD 0D025E            JSR     <CLOCK_RESET_LINE_BY_LINE_XMIT    ;Clock out the array reseting lines
1135      P:0002AE P:0002AE 000000            NOP
1136      P:0002AF P:0002AF 0A0004            BCLR    #ST_RDC,X:<STATUS                 ; Set status to not reading out
1137      P:0002B0 P:0002B0 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
1138      P:0002B1 P:0002B1 0D034A            JSR     <INC_FRAME_COUNT
1139      P:0002B2 P:0002B2 000000            NOP
1140      P:0002B3 P:0002B3 0A0016            BCLR    #ST_RST,X:<STATUS
1141                                                                                    ;we already read sent 1 image, so we skip fi
rst frame and start with fowler (FS)
1142      P:0002B4 P:0002B4 0C0369            JMP     <FS_RST
1143   
1144                                BACK_NO_RESET
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 22



1145      P:0002B5 P:0002B5 0A0016            BCLR    #ST_RST,X:<STATUS
1146      P:0002B6 P:0002B6 00000C            RTS
1147   
1148      P:0002B7 P:0002B7 000000  MANGBL    NOP
1149                                                                                    ;here code to line to manual global reset
1150                                ;       JSR     <CLOCK_RESET_LINE_BY_LINE_NO_XMIT
1151      P:0002B8 P:0002B8 0D027E            JSR     <CLOCK_RESET_LINE_BY_LINE_NO_XMIT
1152      P:0002B9 P:0002B9 0A0016            BCLR    #ST_RST,X:<STATUS
1153      P:0002BA P:0002BA 00000C            RTS
1154                                DETGBL
1155      P:0002BB P:0002BB 0D02BD            JSR     <GBL_RESET
1156      P:0002BC P:0002BC 00000C            RTS
1157                                ;       JMP     <FIRST_FRAME
1158   
1159                                GBL_RESET
1160      P:0002BD P:0002BD 0A0036            BSET    #ST_RST,X:<STATUS
1161      P:0002BE P:0002BE 60F400            MOVE              #GLOBAL_RESET,R0
                            000062
1162      P:0002C0 P:0002C0 0D03EB            JSR     <CLOCK
1163      P:0002C1 P:0002C1 0A0016            BCLR    #ST_RST,X:<STATUS
1164      P:0002C2 P:0002C2 00000C            RTS
1165                                ;************************* !!! Not Working - Later !!!! *******************************
1166                                ; Continuously execute line-by-line reset array, checking for commands each line
1167                                CONT_RST
1168      P:0002C3 P:0002C3 060084            DO      #1024,L_RESET                     ; Clock entire FPA
                            0002CD
1169      P:0002C5 P:0002C5 60F400            MOVE              #CLOCK_ROW_RESET,R0     ; Reset one row
                            000039
1170      P:0002C7 P:0002C7 0D03EB            JSR     <CLOCK
1171   
1172      P:0002C8 P:0002C8 330700            MOVE              #<COM_BUF,R3
1173      P:0002C9 P:0002C9 0D00A5            JSR     <GET_RCV                          ; Look for a new command every 4 rows
1174      P:0002CA P:0002CA 0E02CD            JCC     <NO_COM                           ; If none, then stay here
1175      P:0002CB P:0002CB 00008C            ENDDO
1176      P:0002CC P:0002CC 0C005D            JMP     <PRC_RCV
1177      P:0002CD P:0002CD 000000  NO_COM    NOP
1178                                L_RESET
1179      P:0002CE P:0002CE 0C02C3            JMP     <CONT_RST
1180   
1181                                ; Include all the miscellaneous, generic support routines
1182                                          INCLUDE "timIRmisc.asm"
1183                                ; Clear all video processor analog switches to lower their power dissipation
1184   
1185                                POWER_OFF
1186      P:0002CF P:0002CF 0D030D            JSR     <CLEAR_SWITCHES_AND_DACS          ; Clear switches and DACs
1187      P:0002D0 P:0002D0 0A8922            BSET    #LVEN,X:HDR
1188      P:0002D1 P:0002D1 0A8923            BSET    #HVEN,X:HDR
1189      P:0002D2 P:0002D2 0C008F            JMP     <FINISH
1190   
1191                                ; Execute the power-on cycle, as a command
1192                                POWER_ON
1193      P:0002D3 P:0002D3 0D030D            JSR     <CLEAR_SWITCHES_AND_DACS          ; Clear switches and DACs
1194   
1195                                ; Turn on the low voltages (+/- 6.5V, +/- 16.5V) and then delay awhile
1196      P:0002D4 P:0002D4 0A8902            BCLR    #LVEN,X:HDR                       ; Turn on the low voltage power
1197      P:0002D5 P:0002D5 56F400            MOVE              #>100,A
                            000064
1198      P:0002D7 P:0002D7 0D039C            JSR     <MILLISEC_DELAY                   ; Wait one hundred milliseconds
1199      P:0002D8 P:0002D8 0A8980            JCLR    #PWROK,X:HDR,PWR_ERR              ; Test if the power turned on properly
                            0002E4
1200      P:0002DA P:0002DA 0D02E7            JSR     <SET_BIASES                       ; Turn on the DC bias supplies
1201      P:0002DB P:0002DB 60F400            MOVE              #CONT_RST,R0            ; --> continuous readout state
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 23



                            0002C3
1202      P:0002DD P:0002DD 601F00            MOVE              R0,X:<IDL_ADR
1203      P:0002DE P:0002DE 0A002F            BSET    #ST_DIRTY,X:<STATUS
1204      P:0002DF P:0002DF 60F400            MOVE              #RST_INTERNAL_REGISTERS,R0 ; Clear the H2RG internal registers
                            000081
1205      P:0002E1 P:0002E1 0D03EB            JSR     <CLOCK
1206      P:0002E2 P:0002E2 0D04DC            JSR     <INIT_H2RG                        ; Initialize the H2RG array
1207      P:0002E3 P:0002E3 0C008F            JMP     <FINISH
1208   
1209                                ; The power failed to turn on because of an error on the power control board
1210      P:0002E4 P:0002E4 0A8922  PWR_ERR   BSET    #LVEN,X:HDR                       ; Turn off the low voltage emable line
1211      P:0002E5 P:0002E5 0A8923            BSET    #HVEN,X:HDR                       ; Turn off the high voltage emable line
1212      P:0002E6 P:0002E6 0C008D            JMP     <ERROR
1213   
1214                                ; Set all the DC bias voltages and video processor offset values,
1215                                ;   reading them from the 'DACS' table
1216                                SET_BIASES
1217      P:0002E7 P:0002E7 012F23            BSET    #3,X:PCRD                         ; Turn on the serial clock
1218      P:0002E8 P:0002E8 0A0F01            BCLR    #1,X:<LATCH                       ; Separate updates of clock driver
1219      P:0002E9 P:0002E9 0A0F20            BSET    #CDAC,X:<LATCH                    ; Disable clearing of DACs
1220      P:0002EA P:0002EA 0A0F02            BCLR    #ENCK,X:<LATCH                    ; Disable clock and DAC output switches
1221      P:0002EB P:0002EB 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Write it to the hardware
                            00000F
1222      P:0002ED P:0002ED 0D03F6            JSR     <PAL_DLY                          ; Delay for all this to happen
1223   
1224                                ; Specialized turn-on sequence for H2RG
1225      P:0002EE P:0002EE 30F900            MOVE              #<ZERO_BIASES,R0        ; Zero out all the DC bias DACs
1226      P:0002EF P:0002EF 0D0303            JSR     <WR_TABLE
1227      P:0002F0 P:0002F0 56F400            MOVE              #>3,A
                            000003
1228      P:0002F2 P:0002F2 0D039C            JSR     <MILLISEC_DELAY                   ; Wait three milliseconds
1229      P:0002F3 P:0002F3 0A0F22            BSET    #ENCK,X:<LATCH                    ; Enable clock and DAC output switches
1230      P:0002F4 P:0002F4 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Write it to the hardware
                            00000F
1231   
1232      P:0002F6 P:0002F6 60F400            MOVE              #DC_BIASES,R0           ; Write to the DC bias DACS
                            00008A
1233      P:0002F8 P:0002F8 0D0303            JSR     <WR_TABLE
1234      P:0002F9 P:0002F9 56F400            MOVE              #>10,A
                            00000A
1235      P:0002FB P:0002FB 0D039C            JSR     <MILLISEC_DELAY                   ; Wait ten milliseconds
1236      P:0002FC P:0002FC 301800            MOVE              #<DACS,R0               ; Write to the clock driver DACs
1237      P:0002FD P:0002FD 0D0303            JSR     <WR_TABLE
1238      P:0002FE P:0002FE 56F400            MOVE              #>10,A
                            00000A
1239      P:000300 P:000300 0D039C            JSR     <MILLISEC_DELAY                   ; Wait ten milliseconds
1240   
1241      P:000301 P:000301 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1242      P:000302 P:000302 00000C            RTS
1243   
1244                                WR_TABLE
1245      P:000303 P:000303 065840            DO      Y:(R0)+,L_DAC0
                            000307
1246      P:000305 P:000305 5ED800            MOVE                          Y:(R0)+,A
1247      P:000306 P:000306 0D020C            JSR     <XMIT_A_WORD
1248      P:000307 P:000307 000000            NOP
1249                                L_DAC0
1250      P:000308 P:000308 00000C            RTS
1251   
1252                                SET_BIAS_VOLTAGES
1253      P:000309 P:000309 0D02E7            JSR     <SET_BIASES
1254      P:00030A P:00030A 0C008F            JMP     <FINISH
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 24



1255   
1256      P:00030B P:00030B 0D030D  CLR_SWS   JSR     <CLEAR_SWITCHES_AND_DACS          ; Clear switches and DACs
1257      P:00030C P:00030C 0C008F            JMP     <FINISH
1258   
1259                                CLEAR_SWITCHES_AND_DACS
1260      P:00030D P:00030D 0A0F00            BCLR    #CDAC,X:<LATCH                    ; Clear all the DACs
1261      P:00030E P:00030E 0A0F02            BCLR    #ENCK,X:<LATCH                    ; Disable all the output switches
1262      P:00030F P:00030F 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Write it to the hardware
                            00000F
1263      P:000311 P:000311 012F23            BSET    #3,X:PCRD                         ; Turn the serial clock on
1264      P:000312 P:000312 0D03F6            JSR     <PAL_DLY
1265      P:000313 P:000313 30F900            MOVE              #<ZERO_BIASES,R0
1266      P:000314 P:000314 0D0303            JSR     <WR_TABLE
1267      P:000315 P:000315 56F400            MOVE              #GAIN,A                 ; Video processor gain, x1 or x4
                            0C3000
1268      P:000317 P:000317 20001B            CLR     B
1269      P:000318 P:000318 241000            MOVE              #$100000,X0             ; Increment over board numbers for DAC write
s
1270      P:000319 P:000319 45F400            MOVE              #$001000,X1             ; Increment over board numbers for WRSS writ
es
                            001000
1271      P:00031B P:00031B 060880            DO      #8,L_VIDEO                        ; Eight video processor boards maximum
                            000322
1272      P:00031D P:00031D 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
1273      P:00031E P:00031E 200040            ADD     X0,A
1274      P:00031F P:00031F 5F7000            MOVE                          B,Y:WRSS
                            FFFFF3
1275      P:000321 P:000321 0D03F6            JSR     <PAL_DLY                          ; Delay for the serial data transmission
1276      P:000322 P:000322 200068            ADD     X1,B
1277                                L_VIDEO
1278      P:000323 P:000323 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1279      P:000324 P:000324 00000C            RTS
1280   
1281                                ; Start the exposure timer and monitor its progress
1282      P:000325 P:000325 07F40E  EXPOSE    MOVEP             #0,X:TLR0               ; Load 0 into counter timer
                            000000
1283      P:000327 P:000327 240000            MOVE              #0,X0
1284      P:000328 P:000328 441100            MOVE              X0,X:<ELAPSED_TIME      ; Set elapsed exposure time to zero
1285      P:000329 P:000329 579000            MOVE              X:<EXPOSURE_TIME,B
1286      P:00032A P:00032A 20000B            TST     B                                 ; Special test for zero exposure time
1287      P:00032B P:00032B 0EA335            JEQ     <END_EXP                          ; Don't even start an exposure
1288      P:00032C P:00032C 01418C            SUB     #1,B                              ; Timer counts from X:TCPR0+1 to zero
1289      P:00032D P:00032D 010F20            BSET    #TIM_BIT,X:TCSR0                  ; Enable the timer #0
1290      P:00032E P:00032E 577000            MOVE              B,X:TCPR0
                            FFFF8D
1291      P:000330 P:000330 330700  CHK_RCV   MOVE              #<COM_BUF,R3            ; The beginning of the command buffer
1292      P:000331 P:000331 0D00A5            JSR     <GET_RCV                          ; Check for an incoming command
1293      P:000332 P:000332 0E805D            JCS     <PRC_RCV                          ; If command is received, go check it
1294      P:000333 P:000333 018F95  CHK_TIM   JCLR    #TCF,X:TCSR0,CHK_RCV              ; Wait for timer to equal compare value
                            000330
1295      P:000335 P:000335 010F00  END_EXP   BCLR    #TIM_BIT,X:TCSR0                  ; Disable the timer
1296      P:000336 P:000336 0AE780            JMP     (R7)                              ; This contains the return address
1297   
1298                                ; Start the exposure and initiate FPA readout
1299                                START_EXPOSURE
1300   
1301                                ; Setup windowing or whole image readout mode if needed
1302                                ;       JSSET   #ST_DIRTY,X:STATUS,SETUP_READ_MODE
1303                                ;       MOVE    #TST_RCV,R0             ; Process commands, don't idle
1304                                ;       MOVE    R0,X:<IDL_ADR           ;    during the exposure
1305      P:000337 P:000337 0D0506            JSR     <WHOLE_FRAME
1306                                ; Test for continuous readout
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 25



1307                                ;       MOVE    Y:<N_FRAMES,A
1308                                ;       CMP     #1,A
1309                                ;       JLE     <INIT_PCI_BOARD
1310      P:000338 P:000338 000000            NOP
1311      P:000339 P:000339 0D033E            JSR     <INIT_FRAME_COUNT
1312      P:00033A P:00033A 000000            NOP
1313      P:00033B P:00033B 0D0353            JSR     <INIT_PCI_BOARD
1314      P:00033C P:00033C 000000            NOP
1315      P:00033D P:00033D 0C0363            JMP     <RST_AR
1316   
1317                                INIT_FRAME_COUNT
1318      P:00033E P:00033E 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
1319      P:00033F P:00033F 57F400            MOVE              #$020102,B              ; Initialize the PCI frame counter
                            020102
1320      P:000341 P:000341 0D00EB            JSR     <XMT_WRD
1321      P:000342 P:000342 57F400            MOVE              #'IFC',B
                            494643
1322      P:000344 P:000344 0D00EB            JSR     <XMT_WRD
1323      P:000345 P:000345 240000            MOVE              #0,X0
1324      P:000346 P:000346 4C0C00            MOVE                          X0,Y:<I_FRAME ; Initialize the frame number
1325      P:000347 P:000347 0D0353            JSR     <INIT_PCI_BOARD
1326      P:000348 P:000348 000000            NOP
1327      P:000349 P:000349 00000C            RTS
1328   
1329                                INC_FRAME_COUNT
1330      P:00034A P:00034A 5E8D00            MOVE                          Y:<IBUFFER,A ; Get the position in the buffer
1331      P:00034B P:00034B 014180            ADD     #1,A
1332      P:00034C P:00034C 4C8E00            MOVE                          Y:<N_FPB,X0
1333      P:00034D P:00034D 5C0D00            MOVE                          A1,Y:<IBUFFER
1334      P:00034E P:00034E 200045            CMP     X0,A
1335      P:00034F P:00034F 0E9352            JLT     <BYE                              ; Test if the frame buffer is full
1336      P:000350 P:000350 0D0353            JSR     <INIT_PCI_BOARD
1337      P:000351 P:000351 000000            NOP
1338                                BYE
1339      P:000352 P:000352 00000C            RTS
1340   
1341                                INIT_PCI_BOARD
1342      P:000353 P:000353 240000            MOVE              #0,X0
1343      P:000354 P:000354 4C0D00            MOVE                          X0,Y:<IBUFFER ; IBUFFER counts from 0 to N_FPB
1344      P:000355 P:000355 57F400            MOVE              #$020102,B
                            020102
1345      P:000357 P:000357 0D00EB            JSR     <XMT_WRD
1346      P:000358 P:000358 57F400            MOVE              #'IIA',B                ; Initialize the PCI image address
                            494941
1347      P:00035A P:00035A 0D00EB            JSR     <XMT_WRD
1348      P:00035B P:00035B 000000            NOP
1349      P:00035C P:00035C 00000C            RTS
1350   
1351                                ; Start up the next frame of the coaddition series^M
1352                                NEXT_FRAME
1353      P:00035D P:00035D 5E8C00            MOVE                          Y:<I_FRAME,A ; Get the # of frames coadded so far^M
1354      P:00035E P:00035E 014180            ADD     #1,A
1355      P:00035F P:00035F 4C8B00            MOVE                          Y:<N_FRAMES,X0 ; See if we've coadded enough frames^M
1356      P:000360 P:000360 5C0C00            MOVE                          A1,Y:<I_FRAME ; Increment the coaddition frame counter^M
1357      P:000361 P:000361 200045            CMP     X0,A
1358      P:000362 P:000362 0E1237            JGE     <RDA_END                          ; End of coaddition sequence^M
1359   
1360                                ; Clear the FPA if needed, and expose
1361                                RST_AR
1362      P:000363 P:000363 0A0091            JCLR    #ST_CDS,X:STATUS,NO_CDS
                            00036F
1363      P:000365 P:000365 0B00B4            JSSET   #ST_RST_MODE,X:STATUS,RESET_ARRAY ; Reset array if needed
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 26



                            00029E
1364                                ;first reset read, not done id reset line by line (already xmited this one)
1365                                 FIRST_FRAME
1366      P:000367 P:000367 0D022D            JSR     <RD_ARRAY
1367      P:000368 P:000368 000000            NOP
1368                                FS_RST
1369      P:000369 P:000369 000000            NOP
1370      P:00036A P:00036A 061740            DO      Y:<NFS_1,FS_RSTEND
                            00036D
1371      P:00036C P:00036C 0D022D            JSR     <RD_ARRAY
1372      P:00036D P:00036D 000000            NOP
1373                                FS_RSTEND
1374      P:00036E P:00036E 0D03E8            JSR     <WAIT_TO_FINISH_CLOCKING
1375      P:00036F P:00036F 67F400  NO_CDS    MOVE              #L_SEX1,R7              ; Return address at end of exposure
                            000374
1376      P:000371 P:000371 061440            DO      Y:<NUTR,S_UP_THE_RAMP
                            000378
1377      P:000373 P:000373 0C0325            JMP     <EXPOSE                           ; Delay for specified exposure time
1378                                L_SEX1
1379      P:000374 P:000374 061640            DO      Y:<NFS,FS_END
                            000377
1380      P:000376 P:000376 0D022D            JSR     <RD_ARRAY                         ; Finally, go read out the FPA
1381      P:000377 P:000377 000000            NOP
1382                                FS_END
1383      P:000378 P:000378 000000            NOP
1384                                S_UP_THE_RAMP
1385      P:000379 P:000379 0C035D            JMP     <NEXT_FRAME
1386      P:00037A P:00037A 000000            NOP
1387      P:00037B P:00037B 0C0054            JMP     <START
1388   
1389                                ; Set the desired exposure time
1390                                SET_EXPOSURE_TIME
1391      P:00037C P:00037C 46DB00            MOVE              X:(R3)+,Y0
1392      P:00037D P:00037D 461000            MOVE              Y0,X:EXPOSURE_TIME
1393      P:00037E P:00037E 018F80            JCLR    #TIM_BIT,X:TCSR0,FINISH           ; Return if exposure not occurring
                            00008F
1394      P:000380 P:000380 467000            MOVE              Y0,X:TCPR0              ; Update timer if exposure in progress
                            FFFF8D
1395      P:000382 P:000382 0C008F            JMP     <FINISH
1396   
1397                                ; Read the time remaining until the exposure ends
1398                                READ_EXPOSURE_TIME
1399      P:000383 P:000383 018FA0            JSET    #TIM_BIT,X:TCSR0,RD_TIM           ; Read DSP timer if its running
                            000387
1400      P:000385 P:000385 479100            MOVE              X:<ELAPSED_TIME,Y1
1401      P:000386 P:000386 0C0090            JMP     <FINISH1
1402      P:000387 P:000387 47F000  RD_TIM    MOVE              X:TCR0,Y1               ; Read elapsed exposure time
                            FFFF8C
1403      P:000389 P:000389 0C0090            JMP     <FINISH1
1404   
1405                                ; Pause the exposure - just stop the timer
1406                                PAUSE_EXPOSURE
1407      P:00038A P:00038A 07700C            MOVEP             X:TCR0,X:ELAPSED_TIME   ; Save the elapsed exposure time
                            000011
1408      P:00038C P:00038C 010F00            BCLR    #TIM_BIT,X:TCSR0                  ; Disable the DSP exposure timer
1409      P:00038D P:00038D 0C008F            JMP     <FINISH
1410   
1411                                ; Resume the exposure - restart the timer
1412                                RESUME_EXPOSURE
1413      P:00038E P:00038E 07F00E            MOVEP             X:ELAPSED_TIME,X:TLR0   ; Restore elapsed exposure time
                            000011
1414      P:000390 P:000390 010F20            BSET    #TIM_BIT,X:TCSR0                  ; Re-enable the DSP exposure timer
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 27



1415      P:000391 P:000391 0C008F            JMP     <FINISH
1416   
1417                                ; Enable continuous readout mode
1418      P:000392 P:000392 60F400  IDLE      MOVE              #CONT_RST,R0
                            0002C3
1419      P:000394 P:000394 601F00            MOVE              R0,X:<IDL_ADR
1420      P:000395 P:000395 0C008F            JMP     <FINISH
1421   
1422                                ; Exit continuous readout mode
1423      P:000396 P:000396 305A00  STP       MOVE              #TST_RCV,R0
1424      P:000397 P:000397 601F00            MOVE              R0,X:<IDL_ADR
1425      P:000398 P:000398 0C008F            JMP     <FINISH
1426   
1427                                ; Abort exposure - stop the timer and resume continuous readout mode
1428                                ABORT_EXPOSURE
1429      P:000399 P:000399 010F00            BCLR    #TIM_BIT,X:TCSR0                  ; Disable the DSP exposure timer
1430      P:00039A P:00039A 0D0237            JSR     <RDA_END
1431      P:00039B P:00039B 0C0054            JMP     <START
1432   
1433                                ; Delay by by the number of milliseconds in Accumulator A1
1434                                MILLISEC_DELAY
1435      P:00039C P:00039C 200003            TST     A
1436      P:00039D P:00039D 0E239F            JNE     <DLY_IT
1437      P:00039E P:00039E 00000C            RTS
1438      P:00039F P:00039F 014184  DLY_IT    SUB     #1,A
1439      P:0003A0 P:0003A0 07F40E            MOVEP             #0,X:TLR0               ; Load 0 into counter timer
                            000000
1440      P:0003A2 P:0003A2 010F20            BSET    #TIM_BIT,X:TCSR0                  ; Enable the timer #0
1441      P:0003A3 P:0003A3 567000            MOVE              A,X:TCPR0               ; Desired elapsed time
                            FFFF8D
1442      P:0003A5 P:0003A5 018F95  CNT_DWN   JCLR    #TCF,X:TCSR0,CNT_DWN              ; Wait here for timer to count down
                            0003A5
1443      P:0003A7 P:0003A7 010F00            BCLR    #TIM_BIT,X:TCSR0
1444      P:0003A8 P:0003A8 00000C            RTS
1445   
1446                                ; Generate a synthetic image by simply incrementing the pixel counts
1447                                SYNTHETIC_IMAGE
1448      P:0003A9 P:0003A9 200013            CLR     A
1449      P:0003AA P:0003AA 060240            DO      Y:<NROWS,LPR_TST                  ; Loop over each line readout
                            0003B5
1450      P:0003AC P:0003AC 060140            DO      Y:<NCOLS,LSR_TST                  ; Loop over number of pixels per line
                            0003B4
1451      P:0003AE P:0003AE 0614A0            REP     #20                               ; #20 => 1.0 microsec per pixel
1452      P:0003AF P:0003AF 000000            NOP
1453      P:0003B0 P:0003B0 014180            ADD     #1,A                              ; Pixel data = Pixel data + 1
1454      P:0003B1 P:0003B1 000000            NOP
1455      P:0003B2 P:0003B2 21CF00            MOVE              A,B
1456      P:0003B3 P:0003B3 0D03B8            JSR     <XMT_PIX                          ;  transmit them
1457      P:0003B4 P:0003B4 000000            NOP
1458                                LSR_TST
1459      P:0003B5 P:0003B5 000000            NOP
1460                                LPR_TST
1461      P:0003B6 P:0003B6 0D0237            JSR     <RDA_END                          ; Normal exit
1462      P:0003B7 P:0003B7 0C0054            JMP     <START
1463   
1464                                ; Transmit the 16-bit pixel datum in B1 to the host computer
1465      P:0003B8 P:0003B8 0C1DA1  XMT_PIX   ASL     #16,B,B
1466      P:0003B9 P:0003B9 000000            NOP
1467      P:0003BA P:0003BA 216500            MOVE              B2,X1
1468      P:0003BB P:0003BB 0C1D91            ASL     #8,B,B
1469      P:0003BC P:0003BC 000000            NOP
1470      P:0003BD P:0003BD 216400            MOVE              B2,X0
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 28



1471      P:0003BE P:0003BE 000000            NOP
1472      P:0003BF P:0003BF 09C532            MOVEP             X1,Y:WRFO
1473      P:0003C0 P:0003C0 09C432            MOVEP             X0,Y:WRFO
1474      P:0003C1 P:0003C1 00000C            RTS
1475   
1476                                ; Test the hardware to read A/D values directly into the DSP instead
1477                                ;   of using the SXMIT option, A/Ds #2 and 3.
1478      P:0003C2 P:0003C2 57F000  READ_AD   MOVE              X:(RDAD+2),B
                            010002
1479      P:0003C4 P:0003C4 0C1DA1            ASL     #16,B,B
1480      P:0003C5 P:0003C5 000000            NOP
1481      P:0003C6 P:0003C6 216500            MOVE              B2,X1
1482      P:0003C7 P:0003C7 0C1D91            ASL     #8,B,B
1483      P:0003C8 P:0003C8 000000            NOP
1484      P:0003C9 P:0003C9 216400            MOVE              B2,X0
1485      P:0003CA P:0003CA 000000            NOP
1486      P:0003CB P:0003CB 09C532            MOVEP             X1,Y:WRFO
1487      P:0003CC P:0003CC 09C432            MOVEP             X0,Y:WRFO
1488      P:0003CD P:0003CD 060AA0            REP     #10
1489      P:0003CE P:0003CE 000000            NOP
1490      P:0003CF P:0003CF 57F000            MOVE              X:(RDAD+3),B
                            010003
1491      P:0003D1 P:0003D1 0C1DA1            ASL     #16,B,B
1492      P:0003D2 P:0003D2 000000            NOP
1493      P:0003D3 P:0003D3 216500            MOVE              B2,X1
1494      P:0003D4 P:0003D4 0C1D91            ASL     #8,B,B
1495      P:0003D5 P:0003D5 000000            NOP
1496      P:0003D6 P:0003D6 216400            MOVE              B2,X0
1497      P:0003D7 P:0003D7 000000            NOP
1498      P:0003D8 P:0003D8 09C532            MOVEP             X1,Y:WRFO
1499      P:0003D9 P:0003D9 09C432            MOVEP             X0,Y:WRFO
1500      P:0003DA P:0003DA 060AA0            REP     #10
1501      P:0003DB P:0003DB 000000            NOP
1502      P:0003DC P:0003DC 00000C            RTS
1503   
1504                                ; Alert the PCI interface board that images are coming soon
1505                                PCI_READ_IMAGE
1506      P:0003DD P:0003DD 57F400            MOVE              #$020104,B              ; Send header word to the FO transmitter
                            020104
1507      P:0003DF P:0003DF 0D00EB            JSR     <XMT_WRD
1508      P:0003E0 P:0003E0 57F400            MOVE              #'RDA',B
                            524441
1509      P:0003E2 P:0003E2 0D00EB            JSR     <XMT_WRD
1510      P:0003E3 P:0003E3 5F8100            MOVE                          Y:<NCOLS,B  ; Number of columns to read
1511      P:0003E4 P:0003E4 0D00EB            JSR     <XMT_WRD
1512      P:0003E5 P:0003E5 5F8200            MOVE                          Y:<NROWS,B  ; Number of rows to read
1513                                ;       JCLR    #ST_CDS,X:<STATUS,NO_CDS1
1514                                ;       LSR     B
1515      P:0003E6 P:0003E6 0D00EB  NO_CDS1   JSR     <XMT_WRD
1516      P:0003E7 P:0003E7 00000C            RTS
1517   
1518                                ; Wait for the clocking to be complete before proceeding
1519                                WAIT_TO_FINISH_CLOCKING
1520      P:0003E8 P:0003E8 01ADA1            JSET    #SSFEF,X:PDRD,*                   ; Wait for the SS FIFO to be empty
                            0003E8
1521      P:0003EA P:0003EA 00000C            RTS
1522   
1523                                ; This MOVEP instruction executes in 30 nanosec, 20 nanosec for the MOVEP,
1524                                ;   and 10 nanosec for the wait state that is required for SRAM writes and
1525                                ;   FIFO setup times. It looks reliable, so will be used for now.
1526   
1527                                ; Core subroutine for clocking out FPA charge
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 29



1528      P:0003EB P:0003EB 0A898E  CLOCK     JCLR    #SSFHF,X:HDR,*                    ; Only write to FIFO if < half full
                            0003EB
1529      P:0003ED P:0003ED 000000            NOP
1530      P:0003EE P:0003EE 0A898E            JCLR    #SSFHF,X:HDR,CLOCK                ; Guard against metastability
                            0003EB
1531      P:0003F0 P:0003F0 4CD800            MOVE                          Y:(R0)+,X0  ; # of waveform entries
1532      P:0003F1 P:0003F1 06C400            DO      X0,CLK1                           ; Repeat X0 times
                            0003F3
1533      P:0003F3 P:0003F3 09D8F3            MOVEP             Y:(R0)+,Y:WRSS          ; 30 nsec Write the waveform to the SS
1534                                CLK1
1535      P:0003F4 P:0003F4 000000            NOP
1536      P:0003F5 P:0003F5 00000C            RTS                                       ; Return from subroutine
1537   
1538                                ; Delay for serial writes to the PALs and DACs by 8 microsec
1539      P:0003F6 P:0003F6 062083  PAL_DLY   DO      #800,DLY                          ; Wait 8 usec for serial data transmission
                            0003F8
1540      P:0003F8 P:0003F8 000000            NOP
1541      P:0003F9 P:0003F9 000000  DLY       NOP
1542      P:0003FA P:0003FA 00000C            RTS
1543   
1544                                ; Write a number to an analog board over the serial link
1545      P:0003FB P:0003FB 012F23  WR_BIAS   BSET    #3,X:PCRD                         ; Turn on the serial clock
1546      P:0003FC P:0003FC 0D03F6            JSR     <PAL_DLY
1547      P:0003FD P:0003FD 0D020C            JSR     <XMIT_A_WORD                      ; Transmit it to TIM-A-STD
1548      P:0003FE P:0003FE 0D03F6            JSR     <PAL_DLY
1549      P:0003FF P:0003FF 012F03            BCLR    #3,X:PCRD                         ; Turn off the serial clock
1550      P:000400 P:000400 0D03F6            JSR     <PAL_DLY
1551      P:000401 P:000401 00000C            RTS
1552   
1553                                ; Let the host computer read the controller configuration
1554                                READ_CONTROLLER_CONFIGURATION
1555      P:000402 P:000402 4F8500            MOVE                          Y:<CONFIG,Y1 ; Just transmit the configuration
1556      P:000403 P:000403 0C0090            JMP     <FINISH1
1557   
1558                                ; Set a particular DAC numbers, for setting DC bias voltages on the ARC32
1559                                ;   clock driver and ARC46 IR video processor
1560                                ;
1561                                ; SBN  #BOARD  #DAC  ['CLK' or 'VID'] voltage
1562                                ;
1563                                ;                               #BOARD is from 0 to 15
1564                                ;                               #DAC number
1565                                ;                               #voltage is from 0 to 4095
1566   
1567                                SET_BIAS_NUMBER                                     ; Set bias number
1568      P:000404 P:000404 012F23            BSET    #3,X:PCRD                         ; Turn on the serial clock
1569      P:000405 P:000405 56DB00            MOVE              X:(R3)+,A               ; First argument is board number, 0 to 15
1570      P:000406 P:000406 0614A0            REP     #20
1571      P:000407 P:000407 200033            LSL     A
1572      P:000408 P:000408 000000            NOP
1573      P:000409 P:000409 21C500            MOVE              A,X1                    ; Save the board number
1574      P:00040A P:00040A 56DB00            MOVE              X:(R3)+,A               ; Second argument is DAC number
1575      P:00040B P:00040B 57DB00            MOVE              X:(R3)+,B               ; Third argument is 'VID' or 'CLK' string
1576      P:00040C P:00040C 0140CD            CMP     #'VID',B
                            564944
1577      P:00040E P:00040E 0EA449            JEQ     <VID_SET
1578      P:00040F P:00040F 0140CD            CMP     #'CLK',B
                            434C4B
1579      P:000411 P:000411 0E2446            JNE     <ERR_SBN
1580   
1581                                ; For ARC32 do some trickiness to set the chip select and address bits
1582      P:000412 P:000412 218F00            MOVE              A1,B
1583      P:000413 P:000413 060EA0            REP     #14
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 30



1584      P:000414 P:000414 200033            LSL     A
1585      P:000415 P:000415 240E00            MOVE              #$0E0000,X0
1586      P:000416 P:000416 200046            AND     X0,A
1587      P:000417 P:000417 44F400            MOVE              #>7,X0
                            000007
1588      P:000419 P:000419 20004E            AND     X0,B                              ; Get 3 least significant bits of clock #
1589      P:00041A P:00041A 01408D            CMP     #0,B
1590      P:00041B P:00041B 0E241E            JNE     <CLK_1
1591      P:00041C P:00041C 0ACE68            BSET    #8,A
1592      P:00041D P:00041D 0C0439            JMP     <BD_SET
1593      P:00041E P:00041E 01418D  CLK_1     CMP     #1,B
1594      P:00041F P:00041F 0E2422            JNE     <CLK_2
1595      P:000420 P:000420 0ACE69            BSET    #9,A
1596      P:000421 P:000421 0C0439            JMP     <BD_SET
1597      P:000422 P:000422 01428D  CLK_2     CMP     #2,B
1598      P:000423 P:000423 0E2426            JNE     <CLK_3
1599      P:000424 P:000424 0ACE6A            BSET    #10,A
1600      P:000425 P:000425 0C0439            JMP     <BD_SET
1601      P:000426 P:000426 01438D  CLK_3     CMP     #3,B
1602      P:000427 P:000427 0E242A            JNE     <CLK_4
1603      P:000428 P:000428 0ACE6B            BSET    #11,A
1604      P:000429 P:000429 0C0439            JMP     <BD_SET
1605      P:00042A P:00042A 01448D  CLK_4     CMP     #4,B
1606      P:00042B P:00042B 0E242E            JNE     <CLK_5
1607      P:00042C P:00042C 0ACE6D            BSET    #13,A
1608      P:00042D P:00042D 0C0439            JMP     <BD_SET
1609      P:00042E P:00042E 01458D  CLK_5     CMP     #5,B
1610      P:00042F P:00042F 0E2432            JNE     <CLK_6
1611      P:000430 P:000430 0ACE6E            BSET    #14,A
1612      P:000431 P:000431 0C0439            JMP     <BD_SET
1613      P:000432 P:000432 01468D  CLK_6     CMP     #6,B
1614      P:000433 P:000433 0E2436            JNE     <CLK_7
1615      P:000434 P:000434 0ACE6F            BSET    #15,A
1616      P:000435 P:000435 0C0439            JMP     <BD_SET
1617      P:000436 P:000436 01478D  CLK_7     CMP     #7,B
1618      P:000437 P:000437 0E2439            JNE     <BD_SET
1619      P:000438 P:000438 0ACE70            BSET    #16,A
1620   
1621      P:000439 P:000439 200062  BD_SET    OR      X1,A                              ; Add on the board number
1622      P:00043A P:00043A 000000            NOP
1623      P:00043B P:00043B 21C400            MOVE              A,X0
1624      P:00043C P:00043C 56DB00            MOVE              X:(R3)+,A               ; Fourth argument is voltage value, 0 to $ff
f
1625      P:00043D P:00043D 0C1EC8            LSR     #4,A                              ; Convert 12 bits to 8 bits for ARC32
1626      P:00043E P:00043E 46F400            MOVE              #>$FF,Y0                ; Mask off just 8 bits
                            0000FF
1627      P:000440 P:000440 200056            AND     Y0,A
1628      P:000441 P:000441 200042            OR      X0,A
1629      P:000442 P:000442 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
1630      P:000443 P:000443 0D03F6            JSR     <PAL_DLY                          ; Wait for the number to be sent
1631      P:000444 P:000444 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1632      P:000445 P:000445 0C008F            JMP     <FINISH
1633      P:000446 P:000446 56DB00  ERR_SBN   MOVE              X:(R3)+,A               ; Read and discard the fourth argument
1634      P:000447 P:000447 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1635      P:000448 P:000448 0C008D            JMP     <ERROR
1636   
1637                                ; The command is for the DC biases on the ARC-46 video board
1638      P:000449 P:000449 0C1E9C  VID_SET   LSL     #14,A                             ; Put the DAC number 0-7 into bits 16-14
1639      P:00044A P:00044A 000000            NOP
1640      P:00044B P:00044B 0ACC73            BSET    #19,A1                            ; Set bits to mean video processor DAC
1641      P:00044C P:00044C 000000            NOP
1642      P:00044D P:00044D 0ACC72            BSET    #18,A1
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 31



1643      P:00044E P:00044E 44DB00            MOVE              X:(R3)+,X0              ; Fourth argument is voltage value for ARC46
,
1644      P:00044F P:00044F 200042            OR      X0,A                              ;  12 bits, bits 11-0
1645      P:000450 P:000450 200062            OR      X1,A                              ; Add on the board number, bits 23-20
1646      P:000451 P:000451 000000            NOP
1647      P:000452 P:000452 5C0000            MOVE                          A1,Y:0      ; Save the DAC number for a little while
1648      P:000453 P:000453 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
1649      P:000454 P:000454 0D03F6            JSR     <PAL_DLY                          ; Wait for the number to be sent
1650      P:000455 P:000455 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1651      P:000456 P:000456 0C008F            JMP     <FINISH
1652   
1653                                ; Set the ARC-46 video board video offsets -
1654                                ;
1655                                ;       SVO  #BOARD  #DAC  number
1656   
1657                                SET_VIDEO_OFFSET
1658      P:000457 P:000457 012F23            BSET    #3,X:PCRD                         ; Turn on the serial clock
1659      P:000458 P:000458 0D03F6            JSR     <PAL_DLY                          ; Let the serial transmitter get started
1660      P:000459 P:000459 56DB00            MOVE              X:(R3)+,A               ; First argument is board number, 0 to 15
1661      P:00045A P:00045A 0C1EA8            LSL     #20,A
1662      P:00045B P:00045B 000000            NOP
1663      P:00045C P:00045C 21C500            MOVE              A,X1                    ; Board number, bits 23-20
1664      P:00045D P:00045D 56DB00            MOVE              X:(R3)+,A               ; Second argument is DAC number
1665      P:00045E P:00045E 0C1E9C            LSL     #14,A                             ; Put the DAC number 0-7 into bits 16-14
1666      P:00045F P:00045F 000000            NOP
1667      P:000460 P:000460 200062            OR      X1,A                              ; Add on the board number, bits 23-20
1668      P:000461 P:000461 000000            NOP
1669      P:000462 P:000462 0140C2            OR      #$0E0000,A                        ; Set bits 19-17 to mean video offset DAC
                            0E0000
1670      P:000464 P:000464 44DB00            MOVE              X:(R3)+,X0              ; Third argument is voltage value for ARC46,
1671      P:000465 P:000465 200042            OR      X0,A                              ;  12 bits, bits 11-0
1672      P:000466 P:000466 000000            NOP
1673      P:000467 P:000467 5C0000            MOVE                          A1,Y:0      ; Save the DAC number for a little while
1674      P:000468 P:000468 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
1675      P:000469 P:000469 0D03F6            JSR     <PAL_DLY                          ; Wait for the number to be sent
1676      P:00046A P:00046A 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1677      P:00046B P:00046B 0C008F            JMP     <FINISH
1678   
1679                                ; Specify the MUX value to be output on the clock driver board
1680                                ; Command syntax is  SMX  #clock_driver_board #MUX1 #MUX2
1681                                ;                               #clock_driver_board from 0 to 15
1682                                ;                               #MUX1, #MUX2 from 0 to 23
1683   
1684      P:00046C P:00046C 012F23  SET_MUX   BSET    #3,X:PCRD                         ; Turn on the serial clock
1685      P:00046D P:00046D 56DB00            MOVE              X:(R3)+,A               ; Clock driver board number
1686      P:00046E P:00046E 0C1EA8            LSL     #20,A
1687      P:00046F P:00046F 44F400            MOVE              #$003000,X0             ; Bits to select MUX on ARC32 board
                            003000
1688      P:000471 P:000471 200042            OR      X0,A
1689      P:000472 P:000472 000000            NOP
1690      P:000473 P:000473 218500            MOVE              A1,X1                   ; Move here for later use
1691   
1692                                ; Get the first MUX number
1693      P:000474 P:000474 56DB00            MOVE              X:(R3)+,A               ; Get the first MUX number
1694      P:000475 P:000475 200003            TST     A
1695      P:000476 P:000476 0E94BB            JLT     <ERR_SM1
1696      P:000477 P:000477 44F400            MOVE              #>24,X0                 ; Check for argument less than 32
                            000018
1697      P:000479 P:000479 200045            CMP     X0,A
1698      P:00047A P:00047A 0E14BB            JGE     <ERR_SM1
1699      P:00047B P:00047B 21CF00            MOVE              A,B
1700      P:00047C P:00047C 44F400            MOVE              #>7,X0
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 32



                            000007
1701      P:00047E P:00047E 20004E            AND     X0,B
1702      P:00047F P:00047F 44F400            MOVE              #>$18,X0
                            000018
1703      P:000481 P:000481 200046            AND     X0,A
1704      P:000482 P:000482 0E2485            JNE     <SMX_1                            ; Test for 0 <= MUX number <= 7
1705      P:000483 P:000483 0ACD63            BSET    #3,B1
1706      P:000484 P:000484 0C0490            JMP     <SMX_A
1707      P:000485 P:000485 44F400  SMX_1     MOVE              #>$08,X0
                            000008
1708      P:000487 P:000487 200045            CMP     X0,A                              ; Test for 8 <= MUX number <= 15
1709      P:000488 P:000488 0E248B            JNE     <SMX_2
1710      P:000489 P:000489 0ACD64            BSET    #4,B1
1711      P:00048A P:00048A 0C0490            JMP     <SMX_A
1712      P:00048B P:00048B 44F400  SMX_2     MOVE              #>$10,X0
                            000010
1713      P:00048D P:00048D 200045            CMP     X0,A                              ; Test for 16 <= MUX number <= 23
1714      P:00048E P:00048E 0E24BB            JNE     <ERR_SM1
1715      P:00048F P:00048F 0ACD65            BSET    #5,B1
1716      P:000490 P:000490 20006A  SMX_A     OR      X1,B1                             ; Add prefix to MUX numbers
1717      P:000491 P:000491 000000            NOP
1718      P:000492 P:000492 21A700            MOVE              B1,Y1
1719   
1720                                ; Add on the second MUX number
1721      P:000493 P:000493 56DB00            MOVE              X:(R3)+,A               ; Get the next MUX number
1722      P:000494 P:000494 200003            TST     A
1723      P:000495 P:000495 0E94BC            JLT     <ERR_SM2
1724      P:000496 P:000496 44F400            MOVE              #>24,X0                 ; Check for argument less than 32
                            000018
1725      P:000498 P:000498 200045            CMP     X0,A
1726      P:000499 P:000499 0E14BC            JGE     <ERR_SM2
1727      P:00049A P:00049A 0606A0            REP     #6
1728      P:00049B P:00049B 200033            LSL     A
1729      P:00049C P:00049C 000000            NOP
1730      P:00049D P:00049D 21CF00            MOVE              A,B
1731      P:00049E P:00049E 44F400            MOVE              #$1C0,X0
                            0001C0
1732      P:0004A0 P:0004A0 20004E            AND     X0,B
1733      P:0004A1 P:0004A1 44F400            MOVE              #>$600,X0
                            000600
1734      P:0004A3 P:0004A3 200046            AND     X0,A
1735      P:0004A4 P:0004A4 0E24A7            JNE     <SMX_3                            ; Test for 0 <= MUX number <= 7
1736      P:0004A5 P:0004A5 0ACD69            BSET    #9,B1
1737      P:0004A6 P:0004A6 0C04B2            JMP     <SMX_B
1738      P:0004A7 P:0004A7 44F400  SMX_3     MOVE              #>$200,X0
                            000200
1739      P:0004A9 P:0004A9 200045            CMP     X0,A                              ; Test for 8 <= MUX number <= 15
1740      P:0004AA P:0004AA 0E24AD            JNE     <SMX_4
1741      P:0004AB P:0004AB 0ACD6A            BSET    #10,B1
1742      P:0004AC P:0004AC 0C04B2            JMP     <SMX_B
1743      P:0004AD P:0004AD 44F400  SMX_4     MOVE              #>$400,X0
                            000400
1744      P:0004AF P:0004AF 200045            CMP     X0,A                              ; Test for 16 <= MUX number <= 23
1745      P:0004B0 P:0004B0 0E24BC            JNE     <ERR_SM2
1746      P:0004B1 P:0004B1 0ACD6B            BSET    #11,B1
1747      P:0004B2 P:0004B2 200078  SMX_B     ADD     Y1,B                              ; Add prefix to MUX numbers
1748      P:0004B3 P:0004B3 000000            NOP
1749      P:0004B4 P:0004B4 21AE00            MOVE              B1,A
1750      P:0004B5 P:0004B5 0140C6            AND     #$F01FFF,A                        ; Just to be sure
                            F01FFF
1751      P:0004B7 P:0004B7 0D020C            JSR     <XMIT_A_WORD                      ; Transmit A to TIM-A-STD
1752      P:0004B8 P:0004B8 0D03F6            JSR     <PAL_DLY                          ; Delay for all this to happen
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 33



1753      P:0004B9 P:0004B9 012F03            BCLR    #3,X:PCRD                         ; Turn the serial clock off
1754      P:0004BA P:0004BA 0C008F            JMP     <FINISH
1755      P:0004BB P:0004BB 56DB00  ERR_SM1   MOVE              X:(R3)+,A               ; Throw off the last argument
1756      P:0004BC P:0004BC 012F03  ERR_SM2   BCLR    #3,X:PCRD                         ; Turn the serial clock off
1757      P:0004BD P:0004BD 0C008D            JMP     <ERROR
1758   
1759                                ;***********  Special H2RG commands *******************
1760   
1761                                ; This should clear the array when the 'Clear Array' button in the
1762                                ;   main Voodoo window is pressed.
1763                                CLR_ARRAY
1764      P:0004BE P:0004BE 0D02BD            JSR     <GBL_RESET
1765      P:0004BF P:0004BF 0C008F            JMP     <FINISH
1766   
1767                                ; Transmit a serial register command to the H2RG array
1768      P:0004C0 P:0004C0 60F400  SER_COM   MOVE              #CSB_LOW,R0             ; Enable one serial command link
                            000073
1769      P:0004C2 P:0004C2 0D03EB            JSR     <CLOCK
1770      P:0004C3 P:0004C3 061080  NEXT      DO      #16,L_SERCOM                      ; The commands are 16 bits long
                            0004CF
1771      P:0004C5 P:0004C5 0ACC2F            JSET    #15,A1,B_SET                      ; Check if the bit is set or cleared
                            0004CB
1772      P:0004C7 P:0004C7 60F400            MOVE              #CLOCK_SERIAL_ZERO,R0   ; Transmit a zero bit
                            00007E
1773      P:0004C9 P:0004C9 0D03EB            JSR     <CLOCK
1774      P:0004CA P:0004CA 0C04CE            JMP     <NEXTBIT
1775      P:0004CB P:0004CB 60F400  B_SET     MOVE              #CLOCK_SERIAL_ONE,R0    ; Transmit a one bit
                            00007B
1776      P:0004CD P:0004CD 0D03EB            JSR     <CLOCK
1777      P:0004CE P:0004CE 200033  NEXTBIT   LSL     A                                 ; Get the next most significant bit
1778      P:0004CF P:0004CF 000000            NOP
1779                                L_SERCOM
1780      P:0004D0 P:0004D0 60F400            MOVE              #CSB_HIGH,R0            ; Disable the serial command link
                            000077
1781      P:0004D2 P:0004D2 0D03EB            JSR     <CLOCK
1782      P:0004D3 P:0004D3 00000C            RTS
1783   
1784                                SERIAL_COMMAND
1785      P:0004D4 P:0004D4 56DB00            MOVE              X:(R3)+,A               ; Get the command
1786      P:0004D5 P:0004D5 0D04C0            JSR     <SER_COM                          ; Send it to the serial command register
1787      P:0004D6 P:0004D6 0C008F            JMP     <FINISH
1788   
1789                                ; Assert MAINRESETB to clear all internal registers to default settings
1790                                RESET_INTERNAL_REGISTERS
1791      P:0004D7 P:0004D7 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
1792      P:0004D8 P:0004D8 60F400            MOVE              #RST_INTERNAL_REGISTERS,R0
                            000081
1793      P:0004DA P:0004DA 0D03EB            JSR     <CLOCK
1794      P:0004DB P:0004DB 0C008F            JMP     <FINISH
1795   
1796                                ; Initialize the internal registers to default settings
1797                                INIT_H2RG
1798      P:0004DC P:0004DC 0A002F            BSET    #ST_DIRTY,X:<STATUS
1799      P:0004DD P:0004DD 56F400            MOVE              #(OUTPUT_MODE_REGISTER+$4),A ; 32-output mode, use pin #7
                            003004
1800      P:0004DF P:0004DF 0D04C0            JSR     <SER_COM                          ;  for window output
1801      P:0004E0 P:0004E0 56F400            MOVE              #(OUTPUT_BUF_REG+$11),A ; Slow bufferred readout mode
                            004011
1802      P:0004E2 P:0004E2 0D04C0            JSR     <SER_COM                          ;  Unused outputs in high ohmic states
1803      P:0004E3 P:0004E3 56F400            MOVE              #(NORMAL_MODE_REG+$83),A ; Enhanced clocking, global reset
                            005083
1804      P:0004E5 P:0004E5 0D04C0            JSR     <SER_COM
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 34



1805      P:0004E6 P:0004E6 56F400            MOVE              #(WINDOW_MODE_REG+$83),A ; Windowing mode is enhanced
                            006083
1806      P:0004E8 P:0004E8 0D04C0            JSR     <SER_COM                          ;  clocking, global reset
1807      P:0004E9 P:0004E9 00000C            RTS
1808   
1809                                INITIALIZE_H2RG
1810      P:0004EA P:0004EA 0D04DC            JSR     <INIT_H2RG
1811      P:0004EB P:0004EB 0C008F            JMP     <FINISH
1812   
1813                                ; Specify subarray readout size
1814                                SET_SUBARRAY_SIZE
1815      P:0004EC P:0004EC 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
1816      P:0004ED P:0004ED 0A0010            BCLR    #ST_SA,X:<STATUS
1817      P:0004EE P:0004EE 44DB00            MOVE              X:(R3)+,X0              ; Not used
1818      P:0004EF P:0004EF 56DB00            MOVE              X:(R3)+,A               ; Whole array mode if ncols = 0
1819      P:0004F0 P:0004F0 200003            TST     A
1820      P:0004F1 P:0004F1 0EA08F            JEQ     <FINISH
1821      P:0004F2 P:0004F2 0A0030            BSET    #ST_SA,X:<STATUS
1822      P:0004F3 P:0004F3 5C0F00            MOVE                          A1,Y:<SA_NCOLS ; Number of columns in subimage read
1823      P:0004F4 P:0004F4 44DB00            MOVE              X:(R3)+,X0
1824      P:0004F5 P:0004F5 4C1000            MOVE                          X0,Y:<SA_NROWS ; Number of rows in subimage read
1825      P:0004F6 P:0004F6 0C008F            JMP     <FINISH
1826   
1827                                ; Specify subarray readout position
1828                                SET_SUBARRAY_POSITION
1829      P:0004F7 P:0004F7 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
1830      P:0004F8 P:0004F8 44DB00            MOVE              X:(R3)+,X0
1831      P:0004F9 P:0004F9 4C1300            MOVE                          X0,Y:<SA_STARTROW ; Number of rows skip over
1832      P:0004FA P:0004FA 44DB00            MOVE              X:(R3)+,X0
1833      P:0004FB P:0004FB 4C1100            MOVE                          X0,Y:<SA_BEGCOL ; Number of columns to skip over
1834      P:0004FC P:0004FC 44DB00            MOVE              X:(R3)+,X0              ; Not used
1835      P:0004FD P:0004FD 0C008F            JMP     <FINISH
1836   
1837                                ; Specify the delay time between clocking a row and beginning to read
1838                                SET_READ_DELAY
1839      P:0004FE P:0004FE 44DB00            MOVE              X:(R3)+,X0
1840      P:0004FF P:0004FF 4C0600            MOVE                          X0,Y:<READ_DELAY
1841      P:000500 P:000500 0C008F            JMP     <FINISH
1842   
1843                                SETUP_READ_MODE_FINISH
1844      P:000501 P:000501 0D0503            JSR     <SETUP_READ_MODE
1845      P:000502 P:000502 0C008F            JMP     <FINISH
1846   
1847                                ; Set up for subarray, windowing and number of readout channels
1848                                SETUP_READ_MODE
1849                                ;       BCLR    #ST_DIRTY,X:<STATUS
1850      P:000503 P:000503 0A0090            JCLR    #ST_SA,X:STATUS,WHOLE_FRAME
                            000506
1851                                ;       JCLR    #ST_WM,X:STATUS,NOT_WM
1852                                ;
1853                                ; Set up for native H2RG windowing readout mode through one channel
1854                                ;       MOVE    Y:<SA_NCOLS,X0
1855                                ;       MOVE    X0,Y:<NCOLS_CLOCK
1856                                ;       MOVE    Y:<SA_NROWS,A
1857                                ;       JCLR    #ST_CDS,X:STATUS,NO_CDS4
1858                                ;       LSR     A
1859                                ;       NOP
1860                                ;NO_CDS4        MOVE    A1,Y:<NROWS_CLOCK
1861                                ;       MOVE    #XMIT_1,X0              ; Only one channel transmits in
1862                                ;       MOVE    X0,Y:XMT_PXL            ;   windowing mode
1863                                ;       MOVE    #>1,X0
1864                                ;       MOVE    X0,Y:<NCHS
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 35



1865                                ;
1866                                ;       MOVE    Y:<SA_BEGCOL,A
1867                                ;       MOVE    #HORIZ_START_REG,X0     ; Address of HorizStartReg
1868                                ;       ADD     X0,A
1869                                ;       JSR     <SER_COM
1870                                ;
1871                                ;       MOVE    Y:<SA_NCOLS,X0
1872                                ;       MOVE    Y:<SA_BEGCOL,A
1873                                ;       ADD     X0,A
1874                                ;       MOVE    #HORIZ_STOP_REG,X0      ; Address of HorizStopReg
1875                                ;       ADD     X0,A
1876                                ;       JSR     <SER_COM
1877                                ;
1878                                ;       MOVE    Y:<SA_STARTROW,A
1879                                ;       MOVE    #VERT_START_REG,X0      ; Address of VertStartReg
1880                                ;       ADD     X0,A
1881                                ;       JSR     <SER_COM
1882                                ;
1883                                ;       MOVE    Y:<SA_NROWS,X0
1884                                ;       MOVE    Y:<SA_STARTROW,A
1885                                ;       ADD     X0,A
1886                                ;       MOVE    #VERT_STOP_REG,X0       ; Address of VertStopReg
1887                                ;       ADD     X0,A
1888                                ;       JSR     <SER_COM
1889                                ;
1890                                ;       MOVE    #(MISC_REG+$F),A        ; Enable windowing mode
1891                                ;       JSR     <SER_COM
1892                                ;
1893                                ;       MOVE    #ENABLE_WM,R0           ; Enable windowing mode with signal lines, too
1894                                ;       JSR     <CLOCK
1895                                ;
1896                                ;       MOVE    #CLK_COL,R1
1897                                ;       MOVE    #(END_CLK_COL-CLK_COL-1),X0
1898                                ;       MOVE    X0,Y:(R1)
1899      P:000505 P:000505 00000C            RTS
1900   
1901                                WHOLE_FRAME
1902      P:000506 P:000506 5E8200            MOVE                          Y:<NROWS,A
1903                                ;       JCLR    #ST_CDS,X:STATUS,NO_CDS2
1904                                ;       LSR     A
1905      P:000507 P:000507 000000            NOP
1906      P:000508 P:000508 5C0400  NO_CDS2   MOVE                          A1,Y:<NROWS_CLOCK
1907      P:000509 P:000509 5E8100            MOVE                          Y:<NCOLS,A
1908                                ;       MOVE    Y:<N_SHFTS,X0
1909                                ;       LSR     X0,A                    ; n-port readout in whole image mode
1910      P:00050A P:00050A 0C1ECA            LSR     #5,A                              ; n-port readout in whole image mode
1911      P:00050B P:00050B 000000            NOP
1912      P:00050C P:00050C 5C0300            MOVE                          A1,Y:<NCOLS_CLOCK
1913      P:00050D P:00050D 000000            NOP
1914      P:00050E P:00050E 4C8700  END_SWM   MOVE                          Y:<SXMIT,X0 ; Transmit n channels
1915                                ;       MOVE    X0,Y:XMT_PXL
1916                                ;       MOVE    Y:<NUM_CHANS,X0
1917                                ;       MOVE    X0,Y:<NCHS
1918                                ;       MOVE    #(MISC_REG+$C),A        ; Disable windowing mode
1919                                ;       JSR     <SER_COM
1920                                ;       MOVE    #DISABLE_WM,R0          ; Disable windowing mode with signal lines, too
1921                                ;       JSR     <CLOCK
1922      P:00050F P:00050F 00000C            RTS
1923   
1924                                ; Sub array readout, but not native H2RG windowing mode
1925      P:000510 P:000510 5E8F00  NOT_WM    MOVE                          Y:<SA_NCOLS,A
1926      P:000511 P:000511 4C8A00            MOVE                          Y:<N_SHFTS,X0
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 36



1927      P:000512 P:000512 0C1E38            LSR     X0,A                              ; n-port readout in whole image mode
1928      P:000513 P:000513 000000            NOP
1929      P:000514 P:000514 5C0300            MOVE                          A1,Y:<NCOLS_CLOCK
1930      P:000515 P:000515 5E9100            MOVE                          Y:<SA_BEGCOL,A ; One clock advances image by n coluMns
1931      P:000516 P:000516 0C1E38            LSR     X0,A                              ; n-port readout in whole image mode
1932      P:000517 P:000517 000000            NOP
1933      P:000518 P:000518 5C1200            MOVE                          A1,Y:<SA_NCOLS_CK
1934      P:000519 P:000519 5E9000            MOVE                          Y:<SA_NROWS,A
1935      P:00051A P:00051A 0A0091            JCLR    #ST_CDS,X:STATUS,NO_CDS3
                            00051E
1936      P:00051C P:00051C 200023            LSR     A
1937      P:00051D P:00051D 000000            NOP
1938      P:00051E P:00051E 5C0400  NO_CDS3   MOVE                          A1,Y:<NROWS_CLOCK
1939      P:00051F P:00051F 0C050E            JMP     <END_SWM
1940   
1941                                ; Specify number of readout channels. Must be 1, 4, 8, or 32.
1942                                ;   NUM_CHANS and SXMIT are set here and used elsewhere to set NCHS and XMT_PXL
1943                                ;   if not in windowing mode.
1944                                SPECIFY_NUMBER_OF_CHANNELS
1945      P:000520 P:000520 000000            NOP
1946      P:000521 P:000521 0C008F            JMP     <FINISH
1947   
1948                                ;SPECIFY_NUMBER_OF_CHANNELS
1949                                KK
1950      P:000522 P:000522 0A002F            BSET    #ST_DIRTY,X:<STATUS
1951      P:000523 P:000523 56DB00            MOVE              X:(R3)+,A
1952      P:000524 P:000524 000000            NOP
1953      P:000525 P:000525 5C0900            MOVE                          A1,Y:<NUM_CHANS ; Number of channels in non-windowing mo
de
1954      P:000526 P:000526 014185            CMP     #1,A
1955      P:000527 P:000527 0E253B            JNE     <CMP_4
1956      P:000528 P:000528 240000            MOVE              #0,X0
1957      P:000529 P:000529 4C0A00            MOVE                          X0,Y:<N_SHFTS
1958      P:00052A P:00052A 44F400            MOVE              #$00F000,X0
                            00F000
1959      P:00052C P:00052C 4C0700            MOVE                          X0,Y:<SXMIT
1960      P:00052D P:00052D 61F400            MOVE              #CLK_COL,R1
                            000048
1961      P:00052F P:00052F 0A0033            BSET    #ST_RDM,X:<STATUS                 ; Set video readout mode to 1-by-1
1962      P:000530 P:000530 0A0F23            BSET    #RD_MODE,X:<LATCH
1963      P:000531 P:000531 44F400            MOVE              #(END_CLK_COL-CLK_COL-1),X0
                            00000C
1964      P:000533 P:000533 4C6100            MOVE                          X0,Y:(R1)
1965      P:000534 P:000534 09F0B5            MOVEP             X:LATCH,Y:WRLATCH
                            00000F
1966      P:000536 P:000536 56F400            MOVE              #(OUTPUT_MODE_REGISTER+$1),A ; 1 channel readout mode
                            003001
1967      P:000538 P:000538 0D04C0            JSR     <SER_COM
1968      P:000539 P:000539 0D0503            JSR     <SETUP_READ_MODE
1969      P:00053A P:00053A 0C008F            JMP     <FINISH
1970      P:00053B P:00053B 014485  CMP_4     CMP     #4,A
1971      P:00053C P:00053C 0E2557            JNE     <CMP_8
1972      P:00053D P:00053D 44F400            MOVE              #>2,X0
                            000002
1973      P:00053F P:00053F 4C0A00            MOVE                          X0,Y:<N_SHFTS
1974      P:000540 P:000540 44F400            MOVE              #XMIT7,X0
                            00F1C7
1975      P:000542 P:000542 4C0700            MOVE                          X0,Y:<SXMIT
1976      P:000543 P:000543 61F400            MOVE              #CLK_COL,R1
                            000048
1977      P:000545 P:000545 0A0033            BSET    #ST_RDM,X:<STATUS                 ; Set video readout mode to 1-by-1
1978      P:000546 P:000546 0A0F23            BSET    #RD_MODE,X:<LATCH
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 37



1979                                ;       MOVE    #(END_CLK_COL_4-CLK_COL-1),X0
1980      P:000547 P:000547 4C6100            MOVE                          X0,Y:(R1)
1981      P:000548 P:000548 09F0B5            MOVEP             X:LATCH,Y:WRLATCH
                            00000F
1982      P:00054A P:00054A 56F400            MOVE              #(OUTPUT_MODE_REGISTER+$2),A ; 4 channel readout mode
                            003002
1983      P:00054C P:00054C 0D04C0            JSR     <SER_COM
1984      P:00054D P:00054D 0D0503            JSR     <SETUP_READ_MODE
1985   
1986                                ; Set up the fiber optic serial transmitter so it transmits from channels #7, 15, 23 and 31 only
1987      P:00054E P:00054E 60F400            MOVE              #XMT_PXL,R0
                            000055
1988      P:000550 P:000550 45F400            MOVE              #>$7,X1                 ; Don't disturb the video processor
                            000007
1989      P:000552 P:000552 44F400            MOVE              #XMIT7,X0
                            00F1C7
1990      P:000554 P:000554 4C5800            MOVE                          X0,Y:(R0)+  ; Readout #7
1991      P:000555 P:000555 4D5800            MOVE                          X1,Y:(R0)+
1992      P:000556 P:000556 0C008F            JMP     <FINISH
1993      P:000557 P:000557 014885  CMP_8     CMP     #8,A                              ; Useful for testing 8-channel video boards
1994      P:000558 P:000558 0E256D            JNE     <CMP_32
1995      P:000559 P:000559 44F400            MOVE              #>3,X0
                            000003
1996      P:00055B P:00055B 4C0A00            MOVE                          X0,Y:<N_SHFTS
1997      P:00055C P:00055C 44F400            MOVE              #$00F1C0,X0
                            00F1C0
1998      P:00055E P:00055E 4C0700            MOVE                          X0,Y:<SXMIT
1999      P:00055F P:00055F 61F400            MOVE              #CLK_COL,R1
                            000048
2000      P:000561 P:000561 0A0013            BCLR    #ST_RDM,X:<STATUS                 ; Set video readout mode to all eight at onc
e
2001      P:000562 P:000562 0A0F03            BCLR    #RD_MODE,X:<LATCH
2002      P:000563 P:000563 44F400            MOVE              #(END_CLK_COL-CLK_COL-1),X0
                            00000C
2003      P:000565 P:000565 4C6100            MOVE                          X0,Y:(R1)
2004      P:000566 P:000566 09F0B5            MOVEP             X:LATCH,Y:WRLATCH
                            00000F
2005      P:000568 P:000568 56F400            MOVE              #(OUTPUT_MODE_REGISTER+$2),A ; 8 channel readout mode, watch out -
                            003002
2006      P:00056A P:00056A 0D04C0            JSR     <SER_COM                          ;  this is not supported by H2RG
2007      P:00056B P:00056B 0D0503            JSR     <SETUP_READ_MODE
2008      P:00056C P:00056C 0C008F            JMP     <FINISH
2009      P:00056D P:00056D 016085  CMP_32    CMP     #32,A
2010      P:00056E P:00056E 0E208D            JNE     <ERROR
2011      P:00056F P:00056F 44F400            MOVE              #>5,X0
                            000005
2012      P:000571 P:000571 4C0A00            MOVE                          X0,Y:<N_SHFTS
2013      P:000572 P:000572 44F400            MOVE              #$00F7C0,X0
                            00F7C0
2014      P:000574 P:000574 4C0700            MOVE                          X0,Y:<SXMIT
2015      P:000575 P:000575 61F400            MOVE              #CLK_COL,R1
                            000048
2016      P:000577 P:000577 0A0013            BCLR    #ST_RDM,X:<STATUS                 ; Clear video readout mode to all eight at o
nce
2017      P:000578 P:000578 0A0F03            BCLR    #RD_MODE,X:<LATCH
2018      P:000579 P:000579 44F400            MOVE              #(END_CLK_COL-CLK_COL-1),X0
                            00000C
2019      P:00057B P:00057B 4C6100            MOVE                          X0,Y:(R1)
2020      P:00057C P:00057C 09F0B5            MOVEP             X:LATCH,Y:WRLATCH
                            00000F
2021      P:00057E P:00057E 56F400            MOVE              #(OUTPUT_MODE_REGISTER+$4),A ; 32 channel readout mode
                            003004
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 38



2022      P:000580 P:000580 0D04C0            JSR     <SER_COM
2023      P:000581 P:000581 0D0503            JSR     <SETUP_READ_MODE
2024      P:000582 P:000582 0C008F            JMP     <FINISH
2025   
2026                                ; Continuous readout commands
2027                                SET_NUMBER_OF_FRAMES                                ; Number of frames to obtain
2028      P:000583 P:000583 44DB00            MOVE              X:(R3)+,X0              ;   in an exposure sequence
2029      P:000584 P:000584 4C0B00            MOVE                          X0,Y:<N_FRAMES
2030      P:000585 P:000585 0C008F            JMP     <FINISH
2031   
2032                                SET_NUMBER_OF_FRAMES_PER_BUFFER                     ; Number of frames in each image
2033      P:000586 P:000586 44DB00            MOVE              X:(R3)+,X0              ;   buffer in the host computer
2034      P:000587 P:000587 4C0E00            MOVE                          X0,Y:<N_FPB ;   system memory
2035      P:000588 P:000588 0C008F            JMP     <FINISH
2036   
2037                                SET_RESET_MODE
2038      P:000589 P:000589 44DB00            MOVE              X:(R3)+,X0              ; Get the command argument
2039      P:00058A P:00058A 0AC400            JCLR    #0,X0,NO_RESET                    ; 1 to reset FPA in continuous
                            000591
2040      P:00058C P:00058C 0A0034            BSET    #ST_RST_MODE,X:<STATUS            ;   readout mode
2041      P:00058D P:00058D 44DB00            MOVE              X:(R3)+,X0
2042      P:00058E P:00058E 000000            NOP
2043      P:00058F P:00058F 4C1500            MOVE                          X0,Y:<RSTMODE
2044      P:000590 P:000590 0C008F            JMP     <FINISH
2045                                NO_RESET
2046      P:000591 P:000591 0A0014            BCLR    #ST_RST_MODE,X:<STATUS
2047      P:000592 P:000592 0C008F            JMP     <FINISH
2048   
2049                                SELECT_WINDOWING_MODE
2050      P:000593 P:000593 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
2051      P:000594 P:000594 0A5B80            JCLR    #0,X:(R3)+,NO_WM                  ; 1 to read out in native H2RG
                            000599
2052      P:000596 P:000596 0A0035            BSET    #ST_WM,X:<STATUS
2053      P:000597 P:000597 0A0030            BSET    #ST_SA,X:<STATUS
2054      P:000598 P:000598 0C008F            JMP     <FINISH
2055      P:000599 P:000599 0A0015  NO_WM     BCLR    #ST_WM,X:<STATUS
2056      P:00059A P:00059A 0A0010            BCLR    #ST_SA,X:<STATUS
2057      P:00059B P:00059B 0C008F            JMP     <FINISH
2058   
2059                                READ_NUMBER_OF_CHANNELS
2060      P:00059C P:00059C 0D0503            JSR     <SETUP_READ_MODE                  ; Keep NCHS updated
2061      P:00059D P:00059D 4F8800            MOVE                          Y:<NCHS,Y1
2062      P:00059E P:00059E 0C0090            JMP     <FINISH1
2063   
2064                                ; Select which video channel number, from 0 to 63, but only one
2065                                SELECT_VIDEO_CHANNEL_NUMBER
2066      P:00059F P:00059F 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
2067      P:0005A0 P:0005A0 56DB00            MOVE              X:(R3)+,A
2068      P:0005A1 P:0005A1 000000            NOP
2069      P:0005A2 P:0005A2 218400            MOVE              A1,X0
2070      P:0005A3 P:0005A3 0C1E8C            LSL     #6,A
2071      P:0005A4 P:0005A4 000000            NOP
2072      P:0005A5 P:0005A5 200042            OR      X0,A
2073      P:0005A6 P:0005A6 44F400            MOVE              #$00F000,X0
                            00F000
2074      P:0005A8 P:0005A8 200042            OR      X0,A
2075      P:0005A9 P:0005A9 000000            NOP
2076      P:0005AA P:0005AA 000000            NOP
2077      P:0005AB P:0005AB 5C0700            MOVE                          A1,Y:<SXMIT
2078      P:0005AC P:0005AC 0C008F            JMP     <FINISH
2079   
2080                                CORRELATED_DOUBLE_SAMPLE
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 39



2081      P:0005AD P:0005AD 0A002F            BSET    #ST_DIRTY,X:<STATUS               ; A readout parameter will be changed
2082      P:0005AE P:0005AE 56DB00            MOVE              X:(R3)+,A
2083      P:0005AF P:0005AF 0A002F            BSET    #ST_DIRTY,X:<STATUS
2084      P:0005B0 P:0005B0 0ACC00            JCLR    #0,A1,NOT_CDS
                            0005B4
2085      P:0005B2 P:0005B2 0A0031            BSET    #ST_CDS,X:STATUS
2086      P:0005B3 P:0005B3 0C008F            JMP     <FINISH
2087      P:0005B4 P:0005B4 0A0011  NOT_CDS   BCLR    #ST_CDS,X:<STATUS
2088      P:0005B5 P:0005B5 0C008F            JMP     <FINISH
2089   
2090                                SELECT_DUAL_TRANSMITTER
2091      P:0005B6 P:0005B6 0A5B80            JCLR    #0,X:(R3)+,SINGLE_XMTR
                            0005BA
2092      P:0005B8 P:0005B8 0A0037            BSET    #ST_XMT2,X:<STATUS
2093      P:0005B9 P:0005B9 0C008F            JMP     <FINISH
2094                                SINGLE_XMTR
2095      P:0005BA P:0005BA 0A0017            BCLR    #ST_XMT2,X:<STATUS
2096      P:0005BB P:0005BB 0C008F            JMP     <FINISH
2097   
2098                                ; Select readout mode as either
2099                                ;   = 0 -> 8-channels move to the FIFO whenever XFER has a high going edge
2100                                ;   = 1 -> 1-channel at a time moves to the FIFO on SXMIT
2101   
2102                                VIDEO_READOUT_MODE
2103      P:0005BC P:0005BC 0A5B80            JCLR    #0,X:(R3)+,EIGHT_CHANNELS
                            0005C3
2104      P:0005BE P:0005BE 0A0033            BSET    #ST_RDM,X:<STATUS
2105      P:0005BF P:0005BF 0A0F23            BSET    #RD_MODE,X:<LATCH
2106      P:0005C0 P:0005C0 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Write the bit to the IR video PAL
                            00000F
2107      P:0005C2 P:0005C2 0C008F            JMP     <FINISH
2108   
2109                                EIGHT_CHANNELS
2110      P:0005C3 P:0005C3 0A0013            BCLR    #ST_RDM,X:<STATUS
2111      P:0005C4 P:0005C4 0A0F03            BCLR    #RD_MODE,X:<LATCH
2112      P:0005C5 P:0005C5 09F0B5            MOVEP             X:LATCH,Y:WRLATCH       ; Write the bit to the IR video PAL
                            00000F
2113      P:0005C7 P:0005C7 0C008F            JMP     <FINISH
2114   
2115                                SET_NUMBER_OF_FOWLER_SAMPLES
2116      P:0005C8 P:0005C8 44DB00            MOVE              X:(R3)+,X0
2117      P:0005C9 P:0005C9 4C7000            MOVE                          X0,Y:NFS
                            000016
2118      P:0005CB P:0005CB 208E00            MOVE              X0,A
2119      P:0005CC P:0005CC 000000            NOP
2120      P:0005CD P:0005CD 014184            SUB     #1,A
2121      P:0005CE P:0005CE 000000            NOP
2122      P:0005CF P:0005CF 5E7000            MOVE                          A,Y:NFS_1
                            000017
2123      P:0005D1 P:0005D1 0C008F            JMP     <FINISH
2124   
2125                                ; **********************************************************************************************
*************
2126                                SET_UP_THE_RAMP
2127      P:0005D2 P:0005D2 44DB00            MOVE              X:(R3)+,X0
2128      P:0005D3 P:0005D3 4C7000            MOVE                          X0,Y:NUTR
                            000014
2129      P:0005D5 P:0005D5 0C008F            JMP     <FINISH
2130   
2131                                ; **********************************************************************************************
*************
2132   
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  timIRmisc.asm  Page 40



2133   
2134   
2135                                SET_AMPLIFIERS
2136      P:0005D6 P:0005D6 0C008F            JMP     <FINISH
2137   
2138                                ; **********************************************************************************************
*************
2139                                READ_FRAME_TIME
2140      P:0005D7 P:0005D7 0C008F            JMP     <FINISH
2141   
2142                                ; **********************************************************************************************
*************
2143                                SELECT_RESET_MODE
2144      P:0005D8 P:0005D8 0C008F            JMP     <FINISH
2145   
2146   
2147                                 TIMBOOT_X_MEMORY
2148      0005D9                              EQU     @LCV(L)
2149   
2150                                ;  ****************  Setup memory tables in X: space ********************
2151   
2152                                ; Define the address in P: space where the table of constants begins
2153   
2154                                          IF      @SCP("HOST","HOST")
2155      X:000036 X:000036                   ORG     X:END_COMMAND_TABLE,X:END_COMMAND_TABLE
2156                                          ENDIF
2157   
2158                                          IF      @SCP("HOST","ROM")
2160                                          ENDIF
2161   
2162      X:000036 X:000036                   DC      'SEX',START_EXPOSURE              ; Voodoo and CCDTool start exposure
2163      X:000038 X:000038                   DC      'PON',POWER_ON                    ; Turn on all camera biases and clocks
2164      X:00003A X:00003A                   DC      'POF',POWER_OFF                   ; Turn +/- 15V power supplies off
2165      X:00003C X:00003C                   DC      'SBN',SET_BIAS_NUMBER
2166      X:00003E X:00003E                   DC      'SMX',SET_MUX                     ; Set MUX number on clock driver board
2167      X:000040 X:000040                   DC      'DON',START
2168      X:000042 X:000042                   DC      'SET',SET_EXPOSURE_TIME
2169      X:000044 X:000044                   DC      'RET',READ_EXPOSURE_TIME
2170      X:000046 X:000046                   DC      'PEX',PAUSE_EXPOSURE
2171      X:000048 X:000048                   DC      'REX',RESUME_EXPOSURE
2172      X:00004A X:00004A                   DC      'AEX',ABORT_EXPOSURE
2173      X:00004C X:00004C                   DC      'ABR',ABORT_EXPOSURE
2174      X:00004E X:00004E                   DC      'RCC',READ_CONTROLLER_CONFIGURATION
2175      X:000050 X:000050                   DC      'STP',STP                         ; Exit continuous reset mode
2176      X:000052 X:000052                   DC      'IDL',IDLE                        ; Enable continuous reset mode
2177   
2178                                ; Continuous readout commands
2179      X:000054 X:000054                   DC      'SNF',SET_NUMBER_OF_FRAMES
2180      X:000056 X:000056                   DC      'FPB',SET_NUMBER_OF_FRAMES_PER_BUFFER
2181   
2182                                ; Test the second fiber optic transmitter
2183      X:000058 X:000058                   DC      'XMT',SELECT_DUAL_TRANSMITTER
2184   
2185                                ; More commands
2186      X:00005A X:00005A                   DC      'SSS',SET_SUBARRAY_SIZE
2187      X:00005C X:00005C                   DC      'SSP',SET_SUBARRAY_POSITION
2188      X:00005E X:00005E                   DC      'SWM',SELECT_WINDOWING_MODE
2189      X:000060 X:000060                   DC      'SNC',SPECIFY_NUMBER_OF_CHANNELS
2190      X:000062 X:000062                   DC      'SER',SERIAL_COMMAND
2191      X:000064 X:000064                   DC      'RIR',RESET_INTERNAL_REGISTERS
2192      X:000066 X:000066                   DC      'CLR',CLR_ARRAY
2193      X:000068 X:000068                   DC      'INI',INITIALIZE_H2RG
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 41



2194      X:00006A X:00006A                   DC      'SRM',SET_RESET_MODE
2195      X:00006C X:00006C                   DC      'VRM',VIDEO_READOUT_MODE          ; One-by-one or all eight channels at once
2196      X:00006E X:00006E                   DC      'SRD',SET_READ_DELAY
2197      X:000070 X:000070                   DC      'SVO',SET_VIDEO_OFFSET
2198      X:000072 X:000072                   DC      'RNC',READ_NUMBER_OF_CHANNELS
2199      X:000074 X:000074                   DC      'SVC',SELECT_VIDEO_CHANNEL_NUMBER
2200      X:000076 X:000076                   DC      'CDS',CORRELATED_DOUBLE_SAMPLE
2201                                ; ArcVIEW compatibility
2202      X:000078 X:000078                   DC      'SAM',SET_AMPLIFIERS
2203      X:00007A X:00007A                   DC      'RFT',READ_FRAME_TIME
2204      X:00007C X:00007C                   DC      'SNR',SET_NUMBER_OF_FOWLER_SAMPLES
2205      X:00007E X:00007E                   DC      'SRE',SET_RESET_MODE
2206      X:000080 X:000080                   DC      'SNC',SET_NUMBER_OF_FRAMES
2207      X:000082 X:000082                   DC      'SUR',SET_UP_THE_RAMP
2208   
2209                                 END_APPLICATON_COMMAND_TABLE
2210      000084                              EQU     @LCV(L)
2211   
2212                                          IF      @SCP("HOST","HOST")
2213      00002E                    NUM_COM   EQU     (@LCV(R)-COM_TBL_R)/2             ; Number of boot +
2214                                                                                    ;  application commands
2215      000333                    EXPOSING  EQU     CHK_TIM                           ; Address if exposing
2216                                 CONTINUE_READING
2217      100000                              EQU     CONT_RD                           ; Address if reading out
2218                                          ENDIF
2219   
2220                                          IF      @SCP("HOST","ROM")
2222                                          ENDIF
2223   
2224                                ; Now let's go for the timing waveform tables
2225                                          IF      @SCP("HOST","HOST")
2226      Y:000000 Y:000000                   ORG     Y:0,Y:0
2227                                          ENDIF
2228   
2229      Y:000000 Y:000000                   DC      END_APPLICATON_Y_MEMORY-@LCV(L)-1
2230   
2231      Y:000001 Y:000001         NCOLS     DC      0                                 ; Image dimensions set by host computer
2232      Y:000002 Y:000002         NROWS     DC      0
2233                                 NCOLS_CLOCK
2234      Y:000003 Y:000003                   DC      0                                 ; Number of columns clocked each frame
2235                                 NROWS_CLOCK
2236      Y:000004 Y:000004                   DC      0                                 ; Number of rows clocked each frame
2237      Y:000005 Y:000005         CONFIG    DC      CC                                ; Controller configuration
2238      Y:000006 Y:000006         READ_DELAY DC     0                                 ; Read delay in microsec
2239      Y:000007 Y:000007         SXMIT     DC      $00F7C0                           ; Magic number for series transmit of image 
data
2240      Y:000008 Y:000008         NCHS      DC      32                                ; Number of channels of readout
2241      Y:000009 Y:000009         NUM_CHANS DC      32                                ; Number of channels in not windowing mode
2242      Y:00000A Y:00000A         N_SHFTS   DC      5                                 ; Number of readout channels as a power of 2
2243   
2244                                ; Continuous readout parameters
2245      Y:00000B Y:00000B         N_FRAMES  DC      1                                 ; Total number of frames to read out
2246      Y:00000C Y:00000C         I_FRAME   DC      0                                 ; Number of frames read out so far
2247      Y:00000D Y:00000D         IBUFFER   DC      0                                 ; Number of frames read into the PCI buffer
2248      Y:00000E Y:00000E         N_FPB     DC      0                                 ; Number of frames per PCI image buffer
2249   
2250                                ; Subarray readout parameters
2251      Y:00000F Y:00000F         SA_NCOLS  DC      0                                 ; Number of columns in the image
2252      Y:000010 Y:000010         SA_NROWS  DC      0                                 ; Number of rows in the image
2253      Y:000011 Y:000011         SA_BEGCOL DC      0                                 ; Beginning column number to read
2254                                 SA_NCOLS_CK
2255      Y:000012 Y:000012                   DC      0                                 ; Number of clocks needed to skip over colum
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  tim.asm  Page 42



ns
2256                                 SA_STARTROW
2257      Y:000013 Y:000013                   DC      0                                 ; Beginning row number to read
2258   
2259                                ; ArcVIEW parameters
2260      Y:000014 Y:000014         NUTR      DC      1                                 ; [0x--] number of up-the-ramp to perform
2261      Y:000015 Y:000015         RSTMODE   DC      1                                 ; [0x--] number of coadds per exposure
2262      Y:000016 Y:000016         NFS       DC      1                                 ; [0x--] number of fowler samples
2263      Y:000017 Y:000017         NFS_1     DC      0                                 ; [0x--] number of fowler samples - 1
2264   
2265                                ; Include the waveform table for the designated IR array
2266                                          INCLUDE "H2RG.waveforms"                  ; Readout and clocking waveform file
2267   
2268                                ; The hardware consists of the ARC-22 timing board, ARC-32 clock driver board and eight
2269                                ;   8-channel ARC-46 Rev. 3B IR video processing board. This waveforms file is written
2270                                ;   for 32 channel readout of an H2RG array.
2271   
2272                                ; Miscellaneous definitions
2273      000000                    VID0      EQU     $000000                           ; Video board select = 0
2274      002000                    CLK2      EQU     $002000                           ; Select bottom of the first clock driver bo
ard
2275      003000                    CLK3      EQU     $003000                           ; Select top of the first clock driver board
2276                                 RESET_FIFOS
2277      0C1000                              EQU     $0C1000                           ; Reset image data FIFOs before each readout
2278      0C3000                    HI_GAIN   EQU     $0C3000                           ; High video gain = x4
2279      0C3001                    LO_GAIN   EQU     $0C3001                           ; Low video gain = x1
2280      0C3000                    GAIN      EQU     HI_GAIN
2281   
2282      000000                    DLY0      EQU     $000000                           ; no delay
2283      240000                    DLY1      EQU     $240000                           ; ~ 1.5 microsec
2284      320000                    DLY2      EQU     $320000                           ; ~ 2 microsec
2285      020000                    DLYS      EQU     $020000
2286      060000                    DLYR      EQU     $060000
2287      00F1C7                    XMIT_1    EQU     $00F1C7                           ; Transmit only video channel #7 in windowin
g mode
2288      00F7C0                    XMIT_32   EQU     $00F7C0                           ; Transmit video channels #0 to #31
2289   
2290                                ;XMIT7  EQU     $00F041 ; Transmit only channel #7      ; Test for one ARC-46 only
2291                                ;XMIT15 EQU     $00F0C3 ; Transmit only channel #15
2292                                ;XMIT23 EQU     $00F145 ; Transmit only channel #23
2293                                ;XMIT31 EQU     $00F1C7 ; Transmit only channel #31
2294   
2295      00F1C7                    XMIT7     EQU     $00F1C7                           ; Transmit only channel #7
2296      00F3CF                    XMIT15    EQU     $00F3CF                           ; Transmit only channel #15
2297      00F5D7                    XMIT23    EQU     $00F5D7                           ; Transmit only channel #23
2298      00F7DF                    XMIT31    EQU     $00F7DF                           ; Transmit only channel #31
2299   
2300                                ; H2RG internal register names and addresses
2301                                 OUTPUT_MODE_REGISTER
2302      003000                              EQU     $3000
2303                                 OUTPUT_BUF_REG
2304      004000                              EQU     $4000
2305                                 NORMAL_MODE_REG
2306      005000                              EQU     $5000
2307                                 WINDOW_MODE_REG
2308      006000                              EQU     $6000
2309      007000                    MISC_REG  EQU     $7000
2310                                 VERT_START_REG
2311      008000                              EQU     $8000
2312                                 VERT_STOP_REG
2313      009000                              EQU     $9000
2314                                 HORIZ_START_REG
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 43



2315      00A000                              EQU     $A000
2316                                 HORIZ_STOP_REG
2317      00B000                              EQU     $B000
2318   
2319                                ; Voltage tables
2320      3.100000E+000             CLK_HI    EQU     3.10                              ; High clock voltage
2321      1.000000E-001             CLK_LO    EQU     0.10                              ; Low clock voltage
2322      3.310000E+000             CLKmax    EQU     3.31                              ; Maximum clock voltage
2323      3.310000E+000             VIDEOmax  EQU     3.31                              ; Maximum video board DC bias voltage
2324      0.000000E+000             ZERO      EQU     0.0                               ; Zero volts for power-on sequence
2325      1.000000E+000             VSOURCE   EQU     1.0                               ; Source load voltage on the ARC46 video boa
rd
2326   
2327                                ; Define the video processor outpout offset values
2328      000810                    OFFSET    EQU     $810                              ; Good value for a grounded input
2329                                ;OFFSET         EQU     $8A0            ; Good value for a grounded input
2330                                ;OFFSET         EQU     $970            ; Good value for H2RG
2331   
2332                                ; DC bias voltages going to the H2RG array
2333      4.000000E-001             DRAIN     EQU     0.40                              ; Drain node of the output source follower
2334      0.000000E+000             SUB       EQU     0.00                              ; Multiplexer substrate
2335      3.300000E+000             VDDA      EQU     3.30                              ; Power supply
2336      3.300000E+000             VBIASPOWER EQU    3.30                              ; Power for pixel source follower
2337      2.400000E+000             VBIASGATE EQU     2.40                              ; Bias for pixel source follower
2338      1.500000E-001             VRESET    EQU     0.15                              ; Detector reset voltage
2339      5.500000E-001             DSUB      EQU     0.55                              ; Detector substrate
2340      0.000000E+000             CELLDRAIN EQU     0.00                              ; Drain node of pixel source follower
2341      3.300000E+000             VDD       EQU     3.30                              ; Power supply
2342   
2343                                ; Clock board #1 voltage settings
2344      Y:000018 Y:000018         DACS      DC      END_DACS-DACS-1
2345      Y:000019 Y:000019                   DC      $2A0080                           ; DAC = unbuffered mode
2346      Y:00001A Y:00001A                   DC      $200100+@CVI(CLK_HI/CLKmax*255)   ; Pin #1, FSYNCB
2347      Y:00001B Y:00001B                   DC      $200200+@CVI(CLK_LO/CLKmax*255)
2348      Y:00001C Y:00001C                   DC      $200400+@CVI(CLK_HI/CLKmax*255)   ; Pin #2, LSYNCB
2349      Y:00001D Y:00001D                   DC      $200800+@CVI(CLK_LO/CLKmax*255)
2350      Y:00001E Y:00001E                   DC      $202000+@CVI(CLK_HI/CLKmax*255)   ; Pin #3, VCLK
2351      Y:00001F Y:00001F                   DC      $204000+@CVI(CLK_LO/CLKmax*255)
2352      Y:000020 Y:000020                   DC      $208000+@CVI(CLK_HI/CLKmax*255)   ; Pin #4, HCLK
2353      Y:000021 Y:000021                   DC      $210000+@CVI(CLK_LO/CLKmax*255)
2354      Y:000022 Y:000022                   DC      $220100+@CVI(CLK_HI/CLKmax*255)   ; Pin #5, RESETEN
2355      Y:000023 Y:000023                   DC      $220200+@CVI(CLK_LO/CLKmax*255)
2356      Y:000024 Y:000024                   DC      $220400+@CVI(CLK_HI/CLKmax*255)   ; Pin #6, READEN
2357      Y:000025 Y:000025                   DC      $220800+@CVI(CLK_LO/CLKmax*255)
2358   
2359      Y:000026 Y:000026                   DC      $260100+@CVI(CLK_HI/CLKmax*255)   ; Pin #13, MAINRESETB
2360      Y:000027 Y:000027                   DC      $260200+@CVI(CLK_LO/CLKmax*255)
2361      Y:000028 Y:000028                   DC      $260400+@CVI(CLK_HI/CLKmax*255)   ; Pin #14, CSB
2362      Y:000029 Y:000029                   DC      $260800+@CVI(CLK_LO/CLKmax*255)
2363      Y:00002A Y:00002A                   DC      $262000+@CVI(CLK_HI/CLKmax*255)   ; Pin #15, DATACLK
2364      Y:00002B Y:00002B                   DC      $264000+@CVI(CLK_LO/CLKmax*255)
2365      Y:00002C Y:00002C                   DC      $268000+@CVI(CLK_HI/CLKmax*255)   ; Pin #16, DATAIN
2366      Y:00002D Y:00002D                   DC      $270000+@CVI(CLK_LO/CLKmax*255)
2367      Y:00002E Y:00002E                   DC      $280100+@CVI(CLK_HI/CLKmax*255)   ; Pin #17, VERTWMEN
2368      Y:00002F Y:00002F                   DC      $280200+@CVI(CLK_LO/CLKmax*255)
2369      Y:000030 Y:000030                   DC      $280400+@CVI(CLK_HI/CLKmax*255)   ; Pin #18, HORIWMEN
2370      Y:000031 Y:000031                   DC      $280800+@CVI(CLK_LO/CLKmax*255)
2371                                END_DACS
2372   
2373                                ; Define switch state bits for CLK2 = "bottom" of clock board #1 = channels 0 to 11
2374      000001                    FSYNCB    EQU     1                                 ; Frame Sync                            Pin 
#1
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 44



2375      000002                    LSYNCB    EQU     2                                 ; Line Sync                             Pin 
#2
2376      000004                    VCLK      EQU     4                                 ; Vertical Clock                        Pin 
#3
2377      000008                    HCLK      EQU     8                                 ; Horizontal (fast pixel) Clock         Pin 
#4
2378      000010                    RESETEN   EQU     $10                               ; Reset enable                          Pin 
#5
2379      000020                    READEN    EQU     $20                               ; Read enable                           Pin 
#6
2380   
2381                                ; Define switch state bits for CLK3 = "top" of clock board #1 = channels 12 to 23
2382      000001                    MAINRESETB EQU    1                                 ; Reset the serial command register     Pin 
#13
2383      000002                    CSB       EQU     2                                 ; Serial Chip Select Bar                Pin 
#14
2384      000004                    DATACLK   EQU     4                                 ; Serial register clock input           Pin 
#15
2385      000008                    DATAIN    EQU     8                                 ; Serial register data input            Pin 
#16
2386      000010                    VERTWMEN  EQU     $10                               ; Enable vertical windowing mode        Pin 
#17
2387      000020                    HORIWMEN  EQU     $20                               ; Enable horizontal windowing mode      Pin 
#18
2388   
2389      000008                    XFER      EQU     8                                 ; Bit #3 = A/D data -> FIFO     (high going 
edge)
2390      000000                    X___      EQU     0
2391      000000                    START_AD  EQU     0                                 ; Bit #2 = A/D Convert          (low going e
dge to start conversion)
2392      000004                    S_______  EQU     4
2393      000000                    RESET_INTG EQU    0                                 ; Bit #1 = Reset Integrator     (=0 to reset
)
2394      000002                    R_________ EQU    2
2395                                 ENABLE_INTG
2396      000000                              EQU     0                                 ; Bit #0 = Integrate            (=0 to integ
rate)
2397                                 E__________
2398      000001                              EQU     1
2399   
2400      32202F                    HCLK_H    EQU     CLK2+DLY2+FSYNCB+LSYNCB+VCLK+HCLK+0000000+READEN
2401      322027                    HCLK_L    EQU     CLK2+DLY2+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2402   
2403                                ; Copy of the clocking bit definition for easy reference
2404                                ;       DC      CLK2+DELAY+FSYNCB+LSYNCB+VCLK+HCLK+RESETEN+READEN
2405                                ;       DC      CLK3+DELAY+MAINRESETB+CSB+DATACLK+DATAIN+VERTWMEN+HORIWMEN
2406   
2407                                FRAME_INIT
2408      Y:000032 Y:000032                   DC      END_FRAME_INIT-FRAME_INIT-1
2409      Y:000033 Y:000033                   DC      CLK2+DLY0+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2410      Y:000034 Y:000034                   DC      CLK2+DLY1+000000+LSYNCB+VCLK+0000+0000000+READEN ; Pulse FSYNCB low
2411      Y:000035 Y:000035                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2412                                END_FRAME_INIT
2413   
2414                                CLOCK_ROW
2415      Y:000036 Y:000036                   DC      END_CLOCK_ROW-CLOCK_ROW-1
2416      Y:000037 Y:000037                   DC      CLK2+DLY1+FSYNCB+000000+0000+0000+0000000+READEN ; Pulse VCLK and LSYNC low
2417      Y:000038 Y:000038                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2418                                END_CLOCK_ROW
2419   
2420                                CLOCK_ROW_RESET
2421      Y:000039 Y:000039                   DC      END_CLOCK_ROW_RESET-CLOCK_ROW_RESET-1
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 45



2422      Y:00003A Y:00003A                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+RESETEN+000000
2423      Y:00003B Y:00003B                   DC      CLK2+DLY1+FSYNCB+000000+VCLK+0000+RESETEN+000000 ; Pulse VCLK and LSYNC low
2424                                END_CLOCK_ROW_RESET
2425   
2426                                OVERCLOCK_ROW
2427      Y:00003C Y:00003C                   DC      END_OVERCLOCK_ROW-OVERCLOCK_ROW-1
2428      Y:00003D Y:00003D                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2429      Y:00003E Y:00003E                   DC      CLK2+DLY1+FSYNCB+LSYNCB+0000+0000+0000000+READEN ; Pulse VCLK low
2430      Y:00003F Y:00003F                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2431      Y:000040 Y:000040                   DC      CLK2+DLY0+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN ; This is the end of the read
out, put
2432      Y:000041 Y:000041                   DC      CLK2+DLY0+FSYNCB+LSYNCB+VCLK+0000+RESETEN+000000 ;   the H2RG in reset mode
2433                                END_OVERCLOCK_ROW
2434   
2435                                SKIP_ROW
2436      Y:000042 Y:000042                   DC      END_SKIP_ROW-SKIP_ROW-1
2437      Y:000043 Y:000043                   DC      CLK2+DLY1+FSYNCB+000000+0000+0000+0000000+READEN ; Pulse VCLK and LSYNC low
2438      Y:000044 Y:000044                   DC      CLK2+DLY1+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2439                                END_SKIP_ROW
2440   
2441                                SKIP_COL
2442      Y:000045 Y:000045                   DC      END_SKIP_COL-SKIP_COL-1
2443      Y:000046 Y:000046                   DC      $0A0000+HCLK_L
2444      Y:000047 Y:000047                   DC      $0A0000+HCLK_H
2445                                END_SKIP_COL
2446   
2447                                ; Clock the multiplexer, integrate the video signal, A/D convert and transmit
2448                                CLK_COL
2449      Y:000048 Y:000048                   DC      END_CLK_COL-CLK_COL-1
2450      Y:000049 Y:000049                   DC      HCLK_L                            ; HCLK low
2451      Y:00004A Y:00004A                   DC      VID0+$080000+X___+S_______+RESET_INTG+E__________ ; Reset integrator
2452      Y:00004B Y:00004B                   DC      VID0+$060000+X___+S_______+R_________+E__________ ; Settling time
2453      Y:00004C Y:00004C                   DC      VID0+$140000+X___+S_______+R_________+ENABLE_INTG ; Integrate
2454      Y:00004D Y:00004D                   DC      HCLK_H                            ; HCLK High
2455      Y:00004E Y:00004E                   DC      VID0+$120000+X___+S_______+R_________+ENABLE_INTG ; Integrate
2456      Y:00004F Y:00004F                   DC      VID0+$010000+X___+S_______+R_________+E__________ ; Settling time
2457      Y:000050 Y:000050                   DC      VID0+$000000+X___+START_AD+R_________+E__________ ; Start A/D conversion
2458      Y:000051 Y:000051                   DC      VID0+$0B0000+X___+S_______+R_________+E__________ ; A/D conversion time
2459      Y:000052 Y:000052                   DC      VID0+$000000+XFER+S_______+R_________+E__________ ; A/D data--> FIFO
2460      Y:000053 Y:000053                   DC      VID0+$000000+X___+S_______+R_________+E__________
2461      Y:000054 Y:000054                   DC      XMIT_32                           ; Transmit pixels
2462                                END_CLK_COL
2463                                ;XMT_PXL        DC      XMIT_32                                                 ; Transmit pixel
s
2464                                ;END_CLK_COL
2465                                ;       DC      7,XMIT15,7,XMIT23,7,XMIT31,7                    ; SXMITs in 4-channel mode
2466                                ;END_CLK_COL_4
2467      Y:000055 Y:000055         XMT_PXL   DC      XMIT_32                           ; Transmit pixels
2468   
2469                                ; Here is an alternative syntax for the same thing:
2470                                ; Video processor bit definitions
2471                                ;       Bit #3 = Move A/D data to FIFO  (high going edge)
2472                                ;       Bit #2 = A/D Convert            (low going edge to start conversion)
2473                                ;       Bit #1 = Reset Integrator       (=0 to reset)
2474                                ;       Bit #0 = Integrate              (=0 to integrate)
2475   
2476                                ; Clock the multiplexer, integrate the video signal, A/D convert, do not transmit^M
2477                                CLK_COL_NO_XMIT
2478      Y:000056 Y:000056                   DC      END_CLK_COL_NO_XMIT-CLK_COL_NO_XMIT-1
2479      Y:000057 Y:000057                   DC      HCLK_L                            ; HCLK low^M
2480      Y:000058 Y:000058                   DC      VID0+$080000+X___+S_______+RESET_INTG+E__________ ; Reset integrator^M
2481      Y:000059 Y:000059                   DC      VID0+$060000+X___+S_______+R_________+E__________ ; Settling time^M
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 46



2482      Y:00005A Y:00005A                   DC      VID0+$140000+X___+S_______+R_________+ENABLE_INTG ; Integrate^M
2483      Y:00005B Y:00005B                   DC      HCLK_H                            ; HCLK High^M
2484      Y:00005C Y:00005C                   DC      VID0+$120000+X___+S_______+R_________+ENABLE_INTG ; Integrate^M
2485      Y:00005D Y:00005D                   DC      VID0+$010000+X___+S_______+R_________+E__________ ; Settling time^M
2486      Y:00005E Y:00005E                   DC      VID0+$000000+X___+START_AD+R_________+E__________ ; Start A/D conversion
2487      Y:00005F Y:00005F                   DC      VID0+$0B0000+X___+S_______+R_________+E__________ ; A/D conversion time
2488      Y:000060 Y:000060                   DC      VID0+$000000+XFER+S_______+R_________+E__________ ; A/D data--> FIFO^M
2489      Y:000061 Y:000061                   DC      VID0+$000000+X___+S_______+R_________+E__________
2490                                END_CLK_COL_NO_XMIT
2491   
2492   
2493   
2494                                ; Execute the global reset function
2495                                GLOBAL_RESET
2496      Y:000062 Y:000062                   DC      END_GLOBAL_RESET-GLOBAL_RESET-1
2497      Y:000063 Y:000063                   DC      CLK2+$A00000+FSYNCB+LSYNCB+VCLK+0000+RESETEN+000000 ; Hold RESETEN high for 20
 microsec
2498      Y:000064 Y:000064                   DC      CLK2+$A00000+FSYNCB+LSYNCB+VCLK+0000+RESETEN+000000
2499      Y:000065 Y:000065                   DC      CLK2+$000000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2500                                END_GLOBAL_RESET
2501   
2502                                ; Advance the pixel clock without A/D conversions at the beginning of each line
2503                                FIRST_HCLKS
2504      Y:000066 Y:000066                   DC      END_FIRST_HCLKS-FIRST_HCLKS-1
2505      Y:000067 Y:000067                   DC      CLK2+$240000+FSYNCB+LSYNCB+VCLK+HCLK+0000000+READEN ; Cycle HCLK
2506      Y:000068 Y:000068                   DC      CLK2+$240000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2507      Y:000069 Y:000069                   DC      CLK2+$240000+FSYNCB+LSYNCB+VCLK+HCLK+0000000+READEN ; Cycle HCLK
2508      Y:00006A Y:00006A                   DC      CLK2+$040000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2509                                END_FIRST_HCLKS
2510   
2511                                ; Bring HCLK low at the end of each line
2512                                LAST_HCLKS
2513      Y:00006B Y:00006B                   DC      END_LAST_HCLKS-LAST_HCLKS-1
2514      Y:00006C Y:00006C                   DC      CLK2+DLY0+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2515      Y:00006D Y:00006D                   DC      CLK2+DLY0+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2516      Y:00006E Y:00006E                   DC      CLK2+DLY2+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2517                                END_LAST_HCLKS
2518   
2519                                ; One microsec delay
2520                                ONE_MICROSEC_DELAY
2521      Y:00006F Y:00006F                   DC      END_ONE_MICROSEC_DELAY-ONE_MICROSEC_DELAY-1
2522      Y:000070 Y:000070                   DC      CLK2+$0B0000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2523      Y:000071 Y:000071                   DC      CLK2+$0B0000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2524      Y:000072 Y:000072                   DC      CLK2+$000000+FSYNCB+LSYNCB+VCLK+0000+0000000+READEN
2525                                END_ONE_MICROSEC_DELAY
2526   
2527                                ; The remaining commands are for the serial interface
2528                                CSB_LOW
2529      Y:000073 Y:000073                   DC      END_CSB_LOW-CSB_LOW-1
2530      Y:000074 Y:000074                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+DATAIN+00000000+00000000
2531      Y:000075 Y:000075                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+DATAIN+00000000+00000000
2532      Y:000076 Y:000076                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+DATAIN+00000000+00000000
2533                                END_CSB_LOW
2534   
2535                                CSB_HIGH
2536      Y:000077 Y:000077                   DC      END_CSB_HIGH-CSB_HIGH-1
2537      Y:000078 Y:000078                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+DATAIN+00000000+00000000
2538      Y:000079 Y:000079                   DC      CLK3+DLYR+MAINRESETB+CSB+DATACLK+DATAIN+00000000+00000000
2539      Y:00007A Y:00007A                   DC      CLK3+DLYR+MAINRESETB+CSB+DATACLK+DATAIN+00000000+00000000
2540                                END_CSB_HIGH
2541   
2542                                CLOCK_SERIAL_ONE
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 47



2543      Y:00007B Y:00007B                   DC      END_CLOCK_SERIAL_ONE-CLOCK_SERIAL_ONE-1
2544      Y:00007C Y:00007C                   DC      CLK3+DLYR+MAINRESETB+000+0000000+DATAIN+00000000+00000000
2545      Y:00007D Y:00007D                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+DATAIN+00000000+00000000
2546                                END_CLOCK_SERIAL_ONE
2547   
2548                                CLOCK_SERIAL_ZERO
2549      Y:00007E Y:00007E                   DC      END_CLOCK_SERIAL_ZERO-CLOCK_SERIAL_ZERO-1
2550      Y:00007F Y:00007F                   DC      CLK3+DLYR+MAINRESETB+000+0000000+000000+00000000+00000000
2551      Y:000080 Y:000080                   DC      CLK3+DLYR+MAINRESETB+000+DATACLK+000000+00000000+00000000
2552                                END_CLOCK_SERIAL_ZERO
2553   
2554                                ; Reset all the internal registers to default values
2555                                RST_INTERNAL_REGISTERS
2556      Y:000081 Y:000081                   DC      END_RST_INTERNAL_REGISTERS-RST_INTERNAL_REGISTERS-1
2557      Y:000082 Y:000082                   DC      CLK3+DLY2+0000000000+CSB+DATACLK+DATAIN+00000000+00000000
2558      Y:000083 Y:000083                   DC      CLK3+DLY2+0000000000+CSB+DATACLK+DATAIN+00000000+00000000
2559      Y:000084 Y:000084                   DC      CLK3+DLY0+MAINRESETB+CSB+DATACLK+DATAIN+00000000+00000000
2560      Y:000085 Y:000085                   DC      CLK3+DLY0+MAINRESETB+CSB+DATACLK+DATAIN+00000000+00000000
2561                                END_RST_INTERNAL_REGISTERS
2562   
2563                                ; Enable and disable native H2RG windowing mode
2564                                ENABLE_WM
2565      Y:000086 Y:000086                   DC      END_ENABLE_WM-ENABLE_WM-1
2566      Y:000087 Y:000087                   DC      CLK3+DLY2+MAINRESETB+CSB+DATACLK+DATAIN+VERTWMEN+HORIWMEN
2567                                END_ENABLE_WM
2568   
2569                                DISABLE_WM
2570      Y:000088 Y:000088                   DC      END_DISABLE_WM-DISABLE_WM-1
2571      Y:000089 Y:000089                   DC      CLK3+DLY2+MAINRESETB+CSB+DATACLK+DATAIN+00000000+00000000
2572                                END_DISABLE_WM
2573   
2574      Y:00008A Y:00008A         DC_BIASES DC      END_DC_BIASES-DC_BIASES-1
2575   
2576                                ; Integrator gain and FIFO reset
2577                                ;       DC      $0c3001                 ; Integrate 1, R = 4k, Low gain, Slow
2578      Y:00008B Y:00008B                   DC      $0c3000                           ; Integrate 2, R = 1k, High gain, Fast
2579      Y:00008C Y:00008C                   DC      $1c3000                           ; Integrate 2, R = 1k, High gain, Fast
2580      Y:00008D Y:00008D                   DC      $2c3000                           ; Integrate 2, R = 1k, High gain, Fast
2581      Y:00008E Y:00008E                   DC      $3c3000                           ; Integrate 2, R = 1k, High gain, Fast
2582      Y:00008F Y:00008F                   DC      $4c3000                           ; Integrate 2, R = 1k, High gain, Fast
2583      Y:000090 Y:000090                   DC      $5c3000                           ; Integrate 2, R = 1k, High gain, Fast
2584      Y:000091 Y:000091                   DC      $6c3000                           ; Integrate 2, R = 1k, High gain, Fast
2585      Y:000092 Y:000092                   DC      $7c3000                           ; Integrate 2, R = 1k, High gain, Fast
2586   
2587      Y:000093 Y:000093                   DC      $0c1000                           ; Reset image data FIFOs
2588   
2589                                ;       DC      $0d2000                 ; Low  16 A/D bits
2590      Y:000094 Y:000094                   DC      $0c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2591      Y:000095 Y:000095                   DC      $1c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2592      Y:000096 Y:000096                   DC      $2c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2593      Y:000097 Y:000097                   DC      $3c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2594      Y:000098 Y:000098                   DC      $4c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2595      Y:000099 Y:000099                   DC      $5c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2596      Y:00009A Y:00009A                   DC      $6c2000                           ; High 16 A/D bits to backplane (hardware de
fault)
2597      Y:00009B Y:00009B                   DC      $7c2000                           ; High 16 A/D bits to backplane (hardware de
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 48



fault)
2598   
2599                                ;       DC      $0f2000                 ; WARP On
2600      Y:00009C Y:00009C                   DC      $0e2000                           ; WARP Off
2601      Y:00009D Y:00009D                   DC      $1e2000                           ; WARP Off
2602      Y:00009E Y:00009E                   DC      $2e2000                           ; WARP Off
2603      Y:00009F Y:00009F                   DC      $3e2000                           ; WARP Off
2604      Y:0000A0 Y:0000A0                   DC      $4e2000                           ; WARP Off
2605      Y:0000A1 Y:0000A1                   DC      $5e2000                           ; WARP Off
2606      Y:0000A2 Y:0000A2                   DC      $6e2000                           ; WARP Off
2607      Y:0000A3 Y:0000A3                   DC      $7e2000                           ; WARP Off
2608   
2609                                ; Note that pins #17 and #33 shuold be used to higher currents because they
2610                                ;   have 100 ohm filtering resistors, versus 1k on the other pins.
2611   
2612                                ; Bipolar +/-5 volts output voltages, on the 15 pin DB output connector
2613      Y:0000A4 Y:0000A4         DC_BIAS   DC      $0c4000+@CVI((DRAIN+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #17
2614      Y:0000A5 Y:0000A5                   DC      $0c8000+@CVI((SUB+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #33
2615      Y:0000A6 Y:0000A6                   DC      $0cc000+@CVI((ZERO+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #16
2616   
2617                                ; Unipolar 0 to 5 volts output voltages, video board #0
2618      Y:0000A7 Y:0000A7                   DC      $0d0000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #32
2619      Y:0000A8 Y:0000A8                   DC      $0d4000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #15
2620      Y:0000A9 Y:0000A9                   DC      $0d8000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #31
2621      Y:0000AA Y:0000AA                   DC      $0dc000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #14
2622   
2623                                ; Bipolar +/-5 volts output voltages, on the 15 pin DB output connector
2624      Y:0000AB Y:0000AB                   DC      $1c4000+@CVI((VDDA+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #17
2625      Y:0000AC Y:0000AC                   DC      $1c8000+@CVI((VBIASPOWER+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #33
2626      Y:0000AD Y:0000AD                   DC      $1cc000+@CVI((VBIASGATE+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #16
2627   
2628                                ; Unipolar 0 to 5 volts output voltages, video board #1
2629      Y:0000AE Y:0000AE                   DC      $1d0000+@CVI(VRESET/VIDEOmax*4095) ; P2, Pin #32
2630      Y:0000AF Y:0000AF                   DC      $1d4000+@CVI(DSUB/VIDEOmax*4095)  ; P2, Pin #15
2631      Y:0000B0 Y:0000B0                   DC      $1d8000+@CVI(CELLDRAIN/VIDEOmax*4095) ; P2, Pin #31
2632      Y:0000B1 Y:0000B1                   DC      $1dc000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #14
2633   
2634                                ; Bipolar +/-5 volts output voltages, on the 15 pin DB output connector
2635      Y:0000B2 Y:0000B2                   DC      $2c4000+@CVI((VDD+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #17
2636      Y:0000B3 Y:0000B3                   DC      $2c8000+@CVI((ZERO+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #33
2637      Y:0000B4 Y:0000B4                   DC      $2cc000+@CVI((ZERO+VIDEOmax)/(2.0*VIDEOmax)*4095) ; P2, Pin #162
2638   
2639                                ; Unipolar 0 to 5 volts output voltages, video board #2
2640      Y:0000B5 Y:0000B5                   DC      $2d0000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #32
2641      Y:0000B6 Y:0000B6                   DC      $2d4000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #15
2642      Y:0000B7 Y:0000B7                   DC      $2d8000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #31
2643      Y:0000B8 Y:0000B8                   DC      $2dc000+@CVI(ZERO/VIDEOmax*4095)  ; P2, Pin #14
2644   
2645                                ; Video processor offset voltages
2646      Y:0000B9 Y:0000B9                   DC      $0e0000+OFFSET                    ; Output #0, video board #0
2647      Y:0000BA Y:0000BA                   DC      $0e4000+OFFSET                    ; Output #1
2648      Y:0000BB Y:0000BB                   DC      $0e8000+OFFSET                    ; Output #2
2649      Y:0000BC Y:0000BC                   DC      $0ec000+OFFSET                    ; Output #3
2650      Y:0000BD Y:0000BD                   DC      $0f0000+OFFSET                    ; Output #4
2651      Y:0000BE Y:0000BE                   DC      $0f4000+OFFSET                    ; Output #5
2652      Y:0000BF Y:0000BF                   DC      $0f8000+OFFSET                    ; Output #6
2653      Y:0000C0 Y:0000C0                   DC      $0fc000+OFFSET                    ; Output #7
2654   
2655      Y:0000C1 Y:0000C1                   DC      $1e0000+OFFSET                    ; Output #0, video board #1
2656      Y:0000C2 Y:0000C2                   DC      $1e4000+OFFSET                    ; Output #1
2657      Y:0000C3 Y:0000C3                   DC      $1e8000+OFFSET                    ; Output #2
2658      Y:0000C4 Y:0000C4                   DC      $1ec000+OFFSET                    ; Output #3
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 49



2659      Y:0000C5 Y:0000C5                   DC      $1f0000+OFFSET                    ; Output #4
2660      Y:0000C6 Y:0000C6                   DC      $1f4000+OFFSET                    ; Output #5
2661      Y:0000C7 Y:0000C7                   DC      $1f8000+OFFSET                    ; Output #6
2662      Y:0000C8 Y:0000C8                   DC      $1fc000+OFFSET                    ; Output #7
2663   
2664      Y:0000C9 Y:0000C9                   DC      $2e0000+OFFSET                    ; Output #0, video board #2
2665      Y:0000CA Y:0000CA                   DC      $2e4000+OFFSET                    ; Output #1
2666      Y:0000CB Y:0000CB                   DC      $2e8000+OFFSET                    ; Output #2
2667      Y:0000CC Y:0000CC                   DC      $2ec000+OFFSET                    ; Output #3
2668      Y:0000CD Y:0000CD                   DC      $2f0000+OFFSET                    ; Output #4
2669      Y:0000CE Y:0000CE                   DC      $2f4000+OFFSET                    ; Output #5
2670      Y:0000CF Y:0000CF                   DC      $2f8000+OFFSET                    ; Output #6
2671      Y:0000D0 Y:0000D0                   DC      $2fc000+OFFSET                    ; Output #7
2672   
2673      Y:0000D1 Y:0000D1                   DC      $3e0000+OFFSET                    ; Output #0, video board #3
2674      Y:0000D2 Y:0000D2                   DC      $3e4000+OFFSET                    ; Output #1
2675      Y:0000D3 Y:0000D3                   DC      $3e8000+OFFSET                    ; Output #2
2676      Y:0000D4 Y:0000D4                   DC      $3ec000+OFFSET                    ; Output #3
2677      Y:0000D5 Y:0000D5                   DC      $3f0000+OFFSET                    ; Output #4
2678      Y:0000D6 Y:0000D6                   DC      $3f4000+OFFSET                    ; Output #5
2679      Y:0000D7 Y:0000D7                   DC      $3f8000+OFFSET                    ; Output #6
2680      Y:0000D8 Y:0000D8                   DC      $3fc000+OFFSET                    ; Output #7
2681   
2682      Y:0000D9 Y:0000D9                   DC      $4e0000+OFFSET                    ; Output #0, video board #4
2683      Y:0000DA Y:0000DA                   DC      $4e4000+OFFSET                    ; Output #1
2684      Y:0000DB Y:0000DB                   DC      $4e8000+OFFSET                    ; Output #2
2685      Y:0000DC Y:0000DC                   DC      $4ec000+OFFSET                    ; Output #3
2686      Y:0000DD Y:0000DD                   DC      $4f0000+OFFSET                    ; Output #4
2687      Y:0000DE Y:0000DE                   DC      $4f4000+OFFSET                    ; Output #5
2688      Y:0000DF Y:0000DF                   DC      $4f8000+OFFSET                    ; Output #6
2689      Y:0000E0 Y:0000E0                   DC      $4fc000+OFFSET                    ; Output #7
2690   
2691      Y:0000E1 Y:0000E1                   DC      $5e0000+OFFSET                    ; Output #0, video board #5
2692      Y:0000E2 Y:0000E2                   DC      $5e4000+OFFSET                    ; Output #1
2693      Y:0000E3 Y:0000E3                   DC      $5e8000+OFFSET                    ; Output #2
2694      Y:0000E4 Y:0000E4                   DC      $5ec000+OFFSET                    ; Output #3
2695      Y:0000E5 Y:0000E5                   DC      $5f0000+OFFSET                    ; Output #4
2696      Y:0000E6 Y:0000E6                   DC      $5f4000+OFFSET                    ; Output #5
2697      Y:0000E7 Y:0000E7                   DC      $5f8000+OFFSET                    ; Output #6
2698      Y:0000E8 Y:0000E8                   DC      $5fc000+OFFSET                    ; Output #7
2699   
2700      Y:0000E9 Y:0000E9                   DC      $6e0000+OFFSET                    ; Output #0, video board #6
2701      Y:0000EA Y:0000EA                   DC      $6e4000+OFFSET                    ; Output #1
2702      Y:0000EB Y:0000EB                   DC      $6e8000+OFFSET                    ; Output #2
2703      Y:0000EC Y:0000EC                   DC      $6ec000+OFFSET                    ; Output #3
2704      Y:0000ED Y:0000ED                   DC      $6f0000+OFFSET                    ; Output #4
2705      Y:0000EE Y:0000EE                   DC      $6f4000+OFFSET                    ; Output #5
2706      Y:0000EF Y:0000EF                   DC      $6f8000+OFFSET                    ; Output #6
2707      Y:0000F0 Y:0000F0                   DC      $6fc000+OFFSET                    ; Output #7
2708   
2709      Y:0000F1 Y:0000F1                   DC      $7e0000+OFFSET                    ; Output #0, video board #7
2710      Y:0000F2 Y:0000F2                   DC      $7e4000+OFFSET                    ; Output #1
2711      Y:0000F3 Y:0000F3                   DC      $7e8000+OFFSET                    ; Output #2
2712      Y:0000F4 Y:0000F4                   DC      $7ec000+OFFSET                    ; Output #3
2713      Y:0000F5 Y:0000F5                   DC      $7f0000+OFFSET                    ; Output #4
2714      Y:0000F6 Y:0000F6                   DC      $7f4000+OFFSET                    ; Output #5
2715      Y:0000F7 Y:0000F7                   DC      $7f8000+OFFSET                    ; Output #6
2716      Y:0000F8 Y:0000F8                   DC      $7fc000+OFFSET                    ; Output #7
2717                                END_DC_BIASES
2718   
2719                                ;  Zero out the DC biases during the power-on sequence
2720                                ZERO_BIASES
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 50



2721      Y:0000F9 Y:0000F9                   DC      END_ZERO_BIASES-ZERO_BIASES-1
2722      Y:0000FA Y:0000FA                   DC      $0c4800                           ; Pin #17
2723      Y:0000FB Y:0000FB                   DC      $0c8800                           ; Pin #33
2724      Y:0000FC Y:0000FC                   DC      $0cc800                           ; Pin #16
2725      Y:0000FD Y:0000FD                   DC      $0d0000                           ; Pin #32
2726      Y:0000FE Y:0000FE                   DC      $0d4000                           ; Pin #15
2727      Y:0000FF Y:0000FF                   DC      $0d8000                           ; Pin #31
2728      Y:000100 Y:000100                   DC      $0dc000                           ; Pin #14
2729   
2730      Y:000101 Y:000101                   DC      $1c4800                           ; Pin #17
2731      Y:000102 Y:000102                   DC      $1c8800                           ; Pin #33
2732      Y:000103 Y:000103                   DC      $1cc800                           ; Pin #16
2733      Y:000104 Y:000104                   DC      $1d0000                           ; Pin #32
2734      Y:000105 Y:000105                   DC      $1d4000                           ; Pin #15
2735      Y:000106 Y:000106                   DC      $1d8000                           ; Pin #31
2736      Y:000107 Y:000107                   DC      $1dc000                           ; Pin #14
2737   
2738      Y:000108 Y:000108                   DC      $2c4800                           ; Pin #17
2739      Y:000109 Y:000109                   DC      $2c8800                           ; Pin #33
2740      Y:00010A Y:00010A                   DC      $2cc800                           ; Pin #16
2741      Y:00010B Y:00010B                   DC      $2d0000                           ; Pin #32
2742      Y:00010C Y:00010C                   DC      $2d4000                           ; Pin #15
2743      Y:00010D Y:00010D                   DC      $2d8000                           ; Pin #31
2744      Y:00010E Y:00010E                   DC      $2dc000                           ; Pin #14
2745   
2746      Y:00010F Y:00010F                   DC      $3c4800                           ; Pin #17
2747      Y:000110 Y:000110                   DC      $3c8800                           ; Pin #33
2748      Y:000111 Y:000111                   DC      $3cc800                           ; Pin #16
2749      Y:000112 Y:000112                   DC      $3d0000                           ; Pin #32
2750      Y:000113 Y:000113                   DC      $3d4000                           ; Pin #15
2751      Y:000114 Y:000114                   DC      $3d8000                           ; Pin #31
2752      Y:000115 Y:000115                   DC      $3dc000                           ; Pin #14
2753   
2754      Y:000116 Y:000116                   DC      $4c4800                           ; Pin #17
2755      Y:000117 Y:000117                   DC      $4c8800                           ; Pin #33
2756      Y:000118 Y:000118                   DC      $4cc800                           ; Pin #16
2757      Y:000119 Y:000119                   DC      $4d0000                           ; Pin #32
2758      Y:00011A Y:00011A                   DC      $4d4000                           ; Pin #15
2759      Y:00011B Y:00011B                   DC      $4d8000                           ; Pin #31
2760      Y:00011C Y:00011C                   DC      $4dc000                           ; Pin #14
2761   
2762      Y:00011D Y:00011D                   DC      $5c4800                           ; Pin #17
2763      Y:00011E Y:00011E                   DC      $5c8800                           ; Pin #33
2764      Y:00011F Y:00011F                   DC      $5cc800                           ; Pin #16
2765      Y:000120 Y:000120                   DC      $5d0000                           ; Pin #32
2766      Y:000121 Y:000121                   DC      $5d4000                           ; Pin #15
2767      Y:000122 Y:000122                   DC      $5d8000                           ; Pin #31
2768      Y:000123 Y:000123                   DC      $5dc000                           ; Pin #14
2769   
2770      Y:000124 Y:000124                   DC      $6c4800                           ; Pin #17
2771      Y:000125 Y:000125                   DC      $6c8800                           ; Pin #33
2772      Y:000126 Y:000126                   DC      $6cc800                           ; Pin #16
2773      Y:000127 Y:000127                   DC      $6d0000                           ; Pin #32
2774      Y:000128 Y:000128                   DC      $6d4000                           ; Pin #15
2775      Y:000129 Y:000129                   DC      $6d8000                           ; Pin #31
2776      Y:00012A Y:00012A                   DC      $6dc000                           ; Pin #14
2777   
2778      Y:00012B Y:00012B                   DC      $7c4800                           ; Pin #17
2779      Y:00012C Y:00012C                   DC      $7c8800                           ; Pin #33
2780      Y:00012D Y:00012D                   DC      $7cc800                           ; Pin #16
2781      Y:00012E Y:00012E                   DC      $7d0000                           ; Pin #32
2782      Y:00012F Y:00012F                   DC      $7d4000                           ; Pin #15
Motorola DSP56300 Assembler  Version 6.3.22   12-06-18  07:49:15  H2RG.waveforms  Page 51



2783      Y:000130 Y:000130                   DC      $7d8000                           ; Pin #31
2784      Y:000131 Y:000131                   DC      $7dc000                           ; Pin #14
2785                                END_ZERO_BIASES
2786   
2787   
2788   
2789   
2790   
2791   
2792   
2793                                 END_APPLICATON_Y_MEMORY
2794      000132                              EQU     @LCV(L)
2795   
2796                                ;  End of program
2797                                          END

0    Errors
0    Warnings


