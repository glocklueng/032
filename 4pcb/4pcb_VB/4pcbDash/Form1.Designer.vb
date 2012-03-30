<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.serCOM = New System.IO.Ports.SerialPort(Me.components)
        Me.tmrTX = New System.Windows.Forms.Timer(Me.components)
        Me.cmbCOM = New System.Windows.Forms.ComboBox
        Me.btnCOM = New System.Windows.Forms.Button
        Me.tmrPaint = New System.Windows.Forms.Timer(Me.components)
        Me.trkXtrim = New System.Windows.Forms.TrackBar
        Me.trkRZtrim = New System.Windows.Forms.TrackBar
        Me.trkYtrim = New System.Windows.Forms.TrackBar
        Me.lblXtrim = New System.Windows.Forms.Label
        Me.lblRZtrim = New System.Windows.Forms.Label
        Me.lblYtrim = New System.Windows.Forms.Label
        CType(Me.trkXtrim, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.trkRZtrim, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.trkYtrim, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'serCOM
        '
        Me.serCOM.BaudRate = 57600
        Me.serCOM.ReceivedBytesThreshold = 16
        '
        'tmrTX
        '
        Me.tmrTX.Enabled = True
        Me.tmrTX.Interval = 20
        '
        'cmbCOM
        '
        Me.cmbCOM.BackColor = System.Drawing.Color.Blue
        Me.cmbCOM.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmbCOM.ForeColor = System.Drawing.Color.White
        Me.cmbCOM.FormattingEnabled = True
        Me.cmbCOM.Location = New System.Drawing.Point(12, 12)
        Me.cmbCOM.Name = "cmbCOM"
        Me.cmbCOM.Size = New System.Drawing.Size(121, 28)
        Me.cmbCOM.TabIndex = 0
        '
        'btnCOM
        '
        Me.btnCOM.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnCOM.Location = New System.Drawing.Point(12, 46)
        Me.btnCOM.Name = "btnCOM"
        Me.btnCOM.Size = New System.Drawing.Size(121, 30)
        Me.btnCOM.TabIndex = 1
        Me.btnCOM.Text = "Connect"
        Me.btnCOM.UseVisualStyleBackColor = True
        '
        'tmrPaint
        '
        Me.tmrPaint.Enabled = True
        Me.tmrPaint.Interval = 50
        '
        'trkXtrim
        '
        Me.trkXtrim.Location = New System.Drawing.Point(12, 140)
        Me.trkXtrim.Maximum = 100
        Me.trkXtrim.Minimum = -100
        Me.trkXtrim.Name = "trkXtrim"
        Me.trkXtrim.Size = New System.Drawing.Size(202, 45)
        Me.trkXtrim.TabIndex = 2
        Me.trkXtrim.TickFrequency = 10
        '
        'trkRZtrim
        '
        Me.trkRZtrim.Location = New System.Drawing.Point(12, 191)
        Me.trkRZtrim.Maximum = 100
        Me.trkRZtrim.Minimum = -100
        Me.trkRZtrim.Name = "trkRZtrim"
        Me.trkRZtrim.Size = New System.Drawing.Size(202, 45)
        Me.trkRZtrim.TabIndex = 3
        Me.trkRZtrim.TickFrequency = 10
        '
        'trkYtrim
        '
        Me.trkYtrim.Location = New System.Drawing.Point(331, 24)
        Me.trkYtrim.Maximum = 100
        Me.trkYtrim.Minimum = -100
        Me.trkYtrim.Name = "trkYtrim"
        Me.trkYtrim.Orientation = System.Windows.Forms.Orientation.Vertical
        Me.trkYtrim.Size = New System.Drawing.Size(45, 212)
        Me.trkYtrim.TabIndex = 4
        Me.trkYtrim.TickFrequency = 10
        '
        'lblXtrim
        '
        Me.lblXtrim.AutoSize = True
        Me.lblXtrim.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblXtrim.ForeColor = System.Drawing.Color.White
        Me.lblXtrim.Location = New System.Drawing.Point(220, 140)
        Me.lblXtrim.Name = "lblXtrim"
        Me.lblXtrim.Size = New System.Drawing.Size(72, 20)
        Me.lblXtrim.TabIndex = 5
        Me.lblXtrim.Text = "X-Trim: 0"
        '
        'lblRZtrim
        '
        Me.lblRZtrim.AutoSize = True
        Me.lblRZtrim.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblRZtrim.ForeColor = System.Drawing.Color.White
        Me.lblRZtrim.Location = New System.Drawing.Point(220, 191)
        Me.lblRZtrim.Name = "lblRZtrim"
        Me.lblRZtrim.Size = New System.Drawing.Size(83, 20)
        Me.lblRZtrim.TabIndex = 6
        Me.lblRZtrim.Text = "RZ-Trim: 0"
        '
        'lblYtrim
        '
        Me.lblYtrim.AutoSize = True
        Me.lblYtrim.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblYtrim.ForeColor = System.Drawing.Color.White
        Me.lblYtrim.Location = New System.Drawing.Point(242, 37)
        Me.lblYtrim.Name = "lblYtrim"
        Me.lblYtrim.Size = New System.Drawing.Size(72, 20)
        Me.lblYtrim.TabIndex = 7
        Me.lblYtrim.Text = "Y-Trim: 0"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.Black
        Me.ClientSize = New System.Drawing.Size(388, 248)
        Me.Controls.Add(Me.lblYtrim)
        Me.Controls.Add(Me.lblRZtrim)
        Me.Controls.Add(Me.lblXtrim)
        Me.Controls.Add(Me.trkYtrim)
        Me.Controls.Add(Me.trkRZtrim)
        Me.Controls.Add(Me.trkXtrim)
        Me.Controls.Add(Me.btnCOM)
        Me.Controls.Add(Me.cmbCOM)
        Me.Name = "Form1"
        Me.Text = "4pcb Dashboard"
        CType(Me.trkXtrim, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.trkRZtrim, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.trkYtrim, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents tmrTX As System.Windows.Forms.Timer
    Friend WithEvents cmbCOM As System.Windows.Forms.ComboBox
    Friend WithEvents btnCOM As System.Windows.Forms.Button
    Friend WithEvents tmrPaint As System.Windows.Forms.Timer
    Friend WithEvents trkXtrim As System.Windows.Forms.TrackBar
    Friend WithEvents trkRZtrim As System.Windows.Forms.TrackBar
    Friend WithEvents trkYtrim As System.Windows.Forms.TrackBar
    Friend WithEvents lblXtrim As System.Windows.Forms.Label
    Friend WithEvents lblRZtrim As System.Windows.Forms.Label
    Friend WithEvents lblYtrim As System.Windows.Forms.Label
    Friend WithEvents serCOM As System.IO.Ports.SerialPort

End Class
