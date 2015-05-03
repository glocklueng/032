/*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
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

namespace SCB_P4_v99_0
{
    public partial class CySPIAdvancedTab : CyTabControlWrapper
    {
        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "SPI Advanced";
            }
        }
        #endregion

        #region Constructor(s)
        public CySPIAdvancedTab(CyParameters parameters)
            : base(parameters)
        {
            m_params.m_spiAdvancedTab = this;

            InitializeComponent();

            // Initialize comboboxes' items
            for (int i = 0; i <= 7; i++)
            {
                m_cbRXTriggerLevel.Items.Add(i);
                m_cbTXTriggerLevel.Items.Add(i);
            }

            #region Event handler assigning
            m_chbInterruptSPIDone.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXNotFull.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXEmpty.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXOverflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXUnderflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXTrigger.CheckedChanged += new EventHandler(m_chb_CheckedChanged);

            m_chbInterruptSPIBusError.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXNotEmpty.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXFull.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXOverflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXUnderflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXTrigger.CheckedChanged += new EventHandler(m_chb_CheckedChanged);

            m_cbRXTriggerLevel.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbTXTriggerLevel.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);

            m_rbNone.CheckedChanged += new EventHandler(m_rb_CheckedChanged);
            m_rbInternal.CheckedChanged += new EventHandler(m_rb_CheckedChanged);
            m_rbExternal.CheckedChanged += new EventHandler(m_rb_CheckedChanged);

            m_numRXBufferSize.Validating += new CancelEventHandler(m_numBufferSize_Validating);
            m_numRXBufferSize.TextChanged += new EventHandler(m_numBufferSize_TextChanged);

            m_numTXBufferSize.Validating += new CancelEventHandler(m_numBufferSize_Validating);
            m_numTXBufferSize.TextChanged += new EventHandler(m_numBufferSize_TextChanged);
            #endregion

            m_numRXBufferSize.Maximum = decimal.MaxValue;
            m_numRXBufferSize.Minimum = 0;

            m_numTXBufferSize.Maximum = decimal.MaxValue;
            m_numTXBufferSize.Minimum = 0;
        }
        #endregion

        #region Update UI
        public override void UpdateUI()
        {
            if (m_params == null)
                return;

            m_numRXBufferSize.Text = m_params.SPI_RxBufferSize.ToString();
            m_numTXBufferSize.Text = m_params.SPI_TxBufferSize.ToString();

            m_chbInterruptSPIBusError.Checked = m_params.SPI_InterruptOnSPIBusError;
            m_chbInterruptRXFull.Checked = m_params.SPI_InterruptOnRXFull;
            m_chbInterruptRXNotEmpty.Checked = m_params.SPI_InterruptOnRXNotEmpty;
            m_chbInterruptRXOverflow.Checked = m_params.SPI_InterruptOnRXOverflow;
            m_chbInterruptRXTrigger.Checked = m_params.SPI_InterruptOnRXTrigger;
            m_chbInterruptRXUnderflow.Checked = m_params.SPI_InterruptOnRXUnderflow;

            m_chbInterruptSPIDone.Checked = m_params.SPI_InterruptOnSPIDone;
            m_chbInterruptTXEmpty.Checked = m_params.SPI_InterruptOnTXEmpty;
            m_chbInterruptTXNotFull.Checked = m_params.SPI_InterruptOnTXNotFull;
            m_chbInterruptTXOverflow.Checked = m_params.SPI_InterruptOnTXOverflow;
            m_chbInterruptTXTrigger.Checked = m_params.SPI_InterruptOnTXTrigger;
            m_chbInterruptTXUnderflow.Checked = m_params.SPI_InterruptOnTXUnderflow;

            m_cbRXTriggerLevel.SelectedIndex = m_params.SPI_RxBufferTriggerLevel;
            m_cbTXTriggerLevel.SelectedIndex = m_params.SPI_TxBufferTriggerLevel;

            switch (m_params.SPI_InterruptMode)
            {
                case CyEInterruptModeType.INTERRUPT_NONE:
                    m_rbNone.Checked = true;
                    break;
                case CyEInterruptModeType.INTERNAL:
                    m_rbInternal.Checked = true;
                    break;
                case CyEInterruptModeType.EXTERNAL:
                    m_rbExternal.Checked = true;
                    break;
                default:
                    break;
            }

            UpdateInterruptsVisibility();
            UpdateInterruptSources();
        }

        public void UpdateInterruptSources()
        {
            m_chbInterruptSPIBusError.Checked = m_params.m_spiMode.Value == CyESPIModeType.SPI_SLAVE &&
                m_params.SPI_InterruptOnSPIBusError;
            m_chbInterruptSPIDone.Checked = m_params.m_spiMode.Value == CyESPIModeType.SPI_MASTER &&
                m_params.SPI_InterruptOnSPIDone;
        }

        public void UpdateInterruptsVisibility()
        {
            bool bufSizeOverflow = (m_params.SPI_RxBufferSize > CyParamRanges.SPI_RX_BUFFER_MIN_SIZE) ||
                (m_params.SPI_TxBufferSize > CyParamRanges.SPI_TX_BUFFER_MIN_SIZE);

            m_rbNone.Enabled = !bufSizeOverflow;
            m_rbExternal.Enabled = !bufSizeOverflow;
            m_rbInternal.Checked = bufSizeOverflow || m_params.SPI_InterruptMode == CyEInterruptModeType.INTERNAL;

            bool interruptEnabled = m_params.SPI_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE;

            m_chbInterruptSPIDone.Enabled = interruptEnabled && (m_params.m_spiMode.Value == CyESPIModeType.SPI_MASTER);
            m_chbInterruptSPIBusError.Enabled = interruptEnabled && (m_params.m_spiMode.Value == CyESPIModeType.SPI_SLAVE);
            m_chbInterruptTXNotFull.Enabled = interruptEnabled &&
                (m_params.SPI_TxBufferSize == CyParamRanges.SPI_TX_BUFFER_MIN_SIZE);
            m_chbInterruptTXEmpty.Enabled = interruptEnabled;
            m_chbInterruptTXOverflow.Enabled = interruptEnabled;
            m_chbInterruptTXUnderflow.Enabled = interruptEnabled;
            m_chbInterruptTXTrigger.Enabled = interruptEnabled;

            m_chbInterruptRXNotEmpty.Enabled = interruptEnabled &&
                (m_params.SPI_RxBufferSize == CyParamRanges.SPI_RX_BUFFER_MIN_SIZE);
            m_chbInterruptRXFull.Enabled = interruptEnabled;
            m_chbInterruptRXOverflow.Enabled = interruptEnabled;
            m_chbInterruptRXUnderflow.Enabled = interruptEnabled;
            m_chbInterruptRXTrigger.Enabled = interruptEnabled;

            m_cbRXTriggerLevel.Enabled = interruptEnabled && m_params.SPI_InterruptOnRXTrigger;
            m_cbTXTriggerLevel.Enabled = interruptEnabled && m_params.SPI_InterruptOnTXTrigger;

            if (m_params.SPI_RxBufferSize > CyParamRanges.SPI_RX_BUFFER_MIN_SIZE)
            {
                m_chbInterruptRXNotEmpty.Enabled = false;
            }

            if (m_params.SPI_TxBufferSize > CyParamRanges.SPI_TX_BUFFER_MIN_SIZE)
            {
                m_chbInterruptTXNotFull.Enabled = false;
            }
        }
        #endregion

        #region Event handlers
        void m_numBufferSize_TextChanged(object sender, EventArgs e)
        {
            CancelEventArgs ce = new CancelEventArgs();
            m_numBufferSize_Validating(sender, ce);
            if (!ce.Cancel)
            {
                UInt32 val;
                UInt32.TryParse((sender as NumericUpDown).Text, out val);

                if (sender == m_numRXBufferSize)
                {
                    if (val == CyParamRanges.SPI_RX_BUFFER_MIN_SIZE && m_params.SPI_RxBufferSize > val)
                    {
                        m_chbInterruptRXNotEmpty.Checked = false;
                    }

                    m_params.SPI_RxBufferSize = val;

                    if (m_params.SPI_RxBufferSize > CyParamRanges.SPI_RX_BUFFER_MIN_SIZE)
                    {                        
                        m_chbInterruptRXNotEmpty.Checked = true;
                    }
                }
                if (sender == m_numTXBufferSize)
                {
                    m_params.SPI_TxBufferSize = val;

                    if (m_params.SPI_TxBufferSize > CyParamRanges.SPI_TX_BUFFER_MIN_SIZE)
                    {                        
                        m_chbInterruptTXNotFull.Checked = false;
                    }
                }
            }
            UpdateInterruptsVisibility();
        }

        void m_numBufferSize_Validating(object sender, CancelEventArgs e)
        {
            bool error = false;
            string message = "";
            if (sender is NumericUpDown == false) return;
            NumericUpDown cnt = sender as NumericUpDown;

            UInt32 min = CyParameters.SPI_MIN_TX_BUFFER_SIZE;
            UInt32 max = UInt32.MaxValue;
            try
            {
                UInt32 val = Convert.ToUInt32(cnt.Text);

                if (val < min || val > max) throw new Exception();
            }
            catch
            {
                message = string.Format(Resources.ErrorValueLimitBufferSize, min);
                error = true;
            }

            if (error)
            {
                m_errorProvider.SetError((Control)sender, string.Format(message));
            }
            else
            {
                m_errorProvider.SetError((Control)sender, string.Empty);
            }
        }

        void m_chb_CheckedChanged(object sender, EventArgs e)
        {
            if (sender == m_chbInterruptSPIDone)
            {
                m_params.SPI_InterruptOnSPIDone = m_chbInterruptSPIDone.Checked;
            }
            else if (sender == m_chbInterruptTXNotFull)
            {
                m_params.SPI_InterruptOnTXNotFull = m_chbInterruptTXNotFull.Checked;
            }
            else if (sender == m_chbInterruptTXEmpty)
            {
                m_params.SPI_InterruptOnTXEmpty = m_chbInterruptTXEmpty.Checked;
            }
            else if (sender == m_chbInterruptTXOverflow)
            {
                m_params.SPI_InterruptOnTXOverflow = m_chbInterruptTXOverflow.Checked;
            }
            else if (sender == m_chbInterruptTXUnderflow)
            {
                m_params.SPI_InterruptOnTXUnderflow = m_chbInterruptTXUnderflow.Checked;
            }
            else if (sender == m_chbInterruptTXTrigger)
            {
                m_params.SPI_InterruptOnTXTrigger = m_chbInterruptTXTrigger.Checked;
                m_cbTXTriggerLevel.Enabled = m_chbInterruptTXTrigger.Checked;
            }
            else if (sender == m_chbInterruptSPIBusError)
            {
                m_params.SPI_InterruptOnSPIBusError = m_chbInterruptSPIBusError.Checked;
            }
            else if (sender == m_chbInterruptRXNotEmpty)
            {
                m_params.SPI_InterruptOnRXNotEmpty = m_chbInterruptRXNotEmpty.Checked;
            }
            else if (sender == m_chbInterruptRXFull)
            {
                m_params.SPI_InterruptOnRXFull = m_chbInterruptRXFull.Checked;
            }
            else if (sender == m_chbInterruptRXOverflow)
            {
                m_params.SPI_InterruptOnRXOverflow = m_chbInterruptRXOverflow.Checked;
            }
            else if (sender == m_chbInterruptRXUnderflow)
            {
                m_params.SPI_InterruptOnRXUnderflow = m_chbInterruptRXUnderflow.Checked;
            }
            else if (sender == m_chbInterruptRXTrigger)
            {
                m_params.SPI_InterruptOnRXTrigger = m_chbInterruptRXTrigger.Checked;
                m_cbRXTriggerLevel.Enabled = m_chbInterruptRXTrigger.Checked;
            }
        }

        void m_rb_CheckedChanged(object sender, EventArgs e)
        {
            if (m_rbNone.Checked)
            {
                m_params.SPI_InterruptMode = CyEInterruptModeType.INTERRUPT_NONE;

                #region Clear all interrupts
                m_chbInterruptSPIDone.Checked = false;
                m_chbInterruptTXNotFull.Checked = false;
                m_chbInterruptTXEmpty.Checked = false;
                m_chbInterruptTXOverflow.Checked = false;
                m_chbInterruptTXUnderflow.Checked = false;
                m_chbInterruptTXTrigger.Checked = false;

                m_chbInterruptSPIBusError.Checked = false;
                m_chbInterruptRXNotEmpty.Checked = false;
                m_chbInterruptRXFull.Checked = false;
                m_chbInterruptRXOverflow.Checked = false;
                m_chbInterruptRXUnderflow.Checked = false;
                m_chbInterruptRXTrigger.Checked = false;
                #endregion
            }
            else if (m_rbInternal.Checked)
            {
                m_params.SPI_InterruptMode = CyEInterruptModeType.INTERNAL;
            }
            else
            {
                m_params.SPI_InterruptMode = CyEInterruptModeType.EXTERNAL;
            }

            UpdateInterruptsVisibility();
            if (m_params.m_spiBasicTab != null)
            {
                m_params.m_spiBasicTab.UpdateCheckBoxesStates();
            }
        }

        void m_cb_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender == m_cbRXTriggerLevel)
            {
                m_params.SPI_RxBufferTriggerLevel = (byte)m_cbRXTriggerLevel.SelectedIndex;
            }
            else if (sender == m_cbTXTriggerLevel)
            {
                m_params.SPI_TxBufferTriggerLevel = (byte)m_cbTXTriggerLevel.SelectedIndex;
            }
        }
        #endregion
    }
}
