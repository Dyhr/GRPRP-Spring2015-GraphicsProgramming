using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace WindowApplication.ViewModels
{
    public class RenderViewModel : ViewModelBase
    {

        public BitmapImage Image { get; private set; }

        private int _bitmapHeight;
        private int _bitmapWidth;
        private int _viewPortHeight;
        private int _viewPortWidth;
        private Scene _scene;

        public RenderViewModel(int bitmapWidth, int bitmapHeight, int viewPortWidth, int viewPortHeight)
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

        private BitmapSource _renderBitMapSource;
        public BitmapSource RenderBitmap
        {
            get { return _renderBitMapSource; }
            set
            {
                _renderBitMapSource = value;
                NotifyPropertyChanged("RenderBitmap");
            }
        }

        async public void UpdateImage()
        {
            Task<Color[,]> temp = Controller.GetInstance().Render();  // get the color array from the ray tracing project
            Color[,] colorArray = await temp;
            var width = colorArray.GetUpperBound(0) + 1;
            var height = colorArray.GetUpperBound(1) + 1;
            var stride = width * 4; // bytes per row

            byte[] pixelData = new byte[height * stride];

            for (int x = 0; x < width; ++x)
            {
                for (int z = 0; z < height; ++z)
                {
                    var color = colorArray[x, height - z - 1];
                    var index = (z * stride) + (x * 4);

                    pixelData[index] = color.B;
                    pixelData[index + 1] = color.G;
                    pixelData[index + 2] = color.R;
                    pixelData[index + 3] = color.A;
                }
            }

            RenderBitmap = BitmapSource.Create(width, height, 96, 96, PixelFormats.Bgra32, null, pixelData, stride);
        }
    }
}
