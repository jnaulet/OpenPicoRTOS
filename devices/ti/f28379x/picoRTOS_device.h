#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_ADCARESULT 0xb00
#define ADDR_ADCBRESULT 0xb20
#define ADDR_ADCCRESULT 0xb40
#define ADDR_ADCDRESULT 0xb60

#define ADDR_CPUTIMER0 0xc00
#define ADDR_CPUTIMER1 0xc08
#define ADDR_CPUTIMER2 0xc10

#define ADDR_PIECTRL      0xce0
#define ADDR_PIEVECTTABLE 0xd00

#define ADDR_EPWM1  0x4000
#define ADDR_EPWM2  0x4100
#define ADDR_EPWM3  0x4200
#define ADDR_EPWM4  0x4300
#define ADDR_EPWM5  0x4400
#define ADDR_EPWM6  0x4500
#define ADDR_EPWM7  0x4600
#define ADDR_EPWM8  0x4700
#define ADDR_EPWM9  0x4800
#define ADDR_EPWM10 0x4900
#define ADDR_EPWM11 0x4a00
#define ADDR_EPWM12 0x4b00
#define ADDR_EPWM13 0x4c00
#define ADDR_EPWM14 0x4d00
#define ADDR_EPWM15 0x4e00
#define ADDR_EPWM16 0x4f00

#define ADDR_SPIA 0x6100
#define ADDR_SPIB 0x6110
#define ADDR_SPIC 0x6120
#define ADDR_SPID 0x6130

#define ADDR_WD  0x7000
#define ADDR_NMI 0x7060

#define ADDR_I2CA 0x7300
#define ADDR_I2CB 0x7340

#define ADDR_GPIOCTRL_A 0x7c00
#define ADDR_GPIOCTRL_B 0x7c40
#define ADDR_GPIOCTRL_C 0x7c80
#define ADDR_GPIOCTRL_D 0x7cc0
#define ADDR_GPIOCTRL_E 0x7d00
#define ADDR_GPIOCTRL_F 0x7d40

#define ADDR_GPIODATA_A 0x7f00
#define ADDR_GPIODATA_B 0x7f08
#define ADDR_GPIODATA_C 0x7f10
#define ADDR_GPIODATA_D 0x7f18
#define ADDR_GPIODATA_E 0x7f20
#define ADDR_GPIODATA_F 0x7f28

#define ADDR_ADCA 0x7400
#define ADDR_ADCB 0x7480
#define ADDR_ADCC 0x7500
#define ADDR_ADCD 0x7580

#define ADDR_CANA 0x48000ul
#define ADDR_CANB 0x4a000ul

#define ADDR_ANALOGSUBSYS 0x5d180ul
#define ADDR_CLKCFG       0x5d200ul
#define ADDR_CPUSYS       0x5d300ul

/* IRQs */

#define IRQ_INT1_1     32
#define IRQ_INT1_2     33
#define IRQ_INT1_3     34
#define IRQ_INT1_4     35
#define IRQ_INT1_5     36
#define IRQ_INT1_6     37
#define IRQ_INT1_7     38
#define IRQ_INT1_8     39
#define IRQ_INT1_9     128
#define IRQ_INT1_10    129
#define IRQ_INT1_11    130
#define IRQ_INT1_12    131
#define IRQ_INT1_13    132
#define IRQ_INT1_14    133
#define IRQ_INT1_15    134
#define IRQ_INT1_16    135

#define IRQ_INT2_1     40
#define IRQ_INT2_2     41
#define IRQ_INT2_3     42
#define IRQ_INT2_4     43
#define IRQ_INT2_5     44
#define IRQ_INT2_6     45
#define IRQ_INT2_7     46
#define IRQ_INT2_8     47
#define IRQ_INT2_9     136
#define IRQ_INT2_10    137
#define IRQ_INT2_11    138
#define IRQ_INT2_12    139
#define IRQ_INT2_13    140
#define IRQ_INT2_14    141
#define IRQ_INT2_15    142
#define IRQ_INT2_16    143

#define IRQ_INT3_1     48
#define IRQ_INT3_2     49
#define IRQ_INT3_3     50
#define IRQ_INT3_4     51
#define IRQ_INT3_5     52
#define IRQ_INT3_6     53
#define IRQ_INT3_7     54
#define IRQ_INT3_8     55
#define IRQ_INT3_9     144
#define IRQ_INT3_10    145
#define IRQ_INT3_11    146
#define IRQ_INT3_12    147
#define IRQ_INT3_13    148
#define IRQ_INT3_14    149
#define IRQ_INT3_15    150
#define IRQ_INT3_16    151

#define IRQ_INT4_1     56
#define IRQ_INT4_2     57
#define IRQ_INT4_3     58
#define IRQ_INT4_4     59
#define IRQ_INT4_5     60
#define IRQ_INT4_6     61
#define IRQ_INT4_7     62
#define IRQ_INT4_8     63
#define IRQ_INT4_9     152
#define IRQ_INT4_10    153
#define IRQ_INT4_11    154
#define IRQ_INT4_12    155
#define IRQ_INT4_13    156
#define IRQ_INT4_14    157
#define IRQ_INT4_15    158
#define IRQ_INT4_16    159

#define IRQ_INT5_1     64
#define IRQ_INT5_2     65
#define IRQ_INT5_3     66
#define IRQ_INT5_4     67
#define IRQ_INT5_5     68
#define IRQ_INT5_6     69
#define IRQ_INT5_7     70
#define IRQ_INT5_8     71
#define IRQ_INT5_9     160
#define IRQ_INT5_10    161
#define IRQ_INT5_11    162
#define IRQ_INT5_12    163
#define IRQ_INT5_13    164
#define IRQ_INT5_14    165
#define IRQ_INT5_15    166
#define IRQ_INT5_16    167

#define IRQ_INT6_1     72
#define IRQ_INT6_2     73
#define IRQ_INT6_3     74
#define IRQ_INT6_4     75
#define IRQ_INT6_5     76
#define IRQ_INT6_6     77
#define IRQ_INT6_7     78
#define IRQ_INT6_8     79
#define IRQ_INT6_9     168
#define IRQ_INT6_10    169
#define IRQ_INT6_11    170
#define IRQ_INT6_12    171
#define IRQ_INT6_13    172
#define IRQ_INT6_14    173
#define IRQ_INT6_15    174
#define IRQ_INT6_16    175

#define IRQ_INT7_1     80
#define IRQ_INT7_2     81
#define IRQ_INT7_3     82
#define IRQ_INT7_4     83
#define IRQ_INT7_5     84
#define IRQ_INT7_6     85
#define IRQ_INT7_7     86
#define IRQ_INT7_8     87
#define IRQ_INT7_9     176
#define IRQ_INT7_10    177
#define IRQ_INT7_11    178
#define IRQ_INT7_12    179
#define IRQ_INT7_13    180
#define IRQ_INT7_14    181
#define IRQ_INT7_15    182
#define IRQ_INT7_16    183

#define IRQ_INT8_1     88
#define IRQ_INT8_2     89
#define IRQ_INT8_3     90
#define IRQ_INT8_4     91
#define IRQ_INT8_5     92
#define IRQ_INT8_6     93
#define IRQ_INT8_7     94
#define IRQ_INT8_8     95
#define IRQ_INT8_9     184
#define IRQ_INT8_10    185
#define IRQ_INT8_11    186
#define IRQ_INT8_12    187
#define IRQ_INT8_13    188
#define IRQ_INT8_14    189
#define IRQ_INT8_15    190
#define IRQ_INT8_16    191

#define IRQ_INT9_1     96
#define IRQ_INT9_2     97
#define IRQ_INT9_3     98
#define IRQ_INT9_4     99
#define IRQ_INT9_5     100
#define IRQ_INT9_6     101
#define IRQ_INT9_7     102
#define IRQ_INT9_8     103
#define IRQ_INT9_9     192
#define IRQ_INT9_10    193
#define IRQ_INT9_11    194
#define IRQ_INT9_12    195
#define IRQ_INT9_13    196
#define IRQ_INT9_14    197
#define IRQ_INT9_15    198
#define IRQ_INT9_16    199

#define IRQ_INT10_1     104
#define IRQ_INT10_2     105
#define IRQ_INT10_3     106
#define IRQ_INT10_4     107
#define IRQ_INT10_5     108
#define IRQ_INT10_6     109
#define IRQ_INT10_7     110
#define IRQ_INT10_8     111
#define IRQ_INT10_9     200
#define IRQ_INT10_10    201
#define IRQ_INT10_11    202
#define IRQ_INT10_12    203
#define IRQ_INT10_13    204
#define IRQ_INT10_14    205
#define IRQ_INT10_15    206
#define IRQ_INT10_16    207

#define IRQ_INT11_1     112
#define IRQ_INT11_2     113
#define IRQ_INT11_3     114
#define IRQ_INT11_4     115
#define IRQ_INT11_5     116
#define IRQ_INT11_6     117
#define IRQ_INT11_7     118
#define IRQ_INT11_8     119
#define IRQ_INT11_9     208
#define IRQ_INT11_10    209
#define IRQ_INT11_11    210
#define IRQ_INT11_12    211
#define IRQ_INT11_13    212
#define IRQ_INT11_14    213
#define IRQ_INT11_15    214
#define IRQ_INT11_16    215

#define IRQ_INT12_1     120
#define IRQ_INT12_2     121
#define IRQ_INT12_3     122
#define IRQ_INT12_4     123
#define IRQ_INT12_5     124
#define IRQ_INT12_6     125
#define IRQ_INT12_7     126
#define IRQ_INT12_8     127
#define IRQ_INT12_9     216
#define IRQ_INT12_10    217
#define IRQ_INT12_11    218
#define IRQ_INT12_12    219
#define IRQ_INT12_13    220
#define IRQ_INT12_14    221
#define IRQ_INT12_15    222
#define IRQ_INT12_16    223

/* DEVICE opts */

#define DEVICE_INTERRUPT_VECTOR_COUNT 192

#endif
