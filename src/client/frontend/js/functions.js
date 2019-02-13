function dateFormat(d) {
  return  d.getFullYear() +"-"+ ("0"+(d.getMonth()+1)).slice(-2)+"-"+("0" + d.getDate()).slice(-2)+" " + ("0" + d.getHours()).slice(-2) + ":" + ("0" + d.getMinutes()).slice(-2)+ ":00";
}

var pickerOptions = {format:'d.m.Y H:i',inline:true,lang:'en'}

$('#startDate').datetimepicker(pickerOptions);
$('#endDate').datetimepicker(pickerOptions);

$('#showData').click(function(){
  var begin = $('#startDate').datetimepicker('getValue');
  var end = $('#endDate').datetimepicker('getValue');
  $.ajax({url:'./api',method:'GET',data:{begin:dateFormat(begin),end:dateFormat(end)}}).done(function(respData) {
    var cats = [];
    var points = [];
    for(var i in respData) {
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
