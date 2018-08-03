namespace ProtocolTooler
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.gen = new System.Windows.Forms.Button();
            this.src_addr_input = new System.Windows.Forms.TextBox();
            this.src_addr = new System.Windows.Forms.Label();
            this.dst_addr = new System.Windows.Forms.Label();
            this.dst_addr_input = new System.Windows.Forms.TextBox();
            this.result_input = new System.Windows.Forms.TextBox();
            this.start = new System.Windows.Forms.Label();
            this.start_input = new System.Windows.Forms.TextBox();
            this.len_input = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.frameId_input = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.fctrl_input = new System.Windows.Forms.CheckedListBox();
            this.label5 = new System.Windows.Forms.Label();
            this.dataId_input = new System.Windows.Forms.TextBox();
            this.data_input = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.parse = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.ip_input = new System.Windows.Forms.TextBox();
            this.port_input = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.oper_btn = new System.Windows.Forms.Button();
            this.send_btn = new System.Windows.Forms.Button();
            this.crc_btn = new System.Windows.Forms.Button();
            this.ToDec_btn = new System.Windows.Forms.Button();
            this.ToHex_btn = new System.Windows.Forms.Button();
            this.hex_input = new System.Windows.Forms.TextBox();
            this.dec_input = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.socket_recv_input = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.protocol_type_input = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel6 = new System.Windows.Forms.Panel();
            this.label28 = new System.Windows.Forms.Label();
            this.panel7 = new System.Windows.Forms.Panel();
            this.label30 = new System.Windows.Forms.Label();
            this.p2p_data_input = new System.Windows.Forms.TextBox();
            this.label29 = new System.Windows.Forms.Label();
            this.p2p_dataid_input = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.total_frame_input = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.p2p_checkBox = new System.Windows.Forms.CheckBox();
            this.current_frame_input = new System.Windows.Forms.TextBox();
            this.divPack_checkBox = new System.Windows.Forms.CheckBox();
            this.Security_checkbox = new System.Windows.Forms.CheckBox();
            this.label27 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.select_btn = new System.Windows.Forms.Button();
            this.find_data_id_input = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.save_result_btn = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel6.SuspendLayout();
            this.panel7.SuspendLayout();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // gen
            // 
            resources.ApplyResources(this.gen, "gen");
            this.gen.Name = "gen";
            this.gen.UseVisualStyleBackColor = true;
            this.gen.Click += new System.EventHandler(this.gen_Click);
            // 
            // src_addr_input
            // 
            resources.ApplyResources(this.src_addr_input, "src_addr_input");
            this.src_addr_input.Name = "src_addr_input";
            // 
            // src_addr
            // 
            resources.ApplyResources(this.src_addr, "src_addr");
            this.src_addr.Name = "src_addr";
            // 
            // dst_addr
            // 
            resources.ApplyResources(this.dst_addr, "dst_addr");
            this.dst_addr.Name = "dst_addr";
            // 
            // dst_addr_input
            // 
            resources.ApplyResources(this.dst_addr_input, "dst_addr_input");
            this.dst_addr_input.Name = "dst_addr_input";
            // 
            // result_input
            // 
            resources.ApplyResources(this.result_input, "result_input");
            this.result_input.Name = "result_input";
            // 
            // start
            // 
            resources.ApplyResources(this.start, "start");
            this.start.Name = "start";
            // 
            // start_input
            // 
            resources.ApplyResources(this.start_input, "start_input");
            this.start_input.Name = "start_input";
            this.start_input.ReadOnly = true;
            // 
            // len_input
            // 
            resources.ApplyResources(this.len_input, "len_input");
            this.len_input.Name = "len_input";
            this.len_input.ReadOnly = true;
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // frameId_input
            // 
            resources.ApplyResources(this.frameId_input, "frameId_input");
            this.frameId_input.Name = "frameId_input";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // fctrl_input
            // 
            this.fctrl_input.FormattingEnabled = true;
            this.fctrl_input.Items.AddRange(new object[] {
            resources.GetString("fctrl_input.Items"),
            resources.GetString("fctrl_input.Items1")});
            resources.ApplyResources(this.fctrl_input, "fctrl_input");
            this.fctrl_input.Name = "fctrl_input";
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
            // 
            // dataId_input
            // 
            resources.ApplyResources(this.dataId_input, "dataId_input");
            this.dataId_input.Name = "dataId_input";
            // 
            // data_input
            // 
            resources.ApplyResources(this.data_input, "data_input");
            this.data_input.Name = "data_input";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // parse
            // 
            resources.ApplyResources(this.parse, "parse");
            this.parse.Name = "parse";
            this.parse.UseVisualStyleBackColor = true;
            this.parse.Click += new System.EventHandler(this.parse_Click);
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // ip_input
            // 
            resources.ApplyResources(this.ip_input, "ip_input");
            this.ip_input.Name = "ip_input";
            // 
            // port_input
            // 
            resources.ApplyResources(this.port_input, "port_input");
            this.port_input.Name = "port_input";
            // 
            // label9
            // 
            resources.ApplyResources(this.label9, "label9");
            this.label9.Name = "label9";
            // 
            // oper_btn
            // 
            resources.ApplyResources(this.oper_btn, "oper_btn");
            this.oper_btn.Name = "oper_btn";
            this.oper_btn.UseVisualStyleBackColor = true;
            this.oper_btn.Click += new System.EventHandler(this.oper_btn_Click);
            // 
            // send_btn
            // 
            resources.ApplyResources(this.send_btn, "send_btn");
            this.send_btn.Name = "send_btn";
            this.send_btn.UseVisualStyleBackColor = true;
            this.send_btn.Click += new System.EventHandler(this.send_btn_Click);
            // 
            // crc_btn
            // 
            resources.ApplyResources(this.crc_btn, "crc_btn");
            this.crc_btn.Name = "crc_btn";
            this.crc_btn.UseVisualStyleBackColor = true;
            this.crc_btn.Click += new System.EventHandler(this.crc_btn_Click);
            // 
            // ToDec_btn
            // 
            resources.ApplyResources(this.ToDec_btn, "ToDec_btn");
            this.ToDec_btn.Name = "ToDec_btn";
            this.ToDec_btn.UseVisualStyleBackColor = true;
            this.ToDec_btn.Click += new System.EventHandler(this.ToDec_btn_Click);
            // 
            // ToHex_btn
            // 
            resources.ApplyResources(this.ToHex_btn, "ToHex_btn");
            this.ToHex_btn.Name = "ToHex_btn";
            this.ToHex_btn.UseVisualStyleBackColor = true;
            this.ToHex_btn.Click += new System.EventHandler(this.ToHex_btn_Click);
            // 
            // hex_input
            // 
            resources.ApplyResources(this.hex_input, "hex_input");
            this.hex_input.Name = "hex_input";
            // 
            // dec_input
            // 
            resources.ApplyResources(this.dec_input, "dec_input");
            this.dec_input.Name = "dec_input";
            // 
            // label11
            // 
            resources.ApplyResources(this.label11, "label11");
            this.label11.Name = "label11";
            // 
            // socket_recv_input
            // 
            resources.ApplyResources(this.socket_recv_input, "socket_recv_input");
            this.socket_recv_input.Name = "socket_recv_input";
            // 
            // label12
            // 
            resources.ApplyResources(this.label12, "label12");
            this.label12.Name = "label12";
            // 
            // label13
            // 
            resources.ApplyResources(this.label13, "label13");
            this.label13.Name = "label13";
            // 
            // label14
            // 
            resources.ApplyResources(this.label14, "label14");
            this.label14.Name = "label14";
            // 
            // label15
            // 
            resources.ApplyResources(this.label15, "label15");
            this.label15.Name = "label15";
            // 
            // protocol_type_input
            // 
            resources.ApplyResources(this.protocol_type_input, "protocol_type_input");
            this.protocol_type_input.Name = "protocol_type_input";
            // 
            // label16
            // 
            resources.ApplyResources(this.label16, "label16");
            this.label16.Name = "label16";
            // 
            // label17
            // 
            resources.ApplyResources(this.label17, "label17");
            this.label17.Name = "label17";
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.start);
            this.panel1.Controls.Add(this.start_input);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.len_input);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.label17);
            this.panel2.Controls.Add(this.frameId_input);
            this.panel2.Controls.Add(this.panel3);
            this.panel2.Controls.Add(this.src_addr);
            this.panel2.Controls.Add(this.src_addr_input);
            this.panel2.Controls.Add(this.dst_addr);
            this.panel2.Controls.Add(this.dst_addr_input);
            resources.ApplyResources(this.panel2, "panel2");
            this.panel2.Name = "panel2";
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.Transparent;
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.label16);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Controls.Add(this.fctrl_input);
            this.panel3.Controls.Add(this.protocol_type_input);
            resources.ApplyResources(this.panel3, "panel3");
            this.panel3.Name = "panel3";
            // 
            // panel6
            // 
            this.panel6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel6.Controls.Add(this.label28);
            this.panel6.Controls.Add(this.panel7);
            this.panel6.Controls.Add(this.label5);
            this.panel6.Controls.Add(this.dataId_input);
            this.panel6.Controls.Add(this.label7);
            this.panel6.Controls.Add(this.data_input);
            resources.ApplyResources(this.panel6, "panel6");
            this.panel6.Name = "panel6";
            // 
            // label28
            // 
            resources.ApplyResources(this.label28, "label28");
            this.label28.Name = "label28";
            // 
            // panel7
            // 
            this.panel7.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel7.Controls.Add(this.label30);
            this.panel7.Controls.Add(this.p2p_data_input);
            this.panel7.Controls.Add(this.label29);
            this.panel7.Controls.Add(this.p2p_dataid_input);
            this.panel7.Controls.Add(this.label19);
            this.panel7.Controls.Add(this.total_frame_input);
            this.panel7.Controls.Add(this.label18);
            this.panel7.Controls.Add(this.p2p_checkBox);
            this.panel7.Controls.Add(this.current_frame_input);
            this.panel7.Controls.Add(this.divPack_checkBox);
            this.panel7.Controls.Add(this.Security_checkbox);
            resources.ApplyResources(this.panel7, "panel7");
            this.panel7.Name = "panel7";
            // 
            // label30
            // 
            resources.ApplyResources(this.label30, "label30");
            this.label30.Name = "label30";
            // 
            // p2p_data_input
            // 
            resources.ApplyResources(this.p2p_data_input, "p2p_data_input");
            this.p2p_data_input.Name = "p2p_data_input";
            this.p2p_data_input.ReadOnly = true;
            // 
            // label29
            // 
            resources.ApplyResources(this.label29, "label29");
            this.label29.Name = "label29";
            // 
            // p2p_dataid_input
            // 
            resources.ApplyResources(this.p2p_dataid_input, "p2p_dataid_input");
            this.p2p_dataid_input.Name = "p2p_dataid_input";
            this.p2p_dataid_input.ReadOnly = true;
            // 
            // label19
            // 
            resources.ApplyResources(this.label19, "label19");
            this.label19.Name = "label19";
            // 
            // total_frame_input
            // 
            resources.ApplyResources(this.total_frame_input, "total_frame_input");
            this.total_frame_input.Name = "total_frame_input";
            this.total_frame_input.ReadOnly = true;
            // 
            // label18
            // 
            resources.ApplyResources(this.label18, "label18");
            this.label18.Name = "label18";
            // 
            // p2p_checkBox
            // 
            resources.ApplyResources(this.p2p_checkBox, "p2p_checkBox");
            this.p2p_checkBox.Name = "p2p_checkBox";
            this.p2p_checkBox.UseVisualStyleBackColor = true;
            this.p2p_checkBox.CheckedChanged += new System.EventHandler(this.p2p_checkBox_CheckedChanged);
            // 
            // current_frame_input
            // 
            resources.ApplyResources(this.current_frame_input, "current_frame_input");
            this.current_frame_input.Name = "current_frame_input";
            this.current_frame_input.ReadOnly = true;
            // 
            // divPack_checkBox
            // 
            resources.ApplyResources(this.divPack_checkBox, "divPack_checkBox");
            this.divPack_checkBox.Name = "divPack_checkBox";
            this.divPack_checkBox.UseVisualStyleBackColor = true;
            this.divPack_checkBox.CheckedChanged += new System.EventHandler(this.divPack_checkBox_CheckedChanged);
            // 
            // Security_checkbox
            // 
            resources.ApplyResources(this.Security_checkbox, "Security_checkbox");
            this.Security_checkbox.Name = "Security_checkbox";
            this.Security_checkbox.UseVisualStyleBackColor = true;
            this.Security_checkbox.CheckedChanged += new System.EventHandler(this.Security_checkbox_CheckedChanged);
            // 
            // label27
            // 
            resources.ApplyResources(this.label27, "label27");
            this.label27.Name = "label27";
            // 
            // panel4
            // 
            this.panel4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel4.Controls.Add(this.ip_input);
            this.panel4.Controls.Add(this.port_input);
            this.panel4.Controls.Add(this.label9);
            this.panel4.Controls.Add(this.label8);
            this.panel4.Controls.Add(this.oper_btn);
            this.panel4.Controls.Add(this.send_btn);
            this.panel4.Controls.Add(this.label13);
            this.panel4.Controls.Add(this.socket_recv_input);
            this.panel4.Controls.Add(this.label12);
            this.panel4.Controls.Add(this.label11);
            this.panel4.Controls.Add(this.hex_input);
            this.panel4.Controls.Add(this.ToDec_btn);
            this.panel4.Controls.Add(this.crc_btn);
            this.panel4.Controls.Add(this.dec_input);
            this.panel4.Controls.Add(this.ToHex_btn);
            resources.ApplyResources(this.panel4, "panel4");
            this.panel4.Name = "panel4";
            // 
            // select_btn
            // 
            resources.ApplyResources(this.select_btn, "select_btn");
            this.select_btn.Name = "select_btn";
            this.select_btn.UseVisualStyleBackColor = true;
            this.select_btn.Click += new System.EventHandler(this.select_btn_Click);
            // 
            // find_data_id_input
            // 
            resources.ApplyResources(this.find_data_id_input, "find_data_id_input");
            this.find_data_id_input.Name = "find_data_id_input";
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // save_result_btn
            // 
            resources.ApplyResources(this.save_result_btn, "save_result_btn");
            this.save_result_btn.Name = "save_result_btn";
            this.save_result_btn.UseVisualStyleBackColor = true;
            this.save_result_btn.Click += new System.EventHandler(this.save_result_btn_Click);
            // 
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.Name = "label6";
            // 
            // label10
            // 
            resources.ApplyResources(this.label10, "label10");
            this.label10.Name = "label10";
            // 
            // Form1
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.save_result_btn);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.find_data_id_input);
            this.Controls.Add(this.select_btn);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.label27);
            this.Controls.Add(this.panel6);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.gen);
            this.Controls.Add(this.parse);
            this.Controls.Add(this.result_input);
            this.Controls.Add(this.panel2);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel6.ResumeLayout(false);
            this.panel6.PerformLayout();
            this.panel7.ResumeLayout(false);
            this.panel7.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button gen;
        private System.Windows.Forms.TextBox src_addr_input;
        private System.Windows.Forms.Label src_addr;
        private System.Windows.Forms.Label dst_addr;
        private System.Windows.Forms.TextBox dst_addr_input;
        private System.Windows.Forms.TextBox result_input;
        private System.Windows.Forms.Label start;
        private System.Windows.Forms.TextBox start_input;
        private System.Windows.Forms.TextBox len_input;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox frameId_input;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckedListBox fctrl_input;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox dataId_input;
        private System.Windows.Forms.TextBox data_input;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button parse;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox ip_input;
        private System.Windows.Forms.TextBox port_input;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button oper_btn;
        private System.Windows.Forms.Button send_btn;
        private System.Windows.Forms.TextBox dec_input;
        private System.Windows.Forms.TextBox hex_input;
        private System.Windows.Forms.Button ToDec_btn;
        private System.Windows.Forms.Button ToHex_btn;
        private System.Windows.Forms.Button crc_btn;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox socket_recv_input;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
//         private Microsoft.VisualBasic.PowerPacks.ShapeContainer shapeContainer1;
//         private Microsoft.VisualBasic.PowerPacks.RectangleShape rectangleShape1;
        private System.Windows.Forms.Label label14;
        //private Microsoft.VisualBasic.PowerPacks.RectangleShape rectangleShape2;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox protocol_type_input;
        private System.Windows.Forms.Label label16;
        //private Microsoft.VisualBasic.PowerPacks.RectangleShape rectangleShape3;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Panel panel7;
        private System.Windows.Forms.CheckBox Security_checkbox;
        private System.Windows.Forms.CheckBox divPack_checkBox;
        private System.Windows.Forms.CheckBox p2p_checkBox;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TextBox total_frame_input;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox current_frame_input;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.TextBox p2p_data_input;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.TextBox p2p_dataid_input;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Button select_btn;
        private System.Windows.Forms.TextBox find_data_id_input;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button save_result_btn;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label10;
    }
}

