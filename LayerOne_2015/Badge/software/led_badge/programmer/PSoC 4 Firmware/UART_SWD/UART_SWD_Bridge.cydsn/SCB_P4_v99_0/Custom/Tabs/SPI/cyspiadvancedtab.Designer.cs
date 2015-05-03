namespace SCB_P4_v99_0
{
    partial class CySPIAdvancedTab
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
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.m_numTXBufferSize = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.m_numRXBufferSize = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.m_chbInterruptSPIBusError = new System.Windows.Forms.CheckBox();
            this.m_cbTXTriggerLevel = new System.Windows.Forms.ComboBox();
            this.m_chbInterruptRXNotEmpty = new System.Windows.Forms.CheckBox();
            this.m_cbRXTriggerLevel = new System.Windows.Forms.ComboBox();
            this.m_chbInterruptRXFull = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptTXTrigger = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptSPIDone = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptRXOverflow = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptTXUnderflow = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptRXUnderflow = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptTXOverflow = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptRXTrigger = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptTXEmpty = new System.Windows.Forms.CheckBox();
            this.m_chbInterruptTXNotFull = new System.Windows.Forms.CheckBox();
            this.m_rbNone = new System.Windows.Forms.RadioButton();
            this.m_rbExternal = new System.Windows.Forms.RadioButton();
            this.m_rbInternal = new System.Windows.Forms.RadioButton();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numTXBufferSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numRXBufferSize)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.m_numTXBufferSize);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.m_numRXBufferSize);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Location = new System.Drawing.Point(5, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(182, 71);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Buffer size";
            // 
            // m_numTXBufferSize
            // 
            this.m_numTXBufferSize.Location = new System.Drawing.Point(96, 44);
            this.m_numTXBufferSize.Name = "m_numTXBufferSize";
            this.m_numTXBufferSize.Size = new System.Drawing.Size(68, 20);
            this.m_numTXBufferSize.TabIndex = 1;
            this.m_numTXBufferSize.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 46);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "TX buffer size:";
            // 
            // m_numRXBufferSize
            // 
            this.m_numRXBufferSize.Location = new System.Drawing.Point(96, 18);
            this.m_numRXBufferSize.Name = "m_numRXBufferSize";
            this.m_numRXBufferSize.Size = new System.Drawing.Size(68, 20);
            this.m_numRXBufferSize.TabIndex = 0;
            this.m_numRXBufferSize.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(76, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "RX buffer size:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.m_chbInterruptSPIBusError);
            this.groupBox1.Controls.Add(this.m_cbTXTriggerLevel);
            this.groupBox1.Controls.Add(this.m_chbInterruptRXNotEmpty);
            this.groupBox1.Controls.Add(this.m_cbRXTriggerLevel);
            this.groupBox1.Controls.Add(this.m_chbInterruptRXFull);
            this.groupBox1.Controls.Add(this.m_chbInterruptTXTrigger);
            this.groupBox1.Controls.Add(this.m_chbInterruptSPIDone);
            this.groupBox1.Controls.Add(this.m_chbInterruptRXOverflow);
            this.groupBox1.Controls.Add(this.m_chbInterruptTXUnderflow);
            this.groupBox1.Controls.Add(this.m_chbInterruptRXUnderflow);
            this.groupBox1.Controls.Add(this.m_chbInterruptTXOverflow);
            this.groupBox1.Controls.Add(this.m_chbInterruptRXTrigger);
            this.groupBox1.Controls.Add(this.m_chbInterruptTXEmpty);
            this.groupBox1.Controls.Add(this.m_chbInterruptTXNotFull);
            this.groupBox1.Location = new System.Drawing.Point(5, 81);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(418, 165);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Interrupt sources";
            // 
            // m_chbInterruptSPIBusError
            // 
            this.m_chbInterruptSPIBusError.AutoSize = true;
            this.m_chbInterruptSPIBusError.Location = new System.Drawing.Point(235, 19);
            this.m_chbInterruptSPIBusError.Name = "m_chbInterruptSPIBusError";
            this.m_chbInterruptSPIBusError.Size = new System.Drawing.Size(87, 17);
            this.m_chbInterruptSPIBusError.TabIndex = 7;
            this.m_chbInterruptSPIBusError.Text = "SPI bus error";
            this.m_chbInterruptSPIBusError.UseVisualStyleBackColor = true;
            // 
            // m_cbTXTriggerLevel
            // 
            this.m_cbTXTriggerLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbTXTriggerLevel.Enabled = false;
            this.m_cbTXTriggerLevel.FormattingEnabled = true;
            this.m_cbTXTriggerLevel.Location = new System.Drawing.Point(110, 132);
            this.m_cbTXTriggerLevel.Name = "m_cbTXTriggerLevel";
            this.m_cbTXTriggerLevel.Size = new System.Drawing.Size(58, 21);
            this.m_cbTXTriggerLevel.TabIndex = 6;
            // 
            // m_chbInterruptRXNotEmpty
            // 
            this.m_chbInterruptRXNotEmpty.AutoSize = true;
            this.m_chbInterruptRXNotEmpty.Location = new System.Drawing.Point(235, 42);
            this.m_chbInterruptRXNotEmpty.Name = "m_chbInterruptRXNotEmpty";
            this.m_chbInterruptRXNotEmpty.Size = new System.Drawing.Size(116, 17);
            this.m_chbInterruptRXNotEmpty.TabIndex = 8;
            this.m_chbInterruptRXNotEmpty.Text = "RX FIFO not empty";
            this.m_chbInterruptRXNotEmpty.UseVisualStyleBackColor = true;
            // 
            // m_cbRXTriggerLevel
            // 
            this.m_cbRXTriggerLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbRXTriggerLevel.Enabled = false;
            this.m_cbRXTriggerLevel.FormattingEnabled = true;
            this.m_cbRXTriggerLevel.Location = new System.Drawing.Point(340, 132);
            this.m_cbRXTriggerLevel.Name = "m_cbRXTriggerLevel";
            this.m_cbRXTriggerLevel.Size = new System.Drawing.Size(58, 21);
            this.m_cbRXTriggerLevel.TabIndex = 13;
            // 
            // m_chbInterruptRXFull
            // 
            this.m_chbInterruptRXFull.AutoSize = true;
            this.m_chbInterruptRXFull.Location = new System.Drawing.Point(235, 65);
            this.m_chbInterruptRXFull.Name = "m_chbInterruptRXFull";
            this.m_chbInterruptRXFull.Size = new System.Drawing.Size(83, 17);
            this.m_chbInterruptRXFull.TabIndex = 9;
            this.m_chbInterruptRXFull.Text = "RX FIFO full";
            this.m_chbInterruptRXFull.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptTXTrigger
            // 
            this.m_chbInterruptTXTrigger.AutoSize = true;
            this.m_chbInterruptTXTrigger.Location = new System.Drawing.Point(6, 134);
            this.m_chbInterruptTXTrigger.Name = "m_chbInterruptTXTrigger";
            this.m_chbInterruptTXTrigger.Size = new System.Drawing.Size(101, 17);
            this.m_chbInterruptTXTrigger.TabIndex = 5;
            this.m_chbInterruptTXTrigger.Text = "TX FIFO trigger:";
            this.m_chbInterruptTXTrigger.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptSPIDone
            // 
            this.m_chbInterruptSPIDone.AutoSize = true;
            this.m_chbInterruptSPIDone.Location = new System.Drawing.Point(6, 19);
            this.m_chbInterruptSPIDone.Name = "m_chbInterruptSPIDone";
            this.m_chbInterruptSPIDone.Size = new System.Drawing.Size(70, 17);
            this.m_chbInterruptSPIDone.TabIndex = 0;
            this.m_chbInterruptSPIDone.Text = "SPI done";
            this.m_chbInterruptSPIDone.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptRXOverflow
            // 
            this.m_chbInterruptRXOverflow.AutoSize = true;
            this.m_chbInterruptRXOverflow.Location = new System.Drawing.Point(235, 88);
            this.m_chbInterruptRXOverflow.Name = "m_chbInterruptRXOverflow";
            this.m_chbInterruptRXOverflow.Size = new System.Drawing.Size(110, 17);
            this.m_chbInterruptRXOverflow.TabIndex = 10;
            this.m_chbInterruptRXOverflow.Text = "RX FIFO overflow";
            this.m_chbInterruptRXOverflow.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptTXUnderflow
            // 
            this.m_chbInterruptTXUnderflow.AutoSize = true;
            this.m_chbInterruptTXUnderflow.Location = new System.Drawing.Point(6, 111);
            this.m_chbInterruptTXUnderflow.Name = "m_chbInterruptTXUnderflow";
            this.m_chbInterruptTXUnderflow.Size = new System.Drawing.Size(115, 17);
            this.m_chbInterruptTXUnderflow.TabIndex = 4;
            this.m_chbInterruptTXUnderflow.Text = "TX FIFO underflow";
            this.m_chbInterruptTXUnderflow.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptRXUnderflow
            // 
            this.m_chbInterruptRXUnderflow.AutoSize = true;
            this.m_chbInterruptRXUnderflow.Location = new System.Drawing.Point(235, 111);
            this.m_chbInterruptRXUnderflow.Name = "m_chbInterruptRXUnderflow";
            this.m_chbInterruptRXUnderflow.Size = new System.Drawing.Size(116, 17);
            this.m_chbInterruptRXUnderflow.TabIndex = 11;
            this.m_chbInterruptRXUnderflow.Text = "RX FIFO underflow";
            this.m_chbInterruptRXUnderflow.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptTXOverflow
            // 
            this.m_chbInterruptTXOverflow.AutoSize = true;
            this.m_chbInterruptTXOverflow.Location = new System.Drawing.Point(6, 88);
            this.m_chbInterruptTXOverflow.Name = "m_chbInterruptTXOverflow";
            this.m_chbInterruptTXOverflow.Size = new System.Drawing.Size(109, 17);
            this.m_chbInterruptTXOverflow.TabIndex = 3;
            this.m_chbInterruptTXOverflow.Text = "TX FIFO overflow";
            this.m_chbInterruptTXOverflow.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptRXTrigger
            // 
            this.m_chbInterruptRXTrigger.AutoSize = true;
            this.m_chbInterruptRXTrigger.Location = new System.Drawing.Point(235, 134);
            this.m_chbInterruptRXTrigger.Name = "m_chbInterruptRXTrigger";
            this.m_chbInterruptRXTrigger.Size = new System.Drawing.Size(102, 17);
            this.m_chbInterruptRXTrigger.TabIndex = 12;
            this.m_chbInterruptRXTrigger.Text = "RX FIFO trigger:";
            this.m_chbInterruptRXTrigger.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptTXEmpty
            // 
            this.m_chbInterruptTXEmpty.AutoSize = true;
            this.m_chbInterruptTXEmpty.Location = new System.Drawing.Point(6, 65);
            this.m_chbInterruptTXEmpty.Name = "m_chbInterruptTXEmpty";
            this.m_chbInterruptTXEmpty.Size = new System.Drawing.Size(97, 17);
            this.m_chbInterruptTXEmpty.TabIndex = 2;
            this.m_chbInterruptTXEmpty.Text = "TX FIFO empty";
            this.m_chbInterruptTXEmpty.UseVisualStyleBackColor = true;
            // 
            // m_chbInterruptTXNotFull
            // 
            this.m_chbInterruptTXNotFull.AutoSize = true;
            this.m_chbInterruptTXNotFull.Location = new System.Drawing.Point(6, 42);
            this.m_chbInterruptTXNotFull.Name = "m_chbInterruptTXNotFull";
            this.m_chbInterruptTXNotFull.Size = new System.Drawing.Size(100, 17);
            this.m_chbInterruptTXNotFull.TabIndex = 1;
            this.m_chbInterruptTXNotFull.Text = "TX FIFO not full";
            this.m_chbInterruptTXNotFull.UseVisualStyleBackColor = true;
            // 
            // m_rbNone
            // 
            this.m_rbNone.AutoSize = true;
            this.m_rbNone.Location = new System.Drawing.Point(6, 13);
            this.m_rbNone.Name = "m_rbNone";
            this.m_rbNone.Size = new System.Drawing.Size(51, 17);
            this.m_rbNone.TabIndex = 0;
            this.m_rbNone.Text = "None";
            this.m_rbNone.UseVisualStyleBackColor = true;
            // 
            // m_rbExternal
            // 
            this.m_rbExternal.AutoSize = true;
            this.m_rbExternal.Location = new System.Drawing.Point(6, 49);
            this.m_rbExternal.Name = "m_rbExternal";
            this.m_rbExternal.Size = new System.Drawing.Size(63, 17);
            this.m_rbExternal.TabIndex = 2;
            this.m_rbExternal.Text = "External";
            this.m_rbExternal.UseVisualStyleBackColor = true;
            // 
            // m_rbInternal
            // 
            this.m_rbInternal.AutoSize = true;
            this.m_rbInternal.Location = new System.Drawing.Point(6, 31);
            this.m_rbInternal.Name = "m_rbInternal";
            this.m_rbInternal.Size = new System.Drawing.Size(63, 17);
            this.m_rbInternal.TabIndex = 1;
            this.m_rbInternal.Text = "Internal ";
            this.m_rbInternal.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.m_rbNone);
            this.groupBox3.Controls.Add(this.m_rbInternal);
            this.groupBox3.Controls.Add(this.m_rbExternal);
            this.groupBox3.Location = new System.Drawing.Point(193, 4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(133, 71);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Interrupt";
            // 
            // CySPIAdvancedTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "CySPIAdvancedTab";
            this.Size = new System.Drawing.Size(428, 250);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_numTXBufferSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_numRXBufferSize)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown m_numTXBufferSize;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown m_numRXBufferSize;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox m_cbTXTriggerLevel;
        private System.Windows.Forms.CheckBox m_chbInterruptRXNotEmpty;
        private System.Windows.Forms.ComboBox m_cbRXTriggerLevel;
        private System.Windows.Forms.CheckBox m_chbInterruptRXFull;
        private System.Windows.Forms.CheckBox m_chbInterruptTXTrigger;
        private System.Windows.Forms.CheckBox m_chbInterruptSPIDone;
        private System.Windows.Forms.CheckBox m_chbInterruptRXOverflow;
        private System.Windows.Forms.CheckBox m_chbInterruptTXUnderflow;
        private System.Windows.Forms.CheckBox m_chbInterruptRXUnderflow;
        private System.Windows.Forms.CheckBox m_chbInterruptTXOverflow;
        private System.Windows.Forms.CheckBox m_chbInterruptRXTrigger;
        private System.Windows.Forms.CheckBox m_chbInterruptTXEmpty;
        private System.Windows.Forms.CheckBox m_chbInterruptTXNotFull;
        private System.Windows.Forms.RadioButton m_rbNone;
        private System.Windows.Forms.RadioButton m_rbExternal;
        private System.Windows.Forms.RadioButton m_rbInternal;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox m_chbInterruptSPIBusError;


    }
}
