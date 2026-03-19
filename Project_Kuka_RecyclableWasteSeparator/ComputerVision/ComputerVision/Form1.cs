using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using libplctag;

using System.Linq;
using System.Threading.Tasks;

// Adicionar os namespaces do OpenCVSharp
using OpenCvSharp;
using OpenCvSharp.Extensions; // Para o método ToBitmap()

namespace ComputerVision
{
    public partial class Form1 : Form
    {
        #region Variáveis do Sistema
        // Mudar de Emgu.CV.VideoCapture para OpenCvSharp.VideoCapture
        private VideoCapture camera;
        private bool cameraRunning = false;
        private bool plcConnected = false;
        // Mudar de Emgu.CV.Mat para OpenCvSharp.Mat
        private Mat currentFrame;
        private Mat backFrame;

        private System.Windows.Forms.Timer videoTimer;
        private Dictionary<string, bool> plcVariables;
        private Dictionary<string, Label> resultLabels;
        private Dictionary<string, Rectangle> detectionZones;

        // Cores HSV (Hue, Saturation, Value)
        // Mudar de MCvScalar para OpenCvSharp.Scalar
        private readonly Dictionary<string, (Scalar lower, Scalar upper)> colorHSVRanges = new Dictionary<string, (Scalar, Scalar)>
        {
            { "VERDE", (new Scalar(40, 50, 50), new Scalar(80, 255, 255)) },
            { "AMARELO", (new Scalar(20, 100, 100), new Scalar(30, 255, 255)) },
            { "AZUL", (new Scalar(100, 50, 50), new Scalar(130, 255, 255)) }
        };

        // NOVO: Mapeamento para nomes curtos das cores para exibição na UI
        private readonly Dictionary<string, string> colorShortNames = new Dictionary<string, string>
        {
            { "VERDE", "VER" },
            { "AMARELO", "AME" },
            { "AZUL", "AZU" }
        };

        // Campo global da classe (array de bits)
        bool[] novosBits = { false, false, false, false, false, false, false, false, false }; // valores para Bit0 até Bit8
        Tag[] tags;
        #endregion

        public Form1()
        {
            InitializeComponent();
            InitializeSystem();
        }

        private void InitializeSystem()
        {
            // Inicializar variáveis PLC
            plcVariables = new Dictionary<string, bool>
            {
                { "IN_POS1_VERDE", false }, 
                { "IN_POS1_AMARELO", false },   
                { "IN_POS1_AZUL", false },
                { "IN_POS2_VERDE", false },
                { "IN_POS2_AMARELO", false },
                { "IN_POS2_AZUL", false },
                { "IN_POS3_VERDE", false },
                { "IN_POS3_AMARELO", false },
                { "IN_POS3_AZUL", false },
            };

            // Inicializar zonas de detecção
            detectionZones = new Dictionary<string, Rectangle>
            {
                { "POS1", new Rectangle(475, 100, 100, 100) },
                { "POS2", new Rectangle(275, 100, 100, 100) },
                { "POS3", new Rectangle(75, 100, 100, 100) }
            };

            // Inicializar timer de vídeo
            videoTimer = new System.Windows.Forms.Timer();
            videoTimer.Interval = 33; // ~30 FPS
            videoTimer.Tick += VideoTimer_Tick;

            TagPLC();
            SendValuePLC();

            // Inicializar labels de resultado
            InitializeResultLabels();

            // Configurar interface inicial
            UpdateStatusLabels();
            EnableDisableControls();
        }

        private void InitializeResultLabels()
        {
            resultLabels = new Dictionary<string, Label>
            {
                { "IN_POS1_VERDE", lblPos1Verde },
                { "IN_POS1_AMARELO", lblPos1Amarelo },
                { "IN_POS1_AZUL", lblPos1Azul },
                { "IN_POS2_VERDE", lblPos2Verde },
                { "IN_POS2_AMARELO", lblPos2Amarelo },
                { "IN_POS2_AZUL", lblPos2Azul },
                { "IN_POS3_VERDE", lblPos3Verde },
                { "IN_POS3_AMARELO", lblPos3Amarelo },
                { "IN_POS3_AZUL", lblPos3Azul }
            };

            // Configurar labels iniciais com o nome da cor e o "X"
            foreach (var entry in plcVariables) // Iterar sobre plcVariables para ter as chaves (ex: "IN_POS1_AMARELO")
            {
                if (resultLabels.ContainsKey(entry.Key))
                {
                    Label label = resultLabels[entry.Key];

                    // Extrair o nome da cor (ex: "IN_POS1_AMARELO" -> "AMARELO")
                    string[] parts = entry.Key.Split('_');
                    string fullColorName = parts[2];

                    // Obter o nome curto para exibição (ex: "AMARELO" -> "AME")
                    string shortColorName = colorShortNames.ContainsKey(fullColorName) ? colorShortNames[fullColorName] : fullColorName;

                    label.Text = $"{shortColorName}: ❌"; // Define o texto inicial como "AME: ❌"
                    label.ForeColor = Color.Red;
                }
            }
        }

        #region Controlo da Câmara
        private void btnStartCamera_Click(object sender, EventArgs e)
        {
            try
            {
                camera = new VideoCapture(5); // Abrir câmara predefinida
                // camera.IsOpened é uma propriedade em OpenCVSharp
                if (!camera.IsOpened())
                {
                    throw new Exception("Não foi possível abrir a câmara");
                }

                cameraRunning = true;
                videoTimer.Start();

                UpdateStatusLabels();
                EnableDisableControls();

                MessageBox.Show("Câmara ligada com sucesso!", "Sucesso", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao iniciar câmara: {ex.Message}", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnStopCamera_Click(object sender, EventArgs e)
        {
            try
            {
                cameraRunning = false;
                videoTimer.Stop();

                if (camera != null)
                {
                    camera.Dispose();
                    camera = null;
                }

                // Limpar display de vídeo
                pictureBoxVideo.Image?.Dispose();
                pictureBoxVideo.Image = null;
                pictureBox1.Image?.Dispose();
                pictureBox1.Image = null;

                UpdateStatusLabels();
                EnableDisableControls();

                MessageBox.Show("Câmara desligada", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao parar câmara: {ex.Message}", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void VideoTimer_Tick(object sender, EventArgs e)
        {
            // camera.IsOpened é uma propriedade em OpenCVSharp
            if (cameraRunning && camera != null && camera.IsOpened())
            {
                try
                {
                    // CORREÇÃO: Usando a sobrecarga de Read() que preenche um Mat existente.
                    // Isso é o que a maioria das versões do OpenCvSharp espera para evitar o erro CS1501.
                    // currentFrame deve ser inicializado ou re-inicializado se for nulo.
                    if (currentFrame == null)
                    {
                        currentFrame = new Mat();
                    }

                    // Tenta ler o frame da câmera. O método retorna um bool indicando sucesso.
                    bool success = camera.Read(currentFrame);

                    // A propriedade Empty() do OpenCvSharp.Mat funciona como esperado para verificar se há dados
                    // e garantimos que a leitura foi bem-sucedida.
                    if (success && !currentFrame.Empty()) // CORREÇÃO: Usando .Empty()
                    {
                        // Desenhar zonas de detecção
                        Mat displayFrame = currentFrame.Clone();
                        DrawDetectionZones(displayFrame);
                        // Converter para Bitmap e mostrar - Usar ToBitmap() do OpenCvSharp.Extensions
                        var bitmap = displayFrame.ToBitmap();

                        if (bitmap != null)
                        {
                            // Redimensionar para caber no PictureBox
                            var resizedBitmap = new Bitmap(bitmap, pictureBoxVideo.Width, pictureBoxVideo.Height);
                            bitmap.Dispose();

                            // Limpar imagem anterior
                            pictureBoxVideo.Image?.Dispose();
                            pictureBoxVideo.Image = resizedBitmap;

                            imageblackobjet(displayFrame);
                        }

                        displayFrame.Dispose();
                    }
                    else
                    {
                        // Se o frame for nulo ou vazio, pode indicar que a câmera foi desconectada
                        Console.WriteLine("Falha ao ler o frame da câmera ou o frame é vazio. Verifique a conexão da câmera.");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Erro no timer de vídeo: {ex.Message}");
                }
            }
        }

        private void DrawDetectionZones(Mat frame)
        {
            foreach (var zone in detectionZones)
            {
                string posName = zone.Key;
                Rectangle rect = zone.Value;

                // CORREÇÃO: Converter System.Drawing.Rectangle para OpenCvSharp.Rect
                OpenCvSharp.Rect cvRect = new OpenCvSharp.Rect(rect.X, rect.Y, rect.Width, rect.Height);

                // Desenhar retângulo - CvInvoke.Rectangle torna-se Cv2.Rectangle
                Cv2.Rectangle(frame, cvRect, new Scalar(255, 255, 255), 1); // Linha corrigida!

                // Desenhar texto - CvInvoke.PutText torna-se Cv2.PutText
                // FontFace.HersheySimplex torna-se HersheyFonts.HersheySimplex
                Cv2.PutText(frame, posName,
                    new OpenCvSharp.Point(rect.X, rect.Y - 10), // Argumento 2: Posição do texto (OpenCvSharp.Point)
                    HersheyFonts.HersheySimplex,                // Argumento 3: Tipo de fonte (HersheyFonts)
                    0.7,                                        // Argumento 4: Escala da fonte (double)
                    new Scalar(255, 255, 255),                    // Argumento 5: Cor (OpenCvSharp.Scalar)
                    1);                                         // Argumento 6: Espessura (int)
            }
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            if (currentFrame != null && !currentFrame.Empty())
            {
                Task.Run(() => CaptureAndDetect());
            }
            else
            {
                MessageBox.Show("Nenhum frame disponível para captura", "Aviso", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void CaptureAndDetect()
        {
            try
            {
                if (currentFrame != null)
                {
                    // currentFrame.Clone() para evitar que o frame da câmera seja modificado enquanto detectamos
                    using (Mat frameForDetection = currentFrame.Clone())
                    {
                        var results = DetectColors(frameForDetection);

                        // Atualizar interface na thread principal
                        this.Invoke(new Action(() =>
                        {
                            UpdatePLCVariables(results);
                            UpdateDisplay();
                        }));
                    }
                }
            }
            catch (Exception ex)
            {
                this.Invoke(new Action(() =>
                {
                    MessageBox.Show($"Erro na detecção: {ex.Message}", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }));
            }
        }
        #endregion

        #region Detecção de Cores
        private Dictionary<string, bool> DetectColors(Mat frame)
        {
            var results = new Dictionary<string, bool>();

            // Converter para HSV - CvInvoke.CvtColor torna-se Cv2.CvtColor
            // ColorConversion.Bgr2Hsv torna-se ColorConversionCodes.BGR2HSV
            Mat hsvFrame = new Mat();
            Cv2.CvtColor(frame, hsvFrame, ColorConversionCodes.BGR2HSV);

            foreach (var zone in detectionZones)
            {
                string posName = zone.Key;
                Rectangle rect = zone.Value;

                // Verificar se o retângulo está dentro dos limites da imagem
                if (rect.X >= 0 && rect.Y >= 0 &&
                    rect.X + rect.Width <= frame.Width &&
                    rect.Y + rect.Height <= frame.Height)
                {
                    // Extrair região de interesse
                    // OpenCvSharp.Rect é mais comum, mas System.Drawing.Rectangle funciona na maioria dos casos
                    Mat roi = new Mat(hsvFrame, new OpenCvSharp.Rect(rect.X, rect.Y, rect.Width, rect.Height));

                    foreach (var color in colorHSVRanges)
                    {
                        string colorName = color.Key;
                        var (lower, upper) = color.Value;

                        // Criar máscara para a cor - CvInvoke.InRange torna-se Cv2.InRange
                        // ScalarArray torna-se Scalar diretamente
                        Mat mask = new Mat();
                        Cv2.InRange(roi, lower, upper, mask);

                        // Contar pixels da cor - CvInvoke.CountNonZero torna-se Cv2.CountNonZero
                        int colorPixels = Cv2.CountNonZero(mask);
                        int totalPixels = roi.Width * roi.Height;

                        // Se mais de 10% da área contém a cor, considera detectada
                        bool detected = totalPixels > 0 && (colorPixels / (double)totalPixels) > 0.1;

                        string varName = $"IN_{posName}_{colorName}";
                        results[varName] = detected;

                        mask.Dispose();
                    }

                    roi.Dispose();
                }
            }

            hsvFrame.Dispose();
            return results;
        }

        private void imageblackobjet(Mat displayFrame)
        {
            // ==== NOVO BLOCO PARA FILTRAR APENAS AS CORES ====
            Mat hsv = new Mat();
            Cv2.CvtColor(displayFrame, hsv, ColorConversionCodes.BGR2HSV);

            // Máscaras para as cores desejadas
            Mat maskBlue = new Mat();
            Mat maskGreen = new Mat();
            Mat maskYellow = new Mat();
            Mat combinedMask = new Mat();

            foreach (var color in colorHSVRanges)
            {
                string colorName = color.Key;
                var (lower, upper) = color.Value;

                if(colorName == "VERDE")
                {
                    Cv2.InRange(hsv, lower, upper, maskGreen);
                }
                if (colorName == "AMARELO")
                {
                    Cv2.InRange(hsv, lower, upper, maskYellow);
                }
                if (colorName == "AZUL")
                {
                    Cv2.InRange(hsv, lower, upper, maskBlue);
                }
            }

            // Combinar as máscaras
            Cv2.BitwiseOr(maskBlue, maskGreen, combinedMask);
            Cv2.BitwiseOr(combinedMask, maskYellow, combinedMask);

            // Aplicar a máscara na imagem original (fundo preto)
            Mat filteredFrame = new Mat();
            Cv2.BitwiseAnd(displayFrame, displayFrame, filteredFrame, combinedMask);
            DrawDetectionZones(filteredFrame);

            // Redimensionar para caber no PictureBox
            var bitmap1 = filteredFrame.ToBitmap();
            var resizedBitmap1 = new Bitmap(bitmap1, pictureBox1.Width, pictureBox1.Height);

            // Limpar imagem anterior
            pictureBox1.Image?.Dispose();
            pictureBox1.Image = resizedBitmap1;

            // Limpar
            hsv.Dispose();
            maskBlue.Dispose();
            maskGreen.Dispose();
            maskYellow.Dispose();
            combinedMask.Dispose();
            bitmap1.Dispose();
        }

        private void UpdatePLCVariables(Dictionary<string, bool> results)
        {
            foreach (var result in results)
            {              
                if (plcVariables.ContainsKey(result.Key))
                {
                    plcVariables[result.Key] = result.Value;
                }
            }

            novosBits[0] = plcVariables["IN_POS1_VERDE"];
            novosBits[1] = plcVariables["IN_POS1_AMARELO"];
            novosBits[2] = plcVariables["IN_POS1_AZUL"];
            novosBits[3] = plcVariables["IN_POS2_VERDE"];
            novosBits[4] = plcVariables["IN_POS2_AMARELO"];
            novosBits[5] = plcVariables["IN_POS2_AZUL"];
            novosBits[6] = plcVariables["IN_POS3_VERDE"];
            novosBits[7] = plcVariables["IN_POS3_AMARELO"];
            novosBits[8] = plcVariables["IN_POS3_AZUL"];
            SendValuePLC();
        }

        private void UpdateDisplay()
        {
            foreach (var plcVar in plcVariables)
            {
                if (resultLabels.ContainsKey(plcVar.Key))
                {
                    Label label = resultLabels[plcVar.Key];
                    bool detected = plcVar.Value;

                    // Extrair o nome da cor da chave do dicionário (ex: "IN_POS1_AMARELO" -> "AMARELO")
                    string[] parts = plcVar.Key.Split('_');
                    string fullColorName = parts[2];

                    // Obter o nome curto para exibição (ex: "AMARELO" -> "AME")
                    string shortColorName = colorShortNames.ContainsKey(fullColorName) ? colorShortNames[fullColorName] : fullColorName;

                    // Concatenar o nome da cor com o símbolo de status
                    label.Text = $"{shortColorName}: " + (detected ? "✅" : "❌");
                    label.ForeColor = detected ? Color.Green : Color.Red;
                }
            }
        }
        #endregion


        #region Controlos e Interface
        private void btnReset_Click(object sender, EventArgs e)
        {
            int i = 0;

            try
            {
                // Resetar todas as variáveis PLC
                foreach (var key in plcVariables.Keys.ToList())
                {
                    plcVariables[key] = false;
                    novosBits[i++] = false;
                }

                // Atualizar interface
                UpdateDisplay(); // Importante chamar para resetar os textos das labels também
                SendValuePLC();

                MessageBox.Show("Todas as detecções foram resetadas!", "Reset", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao resetar: {ex.Message}", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void UpdateStatusLabels()
        {
            // Atualizar status da câmara
            lblCameraStatus.Text = cameraRunning ? "Status: Ligada" : "Status: Desligada";
            lblCameraStatus.ForeColor = cameraRunning ? Color.Green : Color.Red;
        }

        private void EnableDisableControls()
        {
            // Controlos da câmara
            btnStartCamera.Enabled = !cameraRunning;
            btnStopCamera.Enabled = cameraRunning;
            btnCapture.Enabled = cameraRunning;
        }
        #endregion

        #region PLC_Configuration
        private void TagPLC()
        {
            tags = new Tag[9]
            {
                new Tag { Name = "V1", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "A1", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Az1", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "V2", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "A2", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Az2", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "V3", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "A3", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron },
                new Tag { Name = "Az3", Gateway = "192.168.250.1", Path = "1,0", Protocol = Protocol.ab_eip, PlcType = PlcType.Omron }
            };

            tags[0].Initialize();                   // Inicializa a conexão
            tags[1].Initialize();                   // Inicializa a conexão
            tags[2].Initialize();                   // Inicializa a conexão
            tags[3].Initialize();                   // Inicializa a conexão
            tags[4].Initialize();                   // Inicializa a conexão
            tags[5].Initialize();                   // Inicializa a conexão
            tags[6].Initialize();                   // Inicializa a conexão
            tags[7].Initialize();                   // Inicializa a conexão
            tags[8].Initialize();                   // Inicializa a conexão

        }
        private void SendValuePLC()
        {

            try
            {
                for (int i = 0; i < novosBits.Length; i++)
                {
                    tags[i].SetBit(0, novosBits[i]);
                    tags[i].Write();
                }

                MessageBox.Show("Bits escritos com sucesso!", "Escrita");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Erro ao escrever: " + ex.Message, "Erro");
            }
        }
        #endregion

        #region Cleanup
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                // Parar câmara
                cameraRunning = false;
                videoTimer?.Stop();
                camera?.Dispose();  

                // Limpar recursos
                currentFrame?.Dispose();
                pictureBoxVideo.Image?.Dispose();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro ao fechar aplicação: {ex.Message}");
            }
        }
        #endregion

        // Event handlers vazios
        private void grpResults_Enter(object sender, EventArgs e) { }
        private void panelResults_Paint(object sender, PaintEventArgs e) { }
        private void panelControl_Paint(object sender, EventArgs e) { }
        private void grpCamera_Enter(object sender, EventArgs e) { }
        private void lblTitle_Click(object sender, EventArgs e) { }
        private void lblPos1Amarelo_Click(object sender, EventArgs e) { }
        private void lblPos2Amarelo_Click(object sender, EventArgs e) { }
        private void lblPos1Azul_Click(object sender, EventArgs e) { }
        private void pictureBoxVideo_Click(object sender, EventArgs e) {}
    }
}