#pragma once
#ifndef __EDMA_H__
#define __EDMA_H__


//***********************************************************************
//  Values that can be used by the application for configuration via APIs
//***********************************************************************/

/** Maximum number of EDMA Shadow regions available */
#define EDMA3_MAX_REGIONS                     (2u)

/** Number of PaRAM Sets available */
#define EDMA3_NUM_PARAMSET                    (128u)

/** Number of Event Queues available */
#define EDMA3_0_NUM_EVTQUE                    (2u)

/** Number of Transfer Controllers available */
#define EDMA3_0_NUM_TC                        (2u)

/** Interrupt no. for Transfer Completion */
#define EDMA3_0_CC_XFER_COMPLETION_INT        (11u)

/** Interrupt no. for CC Error */
#define EDMA3_0_CC0_ERRINT                    (12u)

/** Interrupt no. for TCs Error */
#define EDMA3_0_TC0_ERRINT                    (13u)
#define EDMA3_0_TC1_ERRINT                    (32u)

#define EDMA3CC_DMA_CHANNELS                  (32u)
#define EDMA3CC_QDMA_CHANNELS                 (8u)

/** DMAQNUM bits Clear */
#define EDMA3CC_DMAQNUM_CLR(chNum)            ( ~ (0x7u << (((chNum) % 8u) \
                                                                     * 4u)))
/** DMAQNUM bits Set */
#define EDMA3CC_DMAQNUM_SET(chNum,queNum)     ((0x7u & (queNum)) << \
                                                      (((chNum) % 8u) * 4u))
/** QDMAQNUM bits Clear */
#define EDMA3CC_QDMAQNUM_CLR(chNum)           ( ~ (0x7u << ((chNum) * 4u)))
/** QDMAQNUM bits Set */
#define EDMA3CC_QDMAQNUM_SET(chNum,queNum)    ((0x7u & (queNum)) << \
                                                          ((chNum) * 4u))

#define EDMA3CC_QCHMAP_PAENTRY_CLR            (~EDMA3CC_QCHMAP_PAENTRY)
/** QCHMAP-PaRAMEntry bitfield Set */
#define EDMA3CC_QCHMAP_PAENTRY_SET(paRAMId)   (((EDMA3CC_QCHMAP_PAENTRY >> \
                                             EDMA3CC_QCHMAP_PAENTRY_SHIFT) & \
                                             (paRAMId)) << \
                                             EDMA3CC_QCHMAP_PAENTRY_SHIFT)
/** QCHMAP-TrigWord bitfield Clear */
#define EDMA3CC_QCHMAP_TRWORD_CLR             (~EDMA3CC_QCHMAP_TRWORD)
/** QCHMAP-TrigWord bitfield Set */
#define EDMA3CC_QCHMAP_TRWORD_SET(paRAMId)    (((EDMA3CC_QCHMAP_TRWORD >> \
                                             EDMA3CC_QCHMAP_TRWORD_SHIFT) & \
                                             (paRAMId)) << \
                                             EDMA3CC_QCHMAP_TRWORD_SHIFT)

/** OPT-TCC bitfield Clear */
#define EDMA3CC_OPT_TCC_CLR                   (~EDMA3CC_OPT_TCC)

/** OPT-TCC bitfield Set */
#define EDMA3CC_OPT_TCC_SET(tcc)              (((EDMA3CC_OPT_TCC >> \
                                             EDMA3CC_OPT_TCC_SHIFT) & \
                                             (tcc)) << EDMA3CC_OPT_TCC_SHIFT)

#define EDMA3_SET_ALL_BITS                    (0xFFFFFFFFu)
#define EDMA3_CLR_ALL_BITS                    (0x00000000u)

#define EDMA3CC_COMPL_HANDLER_RETRY_COUNT     (10u)
#define EDMA3CC_ERR_HANDLER_RETRY_COUNT       (10u)

#define EDMA3_TRIG_MODE_MANUAL                (0u)
#define EDMA3_TRIG_MODE_QDMA                  (1u)
#define EDMA3_TRIG_MODE_EVENT                 (2u)

#define EDMA3_CHANNEL_TYPE_DMA                (0u)
#define EDMA3_CHANNEL_TYPE_QDMA               (1u)


#define EDMA3_XFER_COMPLETE                   (0u)
#define EDMA3_CC_DMA_EVT_MISS                 (1u)
#define EDMA3_CC_QDMA_EVT_MISS                (2u)

#define EDMA3_SYNC_A                          (0u)
#define EDMA3_SYNC_AB                         (1u)

#define EDMA3CC_CLR_TCCERR                     EDMA3CC_CCERRCLR_TCCERR
#define EDMA3CC_CLR_QTHRQ0                     EDMA3CC_CCERRCLR_QTHRXCD0
#define EDMA3CC_CLR_QTHRQ1                     EDMA3CC_CCERRCLR_QTHRXCD1


/* paRAMEntry Fields*/
   /**
    * The OPT field (Offset Address 0x0 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_OPT            (0x0u)

   /**
    * The SRC field (Offset Address 0x4 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_SRC            (0x1u)

   /**
    * The (ACNT+BCNT) field (Offset Address 0x8 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_ACNT_BCNT      (0x2u)

   /**
    * The DST field (Offset Address 0xC Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_DST            (0x3u)

   /**
    * The (SRCBIDX+DSTBIDX) field (Offset Address 0x10 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX   (0x4u)

   /**
    * The (LINK+BCNTRLD) field (Offset Address 0x14 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD   (0x5u)

   /**
    * The (SRCCIDX+DSTCIDX) field (Offset Address 0x18 Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX   (0x6u)

   /**
    * The (CCNT+RSVD) field (Offset Address 0x1C Bytes)
    */
#define    EDMA3CC_PARAM_ENTRY_CCNT           (0x7u)


/** The offset for each PaRAM Entry field  */
#define    EDMA3CC_PARAM_FIELD_OFFSET         (0x4u)

/** Number of PaRAM Entry fields
 * OPT, SRC, A_B_CNT, DST, SRC_DST_BIDX, LINK_BCNTRLD, SRC_DST_CIDX
 * and CCNT
 */
#define    EDMA3CC_PARAM_ENTRY_FIELDS         (0x8u)


#define    EDMA_REVID_AM335X                  (0x02u)


#define SOC_EDMA3_NUM_DMACH                 64
#define SOC_EDMA3_NUM_QDMACH                8
#define SOC_EDMA3_NUM_PARAMSETS             256
#define SOC_EDMA3_NUM_EVQUE                 4
#define SOC_EDMA3_CHMAPEXIST                0
#define SOC_EDMA3_NUM_REGIONS               8
#define SOC_EDMA3_MEMPROTECT                0


/** Number of TCCS available */
#define EDMA3_NUM_TCC                        SOC_EDMA3_NUM_DMACH

#if 0
// extern structure....
/**
* \brief EDMA3 Parameter RAM Set in User Configurable format
*
* This is a mapping of the EDMA3 PaRAM set provided to the user
* for ease of modification of the individual fields
*/
typedef struct edma3cc_param_entry {
       /** OPT field of PaRAM Set */
       unsigned int opt;

       /**
        * \brief Starting byte address of Source
        * For FIFO mode, srcAddr must be a 256-bit aligned address.
        */
       unsigned int srcAddr;

       /**
        * \brief Number of bytes in each Array (ACNT)
        */
       unsigned short aCnt;

       /**
        * \brief Number of Arrays in each Frame (BCNT)
        */
       unsigned short bCnt;

       /**
        * \brief Starting byte address of destination
        * For FIFO mode, destAddr must be a 256-bit aligned address.
        * i.e. 5 LSBs should be 0.
        */
       unsigned int destAddr;

       /**
        * \brief Index between consec. arrays of a Source Frame (SRCBIDX)
        */
       short  srcBIdx;

       /**
        * \brief Index between consec. arrays of a Destination Frame (DSTBIDX)
        */
       short  destBIdx;

       /**
        * \brief Address for linking (AutoReloading of a PaRAM Set)
        * This must point to a valid aligned 32-byte PaRAM set
        * A value of 0xFFFF means no linking
        */
       unsigned short linkAddr;

       /**
        * \brief Reload value of the numArrInFrame (BCNT)
        * Relevant only for A-sync transfers
        */
       unsigned short bCntReload;

       /**
        * \brief Index between consecutive frames of a Source Block (SRCCIDX)
        */
       short  srcCIdx;

       /**
        * \brief Index between consecutive frames of a Dest Block (DSTCIDX)
        */
       short  destCIdx;

       /**
        * \brief Number of Frames in a block (CCNT)
        */
       unsigned short cCnt;

       /**
        * \brief  This field is Reserved. Write zero to this field.
        */
       unsigned short rsvd;

};
#endif

/*
** Structure to store the EDMA context
*/
struct edma_context {
   // Channel mapping reg Val
   uint32_t dchMap[64];
   // DMA Queue Number Register Val
   uint32_t dmaQNum[8];

   // DMA Region Access Enable Register val
   uint32_t regAccEnableLow;
   uint32_t regAccEnableHigh;

   // Event Set Register value
   uint32_t eventSetRegLow;
   uint32_t eventSetRegHigh;

   // Enable Event Set Register value
   uint32_t enableEvtSetRegLow;
   uint32_t enableEvtSetRegHigh;

   // Interrupt Enable Set Register value
   uint32_t intEnableSetRegLow;
   uint32_t intEnableSetRegHigh;

   struct edma3cc_param_entry dmaParEntry[512];
};


/****************************************************************************
**                 API FUNCTION PROTOTYPES
****************************************************************************/

void am335x_edma3_clear_missed_event (enum am335x_edma3_controller controller, uint32_t chNum);
uint32_t am335x_edma_get_periph_id(enum am335x_edma3_controller ctl);
void am335x_edma_channel2shadow_enable(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum);
void am335x_edma_channel2shadow_disable(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum);
void am335x_edma_channel2param(enum am335x_edma3_controller controller, uint32_t channel, uint32_t paramSet);
void am335x_edma_channel2eventQ_map(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum, uint32_t evtQNum);
void am335x_edma_channel2eventQ_demap(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum);
void am335x_edma_qdma_channel_2_param(enum am335x_edma3_controller controller, uint32_t chNum, uint32_t *paRAMId);
void am335x_edma_set_triggword_2_qdma(enum am335x_edma3_controller controller, uint32_t chNum, uint32_t trigWord);
void am335x_edma3_qdma_clear_missed_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_clear_ccerr(enum am335x_edma3_controller ctl, uint32_t Flags);
void am335x_edma_set_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_clear_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_enable_dma_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_disable_dma_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_enable_qdma_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_disable_qdma_event(enum am335x_edma3_controller controller, uint32_t chNum);
uint32_t am335x_edma_get_ccerr_status(enum am335x_edma3_controller controller);
uint32_t am335x_edma_get_interrupt_status(enum am335x_edma3_controller controller);
uint32_t am335_edma_get_interrupt_status_high(enum am335x_edma3_controller controller);
uint32_t am335x_edma_get_err_interrupt_status(enum am335x_edma3_controller controller);
uint32_t am335x_edma_get_err_interr_status_high(enum am335x_edma3_controller controller);
uint32_t am335x_edma_get_qedma_err_interrupt_status(enum am335x_edma3_controller controller);
void am335x_edma_enable_interrupt_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_disable_interrupt_event(enum am335x_edma3_controller controller, uint32_t chNum);
void am335x_edma_clear_interrupt(enum am335x_edma3_controller controller, uint32_t value);
void am335x_edma_get_param(enum am335x_edma3_controller controller, uint32_t chNum, struct edma3cc_param_entry* currPaRAM);
void am335x_edma_get_qdma_param(enum am335x_edma3_controller controller, uint32_t paRAMId, struct edma3cc_param_entry* currPaRAM);
void am335x_edma_set_qdma_param(enum am335x_edma3_controller controller, uint32_t paRAMId, struct edma3cc_param_entry* newPaRAM);
void am335x_edma_set_qdma_param_entry(enum am335x_edma3_controller controller, uint32_t paRAMId, uint32_t paRAMEntry, uint32_t newPaRAMEntryVal);
uint32_t am335x_edma_get_qdma_param_entry(enum am335x_edma3_controller controller, uint32_t paRAMId, uint32_t paRAMEntry);
uint32_t am335x_edma_request_channel(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum, uint32_t tccNum, uint32_t evtQNum);
uint32_t am335x_edma_free_channel(enum am335x_edma3_controller controller, uint32_t chType, uint32_t chNum, uint32_t trigMode, uint32_t tccNum);
uint32_t am335x_edma_disable_transfer(enum am335x_edma3_controller controller, uint32_t chNum, uint32_t trigMode);
void am335x_edma_clear_err_bits(enum am335x_edma3_controller controller, uint32_t chNum, uint32_t evtQNum);
void am335x_edma_eval_error(enum am335x_edma3_controller controllerr);
void am335x_edma_deinit(enum am335x_edma3_controller controller);
void am335x_edma_save_context(enum am335x_edma3_controller controllerr, struct edma_context *edmaCntxPtr);
void am335x_edma_restore_context(enum am335x_edma3_controller controllerr, struct edma_context *edmaCntxPtr);

#endif
