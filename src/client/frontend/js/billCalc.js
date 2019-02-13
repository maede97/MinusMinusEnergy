// $('#slider').change(function(){
//   $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').attr('value')) / 100.0);
//   $('#fond').html(parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').attr('value')) / 100.0)));
//   console.log($('#slider').attr('value'));
// });


$(document).on('input', '#slider', function() {
  $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').val()) / 100.0);
  $('#fond').html(Math.round((parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').val()) / 100.0)))*100)/100.0);
});

$('#sendMoney').click(function() {
  console.log("PAY BILL");
})
