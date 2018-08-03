namespace ServerManager
{
    partial class Main
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
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.notify = new System.Windows.Forms.NotifyIcon(this.components);
            this.rightMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.config_menu = new System.Windows.Forms.ToolStripMenuItem();
            this.protocol_menu = new System.Windows.Forms.ToolStripMenuItem();
            this.exit = new System.Windows.Forms.ToolStripMenuItem();
            this.start_stop_service = new System.Windows.Forms.Button();
            this.result = new System.Windows.Forms.TextBox();
            this.textboxMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.parse = new System.Windows.Forms.ToolStripMenuItem();
            this.empty_textbox = new System.Windows.Forms.ToolStripMenuItem();
            this.clear_btn = new System.Windows.Forms.Button();
            this.config_btn = new System.Windows.Forms.Button();
            this.protocol_btn = new System.Windows.Forms.Button();
            this.self_config_btn = new System.Windows.Forms.Button();
            this.db_config_btn = new System.Windows.Forms.Button();
            this.root_path_btn = new System.Windows.Forms.Button();
            this.rightMenu.SuspendLayout();
            this.textboxMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // notify
            // 
            this.notify.BalloonTipText = "再次使用请双击图标";
            this.notify.BalloonTipTitle = "Hi";
            this.notify.ContextMenuStrip = this.rightMenu;
            this.notify.Icon = ((System.Drawing.Icon)(resources.GetObject("notify.Icon")));
            this.notify.Text = "管理器";
            this.notify.Visible = true;
            this.notify.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notify_MouseDoubleClick);
            // 
            // rightMenu
            // 
            this.rightMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.config_menu,
            this.protocol_menu,
            this.exit});
            this.rightMenu.Name = "contextMenuStrip1";
            this.rightMenu.Size = new System.Drawing.Size(101, 70);
            // 
            // config_menu
            // 
            this.config_menu.Name = "config_menu";
            this.config_menu.Size = new System.Drawing.Size(100, 22);
            this.config_menu.Text = "配置";
            this.config_menu.Click += new System.EventHandler(this.config_menu_Click);
            // 
            // protocol_menu
            // 
            this.protocol_menu.Name = "protocol_menu";
            this.protocol_menu.Size = new System.Drawing.Size(100, 22);
            this.protocol_menu.Text = "调试";
            this.protocol_menu.Visible = false;
            this.protocol_menu.Click += new System.EventHandler(this.protocol_menu_Click);
            // 
            // exit
            // 
            this.exit.Name = "exit";
            this.exit.Size = new System.Drawing.Size(100, 22);
            this.exit.Text = "退出";
            this.exit.Click += new System.EventHandler(this.exit_Click);
            // 
            // start_stop_service
            // 
            this.start_stop_service.Location = new System.Drawing.Point(12, 23);
            this.start_stop_service.Name = "start_stop_service";
            this.start_stop_service.Size = new System.Drawing.Size(75, 23);
            this.start_stop_service.TabIndex = 1;
            this.start_stop_service.Text = "启动";
            this.start_stop_service.UseVisualStyleBackColor = true;
            this.start_stop_service.Click += new System.EventHandler(this.start_stop_service_Click);
            // 
            // result
            // 
            this.result.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.result.BackColor = System.Drawing.SystemColors.Window;
            this.result.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.result.ContextMenuStrip = this.textboxMenu;
            this.result.Cursor = System.Windows.Forms.Cursors.Default;
            this.result.Font = new System.Drawing.Font("宋体", 12F);
            this.result.Location = new System.Drawing.Point(12, 66);
            this.result.MaxLength = 102400;
            this.result.Multiline = true;
            this.result.Name = "result";
            this.result.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.result.Size = new System.Drawing.Size(941, 410);
            this.result.TabIndex = 2;
            this.result.WordWrap = false;
            // 
            // textboxMenu
            // 
            this.textboxMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.parse,
            this.empty_textbox});
            this.textboxMenu.Name = "textboxMenu";
            this.textboxMenu.Size = new System.Drawing.Size(101, 48);
            // 
            // parse
            // 
            this.parse.Name = "parse";
            this.parse.Size = new System.Drawing.Size(100, 22);
            this.parse.Text = "解码";
            this.parse.Click += new System.EventHandler(this.parse_Click);
            // 
            // empty_textbox
            // 
            this.empty_textbox.Name = "empty_textbox";
            this.empty_textbox.Size = new System.Drawing.Size(100, 22);
            this.empty_textbox.Text = "清空";
            this.empty_textbox.Click += new System.EventHandler(this.empty_textbox_Click);
            // 
            // clear_btn
            // 
            this.clear_btn.Location = new System.Drawing.Point(668, 23);
            this.clear_btn.Name = "clear_btn";
            this.clear_btn.Size = new System.Drawing.Size(75, 23);
            this.clear_btn.TabIndex = 3;
            this.clear_btn.Text = "清空";
            this.clear_btn.UseVisualStyleBackColor = true;
            this.clear_btn.Visible = false;
            this.clear_btn.Click += new System.EventHandler(this.clear_btn_Click);
            // 
            // config_btn
            // 
            this.config_btn.Location = new System.Drawing.Point(112, 23);
            this.config_btn.Name = "config_btn";
            this.config_btn.Size = new System.Drawing.Size(75, 23);
            this.config_btn.TabIndex = 4;
            this.config_btn.Text = "配置";
            this.config_btn.UseVisualStyleBackColor = true;
            this.config_btn.Click += new System.EventHandler(this.config_btn_Click);
            // 
            // protocol_btn
            // 
            this.protocol_btn.Location = new System.Drawing.Point(332, 23);
            this.protocol_btn.Name = "protocol_btn";
            this.protocol_btn.Size = new System.Drawing.Size(75, 23);
            this.protocol_btn.TabIndex = 5;
            this.protocol_btn.Text = "调试";
            this.protocol_btn.UseVisualStyleBackColor = true;
            this.protocol_btn.Visible = false;
            this.protocol_btn.Click += new System.EventHandler(this.protocol_btn_Click);
            // 
            // self_config_btn
            // 
            this.self_config_btn.Location = new System.Drawing.Point(444, 23);
            this.self_config_btn.Name = "self_config_btn";
            this.self_config_btn.Size = new System.Drawing.Size(75, 23);
            this.self_config_btn.TabIndex = 6;
            this.self_config_btn.Text = "打开配置";
            this.self_config_btn.UseVisualStyleBackColor = true;
            this.self_config_btn.Visible = false;
            this.self_config_btn.Click += new System.EventHandler(this.test_btn_Click);
            // 
            // db_config_btn
            // 
            this.db_config_btn.Location = new System.Drawing.Point(555, 23);
            this.db_config_btn.Name = "db_config_btn";
            this.db_config_btn.Size = new System.Drawing.Size(75, 23);
            this.db_config_btn.TabIndex = 7;
            this.db_config_btn.Text = "打开存储";
            this.db_config_btn.UseVisualStyleBackColor = true;
            this.db_config_btn.Visible = false;
            this.db_config_btn.Click += new System.EventHandler(this.db_config_btn_Click);
            // 
            // root_path_btn
            // 
            this.root_path_btn.Location = new System.Drawing.Point(218, 23);
            this.root_path_btn.Name = "root_path_btn";
            this.root_path_btn.Size = new System.Drawing.Size(75, 23);
            this.root_path_btn.TabIndex = 8;
            this.root_path_btn.Text = "程序目录";
            this.root_path_btn.UseVisualStyleBackColor = true;
            this.root_path_btn.Click += new System.EventHandler(this.root_path_btn_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(965, 488);
            this.ContextMenuStrip = this.rightMenu;
            this.Controls.Add(this.root_path_btn);
            this.Controls.Add(this.result);
            this.Controls.Add(this.db_config_btn);
            this.Controls.Add(this.self_config_btn);
            this.Controls.Add(this.protocol_btn);
            this.Controls.Add(this.config_btn);
            this.Controls.Add(this.clear_btn);
            this.Controls.Add(this.start_stop_service);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.Main_Load);
            this.rightMenu.ResumeLayout(false);
            this.textboxMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NotifyIcon notify;
        private System.Windows.Forms.ContextMenuStrip rightMenu;
        private System.Windows.Forms.ToolStripMenuItem exit;
        private System.Windows.Forms.Button start_stop_service;
        private System.Windows.Forms.TextBox result;
        private System.Windows.Forms.Button clear_btn;
        private System.Windows.Forms.Button config_btn;
        private System.Windows.Forms.Button protocol_btn;
        private System.Windows.Forms.Button self_config_btn;
        private System.Windows.Forms.ToolStripMenuItem config_menu;
        private System.Windows.Forms.ToolStripMenuItem protocol_menu;
        private System.Windows.Forms.Button db_config_btn;
        private System.Windows.Forms.ContextMenuStrip textboxMenu;
        private System.Windows.Forms.ToolStripMenuItem empty_textbox;
        private System.Windows.Forms.ToolStripMenuItem parse;
        private System.Windows.Forms.Button root_path_btn;
    }
}

