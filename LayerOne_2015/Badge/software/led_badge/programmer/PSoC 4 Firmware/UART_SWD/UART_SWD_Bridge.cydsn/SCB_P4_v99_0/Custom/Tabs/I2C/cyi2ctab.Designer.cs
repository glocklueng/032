namespace SCB_P4_v99_0
{
    partial class CyI2CTab
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_lblMode = new System.Windows.Forms.Label();
            this.m_lblDataRate = new System.Windows.Forms.Label();
            this.m_numOversamplingFactor = new System.Windows.Forms.NumericUpDown();
            this.m_lblOversamplingFactorHigh = new System.Windows.Forms.Label();
            this.m_cbDataRate = new System.Windows.Forms.ComboBox();
            this.m_lblSlaveAddress = new System.Windows.Forms.Label();
            this.m_numOversamplingFactorHigh = new System.Windows.Forms.NumericUpDown();
            this.m_lblActualDataRate = new System.Windows.Forms.Label();
            this.m_panelModes = new System.Windows.Forms.Panel();
            this.m_lblDataRateUnknown = new System.Windows.Forms.Label();
            this.m_chbClockFromTerminal = new System.Windows.Forms.CheckBox();
            this.m_cbMode = new System.Windows.Forms.ComboBox();
            this.m_numOversamplingFactorLow = new System.Windows.Forms.NumericUpDown();
            this.m_lblOversamplingFactorLow = new System.Windows.Forms.Label();
            this.m_lblOversamplingFactor = new System.Windows.Forms.Label();
            this.m_chbEnableMedianFilter = new System.Windows.Forms.CheckBox();
            this.m_lblSeparator1 = new System.Windows.Forms.Label();
            this.m_txtSlaveAddress = new System.Windows.Forms.TextBox();
            this.m_chbEnableWakeup = new System.Windows.Forms.CheckBox();
            this.m_lblBit = new System.Windows.Forms.Label();
            this.m_lblBit2 = new System.Windows.Forms.Label();
            this.m_lblBit3 = new System.Windows.Forms.Label();
            this.m_lblBit5 = new System.Windows.Forms.Label();
            this.m_lblBit4 = new System.Windows.Forms.Label();
            this.m_lblBit0 = new System.Windows.Forms.Label();
            this.m_panelSlaveConf = new System.Windows.Forms.Panel();
            this.m_binaryGridSlaveAddressMask = new SCB_P4_v99_0.CyBinaryGrid();
            this.m_binaryGridSlaveAddress = new SCB_P4_v99_0.CyBinaryGrid();
            this.m_lblBit1 = new System.Windows.Forms.Label();
            this.m_lblSlaveAddrMask = new System.Windows.Forms.Label();
            this.m_lblBit7 = new System.Windows.Forms.Label();
            this.m_txtSlaveAddressMask = new System.Windows.Forms.TextBox();
            this.m_lblBit6 = new System.Windows.Forms.Label();
            this.m_chbAcceptAddress = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactorHigh)).BeginInit();
            this.m_panelModes.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactorLow)).BeginInit();
            this.m_panelSlaveConf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_lblMode
            // 
            this.m_lblMode.AutoSize = true;
            this.m_lblMode.Location = new System.Drawing.Point(3, 5);
            this.m_lblMode.Name = "m_lblMode";
            this.m_lblMode.Size = new System.Drawing.Size(37, 13);
            this.m_lblMode.TabIndex = 0;
            this.m_lblMode.Text = "Mode:";
            // 
            // m_lblDataRate
            // 
            this.m_lblDataRate.AutoSize = true;
            this.m_lblDataRate.Location = new System.Drawing.Point(3, 31);
            this.m_lblDataRate.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblDataRate.Name = "m_lblDataRate";
            this.m_lblDataRate.Size = new System.Drawing.Size(86, 13);
            this.m_lblDataRate.TabIndex = 0;
            this.m_lblDataRate.Text = "Data rate (kbps):";
            // 
            // m_numOversamplingFactor
            // 
            this.m_numOversamplingFactor.Location = new System.Drawing.Point(116, 54);
            this.m_numOversamplingFactor.Name = "m_numOversamplingFactor";
            this.m_numOversamplingFactor.Size = new System.Drawing.Size(50, 20);
            this.m_numOversamplingFactor.TabIndex = 2;
            // 
            // m_lblOversamplingFactorHigh
            // 
            this.m_lblOversamplingFactorHigh.AutoSize = true;
            this.m_lblOversamplingFactorHigh.Location = new System.Drawing.Point(279, 56);
            this.m_lblOversamplingFactorHigh.Name = "m_lblOversamplingFactorHigh";
            this.m_lblOversamplingFactorHigh.Size = new System.Drawing.Size(32, 13);
            this.m_lblOversamplingFactorHigh.TabIndex = 0;
            this.m_lblOversamplingFactorHigh.Text = "High:";
            // 
            // m_cbDataRate
            // 
            this.m_cbDataRate.FormattingEnabled = true;
            this.m_cbDataRate.Items.AddRange(new object[] {
            "50",
            "100",
            "400",
            "1000"});
            this.m_cbDataRate.Location = new System.Drawing.Point(116, 28);
            this.m_cbDataRate.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbDataRate.MaxLength = 10;
            this.m_cbDataRate.Name = "m_cbDataRate";
            this.m_cbDataRate.Size = new System.Drawing.Size(50, 21);
            this.m_cbDataRate.TabIndex = 1;
            // 
            // m_lblSlaveAddress
            // 
            this.m_lblSlaveAddress.AutoSize = true;
            this.m_lblSlaveAddress.Location = new System.Drawing.Point(4, 27);
            this.m_lblSlaveAddress.Name = "m_lblSlaveAddress";
            this.m_lblSlaveAddress.Size = new System.Drawing.Size(111, 13);
            this.m_lblSlaveAddress.TabIndex = 0;
            this.m_lblSlaveAddress.Text = "Slave address (7-bits):";
            // 
            // m_numOversamplingFactorHigh
            // 
            this.m_numOversamplingFactorHigh.BackColor = System.Drawing.SystemColors.Window;
            this.m_numOversamplingFactorHigh.Location = new System.Drawing.Point(317, 53);
            this.m_numOversamplingFactorHigh.Name = "m_numOversamplingFactorHigh";
            this.m_numOversamplingFactorHigh.Size = new System.Drawing.Size(42, 20);
            this.m_numOversamplingFactorHigh.TabIndex = 4;
            this.m_numOversamplingFactorHigh.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            // 
            // m_lblActualDataRate
            // 
            this.m_lblActualDataRate.AutoSize = true;
            this.m_lblActualDataRate.Location = new System.Drawing.Point(183, 32);
            this.m_lblActualDataRate.Name = "m_lblActualDataRate";
            this.m_lblActualDataRate.Size = new System.Drawing.Size(117, 13);
            this.m_lblActualDataRate.TabIndex = 0;
            this.m_lblActualDataRate.Text = "Actual data rate (kbps):";
            // 
            // m_panelModes
            // 
            this.m_panelModes.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_panelModes.Controls.Add(this.m_lblDataRateUnknown);
            this.m_panelModes.Controls.Add(this.m_chbClockFromTerminal);
            this.m_panelModes.Controls.Add(this.m_lblMode);
            this.m_panelModes.Controls.Add(this.m_lblDataRate);
            this.m_panelModes.Controls.Add(this.m_numOversamplingFactor);
            this.m_panelModes.Controls.Add(this.m_lblOversamplingFactorHigh);
            this.m_panelModes.Controls.Add(this.m_cbDataRate);
            this.m_panelModes.Controls.Add(this.m_numOversamplingFactorHigh);
            this.m_panelModes.Controls.Add(this.m_lblActualDataRate);
            this.m_panelModes.Controls.Add(this.m_cbMode);
            this.m_panelModes.Controls.Add(this.m_numOversamplingFactorLow);
            this.m_panelModes.Controls.Add(this.m_lblOversamplingFactorLow);
            this.m_panelModes.Controls.Add(this.m_lblOversamplingFactor);
            this.m_panelModes.Controls.Add(this.m_chbEnableMedianFilter);
            this.m_panelModes.Location = new System.Drawing.Point(1, 3);
            this.m_panelModes.Name = "m_panelModes";
            this.m_panelModes.Size = new System.Drawing.Size(401, 122);
            this.m_panelModes.TabIndex = 0;
            // 
            // m_lblDataRateUnknown
            // 
            this.m_lblDataRateUnknown.AutoSize = true;
            this.m_lblDataRateUnknown.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.m_lblDataRateUnknown.Location = new System.Drawing.Point(113, 32);
            this.m_lblDataRateUnknown.Name = "m_lblDataRateUnknown";
            this.m_lblDataRateUnknown.Size = new System.Drawing.Size(72, 13);
            this.m_lblDataRateUnknown.TabIndex = 0;
            this.m_lblDataRateUnknown.Text = "UNKNOWN";
            this.m_lblDataRateUnknown.Visible = false;
            // 
            // m_chbClockFromTerminal
            // 
            this.m_chbClockFromTerminal.AutoSize = true;
            this.m_chbClockFromTerminal.Location = new System.Drawing.Point(6, 80);
            this.m_chbClockFromTerminal.Name = "m_chbClockFromTerminal";
            this.m_chbClockFromTerminal.Size = new System.Drawing.Size(115, 17);
            this.m_chbClockFromTerminal.TabIndex = 5;
            this.m_chbClockFromTerminal.Text = "Clock from terminal";
            this.m_chbClockFromTerminal.UseVisualStyleBackColor = false;
            // 
            // m_cbMode
            // 
            this.m_cbMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbMode.FormattingEnabled = true;
            this.m_cbMode.Location = new System.Drawing.Point(116, 2);
            this.m_cbMode.Name = "m_cbMode";
            this.m_cbMode.Size = new System.Drawing.Size(134, 21);
            this.m_cbMode.TabIndex = 0;
            // 
            // m_numOversamplingFactorLow
            // 
            this.m_numOversamplingFactorLow.BackColor = System.Drawing.SystemColors.Window;
            this.m_numOversamplingFactorLow.Location = new System.Drawing.Point(219, 53);
            this.m_numOversamplingFactorLow.Name = "m_numOversamplingFactorLow";
            this.m_numOversamplingFactorLow.Size = new System.Drawing.Size(42, 20);
            this.m_numOversamplingFactorLow.TabIndex = 3;
            this.m_numOversamplingFactorLow.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            // 
            // m_lblOversamplingFactorLow
            // 
            this.m_lblOversamplingFactorLow.AutoSize = true;
            this.m_lblOversamplingFactorLow.Location = new System.Drawing.Point(183, 56);
            this.m_lblOversamplingFactorLow.Name = "m_lblOversamplingFactorLow";
            this.m_lblOversamplingFactorLow.Size = new System.Drawing.Size(30, 13);
            this.m_lblOversamplingFactorLow.TabIndex = 0;
            this.m_lblOversamplingFactorLow.Text = "Low:";
            // 
            // m_lblOversamplingFactor
            // 
            this.m_lblOversamplingFactor.AutoSize = true;
            this.m_lblOversamplingFactor.Location = new System.Drawing.Point(3, 56);
            this.m_lblOversamplingFactor.Name = "m_lblOversamplingFactor";
            this.m_lblOversamplingFactor.Size = new System.Drawing.Size(104, 13);
            this.m_lblOversamplingFactor.TabIndex = 0;
            this.m_lblOversamplingFactor.Text = "Oversampling factor:";
            // 
            // m_chbEnableMedianFilter
            // 
            this.m_chbEnableMedianFilter.AutoSize = true;
            this.m_chbEnableMedianFilter.Location = new System.Drawing.Point(6, 103);
            this.m_chbEnableMedianFilter.Name = "m_chbEnableMedianFilter";
            this.m_chbEnableMedianFilter.Size = new System.Drawing.Size(83, 17);
            this.m_chbEnableMedianFilter.TabIndex = 6;
            this.m_chbEnableMedianFilter.Text = "Median filter";
            this.m_chbEnableMedianFilter.UseVisualStyleBackColor = true;
            // 
            // m_lblSeparator1
            // 
            this.m_lblSeparator1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_lblSeparator1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.m_lblSeparator1.Location = new System.Drawing.Point(3, 3);
            this.m_lblSeparator1.Name = "m_lblSeparator1";
            this.m_lblSeparator1.Size = new System.Drawing.Size(400, 2);
            this.m_lblSeparator1.TabIndex = 1;
            // 
            // m_txtSlaveAddress
            // 
            this.m_txtSlaveAddress.Location = new System.Drawing.Point(131, 24);
            this.m_txtSlaveAddress.Name = "m_txtSlaveAddress";
            this.m_txtSlaveAddress.Size = new System.Drawing.Size(50, 20);
            this.m_txtSlaveAddress.TabIndex = 0;
            // 
            // m_chbEnableWakeup
            // 
            this.m_chbEnableWakeup.AutoSize = true;
            this.m_chbEnableWakeup.Location = new System.Drawing.Point(7, 101);
            this.m_chbEnableWakeup.Margin = new System.Windows.Forms.Padding(2);
            this.m_chbEnableWakeup.Name = "m_chbEnableWakeup";
            this.m_chbEnableWakeup.Size = new System.Drawing.Size(183, 17);
            this.m_chbEnableWakeup.TabIndex = 5;
            this.m_chbEnableWakeup.Text = "Enable wakeup from Sleep Mode";
            this.m_chbEnableWakeup.UseVisualStyleBackColor = true;
            // 
            // m_lblBit
            // 
            this.m_lblBit.AutoSize = true;
            this.m_lblBit.Location = new System.Drawing.Point(207, 8);
            this.m_lblBit.Name = "m_lblBit";
            this.m_lblBit.Size = new System.Drawing.Size(19, 13);
            this.m_lblBit.TabIndex = 0;
            this.m_lblBit.Text = "Bit";
            // 
            // m_lblBit2
            // 
            this.m_lblBit2.AutoSize = true;
            this.m_lblBit2.Location = new System.Drawing.Point(338, 8);
            this.m_lblBit2.Name = "m_lblBit2";
            this.m_lblBit2.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit2.TabIndex = 0;
            this.m_lblBit2.Text = "2";
            // 
            // m_lblBit3
            // 
            this.m_lblBit3.AutoSize = true;
            this.m_lblBit3.Location = new System.Drawing.Point(317, 8);
            this.m_lblBit3.Name = "m_lblBit3";
            this.m_lblBit3.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit3.TabIndex = 0;
            this.m_lblBit3.Text = "3";
            // 
            // m_lblBit5
            // 
            this.m_lblBit5.AutoSize = true;
            this.m_lblBit5.Location = new System.Drawing.Point(275, 8);
            this.m_lblBit5.Name = "m_lblBit5";
            this.m_lblBit5.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit5.TabIndex = 0;
            this.m_lblBit5.Text = "5";
            // 
            // m_lblBit4
            // 
            this.m_lblBit4.AutoSize = true;
            this.m_lblBit4.Location = new System.Drawing.Point(296, 8);
            this.m_lblBit4.Name = "m_lblBit4";
            this.m_lblBit4.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit4.TabIndex = 0;
            this.m_lblBit4.Text = "4";
            // 
            // m_lblBit0
            // 
            this.m_lblBit0.AutoSize = true;
            this.m_lblBit0.Location = new System.Drawing.Point(380, 8);
            this.m_lblBit0.Name = "m_lblBit0";
            this.m_lblBit0.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit0.TabIndex = 0;
            this.m_lblBit0.Text = "0";
            // 
            // m_panelSlaveConf
            // 
            this.m_panelSlaveConf.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_panelSlaveConf.Controls.Add(this.m_binaryGridSlaveAddressMask);
            this.m_panelSlaveConf.Controls.Add(this.m_lblSeparator1);
            this.m_panelSlaveConf.Controls.Add(this.m_binaryGridSlaveAddress);
            this.m_panelSlaveConf.Controls.Add(this.m_lblSlaveAddress);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit0);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit1);
            this.m_panelSlaveConf.Controls.Add(this.m_txtSlaveAddress);
            this.m_panelSlaveConf.Controls.Add(this.m_chbEnableWakeup);
            this.m_panelSlaveConf.Controls.Add(this.m_lblSlaveAddrMask);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit7);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit);
            this.m_panelSlaveConf.Controls.Add(this.m_txtSlaveAddressMask);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit2);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit6);
            this.m_panelSlaveConf.Controls.Add(this.m_chbAcceptAddress);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit5);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit3);
            this.m_panelSlaveConf.Controls.Add(this.m_lblBit4);
            this.m_panelSlaveConf.Location = new System.Drawing.Point(1, 129);
            this.m_panelSlaveConf.Name = "m_panelSlaveConf";
            this.m_panelSlaveConf.Size = new System.Drawing.Size(401, 122);
            this.m_panelSlaveConf.TabIndex = 1;
            // 
            // m_binaryGridSlaveAddressMask
            // 
            this.m_binaryGridSlaveAddressMask.Bits = new string[] {
        "0",
        "0",
        "0",
        "0",
        "0",
        "0",
        "0",
        "0"};
            this.m_binaryGridSlaveAddressMask.Location = new System.Drawing.Point(232, 53);
            this.m_binaryGridSlaveAddressMask.Name = "m_binaryGridSlaveAddressMask";
            this.m_binaryGridSlaveAddressMask.NumberOfBits = 8;
            this.m_binaryGridSlaveAddressMask.Size = new System.Drawing.Size(166, 20);
            this.m_binaryGridSlaveAddressMask.TabIndex = 3;
            this.m_binaryGridSlaveAddressMask.TabStop = false;
            // 
            // m_binaryGridSlaveAddress
            // 
            this.m_binaryGridSlaveAddress.Bits = new string[] {
        "1",
        "0",
        "0",
        "0",
        "0",
        "0",
        "0",
        "X"};
            this.m_binaryGridSlaveAddress.Location = new System.Drawing.Point(232, 24);
            this.m_binaryGridSlaveAddress.Name = "m_binaryGridSlaveAddress";
            this.m_binaryGridSlaveAddress.NumberOfBits = 8;
            this.m_binaryGridSlaveAddress.Size = new System.Drawing.Size(166, 20);
            this.m_binaryGridSlaveAddress.TabIndex = 1;
            this.m_binaryGridSlaveAddress.TabStop = false;
            // 
            // m_lblBit1
            // 
            this.m_lblBit1.AutoSize = true;
            this.m_lblBit1.Location = new System.Drawing.Point(359, 8);
            this.m_lblBit1.Name = "m_lblBit1";
            this.m_lblBit1.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit1.TabIndex = 0;
            this.m_lblBit1.Text = "1";
            // 
            // m_lblSlaveAddrMask
            // 
            this.m_lblSlaveAddrMask.AutoSize = true;
            this.m_lblSlaveAddrMask.Location = new System.Drawing.Point(4, 56);
            this.m_lblSlaveAddrMask.Name = "m_lblSlaveAddrMask";
            this.m_lblSlaveAddrMask.Size = new System.Drawing.Size(105, 13);
            this.m_lblSlaveAddrMask.TabIndex = 0;
            this.m_lblSlaveAddrMask.Text = "Slave address mask:";
            // 
            // m_lblBit7
            // 
            this.m_lblBit7.AutoSize = true;
            this.m_lblBit7.Location = new System.Drawing.Point(233, 8);
            this.m_lblBit7.Name = "m_lblBit7";
            this.m_lblBit7.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit7.TabIndex = 0;
            this.m_lblBit7.Text = "7";
            // 
            // m_txtSlaveAddressMask
            // 
            this.m_txtSlaveAddressMask.Location = new System.Drawing.Point(131, 53);
            this.m_txtSlaveAddressMask.MaxLength = 5;
            this.m_txtSlaveAddressMask.Name = "m_txtSlaveAddressMask";
            this.m_txtSlaveAddressMask.Size = new System.Drawing.Size(50, 20);
            this.m_txtSlaveAddressMask.TabIndex = 2;
            // 
            // m_lblBit6
            // 
            this.m_lblBit6.AutoSize = true;
            this.m_lblBit6.Location = new System.Drawing.Point(254, 8);
            this.m_lblBit6.Name = "m_lblBit6";
            this.m_lblBit6.Size = new System.Drawing.Size(13, 13);
            this.m_lblBit6.TabIndex = 0;
            this.m_lblBit6.Text = "6";
            // 
            // m_chbAcceptAddress
            // 
            this.m_chbAcceptAddress.AutoSize = true;
            this.m_chbAcceptAddress.Location = new System.Drawing.Point(7, 79);
            this.m_chbAcceptAddress.Name = "m_chbAcceptAddress";
            this.m_chbAcceptAddress.Size = new System.Drawing.Size(201, 17);
            this.m_chbAcceptAddress.TabIndex = 4;
            this.m_chbAcceptAddress.Text = "Accept matching address in RX FIFO";
            this.m_chbAcceptAddress.UseVisualStyleBackColor = true;
            // 
            // CyI2CTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_panelModes);
            this.Controls.Add(this.m_panelSlaveConf);
            this.Name = "CyI2CTab";
            this.Size = new System.Drawing.Size(402, 253);
            this.Load += new System.EventHandler(this.CyI2CTab_Load);
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactorHigh)).EndInit();
            this.m_panelModes.ResumeLayout(false);
            this.m_panelModes.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactorLow)).EndInit();
            this.m_panelSlaveConf.ResumeLayout(false);
            this.m_panelSlaveConf.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label m_lblMode;
        private System.Windows.Forms.Label m_lblDataRate;
        private System.Windows.Forms.NumericUpDown m_numOversamplingFactor;
        private System.Windows.Forms.Label m_lblOversamplingFactorHigh;
        private System.Windows.Forms.ComboBox m_cbDataRate;
        private System.Windows.Forms.Label m_lblSlaveAddress;
        private System.Windows.Forms.NumericUpDown m_numOversamplingFactorHigh;
        private System.Windows.Forms.Label m_lblActualDataRate;
        private System.Windows.Forms.Panel m_panelModes;
        private System.Windows.Forms.ComboBox m_cbMode;
        private System.Windows.Forms.NumericUpDown m_numOversamplingFactorLow;
        private System.Windows.Forms.Label m_lblOversamplingFactorLow;
        private System.Windows.Forms.Label m_lblOversamplingFactor;
        private System.Windows.Forms.CheckBox m_chbEnableMedianFilter;
        private System.Windows.Forms.Label m_lblSeparator1;
        private System.Windows.Forms.Panel m_panelSlaveConf;
        private CyBinaryGrid m_binaryGridSlaveAddressMask;
        private CyBinaryGrid m_binaryGridSlaveAddress;
        private System.Windows.Forms.Label m_lblBit0;
        private System.Windows.Forms.Label m_lblBit1;
        private System.Windows.Forms.TextBox m_txtSlaveAddress;
        private System.Windows.Forms.CheckBox m_chbEnableWakeup;
        private System.Windows.Forms.Label m_lblSlaveAddrMask;
        private System.Windows.Forms.Label m_lblBit7;
        private System.Windows.Forms.Label m_lblBit;
        private System.Windows.Forms.TextBox m_txtSlaveAddressMask;
        private System.Windows.Forms.Label m_lblBit2;
        private System.Windows.Forms.Label m_lblBit6;
        private System.Windows.Forms.CheckBox m_chbAcceptAddress;
        private System.Windows.Forms.Label m_lblBit5;
        private System.Windows.Forms.Label m_lblBit3;
        private System.Windows.Forms.Label m_lblBit4;
        private System.Windows.Forms.CheckBox m_chbClockFromTerminal;
        private System.Windows.Forms.Label m_lblDataRateUnknown;




    }
}
