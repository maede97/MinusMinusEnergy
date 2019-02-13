function updateVals() {
  // updates values
  $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').val()) / 100.0);
  $('#fond').html(Math.round((parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').val()) / 100.0)))*100)/100.0);
}

$(document).on('input', '#slider', updateVals);

$(document).ready(updateVals); // update numbers on startup

$('#sendMoney').click(function() {
  console.log("PAY BILL");
  $.ajax({url:'./pay',method:'GET',data:{bill:$('#bill').html(),fond:$('#fond').html()}}).done(function(respData) {
    console.log(respData);
  });
});
