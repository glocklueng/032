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
    #region Component Parameters Names
    public partial class CyParamNames
    {
        public const string SCB_MODE = "ScbMode";
        public const string SCB_MOSI_SCL_RX_ENABLED = "ScbMosiSclRxEnable";
        public const string SCB_SCLK_ENABLED = "ScbSclkEnable";
        public const string SCB_RX_WAKE = "ScbRxWakeIrqEnable";
        public const string SCB_SS0_ENABLED = "ScbSs0Enable";
        public const string SCB_SS1_ENABLED = "ScbSs1Enable";
        public const string SCB_SS2_ENABLED = "ScbSs2Enable";
        public const string SCB_SS3_ENABLED = "ScbSs3Enable";
        public const string SCB_MISO_SDA_TX_ENABLED = "ScbMisoSdaTxEnable";
    }
    #endregion

    #region Enums
    public enum CyESCBMode
    {
        I2C = 1,
        SPI = 2,
        UART = 4,
        EZI2C = 8,
        UNCONFIG = 255
    }

    public enum CyEInterruptModeType { INTERNAL = 1, EXTERNAL = 2, INTERRUPT_NONE = 0 }
    #endregion

    public partial class CyParameters
    {
        public const string HEX_PREFIX = "0x";

        #region Clock data
        public const string SCB_CLOCK_SCBCLK = "or_sclk";
        public const string SCB_CLOCK_TERM = "clock";
        #endregion

        public ICyInstQuery_v1 m_inst;
        public ICyTabbedParamEditor m_editor = null;
        public ICyTerminalQuery_v1 m_term;

        // During first getting of parameters this variable is false, what means that assigning
        // values to form controls will not immediatly overwrite parameters with the same values.
        private bool m_bGlobalEditMode = false;

        public bool GlobalEditMode
        {
            get { return m_bGlobalEditMode; }
            set { m_bGlobalEditMode = value; }
        }

        // References to tabs used in the customizer
        public CyGeneralTab m_generalTab;
        public CySCBTab m_scbTab;
        public CyEZI2CTab m_ezI2CTab;
        public CyI2CTab m_i2cTab;
        public CyUARTConfigureTab m_uartConfigTab;
        public CyUARTAdvancedTab m_uartAdvancedTab;
        public CySPIBasicTab m_spiBasicTab;
        public CySPIAdvancedTab m_spiAdvancedTab;

        #region Enumerations
        public CyEnum<CyEI2CModeType> m_i2cMode;
        public CyEnum<CyESPIModeType> m_spiMode;
        public CyEnum<CyESPISubModeType> m_spiSubMode;
        public CyEnum<CyESPISCLKModeType> m_spiSCLKMode;
        public CyEnum<CyESPIBitOrderType> m_spiBitsOrder;
        public CyEnum<CyEUARTSubModeType> m_uartSubMode;
        public CyEnum<CyEUARTDirectionType> m_uartDirection;
        public CyEnum<CyEUARTNumberDataBitsType> m_uartNumberDataBits;
        public CyEnum<CyEUARTParityType> m_uartParityType;
        public CyEnum<CyEUARTNumberStopBitsType> m_uartNumberStopBits;
        public CyEnum<CyEEZI2CSubAddressSizeType> m_ezI2CSubAddressSize;
        public CyEnum<CyEEZI2CNumberOfAddressesType> m_ezI2CNumberOfAddresses;
        #endregion

        #region Constructor(s)
        public CyParameters(ICyInstQuery_v1 inst)
        {
            m_inst = inst;

            m_i2cMode = new CyEnum<CyEI2CModeType>(this, CyParamNames.I2C_MODE);
            m_spiMode = new CyEnum<CyESPIModeType>(this, CyParamNames.SPI_MODE);
            m_spiSubMode = new CyEnum<CyESPISubModeType>(this, CyParamNames.SPI_SUB_MODE);
            m_spiSCLKMode = new CyEnum<CyESPISCLKModeType>(this, CyParamNames.SPI_SCLK_MODE);
            m_spiBitsOrder = new CyEnum<CyESPIBitOrderType>(this, CyParamNames.SPI_BITS_ORDER);
            m_uartSubMode = new CyEnum<CyEUARTSubModeType>(this, CyParamNames.UART_SUB_MODE);
            m_uartDirection = new CyEnum<CyEUARTDirectionType>(this, CyParamNames.UART_DIRECTION);
            m_uartParityType = new CyEnum<CyEUARTParityType>(this, CyParamNames.UART_PARITY_TYPE);
            m_uartNumberStopBits = new CyEnum<CyEUARTNumberStopBitsType>(this, CyParamNames.UART_NUMBER_OF_STOP_BITS);
            m_uartNumberDataBits = new CyEnum<CyEUARTNumberDataBitsType>(this, CyParamNames.UART_NUMBER_OF_DATA_BITS);
            m_ezI2CSubAddressSize = new CyEnum<CyEEZI2CSubAddressSizeType>(this, CyParamNames.EZI2C_SUBADDRESS_SIZE);
            m_ezI2CNumberOfAddresses = new CyEnum<CyEEZI2CNumberOfAddressesType>(this,
                CyParamNames.EZI2C_NUMBER_OF_ADDRESSES);
        }
        #endregion

        #region Getting Parameters
        public T GetValue<T>(string paramName)
        {
            T value;
            CyCompDevParam param = m_inst.GetCommittedParam(paramName);
            if (param != null)
            {
                CyCustErr err = param.TryGetValueAs<T>(out value);
                if (err != null && err.IsOK)
                {
                    return value;
                }
            }

            return default(T);
        }
        #endregion

        #region Setting Parameters
        public void SetValue<T>(string paramName, T value)
        {
            if (m_bGlobalEditMode)
            {
                if ((m_inst is ICyInstEdit_v1) == false) return;

                string valueToSet = value.ToString();
                if (value is bool)
                    valueToSet = valueToSet.ToLower();
                if ((m_inst as ICyInstEdit_v1).SetParamExpr(paramName, valueToSet))
                {
                    (m_inst as ICyInstEdit_v1).CommitParamExprs();
                }
            }
        }
        #endregion

        #region Class properties
        public CyESCBMode SCBMode
        {
            get { return GetValue<CyESCBMode>(CyParamNames.SCB_MODE); }
            set { SetValue(CyParamNames.SCB_MODE, value); }
        }

        public bool SCB_MosiSclRxEnabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_MOSI_SCL_RX_ENABLED); }
            set { SetValue(CyParamNames.SCB_MOSI_SCL_RX_ENABLED, value); }
        }

        public bool SCB_SclkEnabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_SCLK_ENABLED); }
            set { SetValue(CyParamNames.SCB_SCLK_ENABLED, value); }
        }

        public bool SCB_RxWake
        {
            get { return GetValue<bool>(CyParamNames.SCB_RX_WAKE); }
            set { SetValue(CyParamNames.SCB_RX_WAKE, value); }
        }

        public bool SCB_Ss0Enabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_SS0_ENABLED); }
            set { SetValue(CyParamNames.SCB_SS0_ENABLED, value); }
        }

        public bool SCB_Ss1Enabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_SS1_ENABLED); }
            set { SetValue(CyParamNames.SCB_SS1_ENABLED, value); }
        }

        public bool SCB_Ss2Enabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_SS2_ENABLED); }
            set { SetValue(CyParamNames.SCB_SS2_ENABLED, value); }
        }

        public bool SCB_Ss3Enabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_SS3_ENABLED); }
            set { SetValue(CyParamNames.SCB_SS3_ENABLED, value); }
        }

        public bool SCB_MisoSdaTxEnabled
        {
            get { return GetValue<bool>(CyParamNames.SCB_MISO_SDA_TX_ENABLED); }
            set { SetValue(CyParamNames.SCB_MISO_SDA_TX_ENABLED, value); }
        }
        #endregion

        #region Service functions
        /// <summary>
        /// Show/hide tabs depending on the configuration
        /// </summary>
        public void UpdateTabVisibility()
        {
            if ((m_editor == null) || (m_scbTab == null) || (m_i2cTab == null) || (m_uartConfigTab == null) ||
                (m_uartAdvancedTab == null) || (m_spiBasicTab == null) || (m_spiAdvancedTab == null) || 
                (m_ezI2CTab == null))
            {
                return;
            }

            m_editor.HideCustomPage(Resources.SCBTabDisplayName);
            m_editor.HideCustomPage(Resources.I2CTabDisplayName);
            m_editor.HideCustomPage(Resources.SPIBasicTabDisplayName);
            m_editor.HideCustomPage(Resources.SPIAdvancedTabDisplayName);
            m_editor.HideCustomPage(Resources.UARTConfigTabDisplayName);
            m_editor.HideCustomPage(Resources.UARTAdvancedTabDisplayName);
            m_editor.HideCustomPage(Resources.EZI2CTabDisplayName);

            switch (SCBMode)
            {
                case CyESCBMode.UNCONFIG:
                    m_editor.ShowCustomPage(Resources.SCBTabDisplayName);
                    break;
                case CyESCBMode.I2C:
                    m_editor.ShowCustomPage(Resources.I2CTabDisplayName);
                    break;
                case CyESCBMode.UART:
                    m_editor.ShowCustomPage(Resources.UARTConfigTabDisplayName);
                    m_editor.ShowCustomPage(Resources.UARTAdvancedTabDisplayName);
                    break;
                case CyESCBMode.SPI:
                    m_editor.ShowCustomPage(Resources.SPIBasicTabDisplayName);
                    m_editor.ShowCustomPage(Resources.SPIAdvancedTabDisplayName);
                    break;
                case CyESCBMode.EZI2C:
                    m_editor.ShowCustomPage(Resources.EZI2CTabDisplayName);
                    break;
            }
        }

        public void UpdateDesigns(ICyInstQuery_v1 query, ICyTerminalQuery_v1 termQuery)
        {
            if (m_i2cTab != null)
                m_i2cTab.UpdateClock();
            if (m_spiBasicTab != null)
                m_spiBasicTab.UpdateClock();
            if (m_uartConfigTab != null)
                m_uartConfigTab.UpdateClock();
            if (m_ezI2CTab != null)
                m_ezI2CTab.UpdateClock();
        }

        /// <summary>
        /// Returns internal clock value
        /// </summary>
        public static double GetInternalClock(ICyTerminalQuery_v1 termQuery)
        {
            List<CyClockData> clkdata = new List<CyClockData>();

            clkdata = termQuery.GetClockData(SCB_CLOCK_SCBCLK, "term1", 0);

            if (clkdata.Count > 0)
            {
                if (clkdata[0].IsFrequencyKnown)
                {
                    return clkdata[0].Frequency * Math.Pow(10, clkdata[0].UnitAsExponent - 3);
                }
            }
            return -1;
        }

        public static double GetExternalClock(ICyTerminalQuery_v1 termQuery)
        {
            List<CyClockData> clkdata = termQuery.GetClockData(SCB_CLOCK_TERM, 0);
            if (clkdata.Count > 0)
            {
                if (clkdata[0].IsFrequencyKnown)
                {
                    return clkdata[0].Frequency * Math.Pow(10, clkdata[0].UnitAsExponent - 3);                    
                }
            }
            return (-1);
        }
        #endregion Service functions
    }

    public class CyEnum<T>
    {
        CyParameters m_prm;
        string m_paramName;

        public CyEnum(CyParameters prm, string paramName)
        {
            m_prm = prm;
            m_paramName = paramName;
        }

        /// <summary>
        /// Gets and sets the parameter's value
        /// </summary>
        public T Value
        {
            get
            {
                return m_prm.GetValue<T>(m_paramName);
            }
            set
            {
                m_prm.SetValue<T>(m_paramName, value);
            }
        }

        /// <summary>
        /// Gets the enum item description or sets the parameter's value by enum item description.
        /// </summary>
        public string Description
        {
            get
            {
                string res = string.Empty;

                CyCompDevParam param = m_prm.m_inst.GetCommittedParam(m_paramName);
                if (param != null)
                {
                    m_prm.m_inst.ResolveEnumParamToDisplay(param, out res);
                }

                return res;
            }
            set
            {
                string enumItemName = m_prm.m_inst.ResolveEnumDisplayToId(m_paramName, value);

                try
                {
                    Value = (T)Enum.Parse(typeof(T), enumItemName);
                }
                catch { }
            }
        }

        /// <summary>
        /// Gets the array of strings - descriptions of the enumeration that is a type of the parameter.
        /// </summary>
        public string[] EnumDescriptionsList
        {
            get
            {
                return new List<string>(m_prm.m_inst.GetPossibleEnumValues(m_paramName)).ToArray();
            }
        }

        /// <summary>
        /// Get the enum item description by the specified enum item value.
        /// </summary>
        /// <param name="value">Enum item value</param>
        /// <returns>Enum item description</returns>
        public string GetDescription(T value)
        {
            string res = string.Empty;

            CyCompDevParam prm = m_prm.m_inst.GetCommittedParam(m_paramName);

            if (prm != null)
            {
                CyCompDevParam newParam = new CyCompDevParam(prm.Name, prm.Expr, prm.ExprType, value.ToString(),
                    prm.TypeName, prm.DefaultExpr, prm.TabName, prm.CategoryName, prm.Description, prm.IsVisible,
                    prm.IsReadOnly, prm.IsHardware, prm.IsFormal, prm.IsDisplayEvaluated, prm.ErrorCount, value,
                    prm.Errors);

                if (newParam != null)
                {
                    m_prm.m_inst.ResolveEnumParamToDisplay(newParam, out res);
                }
            }

            return res;
        }
    }
}
