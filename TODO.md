bin转mp4
```  
ffmpeg -f rawvideo -pix_fmt gray -s 640x480 -r 25 -i input.bin -c:v libx264 -preset slow -crf 22 -t 10 output.mp4  
```

得到参数
```  
int main(int argc, char* argv[])
{
    if (argc > 1) {
        cout << "First command line argument: " << argv[1] << endl;
    } else {
        cout << "No command line arguments provided." << endl;
    }
    return 0;
}  
```

todo清单：  
1.bin转png或mp4   
2.mp4解码，提高准确率  
3.对比前后bin文件  


问题：  
你觉得提问题有用吗sbhw你看看你在说什么
