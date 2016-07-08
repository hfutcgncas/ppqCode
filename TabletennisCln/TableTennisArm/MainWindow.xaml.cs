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

namespace TableTennisArm
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            vision = new cVision();
        }
        //=======================================
        private readonly cVision vision; //接收视觉结果

        public void ConectToVision()
        {
            string notes;
            vision.initRcv();
            Thread RcvThread = new Thread(new ThreadStart(delegate
            {
                while (true)
                {
                    vision.ReciveData();
                    notes = "   X  = " + vision.hitPar[1].ToString() +
                                   "\n Y  = " + vision.hitPar[2].ToString() +
                                   "\n Z  = " + vision.hitPar[3].ToString() +
                                   "\n Vx = " + vision.hitPar[4].ToString() +
                                   "\n Vy = " + vision.hitPar[5].ToString() +
                                   "\n Vz = " + vision.hitPar[6].ToString() +
                                   "\n tg = " + vision.hitPar[12].ToString();
                    SetNotes(notes);
                    // Hit_X = vision.hitPar[1];
                    //  Hit_Y = 200;
                    // bat.pmac_card.m_PMAC_cmd = "P4 = 1";
                    // bat.pmac_card.SendCMD();

                    // MessageBox.Show("get here");
                    Thread.Sleep(50);
                }
            }
            ));
            RcvThread.Start();

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ConectToVision();
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
    }
}
