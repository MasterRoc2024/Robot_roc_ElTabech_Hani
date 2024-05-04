using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;
using System.IO.Ports;
using System.Windows.Threading;


namespace Robot_ElTabech_Aguentil
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;

        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM21", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived1;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

        }

        private void SerialPort1_DataReceived1(object? sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach (byte value in e.Data)
            {
                robot.byteListReceived.Enqueue(value);
            }
        }

        DispatcherTimer timerAffichage;


        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            while (robot.byteListReceived != null)
            {
                var c = robot.byteListReceived.Dequeue();
                textBoxReception.Text += "0x" + c.ToString("X2") + " ";
                //textBoxReception.Text += robot.byteListReceived.Dequeue().ToString;
            }

        }
        
        public byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction & 0xFF);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength & 0xFF);
        
            foreach (byte b in msgPayload)
            {
                checksum ^= b;
            }
        
            return checksum;
        }
        
        bool button = false;

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (button == true)
            { 
                buttonEnvoyer.Background = Brushes.Beige;
                button = false;
                }
                else
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
                button = true;
            }
            SendMessage();
            //RichTextBox.Text += "Reçu: ";
            //RichTextBox.Text += textBoxEmission.Text;
            //RichTextBox.Text += "\n";
        }

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
                byteList[i] = (byte)(2 * i);
            serialPort1.Write(byteList, 0, byteList.Length);

        }





        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }



        private void SendMessage()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxReception.Text += textBoxEmission.Text;
            textBoxEmission.Text = "";
        }




    }
}
