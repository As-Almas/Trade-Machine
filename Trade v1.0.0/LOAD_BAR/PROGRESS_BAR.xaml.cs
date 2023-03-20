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
using System.Windows.Shapes;

namespace Trade_v1_0_0
{
    /// <summary>
    /// Логика взаимодействия для PROGRESS_BAR.xaml
    /// </summary>
    public partial class PROGRESS_BAR : Window
    {
        public PROGRESS_BAR()
        {
            InitializeComponent();
        }

        private void Update()
        {
            ToolTip tip = new ToolTip();
            int PerCent = (int)((this.PROGRESS_POINTER.Value*100)/this.PROGRESS_POINTER.Maximum);
            tip.Content = "Готово на: " + PerCent.ToString() + "%";
            this.PROGRESS_POINTER.ToolTip = tip;
        }

        public void SetMaximum(double max)
        {
            this.PROGRESS_POINTER.Maximum = max;
            Update();
        }
        public void SetValue(double value)
        {
            this.PROGRESS_POINTER.Value = value;
            Update();
        }

        public void AddProgress(double value)
        {
            this.PROGRESS_POINTER.Value += value;
            Update();
        }

        public void SubProgress(double value)
        {
            this.PROGRESS_POINTER.Value -= value;
            Update();
        }

        public void SetTextOfProgress(String text)
        {
           this.PROGRESS_TEXT.Text = text;
        }
    }
}
