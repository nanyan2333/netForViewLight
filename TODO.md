bin转mp4
ffmpeg -f rawvideo -pix_fmt gray -s 640x480 -r 25 -i input.bin -c:v libx264 -preset slow -crf 22 -t 10 output.mp4

得到参数
int main(int argc, char* argv[])
{
    if (argc > 1) {
        cout << "First command line argument: " << argv[1] << endl;
    } else {
        cout << "No command line arguments provided." << endl;
    }
    return 0;
}