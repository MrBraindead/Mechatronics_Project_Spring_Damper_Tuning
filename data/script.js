//websocket
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var chart_front_travel;
var chart_rear_travel;
window.addEventListener('load', onLoad);
function initWebSocket() {
  	console.log('Trying to open a WebSocket connection...');
  	websocket = new WebSocket(gateway);
  	websocket.onopen    = onOpen;
  	websocket.onclose   = onClose;
  	websocket.onmessage = onMessage;
}
function onOpen(event) {
  	console.log('Connection opened');
}
function onClose(event) {
  	console.log('Connection closed');
  	setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
    if(event.data.toString().localeCompare( "SensorOFF") == 0)
    {
        console.log("Sensors OFF");
        document.getElementById("button_start_stop").innerHTML = "Start";
        refresh();
    }
    else if(event.data.localeCompare("SensorON") == 0)
    {
        console.log("Sensors ON");
        document.getElementById("button_start_stop").innerHTML = "Stop";
    }
    else{
        console.log('refreshing chart');
        let result = null;
        const front_data = [];
        const rear_data = [];
        const xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", event.data, false);
        xmlhttp.overrideMimeType("application/x-binary; charset=ISO-8859-1"); // Changing MIME Type so data is properly decoded
        xmlhttp.send();
        if(xmlhttp.status === 200){
            result = xmlhttp.responseText;
        }
        if(result != null) {
            // recreating the chart for fork travel with a different minimum Width
            chart_front_travel = Highcharts.chart('chart_front_travel', {
                chart: { type: 'line', scrollablePlotArea: { minWidth: result.length, scrollPosition: 1 } },
                title: { text: 'fork travel' },
                xAxis: { type: 'datetime' },
                yAxis: { title: { text: 'travel (mm)' } },
                series: [{ name: '' }],
                plotOptions:{ line:{ lineWidth: 1, }, series:{ pointStart:0, pointInterval: 1, turboThreshold: 0, }, }
            });

            // recreating the chart for shock travel with a different minimum Width
            chart_rear_travel = Highcharts.chart('chart_rear_travel', {
                chart: { type: 'line', scrollablePlotArea: { minWidth: result.length, scrollPosition: 1 } },
                title: { text: 'shock travel' },
                xAxis: { type: 'datetime' },
                yAxis: { title: { text: 'travel (mm)' } },
                series: [{ name: '' }],
                plotOptions:{ line:{ lineWidth: 1, }, series:{ pointStart:0, pointInterval: 10 } }
            });
            for(let i = 0; i < result.length; i++) {
                if(i%2)
                {
                    front_data.push(Math.min(result.charCodeAt(i), 255));
                    continue;
                }
                rear_data.push(Math.min(result.charCodeAt(i), 255));
            }
            chart_front_travel.series[0].setData(front_data);
            chart_rear_travel.series[0].setData(rear_data);
        }
    }
}

function refresh(){
    websocket.send('getData');
}
function start_stop(){
    websocket.send('toggleSensors');
}

function clear(){
    websocket.send('clear');
    chart_front_travel.series[0].setData([]);
    chart_rear_travel.series[0].setData([]);
}

function initButton() {
    document.getElementById('button_refresh').addEventListener('click', refresh);
    document.getElementById('button_start_stop').addEventListener('click', start_stop);
    document.getElementById('button_clear').addEventListener('click', clear);
}

function onLoad(event) {
    initWebSocket();
    initButton();

    // create chart for fork travel
    chart_front_travel = Highcharts.chart('chart_front_travel', {
        chart: {
            type: 'line',
            scrollablePlotArea: {
                minWidth: 1200,
                scrollPosition: 1
            }
        },
        title: {
            text: 'fork travel'
        },
        xAxis: {
            type: 'datetime'
        },
        yAxis: {
            title: {
                text: 'travel (mm)'
            }
        },
        series: [{
            name: ''
        }],
        plotOptions:{
            line:{
                lineWidth: 1,
            },
            series:{
                pointStart:0,
                pointInterval: 1, // interval in ms
                turboThreshold: 0,
            },
        }
    });

    // create chart for shock travel
    chart_rear_travel = Highcharts.chart('chart_rear_travel', {
        chart: {
            type: 'line',
            scrollablePlotArea: {
                minWidth: 1200,
                scrollPosition: 1
            }
        },
        title: {
            text: 'shock travel'
        },
        xAxis: {
            type: 'datetime'
        },
        yAxis: {
            title: {
                text: 'travel (mm)'
            }
        },
        series: [{
            name: ''
        }],
        plotOptions:{
            line:{
                lineWidth: 1,
            },
            series:{
                pointStart:0,
                pointInterval: 10 // interval in ms
            }
        }
    });
    chart_front_travel.reflow();
    chart_rear_travel.reflow();
    chart_front_travel.series[0].setData([1,0,1,0]);
    chart_rear_travel.series[0].setData([1,2,1,2]);
}