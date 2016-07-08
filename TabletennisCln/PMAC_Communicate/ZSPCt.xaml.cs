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

using System.ComponentModel;
using System.Threading;

namespace PMAC_Communicate
{
    /// <summary>
    /// Interaction logic for ZSPCt.xaml
    /// </summary>
    public partial class ZSPCt : UserControl
    {
        public ZSPCt()
        {
            InitializeComponent();
            d.SP_Sender.initControler("COM3", 9600, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            d.ZSP_Statue = "Push to Connect";
        }




        public void BTC_WaitZSP(object sender, RoutedEventArgs e)
        {
            d.ZSP_Statue = "Wait for Connect";
            d.statueThread.Start(); //检测按键变化
            d.SP_Sender.Open();
        }


        public void BTC_HomeZSP(object sender, RoutedEventArgs e)
        {
            d.SendSP_HomeCmd();
        }

        //public void BTC_SevZ(object sender, RoutedEventArgs e)
        //{
        //    d.ZSP_Sender.SendMsg("ServoZ");// Z轴开/关伺服命令
        //}

        public void BTC_CloseZSP(object sender, RoutedEventArgs e)
        {
            Console.WriteLine("here");

            d.SP_Sender.Close();// 关闭远端程序命令
        }

        public void UserControl_Unloaded(object sender, RoutedEventArgs e)
        {
            BTC_CloseZSP(this, null);

            d.statueThread.Abort();

            d.SP_Sender.Close();

        }


        #region 外部接口
        public void SetZSP( int s, int p)
        {
            if ( s <= d.S_MAX && s >= d.S_MIN
                && p <= d.P_MAX && p >= d.P_MIN)
            {
                d.Hit_S = s;
                d.Hit_P = p;
            }
            
        }
        #endregion



    }

    public class data : INotifyPropertyChanged//实现接口，详细表述MSDN
    {
        //INotifyPropertyChanged Members 不需更改---------------------------------------------------------
        #region INotifyPropertyChanged Members

        /// <summary>
        /// Event to which the view's controls will subscribe.
        /// This will enable them to refresh themselves when the binded property changes provided you fire this event.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// When property is changed call this method to fire the PropertyChanged Event
        /// </summary>
        /// <param name="propertyName"></param>
        public void OnPropertyChanged(string propertyName)
        {
            //Fire the PropertyChanged event in case somebody subscribed to it
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        public Thread statueThread;
        public SP_Contorler SP_Sender;

        public data()
        {
            SP_Sender = new SP_Contorler();
         
            Hit_S = 0;
            Hit_P = 0;

            statueThread = new Thread(new ThreadStart(ThreadWork));


        }

        public void ThreadWork()
        {
            while (SP_Sender != null)
            {
                if (SP_Sender.isConected)
                {
                    ZSP_Statue = "Connected";
                    SendSP_PosCmd();
                    //Thread.Sleep(10);
                }
                else
                {
                    ZSP_Statue = "Wait for Connect";
                }
                Thread.Sleep(100);
            }
        }


        #region MAX-MIN

        public int S_MAX
        {
            get { return 60; }
            set { }
        }
        public int S_MIN
        {
            get { return -60; }
            set { }
        }

        public int P_MAX
        {
            get { return 30; }
            set { }
        }
        public int P_MIN
        {
            get { return -60; }
            set { }
        }
        #endregion

       


        private int hit_S; //偏航角 yaw
        public int Hit_S
        {
            get { return hit_S; }
            set
            {
                hit_S = value;
                OnPropertyChanged("Hit_S");
             //   SendSP_PosCmd();
            }
        }
        private int hit_P; //俯仰角 pitch
        public int Hit_P
        {
            get { return hit_P; }
            set
            {
                hit_P = value;
                OnPropertyChanged("Hit_P");
             //   SendSP_PosCmd();
            }
        }

        private string zsp_statue;
        public string ZSP_Statue
        {
            get { return zsp_statue; }
            set
            {
                zsp_statue = value;
                OnPropertyChanged("ZSP_Statue");
            }
        }




        public void SendSP_PosCmd()
        {
   
            if (Hit_S > S_MAX) { Hit_S = S_MAX; }
            if (Hit_S < S_MIN) { Hit_S = S_MIN; }
            if (Hit_P > P_MAX) { Hit_P = P_MAX; }
            if (Hit_P < P_MIN) { Hit_P = P_MAX; }

            //SP_Sender.Set_SP_for_Once(hit_S, hit_P);
            SP_Sender.Set_SP_for_OnceV2(hit_S, hit_P);

            
        }

        public void SendSP_HomeCmd()
        {
            Hit_S = 0;
            Hit_P = 0;

                SP_Sender.Set_SP_for_OnceV2(0, 0);

        }
    }
}
