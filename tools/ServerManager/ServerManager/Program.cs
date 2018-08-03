using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ServerManager
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            int iCase = 10;

            if (iCase == 1)
            {
                Application.Run(new Monitor());
            }
            else if (iCase == 2)
            {
                Application.Run(new ProtocolTools());
            }
            else if (iCase == 3)
            {
                Application.Run(new ConfigForm());
            }
            else if (iCase == 10)
            {
                //Process[] processes = System.Diagnostics.Process.GetProcessesByName(Application.ProductName);
                //if (processes.Length > 1)
                //{
                //    MessageBox.Show("应用程序已经在运行中。。");
                //    Thread.Sleep(1000);
                //    System.Environment.Exit(1);
                //}
                //else
                {
                    Application.Run(new Main());
                }
            }
        }
    }
}
