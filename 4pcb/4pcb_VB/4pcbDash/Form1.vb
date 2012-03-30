Public Class Form1

    ' COM status indicator
    Dim com_status As Integer = 0
    ' COM status constants
    Const DISCONNECTED = 0
    Const OPEN = 1
    Const CONNECTED = 2

    Dim rx_i As Integer
    Dim rx_packet(0 To 7) As Byte

    ' COM timeout counter
    Dim com_timeout As Integer = 5
    Const TIMEOUT = 5

    ' DirectInput stuff
    Dim devices As Microsoft.DirectX.DirectInput.DeviceList
    Dim dev As Microsoft.DirectX.DirectInput.DeviceInstance
    Dim joystick As Microsoft.DirectX.DirectInput.Device
    Dim oldbutton(12) As Byte
    Dim oldhat As Integer

    ' trims
    Dim xtrim = 0
    Dim ytrim = 0
    Dim rztrim = 0

    Dim angle_pitch As Single
    Dim rate_pitch As Single
    Dim rear_command As Byte
    Dim vref As Integer

    ' Data logging.
    Dim path As String                          ' data recording path
    Dim datafile As System.IO.StreamWriter      ' data recording file class
    Dim starttime As Date                       ' recording start time
    Dim time As Date

    Private Sub tmrTX_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tmrTX.Tick
        Dim tx_packet(0 To 5) As Byte

        Dim thr, x, y, rz As Integer
        Dim newbutton(13) As Byte
        Dim newhat As Integer

        If Not joystick Is Nothing Then
            Dim joystick_state As Microsoft.DirectX.DirectInput.JoystickState
            joystick_state = joystick.CurrentJoystickState

            ' Joystick
            ' x = (joystick_state.X / 256) + xtrim
            ' y = (256 - joystick_state.Y / 256) + ytrim
            ' thr = (256 - joystick_state.GetSlider(0) / 256)
            ' rz = (256 - joystick_state.Rz / 256) + rztrim

            ' Gamepad
            x = (joystick_state.Z / 256) + xtrim
            y = (256 - joystick_state.Rz / 256) + ytrim
            thr = (256 - joystick_state.Y / 127)
            rz = (256 - joystick_state.X / 256) + rztrim

            newbutton = joystick_state.GetButtons()
            newhat = joystick_state.GetPointOfView(0)

            ' POV hat controls x any y trim.
            If newhat <> oldhat Then
                Select Case newhat
                    Case -1 : Exit Select
                    Case 0 : ytrim = ytrim + 1 : Exit Select
                    Case 4500 : ytrim = ytrim + 1 : xtrim = xtrim + 1 : Exit Select
                    Case 9000 : xtrim = xtrim + 1 : Exit Select
                    Case 13500 : ytrim = ytrim - 1 : xtrim = xtrim + 1 : Exit Select
                    Case 18000 : ytrim = ytrim - 1 : Exit Select
                    Case 22500 : ytrim = ytrim - 1 : xtrim = xtrim - 1 : Exit Select
                    Case 27000 : xtrim = xtrim - 1 : Exit Select
                    Case 31500 : ytrim = ytrim + 1 : xtrim = xtrim - 1 : Exit Select
                    Case Else : Exit Select
                End Select

                trkXtrim.Value = xtrim
                trkYtrim.Value = ytrim

                lblXtrim.Text = "X-Trim: " + Format(xtrim, "##0")
                lblYtrim.Text = "Y-Trim: " + Format(ytrim, "##0")

            End If

            ' Buttons 3 and 4 control rz trim.
            If newbutton(2) <> oldbutton(2) And newbutton(2) = 128 Then
                rztrim = rztrim + 1
                trkRZtrim.Value = rztrim
                lblRZtrim.Text = "RZ-Trim: " + Format(rztrim, "##0")
            End If
            If newbutton(3) <> oldbutton(3) And newbutton(3) = 128 Then
                rztrim = rztrim - 1
                trkRZtrim.Value = rztrim
                lblRZtrim.Text = "RZ-Trim: " + Format(rztrim, "##0")
            End If

            oldbutton = newbutton
            oldhat = newhat

        Else
            x = 127 + xtrim
            y = 127 + ytrim
            rz = 127 + rztrim
            thr = 0
        End If

        If x > 254 Then : x = 254 : End If
        If x < 0 Then : x = 0 : End If
        If y > 254 Then : y = 254 : End If
        If y < 0 Then : y = 0 : End If
        If rz > 254 Then : rz = 254 : End If
        If rz < 0 Then : rz = 0 : End If
        If thr > 254 Then : thr = 254 : End If
        If thr < 0 Then : thr = 0 : End If

        tx_packet(0) = 255
        tx_packet(1) = thr
        tx_packet(2) = y
        tx_packet(3) = x
        tx_packet(4) = rz

        Try
            If serCOM.IsOpen Then
                serCOM.Write(tx_packet, 0, 5)
            End If
        Catch ex As Exception
            MsgBox(ex.Message)
        End Try

    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim retry As Integer

        ' stop yelling at me, I don't even have a multi-threading processor
        System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = False

        Do
            retry = 0
            devices = Microsoft.DirectX.DirectInput.Manager.GetDevices(Microsoft.DirectX.DirectInput.DeviceClass.GameControl, Microsoft.DirectX.DirectInput.EnumDevicesFlags.AttachedOnly)
            If devices.Count <> 0 Then
                devices.MoveNext()
                dev = devices.Current
                joystick = New Microsoft.DirectX.DirectInput.Device(dev.InstanceGuid)
                joystick.Acquire()
            Else
                If MsgBox("No gamepad detected. Plug in gamepad and choose Retry.", MsgBoxStyle.RetryCancel) = MsgBoxResult.Retry Then
                    retry = 1
                End If
            End If
        Loop While retry = 1

        ' initialize to COM1
        cmbCOM_DropDown(Nothing, Nothing)
        cmbCOM.SelectedItem = "COM1"

    End Sub

    Private Sub Form1_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        ' clear up some resources when exiting
        tmrPaint.Enabled = False
        tmrTX.Enabled = False
        Try
            If Not joystick Is Nothing Then
                joystick.Dispose()
            End If
            If serCOM.IsOpen Then
                serCOM.Close()
            End If
        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Private Sub cmbCOM_DropDown(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmbCOM.DropDown
        ' Refresh the available port list.
        Dim PortList As Array
        Dim i As Integer

        For i = 0 To cmbCOM.Items.Count - 1
            cmbCOM.Items.RemoveAt(0)
        Next

        PortList = System.IO.Ports.SerialPort.GetPortNames()

        For Each PortString As String In PortList
            cmbCOM.Items.Insert(0, PortString)
        Next
    End Sub

    Private Sub btnCOM_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnCOM.Click
        ' toggle connection on or off
        Try
            If serCOM.IsOpen() Then
                serCOM.Close()
            End If
            If com_status = OPEN Or com_status = CONNECTED Then
                tmrTX.Enabled = False
                com_status = DISCONNECTED
                btnCOM.Text = "Connect"
                end_data()
            Else
                serCOM.PortName = cmbCOM.SelectedItem
                serCOM.Open()
                serCOM.DtrEnable = False
                serCOM.DtrEnable = True
                com_status = OPEN
                com_timeout = 0
                btnCOM.Text = "Disconnect"
                tmrTX.Enabled = True
                start_data()
            End If
        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Private Sub tmrPaint_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tmrPaint.Tick

        ' timeout handler
        If com_timeout < TIMEOUT Then
            com_timeout = com_timeout + 1
        ElseIf com_status = CONNECTED Then
            com_status = OPEN
        End If

        ' COM status indicator
        Select Case com_status
            Case DISCONNECTED : cmbCOM.BackColor = Color.DarkBlue : Exit Select
            Case OPEN : cmbCOM.BackColor = Color.Blue : Exit Select
            Case CONNECTED : cmbCOM.BackColor = Color.DeepSkyBlue : Exit Select
        End Select

        ' Me.Text = Format(xtrim, "##0") + " | " + Format(ytrim, "##0") + " | " + Format(rztrim, "##0")

    End Sub

    Private Sub serCOM_DataReceived(ByVal sender As System.Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles serCOM.DataReceived
        Dim rx_n As Integer = serCOM.BytesToRead
        Dim rx_buffer(0 To rx_n - 1) As Byte
        Dim i As Integer

        serCOM.Read(rx_buffer, 0, rx_n)
        serCOM.DiscardInBuffer()

        For i = 0 To rx_n - 1
            If (rx_buffer(i) = 255) Then
                rx_packet(0) = 255
                rx_i = 1
            ElseIf rx_i <= 7 Then
                rx_packet(rx_i) = rx_buffer(i)
                rx_i = rx_i + 1
            End If
            If rx_i = 8 Then
                rate_pitch = (rx_packet(1) * 128 + rx_packet(2) - 8192) / 10
                angle_pitch = (rx_packet(3) * 128 + rx_packet(4) - 8192) / 10
                rear_command = rx_packet(5)
                vref = rx_packet(6) * 128 + rx_packet(7)
                time = System.DateTime.Now
                Me.Text = Format(angle_pitch, "0.0")
                ' write data
                If Not datafile Is Nothing Then
                    write_data()
                End If
                rx_i = 9
            End If
        Next

    End Sub

    Private Sub start_data()
        ' Begin recording to file. (Data is written every time a full packet is processed.)
        path = "4pcbdata.txt"
        If System.IO.File.Exists(path) Then
            System.IO.File.Delete(path)
        End If
        datafile = System.IO.File.CreateText(path)
        starttime = System.DateTime.Now

        datafile.Write("4pcb Data Log: " + System.DateTime.Now.ToString)
        datafile.WriteLine()
        datafile.Write("Time [s], ")
        datafile.Write("Pitch Angle [deg], ")
        datafile.Write("Pitch Rate [deg/s], ")
        datafile.Write("Rear Command [0-255], ")
        datafile.Write("Voltage Reference [0-1023]")
        datafile.WriteLine()
    End Sub

    Private Sub write_data()
        datafile.Write(Format((time - starttime).TotalSeconds, "0.00") + ", ")
        datafile.Write(Format(angle_pitch, "0.00") + ", ")
        datafile.Write(Format(rate_pitch, "0.00") + ", ")
        datafile.Write(Format(rear_command, "0") + ", ")
        datafile.Write(Format(vref, "0"))
        datafile.WriteLine()
    End Sub

    Private Sub end_data()
        Try
            datafile.Close()
            datafile.Dispose()
        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

End Class
