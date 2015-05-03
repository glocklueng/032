namespace SCB_P4_v99_0
{
    partial class CySPIBasicTab
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
            this.m_cbBitRate = new System.Windows.Forms.ComboBox();
            this.m_numOversamplingFactor = new System.Windows.Forms.NumericUpDown();
            this.m_gbTransferSeparation = new System.Windows.Forms.GroupBox();
            this.m_rbTransferSeparated = new System.Windows.Forms.RadioButton();
            this.m_rbTransferContinuous = new System.Windows.Forms.RadioButton();
            this.m_numTXDataBits = new System.Windows.Forms.NumericUpDown();
            this.m_lblTxDataBits = new System.Windows.Forms.Label();
            this.m_lblActualBitRate = new System.Windows.Forms.Label();
            this.m_chbEnableLateMISOSample = new System.Windows.Forms.CheckBox();
            this.m_chbEnableMedianFilter = new System.Windows.Forms.CheckBox();
            this.m_numRXDataBits = new System.Windows.Forms.NumericUpDown();
            this.m_cbBitRateUnits = new System.Windows.Forms.ComboBox();
            this.m_cbBitOrder = new System.Windows.Forms.ComboBox();
            this.m_cbSSNumber = new System.Windows.Forms.ComboBox();
            this.m_cbSCLKMode = new System.Windows.Forms.ComboBox();
            this.m_lblBitOrder = new System.Windows.Forms.Label();
            this.m_lblSSNumber = new System.Windows.Forms.Label();
            this.m_lblOversample = new System.Windows.Forms.Label();
            this.m_lblDataRate = new System.Windows.Forms.Label();
            this.m_cbSubMode = new System.Windows.Forms.ComboBox();
            this.m_lblRxDataBits = new System.Windows.Forms.Label();
            this.m_lblSclkMode = new System.Windows.Forms.Label();
            this.m_lblSubMode = new System.Windows.Forms.Label();
            this.m_pictDrawing = new System.Windows.Forms.PictureBox();
            this.m_chbEnableWakeup = new System.Windows.Forms.CheckBox();
            this.m_lblMode = new System.Windows.Forms.Label();
            this.m_cbMode = new System.Windows.Forms.ComboBox();
            this.m_panelBitProperties = new System.Windows.Forms.Panel();
            this.m_lblSeparator = new System.Windows.Forms.Label();
            this.m_chbClockFromTerminal = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).BeginInit();
            this.m_gbTransferSeparation.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numTXDataBits)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numRXDataBits)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_pictDrawing)).BeginInit();
            this.m_panelBitProperties.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_cbBitRate
            // 
            this.m_cbBitRate.FormattingEnabled = true;
            this.m_cbBitRate.Location = new System.Drawing.Point(83, 215);
            this.m_cbBitRate.Name = "m_cbBitRate";
            this.m_cbBitRate.Size = new System.Drawing.Size(75, 21);
            this.m_cbBitRate.TabIndex = 3;
            // 
            // m_numOversamplingFactor
            // 
            this.m_numOversamplingFactor.Location = new System.Drawing.Point(83, 242);
            this.m_numOversamplingFactor.Maximum = new decimal(new int[] {
            320000,
            0,
            0,
            0});
            this.m_numOversamplingFactor.Name = "m_numOversamplingFactor";
            this.m_numOversamplingFactor.Size = new System.Drawing.Size(60, 20);
            this.m_numOversamplingFactor.TabIndex = 5;
            this.m_numOversamplingFactor.Validating += new System.ComponentModel.CancelEventHandler(this.m_num_Validating);
            // 
            // m_gbTransferSeparation
            // 
            this.m_gbTransferSeparation.Controls.Add(this.m_rbTransferSeparated);
            this.m_gbTransferSeparation.Controls.Add(this.m_rbTransferContinuous);
            this.m_gbTransferSeparation.Location = new System.Drawing.Point(209, 360);
            this.m_gbTransferSeparation.Name = "m_gbTransferSeparation";
            this.m_gbTransferSeparation.Size = new System.Drawing.Size(132, 72);
            this.m_gbTransferSeparation.TabIndex = 11;
            this.m_gbTransferSeparation.TabStop = false;
            this.m_gbTransferSeparation.Text = "Transfer separation";
            // 
            // m_rbTransferSeparated
            // 
            this.m_rbTransferSeparated.AutoSize = true;
            this.m_rbTransferSeparated.Location = new System.Drawing.Point(6, 42);
            this.m_rbTransferSeparated.Name = "m_rbTransferSeparated";
            this.m_rbTransferSeparated.Size = new System.Drawing.Size(74, 17);
            this.m_rbTransferSeparated.TabIndex = 1;
            this.m_rbTransferSeparated.Text = "Separated";
            this.m_rbTransferSeparated.UseVisualStyleBackColor = true;
            this.m_rbTransferSeparated.CheckedChanged += new System.EventHandler(this.m_rbTransfer_CheckedChanged);
            // 
            // m_rbTransferContinuous
            // 
            this.m_rbTransferContinuous.AutoSize = true;
            this.m_rbTransferContinuous.Checked = true;
            this.m_rbTransferContinuous.Location = new System.Drawing.Point(6, 19);
            this.m_rbTransferContinuous.Name = "m_rbTransferContinuous";
            this.m_rbTransferContinuous.Size = new System.Drawing.Size(78, 17);
            this.m_rbTransferContinuous.TabIndex = 0;
            this.m_rbTransferContinuous.TabStop = true;
            this.m_rbTransferContinuous.Text = "Continuous";
            this.m_rbTransferContinuous.UseVisualStyleBackColor = true;
            this.m_rbTransferContinuous.CheckedChanged += new System.EventHandler(this.m_rbTransfer_CheckedChanged);
            // 
            // m_numTXDataBits
            // 
            this.m_numTXDataBits.Location = new System.Drawing.Point(85, 4);
            this.m_numTXDataBits.Maximum = new decimal(new int[] {
            3200000,
            0,
            0,
            0});
            this.m_numTXDataBits.Name = "m_numTXDataBits";
            this.m_numTXDataBits.Size = new System.Drawing.Size(88, 20);
            this.m_numTXDataBits.TabIndex = 1;
            this.m_numTXDataBits.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.m_numTXDataBits.Validating += new System.ComponentModel.CancelEventHandler(this.m_num_Validating);
            // 
            // m_lblTxDataBits
            // 
            this.m_lblTxDataBits.AutoSize = true;
            this.m_lblTxDataBits.Location = new System.Drawing.Point(3, 6);
            this.m_lblTxDataBits.Name = "m_lblTxDataBits";
            this.m_lblTxDataBits.Size = new System.Drawing.Size(67, 13);
            this.m_lblTxDataBits.TabIndex = 0;
            this.m_lblTxDataBits.Text = "TX data bits:";
            // 
            // m_lblActualBitRate
            // 
            this.m_lblActualBitRate.AutoSize = true;
            this.m_lblActualBitRate.Location = new System.Drawing.Point(238, 218);
            this.m_lblActualBitRate.Name = "m_lblActualBitRate";
            this.m_lblActualBitRate.Size = new System.Drawing.Size(164, 13);
            this.m_lblActualBitRate.TabIndex = 0;
            this.m_lblActualBitRate.Text = "Actual data rate (kbps): unknown";
            // 
            // m_chbEnableLateMISOSample
            // 
            this.m_chbEnableLateMISOSample.AutoSize = true;
            this.m_chbEnableLateMISOSample.Location = new System.Drawing.Point(3, 314);
            this.m_chbEnableLateMISOSample.Name = "m_chbEnableLateMISOSample";
            this.m_chbEnableLateMISOSample.Size = new System.Drawing.Size(117, 17);
            this.m_chbEnableLateMISOSample.TabIndex = 8;
            this.m_chbEnableLateMISOSample.Text = "MISO late sampling";
            this.m_chbEnableLateMISOSample.UseVisualStyleBackColor = true;
            // 
            // m_chbEnableMedianFilter
            // 
            this.m_chbEnableMedianFilter.AutoSize = true;
            this.m_chbEnableMedianFilter.Location = new System.Drawing.Point(3, 291);
            this.m_chbEnableMedianFilter.Name = "m_chbEnableMedianFilter";
            this.m_chbEnableMedianFilter.Size = new System.Drawing.Size(83, 17);
            this.m_chbEnableMedianFilter.TabIndex = 7;
            this.m_chbEnableMedianFilter.Text = "Median filter";
            this.m_chbEnableMedianFilter.UseVisualStyleBackColor = true;
            // 
            // m_numRXDataBits
            // 
            this.m_numRXDataBits.Location = new System.Drawing.Point(85, 29);
            this.m_numRXDataBits.Maximum = new decimal(new int[] {
            3200000,
            0,
            0,
            0});
            this.m_numRXDataBits.Name = "m_numRXDataBits";
            this.m_numRXDataBits.Size = new System.Drawing.Size(88, 20);
            this.m_numRXDataBits.TabIndex = 2;
            this.m_numRXDataBits.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.m_numRXDataBits.Validating += new System.ComponentModel.CancelEventHandler(this.m_num_Validating);
            // 
            // m_cbBitRateUnits
            // 
            this.m_cbBitRateUnits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbBitRateUnits.FormattingEnabled = true;
            this.m_cbBitRateUnits.Items.AddRange(new object[] {
            "kbps",
            "Mbps"});
            this.m_cbBitRateUnits.Location = new System.Drawing.Point(176, 215);
            this.m_cbBitRateUnits.Name = "m_cbBitRateUnits";
            this.m_cbBitRateUnits.Size = new System.Drawing.Size(56, 21);
            this.m_cbBitRateUnits.TabIndex = 4;
            // 
            // m_cbBitOrder
            // 
            this.m_cbBitOrder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbBitOrder.FormattingEnabled = true;
            this.m_cbBitOrder.Location = new System.Drawing.Point(85, 55);
            this.m_cbBitOrder.Name = "m_cbBitOrder";
            this.m_cbBitOrder.Size = new System.Drawing.Size(88, 21);
            this.m_cbBitOrder.TabIndex = 3;
            // 
            // m_cbSSNumber
            // 
            this.m_cbSSNumber.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbSSNumber.FormattingEnabled = true;
            this.m_cbSSNumber.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3",
            "4"});
            this.m_cbSSNumber.Location = new System.Drawing.Point(85, 82);
            this.m_cbSSNumber.Name = "m_cbSSNumber";
            this.m_cbSSNumber.Size = new System.Drawing.Size(60, 21);
            this.m_cbSSNumber.TabIndex = 4;
            // 
            // m_cbSCLKMode
            // 
            this.m_cbSCLKMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbSCLKMode.FormattingEnabled = true;
            this.m_cbSCLKMode.Location = new System.Drawing.Point(83, 188);
            this.m_cbSCLKMode.Name = "m_cbSCLKMode";
            this.m_cbSCLKMode.Size = new System.Drawing.Size(209, 21);
            this.m_cbSCLKMode.TabIndex = 2;
            // 
            // m_lblBitOrder
            // 
            this.m_lblBitOrder.AutoSize = true;
            this.m_lblBitOrder.Location = new System.Drawing.Point(3, 58);
            this.m_lblBitOrder.Name = "m_lblBitOrder";
            this.m_lblBitOrder.Size = new System.Drawing.Size(49, 13);
            this.m_lblBitOrder.TabIndex = 0;
            this.m_lblBitOrder.Text = "Bit order:";
            // 
            // m_lblSSNumber
            // 
            this.m_lblSSNumber.AutoSize = true;
            this.m_lblSSNumber.Location = new System.Drawing.Point(3, 85);
            this.m_lblSSNumber.Name = "m_lblSSNumber";
            this.m_lblSSNumber.Size = new System.Drawing.Size(76, 13);
            this.m_lblSSNumber.TabIndex = 0;
            this.m_lblSSNumber.Text = "Number of SS:";
            // 
            // m_lblOversample
            // 
            this.m_lblOversample.AutoSize = true;
            this.m_lblOversample.Location = new System.Drawing.Point(3, 244);
            this.m_lblOversample.Name = "m_lblOversample";
            this.m_lblOversample.Size = new System.Drawing.Size(74, 13);
            this.m_lblOversample.TabIndex = 0;
            this.m_lblOversample.Text = "Oversampling:";
            // 
            // m_lblDataRate
            // 
            this.m_lblDataRate.AutoSize = true;
            this.m_lblDataRate.Location = new System.Drawing.Point(3, 218);
            this.m_lblDataRate.Name = "m_lblDataRate";
            this.m_lblDataRate.Size = new System.Drawing.Size(54, 13);
            this.m_lblDataRate.TabIndex = 0;
            this.m_lblDataRate.Text = "Data rate:";
            // 
            // m_cbSubMode
            // 
            this.m_cbSubMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbSubMode.FormattingEnabled = true;
            this.m_cbSubMode.Location = new System.Drawing.Point(83, 161);
            this.m_cbSubMode.Name = "m_cbSubMode";
            this.m_cbSubMode.Size = new System.Drawing.Size(209, 21);
            this.m_cbSubMode.TabIndex = 1;
            // 
            // m_lblRxDataBits
            // 
            this.m_lblRxDataBits.AutoSize = true;
            this.m_lblRxDataBits.Location = new System.Drawing.Point(3, 31);
            this.m_lblRxDataBits.Name = "m_lblRxDataBits";
            this.m_lblRxDataBits.Size = new System.Drawing.Size(68, 13);
            this.m_lblRxDataBits.TabIndex = 0;
            this.m_lblRxDataBits.Text = "RX data bits:";
            // 
            // m_lblSclkMode
            // 
            this.m_lblSclkMode.AutoSize = true;
            this.m_lblSclkMode.Location = new System.Drawing.Point(3, 191);
            this.m_lblSclkMode.Name = "m_lblSclkMode";
            this.m_lblSclkMode.Size = new System.Drawing.Size(66, 13);
            this.m_lblSclkMode.TabIndex = 0;
            this.m_lblSclkMode.Text = "SCLK mode:";
            // 
            // m_lblSubMode
            // 
            this.m_lblSubMode.AutoSize = true;
            this.m_lblSubMode.Location = new System.Drawing.Point(3, 164);
            this.m_lblSubMode.Name = "m_lblSubMode";
            this.m_lblSubMode.Size = new System.Drawing.Size(58, 13);
            this.m_lblSubMode.TabIndex = 0;
            this.m_lblSubMode.Text = "Sub mode:";
            // 
            // m_pictDrawing
            // 
            this.m_pictDrawing.BackColor = System.Drawing.Color.White;
            this.m_pictDrawing.Location = new System.Drawing.Point(0, 0);
            this.m_pictDrawing.Name = "m_pictDrawing";
            this.m_pictDrawing.Size = new System.Drawing.Size(433, 128);
            this.m_pictDrawing.TabIndex = 75;
            this.m_pictDrawing.TabStop = false;
            // 
            // m_chbEnableWakeup
            // 
            this.m_chbEnableWakeup.AutoSize = true;
            this.m_chbEnableWakeup.Location = new System.Drawing.Point(3, 336);
            this.m_chbEnableWakeup.Margin = new System.Windows.Forms.Padding(2);
            this.m_chbEnableWakeup.Name = "m_chbEnableWakeup";
            this.m_chbEnableWakeup.Size = new System.Drawing.Size(183, 17);
            this.m_chbEnableWakeup.TabIndex = 9;
            this.m_chbEnableWakeup.Text = "Enable wakeup from Sleep Mode";
            this.m_chbEnableWakeup.UseVisualStyleBackColor = true;
            // 
            // m_lblMode
            // 
            this.m_lblMode.AutoSize = true;
            this.m_lblMode.Location = new System.Drawing.Point(3, 137);
            this.m_lblMode.Name = "m_lblMode";
            this.m_lblMode.Size = new System.Drawing.Size(37, 13);
            this.m_lblMode.TabIndex = 0;
            this.m_lblMode.Text = "Mode:";
            // 
            // m_cbMode
            // 
            this.m_cbMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbMode.FormattingEnabled = true;
            this.m_cbMode.Location = new System.Drawing.Point(83, 134);
            this.m_cbMode.Name = "m_cbMode";
            this.m_cbMode.Size = new System.Drawing.Size(209, 21);
            this.m_cbMode.TabIndex = 0;
            // 
            // m_panelBitProperties
            // 
            this.m_panelBitProperties.Controls.Add(this.m_lblTxDataBits);
            this.m_panelBitProperties.Controls.Add(this.m_lblRxDataBits);
            this.m_panelBitProperties.Controls.Add(this.m_lblSSNumber);
            this.m_panelBitProperties.Controls.Add(this.m_lblBitOrder);
            this.m_panelBitProperties.Controls.Add(this.m_cbSSNumber);
            this.m_panelBitProperties.Controls.Add(this.m_cbBitOrder);
            this.m_panelBitProperties.Controls.Add(this.m_numRXDataBits);
            this.m_panelBitProperties.Controls.Add(this.m_numTXDataBits);
            this.m_panelBitProperties.Location = new System.Drawing.Point(3, 360);
            this.m_panelBitProperties.Name = "m_panelBitProperties";
            this.m_panelBitProperties.Size = new System.Drawing.Size(200, 109);
            this.m_panelBitProperties.TabIndex = 10;
            // 
            // m_lblSeparator
            // 
            this.m_lblSeparator.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_lblSeparator.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.m_lblSeparator.Location = new System.Drawing.Point(3, 355);
            this.m_lblSeparator.Name = "m_lblSeparator";
            this.m_lblSeparator.Size = new System.Drawing.Size(429, 2);
            this.m_lblSeparator.TabIndex = 0;
            // 
            // m_chbClockFromTerminal
            // 
            this.m_chbClockFromTerminal.AutoSize = true;
            this.m_chbClockFromTerminal.Location = new System.Drawing.Point(3, 268);
            this.m_chbClockFromTerminal.Name = "m_chbClockFromTerminal";
            this.m_chbClockFromTerminal.Size = new System.Drawing.Size(115, 17);
            this.m_chbClockFromTerminal.TabIndex = 6;
            this.m_chbClockFromTerminal.Text = "Clock from terminal";
            this.m_chbClockFromTerminal.UseVisualStyleBackColor = false;
            // 
            // CySPIBasicTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_chbClockFromTerminal);
            this.Controls.Add(this.m_lblSeparator);
            this.Controls.Add(this.m_panelBitProperties);
            this.Controls.Add(this.m_cbMode);
            this.Controls.Add(this.m_lblMode);
            this.Controls.Add(this.m_chbEnableWakeup);
            this.Controls.Add(this.m_cbBitRate);
            this.Controls.Add(this.m_numOversamplingFactor);
            this.Controls.Add(this.m_gbTransferSeparation);
            this.Controls.Add(this.m_lblActualBitRate);
            this.Controls.Add(this.m_chbEnableLateMISOSample);
            this.Controls.Add(this.m_chbEnableMedianFilter);
            this.Controls.Add(this.m_cbBitRateUnits);
            this.Controls.Add(this.m_cbSCLKMode);
            this.Controls.Add(this.m_lblOversample);
            this.Controls.Add(this.m_lblDataRate);
            this.Controls.Add(this.m_cbSubMode);
            this.Controls.Add(this.m_lblSclkMode);
            this.Controls.Add(this.m_lblSubMode);
            this.Controls.Add(this.m_pictDrawing);
            this.Name = "CySPIBasicTab";
            this.Size = new System.Drawing.Size(435, 470);
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).EndInit();
            this.m_gbTransferSeparation.ResumeLayout(false);
            this.m_gbTransferSeparation.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numTXDataBits)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numRXDataBits)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_pictDrawing)).EndInit();
            this.m_panelBitProperties.ResumeLayout(false);
            this.m_panelBitProperties.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox m_cbBitRate;
        private System.Windows.Forms.NumericUpDown m_numOversamplingFactor;
        private System.Windows.Forms.GroupBox m_gbTransferSeparation;
        private System.Windows.Forms.RadioButton m_rbTransferSeparated;
        private System.Windows.Forms.RadioButton m_rbTransferContinuous;
        private System.Windows.Forms.NumericUpDown m_numTXDataBits;
        private System.Windows.Forms.Label m_lblTxDataBits;
        private System.Windows.Forms.Label m_lblActualBitRate;
        private System.Windows.Forms.CheckBox m_chbEnableLateMISOSample;
        private System.Windows.Forms.CheckBox m_chbEnableMedianFilter;
        private System.Windows.Forms.NumericUpDown m_numRXDataBits;
        private System.Windows.Forms.ComboBox m_cbBitRateUnits;
        private System.Windows.Forms.ComboBox m_cbBitOrder;
        private System.Windows.Forms.ComboBox m_cbSSNumber;
        private System.Windows.Forms.ComboBox m_cbSCLKMode;
        private System.Windows.Forms.Label m_lblBitOrder;
        private System.Windows.Forms.Label m_lblSSNumber;
        private System.Windows.Forms.Label m_lblOversample;
        private System.Windows.Forms.Label m_lblDataRate;
        private System.Windows.Forms.ComboBox m_cbSubMode;
        private System.Windows.Forms.Label m_lblRxDataBits;
        private System.Windows.Forms.Label m_lblSclkMode;
        private System.Windows.Forms.Label m_lblSubMode;
        private System.Windows.Forms.PictureBox m_pictDrawing;
        public System.Windows.Forms.CheckBox m_chbEnableWakeup;
        private System.Windows.Forms.Label m_lblMode;
        private System.Windows.Forms.ComboBox m_cbMode;
        private System.Windows.Forms.Panel m_panelBitProperties;
        private System.Windows.Forms.Label m_lblSeparator;
        private System.Windows.Forms.CheckBox m_chbClockFromTerminal;

    }
}
