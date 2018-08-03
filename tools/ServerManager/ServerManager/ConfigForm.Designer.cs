namespace ServerManager
{
    partial class ConfigForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            this.network_status = new System.Windows.Forms.Label();
            this.network_status_checkBox = new System.Windows.Forms.CheckBox();
            this.site_info_data = new System.Windows.Forms.DataGridView();
            this.phy_addr = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.site_type = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.add = new System.Windows.Forms.Button();
            this.del = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.del_device = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.rebuild = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.site_info_data)).BeginInit();
            this.SuspendLayout();
            // 
            // network_status
            // 
            this.network_status.AutoSize = true;
            this.network_status.Location = new System.Drawing.Point(134, 32);
            this.network_status.Name = "network_status";
            this.network_status.Size = new System.Drawing.Size(53, 12);
            this.network_status.TabIndex = 2;
            this.network_status.Text = "网络状态";
            // 
            // network_status_checkBox
            // 
            this.network_status_checkBox.AutoSize = true;
            this.network_status_checkBox.Location = new System.Drawing.Point(205, 31);
            this.network_status_checkBox.Name = "network_status_checkBox";
            this.network_status_checkBox.Size = new System.Drawing.Size(48, 16);
            this.network_status_checkBox.TabIndex = 3;
            this.network_status_checkBox.Text = "开放";
            this.network_status_checkBox.UseVisualStyleBackColor = true;
            this.network_status_checkBox.CheckedChanged += new System.EventHandler(this.network_status_checkBox_CheckedChanged);
            // 
            // site_info_data
            // 
            this.site_info_data.AllowUserToAddRows = false;
            this.site_info_data.AllowUserToDeleteRows = false;
            this.site_info_data.AllowUserToResizeColumns = false;
            this.site_info_data.AllowUserToResizeRows = false;
            this.site_info_data.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.site_info_data.BackgroundColor = System.Drawing.Color.White;
            this.site_info_data.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.site_info_data.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.phy_addr,
            this.site_type});
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.site_info_data.DefaultCellStyle = dataGridViewCellStyle3;
            this.site_info_data.EditMode = System.Windows.Forms.DataGridViewEditMode.EditOnKeystroke;
            this.site_info_data.Location = new System.Drawing.Point(32, 84);
            this.site_info_data.Name = "site_info_data";
            dataGridViewCellStyle4.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.site_info_data.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this.site_info_data.RowHeadersVisible = false;
            this.site_info_data.RowTemplate.Height = 23;
            this.site_info_data.Size = new System.Drawing.Size(377, 229);
            this.site_info_data.TabIndex = 6;
            // 
            // phy_addr
            // 
            this.phy_addr.FillWeight = 50F;
            this.phy_addr.HeaderText = "基站物理地址";
            this.phy_addr.Name = "phy_addr";
            // 
            // site_type
            // 
            this.site_type.DisplayStyleForCurrentCellOnly = true;
            this.site_type.FillWeight = 50F;
            this.site_type.HeaderText = "基站类型";
            this.site_type.Items.AddRange(new object[] {
            "M基站",
            "R基站",
            "C基站"});
            this.site_type.Name = "site_type";
            this.site_type.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.site_type.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // add
            // 
            this.add.Location = new System.Drawing.Point(415, 84);
            this.add.Name = "add";
            this.add.Size = new System.Drawing.Size(75, 23);
            this.add.TabIndex = 7;
            this.add.Text = "新增";
            this.add.UseVisualStyleBackColor = true;
            this.add.Click += new System.EventHandler(this.add_Click);
            // 
            // del
            // 
            this.del.Location = new System.Drawing.Point(415, 155);
            this.del.Name = "del";
            this.del.Size = new System.Drawing.Size(75, 23);
            this.del.TabIndex = 9;
            this.del.Text = "删除";
            this.del.UseVisualStyleBackColor = true;
            this.del.Click += new System.EventHandler(this.del_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(31, 381);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 21);
            this.textBox1.TabIndex = 10;
            // 
            // del_device
            // 
            this.del_device.Location = new System.Drawing.Point(159, 380);
            this.del_device.Name = "del_device";
            this.del_device.Size = new System.Drawing.Size(75, 23);
            this.del_device.TabIndex = 11;
            this.del_device.Text = "删除终端";
            this.del_device.UseVisualStyleBackColor = true;
            this.del_device.Click += new System.EventHandler(this.del_device_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 357);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 12;
            this.label1.Text = "物理地址";
            // 
            // rebuild
            // 
            this.rebuild.Location = new System.Drawing.Point(32, 27);
            this.rebuild.Name = "rebuild";
            this.rebuild.Size = new System.Drawing.Size(75, 23);
            this.rebuild.TabIndex = 13;
            this.rebuild.Text = "重新组网";
            this.rebuild.UseVisualStyleBackColor = true;
            this.rebuild.Click += new System.EventHandler(this.rebuild_Click);
            // 
            // ConfigForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(518, 451);
            this.Controls.Add(this.rebuild);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.del_device);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.del);
            this.Controls.Add(this.add);
            this.Controls.Add(this.site_info_data);
            this.Controls.Add(this.network_status_checkBox);
            this.Controls.Add(this.network_status);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ConfigForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "控制面板";
            ((System.ComponentModel.ISupportInitialize)(this.site_info_data)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label network_status;
        private System.Windows.Forms.CheckBox network_status_checkBox;
        private System.Windows.Forms.DataGridView site_info_data;
        private System.Windows.Forms.Button add;
        private System.Windows.Forms.Button del;
        private System.Windows.Forms.DataGridViewTextBoxColumn phy_addr;
        private System.Windows.Forms.DataGridViewComboBoxColumn site_type;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button del_device;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button rebuild;
    }
}