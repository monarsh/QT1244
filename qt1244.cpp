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
#include "qt1244.h"


QT1244::QT1244() {
#if defined (STM32F4)

  // For MCUs STM32F4xx

#else

  // Others MCUs

#endif
}

bool QT1244::begin(uint8_t devAddr) {
/*
	(For MCUs STM32F4xx) DevAddress Target device address:
	The device 7 bits address value in datasheet
	must be shifted to the left before calling the interface.

	(Others MCUs) No shifted.
*/
#if defined (STM32F4)

  // For MCUs STM32F4xx
  if (devAddr == 57) {
    DEVADDR = QT1244_ADDR_1 << 1;
  }
  else if (devAddr == 7) {
    DEVADDR = QT1244_ADDR_2 << 1;
  }
  else if (devAddr == 17) {
    DEVADDR = QT1244_ADDR_3 << 1;
  }
  else if (devAddr == 117) {
    DEVADDR = QT1244_ADDR_4 << 1;
  }
  else {
    // TODO: Error Handling
  }
	
  I2C_HandleTypeDef hi2c = qt1244Init();
  while (hi2c.State != HAL_I2C_STATE_READY);

#else

  // Others MCUs

#endif

  return true;
}

bool QT1244::setups(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  qt1244Write(DEVADDR, COMMAND_ADDR, SETUPS_WRITE_ENABLE);

  qt1244Write(DEVADDR, NTHR_PTHR_NDRIFT_BL_ADDR, ((BL_VALUE << 6) | (NDRIFT_VALUE << 3) | NTHR_PTHR_VALUE));
  qt1244Write(DEVADDR, NDIL_FDIL_AKS_WAKE_ADDR, ((WAKE_VALUE << 7) | (AKS_VALUE << 6) | (FDIL_VALUE << 3) | NDIL_VALUE));
  qt1244Write(DEVADDR, CFO_1_ADDR, CFO_1_VALUE);
  qt1244Write(DEVADDR, CFO_2_ADDR, CFO_2_VALUE);
  qt1244Write(DEVADDR, NRD_ADDR, NRD_VALUE);
  qt1244Write(DEVADDR, SLEEP_MSYNC_NHYST_DEBUG_ADDR, ((DEBUG_VALUE << 6) | (NHYST_VALUE << 4) | (MSYNC_VALUE << 3) | SLEEP_VALUE));
  qt1244Write(DEVADDR, AWAKE_ADDR, AWAKE_VALUE);
  qt1244Write(DEVADDR, DHT_ADDR, DHT_VALUE);
  qt1244Write(DEVADDR, PDRIFT_SSYNC_ADDR, ((SSYNC_VALUE << 3) | PDRIFT_VALUE));
  qt1244Write(DEVADDR, LSLlsb_ADDR, LSLlsb_VALUE);
  qt1244Write(DEVADDR, LSLmsb_KGTT_ADDR, ((KGTT_VALUE << 4) | LSLmsb_VALUE));
  qt1244Write(DEVADDR, DWELL_RIB_THRM_FHM_ADDR, ((FHM_VALUE << 6) | (THRM_VALUE << 4) | (RIB_VALUE << 3) | DWELL_VALUE));
  qt1244Write(DEVADDR, FREQ0_ADDR, FREQ0_VALUE);
  qt1244Write(DEVADDR, FREQ1_ADDR, FREQ1_VALUE);
  qt1244Write(DEVADDR, FREQ2_ADDR, FREQ2_VALUE);
  qt1244Write(DEVADDR, NSTHR_NIL_ADDR, ((NIL_VALUE << 4) | NSTHR_VALUE));

#else

  // Others MCUs

#endif

  return true;
}

#if defined (STM32F4)

// For MCUs STM32F4xx
void QT1244::hardwareReset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	Delay_us(10);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

#else

  // Others MCUs

#endif
}

void QT1244::softwareReset(void) {
#if defined (STM32F4)
	
  // For MCUs STM32F4xx
	qt1244Write(DEVADDR, COMMAND_ADDR, FORCE_RESET);
	Delay_us(10);

#else

  // Others MCUs

#endif
}

#if defined (STM32F4)

// For MCUs STM32F4xx
uint8_t QT1244::changeStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	
#else

  // Others MCUs

#endif
}

bool QT1244::calibrateKeyAll(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_ALL);

#else

  // Others MCUs

#endif

  return true;
}

bool QT1244::calibrateKey(uint8_t key) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  if (key == 0) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_0);
  }
  else if (key == 1) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_1);
  }
  else if (key == 2) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_2);
  }
  else if (key == 3) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_3);
  }
  else if (key == 4) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_4);
  }
  else if (key == 5) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_5);
  }
  else if (key == 6) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_6);
  }
  else if (key == 7) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_7);
  }
  else if (key == 8) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_8);
  }
  else if (key == 9) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_9);
  }
  else if (key == 10) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_10);
  }
  else if (key == 11) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_11);
  }
  else if (key == 12) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_12);
  }
  else if (key == 13) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_13);
  }
  else if (key == 14) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_14);
  }
  else if (key == 15) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_15);
  }
  else if (key == 16) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_16);
  }
  else if (key == 17) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_17);
  }
  else if (key == 18) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_18);
  }
  else if (key == 19) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_19);
  }
  else if (key == 20) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_20);
  }
  else if (key == 21) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_21);
  }
  else if (key == 22) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_22);
  }
  else if (key == 23) {
    qt1244Write(DEVADDR, COMMAND_ADDR, CALIBRATE_KEY_23);
  }
  else {
    // TODO: Error Handling
  }

#else

  // Others MCUs

#endif

  return true;
}

uint8_t QT1244::scanKey(void) {
#if defined (STM32F4)
	
	// For MCUs STM32F4xx
	uint8_t key, keyMask;

	key = qt1244Read(DEVADDR, KEY_0TO7_ADDR);

	keyMask = 0x01;

	for (uint8_t i = 0; i <= 7; i++) {
		if (key == keyMask) {
			return key = i + 1;
		}
		else {
			keyMask <<= 1;
		}
	}

	key = qt1244Read(DEVADDR, KEY_8TO15_ADDR);

	keyMask = 0x01;

	for (uint8_t i = 8; i <= 15; i++) {
		if (key == keyMask) {
			return key = i + 1;
		}
		else {
			keyMask <<= 1;
		}
	}

	key = qt1244Read(DEVADDR, KEY_16TO23_ADDR);

	keyMask = 0x01;

	for (uint8_t i = 16; i <= 23; i++) {
		if (key == keyMask) {
			return key = i + 1;
		}
		else {
			keyMask <<= 1;
		}
	}


//	// For MCUs STM32F4xx
//	uint8_t key;
//	uint32_t keys[4], keyMask;	

//	keys[0] = 0x00000000;
//	keys[1] = 0x00000000;
//	keys[2] = 0x00000000;
//	keys[3] = 0x00000000;

//	keys[0] = qt1244Read(DEVADDR, KEY_0TO7_ADDR);
//	keys[1] = qt1244Read(DEVADDR, KEY_8TO15_ADDR);
//	keys[2] = qt1244Read(DEVADDR, KEY_16TO23_ADDR);
//	keys[3] = (keys[2] << 16) | (keys[1] << 8) | keys[0];

//	keyMask = 0x00000001;

//	for (uint8_t i = 0; i <= 23; i++) {
//		if (keys[3] == keyMask) {
//			return key = i + 1;
//		}
//		else {
//			keyMask <<= 1;
//		}
//	}


//	// For MCUs STM32F4xx
//	uint8_t key, keyMin, keyMax, keyMask;

//	for (uint8_t x = 0; x <= 2; x++) {
//		if (x == 0) {
//			key = qt1244Read(DEVADDR, KEY_0TO7_ADDR);
//			keyMin = 0;
//			keyMax = 7;
//		}
//		else if (x == 1) {
//			key = qt1244Read(DEVADDR, KEY_8TO15_ADDR);
//			keyMin = 8;
//			keyMax = 15;
//		}
//		else if (x == 2) {
//			key = qt1244Read(DEVADDR, KEY_16TO23_ADDR);
//			keyMin = 16;
//			keyMax = 23;
//		}
//		
//		keyMask = 0x01;

//		for (uint8_t i = keyMin; i <= keyMax; i++) {
//			if (key == keyMask) {
//				return key = i + 1;
//			}
//			else {
//				keyMask <<= 1;
//			}
//		}
//	}


//	// For MCUs STM32F4xx
//	uint8_t key;
//	uint32_t keys, keyMask;

//	keys = 0x00000000;

//	keys = qt1244Read(DEVADDR, KEY_0TO7_ADDR);
//	keys |= (qt1244Read(DEVADDR, KEY_0TO7_ADDR) << 8);
//	keys |= (qt1244Read(DEVADDR, KEY_0TO7_ADDR) << 16);

//	keyMask = 0x00000001;

//	for (uint8_t i = 0; i < 24; i++) {
//		if (keys == keyMask) {
//			key = i;
//			return key += 1;
//		}
//		else {
//			keyMask <<= 1;
//		}
//	}

#else

  // Others MCUs

#endif
	
	return 0;
}

bool QT1244::HCRCStatus(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  uint8_t x = qt1244Read(DEVADDR, STATUS_ADDR);
  x &= 0x01;
  if (x == 0x01) {
    return true;
  }
  else {
    return false;
  }

#else

  // Others MCUs

#endif
}

bool QT1244::mainSyncErrorStatus(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  uint8_t x = qt1244Read(DEVADDR, STATUS_ADDR);
  x &= 0x02;
  if (x == 0x02) {
    return true;
  }
  else {
    return false;
  }

#else

  // Others MCUs

#endif
}

bool QT1244::keyCalibrationStatus(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  uint8_t x = qt1244Read(DEVADDR, STATUS_ADDR);
  x &= 0x04;
  if (x == 0x04) {
    return true;
  }
  else {
    return false;
  }

#else

  // Others MCUs

#endif
}

bool QT1244::LSLStatus(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  uint8_t x = qt1244Read(DEVADDR, STATUS_ADDR);
  x &= 0x08;
  if (x == 0x08) {
    return true;
  }
  else {
    return false;
  }

#else

  // Others MCUs

#endif
}

bool QT1244::FMEAStatus(void) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  uint8_t x = qt1244Read(DEVADDR, STATUS_ADDR);
  x &= 0x10;
  if (x == 0x10) {
    return true;
  }
  else {
    return false;
  }

#else

  // Others MCUs

#endif
}

void QT1244::debug(uint8_t no) {
#if defined (STM32F4)

  // For MCUs STM32F4xx
  if (no == 0) {
//    uint8_t NTHR_PTHR = NTHR_PTHR_VALUE;
//    uint8_t NDRIFT = NDRIFT_VALUE << 3;
//    uint8_t BL = BL_VALUE << 6;
//    uint8_t NDIL = NDIL_VALUE;
//    uint8_t FDIL = FDIL_VALUE << 3;
//    uint8_t AKS = AKS_VALUE << 6;
//    uint8_t WAKE = WAKE_VALUE << 7;
//    uint8_t CFO_1 = CFO_1_VALUE;
//    uint8_t CFO_2 = CFO_2_VALUE;
//    uint8_t NRD = NRD_VALUE;
//    uint8_t SLEEP = SLEEP_VALUE;
//    uint8_t MSYNC = MSYNC_VALUE << 3;
//    uint8_t NHYST = NHYST_VALUE << 4;
//    uint8_t DEBUG = DEBUG_VALUE << 6;
//    uint8_t AWAKE = AWAKE_VALUE;
//    uint8_t DHT = DHT_VALUE;
//    uint8_t PDRIFT = PDRIFT_VALUE;
//    uint8_t SSYNC = SSYNC_VALUE << 3;
//    uint8_t LSL_LSB = LSLlsb_VALUE;
//    uint8_t LSL_MSB = LSLmsb_VALUE;
//    uint8_t KGTT = KGTT_VALUE << 4;
//    uint8_t DWELL = DWELL_VALUE;
//    uint8_t RIB = RIB_VALUE << 3;
//    uint8_t THRM = THRM_VALUE << 4;
//    uint8_t FHM = FHM_VALUE << 6;
//    uint8_t FREQ0 = FREQ0_VALUE;
//    uint8_t FREQ1 = FREQ1_VALUE;
//    uint8_t FREQ2 = FREQ2_VALUE;
//    uint8_t NSTHR = NSTHR_VALUE;
//    uint8_t NIL = NIL_VALUE << 4;
  }

#else

  // Others MCUs

#endif
}

/********************************************************************
  16 bits crc calculation. Initial crc entry value must be 0.
  The message is not augmented with 'zero' bits.
  polynomial = X16 + X15 + X2 + 1
  data is an 8 bit number, unsigned
  crc is a 16 bit number, unsigned
  repeat this function for each data block byte, folding the result
  back into the call parameter crc
********************************************************************/
unsigned long CRC16BitCalc(unsigned long crc, unsigned char data) {
  unsigned char index;  // shift counter
  crc ^= (unsigned long)(data) << 8;
  index = 8;

  do {  // loop 8 times
    if (crc & 0x8000) {
      crc = (crc << 1) ^ 0x1021;
    }
    else {
      crc = crc << 1;
    }
  } while (--index);

  return crc;
}
