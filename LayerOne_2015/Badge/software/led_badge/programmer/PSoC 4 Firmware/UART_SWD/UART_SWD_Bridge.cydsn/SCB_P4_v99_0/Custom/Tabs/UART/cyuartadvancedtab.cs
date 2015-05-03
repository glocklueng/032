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

namespace SCB_P4_v99_0
{
    public partial class CyUARTAdvancedTab : CyTabControlWrapper
    {
        private readonly CheckBox[] m_txSources;
        private readonly CheckBox[] m_rxSources;

        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "UART Advanced";
            }
        }
        #endregion

        #region Constructor(s)
        public CyUARTAdvancedTab(CyParameters parameters)
            : base(parameters)
        {
            m_params = parameters;
            m_params.m_uartAdvancedTab = this;

            InitializeComponent();

            m_txSources = new CheckBox[] { m_chbInterruptUARTDone, m_chbInterruptTXEmpty, m_chbInterruptTXOverflow,
                m_chbInterruptTXUnderflow, m_chbInterruptTXTrigger };
            m_rxSources = new CheckBox[] { m_chbInterruptRXFull, m_chbInterruptRXOverflow, m_chbInterruptRXUnderflow,
                m_chbInterruptRXFrameErr, m_chbInterruptRXParityErr, m_chbInterruptRXTrigger };

            #region Add combobox ranges
            for (int level = 0; level < CyParamRanges.UART_MAX_TRIGGER_LEVEL; level++)
            {
                m_cbRXTriggerLevel.Items.Add(level.ToString());
                m_cbTXTriggerLevel.Items.Add(level.ToString());
            }
            #endregion

            #region Add event handlers
            m_numRXBufferSize.TextChanged += new EventHandler(m_numBufferSize_TextChanged);
            m_numTXBufferSize.TextChanged += new EventHandler(m_numBufferSize_TextChanged);

            m_chbInterruptUARTDone.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXNotFull.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXEmpty.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXOverflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXUnderflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXLostArb.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTxNack.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptTXTrigger.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXFull.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXNotEmpty.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXOverflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXUnderflow.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXFrameErr.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXParityErr.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbInterruptRXTrigger.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnMultiProcMode.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbAcceptAddress.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbDropOnParityError.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbDropOnFrameError.CheckedChanged += new EventHandler(m_chb_CheckedChanged);

            m_cbTXTriggerLevel.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbRXTriggerLevel.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);

            m_rbNone.CheckedChanged += new EventHandler(m_rbIntrMode_CheckedChanged);
            m_rbExternal.CheckedChanged += new EventHandler(m_rbIntrMode_CheckedChanged);
            m_rbInternal.CheckedChanged += new EventHandler(m_rbIntrMode_CheckedChanged);

            m_numAddress.TextChanged += new EventHandler(m_numAddress_TextChanged);
            m_numAddressMask.TextChanged += new EventHandler(m_numAddress_TextChanged);
            #endregion

            #region Initialize numerics
            m_numRXBufferSize.Maximum = decimal.MaxValue;
            m_numRXBufferSize.Minimum = 0;

            m_numTXBufferSize.Maximum = decimal.MaxValue;
            m_numTXBufferSize.Minimum = 0;

            m_numAddress.Maximum = decimal.MaxValue;
            m_numAddress.Minimum = 0;

            m_numAddressMask.Maximum = decimal.MaxValue;
            m_numAddressMask.Minimum = 0;
            #endregion
        }
        #endregion

        #region Update UI
        public override void UpdateUI()
        {
            // Buffer sizes
            m_numRXBufferSize.Text = m_params.UART_RXBufferSize.ToString();
            m_numTXBufferSize.Text = m_params.UART_TXBufferSize.ToString();

            // Interrupt sources
            m_chbInterruptUARTDone.Checked = m_params.UART_InterruptOnUARTDone;
            m_chbInterruptTXNotFull.Checked = m_params.UART_InterruptOnTXFIFONotFull;
            m_chbInterruptTXEmpty.Checked = m_params.UART_InterruptOnTXFIFOEmpty;
            m_chbInterruptTXOverflow.Checked = m_params.UART_InterruptOnTXFIFOOverflow;
            m_chbInterruptTXUnderflow.Checked = m_params.UART_InterruptOnTXFIFOUnderflow;
            m_chbInterruptTXLostArb.Checked = m_params.UART_InterruptOnTXLostArbitration;
            m_chbInterruptTxNack.Checked = m_params.UART_InterruptOnTXNACK;
            m_chbInterruptTXTrigger.Checked = m_params.UART_InterruptOnTXFIFOTrigger;

            m_cbTXTriggerLevel.SelectedIndex = m_params.UART_TXTriggerLevel;

            m_chbInterruptRXFull.Checked = m_params.UART_InterruptOnRXFIFOFull;
            m_chbInterruptRXNotEmpty.Checked = m_params.UART_InterruptOnRXFIFONotEmpty;
            m_chbInterruptRXOverflow.Checked = m_params.UART_InterruptOnRXFIFOOverflow;
            m_chbInterruptRXUnderflow.Checked = m_params.UART_InterruptOnRXFIFOUnderflow;
            m_chbInterruptRXFrameErr.Checked = m_params.UART_InterruptOnRXFrameError;
            m_chbInterruptRXParityErr.Checked = m_params.UART_InterruptOnRXParityError;
            m_chbInterruptRXTrigger.Checked = m_params.UART_InterruptOnRXFIFOTrigger;

            m_cbRXTriggerLevel.SelectedIndex = m_params.UART_RXTriggerLevel;

            // Interrupt connections
            switch (m_params.UART_InterruptMode)
            {
                case CyEInterruptModeType.INTERRUPT_NONE:
                    m_rbNone.Checked = true;
                    break;
                case CyEInterruptModeType.EXTERNAL:
                    m_rbExternal.Checked = true;
                    break;
                case CyEInterruptModeType.INTERNAL:
                    m_rbInternal.Checked = true;
                    break;
            }

            // Multiprocessor mode
            m_chbEnMultiProcMode.Checked = m_params.UART_MultiProcessorModeEnabled;
            m_numAddress.Text = m_params.UART_MultiProcessorAddress.ToString("X2");
            m_numAddressMask.Text = m_params.UART_MultiProcessorAddressMask.ToString("X2");
            m_chbAcceptAddress.Checked = m_params.UART_MultiProcessorAcceptAddress;

            // RX FIFO Drop
            m_chbDropOnParityError.Checked = m_params.UART_RXFIFODropOnParityError;
            m_chbDropOnFrameError.Checked = m_params.UART_RXFIFODropOnFrameError;

            // Update controls' state
            UpdateBuffersSize();
            UpdateTriggersLevel();
            UpdateRXFrameErrorIntr();
            UpdateInterruptConnection();
            UpdateInterruptSources();
            UpdateMultiprocessorMode();
            UpdateRXFIFODrop();
        }

        private void UpdateBuffersSize()
        {
            m_numTXBufferSize.Enabled = m_params.m_uartDirection.Value != CyEUARTDirectionType.RX;
            m_lblTXBufferSize.Enabled = m_params.m_uartDirection.Value != CyEUARTDirectionType.RX;

            m_numRXBufferSize.Enabled = m_params.m_uartDirection.Value != CyEUARTDirectionType.TX;
            m_lblRXBufferSize.Enabled = m_params.m_uartDirection.Value != CyEUARTDirectionType.TX;
        }

        private void UpdateTriggersLevel()
        {
            m_cbTXTriggerLevel.Enabled = (m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.RX) && m_params.UART_InterruptOnTXFIFOTrigger;

            m_cbRXTriggerLevel.Enabled = (m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX) && m_params.UART_InterruptOnRXFIFOTrigger;
        }

        public void UpdateRXFrameErrorIntr()
        {
            m_chbInterruptRXFrameErr.Enabled = (m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX) &&
                (m_params.m_uartNumberStopBits.Value != CyEUARTNumberStopBitsType.ONE_BIT ||
                m_params.m_uartDirection.Value != CyEUARTDirectionType.RX);
        }

        private void UpdateInterruptConnection()
        {
            bool bufSizeOverflow = (m_params.UART_RXBufferSize > CyParamRanges.UART_RX_BUFFER_MIN_SIZE) ||
                (m_params.UART_TXBufferSize > CyParamRanges.UART_TX_BUFFER_MIN_SIZE);

            m_rbNone.Enabled = bufSizeOverflow == false;
            m_rbExternal.Enabled = bufSizeOverflow == false;
        }

        private void UpdateMultiprocessorMode()
        {
            bool enabled = (m_params.m_uartNumberDataBits.Value == CyEUARTNumberDataBitsType.NINE) &&
                (m_params.m_uartSubMode.Value == CyEUARTSubModeType.STANDARD);
            bool multiprocModeEnabled = enabled && m_params.UART_MultiProcessorModeEnabled;

            m_chbEnMultiProcMode.Enabled = enabled;

            m_lblAddress.Enabled = multiprocModeEnabled;
            m_numAddress.Enabled = multiprocModeEnabled;
            m_lblMask.Enabled = multiprocModeEnabled;
            m_numAddressMask.Enabled = multiprocModeEnabled;
            m_chbAcceptAddress.Enabled = multiprocModeEnabled;
        }

        private void UpdateRXFIFODrop()
        {
            bool dropOnParityEnable = m_params.m_uartParityType.Value != CyEUARTParityType.PARITY_NONE;
            bool dropOnFrameEnable = m_params.m_uartNumberStopBits.Value != CyEUARTNumberStopBitsType.ONE_BIT;

            m_grpRxFifoDrop.Enabled = (dropOnParityEnable || dropOnFrameEnable);

            m_chbDropOnParityError.Enabled = dropOnParityEnable;
            m_chbDropOnFrameError.Enabled = dropOnFrameEnable;
        }

        private void UpdateInterruptSources()
        {
            bool interruptEnabled = m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE;
            bool txIntrEnabled = interruptEnabled && m_params.m_uartDirection.Value != CyEUARTDirectionType.RX;
            bool rxIntrEnabled = interruptEnabled && m_params.m_uartDirection.Value != CyEUARTDirectionType.TX;

            foreach (CheckBox txSource in m_txSources)
            {
                txSource.Enabled = txIntrEnabled;
            }

            foreach (CheckBox rxSource in m_rxSources)
            {
                rxSource.Enabled = rxIntrEnabled;
            }

            UpdateTXSmartCardInterrupts();
            UpdateRXNotEmptyInterrupt();
            UpdateTXNotFullInterrupt();
        }

        private void UpdateRXNotEmptyInterrupt()
        {
            m_chbInterruptRXNotEmpty.Enabled =
                (m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX) &&
                (m_params.UART_RXBufferSize <= CyParamRanges.UART_RX_BUFFER_MIN_SIZE);
        }

        private void UpdateTXNotFullInterrupt()
        {
            m_chbInterruptTXNotFull.Enabled =
                (m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.RX) &&
                (m_params.UART_TXBufferSize <= CyParamRanges.UART_TX_BUFFER_MIN_SIZE);
        }

        private void UpdateTXSmartCardInterrupts()
        {
            if ((m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.RX) &&
                (m_params.m_uartSubMode.Value == CyEUARTSubModeType.SMART_CARD))
            {
                m_chbInterruptTXLostArb.Enabled = true;
                m_chbInterruptTxNack.Enabled = true;
            }
            else
            {
                m_chbInterruptTXLostArb.Enabled = false;
                m_chbInterruptTxNack.Enabled = false;
            }
        }

        public void ResetTXBufferSize()
        {
            if (m_params.m_uartDirection.Value == CyEUARTDirectionType.RX)
            {
                m_numTXBufferSize.Text = CyParamRanges.UART_TX_BUFFER_MIN_SIZE.ToString();
            }
            UpdateBuffersSize();
        }

        public void ResetRXBufferSize()
        {
            if (m_params.m_uartDirection.Value == CyEUARTDirectionType.TX)
            {
                m_numRXBufferSize.Text = CyParamRanges.UART_RX_BUFFER_MIN_SIZE.ToString();
            }
            UpdateBuffersSize();
        }

        private void ResetInterruptConnection()
        {
            if ((m_params.UART_RXBufferSize > CyParamRanges.UART_RX_BUFFER_MIN_SIZE) ||
                (m_params.UART_TXBufferSize > CyParamRanges.UART_TX_BUFFER_MIN_SIZE) ||
                (m_params.UART_InterruptMode == CyEInterruptModeType.INTERNAL))
            {
                m_rbInternal.Checked = true;
            }

            UpdateInterruptConnection();
        }

        public void ResetMultiprocessorMode()
        {
            if ((m_params.m_uartNumberDataBits.Value != CyEUARTNumberDataBitsType.NINE) ||
                (m_params.m_uartSubMode.Value != CyEUARTSubModeType.STANDARD))
            {
                m_chbEnMultiProcMode.Checked = false;
            }
            if (m_params.UART_MultiProcessorModeEnabled == false)
            {
                m_chbAcceptAddress.Checked = false;
            }

            UpdateMultiprocessorMode();
        }

        public void ResetRXFIFODrop()
        {
            if (m_params.m_uartParityType.Value == CyEUARTParityType.PARITY_NONE)
            {
                m_chbDropOnParityError.Checked = false;
            }

            if (m_params.m_uartNumberStopBits.Value == CyEUARTNumberStopBitsType.ONE_BIT)
            {
                m_chbDropOnFrameError.Checked = false;
            }

            UpdateRXFIFODrop();
        }

        public void ResetInterruptSources()
        {
            if ((m_params.UART_InterruptMode == CyEInterruptModeType.INTERRUPT_NONE) ||
                (m_params.m_uartDirection.Value == CyEUARTDirectionType.RX))
            {
                foreach (CheckBox txSource in m_txSources)
                {
                    txSource.Checked = false;
                }
            }
            if ((m_params.UART_InterruptMode == CyEInterruptModeType.INTERRUPT_NONE) ||
                (m_params.m_uartDirection.Value == CyEUARTDirectionType.TX))
            {
                foreach (CheckBox rxSource in m_rxSources)
                {
                    rxSource.Checked = false;
                }
            }

            ResetRXNotEmptyInterrupt();
            ResetTXNotFullInterrupt();
            ResetTXSmartCardInterrupts();

            UpdateInterruptSources();
        }

        private void ResetRXNotEmptyInterrupt()
        {
            if ((m_params.UART_InterruptMode != CyEInterruptModeType.INTERRUPT_NONE) &&
                (m_params.m_uartDirection.Value != CyEUARTDirectionType.TX) &&
                (m_params.UART_RXBufferSize > CyParamRanges.UART_RX_BUFFER_MIN_SIZE))
            {
                m_chbInterruptRXNotEmpty.Checked = true;
            }

            UpdateRXNotEmptyInterrupt();
        }

        private void ResetTXNotFullInterrupt()
        {
            if (m_params.UART_TXBufferSize > CyParamRanges.UART_TX_BUFFER_MIN_SIZE)
            {
                m_chbInterruptTXNotFull.Checked = false;
            }

            UpdateTXNotFullInterrupt();
        }

        public void ResetTXSmartCardInterrupts()
        {
            if ((m_params.UART_InterruptMode == CyEInterruptModeType.INTERRUPT_NONE) ||
                (m_params.m_uartDirection.Value == CyEUARTDirectionType.RX) ||
                (m_params.m_uartSubMode.Value != CyEUARTSubModeType.SMART_CARD))
            {
                m_chbInterruptTXLostArb.Checked = false;
                m_chbInterruptTxNack.Checked = false;
            }

            UpdateTXSmartCardInterrupts();
        }
        #endregion

        #region Event handlers
        void m_numBufferSize_TextChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_numRXBufferSize || sender == m_numTXBufferSize)
                {
                    m_errorProvider.SetError(sender as NumericUpDown, string.Empty);

                    UInt32 value;

                    if (UInt32.TryParse((sender as NumericUpDown).Text, out value))
                    {
                        UInt32 minValue = 1;
                        string errorMsg = string.Empty;

                        if (sender == m_numRXBufferSize)
                        {
                            if (value == CyParamRanges.UART_RX_BUFFER_MIN_SIZE && m_params.UART_RXBufferSize > value)
                            {
                                m_chbInterruptRXNotEmpty.Checked = false;
                            }

                            m_params.UART_RXBufferSize = value;

                            minValue = CyParamRanges.UART_RX_BUFFER_MIN_SIZE;
                            if (value < minValue)
                            {
                                errorMsg = string.Format(Resources.UARTRXBufferSizeError, minValue);
                            }
                        }
                        else if (sender == m_numTXBufferSize)
                        {
                            m_params.UART_TXBufferSize = value;

                            minValue = CyParamRanges.UART_TX_BUFFER_MIN_SIZE;
                            if (value < minValue)
                            {
                                errorMsg = string.Format(Resources.UARTTXBufferSizeError, minValue);
                            }
                        }

                        ResetInterruptConnection();
                        ResetInterruptSources();

                        m_errorProvider.SetError(sender as Control, errorMsg);
                    }
                    else
                    {
                        m_errorProvider.SetError(sender as Control, Resources.UARTBufferSizeValueFormatError);
                    }
                }
            }
        }

        void m_numAddress_TextChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_numAddress || sender == m_numAddressMask)
                {
                    try
                    {
                        UInt16 value = Convert.ToUInt16((sender as NumericUpDown).Text, 16);

                        if (sender == m_numAddress)
                        {
                            m_params.UART_MultiProcessorAddress = (byte)value;
                        }
                        else
                        {
                            m_params.UART_MultiProcessorAddressMask = (byte)value;
                        }

                        if (value >= byte.MinValue && value <= byte.MaxValue)
                        {
                            m_errorProvider.SetError(sender as Control, string.Empty);
                        }
                        else
                        {
                            throw new Exception();
                        }
                    }
                    catch
                    {
                        m_errorProvider.SetError(sender as Control, string.Format(Resources.UARTAddressFormatError,
                            byte.MinValue, byte.MaxValue));
                    }
                }
            }
        }

        void m_chb_CheckedChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                // Interrupts
                if (sender == m_chbInterruptUARTDone)
                {
                    m_params.UART_InterruptOnUARTDone = m_chbInterruptUARTDone.Checked;
                }
                else if (sender == m_chbInterruptTXNotFull)
                {
                    m_params.UART_InterruptOnTXFIFONotFull = m_chbInterruptTXNotFull.Checked;
                }
                else if (sender == m_chbInterruptTXEmpty)
                {
                    m_params.UART_InterruptOnTXFIFOEmpty = m_chbInterruptTXEmpty.Checked;
                }
                else if (sender == m_chbInterruptTXOverflow)
                {
                    m_params.UART_InterruptOnTXFIFOOverflow = m_chbInterruptTXOverflow.Checked;
                }
                else if (sender == m_chbInterruptTXUnderflow)
                {
                    m_params.UART_InterruptOnTXFIFOUnderflow = m_chbInterruptTXUnderflow.Checked;
                }
                else if (sender == m_chbInterruptTXLostArb)
                {
                    m_params.UART_InterruptOnTXLostArbitration = m_chbInterruptTXLostArb.Checked;
                }
                else if (sender == m_chbInterruptTxNack)
                {
                    m_params.UART_InterruptOnTXNACK = m_chbInterruptTxNack.Checked;
                }
                else if (sender == m_chbInterruptTXTrigger)
                {
                    m_params.UART_InterruptOnTXFIFOTrigger = m_chbInterruptTXTrigger.Checked;
                    UpdateTriggersLevel();
                }
                else if (sender == m_chbInterruptRXFull)
                {
                    m_params.UART_InterruptOnRXFIFOFull = m_chbInterruptRXFull.Checked;
                }
                else if (sender == m_chbInterruptRXNotEmpty)
                {
                    m_params.UART_InterruptOnRXFIFONotEmpty = m_chbInterruptRXNotEmpty.Checked;
                }
                else if (sender == m_chbInterruptRXOverflow)
                {
                    m_params.UART_InterruptOnRXFIFOOverflow = m_chbInterruptRXOverflow.Checked;
                }
                else if (sender == m_chbInterruptRXUnderflow)
                {
                    m_params.UART_InterruptOnRXFIFOUnderflow = m_chbInterruptRXUnderflow.Checked;
                }
                else if (sender == m_chbInterruptRXFrameErr)
                {
                    m_params.UART_InterruptOnRXFrameError = m_chbInterruptRXFrameErr.Checked;
                }
                else if (sender == m_chbInterruptRXParityErr)
                {
                    m_params.UART_InterruptOnRXParityError = m_chbInterruptRXParityErr.Checked;
                }
                else if (sender == m_chbInterruptRXTrigger)
                {
                    m_params.UART_InterruptOnRXFIFOTrigger = m_chbInterruptRXTrigger.Checked;
                    UpdateTriggersLevel();
                }
                // Multi processor mode
                else if (sender == m_chbEnMultiProcMode)
                {
                    m_params.UART_MultiProcessorModeEnabled = m_chbEnMultiProcMode.Checked;
                    ResetMultiprocessorMode();
                }
                // RX FIFO Drop
                else if (sender == m_chbDropOnFrameError)
                {
                    m_params.UART_RXFIFODropOnFrameError = m_chbDropOnFrameError.Checked;
                }
                else if (sender == m_chbDropOnParityError)
                {
                    m_params.UART_RXFIFODropOnParityError = m_chbDropOnParityError.Checked;
                }
                else if (sender == m_chbAcceptAddress)
                {
                    m_params.UART_MultiProcessorAcceptAddress = m_chbAcceptAddress.Checked;
                }
            }
        }

        void m_cb_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (sender == m_cbRXTriggerLevel)
                {
                    m_params.UART_RXTriggerLevel = (byte)m_cbRXTriggerLevel.SelectedIndex;
                }
                else if (sender == m_cbTXTriggerLevel)
                {
                    m_params.UART_TXTriggerLevel = (byte)m_cbTXTriggerLevel.SelectedIndex;
                }
            }
        }

        void m_rbIntrMode_CheckedChanged(object sender, EventArgs e)
        {
            if (m_params.GlobalEditMode)
            {
                if (m_rbNone.Checked)
                {
                    m_params.UART_InterruptMode = CyEInterruptModeType.INTERRUPT_NONE;
                }
                else if (m_rbExternal.Checked)
                {
                    m_params.UART_InterruptMode = CyEInterruptModeType.EXTERNAL;
                }
                else if (m_rbInternal.Checked)
                {
                    m_params.UART_InterruptMode = CyEInterruptModeType.INTERNAL;
                }
            }

            ResetInterruptSources();

            if (m_params.m_uartConfigTab != null)
            {
                m_params.m_uartConfigTab.UpdateEnableWakeup();
            }
        }
        #endregion
    }
}
