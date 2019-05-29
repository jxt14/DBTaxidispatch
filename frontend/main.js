const {app, BrowserWindow} = require('electron')

let mainWindow

function createWindow () {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true
    }
  })

  mainWindow.loadFile('index.html')

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()

  // Emitted when the window is closed.
  mainWindow.on('closed', function () {
    mainWindow = null
  })
}

app.on('ready', createWindow)

var p = 1;
var net = require('net');
var HOST = '127.0.0.1';
var PORT = 7000;
 
var client = new net.Socket();
client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // 建立连接后立即向服务器发送数据，服务器将收到这些数据
    client.write('I am Chuck Norris!');
});
 
// 为客户端添加“data”事件处理函数
// data是服务器发回的数据
client.on('data', function(data) {
    var s = String(data);
    if (s=='end'){
      console.log('SEARCH FINISHED');
      client.destroy();
    }
    else{
      var ndx = s.indexOf(',');
      var s1 = s.substr(0, ndx);
      var s2 = s.substr(ndx+1, s.length-ndx);
    
      if (p == 1){
        mainWindow.webContents.send('start', data);
        p = 0;
      }
      else {
        mainWindow.webContents.send('dest', data);
      }
      console.log('DATA: ' + s1 + ' ' + s2);
    }
    // 完全关闭连接
});
 
// 为客户端添加“close”事件处理函数
client.on('close', function() {
    console.log('Connection closed');
});
