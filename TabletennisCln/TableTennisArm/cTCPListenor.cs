using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Net;
using System.Net.Sockets;

using System.Windows.Threading;

using System.Windows;
using System.Threading;


namespace TableTennisArm
{
    class cTCPListenor
    {
        public bool isConected = false;
        private TcpListener server = null;
        Int32 port;
        IPAddress localAddr;

        TcpClient client;

        // Buffer for reading data
        Byte[] bytes;
        public String Recvdata;

        NetworkStream stream;

        //public cTCPListenor()
        //{

        //}

        public bool StartWaitting()
        {
            port = 13000;
            localAddr = IPAddress.Parse("127.0.0.1");

            // TcpListener server = new TcpListener(port);
            server = new TcpListener(localAddr, port);
            // Start listening for client requests.
            server.Start();

            WaitForCLientThread();


            return true;
        }

        public void CloseSocket()
        {
            if (client != null)
            {
                // Shutdown and end connection
                client.Close();
            }

            if (server != null)
            {
                // Stop listening for new clients.
                server.Stop();
            }

        }

        public void WaitForReceive()
        {
            // Console.Write("rcv\n");
            int i;
            // Loop to receive all the data sent by the client. 
            while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                // Translate data bytes to a ASCII string.
                Recvdata = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
                break;
            }
        }

        public void SendMsg(string SendData)
        {
            try
            {
                if (stream != null && isConected)
                {
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(SendData);
                    // Send 
                    stream.Write(msg, 0, msg.Length);
                }
            }
            catch (SocketException e)
            {
                // Console.WriteLine("SocketException: {0}", e);
            }

        }

        //===========================================
        public void WaitForCLient()
        {
            // Buffer for reading data
            bytes = new Byte[256];
            Recvdata = null;

            //  Console.Write("Waiting for a connection... ");
            client = server.AcceptTcpClient();
            // Console.WriteLine("Connected!");

            Recvdata = null;

            // Get a stream object for reading and writing
            stream = client.GetStream();

            isConected = true;
            //  MessageBox.Show("OK");
            //   System.Threading.Thread.Sleep(1000);

        }

        // 使用线程 方法
        public void WaitForCLientThread()
        {
            ThreadStart startWaitForCLient = new ThreadStart(WaitForCLient); //注意：WaitForCLient()必须为不带有参数的方法
            Thread WaitForCLientTD = new Thread(startWaitForCLient); //实例化要开启的新类
            WaitForCLientTD.Start();//开启线程
        }
        //===========================================
    }
}
