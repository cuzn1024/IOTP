using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Thrift.Transport;
using Thrift.Protocol;
using Thrift.Server;

namespace ServerManager
{
    public partial class ConfigForm : Form
    {
        TTransport trans = null;

        LogicFunctionService.Iface client = null;

        Dictionary<int, string> typeKey = new Dictionary<int, string>();

        Dictionary<string, int> typeValue = new Dictionary<string, int>();

        //新增行号
        int insertModeIndex = -1;

        public ConfigForm()
        {
            InitializeComponent();

            initData();

            initEvent();
        }

        public void initData()
        {
            typeKey.Add(10, "M基站");
            typeValue.Add("M基站", 10);

            typeKey.Add(11, "R基站");
            typeValue.Add("R基站", 11);

            typeKey.Add(12, "C基站");
            typeValue.Add("C基站", 12);

            try
            {
                trans = new TSocket("localhost", 9090);
                trans = new TFramedTransport(trans);
                trans.Open();

                TProtocol Protocol = new TBinaryProtocol(trans, true, true);
                
                TMultiplexedProtocol multiplex;
                multiplex = new TMultiplexedProtocol(Protocol, "LogicFunctionService");
                client = new LogicFunctionService.Client(multiplex);
            }
            catch (Exception ex)
            {
                MessageBox.Show("初始化异常:"+ex.Message);
                return;
            }

            loadDataToView();
        }

        public void initEvent()
        {
            //site_info_data.CellValueChanged += new DataGridViewCellEventHandler(site_info_data_CellValueChanged);
        }

        public void loadDataToView()
        {
            if (client != null)
            {
                network_status_checkBox.Checked = client.getNetworkStatus() > 0 ? true : false;

                Dictionary<string, eIOTNodeType> siteMap = client.getAllowSites();

                foreach (String key in siteMap.Keys)
                {
                    int index = site_info_data.Rows.Add();

                    site_info_data.Rows[index].Cells[0].Value = key;

                    site_info_data.Rows[index].Cells[1].Value = typeKey[(int)siteMap[key]]; ;

                    site_info_data.Rows[index].Cells[0].ReadOnly = true;

                    site_info_data.Rows[index].Cells[1].ReadOnly = true;
                }
            }
        }

        private void add_Click(object sender, EventArgs e)
        {
            if (insertModeIndex < 0)
            {
                insertModeIndex = site_info_data.Rows.Add();
                add.Text = "保存";
            }
            else
            {
                site_info_data_CellValueChanged(sender,null);
            }
        }

        private void edit_Click(object sender, EventArgs e)
        {
            //site_info_data.CurrentRow.Cells[1].ReadOnly = false;            
        }

        private void del_Click(object sender, EventArgs e)
        {
            MessageBox.Show("暂未开放");
        }

        private void site_info_data_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (insertModeIndex < 0)
            {
                return;
            }

            if (site_info_data.Rows[insertModeIndex].Cells[0].Value == null || site_info_data.Rows[insertModeIndex].Cells[1].Value == null)
            {
                return;
            }

            string addr = site_info_data.Rows[insertModeIndex].Cells[0].Value.ToString();

            string type = site_info_data.Rows[insertModeIndex].Cells[1].Value.ToString();

            if (addr == null || type == null || addr.Length <= 0 || type.Length <= 0)
            {
                return;
            }

            if (addr.Length != (8 * 2))
            {
                MessageBox.Show("物理地址不合法，应为8Byte 十六进制按零补齐");
                return;
            }

            site_info_data.Rows[insertModeIndex].Cells[1].ReadOnly = true;

            client.addSite(addr, (eIOTNodeType)(typeValue[type]));

            insertModeIndex = -1;

            add.Text = "新增";
        }

        private void network_status_checkBox_CheckedChanged(object sender, EventArgs e)
        {
            if (client != null)
            {
                client.operateNetwork(network_status_checkBox.Checked);
            }
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            if (trans != null)
            {
                trans.Close();
            }
        }

        private void rebuild_Click(object sender, EventArgs e)
        {
            MessageBox.Show("暂未开放");
        }

        private void del_device_Click(object sender, EventArgs e)
        {
            MessageBox.Show("暂未开放");
        }
    }
}
