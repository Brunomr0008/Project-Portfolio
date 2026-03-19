using System;

namespace ComputerVision
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panelMain = new System.Windows.Forms.Panel();
            this.panelVideo = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.lblInstructions = new System.Windows.Forms.Label();
            this.pictureBoxVideo = new System.Windows.Forms.PictureBox();
            this.lblVideoTitle = new System.Windows.Forms.Label();
            this.panelControl = new System.Windows.Forms.Panel();
            this.grpResults = new System.Windows.Forms.GroupBox();
            this.panelResults = new System.Windows.Forms.Panel();
            this.btnReset = new System.Windows.Forms.Button();
            this.lblPos3Verde = new System.Windows.Forms.Label();
            this.lblPos3Azul = new System.Windows.Forms.Label();
            this.lblPos3Amarelo = new System.Windows.Forms.Label();
            this.lblPos3Title = new System.Windows.Forms.Label();
            this.lblPos2Verde = new System.Windows.Forms.Label();
            this.lblPos2Azul = new System.Windows.Forms.Label();
            this.lblPos2Amarelo = new System.Windows.Forms.Label();
            this.lblPos2Title = new System.Windows.Forms.Label();
            this.lblPos1Verde = new System.Windows.Forms.Label();
            this.lblPos1Azul = new System.Windows.Forms.Label();
            this.lblPos1Amarelo = new System.Windows.Forms.Label();
            this.lblPos1Title = new System.Windows.Forms.Label();
            this.grpCamera = new System.Windows.Forms.GroupBox();
            this.btnCapture = new System.Windows.Forms.Button();
            this.btnStopCamera = new System.Windows.Forms.Button();
            this.btnStartCamera = new System.Windows.Forms.Button();
            this.lblCameraStatus = new System.Windows.Forms.Label();
            this.btnDisconnectPLC = new System.Windows.Forms.Button();
            this.btnConnectPLC = new System.Windows.Forms.Button();
            this.lblPLCStatus = new System.Windows.Forms.Label();
            this.cmbProtocol = new System.Windows.Forms.ComboBox();
            this.lblProtocol = new System.Windows.Forms.Label();
            this.txtPLCPort = new System.Windows.Forms.TextBox();
            this.lblPLCPort = new System.Windows.Forms.Label();
            this.txtPLCIP = new System.Windows.Forms.TextBox();
            this.lblPLCIP = new System.Windows.Forms.Label();
            this.lblTitle = new System.Windows.Forms.Label();
            this.panelMain.SuspendLayout();
            this.panelVideo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxVideo)).BeginInit();
            this.panelControl.SuspendLayout();
            this.grpResults.SuspendLayout();
            this.panelResults.SuspendLayout();
            this.grpCamera.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelMain
            // 
            this.panelMain.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(62)))), ((int)(((byte)(80)))));
            this.panelMain.Controls.Add(this.panelVideo);
            this.panelMain.Controls.Add(this.panelControl);
            this.panelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMain.Location = new System.Drawing.Point(0, 74);
            this.panelMain.Margin = new System.Windows.Forms.Padding(4);
            this.panelMain.Name = "panelMain";
            this.panelMain.Size = new System.Drawing.Size(1541, 604);
            this.panelMain.TabIndex = 0;
            // 
            // panelVideo
            // 
            this.panelVideo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(73)))), ((int)(((byte)(94)))));
            this.panelVideo.Controls.Add(this.pictureBox1);
            this.panelVideo.Controls.Add(this.lblInstructions);
            this.panelVideo.Controls.Add(this.pictureBoxVideo);
            this.panelVideo.Controls.Add(this.lblVideoTitle);
            this.panelVideo.Location = new System.Drawing.Point(287, 0);
            this.panelVideo.Margin = new System.Windows.Forms.Padding(4);
            this.panelVideo.Name = "panelVideo";
            this.panelVideo.Padding = new System.Windows.Forms.Padding(13, 12, 13, 12);
            this.panelVideo.Size = new System.Drawing.Size(1255, 604);
            this.panelVideo.TabIndex = 1;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Black;
            this.pictureBox1.Location = new System.Drawing.Point(642, 61);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(596, 500);
            this.pictureBox1.TabIndex = 2;
            this.pictureBox1.TabStop = false;
            // 
            // lblInstructions
            // 
            this.lblInstructions.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.lblInstructions.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Italic);
            this.lblInstructions.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblInstructions.Location = new System.Drawing.Point(13, 561);
            this.lblInstructions.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblInstructions.Name = "lblInstructions";
            this.lblInstructions.Size = new System.Drawing.Size(1229, 31);
            this.lblInstructions.TabIndex = 0;
            this.lblInstructions.Text = "ℹ️ Use os botões à esquerda para controlar a câmara e o PLC.";
            this.lblInstructions.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pictureBoxVideo
            // 
            this.pictureBoxVideo.BackColor = System.Drawing.Color.Black;
            this.pictureBoxVideo.Location = new System.Drawing.Point(13, 61);
            this.pictureBoxVideo.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBoxVideo.Name = "pictureBoxVideo";
            this.pictureBoxVideo.Size = new System.Drawing.Size(600, 500);
            this.pictureBoxVideo.TabIndex = 0;
            this.pictureBoxVideo.TabStop = false;
            this.pictureBoxVideo.Click += new System.EventHandler(this.pictureBoxVideo_Click);
            // 
            // lblVideoTitle
            // 
            this.lblVideoTitle.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(73)))), ((int)(((byte)(94)))));
            this.lblVideoTitle.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblVideoTitle.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Bold);
            this.lblVideoTitle.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(236)))), ((int)(((byte)(240)))), ((int)(((byte)(241)))));
            this.lblVideoTitle.Location = new System.Drawing.Point(13, 12);
            this.lblVideoTitle.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblVideoTitle.Name = "lblVideoTitle";
            this.lblVideoTitle.Size = new System.Drawing.Size(1229, 45);
            this.lblVideoTitle.TabIndex = 1;
            this.lblVideoTitle.Text = "📹 Visualização da Câmara";
            this.lblVideoTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panelControl
            // 
            this.panelControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(73)))), ((int)(((byte)(94)))));
            this.panelControl.Controls.Add(this.grpResults);
            this.panelControl.Controls.Add(this.grpCamera);
            this.panelControl.Location = new System.Drawing.Point(0, 0);
            this.panelControl.Margin = new System.Windows.Forms.Padding(4);
            this.panelControl.Name = "panelControl";
            this.panelControl.Padding = new System.Windows.Forms.Padding(13, 12, 13, 12);
            this.panelControl.Size = new System.Drawing.Size(292, 604);
            this.panelControl.TabIndex = 0;
            this.panelControl.Paint += new System.Windows.Forms.PaintEventHandler(this.panelControl_Paint);
            // 
            // grpResults
            // 
            this.grpResults.Controls.Add(this.panelResults);
            this.grpResults.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpResults.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.grpResults.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(236)))), ((int)(((byte)(240)))), ((int)(((byte)(241)))));
            this.grpResults.Location = new System.Drawing.Point(13, 184);
            this.grpResults.Margin = new System.Windows.Forms.Padding(4);
            this.grpResults.Name = "grpResults";
            this.grpResults.Padding = new System.Windows.Forms.Padding(4);
            this.grpResults.Size = new System.Drawing.Size(266, 408);
            this.grpResults.TabIndex = 2;
            this.grpResults.TabStop = false;
            this.grpResults.Text = "📊 Resultados da Deteção";
            this.grpResults.Enter += new System.EventHandler(this.grpResults_Enter);
            // 
            // panelResults
            // 
            this.panelResults.AutoScroll = true;
            this.panelResults.Controls.Add(this.btnReset);
            this.panelResults.Controls.Add(this.lblPos3Verde);
            this.panelResults.Controls.Add(this.lblPos3Azul);
            this.panelResults.Controls.Add(this.lblPos3Amarelo);
            this.panelResults.Controls.Add(this.lblPos3Title);
            this.panelResults.Controls.Add(this.lblPos2Verde);
            this.panelResults.Controls.Add(this.lblPos2Azul);
            this.panelResults.Controls.Add(this.lblPos2Amarelo);
            this.panelResults.Controls.Add(this.lblPos2Title);
            this.panelResults.Controls.Add(this.lblPos1Verde);
            this.panelResults.Controls.Add(this.lblPos1Azul);
            this.panelResults.Controls.Add(this.lblPos1Amarelo);
            this.panelResults.Controls.Add(this.lblPos1Title);
            this.panelResults.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelResults.Location = new System.Drawing.Point(4, 27);
            this.panelResults.Margin = new System.Windows.Forms.Padding(4);
            this.panelResults.Name = "panelResults";
            this.panelResults.Size = new System.Drawing.Size(258, 377);
            this.panelResults.TabIndex = 0;
            this.panelResults.Paint += new System.Windows.Forms.PaintEventHandler(this.panelResults_Paint);
            // 
            // btnReset
            // 
            this.btnReset.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(155)))), ((int)(((byte)(89)))), ((int)(((byte)(182)))));
            this.btnReset.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnReset.FlatAppearance.BorderSize = 0;
            this.btnReset.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnReset.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.btnReset.ForeColor = System.Drawing.Color.White;
            this.btnReset.Location = new System.Drawing.Point(34, 206);
            this.btnReset.Margin = new System.Windows.Forms.Padding(4);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(208, 37);
            this.btnReset.TabIndex = 12;
            this.btnReset.Text = "🔄 Reset Deteções";
            this.btnReset.UseVisualStyleBackColor = false;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // lblPos3Verde
            // 
            this.lblPos3Verde.AutoSize = true;
            this.lblPos3Verde.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos3Verde.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos3Verde.Location = new System.Drawing.Point(31, 173);
            this.lblPos3Verde.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos3Verde.Name = "lblPos3Verde";
            this.lblPos3Verde.Size = new System.Drawing.Size(58, 16);
            this.lblPos3Verde.TabIndex = 11;
            this.lblPos3Verde.Text = "VER: ❌";
            // 
            // lblPos3Azul
            // 
            this.lblPos3Azul.AutoSize = true;
            this.lblPos3Azul.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos3Azul.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos3Azul.Location = new System.Drawing.Point(186, 173);
            this.lblPos3Azul.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos3Azul.Name = "lblPos3Azul";
            this.lblPos3Azul.Size = new System.Drawing.Size(56, 16);
            this.lblPos3Azul.TabIndex = 10;
            this.lblPos3Azul.Text = "AZU: ❌";
            // 
            // lblPos3Amarelo
            // 
            this.lblPos3Amarelo.AutoSize = true;
            this.lblPos3Amarelo.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos3Amarelo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos3Amarelo.Location = new System.Drawing.Point(108, 173);
            this.lblPos3Amarelo.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos3Amarelo.Name = "lblPos3Amarelo";
            this.lblPos3Amarelo.Size = new System.Drawing.Size(59, 16);
            this.lblPos3Amarelo.TabIndex = 9;
            this.lblPos3Amarelo.Text = "AME: ❌";
            // 
            // lblPos3Title
            // 
            this.lblPos3Title.AutoSize = true;
            this.lblPos3Title.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold);
            this.lblPos3Title.Location = new System.Drawing.Point(31, 140);
            this.lblPos3Title.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos3Title.Name = "lblPos3Title";
            this.lblPos3Title.Size = new System.Drawing.Size(52, 18);
            this.lblPos3Title.TabIndex = 8;
            this.lblPos3Title.Text = "POS3:";
            // 
            // lblPos2Verde
            // 
            this.lblPos2Verde.AutoSize = true;
            this.lblPos2Verde.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos2Verde.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos2Verde.Location = new System.Drawing.Point(31, 110);
            this.lblPos2Verde.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos2Verde.Name = "lblPos2Verde";
            this.lblPos2Verde.Size = new System.Drawing.Size(58, 16);
            this.lblPos2Verde.TabIndex = 7;
            this.lblPos2Verde.Text = "VER: ❌";
            // 
            // lblPos2Azul
            // 
            this.lblPos2Azul.AutoSize = true;
            this.lblPos2Azul.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos2Azul.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos2Azul.Location = new System.Drawing.Point(186, 110);
            this.lblPos2Azul.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos2Azul.Name = "lblPos2Azul";
            this.lblPos2Azul.Size = new System.Drawing.Size(56, 16);
            this.lblPos2Azul.TabIndex = 6;
            this.lblPos2Azul.Text = "AZU: ❌";
            // 
            // lblPos2Amarelo
            // 
            this.lblPos2Amarelo.AutoSize = true;
            this.lblPos2Amarelo.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos2Amarelo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos2Amarelo.Location = new System.Drawing.Point(108, 110);
            this.lblPos2Amarelo.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos2Amarelo.Name = "lblPos2Amarelo";
            this.lblPos2Amarelo.Size = new System.Drawing.Size(59, 16);
            this.lblPos2Amarelo.TabIndex = 5;
            this.lblPos2Amarelo.Text = "AME: ❌";
            this.lblPos2Amarelo.Click += new System.EventHandler(this.lblPos2Amarelo_Click);
            // 
            // lblPos2Title
            // 
            this.lblPos2Title.AutoSize = true;
            this.lblPos2Title.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold);
            this.lblPos2Title.Location = new System.Drawing.Point(31, 82);
            this.lblPos2Title.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos2Title.Name = "lblPos2Title";
            this.lblPos2Title.Size = new System.Drawing.Size(52, 18);
            this.lblPos2Title.TabIndex = 4;
            this.lblPos2Title.Text = "POS2:";
            // 
            // lblPos1Verde
            // 
            this.lblPos1Verde.AutoSize = true;
            this.lblPos1Verde.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos1Verde.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos1Verde.Location = new System.Drawing.Point(31, 47);
            this.lblPos1Verde.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos1Verde.Name = "lblPos1Verde";
            this.lblPos1Verde.Size = new System.Drawing.Size(58, 16);
            this.lblPos1Verde.TabIndex = 3;
            this.lblPos1Verde.Text = "VER: ❌";
            // 
            // lblPos1Azul
            // 
            this.lblPos1Azul.AutoSize = true;
            this.lblPos1Azul.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos1Azul.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos1Azul.Location = new System.Drawing.Point(186, 47);
            this.lblPos1Azul.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos1Azul.Name = "lblPos1Azul";
            this.lblPos1Azul.Size = new System.Drawing.Size(56, 16);
            this.lblPos1Azul.TabIndex = 2;
            this.lblPos1Azul.Text = "AZU: ❌";
            this.lblPos1Azul.Click += new System.EventHandler(this.lblPos1Azul_Click);
            // 
            // lblPos1Amarelo
            // 
            this.lblPos1Amarelo.AutoSize = true;
            this.lblPos1Amarelo.Font = new System.Drawing.Font("Arial", 8F);
            this.lblPos1Amarelo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(189)))), ((int)(((byte)(195)))), ((int)(((byte)(199)))));
            this.lblPos1Amarelo.Location = new System.Drawing.Point(108, 47);
            this.lblPos1Amarelo.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos1Amarelo.Name = "lblPos1Amarelo";
            this.lblPos1Amarelo.Size = new System.Drawing.Size(59, 16);
            this.lblPos1Amarelo.TabIndex = 1;
            this.lblPos1Amarelo.Text = "AME: ❌";
            this.lblPos1Amarelo.Click += new System.EventHandler(this.lblPos1Amarelo_Click);
            // 
            // lblPos1Title
            // 
            this.lblPos1Title.AutoSize = true;
            this.lblPos1Title.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold);
            this.lblPos1Title.Location = new System.Drawing.Point(31, 17);
            this.lblPos1Title.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblPos1Title.Name = "lblPos1Title";
            this.lblPos1Title.Size = new System.Drawing.Size(52, 18);
            this.lblPos1Title.TabIndex = 0;
            this.lblPos1Title.Text = "POS1:";
            // 
            // grpCamera
            // 
            this.grpCamera.Controls.Add(this.btnCapture);
            this.grpCamera.Controls.Add(this.btnStopCamera);
            this.grpCamera.Controls.Add(this.btnStartCamera);
            this.grpCamera.Controls.Add(this.lblCameraStatus);
            this.grpCamera.Dock = System.Windows.Forms.DockStyle.Top;
            this.grpCamera.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.grpCamera.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(236)))), ((int)(((byte)(240)))), ((int)(((byte)(241)))));
            this.grpCamera.Location = new System.Drawing.Point(13, 12);
            this.grpCamera.Margin = new System.Windows.Forms.Padding(4);
            this.grpCamera.Name = "grpCamera";
            this.grpCamera.Padding = new System.Windows.Forms.Padding(4);
            this.grpCamera.Size = new System.Drawing.Size(266, 172);
            this.grpCamera.TabIndex = 1;
            this.grpCamera.TabStop = false;
            this.grpCamera.Text = "📷 Controlo da Câmara";
            this.grpCamera.Enter += new System.EventHandler(this.grpCamera_Enter);
            // 
            // btnCapture
            // 
            this.btnCapture.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(243)))), ((int)(((byte)(156)))), ((int)(((byte)(18)))));
            this.btnCapture.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCapture.Enabled = false;
            this.btnCapture.FlatAppearance.BorderSize = 0;
            this.btnCapture.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCapture.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.btnCapture.ForeColor = System.Drawing.Color.White;
            this.btnCapture.Location = new System.Drawing.Point(20, 132);
            this.btnCapture.Margin = new System.Windows.Forms.Padding(4);
            this.btnCapture.Name = "btnCapture";
            this.btnCapture.Size = new System.Drawing.Size(226, 31);
            this.btnCapture.TabIndex = 3;
            this.btnCapture.Text = "📸 Capturar e Detetar";
            this.btnCapture.UseVisualStyleBackColor = false;
            this.btnCapture.Click += new System.EventHandler(this.btnCapture_Click);
            // 
            // btnStopCamera
            // 
            this.btnStopCamera.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(231)))), ((int)(((byte)(76)))), ((int)(((byte)(60)))));
            this.btnStopCamera.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnStopCamera.Enabled = false;
            this.btnStopCamera.FlatAppearance.BorderSize = 0;
            this.btnStopCamera.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnStopCamera.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.btnStopCamera.ForeColor = System.Drawing.Color.White;
            this.btnStopCamera.Location = new System.Drawing.Point(20, 96);
            this.btnStopCamera.Margin = new System.Windows.Forms.Padding(4);
            this.btnStopCamera.Name = "btnStopCamera";
            this.btnStopCamera.Size = new System.Drawing.Size(226, 31);
            this.btnStopCamera.TabIndex = 2;
            this.btnStopCamera.Text = "Desligar Câmara";
            this.btnStopCamera.UseVisualStyleBackColor = false;
            this.btnStopCamera.Click += new System.EventHandler(this.btnStopCamera_Click);
            // 
            // btnStartCamera
            // 
            this.btnStartCamera.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(152)))), ((int)(((byte)(219)))));
            this.btnStartCamera.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnStartCamera.FlatAppearance.BorderSize = 0;
            this.btnStartCamera.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnStartCamera.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.btnStartCamera.ForeColor = System.Drawing.Color.White;
            this.btnStartCamera.Location = new System.Drawing.Point(20, 59);
            this.btnStartCamera.Margin = new System.Windows.Forms.Padding(4);
            this.btnStartCamera.Name = "btnStartCamera";
            this.btnStartCamera.Size = new System.Drawing.Size(226, 31);
            this.btnStartCamera.TabIndex = 1;
            this.btnStartCamera.Text = "Ligar Câmara";
            this.btnStartCamera.UseVisualStyleBackColor = false;
            this.btnStartCamera.Click += new System.EventHandler(this.btnStartCamera_Click);
            // 
            // lblCameraStatus
            // 
            this.lblCameraStatus.AutoSize = true;
            this.lblCameraStatus.Font = new System.Drawing.Font("Arial", 10F);
            this.lblCameraStatus.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(231)))), ((int)(((byte)(76)))), ((int)(((byte)(60)))));
            this.lblCameraStatus.Location = new System.Drawing.Point(20, 31);
            this.lblCameraStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblCameraStatus.Name = "lblCameraStatus";
            this.lblCameraStatus.Size = new System.Drawing.Size(136, 19);
            this.lblCameraStatus.TabIndex = 0;
            this.lblCameraStatus.Text = "Status: Desligada";
            // 
            // btnDisconnectPLC
            // 
            this.btnDisconnectPLC.Location = new System.Drawing.Point(0, 0);
            this.btnDisconnectPLC.Name = "btnDisconnectPLC";
            this.btnDisconnectPLC.Size = new System.Drawing.Size(75, 23);
            this.btnDisconnectPLC.TabIndex = 0;
            // 
            // btnConnectPLC
            // 
            this.btnConnectPLC.Location = new System.Drawing.Point(0, 0);
            this.btnConnectPLC.Name = "btnConnectPLC";
            this.btnConnectPLC.Size = new System.Drawing.Size(75, 23);
            this.btnConnectPLC.TabIndex = 0;
            // 
            // lblPLCStatus
            // 
            this.lblPLCStatus.Location = new System.Drawing.Point(0, 0);
            this.lblPLCStatus.Name = "lblPLCStatus";
            this.lblPLCStatus.Size = new System.Drawing.Size(100, 23);
            this.lblPLCStatus.TabIndex = 0;
            // 
            // cmbProtocol
            // 
            this.cmbProtocol.Location = new System.Drawing.Point(0, 0);
            this.cmbProtocol.Name = "cmbProtocol";
            this.cmbProtocol.Size = new System.Drawing.Size(121, 24);
            this.cmbProtocol.TabIndex = 0;
            // 
            // lblProtocol
            // 
            this.lblProtocol.Location = new System.Drawing.Point(0, 0);
            this.lblProtocol.Name = "lblProtocol";
            this.lblProtocol.Size = new System.Drawing.Size(100, 23);
            this.lblProtocol.TabIndex = 0;
            // 
            // txtPLCPort
            // 
            this.txtPLCPort.Location = new System.Drawing.Point(0, 0);
            this.txtPLCPort.Name = "txtPLCPort";
            this.txtPLCPort.Size = new System.Drawing.Size(100, 22);
            this.txtPLCPort.TabIndex = 0;
            // 
            // lblPLCPort
            // 
            this.lblPLCPort.Location = new System.Drawing.Point(0, 0);
            this.lblPLCPort.Name = "lblPLCPort";
            this.lblPLCPort.Size = new System.Drawing.Size(100, 23);
            this.lblPLCPort.TabIndex = 0;
            // 
            // txtPLCIP
            // 
            this.txtPLCIP.Location = new System.Drawing.Point(0, 0);
            this.txtPLCIP.Name = "txtPLCIP";
            this.txtPLCIP.Size = new System.Drawing.Size(100, 22);
            this.txtPLCIP.TabIndex = 0;
            // 
            // lblPLCIP
            // 
            this.lblPLCIP.Location = new System.Drawing.Point(0, 0);
            this.lblPLCIP.Name = "lblPLCIP";
            this.lblPLCIP.Size = new System.Drawing.Size(100, 23);
            this.lblPLCIP.TabIndex = 0;
            // 
            // lblTitle
            // 
            this.lblTitle.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(62)))), ((int)(((byte)(80)))));
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblTitle.Font = new System.Drawing.Font("Arial", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(236)))), ((int)(((byte)(240)))), ((int)(((byte)(241)))));
            this.lblTitle.Location = new System.Drawing.Point(0, 0);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(1541, 74);
            this.lblTitle.TabIndex = 1;
            this.lblTitle.Text = "🔍 Sistema de Visão por Computador";
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTitle.Click += new System.EventHandler(this.lblTitle_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(62)))), ((int)(((byte)(80)))));
            this.ClientSize = new System.Drawing.Size(1541, 678);
            this.Controls.Add(this.panelMain);
            this.Controls.Add(this.lblTitle);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AUTOFOX - Separador de Lixo";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.panelMain.ResumeLayout(false);
            this.panelVideo.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxVideo)).EndInit();
            this.panelControl.ResumeLayout(false);
            this.grpResults.ResumeLayout(false);
            this.panelResults.ResumeLayout(false);
            this.panelResults.PerformLayout();
            this.grpCamera.ResumeLayout(false);
            this.grpCamera.PerformLayout();
            this.ResumeLayout(false);

        }

        private void SuspendLayout()
        {
            throw new NotImplementedException();
        }

        #endregion

        private System.Windows.Forms.Panel panelMain;
        private System.Windows.Forms.Panel panelControl;
        private System.Windows.Forms.Panel panelVideo;
        private System.Windows.Forms.Label lblTitle;
        private System.Windows.Forms.Label lblPLCIP;
        private System.Windows.Forms.TextBox txtPLCIP;
        private System.Windows.Forms.Label lblPLCPort;
        private System.Windows.Forms.TextBox txtPLCPort;
        private System.Windows.Forms.Label lblProtocol;
        private System.Windows.Forms.ComboBox cmbProtocol;
        private System.Windows.Forms.Label lblPLCStatus;
        private System.Windows.Forms.Button btnConnectPLC;
        private System.Windows.Forms.Button btnDisconnectPLC;

        private System.Windows.Forms.GroupBox grpCamera;
        private System.Windows.Forms.Label lblCameraStatus;
        private System.Windows.Forms.Button btnStartCamera;
        private System.Windows.Forms.Button btnStopCamera;
        private System.Windows.Forms.Button btnCapture;

        private System.Windows.Forms.GroupBox grpResults;
        private System.Windows.Forms.Panel panelResults;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Label lblPos1Title;
        private System.Windows.Forms.Label lblPos1Amarelo;
        private System.Windows.Forms.Label lblPos1Azul;
        private System.Windows.Forms.Label lblPos1Verde;
        private System.Windows.Forms.Label lblPos2Title;
        private System.Windows.Forms.Label lblPos2Amarelo;
        private System.Windows.Forms.Label lblPos2Azul;
        private System.Windows.Forms.Label lblPos2Verde;
        private System.Windows.Forms.Label lblPos3Title;
        private System.Windows.Forms.Label lblPos3Amarelo;
        private System.Windows.Forms.Label lblPos3Azul;
        private System.Windows.Forms.Label lblPos3Verde;

        private System.Windows.Forms.Label lblVideoTitle;
        private System.Windows.Forms.PictureBox pictureBoxVideo;
        private System.Windows.Forms.Label lblInstructions;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

