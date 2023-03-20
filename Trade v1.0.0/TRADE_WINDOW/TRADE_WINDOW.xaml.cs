using System.Windows;

namespace Trade_v1_0_0
{
    struct DATA_BASE {
        public string[][] values { get; set; }
        public string[] headers { get; set; }
        public int headersCount { get; set; }
        public int ValueCount { get; set; }
        
    };
    /// <summary>
    /// Логика взаимодействия для TRADE_WINDOW.xaml
    /// </summary>
    public partial class TRADE_WINDOW : Window
    {
        public TRADE_WINDOW()
        {
            InitializeComponent();
        }

        private void ToMainMenu(object sender, System.EventArgs e)
        {
            MainWindow nMainWindow = new MainWindow();
            nMainWindow.Show();
            this.Close();
        }

        public bool DoTableFromDB()
        {
            return true;
        }
       
    }
}
