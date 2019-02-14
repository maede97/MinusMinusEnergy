var provider = null;

if (typeof web3 !== 'undefined') {
  provider = web3.currentProvider;
  web3 = new Web3(web3.currentProvider);
} else {
  // set the provider you want from Web3.providers
  provider = new Web3.providers.HttpProvider('http://127.0.0.1:7545');
  web3 = new Web3(App.web3Provider);
}

$.getJSON("./tokens/MMEToken.json",function(MMETokenArtifact){
  var contracts = {};
  contracts.MMEToken = TruffleContract(MMETokenArtifact);
  contracts.MMEToken.setProvider(provider);


  $('#sendMoney').click(function() {
    contracts.MMEToken.deployed().then(function(MMETokenInstance){
      var payAmount = parseInt($("#energy").html()) + parseInt($("#fond").html());
      MMETokenInstance.payBill($('#fond').html(),{from: web3.eth.accounts[0], gas: 2000000, value: web3.toWei(payAmount, "milliether")}).then(console.log);
    });
    // console.log("PAY BILL");
    // $.ajax({url:'./pay',method:'GET',data:{bill:$('#bill').html(),fond:$('#fond').html()}}).done(function(respData) {
    //   console.log(respData);
    // });
  });
});

function updateVals() {
  // updates values
  $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').val()) / 100.0);
  $('#fond').html(Math.round((parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').val()) / 100.0)))*100)/100.0);
}

$(document).on('input', '#slider', updateVals);

$(document).ready(updateVals); // update numbers on startup
