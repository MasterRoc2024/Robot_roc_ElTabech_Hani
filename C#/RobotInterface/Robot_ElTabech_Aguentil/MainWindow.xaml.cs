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


namespace Robot_ElTabech_Aguentil
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;


        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();

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
            //RichTextBox.Text += "Reçu: ";
            //RichTextBox.Text += textBoxEmission.Text;
            //RichTextBox.Text += "\n";
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
            WriteLine("ff");
            //RichTextBox.Text += textBoxEmission.Text;
        }

      
    }
}