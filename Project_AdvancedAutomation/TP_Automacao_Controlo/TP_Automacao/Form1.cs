using System;
using System.Drawing;
using System.Windows.Forms;
using libplctag;
using System.Threading;

namespace TP_Automacao
{
    public partial class Form1 : Form
    {
        Tag[] tag_read_cor;
        Thread read_cor;

        Tag[] Emergency;
        Thread Emergency_thread;

        public Form1()
        {
            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterScreen;
            this.BackColor = Color.FromArgb(9, 35, 50); // #092332

            // Configurar a imagem no topo
            ConfigurarImagemTopo();

            // Configurar ícone da janela
            ConfigurarIconeJanela();

            // Inicializar o ecrã de logs
            // Adicionar algumas mensagens de exemplo ao iniciar
            AddLogMessage("Sistema de Controlo PLC iniciado.");
            AddLogMessage("A aguardar comunicação com o PLC...");

            TagPLC_posicao();
            TagPLC_emergency();

            read_cor = new Thread(new ThreadStart(read_cor_function));
            read_cor.Start();

            Emergency_thread = new Thread(new ThreadStart(emergency_function));
            Emergency_thread.Start();
        }

        private void ConfigurarImagemTopo()
        {
            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    pictureBoxTopo.Image = Image.FromFile(imagePath);
                }
                else
                {
                    Bitmap placeholder = new Bitmap(150, 100);
                    using (Graphics g = Graphics.FromImage(placeholder))
                    {
                        g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                        g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                    }
                    pictureBoxTopo.Image = placeholder;
                }
            }
            catch
            {
                Bitmap placeholder = new Bitmap(150, 100);
                using (Graphics g = Graphics.FromImage(placeholder))
                {
                    g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                    g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                }
                pictureBoxTopo.Image = placeholder;
            }
        }

        private void ConfigurarIconeJanela()
        {
            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    using (Bitmap bitmap = new Bitmap(imagePath))
                    {
                        Bitmap iconBitmap = new Bitmap(bitmap, new Size(32, 32));
                        IntPtr hIcon = iconBitmap.GetHicon();
                        this.Icon = Icon.FromHandle(hIcon);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro ao carregar ícone: {ex.Message}");
            }
        }

        private void btnSelecaoCores_Click(object sender, EventArgs e)
        {
            FormSelecaoCores form = new FormSelecaoCores();
            form.ShowDialog();
        }

        private void btnControleManual_Click(object sender, EventArgs e)
        {
            FormControleManual form = new FormControleManual();
            form.ShowDialog();
        }

        private void pictureBoxTopo_Click(object sender, EventArgs e) { }

        /// <summary>
        /// Adiciona uma mensagem ao ecrã de logs.
        /// </summary>
        /// <param name="message">A mensagem a ser exibida.</param>
        public void AddLogMessage(string message)
        {
            // Para garantir que a atualização da UI ocorre na thread correta
            if (this.InvokeRequired)
            {
                this.Invoke(new Action<string>(AddLogMessage), message);
            }
            else
            {
                string timestamp = DateTime.Now.ToString("HH:mm:ss");
                listBoxLogs.Items.Insert(0, $"[{timestamp}] {message}"); // Adiciona a mensagem no topo
                if (listBoxLogs.Items.Count > 100) // Limita o número de logs para evitar sobrecarga
                {
                    listBoxLogs.Items.RemoveAt(listBoxLogs.Items.Count - 1);
                }
            }
        }
        private void read_cor_function()
        {
            string[] message_cor = { "Posição 1 Ocupada", "Posição 2 Ocupada", "Posição 3 Ocupada", "Posição 4 Ocupada" };
            string[] message_cor_clear = { "Posição 1 Livre", "Posição 2 Livre", "Posição 3 Livre", "Posição 4 Livre" };

            bool[] previousBit = { false, false, false, false }; // inicialização

            while (true)
            {
                for (int i = 0; i < 4; i++)
                {
                    tag_read_cor[i].Read(); // Atualiza o estado
                    bool currentBit = tag_read_cor[i].GetBit(0);

                    // Detecta transição de 0 -> 1
                    if (!previousBit[i] && currentBit)
                    {
                        AddLogMessage(message_cor[i]); // mensagem de ativação
                    }

                    // Detecta transição de 1 -> 0
                    if (previousBit[i] && !currentBit)
                    {
                        AddLogMessage(message_cor_clear[i]); // mensagem de desativação
                    }

                    // Atualiza estado anterior
                    previousBit[i] = currentBit;
                }

                Thread.Sleep(100);
            }
        }
        private void TagPLC_posicao()
        {
            tag_read_cor = new Tag[4]
            {
                new Tag { Name = "espaco_posicao_1", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "espaco_posicao_2", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "espaco_posicao_3", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "espaco_posicao_4", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron }
            };

            tag_read_cor[0].Initialize(); // Inicializa a conexão
            tag_read_cor[1].Initialize(); // Inicializa a conexão
            tag_read_cor[2].Initialize(); // Inicializa a conexão
            tag_read_cor[3].Initialize(); // Inicializa a conexão
        }
        private void emergency_function()
        {
            string[] message_emergency = { "⚠EMERGENCIA Cilindro Recuo", "⚠EMERGENCIA Ciclindro Avanço", "⚠EMERGENCIA vacuo pneumatico movimento para cima", "⚠EMERGENCIA Vacuo pneumatico movimento para baixo",
                "⚠EMERGENCIA Vacuo pneumatico movimento para tras", "⚠EMERGENCIA Vacuo pneumatico movimento para frente", "⚠EMERGENCIA peça vacuo não detetada"};
            bool[] previousBit = { false, false, false, false, false, false, false }; // inicialização

            while (true)
            {
                for (int i = 0; i < 7; i++)
                {
                    Emergency[i].Read(); // Atualiza o estado
                    bool currentBit = Emergency[i].GetBit(0);

                    // Detecta transição de 0 -> 1
                    if (!previousBit[i] && currentBit)
                    {
                        AddLogMessage(message_emergency[i]); // mensagem de ativação
                    }

                    // Atualiza estado anterior
                    previousBit[i] = currentBit;
                }

                Thread.Sleep(100);
            }
        }
        private void TagPLC_emergency()
        {
            Emergency = new Tag[7]
            {
                new Tag { Name = "Emergencia_Cr", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_Ca", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_Vpmpc", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_Vpmpb", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_Vpmpt", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_Vppf", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Emergencia_pvnd", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron }
            };

            for(int i = 0; i < 7; i++)
            {
                Emergency[i].Initialize(); // Inicializa a conexão
            }
        }
    }

    // ===================== //
    // Form Seleção de Cores //
    // ===================== //
    public partial class FormSelecaoCores : Form
    {
        Tag[] tag_cor;

        private Button[] quadrados = new Button[4];
        private Label[] labels = new Label[4];
        private bool[] estados = new bool[4]; // false = preto, true = vermelho
        private PictureBox pictureBox;
      
        public FormSelecaoCores()
        {
            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterScreen;
            ConfigurarIconeJanela();
        }

        private void ConfigurarIconeJanela()
        {
            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    using (Bitmap bitmap = new Bitmap(imagePath))
                    {
                        Bitmap iconBitmap = new Bitmap(bitmap, new Size(32, 32));
                        IntPtr hIcon = iconBitmap.GetHicon();
                        this.Icon = Icon.FromHandle(hIcon);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro ao carregar ícone: {ex.Message}");
            }
        }

        private void InitializeComponent()
        {
            this.Text = "Controlo PLC - Seleção de Cores";
            this.Size = new Size(500, 600);
            this.BackColor = Color.FromArgb(9, 35, 50); // #092332
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;

            TagPLC_write_cor();
            

            // Imagem no topo
            pictureBox = new PictureBox
            {
                Location = new Point(175, 10), // Centralizado no topo
                Size = new Size(130, 110),
                SizeMode = PictureBoxSizeMode.StretchImage,
                BorderStyle = BorderStyle.None,
                BackColor = Color.White
            };

            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    pictureBox.Image = Image.FromFile(imagePath);
                }
                else
                {
                    Bitmap placeholder = new Bitmap(150, 100);
                    using (Graphics g = Graphics.FromImage(placeholder))
                    {
                        g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                        g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                    }
                    pictureBox.Image = placeholder;
                }
            }
            catch
            {
                Bitmap placeholder = new Bitmap(150, 100);
                using (Graphics g = Graphics.FromImage(placeholder))
                {
                    g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                    g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                }
                pictureBox.Image = placeholder;
            }

            // Título
            Label lblTitulo = new Label
            {
                Text = "Seleção de Cores - Clique nos quadrados para alterar cor",
                Font = new Font("Segoe UI", 11, FontStyle.Bold),
                Location = new Point(50, 130),
                Size = new Size(400, 25),
                TextAlign = ContentAlignment.MiddleCenter,
                ForeColor = Color.White,
                BackColor = Color.Transparent
            };

            for (int i = 0; i < 4; i++)
            {
                int row = i / 2;
                int col = i % 2;

                quadrados[i] = new Button
                {
                    Size = new Size(80, 80),
                    Location = new Point(150 + col * 100, 160 + row * 100),
                    BackColor = Color.Black,
                    FlatStyle = FlatStyle.Flat,
                    Tag = i
                };
                quadrados[i].FlatAppearance.BorderSize = 2;
                quadrados[i].FlatAppearance.BorderColor = Color.Gray;
                quadrados[i].Click += QuadradoClick;

                labels[i] = new Label
                {
                    Text = $"Q{i + 1}: Preto",
                    Font = new Font("Segoe UI", 10, FontStyle.Bold),
                    Location = new Point(130 + col * 120, 350 + row * 30),
                    Size = new Size(100, 20),
                    TextAlign = ContentAlignment.MiddleCenter,
                    ForeColor = Color.White,
                    BackColor = Color.Transparent
                };

                estados[i] = false;
            }

            // Botão Executar
            Button btnExecutar = new Button
            {
                Text = "Executar - Enviar para PLC",
                Font = new Font("Segoe UI", 11, FontStyle.Bold),
                Location = new Point(140, 430),
                Size = new Size(200, 40),
                BackColor = Color.FromArgb(255, 165, 0),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };
            btnExecutar.FlatAppearance.BorderSize = 0;
            btnExecutar.Click += BtnExecutar_Click;

            // Botão Voltar
            Button btnVoltar = new Button
            {
                Text = "Voltar",
                Font = new Font("Segoe UI", 10),
                Location = new Point(50, 510),
                Size = new Size(80, 30),
                BackColor = Color.Gray,
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };
            btnVoltar.FlatAppearance.BorderSize = 0;
            btnVoltar.Click += (s, e) => this.Close();

            // Adicionar controles ao form
            this.Controls.Add(pictureBox);
            this.Controls.Add(lblTitulo);
            this.Controls.Add(btnExecutar);
            this.Controls.Add(btnVoltar);

            for (int i = 0; i < 4; i++)
            {
                this.Controls.Add(quadrados[i]);
                this.Controls.Add(labels[i]);
            }
        }

        private void QuadradoClick(object sender, EventArgs e)
        {
            Button quadrado = (Button)sender;
            int index = (int)quadrado.Tag;

            estados[index] = !estados[index];

            if (estados[index])
            {
                quadrado.BackColor = Color.Red;
                labels[index].Text = $"Q{index + 1}: Vermelho";
                labels[index].ForeColor = Color.Red;
            }
            else
            {
                quadrado.BackColor = Color.Black;
                labels[index].Text = $"Q{index + 1}: Preto";
                labels[index].ForeColor = Color.White;
            }
        }

        private void BtnExecutar_Click(object sender, EventArgs e)
        {
            string comando = "Estados enviados para PLC:\n";

            for (int i = 0; i < 4; i++)
            {
                comando += $"Q{i + 1}: {(estados[i] ? "Vermelho" : "Preto")}\n";
                tag_cor[i].SetBit(0, estados[i]);
                tag_cor[i].Write();
            }

            MessageBox.Show(comando, "Comando Executado", MessageBoxButtons.OK, MessageBoxIcon.Information);

            // Exemplo de como você enviaria um log para o Form1
            // Você precisaria de uma referência à instância de Form1, por exemplo, passando-a no construtor
            // ou usando um evento/delegado. Para simplificar, vou assumir que você terá essa referência.
            // ((Form1)this.Owner).AddLogMessage($"Comando de seleção de cores executado.");
        }

        private void TagPLC_write_cor()
        {
            tag_cor = new Tag[4]
            {
                new Tag { Name = "cor_desejada_1", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "cor_desejada_2", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "cor_desejada_3", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "cor_desejada_4", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron }
            };

            tag_cor[0].Initialize(); // Inicializa a conexão
            tag_cor[1].Initialize(); // Inicializa a conexão
            tag_cor[2].Initialize(); // Inicializa a conexão
            tag_cor[3].Initialize(); // Inicializa a conexão
        }
    }


    // ==================== //
    // Form Controlo Manual //
    // ==================== //
    public partial class FormControleManual : Form
    {
        Tag[] tag_Manual;

        private Button[] botoesSaidas;
        private PictureBox pictureBoxTopo;
        private string[] nomesSaidas = {
            "Cilindro_Base", "Cilindro_Tampa_Front","Cilindro_Tampa_Back", "Cilindro_Tampa_Down", "Vácuo", "Travão_Montagem",
            "Tapete", "Start", "Stop", "Reset", "Pick_and_Place_Right",
            "Pick_and_Place_Left", "Pick_and_Place_Down", "Garra_Pick_and_Place", "Paletização_Back", "Paletização_Front"
        };

        public FormControleManual()
        {
            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterScreen;
            ConfigurarIconeJanela();
        }

        private void ConfigurarIconeJanela()
        {
            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    using (Bitmap bitmap = new Bitmap(imagePath))
                    {
                        Bitmap iconBitmap = new Bitmap(bitmap, new Size(32, 32));
                        IntPtr hIcon = iconBitmap.GetHicon();
                        this.Icon = Icon.FromHandle(hIcon);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro ao carregar ícone: {ex.Message}");
            }
        }

        private void InitializeComponent()
        {
            this.Text = "Controlo PLC - Saídas Manuais";
            this.Size = new Size(900, 700);
            this.BackColor = Color.FromArgb(9, 35, 50); // #092332
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;

            TagPLC_Manual();

            // Imagem no topo
            pictureBoxTopo = new PictureBox
            {
                Location = new Point(375, 10), // Centralizado no topo
                Size = new Size(130, 110),
                SizeMode = PictureBoxSizeMode.StretchImage,
                BorderStyle = BorderStyle.None,
                BackColor = Color.White
            };

            try
            {
                string imagePath = @"C:\Users\bruno\Desktop\31015_14905_30843\TP_Automacao\TP_Automacao\AUTOFOX.png";
                if (System.IO.File.Exists(imagePath))
                {
                    pictureBoxTopo.Image = Image.FromFile(imagePath);
                }
                else
                {
                    Bitmap placeholder = new Bitmap(150, 100);
                    using (Graphics g = Graphics.FromImage(placeholder))
                    {
                        g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                        g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                    }
                    pictureBoxTopo.Image = placeholder;
                }
            }
            catch
            {
                Bitmap placeholder = new Bitmap(150, 100);
                using (Graphics g = Graphics.FromImage(placeholder))
                {
                    g.FillRectangle(Brushes.LightGray, 0, 0, 150, 100);
                    g.DrawString("FESTO", new Font("Arial", 12, FontStyle.Bold), Brushes.Black, 50, 40);
                }
                pictureBoxTopo.Image = placeholder;
            }

            // Título
            Label lblTitulo = new Label
            {
                Text = "Controlo Manual de Saídas",
                Font = new Font("Segoe UI", 16, FontStyle.Bold),
                Location = new Point(300, 130),
                Size = new Size(300, 30),
                TextAlign = ContentAlignment.MiddleCenter,
                ForeColor = Color.White,
                BackColor = Color.Transparent
            };

            // Criar botões de saída em grade 5x3
            botoesSaidas = new Button[16];
            for (int i = 0; i < 16; i++)
            {
                int row = i / 4;
                int col = i % 4;

                botoesSaidas[i] = new Button
                {
                    Text = nomesSaidas[i] + "\nOFF",
                    Font = new Font("Segoe UI", 9, FontStyle.Bold),
                    Location = new Point(60 + col * 200, 180 + row * 100),
                    Size = new Size(140, 80),
                    BackColor = Color.Gray,
                    ForeColor = Color.White,
                    FlatStyle = FlatStyle.Flat,
                    TextAlign = ContentAlignment.MiddleCenter,
                    Tag = i
                };
                botoesSaidas[i].FlatAppearance.BorderSize = 1;
                botoesSaidas[i].FlatAppearance.BorderColor = Color.DarkGray;

                botoesSaidas[i].MouseDown += BotaoSaida_MouseDown;
                botoesSaidas[i].MouseUp += BotaoSaida_MouseUp;
            }

            // Botão Voltar
            Button btnVoltar = new Button
            {
                Text = "Voltar",
                Font = new Font("Segoe UI", 11),
                Location = new Point(50, 600),
                Size = new Size(100, 35),
                BackColor = Color.Gray,
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };
            btnVoltar.FlatAppearance.BorderSize = 0;
            btnVoltar.Click += (s, e) => this.Close();

            // Adicionar controles ao form
            this.Controls.Add(pictureBoxTopo);
            this.Controls.Add(lblTitulo);
            this.Controls.Add(btnVoltar);

            foreach (Button btn in botoesSaidas)
            {
                this.Controls.Add(btn);
            }
        }

        private void BotaoSaida_MouseDown(object sender, MouseEventArgs e)
        {
            Button btn = (Button)sender;
            int index = (int)btn.Tag;
            
            btn.BackColor = Color.Green;
            btn.Text = nomesSaidas[index] + "\nON";

            tag_Manual[index].SetBit(0, true);
            tag_Manual[index].Write();

            Console.WriteLine($"Saída {nomesSaidas[index]} ativada");
            // Exemplo de como você enviaria um log para o Form1
            // ((Form1)this.Owner).AddLogMessage($"Saída '{nomesSaidas[index]}' ativada.");
        }

        private void BotaoSaida_MouseUp(object sender, MouseEventArgs e)
        {
            Button btn = (Button)sender;
            int index = (int)btn.Tag;

            btn.BackColor = Color.Gray;
            btn.Text = nomesSaidas[index] + "\nOFF";

            tag_Manual[index].SetBit(0, false);
            tag_Manual[index].Write();

            Console.WriteLine($"Saída {nomesSaidas[index]} desativada");
            // Exemplo de como você enviaria um log para o Form1
            // ((Form1)this.Owner).AddLogMessage($"Saída '{nomesSaidas[index]}' desativada.");
        }
        private void TagPLC_Manual()
        {
            tag_Manual = new Tag[16]
            {
                new Tag { Name = "Cilindro_Base", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Cilindro_Tampa_Front", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Cilindro_Tampa_Back", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Cilindro_Tampa_Down", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Vacuo", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Travao_Montagem", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Tapete", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Start", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Stop", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Reset", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Pick_and_Place_Right", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Pick_and_Place_Left", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Pick_and_Place_Down", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Garra_Pick_and_Place", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Paletizacao_Back", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Paletizacao_Front", Gateway = "192.168.250.2", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron }
            };

            for(int i = 0; i < 16; i++)
            {
                tag_Manual[i].Initialize(); // Inicializa a conexão
            }
        }
    }
}