import subprocess
import webview

class Api:
    def getMessage(self):
        return 'Hello from the Python backend!'
    def test(self):
        return 'is a return'

    def run_exe(self):
        # 使用 subprocess 运行可执行文件，并捕获其输出
        result = subprocess.run(['main.exe','10'], capture_output=True, text=True, shell=True)
        # 获取可执行文件的输出
        output = result.stdout
        print(output)
        # 将输出传递给网页
        return output

def main():
    api = Api()
    webview.create_window('Pywebview Example', 'ui.html', js_api=api, width=800, height=600)
    webview.start()

if __name__ == '__main__':
    main()
