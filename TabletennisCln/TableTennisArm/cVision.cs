using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.Net;
using System.Net.Sockets;

using System.Threading;
using System.Windows;

namespace TableTennisArm
{
    class cVision
    {
        Socket newsock;
        EndPoint Remote;
        int recv;

        public double[] hitPar = new double[13];

        byte[] data = new byte[5000];

        public void initRcv()
        {
           int Visionport = 7000;
           //public const string VisionIP = "192.168.64.5";
           string VisionIP = "127.0.0.1";

            try
            {
                IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Visionport);
                newsock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                newsock.Bind(ipep);
                //----------------------------------------------------------------
                IPEndPoint sender = new IPEndPoint(IPAddress.Parse(VisionIP), Visionport);
                Remote = (EndPoint)sender;
            }
            catch (Exception e)
            {

            }

        }

        public void ReciveData()
        {
            if (Remote == null)
            {
                MessageBox.Show("Didn't conect");
            }
            else
            {
                recv = newsock.ReceiveFrom(data, ref Remote);
            }

            for (int i = 0; i < 13; i++)
            {
                hitPar[i] = BitConverter.ToDouble(data, i * 8);
            }
            return;
            // 
        }
    }
}
