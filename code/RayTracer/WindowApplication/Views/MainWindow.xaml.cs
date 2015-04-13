using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows;
using System.Windows.Media.Imaging;
using WindowApplication.ViewModels;

namespace WindowApplication.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
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
    }
}
