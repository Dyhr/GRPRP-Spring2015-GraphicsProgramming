using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using RayTracer;
using PixelFormat = System.Drawing.Imaging.PixelFormat;

namespace WindowApplication
{
    class ViewModel
    {

        public BitmapImage Image { get; private set; }

        private int _bitmapHeight;
        private int _bitmapWidth;
        private int _viewPortHeight;
        private int _viewPortWidth;
        private Scene _scene;

        public ViewModel(int bitmapWidth, int bitmapHeight, int viewPortWidth, int viewPortHeight)
        {
            _bitmapWidth = bitmapWidth;
            _bitmapHeight = bitmapHeight;

            _viewPortWidth = viewPortWidth;
            _viewPortHeight = viewPortHeight;

            int zDepth = 10;
            _scene = new Scene(viewPortWidth, viewPortHeight, zDepth);
            _scene.init(bitmapWidth, bitmapHeight);
        }

        private void PrepareImage()
        {
            unsafe
            {
                // Get array from C++ module
                var array = _scene.render();

                for (int i = 0; i < array.Length; i++)
                {
                    var color = array[i];

                    int red = color.red;
                    int green = color.green;
                    int blue = color.blue;
                    int alpha = color.alpha;

                    int x = i / _bitmapWidth;
                    int y = i % _bitmapWidth;

                    var colorToInsert = System.Drawing.Color.FromArgb(alpha, red, green, blue);
                }

                // Set BitMapImage
            }
        }

        private BitmapImage ConvertImage(byte[] array)
        {



            using (var ms = new System.IO.MemoryStream(array))
            {
                var image = new BitmapImage();
                image.BeginInit();
                image.CacheOption = BitmapCacheOption.OnLoad;
                image.StreamSource = ms;
                image.EndInit();
                return image;
            }
        }
    }
}
