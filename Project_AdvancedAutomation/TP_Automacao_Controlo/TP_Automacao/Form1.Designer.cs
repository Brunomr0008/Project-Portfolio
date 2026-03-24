namespace TP_Automacao
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Main Window

        private void InitializeComponent()
        {
            this.lblTitulo = new System.Windows.Forms.Label();
            this.btnSelecaoCores = new System.Windows.Forms.Button();
            this.btnControleManual = new System.Windows.Forms.Button();
            this.lblVersao = new System.Windows.Forms.Label();
            this.pictureBoxTopo = new System.Windows.Forms.PictureBox();
            this.lblGrupo = new System.Windows.Forms.Label();
            this.listBoxLogs = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTopo)).BeginInit();
            this.SuspendLayout();
            // 
            // lblTitulo
            // 
            this.lblTitulo.BackColor = System.Drawing.Color.Transparent;
            this.lblTitulo.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitulo.ForeColor = System.Drawing.Color.White;
            this.lblTitulo.Location = new System.Drawing.Point(176, 193);
            this.lblTitulo.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblTitulo.Name = "lblTitulo";
            this.lblTitulo.Size = new System.Drawing.Size(526, 49);
            this.lblTitulo.TabIndex = 0;
            this.lblTitulo.Text = "🏭 Sistema de Controlo do PLC";
            this.lblTitulo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnSelecaoCores
            // 
            this.btnSelecaoCores.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(165)))), ((int)(((byte)(0)))));
            this.btnSelecaoCores.FlatAppearance.BorderSize = 0;
            this.btnSelecaoCores.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSelecaoCores.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSelecaoCores.ForeColor = System.Drawing.Color.White;
            this.btnSelecaoCores.Location = new System.Drawing.Point(67, 268);
            this.btnSelecaoCores.Margin = new System.Windows.Forms.Padding(4);
            this.btnSelecaoCores.Name = "btnSelecaoCores";
            this.btnSelecaoCores.Size = new System.Drawing.Size(270, 82);
            this.btnSelecaoCores.TabIndex = 1;
            this.btnSelecaoCores.Text = "⚫ Seleção de Cores 🔴";
            this.btnSelecaoCores.UseVisualStyleBackColor = false;
            this.btnSelecaoCores.Click += new System.EventHandler(this.btnSelecaoCores_Click);
            // 
            // btnControleManual
            // 
            this.btnControleManual.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(130)))), ((int)(((byte)(180)))));
            this.btnControleManual.FlatAppearance.BorderSize = 0;
            this.btnControleManual.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnControleManual.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnControleManual.ForeColor = System.Drawing.Color.White;
            this.btnControleManual.Location = new System.Drawing.Point(67, 377);
            this.btnControleManual.Margin = new System.Windows.Forms.Padding(4);
            this.btnControleManual.Name = "btnControleManual";
            this.btnControleManual.Size = new System.Drawing.Size(270, 82);
            this.btnControleManual.TabIndex = 2;
            this.btnControleManual.Text = "⚙️ Controlo Manual\nde Saídas";
            this.btnControleManual.UseVisualStyleBackColor = false;
            this.btnControleManual.Click += new System.EventHandler(this.btnControleManual_Click);
            // 
            // lblVersao
            // 
            this.lblVersao.BackColor = System.Drawing.Color.Transparent;
            this.lblVersao.Font = new System.Drawing.Font("Segoe UI", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVersao.ForeColor = System.Drawing.Color.LightGray;
            this.lblVersao.Location = new System.Drawing.Point(55, 467);
            this.lblVersao.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblVersao.Name = "lblVersao";
            this.lblVersao.Size = new System.Drawing.Size(709, 59);
            this.lblVersao.TabIndex = 4;
            this.lblVersao.Text = "🔧 Sistema Festo -> Automação / Redes";
            this.lblVersao.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // pictureBoxTopo
            // 
            this.pictureBoxTopo.BackColor = System.Drawing.Color.White;
            this.pictureBoxTopo.Location = new System.Drawing.Point(336, 5);
            this.pictureBoxTopo.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBoxTopo.Name = "pictureBoxTopo";
            this.pictureBoxTopo.Size = new System.Drawing.Size(190, 170);
            this.pictureBoxTopo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxTopo.TabIndex = 5;
            this.pictureBoxTopo.TabStop = false;
            this.pictureBoxTopo.Click += new System.EventHandler(this.pictureBoxTopo_Click);
            // 
            // lblGrupo
            // 
            this.lblGrupo.BackColor = System.Drawing.Color.Transparent;
            this.lblGrupo.Font = new System.Drawing.Font("Segoe UI", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGrupo.ForeColor = System.Drawing.Color.LightGray;
            this.lblGrupo.Location = new System.Drawing.Point(105, 538);
            this.lblGrupo.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblGrupo.Name = "lblGrupo";
            this.lblGrupo.Size = new System.Drawing.Size(675, 30);
            this.lblGrupo.TabIndex = 6;
            this.lblGrupo.Text = "👥 Bruno Rodrigues, 31015 👥 Pedro Rego, 14905 👥 Tiago Miranda, 30843    === Gru" + "po C ===";
            this.lblGrupo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // listBoxLogs
            // 
            this.listBoxLogs.BackColor = System.Drawing.Color.Black;
            this.listBoxLogs.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listBoxLogs.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listBoxLogs.ForeColor = System.Drawing.Color.Lime;
            this.listBoxLogs.FormattingEnabled = true;
            this.listBoxLogs.ItemHeight = 18;
            this.listBoxLogs.Location = new System.Drawing.Point(367, 267);
            this.listBoxLogs.Name = "listBoxLogs";
            this.listBoxLogs.Size = new System.Drawing.Size(469, 200);
            this.listBoxLogs.TabIndex = 7;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(9)))), ((int)(((byte)(35)))), ((int)(((byte)(50)))));
            this.ClientSize = new System.Drawing.Size(912, 567);
            this.Controls.Add(this.listBoxLogs);
            this.Controls.Add(this.lblGrupo);
            this.Controls.Add(this.pictureBoxTopo);
            this.Controls.Add(this.lblVersao);
            this.Controls.Add(this.btnControleManual);
            this.Controls.Add(this.btnSelecaoCores);
            this.Controls.Add(this.lblTitulo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AUTOFOX - Sistema de Controlo PLC - Automação Avançada & Redes Ind. de Comunicaçã" + "o";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTopo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblTitulo;
        private System.Windows.Forms.Button btnSelecaoCores;
        private System.Windows.Forms.Button btnControleManual;
        private System.Windows.Forms.Label lblVersao;
        private System.Windows.Forms.PictureBox pictureBoxTopo;
        private System.Windows.Forms.Label lblGrupo;
        private System.Windows.Forms.ListBox listBoxLogs; // Declaração do ListBox
    }
}