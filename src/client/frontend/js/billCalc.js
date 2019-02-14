var provider = null;

if (typeof web3 !== 'undefined') {
  provider = web3.currentProvider;
  web3 = new Web3(web3.currentProvider);
} else {
  // set the provider you want from Web3.providers
  provider = new Web3.providers.HttpProvider('http://127.0.0.1:7545');
  web3 = new Web3(App.web3Provider);
}

var address = web3.eth.accounts[0];

$.getJSON("./tokens/MMEToken.json",function(MMETokenArtifact){
  $.getJSON("./tokens/Bill.json",function(BillArtifact) {
    var contracts = {};
    contracts.MMEToken = TruffleContract(MMETokenArtifact);
    contracts.Bill = TruffleContract(BillArtifact);
    contracts.MMEToken.setProvider(provider);
    contracts.Bill.setProvider(provider);

    $('#sendMoney').click(function() {
      contracts.Bill.deployed().then(function(BillInstance){
        var payAmount = parseInt($("#energy").html()) + parseInt($("#fond").html()); //TODO
        BillInstance.payBill($('#fond').html(),{from: address, gas: 2000000, value: web3.toWei(payAmount, "milliether")}).then(console.log);
      });
    });
  });
});

function updateVals() {
  // updates values
  $('#bill').html(parseFloat($('#tokens').html()) * parseFloat($('#slider').val()) / 100.0);
  $('#fond').html(Math.round((parseFloat($('#tokens').html()) * (1.0-(parseFloat($('#slider').val()) / 100.0)))*100)/100.0);
}

$(document).on('input', '#slider', updateVals);

$(document).ready(
  $.getJSON("./tokens/MMEToken.json",function(MMETokenArtifact){
    $.getJSON("./tokens/Bill.json",function(BillArtifact) {
      var contracts = {};
      contracts.MMEToken = TruffleContract(MMETokenArtifact);
      contracts.Bill = TruffleContract(BillArtifact);
      contracts.MMEToken.setProvider(provider);
      contracts.Bill.setProvider(provider);
      contracts.Bill.deployed().then(function(BillInstance){
        contracts.MMEToken.deployed().then(function(MMETokenInstance){
          // get Token amount
          MMETokenInstance.getBalance(address).then(function(tokenAmount){
            BillInstance.getInvoiceAmount().then(function(invoiceAmount){
              $("#tokens").html(tokenAmount);
              $("#invoiceAmount").html(invoiceAmount);
            });
          });
        });
      });
    });
  });
  updateVals();
); // update numbers on startup
