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
  	websocket.onmessage = onMessage; // <-- add this line
}
function onOpen(event) {
  	console.log('Connection opened');
}
function onClose(event) {
  	console.log('Connection closed');
  	setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
    console.log('refreshing chart');
    let result = null;
    const front_data = [];
    const rear_data = [];
    const xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", "/data.bin", false);
    xmlhttp.send();
    if(xmlhttp.status === 200){
        result = xmlhttp.responseText;
    }
    if(result != null) {
        // create chart for fork travel
        chart_front_travel = Highcharts.chart('chart_front_travel', {
            chart: {
                type: 'line',
                scrollablePlotArea: {
                    minWidth: result.length * 10,
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
                    minWidth: result.length * 10,
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
        for(let i = 0; i < result.length; i++) {
            if(i%2)
            {
                front_data.push(result.charAt(i).charCodeAt(0));
                console.log(result.charAt(i).charCodeAt(0));
                continue;
            }
            rear_data.push(result.charAt(i).charCodeAt(0));
            console.log(result.charAt(i).charCodeAt(0));
        }
        chart_front_travel.series[0].setData(front_data);
        chart_rear_travel.series[0].setData(rear_data);
    }
}

function refresh(){
    websocket.send('getData');
}

function initButton() {
    document.getElementById('button').addEventListener('click', refresh);
}

function onLoad(event) {
    initWebSocket();
    initButton();

    // create chart for fork travel
    chart_front_travel = Highcharts.chart('chart_front_travel', {
        chart: {
            type: 'line',
            scrollablePlotArea: {
                minWidth: 100,
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
                minWidth: 100,
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
    chart_front_travel.series[0].setData([1,0,1,0]);
    chart_rear_travel.series[0].setData([1,2,1,2]);
}