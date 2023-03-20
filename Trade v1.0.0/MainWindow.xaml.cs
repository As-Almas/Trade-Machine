using System.Windows;
using System;
using System.Runtime.InteropServices;



namespace Trade_v1_0_0
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        ~MainWindow()
        {
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate bool _TLoadDB(string path, char key);
        
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void _TCloseDB();

        private void ToTradeWindow(object sender, RoutedEventArgs e)
        {
            TRADE_WINDOW tWND = null;
            try
            {
            this.Close();
            PROGRESS_BAR pBar = new PROGRESS_BAR();

                pBar.Show();
                pBar.SetMaximum(100);
                pBar.SetValue(1);
                pBar.SetTextOfProgress("Подготовка окна...");
                tWND = new TRADE_WINDOW();

                pBar.SetTextOfProgress("Идёт загрузка \"DB_Handler.dll\"...");
                pBar.AddProgress(2);
                DLL DB_DLL = new DLL("DB_Handler.dll");



                _TLoadDB addr = (_TLoadDB)DB_DLL.GetFunctionAddress("LoadDBData", typeof(_TLoadDB));
                var res = addr("TestDB.tdb", (char)1);
                throw new Exception("Err");

                pBar.SetTextOfProgress("Идёт загрузка \"DB_Handler.dll\"...");
                pBar.AddProgress(4);
            }
            catch(Exception err)
            {
                if(tWND != null)
                    tWND.Close();
                MessageBoxResult result = MessageBox.Show("ОШИБКА! "+err.Message+"!Перезапустить?", "ОШИБКА!", MessageBoxButton.YesNo, MessageBoxImage.Error);
                if(result == MessageBoxResult.Yes)
                {
                    System.Diagnostics.Process.Start(Application.ResourceAssembly.Location);
                }

            }
        }

    }
}
