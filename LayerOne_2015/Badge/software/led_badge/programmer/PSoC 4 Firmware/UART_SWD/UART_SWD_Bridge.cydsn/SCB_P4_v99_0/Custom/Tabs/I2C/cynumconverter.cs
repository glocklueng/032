/*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace SCB_P4_v99_0
{
    class CyConvert
    {
        public static string HEX_PREFIX = "0x";
        private const string ZERO = "0";
        private const string X = "X";

        #region Hex converting
        public static int HexToInt(string hexString)
        {
            int result = 0;
            try
            {
                result = Convert.ToInt32(hexString, 16);
            }
            catch (Exception) { }
            return result;
        }

        public static string[] HexToBin7(string hexString)
        {
            int intValue = HexToInt(hexString);
            return EightBitsToSevenBits(IntToBin(intValue));
        }

        public static string[] HexToBin(string hexString)
        {
            int intValue = HexToInt(hexString);
            return IntToBin(intValue);
        }
        #endregion

        #region Int converting
        public static string IntToHex(int intValue)
        {
            string result = string.Empty;
            string hexStr = intValue.ToString("X");
            if (hexStr.Length == 1)
                result = HEX_PREFIX + ZERO + hexStr;
            else
                result = HEX_PREFIX + hexStr;
            return result;
        }

        public static string[] IntToBin(int intValue)
        {
            string str = Convert.ToString(intValue, 2);
            string remainingZeros = string.Empty;

            for (int i = str.Length; i < 8; i++)
            {
                remainingZeros += ZERO;
            }
            return StringToStringArray(remainingZeros + str);
        }

        public static string[] IntToBin7(int intValue)
        {
            string str = Convert.ToString(intValue, 2);
            string remainingZeros = string.Empty;

            for (int i = str.Length; i < 8; i++)
            {
                remainingZeros += ZERO;
            }

            return EightBitsToSevenBits(IntToBin(intValue));
        }
        #endregion

        #region Bin converting
        public static string BinToHex(string[] strArray)
        {
            int intValue = BinToInt(strArray);
            return IntToHex(intValue);
        }

        public static string Bin7ToHex(string[] strArray)
        {
            string[] sevenBitsArray = new string[strArray.Length];
            sevenBitsArray = SevenBitsToEightBits(strArray);
            int intValue = BinToInt(XToZero(sevenBitsArray));
            return IntToHex(intValue);
        }

        public static int BinToInt(string[] strArray)
        {
            string binaryStr = StringArrayToString(XToZero(strArray));
            return Convert.ToInt32(binaryStr, 2);
        }

        public static int Bin7ToInt(string[] strArray)
        {
           string[] sevenBitsArray = new string[strArray.Length];
           sevenBitsArray = SevenBitsToEightBits(strArray);
           return BinToInt(sevenBitsArray);
        }
        #endregion

        #region String converting
        public static string StringArrayToString(string[] strArray)
        {
            string resultStr = string.Empty;
            for (int i = 0; i < strArray.Length; i++)
            {
                resultStr += strArray[i];
            }
            return resultStr;
        }

        public static string[] StringToStringArray(string str)
        {
            string[] strArray = new string[str.Length];

            for (int i = str.Length - 1; i >= 0; i--)
            {
                strArray[i] = str[i].ToString();
            }
            return strArray;
        }
        #endregion

        #region Eight/seven to seven/eight bits converting
        public static string[] EightBitsToSevenBits(string[] inputArray)
        {
            string[] outputArray = new string[inputArray.Length];

            for (int i = 0; i < inputArray.Length - 1; i++)
            {
                outputArray[i] = inputArray[i + 1];
            }

            outputArray[inputArray.Length - 1] = X;
            return outputArray;
        }

        public static string[] SevenBitsToEightBits(string[] inputArray)
        {
            string[] outputArray = new string[inputArray.Length];

            for (int i = 0; i < inputArray.Length - 1; i++)
            {
                outputArray[i + 1] = inputArray[i];
            }
            outputArray[0] = ZERO;
            return outputArray;
        }
        #endregion

        public static string[] XToZero(string[] inputArray)
        {
            for (int i = 0; i < inputArray.Length; i++)
            {
                if (inputArray[i] == X)
                    inputArray[i] = inputArray[i].Replace(X, ZERO);
            }
            return inputArray;
        }
    }
}
