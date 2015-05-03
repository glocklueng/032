/*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace SCB_P4_v99_0
{
    #region Component Parameters Names
    public partial class CyParamNames
    {
        public const string UART_CLOCK_FROM_TERM = "UartClockFromTerm";
        public const string UART_DATA_RATE = "UartDataRate";
        public const string UART_DROP_ON_FRAME_ERR = "UartDropOnFrameErr";
        public const string UART_DROP_ON_PARITY_ERR = "UartDropOnParityErr";
        public const string UART_HFCLK_KHZ = "UartHFclkKhz";
        public const string UART_WAKEUP_FROM_SLEEP_ENABLED = "UartWakeEnable";
        public const string UART_LOW_POWER = "UartIrdaLowPower";
        public const string UART_INTERRUPT_MODE = "UartInterruptMode";
        public const string UART_INTR_RX_FRAME_ERR = "UartIntrRxFrameErr";
        public const string UART_INTR_RX_FULL = "UartIntrRxFull";
        public const string UART_INTR_RX_NOT_EMPTY = "UartIntrRxNotEmpty";
        public const string UART_INTR_RX_OVERFLOW = "UartIntrRxOverflow";
        public const string UART_INTR_RX_PARITY_ERR = "UartIntrRxParityErr";
        public const string UART_INTR_RX_TRIGGER = "UartIntrRxTrigger";
        public const string UART_INTR_RX_UNDERFLOW = "UartIntrRxUnderflow";
        public const string UART_INTR_TX_EMPTY = "UartIntrTxEmpty";
        public const string UART_INTR_TX_NOT_FULL = "UartIntrTxNotFull";
        public const string UART_INTR_TX_OVERFLOW = "UartIntrTxOverflow";
        public const string UART_INTR_TX_TRIGGER = "UartIntrTxTrigger";
        public const string UART_INTR_TX_UART_DONE = "UartIntrTxUartDone";
        public const string UART_INTR_TX_UART_LOST_ARB = "UartIntrTxUartLostArb";
        public const string UART_INTR_TX_UART_NACK = "UartIntrTxUartNack";
        public const string UART_INTR_TX_UNDERFLOW = "UartIntrTxUnderflow";
        public const string UART_IRDA_POLARITY = "UartIrdaPolarity";
        public const string UART_MEDIAN_FILTER_ENABLED = "UartMedianFilterEnable";
        public const string UART_MP_ENABLED = "UartMpEnable";
        public const string UART_DROP_ON_PARITY_ERROR = "UartDropOnParityErr";
        public const string UART_DROP_ON_FRAME_ERROR = "UartDropOnFrameErr";
        public const string UART_MP_RX_ACCEPT_ADDRESS = "UartMpRxAcceptAddress";
        public const string UART_MP_RX_ADDRESS = "UartMpRxAddress";
        public const string UART_MP_RX_ADDRESS_MASK = "UartMpRxAddressMask";
        public const string UART_NUMBER_OF_DATA_BITS = "UartNumberOfDataBits";
        public const string UART_NUMBER_OF_STOP_BITS = "UartNumberOfStopBits";
        public const string UART_OVS_FACTOR = "UartOvsFactor";
        public const string UART_PARITY_TYPE = "UartParityType";
        public const string UART_RX_BUFFER_SIZE = "UartRxBufferSize";
        public const string UART_RX_ENABLED = "UartRxEnable";
        public const string UART_RX_TRIGGER_LEVEL = "UartRxTriggerLevel";
        public const string UART_SM_CARD_RETRY_ON_NACK = "UartSmCardRetryOnNack";
        public const string UART_SUB_MODE = "UartSubMode";
        public const string UART_DIRECTION = "UartDirection";
        public const string UART_TX_BUFFER_SIZE = "UartTxBufferSize";
        public const string UART_TX_ENABLED = "UartTxEnable";
        public const string UART_TX_TRIGGER_LEVEL = "UartTxTriggerLevel";
    }
    #endregion

    #region Component Parameters Ranges
    public partial class CyParamRanges
    {
        public const byte UART_OVS_FACTOR_MIN = 8;
        public const byte UART_OVS_FACTOR_MAX = 16;

        public const int UART_MAX_TRIGGER_LEVEL = 8;

        public const UInt32 UART_RX_BUFFER_MIN_SIZE = 8;
        public const UInt32 UART_RX_BUFFER_MAX_SIZE = UInt32.MaxValue;
        public const UInt32 UART_TX_BUFFER_MIN_SIZE = 8;
        public const UInt32 UART_TX_BUFFER_MAX_SIZE = UInt32.MaxValue;

        public const UInt32 UART_DATA_RATE_MIN_VALUE = 1;
        public const UInt32 UART_DATA_RATE_MAX_VALUE = 1000000;
    }
    #endregion

    #region Component Enums
    public enum CyEUARTSubModeType { STANDARD, SMART_CARD, IRDA }
    public enum CyEUARTDirectionType { TX_RX = 3, RX = 1, TX = 2 }
    public enum CyEUARTNumberDataBitsType { FIVE = 5, SIX, SEVEN, EIGHT, NINE }
    public enum CyEUARTParityType { EVEN, ODD, PARITY_NONE }
    public enum CyEUARTPolarityType { NON_INVERTING, INVERTING }
    public enum CyEUARTNumberStopBitsType { ONE_H_BITS = 3, TWO_BITS = 4, ONE_BIT = 2 }
    #endregion

    public partial class CyParameters
    {
        #region Class properties
        public UInt32 UART_DataRate
        {
            get { return GetValue<UInt32>(CyParamNames.UART_DATA_RATE); }
            set { SetValue(CyParamNames.UART_DATA_RATE, value); }
        }

        public bool UART_ClockFromTerm
        {
            get
            {
                return GetValue<bool>(CyParamNames.UART_CLOCK_FROM_TERM);
            }
            set
            {
                SetValue(CyParamNames.UART_CLOCK_FROM_TERM, value);
            }
        }

        public UInt16 UART_OversamplingFactor
        {
            get { return GetValue<UInt16>(CyParamNames.UART_OVS_FACTOR); }
            set { SetValue(CyParamNames.UART_OVS_FACTOR, value); }
        }

        public bool UART_MedianFilterEnabled
        {
            get { return GetValue<bool>(CyParamNames.UART_MEDIAN_FILTER_ENABLED); }
            set { SetValue(CyParamNames.UART_MEDIAN_FILTER_ENABLED, value); }
        }

        public bool UART_RetryOnNACK
        {
            get { return GetValue<bool>(CyParamNames.UART_SM_CARD_RETRY_ON_NACK); }
            set { SetValue(CyParamNames.UART_SM_CARD_RETRY_ON_NACK, value); }
        }

        public bool UART_InvertRXProperty
        {
            get
            {
                return GetValue<CyEUARTPolarityType>(CyParamNames.UART_IRDA_POLARITY) == CyEUARTPolarityType.INVERTING;
            }
            set
            {
                SetValue(CyParamNames.UART_IRDA_POLARITY,
                    value ? CyEUARTPolarityType.INVERTING : CyEUARTPolarityType.NON_INVERTING);
            }
        }

        public bool UART_WakeupFromSleepEnabled
        {
            get { return GetValue<bool>(CyParamNames.UART_WAKEUP_FROM_SLEEP_ENABLED); }
            set { SetValue(CyParamNames.UART_WAKEUP_FROM_SLEEP_ENABLED, value); }
        }

        public bool UART_LowPowerReceive
        {
            get { return GetValue<bool>(CyParamNames.UART_LOW_POWER); }
            set { SetValue(CyParamNames.UART_LOW_POWER, value); }
        }

        public UInt32 UART_RXBufferSize
        {
            get { return GetValue<UInt32>(CyParamNames.UART_RX_BUFFER_SIZE); }
            set { SetValue(CyParamNames.UART_RX_BUFFER_SIZE, value); }
        }

        public UInt32 UART_TXBufferSize
        {
            get { return GetValue<UInt32>(CyParamNames.UART_TX_BUFFER_SIZE); }
            set { SetValue(CyParamNames.UART_TX_BUFFER_SIZE, value); }
        }

        public CyEInterruptModeType UART_InterruptMode
        {
            get { return GetValue<CyEInterruptModeType>(CyParamNames.UART_INTERRUPT_MODE); }
            set { SetValue(CyParamNames.UART_INTERRUPT_MODE, value); }
        }

        public bool UART_InterruptOnUARTDone
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_UART_DONE); }
            set { SetValue(CyParamNames.UART_INTR_TX_UART_DONE, value); }
        }

        public bool UART_InterruptOnTXFIFONotFull
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_NOT_FULL); }
            set { SetValue(CyParamNames.UART_INTR_TX_NOT_FULL, value); }
        }

        public bool UART_InterruptOnTXFIFOEmpty
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_EMPTY); }
            set { SetValue(CyParamNames.UART_INTR_TX_EMPTY, value); }
        }

        public bool UART_InterruptOnTXFIFOOverflow
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_OVERFLOW); }
            set { SetValue(CyParamNames.UART_INTR_TX_OVERFLOW, value); }
        }

        public bool UART_InterruptOnTXFIFOUnderflow
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_UNDERFLOW); }
            set { SetValue(CyParamNames.UART_INTR_TX_UNDERFLOW, value); }
        }

        public bool UART_InterruptOnTXLostArbitration
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_UART_LOST_ARB); }
            set { SetValue(CyParamNames.UART_INTR_TX_UART_LOST_ARB, value); }
        }

        public bool UART_InterruptOnTXNACK
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_UART_NACK); }
            set { SetValue(CyParamNames.UART_INTR_TX_UART_NACK, value); }
        }

        public bool UART_InterruptOnTXFIFOTrigger
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_TX_TRIGGER); }
            set { SetValue(CyParamNames.UART_INTR_TX_TRIGGER, value); }
        }

        public byte UART_TXTriggerLevel
        {
            get { return GetValue<byte>(CyParamNames.UART_TX_TRIGGER_LEVEL); }
            set { SetValue(CyParamNames.UART_TX_TRIGGER_LEVEL, value); }
        }

        public bool UART_InterruptOnRXFIFOFull
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_FULL); }
            set { SetValue(CyParamNames.UART_INTR_RX_FULL, value); }
        }

        public bool UART_InterruptOnRXFIFONotEmpty
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_NOT_EMPTY); }
            set { SetValue(CyParamNames.UART_INTR_RX_NOT_EMPTY, value); }
        }

        public bool UART_InterruptOnRXFIFOOverflow
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_OVERFLOW); }
            set { SetValue(CyParamNames.UART_INTR_RX_OVERFLOW, value); }
        }

        public bool UART_InterruptOnRXFIFOUnderflow
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_UNDERFLOW); }
            set { SetValue(CyParamNames.UART_INTR_RX_UNDERFLOW, value); }
        }

        public bool UART_InterruptOnRXFrameError
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_FRAME_ERR); }
            set { SetValue(CyParamNames.UART_INTR_RX_FRAME_ERR, value); }
        }

        public bool UART_InterruptOnRXParityError
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_PARITY_ERR); }
            set { SetValue(CyParamNames.UART_INTR_RX_PARITY_ERR, value); }
        }

        public bool UART_InterruptOnRXFIFOTrigger
        {
            get { return GetValue<bool>(CyParamNames.UART_INTR_RX_TRIGGER); }
            set { SetValue(CyParamNames.UART_INTR_RX_TRIGGER, value); }
        }

        public byte UART_RXTriggerLevel
        {
            get { return GetValue<byte>(CyParamNames.UART_RX_TRIGGER_LEVEL); }
            set { SetValue(CyParamNames.UART_RX_TRIGGER_LEVEL, value); }
        }

        public bool UART_MultiProcessorModeEnabled
        {
            get { return GetValue<bool>(CyParamNames.UART_MP_ENABLED); }
            set { SetValue(CyParamNames.UART_MP_ENABLED, value); }
        }

        public bool UART_RXFIFODropOnParityError
        {
            get { return GetValue<bool>(CyParamNames.UART_DROP_ON_PARITY_ERROR); }
            set { SetValue(CyParamNames.UART_DROP_ON_PARITY_ERROR, value); }
        }

        public bool UART_RXFIFODropOnFrameError
        {
            get { return GetValue<bool>(CyParamNames.UART_DROP_ON_FRAME_ERROR); }
            set { SetValue(CyParamNames.UART_DROP_ON_FRAME_ERROR, value); }
        }

        public byte UART_MultiProcessorAddress
        {
            get { return GetValue<byte>(CyParamNames.UART_MP_RX_ADDRESS); }
            set { SetValue(CyParamNames.UART_MP_RX_ADDRESS, value); }
        }

        public bool UART_MultiProcessorAcceptAddress
        {
            get { return GetValue<bool>(CyParamNames.UART_MP_RX_ACCEPT_ADDRESS); }
            set { SetValue(CyParamNames.UART_MP_RX_ACCEPT_ADDRESS, value); }
        }

        public byte UART_MultiProcessorAddressMask
        {
            get { return GetValue<byte>(CyParamNames.UART_MP_RX_ADDRESS_MASK); }
            set { SetValue(CyParamNames.UART_MP_RX_ADDRESS_MASK, value); }
        }
        #endregion
    }
}
