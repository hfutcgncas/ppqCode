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

using System.Threading;
using System.IO;

using System.Runtime.InteropServices;

using Pmac_Comm;
namespace PMAC_Communicate
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class UserControl1 : UserControl 
    {
        public Pmac_ModeView mdv;
        Thread queryThread;
        public delegate void NextQueryDelegate();
        double delay = -160;

        int fileIndex = 0;
        IniFile cfgFile;
        public UserControl1()
        {
            InitializeComponent();
            mdv = new Pmac_ModeView();

            TB_Delay.Text = delay.ToString();
        //    ZSPCt.BTC_WaitZSP(this, null);//自动打开端口，等待ZSP连接

            cfgFile = new IniFile(".\\historyData\\cfg.ini");
        }
     
        public void UserControl_Unloaded(object sender, RoutedEventArgs e)
        {
            
            ZSPCt.UserControl_Unloaded(this, null);

            try 
            { 
                queryThread.Abort(); 
            }
            catch (Exception e1)
            {
                //这里处理queryThread为空的异常
            }

          
            Thread.Sleep(100);
        }

        public bool isHoming = false;
        public bool isHitting = false;

        public bool HomeComplete = false;
        public bool HitReturn = false;
        public bool HitComplete = false;


        #region button
        private void BT_CNT_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.m_isConnected =  mdv.pmacCard.ConectToPMAC();
            mdv.PMAC_msg = mdv.pmacCard.m_PMAC_msg;
            TBK_outMsg.Text = mdv.PMAC_msg;

            BT_SrvOn.IsEnabled = true;
            BT_HOME.IsEnabled = true;

            
            mdv.pmacCard.SendCMD("DISABLE PLC 1");
            mdv.pmacCard.SendCMD("DISABLE PLC 2");
            mdv.pmacCard.SendCMD("DISABLE PLC 3");
            mdv.pmacCard.SendCMD("i5 = 0");

            BT_ReInit_Click(null, null); //初始化参数

            // 监视状态的线程，每500ms查询一次p100~p104, 并作出相应的响应。
            
            queryThread = new Thread(() =>
            {
                while (true)
                {
                    Thread.Sleep(500);
                    this.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,
                           new Action(() =>
                           {
                               mdv.pmacCard.SendCMD("p100,5");
                               mdv.PMAC_msg = mdv.pmacCard.m_PMAC_msg;
                               //p100: home complete
                               //p101: hit complete
                               string [] statue = mdv.PMAC_msg.Split('\n');
                               TB_Statue.Text = statue.Length.ToString();
                               if (statue.Length != 6)
                               {
                                   TB_Statue.Text = "fail to query";
                                   return;
                               } 

                               HomeComplete = (statue[0] == "1") ? true : false;
                               HitReturn = (statue[1] == "1") ? true : false;
                               HitComplete = (statue[2] == "1") ? true : false;

                               TB_Statue.Text = HomeComplete.ToString() + " " + HitReturn.ToString() + " " + HitComplete.ToString();

                               //  复位监视
                               if (HomeComplete && isHoming)
                               {                               
                                   mdv.pmacCard.SendCMD("DISABLE PLC 1");
                                   this.mdv.pmacCard.SendCMD("#1 k");
                                   this.mdv.pmacCard.SendCMD("#2 k");
                                   this.mdv.pmacCard.SendCMD("#3 k");
                                   this.mdv.pmacCard.SendCMD("i5 = 0");
                                   isHoming = false;
                                   MessageBox.Show("Home Complete");
                               }
                              
                               //  回位保存数据
                              // if (HitComplete && isHitting && (CB_SAVETRACE.IsChecked == true))
                               //if (HitComplete && isHitting)
                               //{
                               //    mdv.pmacCard.SendCMD("LIST GAT");
                               //    Thread.Sleep(500);
                               //    mdv.PMAC_msg = mdv.pmacCard.m_PMAC_msg;

                               //    fileIndex = int.Parse(cfgFile.IniReadValue("SaveFilePara", "fileIndex"));
                               //    string filedist = cfgFile.IniReadValue("SaveFilePara", "DirName");
                               //    filedist = filedist + "\\A" + fileIndex.ToString() + "_PMACData.m"; //m文件必须以字母开始
                               //    WriteToFile(filedist,
                               //        "OriData = {'" + mdv.PMAC_msg.Replace(" ", "' , '").Replace("\n", "' ,...\n '") + "'};");
                               //    isHitting = false;
                               //}                       
                           }
                    ));
                }

            });
            queryThread.Start();



        }

        private void BT_SrvOn_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.ServoON();
            TBK_Statue.Text = "Serve on";
            BT_Run.IsEnabled = true;
        }
        //针对新程序的改动
        private void BT_RunPro_Click(object sender, RoutedEventArgs e)
        {
            //设定坐标系
            mdv.pmacCard.SendCMD("Undefine all");
            mdv.pmacCard.SendCMD("&2\n #1->187.5X \n #3->187.5Z"); //各轴的导程，cnt/mm      
            mdv.pmacCard.SendCMD("&4\n #2->136.05Y");
            mdv.pmacCard.SendCMD("DEF GAT");
            //运行相应程序
            mdv.pmacCard.RunProgram(2, 11);//x轴和Z轴的程序 
          //  mdv.pmacCard.RunProgram(2, 12);//x轴和Z轴的程序，带切球,I5211延时
          //  mdv.pmacCard.RunProgram(4, 13);//y轴的程序
            mdv.pmacCard.RunProgram(4, 15);//y轴用I5212延时的程序
            //mdv.pmacCard.SendCMD("i5=3");
            //mdv.pmacCard.SendCMD("ENABLE PLC 3");

            TBK_Statue.Text = "RUNNING";
            BT_HitOne.IsEnabled = true;
            BT_Stop.IsEnabled = true;
        }

        public void BT_HitOne_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.SendCMD("P4 = 1");
        }

        private void BT_Stop_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.SendCMD("P3 = 0");
            mdv.pmacCard.ServoOFF();

            TBK_Statue.Text = "Stoped";
            mdv.pmacCard.SendCMD("i5=0");
            BT_SrvOn.IsEnabled = true;
        }

        
        //复位命令。调用PLC1的复位程序。
        private void BT_HOME_Click(object sender, RoutedEventArgs e) 
        {
            mdv.pmacCard.SendCMD("i5=3");
            mdv.pmacCard.SendCMD("ENABLE PLC 1");
            isHoming = true;
        }

        private void BT_ReInit_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.SendCMD("i5=3");
            mdv.pmacCard.SendCMD("ENABLE PLC 2");
            Thread.Sleep(100);
            mdv.pmacCard.SendCMD("i5 = 0");
        }


        private void BT_Send_Click(object sender, RoutedEventArgs e)
        {
            mdv.pmacCard.SendCMD(TBX_inputMsg.Text);
            mdv.PMAC_msg = mdv.pmacCard.m_PMAC_msg;
            TBK_outMsg.Text = mdv.PMAC_msg;
           //MessageBox.Show(mdv.PMAC_msg);
        }


        //private void BT_UpLoadData_Click(object sender, RoutedEventArgs e)
        //{
        //   // queryThread.Suspend(); //暂时关闭查询用线程
        //    mdv.pmacCard.SendCMD("LIST GAT");
        //    mdv.PMAC_msg = mdv.pmacCard.m_PMAC_msg;
        //    System.Windows.Forms.SaveFileDialog sfd = new System.Windows.Forms.SaveFileDialog();
        //    sfd.InitialDirectory = "E:\\";
        //    sfd.Filter = "m文件(*.m)|*.m|txt文件(*.txt)|*.txt";
        //    if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
        //    {
        //        WriteToFile(sfd.FileName, "OriData = {'" + mdv.PMAC_msg.Replace(" ","' , '").Replace("\n", "' ,...\n '") + "'};");
        //        MessageBox.Show("uploaded complete");
        //    }
        //    //queryThread.Resume();
        //}

        #endregion

        static void WriteToFile(string dir, string data)
        {
            StreamWriter SW;
            SW = File.CreateText(dir);
            SW.Write(data);
            SW.Close();
        }
        //==========================================
        public bool flag_isMoving = false;

        public int hitcnt = 0;

        public double t_last_RC = 0; //上次接收到信息的时间戳。 只有当时间戳间距大于阈值时，才会启动新轨迹。
        public bool Recv4Vision(double[] padPar)
        {
            if(! mdv.pmacCard.m_isConnected)//若没有连上，则不做处理
            { return false; }


         //   float T = (float)padPar[7] * 1000; //调节时间单位到ms
        //    SetNotes( T.ToString() );
           

            bool flag_alreadySend = (padPar[0] == 1) ? true : false; //判断是否是这条轨迹中第一次收到数据
            
            flag_alreadySend = false;
            SetNotes("New Trace");
            if (padPar[8] - t_last_RC < 100)
            {
                flag_alreadySend = true;
                SetNotes("Old Trace");
            }
            t_last_RC = padPar[8];
            

            flag_isMoving = mdv.QueryIsMoving();

            if (!flag_isMoving) //击球过程结束时，把当前值设为0 以防止死区导致下次运动失效
            {
                mdv.X_Now = 0;
                mdv.Y_Now = 0;
                mdv.Z_Now = 0;
            }

            if (!flag_isMoving && !flag_alreadySend) //只在当前机构没有运动过，且对当前来球轨迹没有响应过时，才启动y轴
           // if (!flag_isMoving ) //只在当前机构没有运动过，且对当前来球轨迹没有响应过时，才启动y轴
            {
                mdv.pmacCard.SendCMD("DEL GAT"); //清除历史数据
                mdv.pmacCard.SendCMD("DEF GAT"); 
                mdv.pmacCard.SendCMD("GAT");//开始数据采集

              //  mdv.SetY((float)padPar[2]);
                mdv.SetT((float)(padPar[7]*1000 + delay+10));  //调整时间
                string cmd = "p108 = 1,"
                           + "p4 = 1," 
                           + mdv.GetCmdXZ((float)padPar[1], (float)padPar[3]);
                mdv.pmacCard.SendCMD(cmd); //发送打球命令
                //mdv.SetXZ((float)padPar[1], (float)padPar[3]);
                isHitting = true;
               // ZSPCt.SetZSP( (int)padPar[5] , (int)padPar[6]); 
                hitcnt = 1;
                return true;
            }
            else
            {
                hitcnt++;
                mdv.SetT((float)(padPar[7] * 1000 + delay + 10));  //调整时间
                mdv.SetXZ((float)padPar[1],(float)(padPar[3]));
             //   ZSPCt.SetZSP((int)padPar[5], (int)padPar[6]); //若连续发，由于串口的延时，会造成整个系统的延时。

                SetNotes("hitcnt = " + hitcnt.ToString());
                return false;                
            }


        }

        public bool HomeZsp()
        {
            ZSPCt.SetZSP(0,0);
            return false;
        }

        private void SetNotes(string notes)
        {
            if (Dispatcher.Thread != Thread.CurrentThread)
            {
                this.TBK_VisionMsg.Dispatcher.Invoke(new Action(() =>
                {
                    this.TBK_VisionMsg.Text = notes;
                }));
            }
            else
            {
                this.TBK_VisionMsg.Text = notes;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            delay = Convert.ToDouble(TB_Delay.Text);
        }

      

    }

    class IniFile
    {
        public string path;
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder
        retVal, int size, string filePath);
        public IniFile(string INIPath)
        {
            path = INIPath;
        }
        public void IniWriteValue(string Section, string Key, string Value)
        {
            WritePrivateProfileString(Section, Key, Value, this.path);
        }
        public string IniReadValue(string Section, string Key)
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section, Key, "", temp, 255, this.path);
            return temp.ToString();
        }
    }
}
