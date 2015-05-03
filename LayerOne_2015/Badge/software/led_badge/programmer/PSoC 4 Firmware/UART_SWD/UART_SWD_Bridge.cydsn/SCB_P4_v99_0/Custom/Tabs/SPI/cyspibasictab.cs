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
    public partial class CySPIBasicTab : CyTabControlWrapper
    {
        #region CyTabControlWrapper Members
        public override string TabName
        {
            get
            {
                return "SPI Basic";
            }
        }
        #endregion

        #region Auxiliary properties
        public bool SimultionesTXRXMode
        {
            get
            {
                //When National Semicomductor Sub Mode is selected, the TX data bits and RX data bits 
                //values can be differ. In the rest sub modes they must change simultionesly. 
                //So, if press increase or decrease buttons for TX data bits, the RX data bits value must be changed 
                //the same and vise versa. If value is inputed manualy, the same value must be copied in other field.

                return m_params.m_spiSubMode.Value != CyESPISubModeType.NS;
            }
        }
        public ushort HertzMultiplier
        {
            get { return ((m_cbBitRateUnits.SelectedIndex == 0) ? (ushort)1 : CyParameters.POW6); }
        }
        #endregion

        #region Constructor(s)
        public CySPIBasicTab(CyParameters parameters)
            : base(parameters)
        {
            m_params.m_spiBasicTab = this;

            InitializeComponent();

            // Add comboboxes' ranges
            m_cbMode.Items.AddRange(m_params.m_spiMode.EnumDescriptionsList);
            m_cbSubMode.Items.AddRange(m_params.m_spiSubMode.EnumDescriptionsList);
            m_cbSCLKMode.Items.AddRange(m_params.m_spiSCLKMode.EnumDescriptionsList);
            m_cbBitOrder.Items.AddRange(m_params.m_spiBitsOrder.EnumDescriptionsList);
            m_cbSSNumber.Items.Clear();
            for (int i = 1; i <= CyParameters.SPI_MAX_SS_NUMBER; i++)
            {
                m_cbSSNumber.Items.Add(i.ToString());
            }

            #region Set event handlers
            m_numOversamplingFactor.TextChanged += new EventHandler(m_num_TextChanged);
            m_numOversamplingFactor.Maximum = UInt32.MaxValue;

            m_numTXDataBits.TextChanged += new EventHandler(m_num_TextChanged);
            m_numTXDataBits.Maximum = UInt32.MaxValue;

            m_numRXDataBits.TextChanged += new EventHandler(m_num_TextChanged);
            m_numRXDataBits.Maximum = UInt32.MaxValue;

            m_chbClockFromTerminal.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableLateMISOSample.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableMedianFilter.CheckedChanged += new EventHandler(m_chb_CheckedChanged);
            m_chbEnableWakeup.CheckedChanged += new EventHandler(m_chb_CheckedChanged);

            m_cbMode.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbSubMode.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbSCLKMode.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbBitOrder.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbBitRateUnits.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);
            m_cbSSNumber.SelectedIndexChanged += new EventHandler(m_cb_SelectedIndexChanged);

            m_cbBitRate.TextChanged += new EventHandler(m_cb_TextChanged);
            #endregion
        }
        #endregion

        #region Update UI
        public override void UpdateUI()
        {
            if (m_params == null) return;

            m_chbEnableMedianFilter.Checked = m_params.SPI_EnableMedianFilter;
            m_chbEnableLateMISOSample.Checked = m_params.SPI_EnableLateMISOSample;
            m_chbEnableWakeup.Checked = m_params.SPI_EnableWakeupFromSleepMode;

            SetBitrateValue((double)m_params.SPI_BitsRateInKHz, true);

            //Clock from terminal
            m_chbClockFromTerminal.Checked = m_params.SPI_ClockFromTerm;

            m_numOversamplingFactor.Text = CyParameters.GetTextFromNullableValue<byte?>(m_params.SPI_OvsFactor);

            switch (m_params.SPI_TransferSeparation)
            {
                case CyESPITransferSeparationType.SEPARATED:
                    m_rbTransferSeparated.Checked = true;
                    break;
                case CyESPITransferSeparationType.CONTINUOUS:
                    m_rbTransferContinuous.Checked = true;
                    break;
                default:
                    break;
            }

            m_cbMode.SelectedItem = m_params.m_spiMode.Description;
            m_cbSubMode.SelectedItem = m_params.m_spiSubMode.Description;
            m_cbSCLKMode.SelectedItem = m_params.m_spiSCLKMode.Description;
            m_cbBitOrder.SelectedItem = m_params.m_spiBitsOrder.Description;
            m_cbSSNumber.SelectedItem = m_params.SPI_NumberOfSSLines.ToString();

            m_numTXDataBits.Text = CyParameters.GetTextFromNullableValue<byte?>(m_params.SPI_NumberOfTXDataBits);
            m_numRXDataBits.Text = CyParameters.GetTextFromNullableValue<byte?>(m_params.SPI_NumberOfRXDataBits);
        }

        public void UpdateCheckBoxesStates()
        {
            if ((m_params.m_spiMode.Value == CyESPIModeType.SPI_SLAVE) &&
                (m_params.m_spiSubMode.Value == CyESPISubModeType.MOTOROLA) &&
                (m_params.m_spiSCLKMode.Value == CyESPISCLKModeType.MODE_00) &&
                (m_params.SPI_InterruptMode == CyEInterruptModeType.INTERNAL))
            {
                m_chbEnableWakeup.Enabled = true;
            }
            else
            {
                m_chbEnableWakeup.Enabled = false;
                m_chbEnableWakeup.Checked = false;
            }
        }

        public void SetBitrateValue(double bitrate, bool detectUnits)
        {
            //When changed the value in Bit Rate drop-down list or value inputed by hand must be 
            //immediately converted from kbps to Mbps and vise versa.

            if (detectUnits)
                m_cbBitRateUnits.SelectedIndex = bitrate > CyParameters.POW6 ? 1 : 0;

            bitrate = bitrate / HertzMultiplier;

            object[] list = new object[CyParameters.SPI_DATA_RATE_LIST.Length];
            for (int i = 0; i < list.Length; i++)
            {
                list[i] = (double)CyParameters.SPI_DATA_RATE_LIST[i] / HertzMultiplier;
            }

            m_cbBitRate.Items.Clear();
            m_cbBitRate.Items.AddRange(list);

            m_cbBitRate.Text = bitrate.ToString();
        }
        #endregion

        #region Event handlers
        void m_chb_CheckedChanged(object sender, EventArgs e)
        {
            if (sender == m_chbEnableMedianFilter)
            {
                m_params.SPI_EnableMedianFilter = m_chbEnableMedianFilter.Checked;
                m_num_Validating(m_numOversamplingFactor, null);
            }
            else if (sender == m_chbEnableLateMISOSample)
            {
                m_params.SPI_EnableLateMISOSample = m_chbEnableLateMISOSample.Checked;
                m_num_Validating(m_numOversamplingFactor, null);
                UpdateDrawing();
            }
            else if (sender == m_chbEnableWakeup)
            {
                m_params.SPI_EnableWakeupFromSleepMode = m_chbEnableWakeup.Checked;
            }
            else if (sender == m_chbClockFromTerminal)
            {
                m_params.SPI_ClockFromTerm = m_chbClockFromTerminal.Checked;
                UpdateClock();
            }
        }

        void m_cb_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender == m_cbMode)
            {
                m_params.m_spiMode.Description = m_cbMode.Text;

                bool isMaster = m_params.m_spiMode.Value == CyESPIModeType.SPI_MASTER;

                m_gbTransferSeparation.Enabled = isMaster;
                if (isMaster == false)
                {
                    // Set number of slave select lines to 1
                    m_cbSSNumber.SelectedIndex = 0;
                }
                m_lblSSNumber.Enabled = isMaster;
                m_cbSSNumber.Enabled = isMaster;
                UpdateCheckBoxesStates();
                if (m_params.m_spiAdvancedTab != null)
                {
                    m_params.m_spiAdvancedTab.UpdateInterruptsVisibility();
                    m_params.m_spiAdvancedTab.UpdateInterruptSources();
                }
            }
            else if (sender == m_cbSubMode)
            {
                m_params.m_spiSubMode.Description = m_cbSubMode.Text;

                //Control should be not editable. It's value can be selected from drop-down list only when 
                //Sub Mode is "Motorola".

                //For  "TI (Start coincides)" and "TI (Start precedes)" must be automatically selected 
                //to "Mode 2 (CPHA = 1, CPOL = 0)" and cannot be changed.

                //For  "National Semiconductor)" must be automatically selected to 
                //"Mode 0 (CPHA = 0, CPOL = 0)" and cannot be changed.

                m_cbSCLKMode.Enabled = m_params.m_spiSubMode.Value == CyESPISubModeType.MOTOROLA;
                switch (m_params.m_spiSubMode.Value)
                {
                    case CyESPISubModeType.TI_COINCIDES:
                    case CyESPISubModeType.TI_PRECEDES:
                        m_params.m_spiSCLKMode.Value = CyESPISCLKModeType.MODE_10;
                        m_cbSCLKMode.SelectedItem = m_params.m_spiSCLKMode.Description;
                        break;
                    case CyESPISubModeType.NS:
                        m_params.m_spiSCLKMode.Value = CyESPISCLKModeType.MODE_00;
                        m_cbSCLKMode.SelectedItem = m_params.m_spiSCLKMode.Description;
                        break;
                    default:
                        break;
                }

                UpdateCheckBoxesStates();

                if (SimultionesTXRXMode)
                    m_numRXDataBits.Text = m_numTXDataBits.Text;

                UpdateDrawing();
            }
            else if (sender == m_cbSCLKMode)
            {
                m_params.m_spiSCLKMode.Description = m_cbSCLKMode.Text;
                UpdateDrawing();
                UpdateCheckBoxesStates();
            }
            else if (sender == m_cbBitRateUnits)
            {
                SetBitrateValue((double)m_params.SPI_BitsRateInKHz, false);
            }
            else if (sender == m_cbBitOrder)
            {
                m_params.m_spiBitsOrder.Description = m_cbBitOrder.Text;
                UpdateDrawing();
            }
            else if (sender == m_cbSSNumber)
            {
                m_params.SPI_NumberOfSSLines = (byte)(m_cbSSNumber.SelectedIndex + 1);
            }
        }

        void m_rbTransfer_CheckedChanged(object sender, EventArgs e)
        {
            if (m_rbTransferContinuous.Checked)
                m_params.SPI_TransferSeparation = CyESPITransferSeparationType.CONTINUOUS;
            else
                m_params.SPI_TransferSeparation = CyESPITransferSeparationType.SEPARATED;
        }

        #region Text Changed
        void m_num_TextChanged(object sender, EventArgs e)
        {
            CancelEventArgs ce = new CancelEventArgs();
            m_num_Validating(sender, ce);

            byte? value = null;

            byte val;
            if (byte.TryParse((sender as NumericUpDown).Text, out val))
                value = val;


            if (sender == m_numOversamplingFactor)
            {
                m_params.SPI_OvsFactor = (byte?)value;
                UpdateClock();
            }


            if (sender == m_numRXDataBits)
            {
                m_params.SPI_NumberOfRXDataBits = (byte?)value;

                if (SimultionesTXRXMode)
                    m_numTXDataBits.Text = m_numRXDataBits.Text;

                if (string.IsNullOrEmpty(m_errorProvider.GetError((Control)sender)))
                {
                    UpdateDrawing();
                }
            }

            if (sender == m_numTXDataBits)
            {
                m_params.SPI_NumberOfTXDataBits = (byte?)value;

                if (SimultionesTXRXMode)
                    m_numRXDataBits.Text = m_numTXDataBits.Text;

                if (string.IsNullOrEmpty(m_errorProvider.GetError((Control)sender)))
                {
                    UpdateDrawing();
                }
            }
        }

        void m_num_Validating(object sender, CancelEventArgs e)
        {
            bool error = false;
            string message = "";
            if (sender is NumericUpDown == false) return;
            NumericUpDown cnt = sender as NumericUpDown;

            UInt32 min = CyParameters.SPI_MIN_DATA_BITS;
            UInt32 max = CyParameters.SPI_MAX_DATA_BITS;

            if (sender == m_numOversamplingFactor)
            {
                min = m_params.GetMinOversampling(ref message);
                max = CyParameters.SPI_MAX_OVER_SAMPLING_FACTOR;
            }

            try
            {
                UInt32 val = Convert.ToUInt32(cnt.Text);

                if (val < min || val > max) throw new Exception();
            }
            catch
            {
                if (sender != m_numOversamplingFactor)
                    message = string.Format(Resources.ErrorValueLimit, min, max);
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

        void m_cb_TextChanged(object sender, EventArgs e)
        {
            if (sender == m_cbBitRate)
            {
                double value;
                bool parsed = double.TryParse(m_cbBitRate.Text, out value);
                if (parsed)
                {
                    value *= HertzMultiplier;
                    m_params.SPI_BitsRateInKHz = (UInt16)value;
                }

                if (parsed && value <= CyParameters.SPI_MAX_DATA_RATE && value >= CyParameters.SPI_MIN_DATA_RATE)
                {
                    m_errorProvider.SetError(m_cbBitRate, string.Empty);
                    UpdateClock();
                }
                else
                {
                    m_errorProvider.SetError(m_cbBitRate, string.Format(Resources.ErrorValueLimit,
                        CyParameters.SPI_MIN_DATA_RATE, CyParameters.SPI_MAX_DATA_RATE));
                }
            }
        }
        #endregion
        #endregion

        #region Form Drawing
        // Image constants
        public const int PB_SPIMTEXT_WIDTH = 40;
        public const int PB_EXTENTS_BORDER = 5;
        public const int PB_POLYGON_WIDTH = 4;
        public const int NUM_WAVEFORMS = 5;

        public void UpdateDrawing()
        {
            if ((m_pictDrawing.Width == 0) || (m_pictDrawing.Height == 0))
                return;

            bool normal_mode = ((m_params.m_spiSCLKMode.Value == CyESPISCLKModeType.MODE_00) ||
                  (m_params.m_spiSCLKMode.Value == CyESPISCLKModeType.MODE_01)) ? true : false;
            bool starthigh = (m_params.m_spiSCLKMode.Value == CyESPISCLKModeType.MODE_01) ||
                (m_params.m_spiSCLKMode.Value == CyESPISCLKModeType.MODE_11) ? true : false;
            bool shiftDir = (m_params.m_spiBitsOrder.Value == CyESPIBitOrderType.MSB_FIRST) ? false : true;

            bool nsMode = m_params.m_spiSubMode.Value == CyESPISubModeType.NS;
            int numberOfBits = nsMode ? 7 : (int)m_params.SPI_NumberOfTXDataBits;
            int numberOfTXBits = (int)m_params.SPI_NumberOfTXDataBits;
            int numberOfRXBits = (int)m_params.SPI_NumberOfRXDataBits;

            int misoStart = nsMode ? 6 : 0;
            int mosiEnd = nsMode ? 6 : numberOfBits * 2;

            int misoSt = 2 + Convert.ToInt32(normal_mode == false) +
               Convert.ToInt32(m_params.SPI_EnableLateMISOSample);


            Image waveform = new Bitmap(m_pictDrawing.Width, m_pictDrawing.Height);

            using (Graphics wfg = Graphics.FromImage(waveform))
            using (SolidBrush blkbrush = new SolidBrush(Color.Black))
            using (Pen extentspen = new Pen(blkbrush))
            {
                wfg.Clear(Color.White);
                //wfg.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

                float extentsleft = PB_EXTENTS_BORDER + PB_SPIMTEXT_WIDTH;
                float extentsright = m_pictDrawing.Width - PB_EXTENTS_BORDER;
                float padding = (extentsright - extentsleft) / 70;
                float startleft = extentsleft + padding;
                float endright = extentsright - padding;

                // Setup the right, left and center indicators
                extentspen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
                // Draw the Left Extents Line
                wfg.DrawLine(extentspen, extentsleft, PB_EXTENTS_BORDER,
                    extentsleft, m_pictDrawing.Height - PB_EXTENTS_BORDER);
                // Draw the Right Extents Line
                wfg.DrawLine(extentspen, extentsright, PB_EXTENTS_BORDER,
                    extentsright, m_pictDrawing.Height - PB_EXTENTS_BORDER);

                extentspen.Dispose();

                // Setup and draw all of the waveforms
                int numwaveforms = NUM_WAVEFORMS;
                string[] wfnames = new string[NUM_WAVEFORMS];

                wfnames[0] = "SS";
                wfnames[1] = "SCLK";
                wfnames[2] = "MOSI";
                wfnames[3] = "MISO";
                wfnames[4] = "Sample";

                Font perfont = new Font("Arial", 10, FontStyle.Regular, GraphicsUnit.Pixel);

                // Each waveform's height is dependent upon the drawing size minus a top and bottom border 
                // and the top period waveform which is the size of two polygon widths, and an bottom 
                // ticker tape of 2 polygon widths
                float wfheight = (m_pictDrawing.Height - (2 * PB_EXTENTS_BORDER) - (4 * PB_POLYGON_WIDTH)) /
                    numwaveforms;
                // Fill in All Waveform Names
                for (int i = 0; i < numwaveforms; i++)
                {
                    PointF pt = new PointF(extentsleft - wfg.MeasureString(wfnames[i], perfont).Width -
                        PB_EXTENTS_BORDER, PB_EXTENTS_BORDER + (2 * PB_POLYGON_WIDTH) + (wfheight * i) +
                        (wfheight / 2) - (wfg.MeasureString(wfnames[i], perfont).Height / 2));
                    wfg.DrawString(wfnames[i], perfont, blkbrush, pt);
                }

                // Draw Waveforms
                int numsegments = 2 + (Convert.ToInt16(numberOfBits) * 2) + 3;

                for (int i = 0; i < numwaveforms; i++)
                {
                    float highY = PB_EXTENTS_BORDER + (2 * PB_POLYGON_WIDTH) + (wfheight * i) + (wfheight / 8);
                    float lowY = PB_EXTENTS_BORDER + (2 * PB_POLYGON_WIDTH) + (wfheight * (i + 1));
                    float middle = (lowY + highY) / 2;
                    float segwidth = (extentsright - extentsleft) / numsegments;

                    List<float> segs = new List<float>();
                    for (int x = 0; x < numsegments; x++)
                    {
                        segs.Add(extentsleft + (x * segwidth));
                    }
                    using (SolidBrush wfbrush = new SolidBrush(Color.Blue))
                    using (Pen pen = new Pen(wfbrush))
                    {
                        string val = null;
                        int j = 0;


                        switch (wfnames[i])
                        {
                            case "SS":
                                switch (m_params.m_spiSubMode.Value)
                                {
                                    case CyESPISubModeType.MOTOROLA:
                                        wfg.DrawLine(pen, segs[0], highY, segs[2], highY);
                                        wfg.DrawLine(pen, segs[2], highY, segs[2], lowY);
                                        wfg.DrawLine(pen, segs[2], lowY, segs[numsegments - 2], lowY);
                                        wfg.DrawLine(pen, segs[numsegments - 2], lowY, segs[numsegments - 2], highY);
                                        wfg.DrawLine(pen, segs[numsegments - 2], highY, segs[numsegments - 1], highY);
                                        break;
                                    case CyESPISubModeType.TI_COINCIDES:
                                        wfg.DrawLine(pen, segs[0], lowY, segs[3], lowY);
                                        wfg.DrawLine(pen, segs[3], highY, segs[3], lowY);

                                        wfg.DrawLine(pen, segs[3], highY, segs[5], highY);
                                        wfg.DrawLine(pen, segs[5], highY, segs[5], lowY);
                                        wfg.DrawLine(pen, segs[5], lowY, segs[numsegments - 1], lowY);
                                        break;
                                    case CyESPISubModeType.TI_PRECEDES:
                                        wfg.DrawLine(pen, segs[0], lowY, segs[1], lowY);
                                        wfg.DrawLine(pen, segs[1], highY, segs[1], lowY);

                                        wfg.DrawLine(pen, segs[1], highY, segs[3], highY);
                                        wfg.DrawLine(pen, segs[3], highY, segs[3], lowY);
                                        wfg.DrawLine(pen, segs[3], lowY, segs[numsegments - 1], lowY);
                                        break;
                                    case CyESPISubModeType.NS:
                                        wfg.DrawLine(pen, segs[0], highY, segs[1], highY);
                                        wfg.DrawLine(pen, segs[1], highY, segs[1], lowY);
                                        wfg.DrawLine(pen, segs[1], lowY, segs[numsegments - 2], lowY);
                                        wfg.DrawLine(pen, segs[numsegments - 2], lowY, segs[numsegments - 2], highY);
                                        wfg.DrawLine(pen, segs[numsegments - 2], highY, segs[numsegments - 1], highY);
                                        break;
                                    default:
                                        break;
                                }

                                break;
                            case "MOSI":
                                bool no_shift = normal_mode;

                                int mosiSt = no_shift ? 2 : 3;

                                // Draw Bus to First Transition Point
                                wfg.DrawLine(pen, segs[0], highY, segs[mosiSt] - 2, highY);
                                wfg.DrawLine(pen, segs[0], lowY, segs[mosiSt] - 2, lowY);
                                // Draw Transition
                                wfg.DrawLine(pen, segs[mosiSt] - 2, highY, segs[mosiSt] + 2, lowY);
                                wfg.DrawLine(pen, segs[mosiSt] - 2, lowY, segs[mosiSt] + 2, highY);

                                for (j = 0; j < (mosiEnd); )
                                {
                                    bool nsEndNode = (j + 2 >= mosiEnd) && nsMode;
                                    // Draw Bus to Transition Point
                                    wfg.DrawLine(pen, segs[mosiSt + j] + 2, highY, segs[mosiSt + (j + 2)] - 2, highY);
                                    wfg.DrawLine(pen, segs[mosiSt + j] + 2, lowY, segs[mosiSt + (j + 2)] - 2, lowY);

                                    if (nsEndNode == false)
                                    {
                                        // Draw Transition line
                                        wfg.DrawLine(pen, segs[mosiSt + (j + 2)] - 2, highY,
                                            segs[mosiSt + (j + 2)] + 2, lowY);
                                        wfg.DrawLine(pen, segs[mosiSt + (j + 2)] - 2, lowY, segs[mosiSt + (j + 2)] + 2,
                                            highY);
                                    }

                                    #region label text
                                    val = String.Format("D{0}", shiftDir ? j / 2 : numberOfBits - (j / 2) - 1);
                                    if (nsMode)
                                    {
                                        if (j == 0)
                                            val = String.Format("D{0}", shiftDir ? 0 : numberOfTXBits - 1);
                                        else if (nsEndNode)
                                            val = String.Format("D{0}", shiftDir ? numberOfTXBits - 1 : 0);
                                        else
                                            val = "...";
                                    }
                                    #endregion

                                    SizeF strsize = wfg.MeasureString(val, perfont);
                                    float centerx = segs[mosiSt + j] + segwidth;
                                    using (SolidBrush strBrush = new SolidBrush(Color.Black))
                                    {
                                        wfg.DrawString(val, perfont, strBrush,
                                            new RectangleF(centerx - (strsize.Width / 2f), highY + ((wfheight) / 2f) -
                                                (strsize.Height / 2f), strsize.Width, strsize.Height));
                                    }

                                    j += 2;
                                }

                                if (nsMode)
                                {
                                    // Draw Transition line
                                    wfg.DrawLine(pen, segs[mosiSt + j] - 2, highY, segs[mosiSt + j] + 1, middle);
                                    wfg.DrawLine(pen, segs[mosiSt + j] - 2, lowY, segs[mosiSt + j] + 1, middle);

                                    //Draw end line
                                    wfg.DrawLine(pen, segs[mosiSt + j] + 1, middle, segs[segs.Count - 1], middle);
                                }
                                else
                                {
                                    // Draw Bus to Transition Point
                                    wfg.DrawLine(pen, segs[mosiSt + j] + 2, lowY, segs[mosiSt + (j + 1 +
                                        Convert.ToInt32(no_shift))], lowY);
                                    wfg.DrawLine(pen, segs[mosiSt + j] + 2, highY, segs[mosiSt + (j + 1 +
                                        Convert.ToInt32(no_shift))], highY);
                                }

                                break;
                            case "MISO":

                                if (nsMode)
                                {
                                    // Draw Bus to First Transition Point
                                    wfg.DrawLine(pen, segs[0], middle, segs[misoSt + (misoStart)], middle);
                                    // Draw Transition for idle
                                    wfg.DrawLine(pen, segs[misoSt + (misoStart)], middle, segs[misoSt + misoStart] + 2,
                                        lowY);
                                }
                                else
                                {
                                    // Draw Bus to First Transition Point
                                    wfg.DrawLine(pen, segs[0], highY, segs[misoSt] - 2, highY);
                                    wfg.DrawLine(pen, segs[0], lowY, segs[misoSt] - 2, lowY);
                                    // Draw Transition
                                    wfg.DrawLine(pen, segs[misoSt] - 2, highY, segs[misoSt] + 2, lowY);
                                    wfg.DrawLine(pen, segs[misoSt] - 2, lowY, segs[misoSt] + 2, highY);
                                }
                                for (j = misoStart; j < (numberOfBits * 2); )
                                {
                                    bool idle = (j == misoStart) && nsMode;

                                    // Draw Bus to Transition Point. For idle node oly bottom line
                                    if (idle == false)
                                        wfg.DrawLine(pen, segs[misoSt + j] + 2, highY, segs[misoSt + (j + 2)] - 2,
                                            highY);
                                    wfg.DrawLine(pen, segs[misoSt + j] + 2, lowY, segs[misoSt + (j + 2)] - 2, lowY);

                                    // Draw Transition. For idle node oly bottom line
                                    if (idle)
                                        wfg.DrawLine(pen, segs[misoSt + (j + 2)], middle, segs[misoSt + (j + 2)] + 2,
                                            lowY);
                                    else
                                        wfg.DrawLine(pen, segs[misoSt + (j + 2)] - 2, highY, segs[misoSt + (j + 2)] + 2,
                                            lowY);
                                    wfg.DrawLine(pen, segs[misoSt + (j + 2)] - 2, lowY, segs[misoSt + (j + 2)] + 2,
                                        highY);

                                    #region label text
                                    val = String.Format("D{0}", shiftDir ? j / 2 : numberOfBits - (j / 2) - 1);
                                    if (nsMode)
                                    {
                                        if (idle)
                                            val = "idle";
                                        else if (j == misoStart + 2)
                                            val = String.Format("D{0}", shiftDir ? 0 : numberOfRXBits - 1);
                                        else if (j + 2 >= (numberOfBits * 2))
                                            val = String.Format("D{0}", shiftDir ? numberOfRXBits - 1 : 0);
                                        else
                                            val = "...";
                                    }
                                    #endregion

                                    SizeF strsize = wfg.MeasureString(val, perfont);
                                    float centerx = segs[misoSt + j] + segwidth;
                                    using (SolidBrush strBrush = new SolidBrush(Color.Black))
                                    {
                                        wfg.DrawString(val, perfont, strBrush,
                                                        new RectangleF(centerx - (strsize.Width / 2f), highY +
                                                            ((wfheight) / 2f) - (strsize.Height / 2f), strsize.Width,
                                                            strsize.Height));
                                    }
                                    j += 2;
                                }

                                // Draw Bus to Transition Point
                                wfg.DrawLine(pen, segs[misoSt + j] + 2, lowY, segs[segs.Count - 1], lowY);
                                wfg.DrawLine(pen, segs[misoSt + j] + 2, highY, segs[segs.Count - 1], highY);

                                break;
                            case "SCLK":
                                int goHighSeg = m_params.m_spiSubMode.Value == CyESPISubModeType.TI_PRECEDES ? 1 : 3;
                                int addSegments = m_params.m_spiSubMode.Value == CyESPISubModeType.TI_PRECEDES ? 1 : 0;

                                wfg.DrawLine(pen, segs[0], starthigh ? highY : lowY, segs[goHighSeg], starthigh ?
                                    highY : lowY);
                                wfg.DrawLine(pen, segs[goHighSeg], starthigh ? highY : lowY, segs[goHighSeg],
                                    starthigh ? highY : lowY);

                                for (j = 0; j < (numberOfBits * 2) + addSegments; )
                                {
                                    wfg.DrawLine(pen, segs[goHighSeg + j], starthigh ? highY : lowY,
                                        segs[goHighSeg + j], starthigh ? lowY : highY);
                                    wfg.DrawLine(pen, segs[goHighSeg + j++], starthigh ? lowY : highY,
                                        segs[goHighSeg + j], starthigh ? lowY : highY);
                                    wfg.DrawLine(pen, segs[goHighSeg + j], starthigh ? lowY : highY,
                                        segs[goHighSeg + j], starthigh ? highY : lowY);
                                    wfg.DrawLine(pen, segs[goHighSeg + j++], starthigh ? highY : lowY,
                                        segs[goHighSeg + j], starthigh ? highY : lowY);
                                }
                                wfg.DrawLine(pen, segs[goHighSeg + j++], starthigh ? highY : lowY,
                                    segs[goHighSeg + j], starthigh ? highY : lowY);
                                break;
                            case "Sample":

                                wfg.DrawLine(pen, segs[0], lowY, segs[misoSt + 1] - 2, lowY);    // Go to first edge 
                                for (j = 0; j < (numberOfBits * 2); )
                                {
                                    wfg.DrawLine(pen, segs[misoSt + 1 + j] - 2, lowY, segs[misoSt + 1 + j] - 2, highY);
                                    wfg.DrawLine(pen, segs[misoSt + 1 + j] - 2, highY, segs[misoSt + 1 + j] + 2, highY);
                                    wfg.DrawLine(pen, segs[misoSt + 1 + j] + 2, highY, segs[misoSt + 1 + j] + 2, lowY);

                                    if (j + 2 < numberOfBits * 2)
                                        wfg.DrawLine(pen, segs[misoSt + 1 + j] + 2, lowY,
                                            segs[misoSt + 1 + (j + 2)] - 2, lowY);
                                    else
                                        wfg.DrawLine(pen, segs[misoSt + 1 + j] + 2, lowY, segs[segs.Count - 1], lowY);

                                    j += 2;
                                }

                                break;
                            case "Interrupt":
                                break;
                        }
                    }
                }
                wfg.Dispose();
            }
            m_pictDrawing.Image = waveform;
        }
        #endregion

        #region Actual data rate
        public void UpdateClock()
        {
            double clkFreq = m_params.SPI_ClockFromTerm ?
                CyParameters.GetExternalClock(m_params.m_term) :
                CyParameters.GetInternalClock(m_params.m_term);

            string dataRateStr = Resources.ClockDisplayUnknown;

            m_cbBitRate.Enabled = !m_params.SPI_ClockFromTerm;
            m_cbBitRateUnits.Enabled = !m_params.SPI_ClockFromTerm;
            m_lblDataRate.Enabled = !m_params.SPI_ClockFromTerm;

            if (clkFreq > 0)
            {
                double actualDataRate = Math.Round(clkFreq / (byte)m_params.SPI_OvsFactor, 3);
                if (actualDataRate > 0)
                {
                    dataRateStr = actualDataRate.ToString();
                }
            }

            // Update Actual Data Rate label
            m_lblActualBitRate.Text = string.Format(Resources.ActualDataRateDisplayFormat, dataRateStr);
        }
        #endregion
    }
}
