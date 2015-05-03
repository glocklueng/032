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
    public partial class CyBinaryGrid : UserControl
    {
        #region Constants
        public const string ZeroSign = "0";
        public const string OneSign = "1";
        public const string XSign = "X";
        #endregion

        #region Class private members
        private int m_numberOfBits = 8;
        private bool m_valueUpdateLocked = false;
        private bool m_locked = false;
        private string[] m_bits;
        private List<Label> m_labelList = new List<Label>();
        #endregion

        // Delegates
        public delegate void BitChangedEventHandler(object sender, EventArgs e);

        // Events
        public event BitChangedEventHandler BitChanged;

        #region Constructor(s)
        public CyBinaryGrid()
        {
            InitializeComponent();

            m_bits = new string[m_numberOfBits];

            // Add labels to the list for easy operating
            m_labelList.Add(lblBit7);
            m_labelList.Add(lblBit6);
            m_labelList.Add(lblBit5);
            m_labelList.Add(lblBit4);
            m_labelList.Add(lblBit3);
            m_labelList.Add(lblBit2);
            m_labelList.Add(lblBit1);
            m_labelList.Add(lblBit0);
        }
        #endregion

        #region Class properties
        public string[] Bits
        {
            get { return m_bits; }
            set
            {
                m_bits = value;
                UpdateGrid();
            }
        }

        public int NumberOfBits
        {
            get { return m_numberOfBits; }
            set { m_numberOfBits = value; }
        }

        public bool Locked
        {
            get { return m_locked; }
            set { m_locked = value; }
        }

        string[] m_errorList = new string[8];

        #endregion

        #region Event handlers
        private void lblBit_Click(object sender, EventArgs e)
        {
            Label currentLabel = (Label)sender;

            switch (currentLabel.Text)
            {
                case ZeroSign:
                    currentLabel.Text = OneSign;
                    break;
                case OneSign:
                    currentLabel.Text = ZeroSign;
                    break;
                default:
                    break;
            }
            UpdateGridError(m_labelList.IndexOf(currentLabel));
        }

        private void CyI2CP4FFBinaryGrid_Load(object sender, EventArgs e)
        {
            // Disable unused bits
            for (int i = m_labelList.Count - 1; i >= m_numberOfBits; i--)
            {
                m_labelList[i].Enabled = false;
                m_bits[i] = XSign;
            }

            UpdateGrid();
        }

        private void lblBit_TextChanged(object sender, EventArgs e)
        {
            if (m_valueUpdateLocked == false)
            {
                // Update values in array
                for (int i = 0; i < m_numberOfBits; i++)
                {
                    this.Bits[i] = m_labelList[i].Text;
                    UpdateGridError(i);
                }

                // Raise Event
                if (BitChanged != null)
                    BitChanged(this, new EventArgs());
            }
        }
        #endregion

        private void UpdateGrid()
        {
            m_valueUpdateLocked = true;
            for (int i = 0; i < m_labelList.Count; i++)
            {
                m_labelList[i].Text = this.Bits[i];
                UpdateGridError(i);
            }
            m_valueUpdateLocked = false;
        }

        public void UpdateGridViewOnly(string[] bits)
        {
            m_valueUpdateLocked = true;
            for (int i = 0; i < m_labelList.Count; i++)
            {
                m_labelList[i].Text = bits[i];
                UpdateGridError(i);
            }
            m_valueUpdateLocked = false;
        }

        public void ResetBits()
        {
            for (int i = 0; i < m_bits.Length; i++)
            {
                m_bits[i] = ZeroSign;
                UpdateGridError(i);
            }
            UpdateGrid();
        }

        public void SetErrorListItem(int index, string invalidValue)
        {
            if ((index < 0) || (index > m_errorList.Length)) return;

            m_errorList[index] = invalidValue;

            UpdateGridError(index);
        }
        private void UpdateGridError(int index)
        {
            if ((index < 0) || (index > m_errorList.Length)) return;

            if (m_labelList[index].Text == m_errorList[index])

                m_labelList[index].BackColor = Color.Red;
            else
                m_labelList[index].BackColor = SystemColors.Window;
        }
    }
}
