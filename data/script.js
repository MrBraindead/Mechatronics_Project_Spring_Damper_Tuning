/***********************************************************************************************************************
 * author: Mika Schmitt scmi1066@h-ka.de *
 * last modified: 26.01.2022 *
 **********************************************************************************************************************/

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
    // When the sensors are turned off the button text is changed to "Start" and the chart is refreshed with the new data
    if(event.data.toString().localeCompare( "SensorOFF") == 0)
    {
        console.log("Sensors OFF");
        document.getElementById("button_start_stop").innerHTML = "Start";
        refresh();
    }
    // When the sensors are turned on the button text is changed to "Stop"
    else if(event.data.localeCompare("SensorON") == 0)
    {
        console.log("Sensors ON");
        document.getElementById("button_start_stop").innerHTML = "Stop";
    }
    // Recieving the Uri for the new data
    else{
        console.log('refreshing chart');
        let result = null;      // http response
        const front_data = [];  // data for fork
        const rear_data = [];   // data for shock
        const xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", event.data, false);
        // Changing MIME Type so data// is properly decoded
        xmlhttp.overrideMimeType("application/x-binary; charset=ISO-8859-1");
        xmlhttp.send();
        if(xmlhttp.status === 200){ // HTTP-request successful
            result = xmlhttp.responseText;
        }
        if(result != null) {
            // recreating the chart for fork travel with an adjusted minimum Width
            chart_front_travel = Highcharts.chart('chart_front_travel', {
                chart: { type: 'line', scrollablePlotArea: { minWidth: result.length, scrollPosition: 1 } },
                title: { text: 'fork travel' },
                xAxis: { type: 'datetime' },
                yAxis: { title: { text: 'travel (mm)' } },
                series: [{ name: '' }],
                plotOptions:{ line:{ lineWidth: 1, }, series:{ pointStart:0, pointInterval: 1, turboThreshold: 0, }, }
            });

            // recreating the chart for shock travel with an adjusted minimum Width
            chart_rear_travel = Highcharts.chart('chart_rear_travel', {
                chart: { type: 'line', scrollablePlotArea: { minWidth: result.length, scrollPosition: 1 } },
                title: { text: 'shock travel' },
                xAxis: { type: 'datetime' },
                yAxis: { title: { text: 'travel (mm)' } },
                series: [{ name: '' }],
                plotOptions:{ line:{ lineWidth: 1, }, series:{ pointStart:0, pointInterval: 10 } }
            });
            // Looping through the response data and adding data points to the respective array
            // Datapoints alternate between Fork and Shock
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
    websocket.send('getData');              // Requests Uri for the data file
}
function start_stop(){
    websocket.send('toggleSensors');        // Request to toggle the Sensors
}
function clear(){
    websocket.send('clear');                // Request to clear the data file
    chart_front_travel.series[0].setData([]);    // clear fork-chart
    chart_rear_travel.series[0].setData([]);     // clear shock chart
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

    chart_front_travel.reflow();    //adjust width to container size
    chart_rear_travel.reflow();     //adjust width to container size
}