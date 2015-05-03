using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace SCB_P4_v99_0
{
    public partial class CyEZI2CTab : CyTabControlWrapper
    {
        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "EZI2C Basic";
            }
        }
        #endregion

        #region Constructor(s)
        public CyEZI2CTab(CyParameters parameters)
            :base(parameters)
        {
            m_params = parameters;
            m_params.m_ezI2CTab = this;

            InitializeComponent();

            m_cbSubAddressSize.Items.Clear();
            m_cbSubAddressSize.Items.AddRange(m_params.m_ezI2CSubAddressSize.EnumDescriptionsList);

            #region Assigning event handlers
            m_chbClockStretching.CheckedChanged += new EventHandler(m_chbCheckedChanged);
            m_chbClockFromTerminal.CheckedChanged += new EventHandler(m_chbCheckedChanged);
            m_chbEnableMedianFilter.CheckedChanged += new EventHandler(m_chbCheckedChanged);
            m_chbEnableWakeup.CheckedChanged += new EventHandler(m_chbCheckedChanged);

            m_cbDataRate.TextChanged += new EventHandler(m_cbDataRate_TextChanged);
            m_cbDataRate.EnabledChanged += new EventHandler(m_cbDataRate_TextChanged);
            m_cbNumberOfAddresses.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbSubAddressSize.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);

            m_numOversamplingFactor.TextChanged += new EventHandler(m_numOversamplingFactorTextChanged);
            m_txtPrimarySlaveAddress.TextChanged += new EventHandler(m_txtSlaveAddressTextChanged);
            m_txtSecondarySlaveAddress.TextChanged += new EventHandler(m_txtSlaveAddressTextChanged);
            #endregion
        }
        #endregion

        #region Update UI
        public override void UpdateUI()
        {
            m_chbClockFromTerminal.Checked = m_params.EZI2C_ClockFromTerminal;
            m_chbClockStretching.Checked = m_params.EZI2C_StretchClock;
            m_chbEnableMedianFilter.Checked = m_params.EZI2C_EnableMedianFilter;
            m_chbEnableWakeup.Checked = m_params.EZI2C_EnableWakeupFromSleep;

            m_numOversamplingFactor.Text = m_params.EZI2C_OversamplingFactor.ToString();

            m_cbDataRate.Text = m_params.EZI2C_DataRate.ToString();

            m_txtPrimarySlaveAddress.Text = m_params.EZI2C_PrimarySlaveAddressStr;
            m_txtSecondarySlaveAddress.Text = m_params.EZI2C_SecondarySlaveAddressStr;
            m_cbNumberOfAddresses.SelectedItem = m_params.m_ezI2CNumberOfAddresses.Description;
            m_cbSubAddressSize.SelectedItem = m_params.m_ezI2CSubAddressSize.Description;

            UpdateSlaveAddressesConfig();
            UpdateClock();
            UpdateEnableWakeUpEnable();
            UpdateMedianFilterOnEnableWakeUp();

            //To update error provider for m_cbDataRate
            m_cbDataRate_TextChanged(m_cbDataRate, new EventArgs());
        }

        private void UpdateSlaveAddressesConfig()
        {
            if (m_params.EZI2C_StretchClock == false)
            {
                m_cbNumberOfAddresses.Text = "1";
                m_cbNumberOfAddresses.Enabled = false;
            }
            else
            {
                m_cbNumberOfAddresses.Enabled = true;
            }

            m_lblSecondarySlaveAddress.Enabled = m_params.EZI2C_StretchClock &&
                (m_params.m_ezI2CNumberOfAddresses.Value != CyEEZI2CNumberOfAddressesType.ONE_ADDRESS);
            m_txtSecondarySlaveAddress.Enabled = m_params.EZI2C_StretchClock &&
                (m_params.m_ezI2CNumberOfAddresses.Value != CyEEZI2CNumberOfAddressesType.ONE_ADDRESS);
            m_txtSlaveAddressTextChanged(m_txtSecondarySlaveAddress, null);
        }
        #endregion

        #region Event handlers
        private void m_chbCheckedChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_chbClockStretching)
                {
                    m_params.EZI2C_StretchClock = m_chbClockStretching.Checked;

                    UpdateSlaveAddressesConfig();
                    UpdateEnableWakeUpEnable();

                    //To update error provider for m_cbDataRate
                    m_cbDataRate_TextChanged(m_cbDataRate, new EventArgs());
                }
                else if (sender == m_chbClockFromTerminal)
                {
                    m_params.EZI2C_ClockFromTerminal = m_chbClockFromTerminal.Checked;
                    UpdateClock();
                }
                else if (sender == m_chbEnableMedianFilter)
                {
                    m_params.EZI2C_EnableMedianFilter = m_chbEnableMedianFilter.Checked;
                    m_numOversamplingFactorTextChanged(m_numOversamplingFactor, new EventArgs());
                }
                else if (sender == m_chbEnableWakeup)
                {
                    m_params.EZI2C_EnableWakeupFromSleep = m_chbEnableWakeup.Checked;
                    UpdateMedianFilterOnEnableWakeUp();
                    m_txtSlaveAddressTextChanged(m_txtPrimarySlaveAddress, new EventArgs());
                }
            }
        }

        private void m_cbDataRate_TextChanged(object sender, EventArgs e)
        {    
            if (sender != m_cbDataRate)
            {
                return;
            }

            double dataRateValue = 0;
            bool parsed = double.TryParse(m_cbDataRate.Text, out dataRateValue);

            if (parsed && m_params.GlobalEditMode)
            {
                m_params.EZI2C_DataRate = (UInt16)dataRateValue;
            }
            int max = m_params.EZI2C_StretchClock ? CyParamRanges.I2C_DATA_RATE_MAX :
                CyParamRanges.I2C_DATA_RATE_NON_ClOCK_STRETCHING_MAX;
            string error = m_params.EZI2C_StretchClock ? Resources.DataRateValueError :
                Resources.DataRateValueErrorNonClockStretching;

            if ((m_cbDataRate.Enabled == false) || (parsed && dataRateValue >= CyParamRanges.I2C_DATA_RATE_MIN &&
                dataRateValue <= max))
            {
                m_errorProvider.SetError(m_cbDataRate, string.Empty);
                UpdateClock();
            }
            else
            {
                m_errorProvider.SetError(m_cbDataRate, string.Format(error, CyParamRanges.I2C_DATA_RATE_MIN, max));
            }
        }

        private void m_cb_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_cbSubAddressSize)
                {
                    m_params.m_ezI2CSubAddressSize.Description = m_cbSubAddressSize.Text;
                }
                else if (sender == m_cbNumberOfAddresses)
                {
                    m_params.m_ezI2CNumberOfAddresses.Description = m_cbNumberOfAddresses.Text;
                    UpdateSlaveAddressesConfig();
                }
            }
        }

        private void m_numOversamplingFactorTextChanged(object sender, EventArgs e)
        {
            if (sender != m_numOversamplingFactor)
            {
                return;
            }

            byte minValue = CyParamRanges.GetI2COvsFactorMinimum(m_params.EZI2C_EnableMedianFilter);
            byte maxValue = CyParamRanges.I2C_OVS_FACTOR_MAX;
            byte value;

            bool parsed = byte.TryParse(m_numOversamplingFactor.Text, out value);

            if (parsed && m_params.GlobalEditMode)
            {
                m_params.EZI2C_OversamplingFactor = value;
            }

            if (parsed && value >= minValue && value <= maxValue)
            {
                m_errorProvider.SetError(m_numOversamplingFactor, string.Empty);

                if (m_params.GlobalEditMode)
                {
                    UpdateClock();
                }
            }
            else
            {
                m_errorProvider.SetError(m_numOversamplingFactor, string.Format(Resources.UARTOvsFactorError,
                    minValue, maxValue));
            }
        }

        private void m_txtSlaveAddressTextChanged(object sender, EventArgs e)
        {
            if (sender == m_txtPrimarySlaveAddress || sender == m_txtSecondarySlaveAddress)
            {
                Control currentControl = sender as Control;
                Control otherControl = (currentControl == m_txtPrimarySlaveAddress) ?
                    m_txtSecondarySlaveAddress : m_txtPrimarySlaveAddress;
                string currErrorStr = GetSlaveAddressValue(currentControl);
                string otherErrorStr = GetSlaveAddressValue(otherControl);
                string oddValueErrorStr1 = GetSlaveAddressError(m_params.EZI2C_PrimarySlaveAddress);
                string oddValueErrorStr2 = GetSlaveAddressError(m_params.EZI2C_SecondarySlaveAddress);

                if (m_txtSecondarySlaveAddress.Enabled && string.IsNullOrEmpty(currErrorStr) &&
                    string.IsNullOrEmpty(otherErrorStr) &&
                    (m_params.EZI2C_PrimarySlaveAddress == m_params.EZI2C_SecondarySlaveAddress))
                {
                    m_errorProvider.SetError(currentControl, Resources.EZI2CSlaveAddressesEqualError);
                    m_errorProvider.SetError(otherControl, Resources.EZI2CSlaveAddressesEqualError);
                }
                else
                {
                    m_errorProvider.SetError(currentControl, currErrorStr);
                    m_errorProvider.SetError(otherControl, otherErrorStr);
                }

                if (String.IsNullOrEmpty(m_errorProvider.GetError(m_txtPrimarySlaveAddress)))
                {
                    m_errorProvider.SetError(m_txtPrimarySlaveAddress, oddValueErrorStr1);
                }
                if (m_txtSecondarySlaveAddress.Enabled &&
                    String.IsNullOrEmpty(m_errorProvider.GetError(m_txtSecondarySlaveAddress)))
                {
                    m_errorProvider.SetError(m_txtSecondarySlaveAddress, oddValueErrorStr2);
                }
            }
        }
        #endregion

        #region Auxiliary methods
        /// <summary>
        /// Parse slave address value from the Text property of the control. If the value is successfully parsed, it is
        /// stored in symbol.
        /// </summary>
        /// <param name="control">Control to parse value from.</param>
        /// <returns>Error string on parsing error, or empty string if parsed successfully.</returns>
        private string GetSlaveAddressValue(Control control)
        {
            // Don't check Secondary Slave Address if its control is disabled
            if ((control == m_txtSecondarySlaveAddress) && (control.Enabled == false))
            {
                return string.Empty;
            }

            byte value = 0;
            bool isValueHex = control.Text.ToLower().StartsWith(CyParameters.HEX_PREFIX);

            string stringToParse = control.Text;
            if (isValueHex)
            {
                // Cut off the hexadecimal display prefix ("0x")
                stringToParse = control.Text.Remove(0, CyParameters.HEX_PREFIX.Length);
            }

            // Parse value with 10 or 16 base
            bool parsed = true;
            try
            {
                value = Convert.ToByte(stringToParse, isValueHex ? 16 : 10);
            }
            catch
            {
                parsed = false;
            }

            // If value's parsed then set the value to the symbol
            if (parsed)
            {
                if (control == m_txtPrimarySlaveAddress)
                {
                    m_params.EZI2C_PrimarySlaveAddress = value;
                    m_params.EZI2C_IsPrimarySlaveAddressHex = isValueHex;
                }
                else if (control == m_txtSecondarySlaveAddress)
                {
                    m_params.EZI2C_SecondarySlaveAddress = value;
                    m_params.EZI2C_IsSecondarySlaveAddressHex = isValueHex;
                }
            }

            byte min = CyParamRanges.I2C_SLAVE_ADDRESS_MIN;
            byte max = CyParamRanges.I2C_SLAVE_ADDRESS_MAX;

            return ((parsed == false) || (value < min) || (value > max)) ?
                string.Format(Resources.EZI2CSlaveAddressError, min, max) : string.Empty;
        }

        private string GetSlaveAddressError(byte slaveAddressValue)
        {
            string res = string.Empty;
            if ((m_params.EZI2C_EnableWakeupFromSleep == true) && ((slaveAddressValue & 1) != 0))
            {
                res = Resources.SlaveAddressOddError;
            }

            return res;
        }

        private void UpdateEnableWakeUpEnable()
        {
            if (m_params.EZI2C_StretchClock == false)
            {
                m_chbEnableWakeup.Enabled = false;
                m_chbEnableWakeup.Checked = false;
            }
            else
            {
                m_chbEnableWakeup.Enabled = true;
            }
        }

        private void UpdateMedianFilterOnEnableWakeUp()
        {
            if (m_params.EZI2C_EnableWakeupFromSleep == true)
            {
                m_chbEnableMedianFilter.Enabled = false;
                m_chbEnableMedianFilter.Checked = false;
            }
            else
            {
                m_chbEnableMedianFilter.Enabled = true;
            }
        }
        #endregion

        #region Actual data rate
        public void UpdateClock()
        {
            double clkFreq = m_params.EZI2C_ClockFromTerminal ?
                CyParameters.GetExternalClock(m_params.m_term) :
                CyParameters.GetInternalClock(m_params.m_term);

            string dataRateStr = Resources.ClockDisplayUnknown;

            m_cbDataRate.Enabled = !m_params.EZI2C_ClockFromTerminal;
            m_lblDataRate.Enabled = !m_params.EZI2C_ClockFromTerminal;

            if (clkFreq > 0)
            {

                double actualDataRate = Math.Round(clkFreq / m_params.EZI2C_OversamplingFactor, 3);
                if (actualDataRate > 0)
                {
                    dataRateStr = actualDataRate.ToString();
                }
            }

            // Update Actual Data Rate label
            m_lblActualDataRate.Text = string.Format(Resources.ActualDataRateDisplayFormat, dataRateStr);
        }
        #endregion
    }
}
