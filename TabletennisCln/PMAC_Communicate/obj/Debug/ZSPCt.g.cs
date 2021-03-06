﻿#pragma checksum "..\..\ZSPCt.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "B1110FBAA83680C26ECD8FDF5B49DA82"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.35317
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using PMAC_Communicate;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace PMAC_Communicate {
    
    
    /// <summary>
    /// ZSPCt
    /// </summary>
    public partial class ZSPCt : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        
        #line 11 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal PMAC_Communicate.data d;
        
        #line default
        #line hidden
        
        
        #line 14 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button BT_WaitZSP;
        
        #line default
        #line hidden
        
        
        #line 16 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Slider SL_s;
        
        #line default
        #line hidden
        
        
        #line 17 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Slider SL_p;
        
        #line default
        #line hidden
        
        
        #line 23 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBlock TBK_sPos;
        
        #line default
        #line hidden
        
        
        #line 24 "..\..\ZSPCt.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBlock TBK_pPos;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/PMAC_Communicate;component/zspct.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\ZSPCt.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            
            #line 7 "..\..\ZSPCt.xaml"
            ((PMAC_Communicate.ZSPCt)(target)).Unloaded += new System.Windows.RoutedEventHandler(this.UserControl_Unloaded);
            
            #line default
            #line hidden
            return;
            case 2:
            this.d = ((PMAC_Communicate.data)(target));
            return;
            case 3:
            this.BT_WaitZSP = ((System.Windows.Controls.Button)(target));
            
            #line 14 "..\..\ZSPCt.xaml"
            this.BT_WaitZSP.Click += new System.Windows.RoutedEventHandler(this.BTC_WaitZSP);
            
            #line default
            #line hidden
            return;
            case 4:
            this.SL_s = ((System.Windows.Controls.Slider)(target));
            return;
            case 5:
            this.SL_p = ((System.Windows.Controls.Slider)(target));
            return;
            case 6:
            
            #line 18 "..\..\ZSPCt.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.BTC_HomeZSP);
            
            #line default
            #line hidden
            return;
            case 7:
            
            #line 21 "..\..\ZSPCt.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.BTC_CloseZSP);
            
            #line default
            #line hidden
            return;
            case 8:
            this.TBK_sPos = ((System.Windows.Controls.TextBlock)(target));
            return;
            case 9:
            this.TBK_pPos = ((System.Windows.Controls.TextBlock)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

