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
using System.ComponentModel;

namespace SCB_P4_v99_0
{
    #region Component Parameters Names
    public partial class CyParamNames
    {
        public const string EZI2C_CLOCK_FROM_TERMINAL = "EzI2cClockFromTerm";
        public const string EZI2C_CLOCK_STRETCHING = "EzI2cClockStretching";
        public const string EZI2C_DATA_RATE = "EzI2cDataRate";
        public const string EZI2C_IS_PRIMARY_SLAVE_ADDRESS_HEX = "EzI2cIsPrimarySlaveAddressHex";
        public const string EZI2C_IS_SECONDRY_SLAVE_ADDRESS_HEX = "EzI2cIsSecondarySlaveAddressHex";
        public const string EZI2C_ENABLE_MEDIAN_FILTER = "EzI2cMedianFilterEnable";
        public const string EZI2C_OVERSAMPLING = "EzI2cOvsFactor";
        public const string EZI2C_PRIMARY_SLAVE_ADDRESS = "EzI2cPrimarySlaveAddress";
        public const string EZI2C_SECONDARY_SLAVE_ADDRESS = "EzI2cSecondarySlaveAddress";
        public const string EZI2C_SUBADDRESS_SIZE = "EzI2cSubAddressSize";
        public const string EZI2C_ENABLE_WAKEUP = "EzI2cWakeEnable";
        public const string EZI2C_NUMBER_OF_ADDRESSES = "EzI2cNumberOfAddresses";
    }
    #endregion

    #region Component Parameters Ranges
    public partial class CyParamRanges
    {
    }
    #endregion

    #region Component Enums
    public enum CyEEZI2CSubAddressSizeType { SUB_ADDR8, SUB_ADDR16 }
    public enum CyEEZI2CNumberOfAddressesType { ONE_ADDRESS = 0, TWO_ADDRESSES = 1 }
    #endregion

    public partial class CyParameters
    {
        #region Class properties
        public bool EZI2C_ClockFromTerminal
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_CLOCK_FROM_TERMINAL); }
            set { SetValue(CyParamNames.EZI2C_CLOCK_FROM_TERMINAL, value); }
        }

        public bool EZI2C_StretchClock
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_CLOCK_STRETCHING); }
            set { SetValue(CyParamNames.EZI2C_CLOCK_STRETCHING, value); }
        }

        public UInt16 EZI2C_DataRate
        {
            get { return GetValue<UInt16>(CyParamNames.EZI2C_DATA_RATE); }
            set { SetValue(CyParamNames.EZI2C_DATA_RATE, value); }
        }

        public bool EZI2C_IsPrimarySlaveAddressHex
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_IS_PRIMARY_SLAVE_ADDRESS_HEX); }
            set { SetValue(CyParamNames.EZI2C_IS_PRIMARY_SLAVE_ADDRESS_HEX, value); }
        }

        public bool EZI2C_IsSecondarySlaveAddressHex
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_IS_SECONDRY_SLAVE_ADDRESS_HEX); }
            set { SetValue(CyParamNames.EZI2C_IS_SECONDRY_SLAVE_ADDRESS_HEX, value); }
        }

        public bool EZI2C_EnableMedianFilter
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_ENABLE_MEDIAN_FILTER); }
            set { SetValue(CyParamNames.EZI2C_ENABLE_MEDIAN_FILTER, value); }
        }

        public byte EZI2C_OversamplingFactor
        {
            get { return GetValue<byte>(CyParamNames.EZI2C_OVERSAMPLING); }
            set { SetValue(CyParamNames.EZI2C_OVERSAMPLING, value); }
        }

        public byte EZI2C_PrimarySlaveAddress
        {
            get { return GetValue<byte>(CyParamNames.EZI2C_PRIMARY_SLAVE_ADDRESS); }
            set { SetValue(CyParamNames.EZI2C_PRIMARY_SLAVE_ADDRESS, value); }
        }

        public byte EZI2C_SecondarySlaveAddress
        {
            get { return GetValue<byte>(CyParamNames.EZI2C_SECONDARY_SLAVE_ADDRESS); }
            set { SetValue(CyParamNames.EZI2C_SECONDARY_SLAVE_ADDRESS, value); }
        }

        public bool EZI2C_EnableWakeupFromSleep
        {
            get { return GetValue<bool>(CyParamNames.EZI2C_ENABLE_WAKEUP); }
            set { SetValue(CyParamNames.EZI2C_ENABLE_WAKEUP, value); }
        }

        public string EZI2C_PrimarySlaveAddressStr
        {
            get
            {
                return EZI2C_IsPrimarySlaveAddressHex ? HEX_PREFIX + EZI2C_PrimarySlaveAddress.ToString("X2") :
                    EZI2C_PrimarySlaveAddress.ToString();
            }
        }

        public string EZI2C_SecondarySlaveAddressStr
        {
            get
            {
                return EZI2C_IsSecondarySlaveAddressHex ? HEX_PREFIX + EZI2C_SecondarySlaveAddress.ToString("X2") :
                    EZI2C_SecondarySlaveAddress.ToString();
            }
        }
        #endregion
    }
}
