// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.
var map = new BMap.Map("container");
var points;
points = new BMap.Point(116.65644,39.89045);
map.addControl(new BMap.NavigationControl());
map.centerAndZoom(points, 15);
map.enableScrollWheelZoom(true);
var marker = new BMap.Marker(points[0]);
//map.addOverlay(marker);
require('electron').ipcRenderer.on('start', (event, data) => {
    var s = String(data);
    var ndx = s.indexOf(',');
    var s1 = s.substr(0, ndx);
    var s2 = s.substr(ndx+1, s.length-ndx);
    var lon = Number(s1);
    var lat = Number(s2);
    var pt = new BMap.Point(lon, lat);
	var myIcon = new BMap.Icon("http://lbsyun.baidu.com/jsdemo/img/fox.gif", new BMap.Size(200,100));
	var marker2 = new BMap.Marker(pt,{icon:myIcon});  // 创建标注
    map.centerAndZoom(pt, 15);
    map.addOverlay(marker2);              // 将标注添加到地图中
})

require('electron').ipcRenderer.on('dest', (event, data) => {
    var s = String(data);
    var ndx = s.indexOf(',');
    var s1 = s.substr(0, ndx);
    var s2 = s.substr(ndx+1, s.length-ndx);
    var lon = Number(s1);
    var lat = Number(s2);
    var pt = new BMap.Point(lon, lat);
    var mk = new BMap.Marker(pt);
    map.addOverlay(mk);
})