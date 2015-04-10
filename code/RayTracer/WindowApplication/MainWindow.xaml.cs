using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
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
            scene.init(800,600);
            var data = scene.render();
            var bytearr = ConvertData(data);
            var img = LoadImage(bytearr);
            Canvas.Source = img;
        }

        private byte[] ConvertData(Color[] data)
        {
            var output = new List<byte>();
            foreach (var color in data)
            {
                output.Add((byte)color.red);
                output.Add((byte)color.blue);
                output.Add((byte)color.green);
                output.Add((byte)color.alpha);
            }
            return output.ToArray();
        }

        private static BitmapImage LoadImage(byte[] imageData)
        {
            if (imageData == null || imageData.Length == 0) return null;
            var image = new BitmapImage();
            using (var mem = new MemoryStream(imageData))
            {
                mem.Position = 0;
                image.BeginInit();
                image.CreateOptions = BitmapCreateOptions.PreservePixelFormat;
                image.CacheOption = BitmapCacheOption.OnLoad;
                image.UriSource = null;
                image.StreamSource = mem;
                image.EndInit();
            }
            image.Freeze();
            return image;
        }
    }
}
