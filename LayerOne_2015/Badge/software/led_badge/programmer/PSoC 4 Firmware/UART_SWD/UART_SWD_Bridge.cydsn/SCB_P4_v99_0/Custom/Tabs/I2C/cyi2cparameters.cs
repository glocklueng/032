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
        public const string I2C_ACCEPT_ADDRESS = "I2cAcceptAddress";
        public const string I2C_DATA_RATE = "I2cDataRate";
        public const string I2C_ENABLE_MEDIAN_FILTER = "I2cMedianFilterEnable";
        public const string I2C_ENABLE_WAKEUP = "I2cWakeEnable";
        public const string I2C_MODE = "I2cMode";
        public const string I2C_OVS_FACTOR = "I2cOvsFactor";
        public const string I2C_OVS_FACTOR_HIGH = "I2cOvsFactorHigh";
        public const string I2C_OVS_FACTOR_LOW = "I2cOvsFactorLow";
        public const string I2C_SLAVE_ADDRESS = "I2cSlaveAddress";
        public const string I2C_SLAVE_ADDRESS_MASK = "I2cSlaveAddressMask";

        public const string I2C_CLOCK_FROM_TERM = "I2cClockFromTerm";

        // Parameters for internal use
        public const string I2C_IS_SLAVE_ADDRESS_HEX = "I2cIsSlaveAddressMaskHex";
        public const string I2C_IS_SLAVE_ADDRESS_MASK_HEX = "I2cIsSlaveAddressMaskHex";
    }
    #endregion

    #region Component Parameters Ranges
    public partial class CyParamRanges
    {
        public const UInt16 I2C_DATA_RATE_MIN = 1;
        public const UInt16 I2C_DATA_RATE_MAX = 1000;
        public const UInt16 I2C_DATA_RATE_NON_ClOCK_STRETCHING_MAX = 400;

        private const byte I2C_OVS_FACTOR_MEDFILTER_MIN = 14;
        private const byte I2C_OVS_FACTOR_NOMEDFILTER_MIN = 12;
        public const byte I2C_OVS_FACTOR_MAX = 32;

        private const byte I2C_OVS_LOW_MEDFILTER_MIN = 8;
        private const byte I2C_OVS_HIGH_MEDFILTER_MIN = 6;
        private const byte I2C_OVS_LOW_NOMEDFILTER_MIN = 7;
        private const byte I2C_OVS_HIGH_NOMEDFILTER_MIN = 5;
        public const byte I2C_OVS_LOW_HIGH_MAX = 16;

        public const byte I2C_SLAVE_ADDRESS_MIN = 8;
        public const byte I2C_SLAVE_ADDRESS_MAX = 120;

        public const byte I2C_SLAVE_ADDRESS_MASK_MIN = 0;
        public const byte I2C_SLAVE_ADDRESS_MASK_MAX = 254;

        // I2C spec characteristics fSCL
        public const int I2C_SCL_FREQ_STANDARD_MODE_MAX = 100;
        public const int I2C_SCL_FREQ_FAST_MODE_MAX = 400;
        public const int I2C_SCL_FREQ_FAST_MODE_PLUS_MAX = 1000;
        // I2C spec characteristics tLOW and tHIGH
        public const double I2C_TLO_STANDARD_MODE_MIN = 4.7;
        public const double I2C_THI_STANDARD_MODE_MIN = 4.0;
        public const double I2C_TLO_FAST_MODE_MIN = 1.3;
        public const double I2C_THI_FAST_MODE_MIN = 0.6;
        public const double I2C_TLO_FAST_MODE_PLUS_MIN = 0.5;
        public const double I2C_THI_FAST_MODE_PLUS_MIN = 0.26;

        public const double I2C_PSVP_CLOCK_FREQ_KHZ = 38400;

        public static byte GetI2COvsFactorMinimum(bool medianFilterEnabled)
        {
            return medianFilterEnabled ? CyParamRanges.I2C_OVS_FACTOR_MEDFILTER_MIN :
                CyParamRanges.I2C_OVS_FACTOR_NOMEDFILTER_MIN;
        }

        public static byte GetI2COvsLowMinimum(bool medianFilterEnabled)
        {
            return medianFilterEnabled ? CyParamRanges.I2C_OVS_LOW_MEDFILTER_MIN :
                CyParamRanges.I2C_OVS_LOW_NOMEDFILTER_MIN;
        }

        public static byte GetI2COvsHighMinimum(bool medianFilterEnabled)
        {
            return medianFilterEnabled ? CyParamRanges.I2C_OVS_HIGH_MEDFILTER_MIN :
                CyParamRanges.I2C_OVS_HIGH_NOMEDFILTER_MIN;
        }
    }
    #endregion

    #region Component Enums
    public enum CyEI2CModeType { I2C_SLAVE = 1, I2C_MASTER = 2, I2C_MULTI_MASTER = 6, I2C_MULTI_MASTER_SLAVE = 7 }
    public enum CyEI2CPostWakeupOperationType { ADDRESS_ACK, ADDRESS_NACK }
    #endregion

    public partial class CyParameters
    {
        public UInt16 I2C_DataRate
        {
            get { return GetValue<UInt16>(CyParamNames.I2C_DATA_RATE); }
            set { SetValue(CyParamNames.I2C_DATA_RATE, value); }
        }

        public bool I2C_ClockFromTerm
        {
            get { return GetValue<bool>(CyParamNames.I2C_CLOCK_FROM_TERM); }
            set { SetValue(CyParamNames.I2C_CLOCK_FROM_TERM, value); }
        }

        public byte I2C_OvsFactor
        {
            get { return GetValue<byte>(CyParamNames.I2C_OVS_FACTOR); }
            set { SetValue(CyParamNames.I2C_OVS_FACTOR, value); }
        }

        public byte I2C_OvsFactorLow
        {
            get { return GetValue<byte>(CyParamNames.I2C_OVS_FACTOR_LOW); }
            set { SetValue(CyParamNames.I2C_OVS_FACTOR_LOW, value); }
        }

        public byte I2C_OvsFactorHigh
        {
            get { return GetValue<byte>(CyParamNames.I2C_OVS_FACTOR_HIGH); }
            set { SetValue(CyParamNames.I2C_OVS_FACTOR_HIGH, value); }
        }

        public bool I2C_EnableMedianFilter
        {
            get { return GetValue<bool>(CyParamNames.I2C_ENABLE_MEDIAN_FILTER); }
            set { SetValue(CyParamNames.I2C_ENABLE_MEDIAN_FILTER, value); }
        }

        public byte I2C_SlaveAddress
        {
            get { return GetValue<byte>(CyParamNames.I2C_SLAVE_ADDRESS); }
            set { SetValue(CyParamNames.I2C_SLAVE_ADDRESS, value); }
        }

        public byte I2C_SlaveAddressMask
        {
            get { return GetValue<byte>(CyParamNames.I2C_SLAVE_ADDRESS_MASK); }
            set { SetValue(CyParamNames.I2C_SLAVE_ADDRESS_MASK, value); }
        }

        public bool I2C_AcceptAddress
        {
            get { return GetValue<bool>(CyParamNames.I2C_ACCEPT_ADDRESS); }
            set { SetValue(CyParamNames.I2C_ACCEPT_ADDRESS, value); }
        }

        public bool I2C_EnableWakeup
        {
            get { return GetValue<bool>(CyParamNames.I2C_ENABLE_WAKEUP); }
            set { SetValue(CyParamNames.I2C_ENABLE_WAKEUP, value); }
        }

        // Properties for internal use
        public bool I2C_IsSlaveAddressHex
        {
            get { return GetValue<bool>(CyParamNames.I2C_IS_SLAVE_ADDRESS_HEX); }
            set { SetValue(CyParamNames.I2C_IS_SLAVE_ADDRESS_HEX, value); }
        }

        public bool I2C_IsSlaveAddressMaskHex
        {
            get { return GetValue<bool>(CyParamNames.I2C_IS_SLAVE_ADDRESS_MASK_HEX); }
            set { SetValue(CyParamNames.I2C_IS_SLAVE_ADDRESS_MASK_HEX, value); }
        }
    }
}
