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

namespace ServerManager
{
    public partial class Monitor : Form
    {
        //存储数据ID对应的名称
        Dictionary<string, string> nameMap = new Dictionary<string, string>();

        //存储数据ID对应的命令
        Dictionary<string, string> commandMap = new Dictionary<string, string>();

        String configPath = Application.StartupPath + "\\" + Application.ProductName + ".ini";

        public Monitor()
        {
            InitializeComponent();
        }

        private void Monitor_Load(object sender, EventArgs e)
        {
            loadConfig();

            drawView();    
        }

        private void loadConfig()
        {
            NameValueCollection nameValue = new NameValueCollection();

            ConfigHelper.ReadSectionValues("Name", nameValue, configPath);

            IEnumerator myEnumerator = nameValue.GetEnumerator();

            foreach (String data_id in nameValue.AllKeys)
            {
                string[] name = nameValue.GetValues(data_id);
                if(name != null && name.Length > 0)
                {
                    nameMap.Add(data_id, name[0]);
                }
            }

            NameValueCollection cmdValue = new NameValueCollection();

            ConfigHelper.ReadSectionValues("Command", cmdValue, configPath);

            IEnumerator myEnumerator2 = cmdValue.GetEnumerator();

            foreach (String data_id in cmdValue.AllKeys)
            {
                string[] cmd = nameValue.GetValues(data_id);
                if (cmd != null && cmd.Length > 0)
                {
                    commandMap.Add(data_id, cmd[0]);
                }
            }
        }

        private void drawView()
        {
            int index = 0;

            foreach(string data_id in nameMap.Keys)
            {
                Button btn = new Button();

                btn.Text = nameMap[data_id];
                btn.Tag = data_id;

                btn.Location = new Point(50 + index * 100, 30 + ( index / 3 ) * 50 );

                btn.Click += new EventHandler(btn_click);

                this.Controls.Add(btn);

                index++;
            }
        }

        private void btn_click(object sender,EventArgs e)
        {
            Button btn = (Button)sender;
            MessageBox.Show(btn.Tag.ToString());
        }

    }
}
