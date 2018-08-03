using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections.Specialized;
using System.Collections;

namespace ServerManager
{
    public partial class ProtocolTools : Form
    {
        Protocol protocol = new Protocol();

        Protocol forShowTips = new Protocol();

        Socket clientSocket = null;

        byte[] recvBuffer = new byte[1024];

        SynchronizationContext m_SyncContext = null;

        String configFilePath = "";

        String ip = "";
        String port = "";

        bool bFormClose = true;

        //存储数据ID对应的名称
        Dictionary<string, string> nameMap = new Dictionary<string, string>();

        public ProtocolTools()
        {
            InitializeComponent();

            m_SyncContext = SynchronizationContext.Current;

            configFilePath = Application.StartupPath + "\\ProtocolTools.ini";

            port = 1024+"";

            reload_btn_Click(null, null);

            bFormClose = true;
        }

        private void initMenu()
        {
            ip_input.Text = GetLocalIp();
            ip = GetLocalIp();
        }

        private string GetLocalIp()
        {
            string hostname = Dns.GetHostName();//得到本机名  
            IPHostEntry localhost = Dns.GetHostEntry(hostname);
            String ip = "255.255.255.255";

            for (int i = 0; i < localhost.AddressList.Length; i++)
            {
                IPAddress localaddr = localhost.AddressList[i];
                if (localaddr.ToString().Contains("192.168."))
                {
                    if (String.Compare(localaddr.ToString(), ip) < 0)
                    {
                        ip = localaddr.ToString();
                    }
                }
            }
            return ip;
        }

        // 重写OnClosing使点击关闭按键时窗体能够缩进托盘
        protected override void OnClosing(CancelEventArgs e)
        {
            if (!bFormClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        /*
        private void ThreadProcSafePost()
        {
            m_SyncContext.Post(SetTextSafePost,"safely.");
        }
        */

        private void SetTextSafePost(object text)
        {
            oper_btn.Text = text.ToString();
        }

        private void SetResultSafePost(object text)
        {
            socket_recv_input.Text = text.ToString();

            string tips;

            forShowTips.setBytes(HexStringToBytes(text.ToString()), out tips);

            string key = Protocol.byteToHexString(forShowTips.getDataId());

            if (nameMap.ContainsKey(key))
            {
                tips_label.Text = "接收 " + nameMap[key] + " 指令";
            }
        }

        private bool isHexString(String s)
        {
            return Regex.IsMatch(s, "^[0-9A-Fa-f]+$");
        }

        private bool isIp(String ip)
        {
            string pattern = @"^((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)$";

            //验证
            return Regex.IsMatch(ip, pattern);
        }

        private void LoadDataFromConfig()
        {
            if (!File.Exists(configFilePath))
            {
                return;
            }

            /*
                Default_Start_Flag=7e7e
                Default_Len=0000
                Default_Frame_Id=01
                Default_Protocol_Type=01
                Default_AckReq=0
                Default_AckFlg=0
                Default_Src=6400
                Default_Dst=0100
                Default_Security=0
                Default_DivPack=0
                Default_P2P=0
                Default_Current_Frame=00
                Default_Total_Frame=01
                Default_P2p_DataId=00
                Default_P2P_Data=00
                Default_DataId=01
                Default_Data=00
              */
            String Default_Start_Flag = ConfigHelper.ReadIniData("Default", "Default_Start_Flag", "", configFilePath);
            protocol.setStartFlag(HexStringToShort(Default_Start_Flag));

            String Default_Frame_Id = ConfigHelper.ReadIniData("Default", "Default_Frame_Id", "", configFilePath);
            protocol.setFrameId(HexStringToByte(Default_Frame_Id));

            String Default_Protocol_Type = ConfigHelper.ReadIniData("Default", "Default_Protocol_Type", "", configFilePath);
            protocol.setProtocolType(HexStringToByte(Default_Protocol_Type));

            String Default_AckReq = ConfigHelper.ReadIniData("Default", "Default_AckReq", "", configFilePath);

            if (Default_AckReq.Equals("0"))
            {
                protocol.setAckReq(false);
            }
            else if (Default_AckReq.Equals("1"))
            {
                protocol.setAckReq(true);
            }

            String Default_AckFlg = ConfigHelper.ReadIniData("Default", "Default_AckFlg", "", configFilePath);

            if (Default_AckFlg.Equals("0"))
            {
                protocol.setAckFlag(false);
            }
            else if (Default_AckFlg.Equals("1"))
            {
                protocol.setAckFlag(true);
            }

            String Default_Src = ConfigHelper.ReadIniData("Default", "Default_Src", "", configFilePath);
            protocol.setSrcNetworkAddr(HexStringToShort(Default_Src));

            String Default_Dst = ConfigHelper.ReadIniData("Default", "Default_Dst", "", configFilePath);
            protocol.setDstNetworkAddr(HexStringToShort(Default_Dst));

            String Default_Security = ConfigHelper.ReadIniData("Default", "Default_Security", "", configFilePath);
            if (Default_Security.Equals("0"))
            {
                protocol.setSecurity(false);
            }
            else if (Default_Security.Equals("1"))
            {
                protocol.setSecurity(true);
            }

            String Default_DivPack = ConfigHelper.ReadIniData("Default", "Default_DivPack", "", configFilePath);
            if (Default_DivPack.Equals("0"))
            {
                protocol.setbDivPack(false);
            }
            else if (Default_DivPack.Equals("1"))
            {
                protocol.setbDivPack(true);
            }

            String Default_P2P = ConfigHelper.ReadIniData("Default", "Default_P2P", "", configFilePath);
            if (Default_P2P.Equals("0"))
            {
                protocol.setbHaveP2p(false);
            }
            else if (Default_P2P.Equals("1"))
            {
                protocol.setbHaveP2p(true);
            }

            String Default_Current_Frame = ConfigHelper.ReadIniData("Default", "Default_Current_Frame", "", configFilePath);
            protocol.setCurrentFrame(HexStringToByte(Default_Current_Frame));

            String Default_Total_Frame = ConfigHelper.ReadIniData("Default", "Default_Total_Frame", "", configFilePath);
            protocol.setTotalFrameSize(HexStringToByte(Default_Total_Frame));

            String Default_P2p_DataId = ConfigHelper.ReadIniData("Default", "Default_P2p_DataId", "", configFilePath);
            protocol.setP2pDataId(HexStringToByte(Default_P2p_DataId));

            String Default_P2P_Data = ConfigHelper.ReadIniData("Default", "Default_P2P_Data", "", configFilePath);
            protocol.setP2pData(HexStringToBytes(Default_P2P_Data));

            String Default_DataId = ConfigHelper.ReadIniData("Default", "Default_DataId", "", configFilePath);
            protocol.setDataId(HexStringToByte(Default_DataId));

            String Default_Data = ConfigHelper.ReadIniData("Default", "Default_Data", "", configFilePath);
            protocol.setData(HexStringToBytes(Default_Data));

            String Default_Ip = ConfigHelper.ReadIniData("Default", "Default_Ip", "", configFilePath);
            ip = Default_Ip;

            String Default_Port = ConfigHelper.ReadIniData("Default", "Default_Port", "", configFilePath);
            port = Default_Port;
        }

        private void refreshView()
        {
            //设置IP
            ip_input.Text = ip;

            //设置端口
            port_input.Text = port;

            //设置开始标志
            start_input.Text = Protocol.shortToHexString(protocol.getStartFlag());

            //设置长度
            len_input.Text = Protocol.shortToHexString(protocol.getLen());

            //设置帧序号
            frameId_input.Text = Protocol.byteToHexString(protocol.getFrameid());

            //设置类型
            protocol_type_input.Text = Protocol.byteToHexString(protocol.getProtocolType());

            //设置帧控制
            fctrl_input.SetItemChecked(0, protocol.getAckReq());
            fctrl_input.SetItemChecked(1, protocol.getAckFlag());

            //设置来源地址
            src_addr_input.Text = Protocol.shortToHexString(protocol.getSrcNetworkAddr());

            //设置目标地址
            dst_addr_input.Text = Protocol.shortToHexString(protocol.getDstNetworkAddr());

            Security_checkbox.Checked = protocol.getSecurity();


            current_frame_input.Text = Protocol.byteToHexString(protocol.getCurrentFrame());

            total_frame_input.Text = Protocol.byteToHexString(protocol.getTotalFrameSize());

            //设置数据ID
            p2p_dataid_input.Text = Protocol.byteToHexString(protocol.getP2pDataId());

            //设置数据ID
            dataId_input.Text = Protocol.byteToHexString(protocol.getDataId());

            //设置数据内容
            byte[] data = protocol.getData();
            if (null != data)
            {
                data_input.Text = Protocol.bytesToHexString(data, 0, data.Length);
            }

            //设置数据内容
            byte[] p2pData = protocol.getP2pData();
            if (null != p2pData)
            {
                p2p_data_input.Text = Protocol.bytesToHexString(p2pData, 0, p2pData.Length);
            }

            divPack_checkBox.Checked = protocol.getbDivPack();

            p2p_checkBox.Checked = protocol.getbHaveP2p();

            if (!protocol.getbDivPack())
            {
                total_frame_input.Text = "";
                current_frame_input.Text = "";
            }

            if (!protocol.getbHaveP2p())
            {
                p2p_dataid_input.Text = "";
                p2p_data_input.Text = "";
            }
        }

        private void gen_Click(object sender, EventArgs e)
        {
            //获取开始标志
            String start_flag = start_input.Text.ToString();
            protocol.setStartFlag(HexStringToShort(start_flag));

            //获取帧序号
            String frame_id = frameId_input.Text.ToString();
            protocol.setFrameId(HexStringToByte(frame_id));

            //获取协议类型
            String protoType = protocol_type_input.Text.ToString();
            protocol.setProtocolType(HexStringToByte(protoType));

            //获取帧控制            
            protocol.setAckReq(fctrl_input.GetItemChecked(0));
            protocol.setAckFlag(fctrl_input.GetItemChecked(1));

            protocol.setSrcNetworkAddr(HexStringToShort(src_addr_input.Text.ToString().Trim()));

            protocol.setDstNetworkAddr(HexStringToShort(dst_addr_input.Text.ToString().Trim()));

            protocol.setSecurity(Security_checkbox.Checked);

            protocol.setbDivPack(divPack_checkBox.Checked);

            protocol.setbHaveP2p(p2p_checkBox.Checked);

            protocol.setCurrentFrame(HexStringToByte(current_frame_input.Text.ToString()));

            protocol.setTotalFrameSize(HexStringToByte(total_frame_input.Text.ToString()));

            protocol.setP2pDataId(HexStringToByte(p2p_dataid_input.Text.ToString()));

            protocol.setDataId(HexStringToByte(dataId_input.Text.ToString().Trim()));

            String sData = data_input.Text.ToString().Trim();

            if (sData.Length == 0)
            {
                //MessageBox.Show("请输入数据内容");
                //return;
            }

            if (sData.Length > 0 && !isHexString(sData))
            {
                if (!isHexString(sData))
                {
                    MessageBox.Show("非十六进制格式数据");
                    return;
                }

                if (sData.Length % 2 != 0)
                {
                    MessageBox.Show("数据存在异议 请检查是否用0补齐所有数据");
                    return;
                }
            }

            byte[] data = HexStringToBytes(sData);

            protocol.setData(data);


            String sP2pData = p2p_data_input.Text.ToString().Trim();

            if (sP2pData.Length > 0 && !isHexString(sP2pData))
            {
                if (!isHexString(sP2pData))
                {
                    MessageBox.Show("非十六进制格式数据");
                    return;
                }

                if (sP2pData.Length % 2 != 0)
                {
                    MessageBox.Show("数据存在异议 请检查是否用0补齐所有数据");
                    return;
                }
            }

            byte[] p2pData = HexStringToBytes(sP2pData);

            protocol.setP2pData(p2pData);


            byte[] b = protocol.getBytes();

            if (b != null && b.Length > 0)
            {
                len_input.Text = Protocol.shortToHexString(protocol.getLen());
                result_input.Text = Protocol.bytesToHexString(b, 0, b.Length);
            }
        }

        private short HexStringToShort(string hexString)
        {
            if (hexString.Length != 4)
            {
                return 0;
            }

            byte L = Convert.ToByte(hexString.Substring(0, 2), 16);

            byte H = Convert.ToByte(hexString.Substring(2, 2), 16);

            return (short)((H << 8) | L);
        }

        private byte HexStringToByte(string hexString)
        {
            if (hexString.Length != 2)
            {
                return 0;
            }


            return Convert.ToByte(hexString, 16);
        }

        private byte[] HexStringToBytes(string hexString)
        {
            if (hexString.Length == 0)
            {
                return null;
            }

            hexString = hexString.Replace(" ", "");

            if ((hexString.Length % 2) != 0)
            {
                return null;
            }

            byte[] returnBytes = new byte[hexString.Length / 2];
            for (int i = 0; i < returnBytes.Length; i++)
            {
                returnBytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            return returnBytes;
        }

        private void parse_Click(object sender, EventArgs e)
        {
            //获取整帧数据

            String frame = socket_recv_input.Text.ToString().Trim().ToLower();

            frame = frame.Replace(" ", "");

            if (frame.Length == 0)
            {
                MessageBox.Show("请输入整帧数据");
                return;
            }

            if (!isHexString(frame))
            {
                MessageBox.Show("非十六进制格式数据");
                return;
            }

            if (frame.Length % 2 != 0)
            {
                MessageBox.Show("数据存在异议 请检查是否用0补齐所有数据");
                return;
            }

            byte[] b = HexStringToBytes(frame);

            //Console.WriteLine(BitConverter.ToString(b));

            if (null == b)
            {
                MessageBox.Show("数据解析异常");
                return;
            }

            String msg = "";

            protocol.setBytes(b, out msg);

            if (null != msg && msg.Length > 0)
            {
                MessageBox.Show(msg);
                return;
            }

            refreshView();
        }

        private void oper_btn_Click(object sender, EventArgs e)
        {
            if (clientSocket == null || !clientSocket.Connected)
            {
                String host = ip_input.Text.ToString().Trim();

                if (!isIp(host))
                {
                    MessageBox.Show("请输入合法的IP地址");
                    return;
                }

                String sPort = port_input.Text.ToString().Trim();

                int port = int.Parse(sPort);

                if (port < 1024 || port >= 60000)
                {
                    MessageBox.Show("端口不合法");
                    return;
                }

                System.Net.IPAddress ip = System.Net.IPAddress.Parse(host);
                clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                Connect(ip, port);
            }
            else
            {
                this.clientSocket.Shutdown(SocketShutdown.Both);
                this.clientSocket.Close();
                checkSocketIsOk();
            }
        }

        public void Connect(IPAddress ip, int port)
        {
            IAsyncResult connResult = this.clientSocket.BeginConnect(ip, port, new AsyncCallback(ConnectCallback), this.clientSocket);

            /*
            connResult.AsyncWaitHandle.WaitOne(2000, true);  //等待2秒

            if (!connResult.IsCompleted)
            {
                this.clientSocket.Close();

                //处理连接不成功的动作
                MessageBox.Show("2");
            }
            else
            {
                //处理连接成功的动作
                MessageBox.Show("1");
            }
            */
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                Socket handler = (Socket)ar.AsyncState;

                if (!handler.Connected)
                {
                    MessageBox.Show("连接失败");
                }

                handler.EndConnect(ar);
            }
            catch (Exception ex)
            {
                MessageBox.Show("连接失败:" + ex.Message);
            }

            checkSocketIsOk();
        }

        private void send_btn_Click(object sender, EventArgs e)
        {
            if (clientSocket == null || !clientSocket.Connected)
            {
                MessageBox.Show("TCP尚未准备好");
                return;
            }

            String frame = result_input.Text.ToString().Trim().ToLower();

            frame = frame.Replace(" ", "");

            if (frame.Length == 0)
            {
                MessageBox.Show("请输入整帧数据");
                return;
            }

            if (!isHexString(frame))
            {
                MessageBox.Show("非十六进制格式数据");
                return;
            }

            if (frame.Length % 2 != 0)
            {
                MessageBox.Show("数据存在异议 请检查是否用0补齐所有数据");
                return;
            }

            byte[] b = HexStringToBytes(frame);

            //Console.WriteLine(BitConverter.ToString(b));

            if (null == b)
            {
                MessageBox.Show("数据解析异常");
                return;
            }

            String tips = "";

            forShowTips.setBytes(b,out tips);

            string key = Protocol.byteToHexString(forShowTips.getDataId());

            if(nameMap.ContainsKey(key))
            {
                tips_label.Text = "发送 " + nameMap[key] + " 指令";
            }

            Send(b);

            ReceiveData();
        }

        private void Send(byte[] byteData)
        {
            try
            {
                this.clientSocket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), this.clientSocket);
            }
            catch (SocketException ex)
            {
                Tools.log(ex.Message);
            }
        }

        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                Socket handler = (Socket)ar.AsyncState;

                if (!handler.Connected)
                {
                    MessageBox.Show("发送失败");
                }

                handler.EndSend(ar);
            }
            catch (SocketException ex)
            {
                MessageBox.Show("发送失败:" + ex.Message);
            }
        }

        public void ReceiveData()
        {
            if (checkSocketIsOk())
            {
                clientSocket.BeginReceive(recvBuffer, 0, recvBuffer.Length, 0, new AsyncCallback(ReceiveCallback), null);
            }
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            if (!checkSocketIsOk())
            {
                return;
            }

            try
            {
                int REnd = clientSocket.EndReceive(ar);
                if (REnd > 0)
                {
                    byte[] data = new byte[REnd];
                    Array.Copy(recvBuffer, 0, data, 0, REnd);

                    m_SyncContext.Post(SetResultSafePost, Protocol.bytesToHexString(data, 0, data.Length));

                    //Console.WriteLine(Protocol.bytesToHexString(data,0,data.Length));
                    //在此次可以对data进行按需处理

                    clientSocket.BeginReceive(recvBuffer, 0, recvBuffer.Length, 0, new AsyncCallback(ReceiveCallback), null);
                }
                else
                {
                    dispose();
                }
            }
            catch (SocketException ex)
            {
                Tools.log(ex.Message);
            }
        }

        private void dispose()
        {
            try
            {
                this.clientSocket.Shutdown(SocketShutdown.Both);
                this.clientSocket.Close();
            }
            catch (Exception ex)
            {
                Tools.log(ex.Message);
            }

            checkSocketIsOk();
        }

        private bool checkSocketIsOk()
        {
            if (clientSocket != null && clientSocket.Connected)
            {
                //oper_btn.Text = "关闭";
                m_SyncContext.Post(SetTextSafePost, "关闭");

                return true;
            }
            else
            {
                //oper_btn.Text = "打开";
                m_SyncContext.Post(SetTextSafePost, "打开");

                return false;
            }
        }

        private void ToHex_btn_Click(object sender, EventArgs e)
        {
            String dec = dec_input.Text;

            if (dec.Length > 4)
            {
                return;
            }

            short value = 0;

            try
            {
                value = short.Parse(dec);
            }
            catch (Exception ex)
            {
                Tools.log(ex.Message);
                hex_input.Text = "";
                dec_input.Text = "";
            }

            if (value > byte.MaxValue)
            {
                hex_input.Text = Protocol.shortToHexString(value);
            }
            else
            {
                hex_input.Text = Protocol.byteToHexString((byte)value);
            }
        }

        private void ToDec_btn_Click(object sender, EventArgs e)
        {
            String hex = hex_input.Text;

            if (!isHexString(hex))
            {
                hex_input.Text = "";
                dec_input.Text = "";
                return;
            }

            if (hex.Length % 2 != 0)
            {
                hex = "0" + hex;
            }

            if (hex.Length == 2)
            {
                dec_input.Text = HexStringToByte(hex) + "";
            }
            else if (hex.Length == 4)
            {
                dec_input.Text = HexStringToShort(hex) + "";
            }
            else
            {
                MessageBox.Show("仅支持Short范围");
            }
        }

        private void crc_btn_Click(object sender, EventArgs e)
        {
            String hex = hex_input.Text;
            if (!isHexString(hex))
            {
                hex_input.Text = "";
                MessageBox.Show("输入为非十六进制字符串");
                return;
            }

            byte[] b = HexStringToBytes(hex);

            short s = protocol.getCRC16(b, 0, b.Length);

            MessageBox.Show("Short:" + s + " Hex:" + Protocol.shortToHexString(s));
        }

        private void Security_checkbox_CheckedChanged(object sender, EventArgs e)
        {
            Console.WriteLine(Security_checkbox.Checked);
        }

        private void divPack_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            total_frame_input.ReadOnly = !divPack_checkBox.Checked;
            current_frame_input.ReadOnly = !divPack_checkBox.Checked;

            if (divPack_checkBox.Checked)
            {
                total_frame_input.Text = Protocol.byteToHexString(protocol.getTotalFrameSize());
                current_frame_input.Text = Protocol.byteToHexString(protocol.getCurrentFrame());
            }
            else
            {
                total_frame_input.Text = "";
                current_frame_input.Text = "";
            }
        }

        private void p2p_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            p2p_dataid_input.ReadOnly = !p2p_checkBox.Checked;
            p2p_data_input.ReadOnly = !p2p_checkBox.Checked;

            if (p2p_checkBox.Checked)
            {
                p2p_dataid_input.Text = Protocol.byteToHexString(protocol.getP2pDataId());

                byte[] p2pData = protocol.getP2pData();
                if (null != p2pData)
                {
                    p2p_data_input.Text = Protocol.bytesToHexString(p2pData, 0, p2pData.Length);
                }
            }
            else
            {
                p2p_dataid_input.Text = "";
                p2p_data_input.Text = "";
            }
        }

        private void select_btn_Click(object sender, EventArgs e)
        {
            String find_data_id = find_data_id_input.Text.ToString();

            if (find_data_id.Length == 0)
            {
                MessageBox.Show("请输入数据ID后检索");
                return;
            }

            String value = ConfigHelper.ReadIniData("Data", find_data_id, "", configFilePath);

            if (value.Length > 0)
            {
                result_input.Text = value;

                socket_recv_input.Text = value;

                parse_Click(sender,e);
            }
            else
            {
                MessageBox.Show("未找到对应ID的完整帧");
            }
        }

        private void save_result_btn_Click(object sender, EventArgs e)
        {
            ConfigHelper.WriteIniData("Data", dataId_input.Text.ToString(), result_input.Text.ToString(), configFilePath);
        }

        private void open_config_file_btn_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(configFilePath);
        }

        private void reload_btn_Click(object sender, EventArgs e)
        {
            LoadDataFromConfig();

            refreshView();

            initMenu();

            loadConfig();
        }

        private void loadConfig()
        {
            nameMap.Clear();

            {
                NameValueCollection nameValue = new NameValueCollection();

                ConfigHelper.ReadSectionValues("Name", nameValue, configFilePath);

                foreach (String data_id in nameValue.AllKeys)
                {
                    string[] name = nameValue.GetValues(data_id);
                    if (name != null && name.Length > 0)
                    {
                        nameMap.Add(data_id, name[0]);
                    }
                }
            }

            Dictionary<string, string> siteMap = new Dictionary<string, string>();

            {
                

                NameValueCollection cmdValue = new NameValueCollection();

                ConfigHelper.ReadSectionValues("SiteCmd", cmdValue, configFilePath);

                foreach (String data_id in cmdValue.AllKeys)
                {
                    string[] cmd = cmdValue.GetValues(data_id);
                    if (cmd != null && cmd.Length > 0)
                    {
                        siteMap.Add(data_id, cmd[0]);
                    }
                }
            }

            Dictionary<string, string> deviceMap = new Dictionary<string, string>();

            {
                NameValueCollection cmdValue2 = new NameValueCollection();

                ConfigHelper.ReadSectionValues("DeviceCmd", cmdValue2, configFilePath);

                foreach (String data_id in cmdValue2.AllKeys)
                {
                    string[] cmd = cmdValue2.GetValues(data_id);
                    if (cmd != null && cmd.Length > 0)
                    {
                        deviceMap.Add(data_id, cmd[0]);
                    }
                }
            }

            drawView(siteMap, deviceMap);
        }

        private void drawView(Dictionary<string, string> siteMap,Dictionary<string, string> deviceMap)
        {
            drawSite(siteMap);
            drawDevice(deviceMap);
        }

        private void drawSite(Dictionary<string, string> siteMap)
        {
            site_btn_panel.Controls.Clear();

            int index = 0;

            int left = 50;

            int top = 10;

            Button[] button = new Button[siteMap.Count];

            foreach (string data_id in siteMap.Keys)
            {
                button[index] = new Button();

                button[index].Text = nameMap.ContainsKey(data_id) ? nameMap[data_id] : "未知";
                button[index].Tag = siteMap[data_id];
                button[index].Width = 150;
                button[index].TextAlign = ContentAlignment.MiddleCenter;

                button[index].Left = left;
                button[index].Top = top;

                button[index].Click += new EventHandler(btn_click);

                site_btn_panel.Controls.Add(button[index]);

                index++;

                if (index % 6 == 0)
                {
                    left = 50;
                    top += 35;
                }
                else
                {
                    left += 170;
                }
            }
        }

        private void drawDevice(Dictionary<string, string> deviceMap)
        {
            device_btn_panel.Controls.Clear();           

            int index = 0;

            int left = 50;

            int top = 10;

            Button[] button = new Button[deviceMap.Count];

            foreach (string data_id in deviceMap.Keys)
            {
                button[index] = new Button();

                button[index].Text = nameMap.ContainsKey(data_id) ? nameMap[data_id] : "未知";
                button[index].Tag = deviceMap[data_id];
                button[index].Width = 150;
                button[index].TextAlign = ContentAlignment.MiddleCenter;

                button[index].Left = left;
                button[index].Top = top;

                button[index].Click += new EventHandler(btn_click);

                device_btn_panel.Controls.Add(button[index]);

                index++;

                if (index % 6 == 0)
                {
                    left = 50;
                    top += 35;
                }
                else
                {
                    left += 170;
                }
            }
        }

        private void btn_click(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            //替换网络地址

            string network_addr = src_addr_input.Text.ToString();

            string frame = btn.Tag.ToString();

            if (frame.Length > 20)
            {
                frame = frame.Substring(0, 16) + network_addr + frame.Substring(20, frame.Length - 20);

                byte[] b = HexStringToBytes(frame);

                short s = protocol.getCRC16(b, 4, b.Length - 2 - 4);

                frame = frame.Substring(0, frame.Length - 4) + Protocol.shortToHexString(s);
            }

            result_input.Text = frame;

            send_btn_Click(sender, e);
        }
    }
}
