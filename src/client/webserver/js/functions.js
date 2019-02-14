function dateFormat(d) {
  // change a date object in the needed date-string
  return  d.getFullYear() +"-"+ ("0"+(d.getMonth()+1)).slice(-2)+"-"+("0" + d.getDate()).slice(-2)+" " + ("0" + d.getHours()).slice(-2) + ":" + ("0" + d.getMinutes()).slice(-2)+ ":00";
}

// options for the data-picker
var pickerOptions = {format:'d.m.Y H:i',inline:true,lang:'en'}

// create datetimepickers
$('#startDate').datetimepicker(pickerOptions);
$('#endDate').datetimepicker(pickerOptions);

// handle click on button
$('#showData').click(function(){
  var begin = $('#startDate').datetimepicker('getValue');
  var end = $('#endDate').datetimepicker('getValue');
  // call api for data
  $.ajax({url:'./api',method:'GET',data:{begin:dateFormat(begin),end:dateFormat(end)}}).done(function(respData) {
    var cats = [];
    var points = [];
    for(var i in respData) {
      // parse data
      cats.push(respData[i]['time']);
      points.push(parseInt(respData[i]['data']));
    }
    var myChart = Highcharts.chart('chart', {
        chart: {
            type: 'column'
        },
        title: {
            text: 'Energy'
        },
        xAxis: {
          categories: cats
        },
        yAxis: {
          title: {
              text: 'Energy per Month'
          }
        },
        plotOptions: {
          column: {
            pointPadding: 0.2,
            borderWidth: 0
          }
        },
        series: [{data:points,step:'center',name:'E'}]
    });
  });
})
