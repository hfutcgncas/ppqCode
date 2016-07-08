using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Diagnostics;
using System.Threading;

using System.Windows.Threading;

namespace TabletennisCln
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Thread RcvThread;
        Thread ZspThread;
        public MainWindow()
        {
            InitializeComponent();
            vision = new cVision();
         //   ZSPCt.BTC_WaitZSP(this, null);//自动打开端口，等待ZSP连接
        }
        //=======================================

        private void Window_Closed(object sender, EventArgs e)
        {
            //--------------------------------------------------------------
            //关闭视觉程序
            Process[] allProgresse = System.Diagnostics.Process.GetProcessesByName("colorCamera");
            foreach (Process closeProgress in allProgresse)
            {
                if (closeProgress.ProcessName.Equals("colorCamera"))
                {
                    closeProgress.Kill();
                    closeProgress.WaitForExit();
                    break;
                }
            }  
            //----------------------------------------------------------------

            if (ZspThread != null && RcvThread.IsAlive)
            {
                ZspThread.Abort();
            }
            if (RcvThread != null && RcvThread.IsAlive)
            {
                if (vision != null) { vision.newsock.Close(); }
                RcvThread.Abort();
            }

            Control_Arm.UserControl_Unloaded(this, null);
            Thread.Sleep(100);
            
        }

        #region 视觉部分通讯代码
        private readonly cVision vision; //接收视觉结果

        
        public bool ConectToVision()
        {
            string notes;
            if(vision.initRcv())
            {        
                RcvThread = new Thread(new ThreadStart(delegate
                {
                    while (true)
                    {
                        vision.ReciveData();
                        notes = " X  = " + vision.hitPar[1].ToString("#0.0") + "         Vx  = " + vision.hitPar[4].ToString("#0.0") +
                              "\n Y  = " + vision.hitPar[2].ToString("#0.0") + "         Vy  = " + vision.hitPar[5].ToString("#0.0") +
                              "\n Z  = " + vision.hitPar[3].ToString("#0.0") + "         Vz  = " + vision.hitPar[6].ToString("#0.0") +
                              "\n tg = " + vision.hitPar[12].ToString("#0.0");
                    //    if (vision.hitPar[0] == 1)
                    //    {
                            double[] padPar;
                            SlovePadPram(vision.hitPar, out padPar);
                            notes += "\n PadV = " + padPar[4].ToString("#0.0") +
                                     "\n PadS = " + padPar[5].ToString("#0.0");
                            SetNotes(notes);

                            //if (vision.hitPar[12] >0.01)
                            Control_Arm.Recv4Vision(padPar);                 
                   //     }
                        //Thread.Sleep(10);
                    }
                }
                ));
                RcvThread.Start();

                //ZspThread = new Thread(new ThreadStart(delegate
                //    {
                //        while(true)
                //        {
                //            if (!Control_Arm.flag_isMoving)
                //            {
                //                Control_Arm.HomeZsp();
                //            }
                //            Thread.Sleep(10);
                //        }  
                //    }
                // ));
                //ZspThread.Start();
                return true;
            }
            return false;
        }
   
        //------------------------------
        private void SetNotes(string notes)
        {
            if (Dispatcher.Thread != Thread.CurrentThread)
            {
                this.TBK_VisionOut.Dispatcher.Invoke(new Action(() =>
                {
                    this.TBK_VisionOut.Text = notes;
                }));
            }
            else
            {
                this.TBK_VisionOut.Text = notes;
            }
        }

        //[DllImport("kernel32.dll")]
        //public static extern int WinExec(string exeName, int operType);  

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (BT_VisionConect.Content.ToString() == "Connect To Vision")
            {
                ////------------------------------------------------------
                //ProcessStartInfo info = new ProcessStartInfo();
                //info.FileName = @".\\colorCamera.exe";
                //info.Arguments = "";
                //info.WindowStyle = ProcessWindowStyle.Minimized;
                //Process pro = Process.Start(info);
                //Thread.Sleep(5000);
                ////pro.WaitForExit();
                ////------------------------------------------------------
                if(ConectToVision())
                {
                    BT_VisionConect.Content = "Connected";
                }
                else
                {
                    MessageBox.Show("连接失败");
                }
            }
            else
            {
                MessageBox.Show("已经连接，不可重复连接");
            }
        }
        #endregion

        
        #region 回球决策代码
        public static double lambda = 0.9;
        public static double length = 0.5;

        private static bool SlovePadPram(double[] inputBall, out double[] output)
        {
            
            output = new double[9];

            double padX = inputBall[1];
            double padY = inputBall[2];
            double padZ = inputBall[3]; 
            double ballVx_In = inputBall[4] / 1000;
            double ballVy_In = inputBall[5] / 1000;
            double ballVz_In = inputBall[6] / 1000;

            //--------------------------------------------------------------------------------------
            //double g = 9.802;

            ////解方程 -1/2 * g * t^2 + Vz * t = -padZ
            //double a = -0.5 * g; double b = lambda * ballVz_In; double c = padZ / 1000;
            //double delta = b * b - 4 * a * c;

            //if (delta < 0) return false;//解算失败

            //double t1 = (-b - Math.Sqrt(delta)) / (2 * a);
            //double t2 = (-b + Math.Sqrt(delta)) / (2 * a);

            //double t = (t1 > t2) ? t1 : t2;

            //double ballV_Out = length / t;

            //double hitV = ballV_Out / lambda;

            //double padVy = Math.Sqrt((hitV * hitV) - (ballVx_In * ballVx_In)) + ballVy_In;
            //--------------------------------------------------------------------------------------

            double padVy = 1650;
            //--------------------------------------------------------------------------------------

            double padS = (Math.Atan(ballVx_In / ballVy_In) / Math.PI * 180);
            double padP = 20;
 

            double flag_AlreadySend = inputBall[11];

            double hit_t = inputBall[12];

            output[0] = flag_AlreadySend;
            output[1] = padX;
            output[2] = padY;
            output[3] = padZ;
            output[4] = padVy; //回球速度
            output[5] = padS;  //回球球拍偏转方向
            output[6] = padP;
            output[7] = hit_t;
            output[8] = inputBall[8];

            //Console.WriteLine("t = "+ t.ToString() );
            return true;
        }
        #endregion

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            double length_tmp = Convert.ToDouble(TB_lambda.Text);
            if (length_tmp == 0)
            {
                Console.WriteLine("Set length wrong");
                return;
            }
            length = length_tmp;
           
        }

    }
}
