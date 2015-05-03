namespace SCB_P4_v99_0
{
    partial class CyUARTConfigureTab
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
            this.m_chbEnableWakeup = new System.Windows.Forms.CheckBox();
            this.m_cbBaudRate = new System.Windows.Forms.ComboBox();
            this.m_numOversamplingFactor = new System.Windows.Forms.NumericUpDown();
            this.m_lblActualBaudRate = new System.Windows.Forms.Label();
            this.m_chbEnableMedianFilter = new System.Windows.Forms.CheckBox();
            this.m_lblOversample = new System.Windows.Forms.Label();
            this.m_lblBaudRate = new System.Windows.Forms.Label();
            this.m_lblMode = new System.Windows.Forms.Label();
            this.m_lblDirection = new System.Windows.Forms.Label();
            this.m_cbDirection = new System.Windows.Forms.ComboBox();
            this.m_cbMode = new System.Windows.Forms.ComboBox();
            this.m_cbDataBits = new System.Windows.Forms.ComboBox();
            this.m_cbParityType = new System.Windows.Forms.ComboBox();
            this.m_lblDataBits = new System.Windows.Forms.Label();
            this.m_lblParityType = new System.Windows.Forms.Label();
            this.m_lblStopBits = new System.Windows.Forms.Label();
            this.m_cbStopBits = new System.Windows.Forms.ComboBox();
            this.m_chbRetryOnNack = new System.Windows.Forms.CheckBox();
            this.m_chbInvertRx = new System.Windows.Forms.CheckBox();
            this.m_chbClockFromTerminal = new System.Windows.Forms.CheckBox();
            this.m_chbLowPowerReceive = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).BeginInit();
            this.SuspendLayout();
            // 
            // m_chbEnableWakeup
            // 
            this.m_chbEnableWakeup.AutoSize = true;
            this.m_chbEnableWakeup.Location = new System.Drawing.Point(6, 286);
            this.m_chbEnableWakeup.Margin = new System.Windows.Forms.Padding(2);
            this.m_chbEnableWakeup.Name = "m_chbEnableWakeup";
            this.m_chbEnableWakeup.Size = new System.Drawing.Size(183, 17);
            this.m_chbEnableWakeup.TabIndex = 11;
            this.m_chbEnableWakeup.Text = "Enable wakeup from Sleep Mode";
            this.m_chbEnableWakeup.UseVisualStyleBackColor = true;
            // 
            // m_cbBaudRate
            // 
            this.m_cbBaudRate.FormattingEnabled = true;
            this.m_cbBaudRate.Location = new System.Drawing.Point(113, 61);
            this.m_cbBaudRate.Name = "m_cbBaudRate";
            this.m_cbBaudRate.Size = new System.Drawing.Size(76, 21);
            this.m_cbBaudRate.TabIndex = 2;
            // 
            // m_numOversamplingFactor
            // 
            this.m_numOversamplingFactor.Location = new System.Drawing.Point(113, 169);
            this.m_numOversamplingFactor.Maximum = new decimal(new int[] {
            320000,
            0,
            0,
            0});
            this.m_numOversamplingFactor.Name = "m_numOversamplingFactor";
            this.m_numOversamplingFactor.Size = new System.Drawing.Size(60, 20);
            this.m_numOversamplingFactor.TabIndex = 6;
            // 
            // m_lblActualBaudRate
            // 
            this.m_lblActualBaudRate.AutoSize = true;
            this.m_lblActualBaudRate.Location = new System.Drawing.Point(203, 64);
            this.m_lblActualBaudRate.Name = "m_lblActualBaudRate";
            this.m_lblActualBaudRate.Size = new System.Drawing.Size(181, 13);
            this.m_lblActualBaudRate.TabIndex = 0;
            this.m_lblActualBaudRate.Text = "Actual baud rate (kbps): UNKNOWN";
            // 
            // m_chbEnableMedianFilter
            // 
            this.m_chbEnableMedianFilter.AutoSize = true;
            this.m_chbEnableMedianFilter.Location = new System.Drawing.Point(6, 218);
            this.m_chbEnableMedianFilter.Name = "m_chbEnableMedianFilter";
            this.m_chbEnableMedianFilter.Size = new System.Drawing.Size(83, 17);
            this.m_chbEnableMedianFilter.TabIndex = 8;
            this.m_chbEnableMedianFilter.Text = "Median filter";
            this.m_chbEnableMedianFilter.UseVisualStyleBackColor = true;
            // 
            // m_lblOversample
            // 
            this.m_lblOversample.AutoSize = true;
            this.m_lblOversample.Location = new System.Drawing.Point(3, 171);
            this.m_lblOversample.Name = "m_lblOversample";
            this.m_lblOversample.Size = new System.Drawing.Size(74, 13);
            this.m_lblOversample.TabIndex = 0;
            this.m_lblOversample.Text = "Oversampling:";
            // 
            // m_lblBaudRate
            // 
            this.m_lblBaudRate.AutoSize = true;
            this.m_lblBaudRate.Location = new System.Drawing.Point(3, 64);
            this.m_lblBaudRate.Name = "m_lblBaudRate";
            this.m_lblBaudRate.Size = new System.Drawing.Size(82, 13);
            this.m_lblBaudRate.TabIndex = 0;
            this.m_lblBaudRate.Text = "Baud rate (bps):";
            // 
            // m_lblMode
            // 
            this.m_lblMode.AutoSize = true;
            this.m_lblMode.Location = new System.Drawing.Point(3, 10);
            this.m_lblMode.Name = "m_lblMode";
            this.m_lblMode.Size = new System.Drawing.Size(37, 13);
            this.m_lblMode.TabIndex = 0;
            this.m_lblMode.Text = "Mode:";
            // 
            // m_lblDirection
            // 
            this.m_lblDirection.AutoSize = true;
            this.m_lblDirection.Location = new System.Drawing.Point(3, 37);
            this.m_lblDirection.Name = "m_lblDirection";
            this.m_lblDirection.Size = new System.Drawing.Size(52, 13);
            this.m_lblDirection.TabIndex = 0;
            this.m_lblDirection.Text = "Direction:";
            // 
            // m_cbDirection
            // 
            this.m_cbDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbDirection.FormattingEnabled = true;
            this.m_cbDirection.Location = new System.Drawing.Point(113, 34);
            this.m_cbDirection.Name = "m_cbDirection";
            this.m_cbDirection.Size = new System.Drawing.Size(76, 21);
            this.m_cbDirection.TabIndex = 1;
            // 
            // m_cbMode
            // 
            this.m_cbMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbMode.FormattingEnabled = true;
            this.m_cbMode.Location = new System.Drawing.Point(113, 7);
            this.m_cbMode.Name = "m_cbMode";
            this.m_cbMode.Size = new System.Drawing.Size(76, 21);
            this.m_cbMode.TabIndex = 0;
            // 
            // m_cbDataBits
            // 
            this.m_cbDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbDataBits.FormattingEnabled = true;
            this.m_cbDataBits.Location = new System.Drawing.Point(113, 88);
            this.m_cbDataBits.Name = "m_cbDataBits";
            this.m_cbDataBits.Size = new System.Drawing.Size(76, 21);
            this.m_cbDataBits.TabIndex = 3;
            // 
            // m_cbParityType
            // 
            this.m_cbParityType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbParityType.FormattingEnabled = true;
            this.m_cbParityType.Location = new System.Drawing.Point(113, 115);
            this.m_cbParityType.Name = "m_cbParityType";
            this.m_cbParityType.Size = new System.Drawing.Size(76, 21);
            this.m_cbParityType.TabIndex = 4;
            // 
            // m_lblDataBits
            // 
            this.m_lblDataBits.AutoSize = true;
            this.m_lblDataBits.Location = new System.Drawing.Point(3, 91);
            this.m_lblDataBits.Name = "m_lblDataBits";
            this.m_lblDataBits.Size = new System.Drawing.Size(52, 13);
            this.m_lblDataBits.TabIndex = 0;
            this.m_lblDataBits.Text = "Data bits:";
            // 
            // m_lblParityType
            // 
            this.m_lblParityType.AutoSize = true;
            this.m_lblParityType.Location = new System.Drawing.Point(3, 118);
            this.m_lblParityType.Name = "m_lblParityType";
            this.m_lblParityType.Size = new System.Drawing.Size(36, 13);
            this.m_lblParityType.TabIndex = 0;
            this.m_lblParityType.Text = "Parity:";
            // 
            // m_lblStopBits
            // 
            this.m_lblStopBits.AutoSize = true;
            this.m_lblStopBits.Location = new System.Drawing.Point(3, 145);
            this.m_lblStopBits.Name = "m_lblStopBits";
            this.m_lblStopBits.Size = new System.Drawing.Size(51, 13);
            this.m_lblStopBits.TabIndex = 0;
            this.m_lblStopBits.Text = "Stop bits:";
            // 
            // m_cbStopBits
            // 
            this.m_cbStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbStopBits.FormattingEnabled = true;
            this.m_cbStopBits.Location = new System.Drawing.Point(113, 142);
            this.m_cbStopBits.Name = "m_cbStopBits";
            this.m_cbStopBits.Size = new System.Drawing.Size(76, 21);
            this.m_cbStopBits.TabIndex = 5;
            // 
            // m_chbRetryOnNack
            // 
            this.m_chbRetryOnNack.AutoSize = true;
            this.m_chbRetryOnNack.Location = new System.Drawing.Point(6, 241);
            this.m_chbRetryOnNack.Name = "m_chbRetryOnNack";
            this.m_chbRetryOnNack.Size = new System.Drawing.Size(98, 17);
            this.m_chbRetryOnNack.TabIndex = 9;
            this.m_chbRetryOnNack.Text = "Retry on NACK";
            this.m_chbRetryOnNack.UseVisualStyleBackColor = true;
            // 
            // m_chbInvertRx
            // 
            this.m_chbInvertRx.AutoSize = true;
            this.m_chbInvertRx.Location = new System.Drawing.Point(6, 264);
            this.m_chbInvertRx.Name = "m_chbInvertRx";
            this.m_chbInvertRx.Size = new System.Drawing.Size(85, 17);
            this.m_chbInvertRx.TabIndex = 10;
            this.m_chbInvertRx.Text = "Inverting RX";
            this.m_chbInvertRx.UseVisualStyleBackColor = true;
            // 
            // m_chbClockFromTerminal
            // 
            this.m_chbClockFromTerminal.AutoSize = true;
            this.m_chbClockFromTerminal.Location = new System.Drawing.Point(6, 195);
            this.m_chbClockFromTerminal.Name = "m_chbClockFromTerminal";
            this.m_chbClockFromTerminal.Size = new System.Drawing.Size(115, 17);
            this.m_chbClockFromTerminal.TabIndex = 7;
            this.m_chbClockFromTerminal.Text = "Clock from terminal";
            this.m_chbClockFromTerminal.UseVisualStyleBackColor = false;
            // 
            // m_chbLowPowerReceive
            // 
            this.m_chbLowPowerReceive.AutoSize = true;
            this.m_chbLowPowerReceive.Location = new System.Drawing.Point(6, 308);
            this.m_chbLowPowerReceive.Name = "m_chbLowPowerReceive";
            this.m_chbLowPowerReceive.Size = new System.Drawing.Size(124, 17);
            this.m_chbLowPowerReceive.TabIndex = 12;
            this.m_chbLowPowerReceive.Text = "Low power receiving";
            this.m_chbLowPowerReceive.UseVisualStyleBackColor = true;
            // 
            // CyUARTConfigureTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_chbLowPowerReceive);
            this.Controls.Add(this.m_chbClockFromTerminal);
            this.Controls.Add(this.m_chbInvertRx);
            this.Controls.Add(this.m_chbRetryOnNack);
            this.Controls.Add(this.m_cbStopBits);
            this.Controls.Add(this.m_lblStopBits);
            this.Controls.Add(this.m_lblParityType);
            this.Controls.Add(this.m_lblDataBits);
            this.Controls.Add(this.m_cbParityType);
            this.Controls.Add(this.m_cbDataBits);
            this.Controls.Add(this.m_cbMode);
            this.Controls.Add(this.m_lblMode);
            this.Controls.Add(this.m_chbEnableWakeup);
            this.Controls.Add(this.m_cbBaudRate);
            this.Controls.Add(this.m_numOversamplingFactor);
            this.Controls.Add(this.m_lblActualBaudRate);
            this.Controls.Add(this.m_chbEnableMedianFilter);
            this.Controls.Add(this.m_lblOversample);
            this.Controls.Add(this.m_lblBaudRate);
            this.Controls.Add(this.m_cbDirection);
            this.Controls.Add(this.m_lblDirection);
            this.Name = "CyUARTConfigureTab";
            this.Size = new System.Drawing.Size(415, 329);
            ((System.ComponentModel.ISupportInitialize)(this.m_numOversamplingFactor)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox m_chbEnableWakeup;
        private System.Windows.Forms.ComboBox m_cbBaudRate;
        private System.Windows.Forms.NumericUpDown m_numOversamplingFactor;
        private System.Windows.Forms.Label m_lblActualBaudRate;
        private System.Windows.Forms.CheckBox m_chbEnableMedianFilter;
        private System.Windows.Forms.Label m_lblOversample;
        private System.Windows.Forms.Label m_lblBaudRate;
        private System.Windows.Forms.Label m_lblMode;
        private System.Windows.Forms.Label m_lblDirection;
        private System.Windows.Forms.ComboBox m_cbDirection;
        private System.Windows.Forms.ComboBox m_cbMode;
        private System.Windows.Forms.ComboBox m_cbDataBits;
        private System.Windows.Forms.ComboBox m_cbParityType;
        private System.Windows.Forms.Label m_lblDataBits;
        private System.Windows.Forms.Label m_lblParityType;
        private System.Windows.Forms.Label m_lblStopBits;
        private System.Windows.Forms.ComboBox m_cbStopBits;
        private System.Windows.Forms.CheckBox m_chbRetryOnNack;
        private System.Windows.Forms.CheckBox m_chbInvertRx;
        private System.Windows.Forms.CheckBox m_chbClockFromTerminal;
        private System.Windows.Forms.CheckBox m_chbLowPowerReceive;

    }
}
