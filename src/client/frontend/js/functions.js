document.addEventListener('DOMContentLoaded', function () {
    var myChart = Highcharts.chart('chart', {
        chart: {
            type: 'column'
        },
        title: {
            text: 'Energy'
        },
        xAxis: {
          categories: CHART_CATEGORIES

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
        series: CHART_DATA
    });
});

var pickerOptions = {format:'d.m.Y H:i',inline:true,lang:'en'}

$('#startDate').datetimepicker(pickerOptions);
$('#endDate').datetimepicker(pickerOptions);
