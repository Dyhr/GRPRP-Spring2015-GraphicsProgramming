using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Drawing;
using System.Windows.Media.Imaging;
using Color = RayTracer.Color;

namespace WindowApplication
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            var viewModel = new ViewModel(20,20,400,400);

            var image = new BitmapImage();
            Canvas.Source = image;
        }

        private static byte[] ConvertData(Color[] data)
        {
            var rnd = new Random();
            var output = new List<byte>();
            foreach (var color in data)
            {
                output.Add((byte)rnd.Next(255));
                output.Add((byte)rnd.Next(255));
                output.Add((byte)rnd.Next(255));
            }
            return output.ToArray();
        }

        private static Image LoadImage(byte[] imageData)
        {
            throw new NotImplementedException(); // TODO implement
        }
    }
}
