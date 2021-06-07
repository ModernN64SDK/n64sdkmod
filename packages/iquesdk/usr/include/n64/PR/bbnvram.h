#ifndef _BBNVRAM_H_
#define _BBNVRAM_H_
/*
 * Defines for the store controller commands and parameters, and also setup/hold/width parameters
 * for the Charge Pump and Novea array
 */

/*
 * NMS commands
 */
#define NMS_CMD_IDLE        0
#define NMS_CMD_SERIAL_SRAM 1
#define NMS_CMD_STORE       2
#define NMS_CMD_RECALL      3
#define NMS_CMD_COMPARE     4
#define NMS_CMD_KEEP        5
#define NMS_CMD_RECALL_AUTO 6

/*
 * Configuration register shift values
 *
 * CRSTO_0
 */
#define NMS_STORE_PW_SHIFT 0
#define NMS_STORE_PW_MASK  0x3
#define NMS_NMAX_SHIFT     2
#define NMS_NMAX_MASK      0xc
#define NMS_VPPLEVEL_SHIFT 4
#define NMS_VPPLEVEL_MASK  0xf0

/*
 * CRSTO_1
 */
#define NMS_VPPMAX_SHIFT   0
#define NMS_VPPMAX_MASK    0x3
#define NMS_VPPDELTA_SHIFT 2
#define NMS_VPPDELTA_MASK  0xc
#define NMS_X_SHIFT        4
#define NMS_X_MASK         0x70

/*
 * CRM_0 - CRM_3
 */
#define NMS_T1_TECC0 (1<<0)
#define NMS_T1_TECC1 (1<<1)
#define NMS_T1_MRCL0 (1<<2)
#define NMS_T1_MRCL1 (1<<3)
#define NMS_T1_BIAS0 (1<<4)
#define NMS_T1_BIAS1 (1<<5)

#define NMS_T2_TECC0 (1<<6)
#define NMS_T2_TECC1 (1<<7)
#define NMS_T2_MRCL0 (1<<0)
#define NMS_T2_MRCL1 (1<<1)
#define NMS_T2_BIAS0 (1<<2)
#define NMS_T2_BIAS1 (1<<3)

#define NMS_T3_TECC0 (1<<4)
#define NMS_T3_TECC1 (1<<5)
#define NMS_T3_MRCL0 (1<<6)
#define NMS_T3_MRCL1 (1<<7)
#define NMS_T3_BIAS0 (1<<0)
#define NMS_T3_BIAS1 (1<<1)

#define NMS_T4_TECC0 (1<<2)
#define NMS_T4_TECC1 (1<<3)
#define NMS_T4_MRCL0 (1<<4)
#define NMS_T4_MRCL1 (1<<5)
#define NMS_T4_BIAS0 (1<<6)
#define NMS_T4_BIAS1 (1<<7)

#define NMS_T5_TECC0 (1<<0)
#define NMS_T5_TECC1 (1<<1)
#define NMS_T5_MRCL0 (1<<2)
#define NMS_T5_MRCL1 (1<<3)
#define NMS_T5_BIAS0 (1<<4)
#define NMS_T5_BIAS1 (1<<5)

/*
 * Store pulse width
 */
#define NMS_STORE_PW_2MS   0
#define NMS_STORE_PW_10MS  1
#define NMS_STORE_PW_50MS  2
#define NMS_STORE_PW_250MS 3

/*
 * VPP level, i.e. starting value for programming voltage
 */
#define NMS_VPPLEVEL_5P8V 0
#define NMS_VPPLEVEL_6P0V 1
#define NMS_VPPLEVEL_6P2V 2
#define NMS_VPPLEVEL_6P4V 3
#define NMS_VPPLEVEL_6P6V 4
#define NMS_VPPLEVEL_6P8V 5
#define NMS_VPPLEVEL_7P0V 6
#define NMS_VPPLEVEL_7P2V 7
#define NMS_VPPLEVEL_7P4V 8
#define NMS_VPPLEVEL_7P6V 9
#define NMS_VPPLEVEL_7P8V 10
#define NMS_VPPLEVEL_8P0V 11
#define NMS_VPPLEVEL_8P2V 12
#define NMS_VPPLEVEL_8P4V 13
#define NMS_VPPLEVEL_8P6V 14
#define NMS_VPPLEVEL_8P8V 15

/*
 * VPPmax level
 */
#define NMS_VPPMAX_7P6V 0
#define NMS_VPPMAX_8P0V 1
#define NMS_VPPMAX_8P4V 2
#define NMS_VPPMAX_8P8V 3

/*
 * VPPdelta. How much to increment by in store algorithm
 */
#define NMS_VPPDELTA_200MV  0
#define NMS_VPPDELTA_400MV  1
#define NMS_VPPDELTA_800MV  2
#define NMS_VPPDELTA_1600MV 3

/*
 * Maximum pulses at VPPmax. 
 */
#define NMS_NMAX_2   0
#define NMS_NMAX_10  1
#define NMS_NMAX_50  2
#define NMS_NMAX_250 3

/*
 * Important timing parameters for the charge pump - in nSecs
 */
#define Trh  500100 // Reset pulse length - XXX Added extra to make everything be ready
#define Trrc 300 // Reset recovery to clock
#define Trsp 180000 // Time from STORE high to PE on
#define Trps 2000000 // Time from PE low to STORE low
#define Tpvh 1000000 // Time from PE high to VPP output high
#define Tpvl 1000000 // Time from PE low to VPP output low

/*
 * Important timing parameters for NoVEA array - in nSec
 *
 * RECALL
 */
#define Trcready 44018 // Time from RECALL high to RCREADY, 
                       // time from COMP high to RCREADY
/*
 * COMPARE
 *
 */
#define Tscomp 8851 // Time from Bus stable to COMP high
#define Tcmatch 44013 // Time from COMP high to MATCH high

/*
 * Recovery times
 */
#define Trstore 3837103 // Time from STORE low to be ready for another operation
#define Trcomp 4009 // Time from COMP low to be ready for another operation
#define Trrecall 4009 // Time from RECALL low to be ready for another operation

#endif
