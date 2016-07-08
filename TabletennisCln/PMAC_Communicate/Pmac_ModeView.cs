using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

using System.Windows;
using Pmac_Comm;

namespace PMAC_Communicate
{
    public class Pmac_ModeView:INotifyPropertyChanged
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
        private float X_MAX = 700;
        private float X_MIN = -700;
        private float Y_MAX = 800;
        private float Y_MIN = 0;

        private float Z_MAX = 90; //相对于0点
        private float Z_MIN = -130;

        
        public float X_Now = 0;
        public float Y_Now = 0;

        public float Z_zeroPos = 320;  //z轴0位时的高度 单位mm
        public float Z_Now = 320;

       

        private float DEADZOOM = 30;//死区

        public string PMAC_msg
        {
            get { return pmacCard.m_PMAC_msg; }
            set
            {
                OnPropertyChanged("PMAC_msg");
            }
        }

        public cPMAC pmacCard = new cPMAC();

        #region cmd
        public void SetT(float T)//ms
        {
          //  pmacCard.SendCMD("P72 = " + T.ToString());

            pmacCard.SendCMD("I5211=(" + T.ToString() + "-20)*8388608/I10");
            pmacCard.SendCMD("P31 = " + T.ToString());
            pmacCard.SendCMD("I5212=(" + T.ToString() + "-(P73+(P71/P72))/2 )*8388608/I10");
            //pmacCard.SendCMD("P31 = " + T.ToString());

         
        }
        //配合rapid程序
        //public void SetX(float X)//mm
        //{
        //    if (X > X_MAX || X < X_MIN)//限幅
        //    {
        //        return;
        //    }
        //    if ((X - X_Now) < DEADZOOM && (X - X_Now) > -DEADZOOM) //死区
        //    {
        //        return;
        //    }
        //    X_Now = X;
        //    pmacCard.SendCMD("&3 !x " + X.ToString());
        //}
        //public void SetZ(float Z)//mm
        //{
        //    if ((Z - Z_zeroPos) > Z_MAX || (Z - Z_zeroPos) < Z_MIN)//限幅
        //    {
        //        return;
        //    }
        //    if ((Z - Z_Now) < DEADZOOM && (Z - Z_Now) > -DEADZOOM) //死区
        //    {
        //        return;
        //    }
        //    Z_Now = Z;
        //    pmacCard.SendCMD("&5 !Z " + (Z - Z_zeroPos).ToString()); //需要调整0位
        //}

        public void SetXZ(float X,float Z)//mm
        {
            if (X > X_MAX || X < X_MIN)//限幅
            {
                return;
            }
            //if ((X - X_Now) < DEADZOOM && (X - X_Now) > -DEADZOOM) //死区
            //{
            //    return;
            //}
            //X_Now = X;

            if ((Z - Z_zeroPos) > Z_MAX || (Z - Z_zeroPos) < Z_MIN)//限幅
            {
                return;
            }
            //if ((Z - Z_Now) < DEADZOOM && (Z - Z_Now) > -DEADZOOM) //死区
            //{
            //    return;
            //}
            //Z_Now = Z;

            pmacCard.SendCMD("&2 !X " + X.ToString() + " Z " + (Z - Z_zeroPos).ToString()+",p108=p108+1"); //需要调整0位
        }

        public string GetCmdXZ(float X,float Z)
        {
            if (X > X_MAX || X < X_MIN)//限幅
            {
                return "";
            }
            //if ((X - X_Now) < DEADZOOM && (X - X_Now) > -DEADZOOM) //死区
            //{
            //    return "";
            //}
            //X_Now = X;

            if ((Z - Z_zeroPos) > Z_MAX || (Z - Z_zeroPos) < Z_MIN)//限幅
            {
                return "";
            }
            //if ((Z - Z_Now) < DEADZOOM && (Z - Z_Now) > -DEADZOOM) //死区
            //{
            //    return "";
            //}
            //Z_Now = Z;

            return ("&2 !X " + X.ToString() + " Z " + (Z - Z_zeroPos).ToString()); //需要调整0位
        }


        public void SetY(float Y)//mm
        {
            if ( (2*Y) > Y_MAX || Y < Y_MIN) //限幅
            {
                return;
            } 
            Y_Now = Y;
            //pmacCard.SendCMD("P7 = " + Y.ToString());
            //pmacCard.SendCMD("P8 = " + (Y + 100).ToString());
            pmacCard.SendCMD("P8 = " + (2*Y).ToString());
        }
        public void SetVy(float Vy)
        {
            pmacCard.SendCMD("P71 = " + Vy.ToString());
        }

        public bool QueryIsMoving()
        {
            pmacCard.SendCMD("P4");
            if (1 == Convert.ToDouble(  pmacCard.m_PMAC_msg))
            { return true; }
            else
            { return false; }
        }
        public bool QueryIsHomeComplete()
        {
            pmacCard.SendCMD("M133");
            double M1InPos = Convert.ToDouble(pmacCard.m_PMAC_msg);
            pmacCard.SendCMD("M145");
            double M1HmComplete = Convert.ToDouble(pmacCard.m_PMAC_msg);

            pmacCard.SendCMD("M233");
            double M2InPos = Convert.ToDouble(pmacCard.m_PMAC_msg);
            pmacCard.SendCMD("M245");
            double M2HmComplete = Convert.ToDouble(pmacCard.m_PMAC_msg);

            pmacCard.SendCMD("M333");
            double M3InPos = Convert.ToDouble(pmacCard.m_PMAC_msg);
            pmacCard.SendCMD("M345");
            double M3HmComplete = Convert.ToDouble(pmacCard.m_PMAC_msg);

            if ((M1InPos==1) && (M1HmComplete==1) && (M2InPos==1) && (M2HmComplete==1) && (M3InPos==1) && (M3HmComplete==1))
            {
                return true;
            }
            return false;



           
        }
        #endregion
        

    }
}
