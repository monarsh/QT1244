/*******************************************************************************************************
  Version 1.0

  Author by: Muhammad Hafiz Bin Mastro
  Title: Firmware Engineer
  Company:
    ParkAide Mobile (M) Sdn. Bhd.
    Unit D-9-13A, Block D, Oasis Square,
    Jalan PJU 1A/7A, Ara Damansara,
    47301 Petaling Jaya,
    Selangor Darul Ehsan,
    Malaysia.
    Mobile: +60 17-341 1270
    Telephone: +60 3-7859 6198
    Fax: +60 3-7859 9198

  This is a STM32F4xx library for the Atmel AT42QT1244 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
  Referred from Atmel-9631-AT42-QT1244_Datasheet.pdf

  Download datasheet from this link,
  http://ww1.microchip.com/downloads/en/devicedoc/Atmel-9631-AT42-QT1244_Datasheet.pdf
*******************************************************************************************************/
#ifndef __QT1244_H
#define __QT1244_H

#include "i2c.h"
#include "delay.h"


// QT1244 Interface Details

/*******************************************************************************
  From page 17
  Section 4.2: I2C Serial Communication Bus
	
	The device communicates over an I2C bus, only in slave mode.
	
	Pins A0, A1 are used to configure the I2C addresses and should be pulled up
	to Vdd or pulled down to Vss using 10 kohm resistors. These pins are read
	after reset, during initialisation.
	
	Table 4-1.: Interface Details
	
	| A1  | A0  | I2C Address |
	|		  |		  |  (decimal)  |
	| Vss | Vss |     57			|
	| Vss | Vdd | 		7				|
	| Vdd | Vss | 		17			|
	| Vdd | Vdd | 		117			|
*******************************************************************************/
#define QT1244_ADDR_1		57
#define QT1244_ADDR_2		7
#define QT1244_ADDR_3		17
#define QT1244_ADDR_4		117


// QT1244 Registers & Values

/*******************************************************************************
  From page 19, 20
  Section 5.1: Introduction

  Table 5-1.: Memory Map
  Table 5-1.: Memory Map (Continued)

  | Address |                       Use                          | Access |
  |    5    | Device Status. Collection of bit flags             |  Read  |
  |    6    | Detect status for keys 0 to 7, one bit per key     |  Read  |
  |    7    | Detect status for keys 8 to 15, one bit per key    |  Read  |
  |    8    | Detect status for keys 16 to 23, one bit per key   |  Read  |

  | Address |                       Use                          | Access |
  |         | Control command. Write 0xFF to calibrate all keys. |        |
  |         | Write 0xFE immediately before writing setups.      |        |
  |   140   | Write 0xFD to perform low level calibration and    | Write  |
  |         | offset for frequency hopping. Write k to calibrate |        |
  |         | key k. Write 0x18 to reset the device.             |        |
*******************************************************************************/
#define STATUS_ADDR       5
#define KEY_0TO7_ADDR     6
#define KEY_8TO15_ADDR    7
#define KEY_16TO23_ADDR		8
#define COMMAND_ADDR      140


/*******************************************************************************
  From page 27
  Section 5.9 Command Address – 140

  The Command Address (140) is a write-only location. Reading from this
  address will not cause any side-effects but returns undefined values.
  The value written defines the action taken.

  (0xFF) Calibrate All:
  Shortly after the QT1244 receives a value of 0xFF at the Command Address it
  recalibrates all keys and restarts operation.
  The host can monitor the progress of the calibration by checking the device
  status byte, and the data set for each key.

  (0xFE) Setups Write-enable:
  Writing a value of 0xFE to the Command Address write-enables the setups block
  of the device. Normally the setups are write-protected. The write-protection
  is engaged as soon as a read operation is performed at any address. By writing
  a value of 0xFE to the Command Address, the write-protection is disengaged.
  The Command Address is located conveniently immediately before the setups so
  that the write protection may be disengaged and the setups written in a single
  I2C communication sequence.

  (0xFD) Low Level Cal and Offset:
  Shortly after receiving this command the QT1244 performs a calibration and
  offset procedure across all keys and restarts operation. If a previous command
  0xFD is still being processed, the new request will be ignored. This command
  takes up to 3 seconds to complete. The host can monitor the progress of the
  calibration by checking the QT1244 Device Status at address 5. The calibration
  bit will be set throughout the process. The low level calibration and offset
  procedure involves the device calibrating each key in turn at each of the
  operating frequencies selected with FREQ0, FREQ1 and FREQ2, calculating the
  difference between the signals at those frequencies and storing the results as
  offsets into CFO_1 and CFO_2 for each key. When the procedure is complete, the
  host can read back the setups and record CFO_1 and CFO_2 into its own copy of
  the setups block. The QT1244 does not change the setups CRC, so there will be
  a mismatch in the setups CRC after this command completes. The onus is on the
  host to compute the CRC and upload a definitive setups block to the QT1244.

  (0x18) Force Reset:
  Shortly after the QT1244 receives a value of 0x18 at the Command Address it
  performs a reset. After any reset, the device automatically performs a full
  key calibration on all keys.

  (k) Calibrate Key:
  Writing a value k in the range 0 – 23 to the Command Address requests the
  QT1244 to recalibrate key k. The operation is the same as if 0xFF were
  written except only one key k is affected where k is from 0 – 23. The chosen
  key k is recalibrated in its native timeslot. Normal running of the part is
  not interrupted and all other keys operate correctly throughout. This command
  is for use only during normal operation to try to recover a single key that
  has failed calibration or is not calibrated correctly.
*******************************************************************************/
#define CALIBRATE_KEY_ALL						0xFF
#define SETUPS_WRITE_ENABLE         0xFE
#define LOW_LEVEL_CAL_AND_OFFSET    0xFD
#define FORCE_RESET                 0x18
#define CALIBRATE_KEY_0             0
#define CALIBRATE_KEY_1             1
#define CALIBRATE_KEY_2             2
#define CALIBRATE_KEY_3             3
#define CALIBRATE_KEY_4             4
#define CALIBRATE_KEY_5             5
#define CALIBRATE_KEY_6             6
#define CALIBRATE_KEY_7             7
#define CALIBRATE_KEY_8             8
#define CALIBRATE_KEY_9             9
#define CALIBRATE_KEY_10            10
#define CALIBRATE_KEY_11            11
#define CALIBRATE_KEY_12            12
#define CALIBRATE_KEY_13            13
#define CALIBRATE_KEY_14            14
#define CALIBRATE_KEY_15            15
#define CALIBRATE_KEY_16            16
#define CALIBRATE_KEY_17            17
#define CALIBRATE_KEY_18            18
#define CALIBRATE_KEY_19            19
#define CALIBRATE_KEY_20            20
#define CALIBRATE_KEY_21            21
#define CALIBRATE_KEY_22            22
#define CALIBRATE_KEY_23            23


/*******************************************************************************
  From page 28, 29, 30
  Section 6.1  : Address 141 – 164: NTHR, PTHR, NDRIFT, BL
  Section 6.1.1: Threshold – NTHR, PTHR
  Section 6.1.2: Drift Compensation – NDRIFT, PDRIFT
  Section 6.1.3: Burst Length – BL

  Table 6-1.: NTHR
  Table 6-2.: NDRIFT
  Table 6-3.: BL

  |  Address  | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  | 141 - 164 |      BL       |         NDRIFT        |       NTHR,PTHR       |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  NTHR, PTHR Default value: 3
  NDRIFT Default value: 4
  BL Default value: 2
*******************************************************************************/
#define NTHR_PTHR_NDRIFT_BL_ADDR		141

#define NTHR_PTHR_VALUE   					3	// Default value = 3
#define NDRIFT_VALUE      					4	// Default value = 4
#define BL_VALUE          					0	// Default value = 2


/*******************************************************************************
  From page 30, 31, 32
  Section 6.2  : Address 165 – 188: NDIL, FDIL, AKS, WAKE
  Section 6.2.1: Detect Integrators – NDIL, FDIL
  Section 6.2.2: Adjacent Key Suppression Technology – AKS
  Section 6.2.3: Wake on Touch – WAKE

  Table 6-4.: NDIL, FDIL
  Table 6-5.: AKS
  Table 6-6.: WAKE

  |  Address  | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  | 165 – 188 | WAKE  |  AKS  |         FDIL          |          NDIL         |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  NDIL Default value: 2
  FDIL Default value: 3
  AKS Default value: 0
  WAKE Default value: 1
*******************************************************************************/
#define NDIL_FDIL_AKS_WAKE_ADDR		165

#define NDIL_VALUE    						2	// Default value = 2
#define FDIL_VALUE    						3	// Default value = 3
#define AKS_VALUE     						1	// Default value = 0
#define WAKE_VALUE    						1	// Default value = 1


/*******************************************************************************
  From page 33
  Section 6.3: Calibrated Frequency Offset – CFO_1 and CFO_2

  Table 6-7.: CFO_1 and CFO_2

  |  Address  | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  | 189 – 212 |                             CFO_1                             |
  | 213 – 236 |                             CFO_2                             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  CFO_1/2 Default value: 0
*******************************************************************************/
#define CFO_1_ADDR    189
#define CFO_2_ADDR    213

#define CFO_1_VALUE   0	// Default value = 0
#define CFO_2_VALUE   0	// Default value = 0


/*******************************************************************************
  From page 34
  Section 6.4: Address 237: Negative Recal Delay – NRD

  Table 6-8.: NRD

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   237   |                              NRD                              |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  NRD Default value: 20
*******************************************************************************/
#define NRD_ADDR    237

#define NRD_VALUE   20	// Default value = 20


/*******************************************************************************
  From page 35, 36
  Section 6.5  : Address 238: SLEEP, MSYNC, NHYST, DEBUG
  Section 6.5.1: Sleep Duration – SLEEP
  Section 6.5.2: Mains Sync – MSYNC
  Section 6.5.3: Negative Hysteresis – NHYST
  Section 6.5.4: DEBUG Output

  Table 6-9. : SLEEP
  Table 6-10.: MSYNC
  Table 6-11.: NHYST
  Table 6-12.: DEBUG

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   238   |     DEBUG     |     NHYST     | MSYNC |         SLEEP         |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  SLEEP default value: 0
  MSYNC Default value: 0
  NHYST Default value: 1
  DEBUG Default value: 0
*******************************************************************************/
#define SLEEP_MSYNC_NHYST_DEBUG_ADDR		238

#define SLEEP_VALUE   									0	// Default value = 0
#define MSYNC_VALUE   									0	// Default value = 0
#define NHYST_VALUE   									1	// Default value = 1
#define DEBUG_VALUE   									0	// Default value = 0


/*******************************************************************************
  From page 37
  Section 6.6: Address 239: Awake Timeout – AWAKE

  Table 6-13.: AWAKE

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   239   |                             AWAKE                             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  AWAKE default value: 25
*******************************************************************************/
#define AWAKE_ADDR    239

#define AWAKE_VALUE   25	// Default value = 25


/*******************************************************************************
  From page 37
  Section 6.7: Address 240: Drift Hold Time – DHT

  Table 6-14.: DHT

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   240   |                              DHT                              |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  DHT default value: 0
*******************************************************************************/
#define DHT_ADDR    240

#define DHT_VALUE   0	// Default value = 0


/*******************************************************************************
  From page 38, 29
  Section 6.8  : Address 241: PDRIFT, SSYNC
  Section 6.8.1: Positive Drift Compensation – PDRIFT
  Section 6.8.2: Oscilloscope Sync – SSYNC

  Table 6-15.: PDRIFT
  Table 6-16.: SSYNC

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   241   |                 SSYNC                 |        PDRIFT         |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  PDRIFT Default value: 4
  SSYNC Default value: 0
*******************************************************************************/
#define PDRIFT_SSYNC_ADDR		241

#define PDRIFT_VALUE    		4	// Default value = 4
#define SSYNC_VALUE     		0	// Default value = 0


/*******************************************************************************
  From page 38, 39
  Section 6.9  : Address 242 – 243: LSL, KGTT
  Section 6.9.1: Lower Signal Limit – LSL
  Section 6.9.2: Key Gain Test Threshold – KGTT

  Table 6-17.: LSL
  Table 6-18.: KGTT

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   242   |                              LSL                              |
  |   243   |             KGTT              |   –   |          LSL          |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  LSL Default value: 100
  KGTT Default value: 6
*******************************************************************************/
#define LSLlsb_ADDR					242
#define LSLmsb_KGTT_ADDR		243

#define LSLlsb_VALUE   			100	// Default value = 100
#define LSLmsb_VALUE   			0  	// Default value = 0
#define KGTT_VALUE      		6  	// Default value = 6


/*******************************************************************************
  From page 39, 40, 41
  Section 6.10  : Address 244: DWELL, RIB, THRM, FHM
  Section 6.10.1: Dwell Time – DWELL
  Section 6.10.2: Restart Interrupted Burst – RIB
  Section 6.10.3: Threshold Multiplier – THRM
  Section 6.10.4: Frequency Hopping Mode – FHM

  Table 6-19.: DWELL
  Table 6-20.: RIB
  Table 6-21.: THRM
  Table 6-23.: FHM

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   244   |      FHM      |     THRM      |  RIB  |         DWELL         |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  DWELL Default value: 0
  RIB Default value: 0
  THRM Default value: 0
  FHM Default value: 3
*******************************************************************************/
#define DWELL_RIB_THRM_FHM_ADDR		244

#define DWELL_VALUE   						0	// Default value = 0
#define RIB_VALUE     						0	// Default value = 0
#define THRM_VALUE    						0	// Default value = 0
#define FHM_VALUE     						3	// Default value = 3


/*******************************************************************************
  From page 42
  Section 6.11: Address 245: Frequency 0 – FREQ0

  Table 6-24.: FREQ0

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   245   |                             FREQ0                             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  FREQ0 Default value: 1
*******************************************************************************/
#define FREQ0_ADDR    245

#define FREQ0_VALUE   1	// Default value = 1


/*******************************************************************************
  From page 42
  Section 6.12: Address 246: Frequency1 – FREQ1

  Table 6-25.: FREQ1

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   246   |                             FREQ1                             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  FREQ1 Default value: 6
*******************************************************************************/
#define FREQ1_ADDR    246

#define FREQ1_VALUE   6	// Default value = 6


/*******************************************************************************
  From page 43
  Section 6.13: Address 247: Frequency2 – FREQ2

  Table 6-26.: FREQ2

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   247   |                             FREQ2                             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  FREQ2 Default value: 63
*******************************************************************************/
#define FREQ2_ADDR    247

#define FREQ2_VALUE   63	// Default value = 63


/*******************************************************************************
  From page 43
  Section 6.14  : Address 248: NSTHR, NIL
  Section 6.14.1: Noise Threshold – NSTHR
  Section 6.14.2: Noise Integrator Limit – NIL

  Table 6-27.: NSTHR
  Table 0-1.: NIL

  | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  |   248   |              NIL              |             NSTHR             |

  From lookup-table (LUT), Table 7-1. on page 45 and Table 7-2. on page 46
  NSTHR Default value: 2
  NIL Default value: 3
*******************************************************************************/
#define NSTHR_NIL_ADDR		248

#define NSTHR_VALUE   		2	// Default value = 2
#define NIL_VALUE     		3	// Default value = 3


/*******************************************************************************
  From page 44
  Section 6.15: Address 249 – 250: Host CRC – HCRC

  Table 6-28.: HCRC

  |  Address  | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
  | 249 – 250 |                             HCRC                              |
*******************************************************************************/
#define HCRClsb_ADDR		249
#define HCRCmsb_ADDR		250


class QT1244 {
	public:
		QT1244();
		bool begin(uint8_t devAddr);
		bool setups(void);
		void hardwareReset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		void softwareReset(void);
		uint8_t changeStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		bool HCRCStatus(void);
		bool mainSyncErrorStatus(void);
		bool keyCalibrationStatus(void);
		bool LSLStatus(void);
		bool FMEAStatus(void);
		bool calibrateKeyAll(void);
		bool calibrateKey(uint8_t key);
		uint8_t scanKey(void);
		void debug(uint8_t no);
	
	private:
		uint8_t DEVADDR;
};

unsigned long CRC16BitCalc(unsigned long crc, unsigned char data);

#endif /* __QT1244_H */
