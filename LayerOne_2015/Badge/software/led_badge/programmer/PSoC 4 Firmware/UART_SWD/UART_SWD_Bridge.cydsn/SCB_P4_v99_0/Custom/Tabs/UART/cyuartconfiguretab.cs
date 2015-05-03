/*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CyDesigner.Extensions.Gde;

namespace SCB_P4_v99_0
{
    public partial class CyUARTConfigureTab : CyTabControlWrapper
    {
        #region Auxiliary data
        readonly UInt32[] IRDA_DATA_RATES_LIST = new UInt32[]
        {
            1200,
            2400,
            9600,
            19200,
            38400,
            57600,
            115200
        };

        readonly UInt16[] IRDA_OVS_FACTOR_LIST = new UInt16[]
        {
            1536,
            768,
            192,
            96,
            48,
            32,
            16
        };

        readonly UInt32[] DATA_RATES_LIST = new UInt32[]
        {
            110,
            300,
            1200,
            2400,
            4800,
            9600,
            19200,
            38400,
            57600,
            115200,
            230400,
            460800,
            921600
        };
        #endregion

        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "UART Basic";
            }
        }
        #endregion

        #region Constructor(s)
        public CyUARTConfigureTab(CyParameters parameters)
            : base(parameters)
        {
            m_params = parameters;
            m_params.m_uartConfigTab = this;

            InitializeComponent();

            #region Add items to comboboxes
            m_cbMode.Items.AddRange(m_params.m_uartSubMode.EnumDescriptionsList);
            m_cbDirection.Items.AddRange(m_params.m_uartDirection.EnumDescriptionsList);
            m_cbDataBits.Items.AddRange(m_params.m_uartNumberDataBits.EnumDescriptionsList);
            m_cbParityType.Items.AddRange(m_params.m_uartParityType.EnumDescriptionsList);
            m_cbStopBits.Items.AddRange(m_params.m_uartNumberStopBits.EnumDescriptionsList);
            UpdateBaudRateList();
            #endregion

            #region Add event handlers
            m_cbMode.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbDirection.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbDataBits.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbParityType.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbStopBits.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);

            m_cbBaudRate.TextChanged += new EventHandler(m_cb_TextChanged);

            m_numOversamplingFactor.TextChanged += new EventHandler(m_num_TextChanged);

            m_chbEnableMedianFilter.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbRetryOnNack.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInvertRx.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableWakeup.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbLowPowerReceive.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbClockFromTerminal.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            #endregion
        }
        #endregion

        #region Update UI
        public override void UpdateUI()
        {
            m_cbMode.SelectedItem = m_params.m_uartSubMode.Description;
            m_cbDirection.SelectedItem = m_params.m_uartDirection.Description;
            m_cbBaudRate.Text = m_params.UART_DataRate.ToString();
            m_cbDataBits.SelectedItem = m_params.m_uartNumberDataBits.Description;
            m_cbParityType.SelectedItem = m_params.m_uartParityType.Description;
            m_cbStopBits.SelectedItem = m_params.m_uartNumberStopBits.Description;

            m_numOversamplingFactor.Text = m_params.UART_OversamplingFactor.ToString();

            m_chbClockFromTerminal.Checked = m_params.UART_ClockFromTerm;
            m_chbRetryOnNack.Checked = m_params.UART_RetryOnNACK;
            m_chbEnableMedianFilter.Checked = m_params.UART_MedianFilterEnabled;
            m_chbInvertRx.Checked = m_params.UART_InvertRXProperty;
            m_chbEnableWakeup.Checked = m_params.UART_WakeupFromSleepEnabled;
            m_chbLowPowerReceive.Checked = m_params.UART_LowPowerReceive;

            UpdateMedianFilter();
            UpdateRetryOnNACK();
            UpdateInvertRX();
            UpdateEnableWakeup();
            UpdateLowPowerEnable();

            UpdateOvsFactor();

            UpdateDirection();
            UpdateParityType();
            UpdateStopBitEnable();

            UpdateClock();
        }

        #region Updating comboboxes' state
        private void UpdateDirection()
        {
            if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD) ||
                (m_params.UART_LowPowerReceive == true))
            {
                m_cbDirection.Enabled = false;
            }
            else
            {
                m_cbDirection.Enabled = true;
            }
        }

        private void ResetDirection()
        {
            if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD)
            {
                m_cbDirection.SelectedItem = m_params.m_uartDirection.GetDescription(CyEUARTDirectionType.TX_RX);
            }
            else if (m_params.UART_LowPowerReceive == true)
            {
                m_cbDirection.SelectedItem = m_params.m_uartDirection.GetDescription(CyEUARTDirectionType.RX);
            }

            UpdateDirection();
        }

        // Baud rate
        private void UpdateBaudRateList()
        {
            m_cbBaudRate.Items.Clear();

            uint[] baudRates = (m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA) ? IRDA_DATA_RATES_LIST :
                DATA_RATES_LIST;

            foreach (UInt32 rate in baudRates)
            {
                m_cbBaudRate.Items.Add(rate);
            }
            m_cbBaudRate.Text = m_params.UART_DataRate.ToString();

            m_cb_TextChanged(m_cbBaudRate, null);
        }
        
        // Data bits
        private void ResetDataBits()
        {
            string nineBits = m_params.m_uartNumberDataBits.GetDescription(CyEUARTNumberDataBitsType.NINE);

            if (m_params.m_uartSubMode.Value != CyEUARTSubModeType.STANDARD)
            {
                if (m_params.m_uartNumberDataBits.Value == CyEUARTNumberDataBitsType.NINE)
                {
                    m_errorProvider.SetError(m_cbDataBits, string.Format(Resources.UARTDataBitsValueError, nineBits));
                }
                else
                {
                    m_errorProvider.SetError(m_cbDataBits, string.Empty);
                    if (m_cbDataBits.Items.Contains(nineBits))
                    {
                        m_cbDataBits.Items.Remove(nineBits);
                    }
                }
            }
            else
            {
                if (!m_cbDataBits.Items.Contains(nineBits))
                {
                    m_cbDataBits.Items.Add(nineBits);
                }
                else
                {
                    m_errorProvider.SetError(m_cbDataBits, string.Empty);
                }
            }
        }

        // Parity
        private void UpdateParityType()
        {
            m_cbParityType.Enabled = (m_params.m_uartSubMode.Value == CyEUARTSubModeType.STANDARD) &&
                (m_params.m_uartNumberDataBits.Value != CyEUARTNumberDataBitsType.NINE);
        }

        private void ResetParityType()
        {
            if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD)
            {
                m_cbParityType.SelectedItem = m_params.m_uartParityType.GetDescription(CyEUARTParityType.EVEN);
            }
            else if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA) ||
                (m_params.m_uartNumberDataBits.Value == CyEUARTNumberDataBitsType.NINE))
            {
                m_params.m_uartParityType.Value = CyEUARTParityType.PARITY_NONE;
                m_cbParityType.SelectedItem = m_params.m_uartParityType.Description;
            }

            UpdateParityType();
        }

        // StopBit enable/disable
        private void UpdateStopBitEnable()
        {
            // In SmartCard mode StopBits should be equal 2.
            if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD)
            {
                if (m_params.GlobalEditMode ||
                    m_params.m_uartNumberStopBits.Value == CyEUARTNumberStopBitsType.TWO_BITS)
                {
                    m_cbStopBits.Enabled = false;
                    m_params.m_uartNumberStopBits.Value = CyEUARTNumberStopBitsType.TWO_BITS;
                    m_cbStopBits.SelectedItem = m_params.m_uartNumberStopBits.
                        GetDescription(CyEUARTNumberStopBitsType.TWO_BITS);
                }
            }
            else
            {
                m_cbStopBits.Enabled = true;
            }

            // Error provider
            if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD) && 
                (m_params.m_uartNumberStopBits.Value != CyEUARTNumberStopBitsType.TWO_BITS))
            {
                m_errorProvider.SetError(m_cbStopBits, Resources.UARTStopBitsSmartCardError);
                m_cbStopBits.Enabled = true;
            }
            else
            {
                m_errorProvider.SetError(m_cbStopBits, string.Empty);
            }
        }
        #endregion

        #region Updating checkboxes' state
        // Clock from terminal - no need

        // Median filter
        private void UpdateMedianFilter()
        {
            // Enabled
            m_chbEnableMedianFilter.Enabled = (m_params.m_uartSubMode.Value != CyEUARTSubModeType.IRDA) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX);

            // Checked
            if (m_params.m_uartDirection.Value == CyEUARTDirectionType.TX)
            {
                m_chbEnableMedianFilter.Checked = false;
            }
            else if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA)
            {
                m_chbEnableMedianFilter.Checked = true;
            }
        }

        // Retry on NACK
        private void UpdateRetryOnNACK()
        {
            if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD)
            {
                m_chbRetryOnNack.Enabled = true;
            }
            else
            {
                m_chbRetryOnNack.Enabled = false;
                m_chbRetryOnNack.Checked = false;
            }
        }

        // Inverting RX
        private void UpdateInvertRX()
        {
            if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX))
            {
                m_chbInvertRx.Enabled = true;
            }
            else
            {
                m_chbInvertRx.Enabled = false;
                m_chbInvertRx.Checked = false;
            }
        }

        // Enable wakeup from Sleep Mode
        public void UpdateEnableWakeup()
        {
            if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.STANDARD) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX) &&
                (m_params.UART_InterruptMode == CyEInterruptModeType.INTERNAL))
            {
                m_chbEnableWakeup.Enabled = true;
            }
            else
            {
                m_chbEnableWakeup.Enabled = false;
                m_chbEnableWakeup.Checked = false;
            }
        }

        // Low power receiving
        private void UpdateLowPowerEnable()
        {
            if ((m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA) &&
                (m_params.m_uartDirection.Value == CyEUARTDirectionType.RX))
            {
                m_chbLowPowerReceive.Enabled = true;
            }
            else
            {
                m_chbLowPowerReceive.Enabled = false;
                m_chbLowPowerReceive.Checked = false;
            }
        }
        #endregion

        private void UpdateOvsFactor()
        {
            bool isIrDAMode = m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA;

            if (isIrDAMode)
            {
                m_numOversamplingFactor.Value = CyParamRanges.UART_OVS_FACTOR_MAX;

                if (m_params.UART_LowPowerReceive == true)
                {
                    for (int i = 0; i < IRDA_DATA_RATES_LIST.GetLength(0); i++)
                    {
                        if (IRDA_DATA_RATES_LIST[i] == m_params.UART_DataRate)
                        {
                            m_numOversamplingFactor.Value = IRDA_OVS_FACTOR_LIST[i];
                        }
                    }
                }
            }
            m_numOversamplingFactor.Enabled = !isIrDAMode;

            m_num_TextChanged(m_numOversamplingFactor, null);
        }
        #endregion

        #region Event handlers
        void m_cb_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_cbMode)
                {
                    m_params.m_uartSubMode.Description = m_cbMode.Text;

                    UpdateBaudRateList();
                    UpdateOvsFactor();

                    UpdateMedianFilter();
                    UpdateRetryOnNACK();
                    UpdateInvertRX();
                    UpdateEnableWakeup();
                    UpdateLowPowerEnable();
                    UpdateParityType();
                    UpdateStopBitEnable();
                    ResetDataBits();
                    ResetDirection();

                    if (m_params.m_uartAdvancedTab != null)
                    {
                        m_params.m_uartAdvancedTab.ResetTXSmartCardInterrupts();
                        m_params.m_uartAdvancedTab.ResetMultiprocessorMode();
                    }
                }
                else if (sender == m_cbDirection)
                {
                    m_params.m_uartDirection.Description = m_cbDirection.Text;

                    UpdateMedianFilter();
                    UpdateInvertRX();
                    UpdateEnableWakeup();
                    UpdateLowPowerEnable();

                    if (m_params.m_uartAdvancedTab != null)
                    {
                        m_params.m_uartAdvancedTab.ResetTXBufferSize();
                        m_params.m_uartAdvancedTab.ResetRXBufferSize();

                        m_params.m_uartAdvancedTab.ResetInterruptSources();
                        m_params.m_uartAdvancedTab.UpdateRXFrameErrorIntr();
                    }
                }
                else if (sender == m_cbDataBits)
                {
                    m_params.m_uartNumberDataBits.Description = m_cbDataBits.Text;

                    UpdateParityType();
                    ResetDataBits();

                    if (m_params.m_uartAdvancedTab != null)
                    {
                        m_params.m_uartAdvancedTab.ResetMultiprocessorMode();
                    }
                }
                else if (sender == m_cbParityType)
                {
                    m_params.m_uartParityType.Description = m_cbParityType.Text;

                    if (m_params.m_uartAdvancedTab != null)
                    {
                        m_params.m_uartAdvancedTab.ResetRXFIFODrop();
                    }
                }
                else if (sender == m_cbStopBits)
                {
                    m_params.m_uartNumberStopBits.Description = m_cbStopBits.Text;
                    UpdateStopBitEnable();

                    if (m_params.m_uartAdvancedTab != null)
                    {
                        m_params.m_uartAdvancedTab.UpdateRXFrameErrorIntr();
                        m_params.m_uartAdvancedTab.ResetRXFIFODrop();
                    }
                }
            }
        }

        void m_num_TextChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_numOversamplingFactor)
                {
                    bool skipCheckRange = (m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA &&
                        m_params.UART_LowPowerReceive);

                    UInt16 value;
                    bool parseSuccess = UInt16.TryParse(m_numOversamplingFactor.Text, out value);

                    if (parseSuccess)
                    {
                        m_params.UART_OversamplingFactor = value;
                    }

                    if (parseSuccess && (skipCheckRange ||
                        (value >= CyParamRanges.UART_OVS_FACTOR_MIN && value <= CyParamRanges.UART_OVS_FACTOR_MAX)))
                    {
                        m_errorProvider.SetError(m_numOversamplingFactor, string.Empty);
                        UpdateClock();
                    }
                    else
                    {
                        m_errorProvider.SetError(m_numOversamplingFactor, string.Format(Resources.UARTOvsFactorError,
                                CyParamRanges.UART_OVS_FACTOR_MIN, CyParamRanges.UART_OVS_FACTOR_MAX));
                    }
                }
            }
        }

        void m_chb_CheckedChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_chbEnableMedianFilter)
                {
                    m_params.UART_MedianFilterEnabled = m_chbEnableMedianFilter.Checked;
                }
                else if (sender == m_chbRetryOnNack)
                {
                    m_params.UART_RetryOnNACK = m_chbRetryOnNack.Checked;
                }
                else if (sender == m_chbInvertRx)
                {
                    m_params.UART_InvertRXProperty = m_chbInvertRx.Checked;
                }
                else if (sender == m_chbEnableWakeup)
                {
                    m_params.UART_WakeupFromSleepEnabled = m_chbEnableWakeup.Checked;
                }
                else if (sender == m_chbLowPowerReceive)
                {
                    m_params.UART_LowPowerReceive = m_chbLowPowerReceive.Checked;
                    ResetDirection();
                    UpdateOvsFactor();
                }
                else if (sender == m_chbClockFromTerminal)
                {
                    m_params.UART_ClockFromTerm = m_chbClockFromTerminal.Checked;
                    UpdateClock();
                }
            }
        }

        void m_cb_TextChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_cbBaudRate)
                {
                    UInt32 value;
                    bool parseSuccess = UInt32.TryParse(m_cbBaudRate.Text, out value);

                    if (parseSuccess)
                    {
                        m_params.UART_DataRate = value;
                    }

                    if (m_params.m_uartSubMode.Value == CyEUARTSubModeType.IRDA)
                    {
                        List<uint> rates = new List<uint>(IRDA_DATA_RATES_LIST);
                        if (parseSuccess && rates.Contains(value))
                        {
                            m_errorProvider.SetError(m_cbBaudRate, string.Empty);
                            UpdateOvsFactor();
                            UpdateClock();
                        }
                        else
                        {
                            m_errorProvider.SetError(m_cbBaudRate, Resources.BaudRateIrDAValueError);
                        }
                    }
                    else
                    {
                        if (parseSuccess && value >= CyParamRanges.UART_DATA_RATE_MIN_VALUE &&
                            value <= CyParamRanges.UART_DATA_RATE_MAX_VALUE)
                        {
                            m_errorProvider.SetError(m_cbBaudRate, string.Empty);
                            UpdateClock();
                        }
                        else
                        {
                            m_errorProvider.SetError(m_cbBaudRate, string.Format(Resources.BaudRateValueError,
                                CyParamRanges.UART_DATA_RATE_MIN_VALUE, CyParamRanges.UART_DATA_RATE_MAX_VALUE));
                        }
                    }
                }
            }
        }
        #endregion

        #region Actual baud rate
        public void UpdateClock()
        {
            double clkFreq = m_params.UART_ClockFromTerm ?
                CyParameters.GetExternalClock(m_params.m_term) :
                CyParameters.GetInternalClock(m_params.m_term);

            string dataRateStr = Resources.ClockDisplayUnknown;

            m_lblBaudRate.Enabled = !m_params.UART_ClockFromTerm;
            m_cbBaudRate.Enabled = !m_params.UART_ClockFromTerm;

            if (clkFreq > 0)
            {
                double actualBaudRate = Math.Round(clkFreq * 1000 / m_params.UART_OversamplingFactor);
                if (actualBaudRate > 0)
                {
                    dataRateStr = actualBaudRate.ToString();
                }
            }

            // Update Actual Data Rate label
            m_lblActualBaudRate.Text = string.Format(Resources.ActualBaudRateDisplayFormat, dataRateStr);
        }
        #endregion
    }
}
