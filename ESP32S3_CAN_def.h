/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Thomas Barth, barth-dev.de
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __DRIVERS_CAN_REGDEF_H_
#define __DRIVERS_CAN_REGDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum  {
	CAN_SPEED_100KBPS=100, 				/**< \brief CAN Node runs at 100kBit/s. */
	CAN_SPEED_125KBPS=125, 				/**< \brief CAN Node runs at 125kBit/s. */
	CAN_SPEED_250KBPS=250, 				/**< \brief CAN Node runs at 250kBit/s. */
	CAN_SPEED_500KBPS=500, 				/**< \brief CAN Node runs at 500kBit/s. */
	CAN_SPEED_800KBPS=800, 				/**< \brief CAN Node runs at 800kBit/s. */
	CAN_SPEED_1000KBPS=1000				/**< \brief CAN Node runs at 1000kBit/s. */
}CAN_speed_t;

/**
 * \brief CAN frame type (standard/extended)
 */
typedef enum {
	CAN_frame_std=0, 						/**< Standard frame, using 11 bit identifer. */
	CAN_frame_ext=1 						/**< Extended frame, using 29 bit identifer. */
}CAN_frame_format_t;

/**
 * \brief CAN RTR
 */
typedef enum {
	CAN_no_RTR=0, 							/**< No RTR frame. */
	CAN_RTR=1 								/**< RTR frame. */
}CAN_RTR_t;

/** \brief Frame information record type */
typedef union{uint32_t U;					/**< \brief Unsigned access */
	 struct {
		uint8_t 			DLC:4;        	/**< \brief [3:0] DLC, Data length container */
		unsigned int 		unknown_2:2;    /**< \brief \internal unknown */
		CAN_RTR_t 			RTR:1;          /**< \brief [6:6] RTR, Remote Transmission Request */
		CAN_frame_format_t 	FF:1;           /**< \brief [7:7] Frame Format, see# CAN_frame_format_t*/
		unsigned int 		reserved_24:24;	/**< \brief \internal Reserved */
	} B;
} CAN_FIR_t;

/** \brief Start address of CAN registers */
#define MODULE_CAN              					((volatile CAN_Module_t    *)0x6002B000)

/** \brief Get standard message ID */
#define _CAN_GET_STD_ID								(((uint32_t)MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.STD.ID[0] << 3) | \
													(MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.STD.ID[1] >> 5))

/** \brief Get extended message ID */
#define _CAN_GET_EXT_ID   (((uint32_t)MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[0] << 21) | \
													(MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[1] << 13) | \
													(MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[2] << 5) | \
													(MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[3] >> 3 ))

/** \brief Set standard message ID */
#define _CAN_SET_STD_ID(x)							MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.STD.ID[0] = ((x) >> 3);	\
													MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.STD.ID[1] = ((x) << 5);

/** \brief Set extended message ID */
#define _CAN_SET_EXT_ID(x)							MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[0] = ((x) >> 21);	\
													MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[1] = ((x) >> 13);	\
													MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[2] = ((x) >> 5);	\
													MODULE_CAN->MBX_CTRL.FCTRL.TX_RX.EXT.ID[3] = ((x) << 3);	\

/** \brief Interrupt status register */
typedef enum  {
	__CAN_IRQ_RX=			BIT(0),					/**< \brief RX Interrupt */
	__CAN_IRQ_TX=			BIT(1),					/**< \brief TX Interrupt */
	__CAN_IRQ_ERR=			BIT(2),					/**< \brief Error Interrupt */
	__CAN_IRQ_DATA_OVERRUN=	BIT(3),					/**< \brief Date Overrun Interrupt */
	__CAN_IRQ_RESERVED=		BIT(4),					/**< \brief Internal Reserved (Wake-up not supported) */
	__CAN_IRQ_ERR_PASSIVE=	BIT(5),					/**< \brief Passive Error Interrupt */
	__CAN_IRQ_ARB_LOST=		BIT(6),					/**< \brief Arbitration lost interrupt */
	__CAN_IRQ_BUS_ERR=		BIT(7),					/**< \brief Bus error Interrupt */
	__CAN_IRQ_MS_ERR=		BIT(8),					/**< \brief Miss error Interrupt */
}__CAN_IRQ_t;


/** \brief OCMODE options. */
typedef enum  {
	__CAN_OC_BOM=			0b00,					/**< \brief bi-phase output mode */
	__CAN_OC_TOM=			0b01,					/**< \brief test output mode */
	__CAN_OC_NOM=			0b10,					/**< \brief normal output mode */
	__CAN_OC_COM=			0b11,					/**< \brief clock output mode */
}__CAN_OCMODE_t;


/**
 * CAN controller (SJA1000).
 */
typedef struct {
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int RM:1; 						/**< \brief MOD.0 Reset Mode */
	        unsigned int LOM:1;            			/**< \brief MOD.1 Listen Only Mode */
	        unsigned int STM:1;                     /**< \brief MOD.2 Self Test Mode */
	        unsigned int AFM:1;                   	/**< \brief MOD.3 Acceptance Filter Mode */
	        unsigned int SM:1;            			/**< \brief MOD.4 Sleep Mode */
	        unsigned int reserved_27:27;            /**< \brief \internal Reserved */
	    } B;
	} MOD;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int TR:1; 						/**< \brief CMR.0 Transmission Request */
	        unsigned int AT:1;            			/**< \brief CMR.1 Abort Transmission */
	        unsigned int RRB:1;                     /**< \brief CMR.2 Release Receive Buffer */
	        unsigned int CDO:1;                   	/**< \brief CMR.3 Clear Data Overrun */
	        unsigned int GTS:1;            			/**< \brief CMR.4 Go To Sleep */
	        unsigned int reserved_27:27;            /**< \brief \internal Reserved */
	    } B;
	} CMR;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int RBS:1; 					/**< \brief SR.0 Receive Buffer Status */
	        unsigned int DOS:1;            			/**< \brief SR.1 Data Overrun Status */
	        unsigned int TBS:1;                     /**< \brief SR.2 Transmit Buffer Status */
	        unsigned int TCS:1;                   	/**< \brief SR.3 Transmission Complete Status */
	        unsigned int RS:1;            			/**< \brief SR.4 Receive Status */
	        unsigned int TS:1;            			/**< \brief SR.5 Transmit Status */
	        unsigned int ES:1;            			/**< \brief SR.6 Error Status */
	        unsigned int BS:1;            			/**< \brief SR.7 Bus Status */
	        unsigned int reserved_24:24;            /**< \brief \internal Reserved */
	    } B;
	} SR;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int RI:1; 						/**< \brief IR.0 Receive Interrupt */
	        unsigned int TI:1;            			/**< \brief IR.1 Transmit Interrupt */
	        unsigned int EI:1;                     	/**< \brief IR.2 Error Interrupt */
	        unsigned int DOI:1;                   	/**< \brief IR.3 Data Overrun Interrupt */
			unsigned int reserved4:1;            	/**< \brief Internal Reserved (Wake-up not supported) */
	        unsigned int EPI:1;            			/**< \brief IR.5 Error Passive Interrupt */
	        unsigned int ALI:1;            			/**< \brief IR.6 Arbitration Lost Interrupt */
	        unsigned int BEI:1;            			/**< \brief IR.7 Bus Error Interrupt */
			unsigned int MSI:1;            			/**< \brief IR.8 Miss Status Interrupt */
	        unsigned int reserved_9:23;             /**< \brief \internal Reserved */
	    } B;
	} IR;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int RIE:1; 					/**< \brief IER.0 Receive Interrupt Enable */
	        unsigned int TIE:1;            			/**< \brief IER.1 Transmit Interrupt Enable */
	        unsigned int EIE:1;                     /**< \brief IER.2 Error Interrupt Enable */
	        unsigned int DOIE:1;                   	/**< \brief IER.3 Data Overrun Interrupt Enable */
			unsigned int reserved4:1;            	/**< \brief Internal Reserved (Wake-up not supported) */
	        unsigned int EPIE:1;            		/**< \brief IER.5 Error Passive Interrupt Enable */
	        unsigned int ALIE:1;            		/**< \brief IER.6 Arbitration Lost Interrupt Enable */
	        unsigned int BEIE:1;            		/**< \brief IER.7 Bus Error Interrupt Enable */
	        unsigned int MSIE:1;            		/**< \brief IER.8 Miss Status Interrupt Enable */
	        unsigned int reserved_9:23;             /**< \brief \internal Reserved  */
	    } B;
	} IER;
    uint32_t RESERVED0;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int BRP:13; 					/**< \brief BTR0[12:0] Baud Rate Prescaler */
	        unsigned int reserved1:1; 				/**< \brief \internal Reserved */
	        unsigned int SJW:2;            			/**< \brief BTR0[15:14] Synchronization Jump Width */
	        unsigned int reserved16:16;            /**< \brief \internal Reserved  */
	    } B;
	} BTR0;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
	        unsigned int TSEG1:4; 					/**< \brief BTR1[3:0] Timing Segment 1 */
	        unsigned int TSEG2:3;            		/**< \brief BTR1[6:4] Timing Segment 2*/
	        unsigned int SAM:1;            			/**< \brief BTR1.7 Sampling*/
	        unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} BTR1;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int OCMODE:2; 					/**< \brief OCR[1:0] Output Control Mode, see # */
			unsigned int OCPOL0:1;                  /**< \brief OCR.2 Output Control Polarity 0 */
			unsigned int OCTN0:1;                   /**< \brief OCR.3 Output Control Transistor N0 */
			unsigned int OCTP0:1;            		/**< \brief OCR.4 Output Control Transistor P0 */
			unsigned int OCPOL1:1;            		/**< \brief OCR.5 Output Control Polarity 1 */
			unsigned int OCTN1:1;            		/**< \brief OCR.6 Output Control Transistor N1 */
			unsigned int OCTP1:1;            		/**< \brief OCR.7 Output Control Transistor P1 */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} OCR;
    uint32_t RESERVED1[2];
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int ALC:8; 					/**< \brief ALC[7:0] Arbitration Lost Capture */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} ALC;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int ECC:8; 					/**< \brief ECC[7:0] Error Code Capture */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} ECC;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int EWLR:8; 					/**< \brief EWLR[7:0] Error Warning Limit */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} EWLR;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int RXERR:8; 					/**< \brief RXERR[7:0] Receive Error Counter */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} RXERR;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int TXERR:8; 					/**< \brief TXERR[7:0] Transmit Error Counter */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved  */
	    } B;
	} TXERR;

    union {
        struct {
            uint32_t CODE[4];						/**< \brief Acceptance Message ID */
            uint32_t MASK[4];						/**< \brief Acceptance Mask */
            uint32_t RESERVED2[5];
        } ACC;										/**< \brief Acceptance filtering */
        struct {
        	CAN_FIR_t	FIR;						/**< \brief Frame information record */
        	union{
				struct {
					uint32_t ID[2];					/**< \brief Standard frame message-ID*/
					uint32_t data[8];				/**< \brief Standard frame payload */
					uint32_t reserved[2];
				} STD;								/**< \brief Standard frame format */
				struct {
					uint32_t ID[4];					/**< \brief Extended frame message-ID*/
					uint32_t data[8];				/**< \brief Extended frame payload */
				} EXT;								/**< \brief Extended frame format */
        	}TX_RX;									/**< \brief RX/TX interface */
        }FCTRL;										/**< \brief Function control regs */
    } MBX_CTRL;										/**< \brief Mailbox control */
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int RMC:8; 					/**< \brief RMC[7:0] RX Message Counter */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved Enable */
	    } B;
	} RMC;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int RBSA:8; 					/**< \brief RBSA[7:0] RX Buffer Start Address  */
			unsigned int reserved_24:24;            /**< \brief \internal Reserved Enable */
	    } B;
	} RBSA;
	union{uint32_t U;								/**< \brief Unsigned access */
	    struct {
			unsigned int COD:8; 					/**< \brief CDR[7:0] CLKOUT frequency selector based of fOSC*/
			unsigned int COFF:1; 					/**< \brief CDR.8 CLKOUT off*/
			unsigned int reserved_9:23;             /**< \brief \internal Reserved  */
	    } B;
	} CDR;
    uint32_t IRAM[2];
}CAN_Module_t;

#ifdef __cplusplus
}
#endif

#endif /* __DRIVERS_CAN_REGDEF_H_ */
