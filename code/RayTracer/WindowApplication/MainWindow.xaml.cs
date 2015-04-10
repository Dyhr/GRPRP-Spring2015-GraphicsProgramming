using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Drawing;
using RayTracer;
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

            var scene = new Scene();
            scene.init(4,4);
            var data = scene.render();
            var bytearr = ConvertData(data);
            var img = LoadImage(bytearr);
            Canvas.Source = (ImageSource)new ImageSourceConverter().ConvertFrom(img);
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
