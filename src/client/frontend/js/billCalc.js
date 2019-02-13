$(document).on('input', '#slider', function() {
  $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').val()) / 100.0);
  $('#fond').html(Math.round((parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').val()) / 100.0)))*100)/100.0);
});

$('#sendMoney').click(function() {
  console.log("PAY BILL");
})
