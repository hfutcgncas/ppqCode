using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Windows;

using PCOMMSERVERLib;




namespace Pmac_Comm
{
    public class cPMAC 
    {
        //pmac 相关变量       
        public static PCOMMSERVERLib.PmacDeviceClass Pmac;
        public int m_nDevice, m_nMotor, m_nServo, m_nDPRBase, m_nDPRSize;
        public bool m_bDeviceOpen, m_bDPRAvailable, m_bTurbo, m_isServoON, m_isConnected;

        public string m_PMAC_msg { get; set; }
        public string m_PMAC_cmd { get; set; }
        #region 构造和析构函数
        public cPMAC()
        {
            Pmac = new PmacDeviceClass();
            SetAllEvents();
            m_bDeviceOpen = false;
            m_isServoON = false;
            m_isConnected = false;
        }
        ~cPMAC()
        {
            if (Pmac != null && m_bDeviceOpen)
            {
                Pmac.Close(m_nDevice);
                m_bDeviceOpen = false;
            }     
        }
        #endregion


        #region 内部函数
        #region 连接函数
        public bool ConectToPMAC()
        {
            int dev = m_nDevice;
            bool bSuccess = false;
            string str;

            string type;


            Pmac.SelectDevice(0, out dev, out bSuccess);
            if (bSuccess)
            {

                m_bDeviceOpen = true;

                if (m_bDeviceOpen)
                    Pmac.Close(m_nDevice);
                m_nDevice = dev;
                m_bDPRAvailable = false;
                m_nDPRBase = 0;
                m_nDPRSize = 0;
                Pmac.Open(m_nDevice, out m_bDeviceOpen);
                if (m_bDeviceOpen)
                {
                    m_bDPRAvailable = Pmac.get_DPRAvailable(m_nDevice);
                    m_nDPRSize = Pmac.get_DPRSize(m_nDevice);
                    Pmac.DPRAddressRange(m_nDevice, false, false, out m_nDPRBase);
                    m_bTurbo = Pmac.get_IsTurbo(m_nDevice);
                    // m_bFGEnabled = Pmac.get_DPRMotorRptEnabled(m_nDevice);
                    // Setup for this device
                    if (m_bTurbo)
                    {
                        type = "Turbo";
                    }
                    else
                    {
                        type = "NO Turbo";
                    }
                    str = string.Format("Device number {0:D} opened successfully." + type, m_nDevice);
                }
                else
                {
                    // Disable all required
                    str = string.Format("Device number {0:D} failed to open.", m_nDevice);
                }
                m_PMAC_msg = str;
            }
            return bSuccess;
        }
        #endregion

        #region 发送命令
        public void SendCMD()
        {
            string Rt;
            int Statue;
            Pmac.GetResponseEx(m_nDevice, m_PMAC_cmd, true, out Rt, out Statue);
            m_PMAC_msg = Rt;
        }

        public void SendCMD(string cmd)
        {
            string Rt;
            int Statue;
            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);
            m_PMAC_msg = Rt;
        }

        #endregion

        #region 开关伺服
        public void ServoON()
        {
            if (m_bDeviceOpen && !m_isServoON)
            {            
                SendCMD("#1 J/");
                SendCMD("#2 J/");
                SendCMD("#3 J/");
                m_isServoON = true;
            }

        }

        public void ServoOFF()
        {
            if (m_bDeviceOpen && m_isServoON)
            {
                SendCMD("p3=0");

                SendCMD("Q");
                
                SendCMD("#1 K");
                SendCMD("#2 K");
                SendCMD("#3 K");
                m_isServoON = false;
            }
           // MessageBox.Show("get here");
        }
        #endregion //开关伺服


        #region 调用程序
        public void RunProgram(int cooIndex, int ProIndex)
        {
            if (m_bDeviceOpen)
            {
                m_PMAC_cmd = "&"+cooIndex.ToString()+" B" + ProIndex.ToString() + " R";
                SendCMD();
            }
            else
            {
                //MessageBox.Show("PMAC unconected");
                System.Console.WriteLine("PMAC unconected");
            }
        }
        #endregion //开伺服


        #endregion //内部函数
        //----------------------------------------------
        //注册PMAC的各种事件
        # region 注册PMAC的各种事件

        private void SetAllEvents()
        {
            if (Pmac != null)
            {
                _IPmacDeviceEvents_UnsolicitedEventHandler DUnsolicitedEventE
                 = new _IPmacDeviceEvents_UnsolicitedEventHandler(OnUnsolicited);
                Pmac.Unsolicited += DUnsolicitedEventE;
                _IPmacDeviceEvents_ProgressEventHandler DProgressEventE
                = new _IPmacDeviceEvents_ProgressEventHandler(OnProgress);
                Pmac.Progress += DProgressEventE;
                _IPmacDeviceEvents_MessageEventHandler DMessageEventE
                = new _IPmacDeviceEvents_MessageEventHandler(OnMessage);
                Pmac.Message += DMessageEventE;
                _IPmacDeviceEvents_ErrorEventHandler DErrorEventE
              = new _IPmacDeviceEvents_ErrorEventHandler(OnError);
                Pmac.Error += DErrorEventE;
                _IPmacDeviceEvents_InterruptEventHandler DInterruptEventE
                = new _IPmacDeviceEvents_InterruptEventHandler(OnInterrupt);
                Pmac.Interrupt += DInterruptEventE;
            }

        }

        // An unsolicited event from the server
        void OnUnsolicited(int device, String Text)
        {
            m_PMAC_msg = Text;
        }
        // A progress update from the server
        void OnProgress(int device, int percent)
        {
            //PB_OnRunning.Value = percent;
        }
        // A message from the server
        void OnMessage(int device, String Text, bool linefeed)
        {
            m_PMAC_msg = Text;
        }
        // A error message from the server
        void OnError(int device, String filename, int errorNumber, int lineNumber, String msg)
        {
            string str;
            str = string.Format("{0} {1:D}, Line:{ 2:D},{ 4} { 3} { 4} ", filename, errorNumber, lineNumber, msg, '"');
            m_PMAC_msg = str;
        }
        // A interrupt message from the server
        void OnInterrupt(int device, int interruptLevel)
        {
            String msg = "";
            switch (interruptLevel)
            {
                case 1:
                    msg = "** Interrupt-> In Position **"; break;
                case 2:
                    msg = "** Interrupt-> Buffer Request **"; break;
                case 4:
                    msg = "** Interrupt-> Warning Following Error **"; break;
                case 8:
                    msg = "** Interrupt-> Fatal Following Error **"; break;
                case 16:
                    msg = "** Interrupt-> Host Request **"; break;
                case 32:
                    msg = "** Interrupt-> IR5 **"; break;
                case 64:
                    msg = "** Interrupt-> IR6 **"; break;
                case 128:
                    msg = "** Interrupt-> IR7 **"; break;
                default: msg = "** Interrupt-> Spurious **"; break;
            }
            m_PMAC_msg = msg;
        }
        #endregion
    }
}
