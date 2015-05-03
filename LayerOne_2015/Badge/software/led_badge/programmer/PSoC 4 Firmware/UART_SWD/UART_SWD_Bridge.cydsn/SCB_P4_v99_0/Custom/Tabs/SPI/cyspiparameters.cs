/*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using CyDesigner.Extensions.Gde;
using CyDesigner.Extensions.Common;
using System.ComponentModel;
using System.Reflection;
using System.Windows.Forms;

namespace SCB_P4_v99_0
{
    #region Component Parameters Ranges
    public partial class CyParamRanges
    {
        public const int SPI_MAX_TRIGGER_LEVEL = 8;

        public const UInt32 SPI_RX_BUFFER_MIN_SIZE = 8;
        public const UInt32 SPI_TX_BUFFER_MIN_SIZE = 8;
    }
    #endregion

    #region Component Parameters Names
    public partial class CyParamNames
    {
        public const string SPI_BITS_ORDER = "SpiBitsOrder";
        public const string SPI_BIT_RATE = "SpiBitRate";
        public const string SPI_ENABLE_MEDIAN_FILTER = "SpiMedianFilterEnable";
        public const string SPI_ENABLE_LATE_MISO_SAMPLE = "SpiLateMisoSampleEnable";
        public const string SPI_ENABLE_WAKEUP_FROM_SLEEP_MODE = "SpiWakeEnable";
        public const string SPI_CLOCK_FROM_TERM = "SpiClockFromTerm";

        public const string SPI_INTERRUPT_MODE = "SpiInterruptMode";
        public const string SPI_INTERRUPT_ON_RX_FULL = "SpiIntrRxFull";
        public const string SPI_INTERRUPT_ON_RX_NOT_EMPTY = "SpiIntrRxNotEmpty";
        public const string SPI_INTERRUPT_ON_RX_OVER_FLOW = "SpiIntrRxOverflow";

        public const string SPI_INTERRUPT_ON_RX_TRIGGER = "SpiIntrRxTrigger";
        public const string SPI_INTERRUPT_ON_RX_UNDER_FLOW = "SpiIntrRxUnderflow";
        public const string SPI_INTERRUPT_ON_SPI_DONE = "SpiIntrMasterSpiDone";
        public const string SPI_INTERRUPT_ON_SPI_BUS_ERROR = "SpiIntrSlaveBusError";
        public const string SPI_INTERRUPT_ON_TX_EMPTY = "SpiIntrTxEmpty";

        public const string SPI_INTERRUPT_ON_TX_NOT_FULL = "SpiIntrTxNotFull";

        public const string SPI_INTERRUPT_ON_TX_OVER_FLOW = "SpiIntrTxOverflow";
        public const string SPI_INTERRUPT_ON_TX_TRIGGER = "SpiIntrTxTrigger";
        public const string SPI_INTERRUPT_ON_TX_UNDER_FLOW = "SpiIntrTxUnderflow";
        public const string SPI_NUMBER_OF_RX_DATA_BITS = "SpiNumberOfRxDataBits";
        public const string SPI_NUMBER_OF_SS_LINES = "SpiNumberOfSelectLines";
        public const string SPI_NUMBER_OF_TX_DATA_BITS = "SpiNumberOfTxDataBits";
        public const string SPI_OVS_FACTOR = "SpiOvsFactor";
        public const string SPI_RX_BUFFER_SIZE = "SpiRxBufferSize";
        public const string SPI_RX_BUFFER_TRIGGER_LEVEL = "SpiRxTriggerLevel";
        public const string SPI_MODE = "SpiMode";
        public const string SPI_SCLK_MODE = "SpiSclkMode";
        public const string SPI_SUB_MODE = "SpiSubMode";
        public const string SPI_TRANSFER_SEPARATION = "SpiTransferSeparation";
        public const string SPI_TX_BUFFER_SIZE = "SpiTxBufferSize";
        public const string SPI_TX_BUFFER_TRIGGER_LEVEL = "SpiTxTriggerLevel";
    }
    #endregion

    #region Enums
    public enum CyESPISCLKModeType
    {
        [Description("CPHA = 0, CPOL = 0")]
        MODE_00 = 0,
        [Description("CPHA = 0, CPOL = 1")]
        MODE_01 = 2,
        [Description("CPHA = 1, CPOL = 0")]
        MODE_10 = 1,
        [Description("CPHA = 1, CPOL = 1")]
        MODE_11 = 3
    }

    public enum CyESPIModeType
    {
        [Description("Slave")]
        SPI_SLAVE,
        [Description("Master")]
        SPI_MASTER
    }

    public enum CyESPISubModeType
    {
        [Description("National Semiconductor")]
        NS = 2,
        [Description("Motorola")]
        MOTOROLA = 0,
        [Description("TI (Start coincides)")]
        TI_COINCIDES = 1,
        [Description("TI (Start precedes)")]
        TI_PRECEDES = 17
    }
    public enum CyESPITransferSeparationType { SEPARATED, CONTINUOUS }

    public enum CyESPIBitOrderType
    {
        [Description("MSB first")]
        MSB_FIRST = 1,
        [Description("LSB first")]
        LSB_FIRST = 0
    }
    #endregion

    public partial class CyParameters
    {
        #region fields
        public static ushort[] SPI_DATA_RATE_LIST = new ushort[]
        {
           500,
           1000, 
           2000,
           4000,
           6000,
           8000
        };


        public const uint SPI_MIN_TX_BUFFER_SIZE = 8;
        public const uint SPI_MAX_SS_NUMBER = 4;

        public const uint SPI_MIN_DATA_BITS = 4;
        public const uint SPI_MAX_DATA_BITS = 16;

        public const UInt16 SPI_MIN_DATA_RATE = 1;
        public const UInt16 SPI_MAX_DATA_RATE = 8000;

        // Unit constants
        public const UInt16 POW6 = 1000;

        public const uint SPI_MAX_OVER_SAMPLING_FACTOR = 16;

        #endregion

        #region Properties
        public bool SPI_ClockFromTerm
        {
            get
            {
                return GetValue<bool>(CyParamNames.SPI_CLOCK_FROM_TERM);
            }
            set
            {
                SetValue(CyParamNames.SPI_CLOCK_FROM_TERM, value);
            }
        }

        public UInt16? SPI_BitsRateInKHz
        {
            get { return GetValue<UInt16>(CyParamNames.SPI_BIT_RATE); }
            set
            {
                SetValue(CyParamNames.SPI_BIT_RATE, value);
            }
        }

        public CyEInterruptModeType SPI_InterruptMode
        {
            get { return GetValue<CyEInterruptModeType>(CyParamNames.SPI_INTERRUPT_MODE); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_MODE, value); }
        }

        public bool SPI_EnableMedianFilter
        {
            get { return GetValue<bool>(CyParamNames.SPI_ENABLE_MEDIAN_FILTER); }
            set { SetValue(CyParamNames.SPI_ENABLE_MEDIAN_FILTER, value); }
        }

        public bool SPI_EnableLateMISOSample
        {
            get { return GetValue<bool>(CyParamNames.SPI_ENABLE_LATE_MISO_SAMPLE); }
            set { SetValue(CyParamNames.SPI_ENABLE_LATE_MISO_SAMPLE, value); }
        }

        public bool SPI_EnableWakeupFromSleepMode
        {
            get { return GetValue<bool>(CyParamNames.SPI_ENABLE_WAKEUP_FROM_SLEEP_MODE); }
            set { SetValue(CyParamNames.SPI_ENABLE_WAKEUP_FROM_SLEEP_MODE, value); }
        }

        public bool SPI_InterruptOnRXFull
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_RX_FULL); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_RX_FULL, value); }
        }
        public bool SPI_InterruptOnRXNotEmpty
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_RX_NOT_EMPTY); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_RX_NOT_EMPTY, value); }
        }
        public bool SPI_InterruptOnRXOverflow
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_RX_OVER_FLOW); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_RX_OVER_FLOW, value); }
        }
        public bool SPI_InterruptOnRXTrigger
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_RX_TRIGGER); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_RX_TRIGGER, value); }
        }
        public bool SPI_InterruptOnRXUnderflow
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_RX_UNDER_FLOW); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_RX_UNDER_FLOW, value); }
        }
        public bool SPI_InterruptOnSPIDone
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_SPI_DONE); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_SPI_DONE, value); }
        }
        public bool SPI_InterruptOnSPIBusError
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_SPI_BUS_ERROR); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_SPI_BUS_ERROR, value); }
        }
        public bool SPI_InterruptOnTXEmpty
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_TX_EMPTY); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_TX_EMPTY, value); }
        }
        public bool SPI_InterruptOnTXNotFull
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_TX_NOT_FULL); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_TX_NOT_FULL, value); }
        }
        public bool SPI_InterruptOnTXOverflow
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_TX_OVER_FLOW); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_TX_OVER_FLOW, value); }
        }
        public bool SPI_InterruptOnTXTrigger
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_TX_TRIGGER); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_TX_TRIGGER, value); }
        }
        public bool SPI_InterruptOnTXUnderflow
        {
            get { return GetValue<bool>(CyParamNames.SPI_INTERRUPT_ON_TX_UNDER_FLOW); }
            set { SetValue(CyParamNames.SPI_INTERRUPT_ON_TX_UNDER_FLOW, value); }
        }
        public byte? SPI_NumberOfRXDataBits
        {
            get { return GetValue<byte>(CyParamNames.SPI_NUMBER_OF_RX_DATA_BITS); }
            set { SetValue(CyParamNames.SPI_NUMBER_OF_RX_DATA_BITS, value); }
        }
        public byte SPI_NumberOfSSLines
        {
            get { return GetValue<byte>(CyParamNames.SPI_NUMBER_OF_SS_LINES); }
            set { SetValue(CyParamNames.SPI_NUMBER_OF_SS_LINES, value); }
        }
        public byte? SPI_NumberOfTXDataBits
        {
            get { return GetValue<byte>(CyParamNames.SPI_NUMBER_OF_TX_DATA_BITS); }
            set { SetValue(CyParamNames.SPI_NUMBER_OF_TX_DATA_BITS, value); }
        }
        public byte? SPI_OvsFactor
        {
            get { return GetValue<byte>(CyParamNames.SPI_OVS_FACTOR); }
            set { SetValue(CyParamNames.SPI_OVS_FACTOR, value); }
        }
        public UInt32 SPI_RxBufferSize
        {
            get { return GetValue<UInt32>(CyParamNames.SPI_RX_BUFFER_SIZE); }
            set { SetValue(CyParamNames.SPI_RX_BUFFER_SIZE, value); }
        }
        public byte SPI_RxBufferTriggerLevel
        {
            get { return GetValue<byte>(CyParamNames.SPI_RX_BUFFER_TRIGGER_LEVEL); }
            set { SetValue(CyParamNames.SPI_RX_BUFFER_TRIGGER_LEVEL, value); }
        }
        public CyESPITransferSeparationType SPI_TransferSeparation
        {
            get { return GetValue<CyESPITransferSeparationType>(CyParamNames.SPI_TRANSFER_SEPARATION); }
            set { SetValue(CyParamNames.SPI_TRANSFER_SEPARATION, value); }
        }
        public UInt32 SPI_TxBufferSize
        {
            get { return GetValue<UInt32>(CyParamNames.SPI_TX_BUFFER_SIZE); }
            set { SetValue(CyParamNames.SPI_TX_BUFFER_SIZE, value); }
        }
        public byte SPI_TxBufferTriggerLevel
        {
            get { return GetValue<byte>(CyParamNames.SPI_TX_BUFFER_TRIGGER_LEVEL); }
            set { SetValue(CyParamNames.SPI_TX_BUFFER_TRIGGER_LEVEL, value); }
        }
        #endregion

        #region Functions
        public uint GetMinOversampling(ref string message)
        {
            if ((SPI_EnableMedianFilter == false) && (SPI_EnableLateMISOSample == false))
            {
                message = Resources.ErrorMsgMedDisMisoDis;
                return 6;
            }
            else if ((SPI_EnableMedianFilter == false) && (SPI_EnableLateMISOSample))
            {
                message = Resources.ErrorMsgMedDisMiso;
                return 3;
            }
            else if ((SPI_EnableMedianFilter) && (SPI_EnableLateMISOSample == false))
            {
                message = Resources.ErrorMsgMedMisoDis;
                return 8;
            }

            //if EnableMedianFilter && EnableLateMISOSample
            message = Resources.ErrorMsgMedMiso;
            return 4;
        }

        public static string GetTextFromNullableValue<T>(T val)
        {
            if (val == null)
                return string.Empty;
            return val.ToString();
        }

        #endregion
    }
}
