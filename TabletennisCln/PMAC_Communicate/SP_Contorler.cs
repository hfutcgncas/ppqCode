using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.IO.Ports;
using System.Threading;


namespace PMAC_Communicate
{
    public class SP_Contorler
    {
        SerialPort serialPort1;
        public bool isConected;
        public void initControler(string portName, int baudRate, Parity parity, int dataBits, StopBits stopBits)
        {

            serialPort1 = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
        }

        public bool Open()
        {
            try
            {
                serialPort1.Open();
            }
            catch
            {
                return false;
            }
            isConected = true;
            return true;
        }

        public bool Close()
        {
            try
            {
                serialPort1.Close();
            }
            catch
            {
                return false;
            }
            isConected = false;
            return true;
        }

        public void Set_S_for_Once(int angle)
        {
            if (isConected)
            {
                serialPort1.WriteLine("#1P" + angle.ToString() + "T200");
            }
            else
            {
                Console.WriteLine("Serial SP is Not connected");
            }
        }

        public void Set_P_for_Once(int angle)
        {
            if (isConected)
            {
                serialPort1.WriteLine("#2P" + angle.ToString() + "T200");
            }
            else
            {
                Console.WriteLine("Serial SP is Not connected");
            }
        }

        //角度转为命令
        private int Convert_S(double angle_S)
        {
            return 1500 + (int)(angle_S * 2000 / 180.0);
        }
        private int Convert_P(double angle_P)
        {
            return 1500 + (int)(angle_P * 2000 / 360.0);
        }

        public void Set_SP_for_Once(int angle_S, int angle_P)
        {
            if (isConected)
            {

                serialPort1.WriteLine("#1P" + Convert_S(angle_S).ToString() + "#2P" + Convert_P(angle_P).ToString() + "T100");
            }
            else
            {
                Console.WriteLine("Serial SP is Not connected");
            }
        }

        public void Set_SP_for_OnceV2(double angle_S, double angle_P)
        {
            double aS = angle_S / -90.0;
            double aP = angle_P / -90.0;

            if (isConected)
            {
                serialPort1.WriteLine("POSE ALL " + aS.ToString() + " " + aP.ToString()+" 0 0\r\n");
                
                //serialPort1.WriteLine("POSE #0 " + aS.ToString() + "\r\n");
                //serialPort1.WriteLine("POSE #1 " + aP.ToString() + "\r\n");
            }
            else
            {
                Console.WriteLine("Serial SP is Not connected");
            }
        }


    }
}
