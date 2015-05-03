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
using CyDesigner.Extensions.Common;

namespace SCB_P4_v99_0
{
    public partial class CyI2CTab : CyTabControlWrapper
    {
        const int POW3 = 1000;
        const int POW6 = 1000000;

        #region Class private members
        private byte m_previousOvsFactor;
        private byte m_previousOvsFactorLow;
        private byte m_previousOvsFactorHigh;
        private bool m_numericLocked = false; // Used for locking numericUpDown control to prevent infinite loop
        private double m_sclk;
        private double m_actualDataRate;
        #endregion

        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "I2C";
            }
        }
        #endregion
        
        #region Constructor(s)
        public CyI2CTab(CyParameters parameters)
            : base(parameters)
        {
            m_params.m_i2cTab = this;

            InitializeComponent();

            // Assigning ModeType possible values to ComboBox
            m_cbMode.Items.AddRange(m_params.m_i2cMode.EnumDescriptionsList);

            #region Assigning event handlers
            m_chbAcceptAddress.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbClockFromTerminal.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableMedianFilter.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableWakeup.CheckedChanged += new EventHandler(m_chb_CheckedChanged);

            m_numOversamplingFactor.TextChanged += new EventHandler(numOversamplingFactor_TextChanged);
            m_numOversamplingFactorHigh.Validating += new CancelEventHandler(numOversamplingFactorLow_Validating);
            m_numOversamplingFactorHigh.ValueChanged += new EventHandler(numOversamplingFactorLow_ValueChanged);
            m_numOversamplingFactorLow.Validating += new CancelEventHandler(numOversamplingFactorLow_Validating);
            m_numOversamplingFactorLow.ValueChanged += new EventHandler(numOversamplingFactorLow_ValueChanged);

            m_cbMode.SelectedIndexChanged += new EventHandler(m_cbMode_SelectedIndexChanged);
            m_cbDataRate.TextChanged += new EventHandler(m_cbDataRate_TextChanged);

            m_txtSlaveAddress.TextChanged += new EventHandler(tbSlaveAddress_TextChanged);
            m_txtSlaveAddressMask.TextChanged += new EventHandler(tbSlaveAddressMask_TextChanged);

            m_binaryGridSlaveAddress.BitChanged += new CyBinaryGrid.BitChangedEventHandler(
                binaryGridSlaveAddress_BitChanged);
            m_binaryGridSlaveAddressMask.BitChanged += new CyBinaryGrid.BitChangedEventHandler(
                binaryGridSlaveAddressMask_BitChanged);

            m_binaryGridSlaveAddressMask.SetErrorListItem(7, CyBinaryGrid.OneSign);

            #endregion
        }
        #endregion

        #region Assigning parameters values to controls
        public override void UpdateUI()
        {
            if (m_params.GlobalEditMode == false)
            {
                // This code runs on customizer load or when something was changed in Expression view
                if ((m_params.I2C_OvsFactorLow + m_params.I2C_OvsFactorHigh) != m_params.I2C_OvsFactor)
                {
                    m_numOversamplingFactorLow.Minimum = 0;
                    m_numOversamplingFactorLow.Maximum = decimal.MaxValue;
                    m_numOversamplingFactorHigh.Minimum = 0;
                    m_numOversamplingFactorHigh.Maximum = decimal.MaxValue;
                }
                numOversamplingFactorLow_Validating(m_numOversamplingFactorLow, new CancelEventArgs());
            }

            // I2CMode
            m_cbMode.SelectedItem = m_params.m_i2cMode.Description;

            // I2CDataRate
            m_cbDataRate.Text = m_params.I2C_DataRate.ToString();

            //Clock from terminal
            m_chbClockFromTerminal.Checked = m_params.I2C_ClockFromTerm;

            // I2COvsFactor
            m_numOversamplingFactor.Text = m_params.I2C_OvsFactor.ToString();

            // OvsFactorLow
            m_numOversamplingFactorLow.Text = m_params.I2C_OvsFactorLow.ToString();

            // OvsFactorHigh
            m_numOversamplingFactorHigh.Text = m_params.I2C_OvsFactorHigh.ToString();

            // I2CEnableMedianFilter
            m_chbEnableMedianFilter.Checked = m_params.I2C_EnableMedianFilter;

            // SlaveAddressMask
            if (m_params.I2C_IsSlaveAddressMaskHex)
                m_txtSlaveAddressMask.Text = CyConvert.IntToHex(m_params.I2C_SlaveAddressMask);
            else
                m_txtSlaveAddressMask.Text = m_params.I2C_SlaveAddressMask.ToString();

            // SlaveAddress
            if (m_params.I2C_IsSlaveAddressHex)
                m_txtSlaveAddress.Text = CyConvert.IntToHex(m_params.I2C_SlaveAddress);
            else
                m_txtSlaveAddress.Text = m_params.I2C_SlaveAddress.ToString();

            // AccepAddress
            m_chbAcceptAddress.Checked = m_params.I2C_AcceptAddress;

            // EnableWakeup
            m_chbEnableWakeup.Checked = m_params.I2C_EnableWakeup;

            // Save current values to know if the value of numeric controls was changed next time
            m_previousOvsFactor = m_params.I2C_OvsFactor;
            m_previousOvsFactorLow = (byte)m_numOversamplingFactorLow.Value;
            m_previousOvsFactorHigh = (byte)m_numOversamplingFactorHigh.Value;
            numOversamplingFactorLow_Validating(m_numOversamplingFactorLow, new CancelEventArgs());

            UpdateClock();
            UpdateValidators();
        }

        private void UpdateValidators()
        {
            m_binaryGridSlaveAddress.SetErrorListItem(6, m_params.I2C_EnableWakeup ? 
                CyBinaryGrid.OneSign : string.Empty);
            m_binaryGridSlaveAddressMask.SetErrorListItem(6, m_params.I2C_EnableWakeup ?
                CyBinaryGrid.ZeroSign : string.Empty);
        }

        private void UpdateTLowTHigh()
        {
            if (m_actualDataRate > 0)
            {
                // Calculate tLow and tHigh values, compare them with I2C spec data and 
                // update Ovs Low/High error providers
                double tLo = Math.Round((POW6 / m_sclk) * m_params.I2C_OvsFactorLow);  // SCLK in nS mult to OvsLow 
                double tHi = Math.Round((POW6 / m_sclk) * m_params.I2C_OvsFactorHigh); // SCLK in nS mult to OvsHigh

                double tLo_ms = Math.Round(tLo / POW3, 3);
                double tHi_ms = Math.Round(tHi / POW3, 3);

                if (m_actualDataRate <= CyParamRanges.I2C_SCL_FREQ_STANDARD_MODE_MAX)
                {
                    if (tLo < (CyParamRanges.I2C_TLO_STANDARD_MODE_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Format(Resources.TLoError, tLo_ms,
                            CyParamRanges.I2C_TLO_STANDARD_MODE_MIN, m_actualDataRate));
                    else if (tLo < (CyParamRanges.I2C_THI_STANDARD_MODE_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Format(Resources.THiError, tHi_ms,
                            CyParamRanges.I2C_THI_STANDARD_MODE_MIN, m_actualDataRate));
                    else
                    {
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Empty);
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Empty);
                    }
                }
                else if (m_actualDataRate <= CyParamRanges.I2C_SCL_FREQ_FAST_MODE_MAX)
                {
                    if (tLo < (CyParamRanges.I2C_TLO_FAST_MODE_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Format(Resources.TLoError, tLo_ms,
                            CyParamRanges.I2C_TLO_FAST_MODE_MIN, m_actualDataRate));
                    else if (tLo < (CyParamRanges.I2C_THI_FAST_MODE_PLUS_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Format(Resources.THiError, tHi_ms,
                            CyParamRanges.I2C_THI_FAST_MODE_PLUS_MIN, m_actualDataRate));
                    else
                    {
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Empty);
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Empty);
                    }
                }
                else if (m_actualDataRate <= CyParamRanges.I2C_SCL_FREQ_FAST_MODE_PLUS_MAX)
                {
                    if (tLo < (CyParamRanges.I2C_TLO_FAST_MODE_PLUS_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Format(Resources.TLoError, tLo_ms,
                            CyParamRanges.I2C_TLO_FAST_MODE_PLUS_MIN, m_actualDataRate));
                    else if (tLo < (CyParamRanges.I2C_THI_FAST_MODE_PLUS_MIN * POW3))
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Format(Resources.THiError, tHi_ms,
                            CyParamRanges.I2C_THI_FAST_MODE_PLUS_MIN, m_actualDataRate));
                    else
                    {
                        m_errorProvider.SetError(m_numOversamplingFactorLow, string.Empty);
                        m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Empty);
                    }
                }
            }
        }
        #endregion

        #region Actual data rate
        public void UpdateClock()
        {
            m_sclk = m_params.I2C_ClockFromTerm ?
                CyParameters.GetExternalClock(m_params.m_term) :
                CyParameters.GetInternalClock(m_params.m_term);

            string dataRateStr = Resources.ClockDisplayUnknown;

            m_cbDataRate.Enabled = !m_params.I2C_ClockFromTerm;
            m_lblDataRate.Enabled = !m_params.I2C_ClockFromTerm;

            if (m_sclk > 0)
            {

                m_actualDataRate = Math.Round(m_sclk / m_params.I2C_OvsFactor, 3);
                if (m_actualDataRate > 0)
                {
                    dataRateStr = m_actualDataRate.ToString();
                }
            }

            // Update Actual Data Rate label
            m_lblActualDataRate.Text = string.Format(Resources.ActualDataRateDisplayFormat, dataRateStr);
        }
        #endregion

        #region Event Handlers
        #region Slave Address configuration
        private void binaryGridSlaveAddress_BitChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode) // avoid this on customizer load
            {
                CyBinaryGrid currentBinaryGrid = (CyBinaryGrid)sender;

                if (currentBinaryGrid.Locked == false)
                {
                    currentBinaryGrid.Locked = true; // lock binary box from updating after textbox update

                    if (m_txtSlaveAddress.Text.StartsWith(CyConvert.HEX_PREFIX))
                    {
                        // Hexadecimal value handling
                        m_txtSlaveAddress.Text = CyConvert.Bin7ToHex(currentBinaryGrid.Bits);
                    }
                    else
                    {
                        // Decimal value handling
                        m_txtSlaveAddress.Text = CyConvert.Bin7ToInt(currentBinaryGrid.Bits).ToString();
                    }
                    currentBinaryGrid.Locked = false; // release lock
                }
            }            
        }

        private void tbSlaveAddress_TextChanged(object sender, EventArgs e)
        {
            TextBox currentTextBox = (TextBox)sender;

            #region Communication between textbox and binary grid
            if (m_binaryGridSlaveAddress.Locked == false)
            {
                m_binaryGridSlaveAddress.Locked = true; // lock textbox from updating after binary box update

                if (currentTextBox.Text.Contains(CyConvert.HEX_PREFIX))
                {
                    // Hexadecimal value handling
                    int value = CyConvert.HexToInt(currentTextBox.Text);
                    if (value >= CyParamRanges.I2C_SLAVE_ADDRESS_MIN && value <= CyParamRanges.I2C_SLAVE_ADDRESS_MAX)                   
                    {
                        m_binaryGridSlaveAddress.Bits = CyConvert.HexToBin7(currentTextBox.Text);
                    }
                }
                else
                {
                    // Decimal value handling
                    int value = 0;
                    if (int.TryParse(currentTextBox.Text, out value))
                    {
                        if (value < CyParamRanges.I2C_SLAVE_ADDRESS_MIN || value > CyParamRanges.I2C_SLAVE_ADDRESS_MAX)
                            value = 0;
                    }
                    m_binaryGridSlaveAddress.Bits = CyConvert.IntToBin7(value);
                }

                string[] tmpBits = new string[m_binaryGridSlaveAddress.Bits.Length];

                // Restore X signs if zero
                for (int i = 0; i < tmpBits.Length; i++)
                {
                    tmpBits[i] = (m_binaryGridSlaveAddressMask.Bits[i] == CyBinaryGrid.ZeroSign) ? CyBinaryGrid.XSign :
                        m_binaryGridSlaveAddress.Bits[i];
                }
                // Change view only here
                m_binaryGridSlaveAddress.UpdateGridViewOnly(tmpBits);

                m_binaryGridSlaveAddress.Locked = false; // release lock
            }
            #endregion

            #region Set control value to appropriate parameter
            try
            {
                byte textboxValue = 0;
                string min, max;
                bool isHex = false;

                if (currentTextBox.Text.Contains(CyConvert.HEX_PREFIX))
                {
                    // Value was input as hexadecimal
                    textboxValue = (byte)CyConvert.HexToInt(currentTextBox.Text);
                    min = CyConvert.IntToHex(CyParamRanges.I2C_SLAVE_ADDRESS_MIN);
                    max = CyConvert.IntToHex(CyParamRanges.I2C_SLAVE_ADDRESS_MAX);
                    isHex = true;
                }
                else
                {
                    // Value was input as decimal
                    textboxValue = byte.Parse(currentTextBox.Text);
                    min = CyParamRanges.I2C_SLAVE_ADDRESS_MIN.ToString();
                    max = CyParamRanges.I2C_SLAVE_ADDRESS_MAX.ToString();
                    isHex = false;
                }

                if (textboxValue >= CyParamRanges.I2C_SLAVE_ADDRESS_MIN && textboxValue <=
                    CyParamRanges.I2C_SLAVE_ADDRESS_MAX)
                {
                    string errorMessage = GetSlaveAddressError(textboxValue);
                    if (string.IsNullOrEmpty(errorMessage) == false) throw new Exception(errorMessage);

                    m_params.I2C_SlaveAddress = textboxValue;
                    m_params.I2C_IsSlaveAddressHex = isHex;
                }
                else
                    throw new Exception(string.Format(Resources.I2CSlaveAddressError, min, max));

                m_errorProvider.SetError(currentTextBox, string.Empty);                
            }
            catch (Exception ex)
            {
                m_errorProvider.SetError(currentTextBox, ex.Message);                
            }
            #endregion
        }

        private string GetSlaveAddressError(byte slaveAddressValue)
        {
            string res = string.Empty;
            if ((m_params.I2C_EnableWakeup == true) && ((slaveAddressValue & 1) != 0))
            {
                res = Resources.SlaveAddressOddError;
            }
            
            return res;
        }

        private string GetSlaveAddressMaskError(byte slaveAddressMaskValue)
        {
            string errorMsg = string.Empty;
            byte min = CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MIN;
            byte max = CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MAX;
            bool isHex = m_txtSlaveAddressMask.Text.Contains(CyConvert.HEX_PREFIX);

            if ((slaveAddressMaskValue < min) || (slaveAddressMaskValue > max))
            {
                string minStr = isHex ? CyConvert.IntToHex(min) : min.ToString();
                string maxStr = isHex ? CyConvert.IntToHex(max) : max.ToString();
                errorMsg = string.Format(Resources.I2CSlaveAddressMaskError, minStr, maxStr);
            }
            else if ((slaveAddressMaskValue & 1) != 0) // even value only accepted
            {
                errorMsg = Resources.SlaveAddressMaskOddError;
            }
            else if ((m_params.I2C_EnableWakeup == true) && ((slaveAddressMaskValue & 2) == 0))
            {
                errorMsg = Resources.SlaveAddressMaskBitOneError;
            }

            return errorMsg;
        }
        #endregion

        #region Slave Address Mask configuration
        private void binaryGridSlaveAddressMask_BitChanged(object sender, EventArgs e)
        {
            CyBinaryGrid currentBinaryGrid = (CyBinaryGrid)sender;

            if (currentBinaryGrid.Locked == false)
            {
                currentBinaryGrid.Locked = true; // lock binary box from updating after textbox update

                if (m_txtSlaveAddressMask.Text.StartsWith(CyConvert.HEX_PREFIX))
                {
                    // Hexadecimal value handling
                    m_txtSlaveAddressMask.Text = CyConvert.BinToHex(currentBinaryGrid.Bits);
                }
                else
                {
                    // Decimal value handling
                    m_txtSlaveAddressMask.Text = CyConvert.BinToInt(currentBinaryGrid.Bits).ToString();
                }
                currentBinaryGrid.Locked = false; // release lock
            }
            UpdateAddressAccordingToAddressMask();
        }

        private void UpdateAddressAccordingToAddressMask()
        {
            // Show X as a bit of slave address when appropriate mask bit equal to 0
            // Create value in temporrary array
            string[] tmpBits = new string[m_binaryGridSlaveAddress.Bits.Length];

            for (int i = 0; i < tmpBits.Length; i++)
            {
                tmpBits[i] = m_binaryGridSlaveAddress.Bits[i];
            }

            for (int i = 0; i < tmpBits.Length; i++)
            {
                tmpBits[i] = (m_binaryGridSlaveAddressMask.Bits[i] == CyBinaryGrid.ZeroSign) ? CyBinaryGrid.XSign :
                    (m_binaryGridSlaveAddress.Bits[i] != CyBinaryGrid.XSign ? m_binaryGridSlaveAddress.Bits[i] :
                    CyBinaryGrid.ZeroSign);
            }
            // In 7-bit value LSB is always X
            if (m_binaryGridSlaveAddress.NumberOfBits == 7)
                tmpBits[m_binaryGridSlaveAddress.NumberOfBits] = CyBinaryGrid.XSign;

            // Do not update slave address value according to slave address mask,
            // only view of control should be changed
            m_binaryGridSlaveAddress.UpdateGridViewOnly(tmpBits);
        }

        private void tbSlaveAddressMask_TextChanged(object sender, EventArgs e)
        {
            TextBox currentTextBox = (TextBox)sender;

            #region Communications between textbox and binary grid
            if (m_binaryGridSlaveAddressMask.Locked == false)
            {
                m_binaryGridSlaveAddressMask.Locked = true; // lock textbox from updating after binary box update

                if (currentTextBox.Text.Contains(CyConvert.HEX_PREFIX))
                {
                    // Hexadecimal value handling
                    int value = CyConvert.HexToInt(currentTextBox.Text);
                    if (value >= CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MIN && value <= 
                        CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MAX)                   
                    {
                        m_binaryGridSlaveAddressMask.Bits = CyConvert.HexToBin(currentTextBox.Text);
                    }
                }
                else
                {
                    // Decimal value handling
                    int value = 0;
                    if (int.TryParse(currentTextBox.Text, out value))
                    {
                        if (value < CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MIN ||
                            value > CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MAX)
                            value = 0;
                    }
                    m_binaryGridSlaveAddressMask.Bits = CyConvert.IntToBin(value);
                }
                m_binaryGridSlaveAddressMask.Locked = false; // release lock

                UpdateAddressAccordingToAddressMask();
            }
            #endregion

            #region Set control value to appropriate parameter
            try
            {
                byte textboxValue = 0;
                string min, max;
                bool isHex = false;

                if (currentTextBox.Text.Contains(CyConvert.HEX_PREFIX))
                {
                    // Value was input as hexadecimal
                    textboxValue = (byte)CyConvert.HexToInt(currentTextBox.Text);
                    min = CyConvert.IntToHex(CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MIN);
                    max = CyConvert.IntToHex(CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MAX);
                    isHex = true;
                }
                else
                {
                    // Value was input as decimal
                    textboxValue = byte.Parse(currentTextBox.Text);
                    min = CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MIN.ToString();
                    max = CyParamRanges.I2C_SLAVE_ADDRESS_MASK_MAX.ToString();
                    isHex = false;
                }

                string errorMessage = GetSlaveAddressMaskError(textboxValue);
                if (string.IsNullOrEmpty(errorMessage) == false) throw new Exception(errorMessage);

                m_params.I2C_SlaveAddressMask = textboxValue;
                m_params.I2C_IsSlaveAddressMaskHex = isHex;

                m_errorProvider.SetError(currentTextBox, string.Empty); 

            }
            catch (Exception ex)
            {
                m_errorProvider.SetError(currentTextBox, ex.Message);
            }
            #endregion
        }
        #endregion

        #region Other controls configuration
        private void CyI2CTab_Load(object sender, EventArgs e)
        {
            UpdateAddressAccordingToAddressMask();
            UpdateClock();
        }

        private void m_cbMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender != m_cbMode)
            {
                return;
            }

            if (m_params.GlobalEditMode)
            {
                m_params.m_i2cMode.Description = m_cbMode.Text;
            }

            bool hasMaster = m_params.m_i2cMode.Value != CyEI2CModeType.I2C_SLAVE;
            bool isSlave = m_params.m_i2cMode.Value == CyEI2CModeType.I2C_SLAVE ||
                m_params.m_i2cMode.Value == CyEI2CModeType.I2C_MULTI_MASTER_SLAVE;

            m_panelSlaveConf.Enabled = isSlave;

            m_numOversamplingFactorHigh.Enabled = hasMaster;
            m_numOversamplingFactorLow.Enabled = hasMaster;
            m_lblOversamplingFactorLow.Enabled = hasMaster;
            m_lblOversamplingFactorHigh.Enabled = hasMaster;

            if (isSlave == false)
            {
                m_chbAcceptAddress.Checked = false;
                m_chbEnableWakeup.Checked = false;
            }

            UpdateClock();
        }

        private void m_cbDataRate_TextChanged(object sender, EventArgs e)
        {
            if (sender != m_cbDataRate)
            {
                return;
            }

            double dataRateValue = 0;
            bool parsed = double.TryParse(m_cbDataRate.Text, out dataRateValue);

            if (m_params.GlobalEditMode && parsed)
            {
                m_params.I2C_DataRate = (UInt16)dataRateValue;
            }

            if (parsed && dataRateValue >= CyParamRanges.I2C_DATA_RATE_MIN &&
                dataRateValue <= CyParamRanges.I2C_DATA_RATE_MAX)
            {
                m_errorProvider.SetError(m_cbDataRate, string.Empty);
                UpdateClock();
            }
            else
            {
                m_errorProvider.SetError(m_cbDataRate, string.Format(Resources.DataRateValueError,
                    CyParamRanges.I2C_DATA_RATE_MIN, CyParamRanges.I2C_DATA_RATE_MAX));
            }
        }

        private void m_chb_CheckedChanged(object sender, EventArgs e)
        {
            if (sender == m_chbAcceptAddress)
            {
                if (m_params.GlobalEditMode)
                {
                    m_params.I2C_AcceptAddress = m_chbAcceptAddress.Checked;
                }
            }
            else if (sender == m_chbClockFromTerminal)
            {
                if (m_params.GlobalEditMode)
                {
                    m_params.I2C_ClockFromTerm = m_chbClockFromTerminal.Checked;
                }
                UpdateClock();
            }
            else if (sender == m_chbEnableMedianFilter)
            {
                m_params.I2C_EnableMedianFilter = m_chbEnableMedianFilter.Checked;

                numOversamplingFactor_TextChanged(m_numOversamplingFactor, new CancelEventArgs());
                numOversamplingFactorLow_Validating(m_numOversamplingFactorLow, new CancelEventArgs());
            }
            else if (sender == m_chbEnableWakeup)
            {

                m_params.I2C_EnableWakeup = m_chbEnableWakeup.Checked;

                if (m_params.I2C_EnableWakeup)
                {
                    m_chbEnableMedianFilter.Enabled = false;
                    m_chbEnableMedianFilter.Checked = false;
                }
                else
                {
                    m_chbEnableMedianFilter.Enabled = true;
                }

                UpdateValidators();

                numOversamplingFactor_TextChanged(m_numOversamplingFactor, new EventArgs());
                tbSlaveAddress_TextChanged(m_txtSlaveAddress, null);
                tbSlaveAddressMask_TextChanged(m_txtSlaveAddressMask, null);
            }
        }

        private void numOversamplingFactor_TextChanged(object sender, EventArgs e)
        {
            NumericUpDown currentTextBox = ((NumericUpDown)sender);
            byte ovsFactorValue = 0;
            byte.TryParse(currentTextBox.Text, out ovsFactorValue);

            if (m_params.GlobalEditMode)
            {
                if (ovsFactorValue < CyParamRanges.GetI2COvsFactorMinimum(m_params.I2C_EnableMedianFilter) ||
                    ovsFactorValue > CyParamRanges.I2C_OVS_FACTOR_MAX)
                {
                    m_errorProvider.SetError(currentTextBox, GetOvsFactorErrorMessage());
                }
                else
                {
                    m_params.I2C_OvsFactor = ovsFactorValue;
                    UpdateClock();
                    m_errorProvider.SetError(currentTextBox, string.Empty);

                    if (ovsFactorValue != m_previousOvsFactor) // check if I2COvsFactor was changed
                    {
                        m_previousOvsFactor = ovsFactorValue;
                        // Oversampling Factor sets the value of Oversampling Low and High dividing by to equal parts.  
                        // In case of odd value the Oversampling Low is greater by 1.
                        m_numericLocked = true; // lock numeric update to avoid infinity loop
                        if ((ovsFactorValue % 2) == 0)
                        {
                            m_numOversamplingFactorLow.Value = ovsFactorValue / 2 + 1;
                            m_numOversamplingFactorHigh.Value = ovsFactorValue / 2 - 1;
                        }
                        else
                        {
                            m_numOversamplingFactorLow.Value = Math.Ceiling((decimal)ovsFactorValue / 2);
                            m_numOversamplingFactorHigh.Value = Math.Floor((decimal)ovsFactorValue / 2);
                        }
                        m_numericLocked = false; // release lock

                        // Set numeric controls values that were changed in consequence of I2COvsFactor change
                        m_params.I2C_OvsFactorLow = (byte)m_numOversamplingFactorLow.Value;
                        m_params.I2C_OvsFactorHigh = (byte)m_numOversamplingFactorHigh.Value;
                    }
                }
            }

            // Call this to update OvsFactorLow and OvsFactorHigh error providers
            numOversamplingFactorLow_Validating(m_numOversamplingFactorLow, new CancelEventArgs());
        }

        private void numOversamplingFactorLow_ValueChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (m_numericLocked == false)
                {
                    m_numericLocked = true;

                    NumericUpDown master = (NumericUpDown)sender;
                    NumericUpDown slave = (master == m_numOversamplingFactorHigh) ? m_numOversamplingFactorLow :
                        m_numOversamplingFactorHigh;

                    if ((m_params.I2C_OvsFactorLow + m_params.I2C_OvsFactorHigh) == m_params.I2C_OvsFactor)
                    {
                        byte previousValue = (master == m_numOversamplingFactorHigh) ? m_previousOvsFactorHigh :
                            m_previousOvsFactorLow;

                        if (master.Value > previousValue)
                        {
                            if (slave.Value - 1 >= slave.Minimum)
                            {
                                slave.Value--;
                            }
                        }
                        if (master.Value < previousValue)
                        {
                            if (slave.Value + 1 <= slave.Maximum)
                            {
                                slave.Value++;
                            }
                        }


                    }
                    m_params.I2C_OvsFactorLow = (byte)m_numOversamplingFactorLow.Value;
                    m_params.I2C_OvsFactorHigh = (byte)m_numOversamplingFactorHigh.Value;
                    m_numericLocked = false;
                }

                // Save current values to know if the value of numeric controls was changed next time
                m_previousOvsFactorLow = (byte)m_numOversamplingFactorLow.Value;
                m_previousOvsFactorHigh = (byte)m_numOversamplingFactorHigh.Value;

                // Call this to update OvsFactorLow and OvsFactorHigh error providers
                numOversamplingFactorLow_Validating(m_numOversamplingFactorLow, new CancelEventArgs());
            }
        }

        private void numOversamplingFactorLow_Validating(object sender, CancelEventArgs e)
        {
            // Compare tLow/tHigh values with I2C spec values and update error providers
            UpdateTLowTHigh();

            if (m_numOversamplingFactorLow.Value <
                CyParamRanges.GetI2COvsLowMinimum(m_params.I2C_EnableMedianFilter))
                m_errorProvider.SetError(m_numOversamplingFactorLow, GetOvsLowErrorMessage());
            else
                m_errorProvider.SetError(m_numOversamplingFactorLow, string.Empty);

            if (m_numOversamplingFactorHigh.Value < CyParamRanges.GetI2COvsHighMinimum(
                m_params.I2C_EnableMedianFilter))
            {
                m_errorProvider.SetError(m_numOversamplingFactorHigh, GetOvsHighErrorMessage());
            }
            else
            {
                m_errorProvider.SetError(m_numOversamplingFactorHigh, string.Empty);
            }

            // Check if value was changed from Expression view
            if ((m_params.I2C_OvsFactorLow + m_params.I2C_OvsFactorHigh) != m_params.I2C_OvsFactor)
            {
                m_errorProvider.SetError(m_numOversamplingFactorLow, Resources.OvsLowOutOfRangeError);
                m_errorProvider.SetError(m_numOversamplingFactorHigh, Resources.OvsHighOutOfRangeError);
            }
        }
        #endregion
        #endregion

        #region Building error messages
        private string GetOvsFactorErrorMessage()
        {
            byte min = CyParamRanges.GetI2COvsFactorMinimum(m_params.I2C_EnableMedianFilter);
            byte max = CyParamRanges.I2C_OVS_FACTOR_MAX;

            return m_params.I2C_EnableMedianFilter ? string.Format(Resources.OvsFactorFilterEnabledError, min, max) :
                string.Format(Resources.OvsFactorFilterDisabledError, min, max);
        }

        private string GetOvsLowErrorMessage()
        {
            byte min = CyParamRanges.GetI2COvsLowMinimum(m_params.I2C_EnableMedianFilter);

            return m_params.I2C_EnableMedianFilter ? string.Format(Resources.OvsLowFilterEnabledError, min) :
                string.Format(Resources.OvsLowFilterDisabledError, min);
        }

        private string GetOvsHighErrorMessage()
        {
            byte min = CyParamRanges.GetI2COvsHighMinimum(m_params.I2C_EnableMedianFilter);

            return m_params.I2C_EnableMedianFilter ? string.Format(Resources.OvsHighFilterEnabledError, min) :
                string.Format(Resources.OvsHighFilterDisabledError, min);
        }
        #endregion
    }
}
