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
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace SCB_P4_v99_0
{
    public partial class CyTabControlWrapper : UserControl, ICyParamEditingControl
    {
        protected CyParameters m_params = null;
        protected ErrorProvider m_errorProvider = null;

        /// <summary>
        /// This property returns the tab name that will be associated with parameters in expression view.
        /// </summary>
        public virtual string TabName
        {
            get { return string.Empty; }
        }

        /// <summary>
        /// This is method that updates UI from the parameters.
        /// </summary>
        public virtual void UpdateUI() { }

        #region Constructor(s)
        public CyTabControlWrapper() { }
        public CyTabControlWrapper(CyParameters prms)
        {
            m_params = prms;

            m_errorProvider = new ErrorProvider();
            m_errorProvider.BlinkStyle = ErrorBlinkStyle.NeverBlink;

            this.Dock = DockStyle.Fill;
            this.AutoScroll = true;
        }
        #endregion

        #region ICyParamEditingControl Members
        public Control DisplayControl
        {
            get { return this; }
        }

        public virtual IEnumerable<CyCustErr> GetErrors()
        {
            string errorMessage = string.Empty;

            if (m_errorProvider != null)
            {
                // Check controls for errors
                foreach (Control control in this.Controls)
                {
                    errorMessage = m_errorProvider.GetError(control);
                    if (string.IsNullOrEmpty(errorMessage) == false)
                        yield return new CyCustErr(errorMessage);

                    // Check controls inside groupbox
                    foreach (Control internalControl in control.Controls)
                    {
                        errorMessage = m_errorProvider.GetError(internalControl);
                        if (string.IsNullOrEmpty(errorMessage) == false)
                            yield return new CyCustErr(errorMessage);
                    }
                }
            }

            foreach (string paramName in m_params.m_inst.GetParamNames())
            {
                CyCompDevParam param = m_params.m_inst.GetCommittedParam(paramName);
                if (param.TabName.Equals(TabName))
                {
                    if (param.ErrorCount > 0)
                    {
                        foreach (string errMsg in param.Errors)
                        {
                            yield return new CyCustErr(errMsg);
                        }
                    }
                }
            }
        }
        #endregion
    }
}
