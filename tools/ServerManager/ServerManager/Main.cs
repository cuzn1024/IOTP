using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Thrift.Transport;
using Thrift.Protocol;
using Thrift.Server;

namespace ServerManager
{
    // 1.定义委托  
    public delegate void DelReadStdOutput(string result);
    public delegate void DelReadErrOutput(string result);
    public delegate void ExitEvent();
    
    public partial class Main : Form
    {
        // 2.定义委托事件  
        public event DelReadStdOutput ReadStdOutput;
        public event DelReadErrOutput ReadErrOutput;
        public event ExitEvent exitEvent;

        //Windows状态
        FormWindowState state  = FormWindowState.Normal;

        //通知栏图标显示最大次数
        const int iMaxShowTimes = 1;

        //通知栏图标显示次数
        int iShowTimes = 0;

        //服务程序的配置路径
        String serverConfigPath_x = Application.StartupPath+"\\conf\\db.ini";

        String selfConfigPath_x = Application.StartupPath+"\\"+Application.ProductName + ".ini";

        //服务器程序名称
        String serverName = "IOT-Platform";

        //服务程序tcp端口
        short port = 0;

        ProtocolTools protocol = null;

        public Main()
        {
            InitializeComponent();
            
            Rectangle rect = SystemInformation.WorkingArea;

            this.Height = (int)(rect.Height * 0.75);

            this.Width = (int)(rect.Width * 0.75);

            init();
        }

        private void init()
        {
            //3.将相应函数注册到委托事件中  
            ReadStdOutput += new DelReadStdOutput(ReadStdOutputAction);
            ReadErrOutput += new DelReadErrOutput(ReadErrOutputAction);
            exitEvent = new ExitEvent(Process_Exit);

            /*
            //判断配置文件是否存在
            if (!File.Exists(serverConfigPath))
            {
                MessageBox.Show("服务程序配置文件未找到");
                System.Environment.Exit(1);
                return;
            }
            

            serverName = ConfigHelper.ReadIniData("Server", "Name", "", selfConfigPath);

            if (serverName.Length <= 0)
            {
                MessageBox.Show("未发现服务程序名称");
                System.Environment.Exit(1);
            }

            port = short.Parse(ConfigHelper.ReadIniData("Server", "Port", "", selfConfigPath));

            config = new ConfigForm(serverConfigPath);

            protocol = new ProtocolTools(selfConfigPath, port,false);
            */
            closeServer();
        }

        // 重写OnClosing使点击关闭按键时窗体能够缩进托盘
        protected override void OnClosing(CancelEventArgs e)
        {
            e.Cancel = true;
            hideForm();
            if(iShowTimes < iMaxShowTimes)
            {
                this.notify.ShowBalloonTip(2000);
                iShowTimes++;
            }
            
        }

        private void notify_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if(!this.Visible)
            {
                showForm();
            }
            else
            {
                hideForm();
            }
        }

        private void hideForm()
        {
            state = this.WindowState;
            this.Hide();
        }

        private void showForm()
        {
            this.Show();
            this.Activate();
            this.WindowState = state;
        }

        private void exit_Click(object sender, EventArgs e)
        {
            //RaiseSignalToServer((int)(eSignalType.E_Signal_Close));
            iShowTimes = 1;
            this.Close();
            notify.Dispose(); 
            closeServer();
            System.Environment.Exit(0);
        }

        private void start_stop_service_Click(object sender, EventArgs e)
        {
            if (start_stop_service.Text.Equals("启动"))
            {
                if (File.Exists(serverName + ".exe"))
                {
                    RunCommand(serverName + ".exe", "");
                    start_stop_service.Text = "停止";
                    this.result.Text = "";
                }
                else
                {
                    MessageBox.Show("未发现程序文件，无法运行");
                }
            }
            else if (start_stop_service.Text.Equals("停止"))
            {
                //RaiseSignalToServer((int)(eSignalType.E_Signal_Close));
                closeServer();
                start_stop_service.Text = "启动";
            }            
        }

        private void closeServer()
        {
            Process[] processes = System.Diagnostics.Process.GetProcessesByName(serverName);
            for (int i = 0; i < processes.Length; i++)
            {
                processes[i].Kill();
            }
        }

        private void RunCommand(string StartFileName, string StartFileArg)
        {
            Process CmdProcess = new Process();
            CmdProcess.StartInfo.FileName = StartFileName;      // 命令  
            CmdProcess.StartInfo.Arguments = StartFileArg;      // 参数  

            CmdProcess.StartInfo.CreateNoWindow = true;         // 不创建新窗口  
            CmdProcess.StartInfo.UseShellExecute = false;
            CmdProcess.StartInfo.RedirectStandardInput = true;  // 重定向输入  
            CmdProcess.StartInfo.RedirectStandardOutput = true; // 重定向标准输出  
            CmdProcess.StartInfo.RedirectStandardError = true;  // 重定向错误输出  
                                                                //CmdProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;  

            CmdProcess.OutputDataReceived += new DataReceivedEventHandler(p_OutputDataReceived);
            CmdProcess.ErrorDataReceived += new DataReceivedEventHandler(p_ErrorDataReceived);

            CmdProcess.EnableRaisingEvents = true;                      // 启用Exited事件  
            CmdProcess.Exited += new EventHandler(CmdProcess_Exited);   // 注册进程结束事件  

            CmdProcess.Start();
            CmdProcess.BeginOutputReadLine();
            CmdProcess.BeginErrorReadLine();

            // 如果打开注释，则以同步方式执行命令，此例子中用Exited事件异步执行。  
            // CmdProcess.WaitForExit();       
        }

        private void p_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                // 4. 异步调用，需要invoke  
                this.Invoke(ReadStdOutput, new object[] { e.Data });
            }
        }

        private void p_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                this.Invoke(ReadErrOutput, new object[] { e.Data });
            }
        }

        private void p_ExitEvent()
        {
            this.Invoke(exitEvent);
        }

        private void ReadStdOutputAction(string s)
        {
            if(result.TextLength > (result.MaxLength - 1000))
            {
                result.Clear();
            }
            this.result.AppendText(s + "\r\n");
        }

        private void ReadErrOutputAction(string s)
        {
            this.result.AppendText(s + "\r\n");
        }

        private void Process_Exit()
        {
            this.start_stop_service.Text = "启动";
            this.result.AppendText("程序已经退出" + "\r\n");
        }

        private void CmdProcess_Exited(object sender, EventArgs e)
        {
            p_ExitEvent();
        }

        private void clear_btn_Click(object sender, EventArgs e)
        {
            this.result.Clear();
        }

        public int RaiseSignalToServer(int key)
        {
            /*
            if (this.start_stop_service.Text == "启动")
            {
                return 0;
            }

            int iRet = 0;
            try
            {
                TTransport trans;
                trans = new TSocket("localhost", port);
                trans = new TFramedTransport(trans);
                trans.Open();

                TProtocol Protocol = new TBinaryProtocol(trans, true, true);

                TMultiplexedProtocol multiplex;

                multiplex = new TMultiplexedProtocol(Protocol, "s1");
                RaiseSignalService.Iface client = new RaiseSignalService.Client(multiplex);

                iRet = client.raiseSignal((eSignalType)(key));
                
                trans.Close();
            }
            catch(Exception ex)
            {
                this.result.AppendText(ex.Message + "\r\n");
                iRet = 0;
            }
            
            return iRet;
            */
            return 0;
        }

        private void config_btn_Click(object sender, EventArgs e)
        {
            if (start_stop_service.Text != "停止")
            {
                MessageBox.Show("请先启动服务");
                return;
            }

            new ConfigForm().ShowDialog();
            //new ConfigForm(serverConfigPath).ShowDialog();
        }

        private void protocol_btn_Click(object sender, EventArgs e)
        {
            protocol.Show();
        }

        private void test_btn_Click(object sender, EventArgs e)
        {
            //System.Diagnostics.Process.Start(selfConfigPath);
        }

        private void config_menu_Click(object sender, EventArgs e)
        {
            config_btn_Click(sender, e);
        }

        private void protocol_menu_Click(object sender, EventArgs e)
        {
            protocol_btn_Click(sender, e);
        }

        private void db_config_btn_Click(object sender, EventArgs e)
        {
            //System.Diagnostics.Process.Start(serverConfigPath);
        }

        private void Main_Load(object sender, EventArgs e)
        {
            //this.SizeChanged += new EventHandler(Main_SizeChanged);
        }

        private void Main_SizeChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                hideForm();
            }
        }

        private void empty_textbox_Click(object sender, EventArgs e)
        {
            clear_btn_Click(sender, e);
        }

        private void parse_Click(object sender, EventArgs e)
        {
            string selectedText = result.SelectedText;

            if(selectedText == null || selectedText.Length <= 0)
            {
                MessageBox.Show("并未选择内容");
                return;
            }

            if(!Tools.isHexString(selectedText))
            {
                MessageBox.Show("所选并非十六进制数据");
                return;
            }

            if((selectedText.Length % 2) != 0)
            {
                MessageBox.Show("数据异常 00 两位补齐");
                return;
            }

            byte[] b = Tools.HexStringToBytes(selectedText);

            if(b == null || b.Length == 0)
            {
                return;
            }

            Protocol protocol = new Protocol();

            string msg = "";

            int iRet = protocol.setBytes(b,out msg);

            if(iRet != 0)
            {
                MessageBox.Show(msg);
                return;
            }

            MessageBox.Show(protocol.ToString());
        }

        private void root_path_btn_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("Explorer.exe", Application.StartupPath);
        }
    }
}
