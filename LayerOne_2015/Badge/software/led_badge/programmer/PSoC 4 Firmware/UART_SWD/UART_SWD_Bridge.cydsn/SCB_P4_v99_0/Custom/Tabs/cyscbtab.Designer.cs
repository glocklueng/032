namespace SCB_P4_v99_0
{
    partial class CySCBTab
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
            this.m_chbSPI_MOSI = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_MISO = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_CLK = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_SS0 = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_SS1 = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_SS2 = new System.Windows.Forms.CheckBox();
            this.m_chbSPI_SS3 = new System.Windows.Forms.CheckBox();
            this.m_chbRxWake = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // m_chbSPI_MOSI
            // 
            this.m_chbSPI_MOSI.AutoSize = true;
            this.m_chbSPI_MOSI.Location = new System.Drawing.Point(4, 4);
            this.m_chbSPI_MOSI.Name = "m_chbSPI_MOSI";
            this.m_chbSPI_MOSI.Size = new System.Drawing.Size(182, 17);
            this.m_chbSPI_MOSI.TabIndex = 0;
            this.m_chbSPI_MOSI.Text = "SPI MOSI / I2C SCL / UART RX";
            this.m_chbSPI_MOSI.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_MISO
            // 
            this.m_chbSPI_MISO.AutoSize = true;
            this.m_chbSPI_MISO.Location = new System.Drawing.Point(4, 50);
            this.m_chbSPI_MISO.Name = "m_chbSPI_MISO";
            this.m_chbSPI_MISO.Size = new System.Drawing.Size(183, 17);
            this.m_chbSPI_MISO.TabIndex = 2;
            this.m_chbSPI_MISO.Text = "SPI MISO / I2C SDA / UART TX";
            this.m_chbSPI_MISO.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_CLK
            // 
            this.m_chbSPI_CLK.AutoSize = true;
            this.m_chbSPI_CLK.Location = new System.Drawing.Point(4, 73);
            this.m_chbSPI_CLK.Name = "m_chbSPI_CLK";
            this.m_chbSPI_CLK.Size = new System.Drawing.Size(73, 17);
            this.m_chbSPI_CLK.TabIndex = 3;
            this.m_chbSPI_CLK.Text = "SPI SCLK";
            this.m_chbSPI_CLK.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_SS0
            // 
            this.m_chbSPI_SS0.AutoSize = true;
            this.m_chbSPI_SS0.Location = new System.Drawing.Point(4, 96);
            this.m_chbSPI_SS0.Name = "m_chbSPI_SS0";
            this.m_chbSPI_SS0.Size = new System.Drawing.Size(66, 17);
            this.m_chbSPI_SS0.TabIndex = 4;
            this.m_chbSPI_SS0.Text = "SPI SS0";
            this.m_chbSPI_SS0.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_SS1
            // 
            this.m_chbSPI_SS1.AutoSize = true;
            this.m_chbSPI_SS1.Location = new System.Drawing.Point(4, 119);
            this.m_chbSPI_SS1.Name = "m_chbSPI_SS1";
            this.m_chbSPI_SS1.Size = new System.Drawing.Size(66, 17);
            this.m_chbSPI_SS1.TabIndex = 5;
            this.m_chbSPI_SS1.Text = "SPI SS1";
            this.m_chbSPI_SS1.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_SS2
            // 
            this.m_chbSPI_SS2.AutoSize = true;
            this.m_chbSPI_SS2.Location = new System.Drawing.Point(4, 142);
            this.m_chbSPI_SS2.Name = "m_chbSPI_SS2";
            this.m_chbSPI_SS2.Size = new System.Drawing.Size(66, 17);
            this.m_chbSPI_SS2.TabIndex = 6;
            this.m_chbSPI_SS2.Text = "SPI SS2";
            this.m_chbSPI_SS2.UseVisualStyleBackColor = true;
            // 
            // m_chbSPI_SS3
            // 
            this.m_chbSPI_SS3.AutoSize = true;
            this.m_chbSPI_SS3.Location = new System.Drawing.Point(4, 165);
            this.m_chbSPI_SS3.Name = "m_chbSPI_SS3";
            this.m_chbSPI_SS3.Size = new System.Drawing.Size(66, 17);
            this.m_chbSPI_SS3.TabIndex = 7;
            this.m_chbSPI_SS3.Text = "SPI SS3";
            this.m_chbSPI_SS3.UseVisualStyleBackColor = true;
            // 
            // m_chbRxWake
            // 
            this.m_chbRxWake.AutoSize = true;
            this.m_chbRxWake.Location = new System.Drawing.Point(23, 27);
            this.m_chbRxWake.Name = "m_chbRxWake";
            this.m_chbRxWake.Size = new System.Drawing.Size(139, 17);
            this.m_chbRxWake.TabIndex = 1;
            this.m_chbRxWake.Text = "Enable UART RX wake";
            this.m_chbRxWake.UseVisualStyleBackColor = true;
            // 
            // CySCBTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_chbRxWake);
            this.Controls.Add(this.m_chbSPI_SS3);
            this.Controls.Add(this.m_chbSPI_SS2);
            this.Controls.Add(this.m_chbSPI_SS1);
            this.Controls.Add(this.m_chbSPI_SS0);
            this.Controls.Add(this.m_chbSPI_CLK);
            this.Controls.Add(this.m_chbSPI_MISO);
            this.Controls.Add(this.m_chbSPI_MOSI);
            this.Name = "CySCBTab";
            this.Size = new System.Drawing.Size(300, 185);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox m_chbSPI_MOSI;
        private System.Windows.Forms.CheckBox m_chbSPI_MISO;
        private System.Windows.Forms.CheckBox m_chbSPI_CLK;
        private System.Windows.Forms.CheckBox m_chbSPI_SS0;
        private System.Windows.Forms.CheckBox m_chbSPI_SS1;
        private System.Windows.Forms.CheckBox m_chbSPI_SS2;
        private System.Windows.Forms.CheckBox m_chbSPI_SS3;
        private System.Windows.Forms.CheckBox m_chbRxWake;
    }
}
