var provider = null;

if (typeof web3 !== 'undefined') {
  provider = web3.currentProvider;
  web3 = new Web3(web3.currentProvider);
} else {
  // set the provider you want from Web3.providers
  provider = new Web3.providers.HttpProvider('http://127.0.0.1:7545');
  web3 = new Web3(App.web3Provider);
}

let _billAmount;
let _tokenAmount;
let _toFondAmount;
let _toBillAmount;
var _percentage;
const MMEExchangeRate = 1;


$.getJSON("./tokens/MMEToken.json",function(MMETokenArtifact){
  $.getJSON("./tokens/Bill.json",function(BillArtifact) {
    var contracts = {};
    contracts.MMEToken = TruffleContract(MMETokenArtifact);
    contracts.Bill = TruffleContract(BillArtifact);
    contracts.MMEToken.setProvider(provider);
    contracts.Bill.setProvider(provider);

    $('#sendMoney').click(function() {
      contracts.Bill.deployed().then(function(BillInstance){
        var payAmount = web3.toBigNumber(_billAmount).minus(_toBillAmount);
        BillInstance.payBill(_percentage,{gas: 2000000, value: payAmount}).then(function(){
          location.reload();
        });
      });
    });
  });
});

function updateVals() {
  // updates values
   _percentage = Math.floor(parseInt($('#slider').val()));
  _toBillAmount = parseFloat(_tokenAmount * MMEExchangeRate) * _percentage / 100.0;
  _toFondAmount = parseFloat(_tokenAmount * MMEExchangeRate) * (100 - _percentage )/100;

  $('#bill').html(web3.fromWei(_toBillAmount)+" ether");
  $('#fond').html(web3.fromWei(_toFondAmount)+" ether");
}

$(document).on('input', '#slider', updateVals);

$(document).ready(function(){
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
          MMETokenInstance.balanceOf(web3.eth.accounts[0]).then(function(tokenAmount){
            BillInstance.getInvoiceAmount().then(function(invoiceAmount){
              _tokenAmount = tokenAmount.toNumber();
              _billAmount = invoiceAmount.toNumber();
              $("#tokens").html(_tokenAmount + " MME = " + web3.fromWei(_tokenAmount * MMEExchangeRate) + " ether");
              $("#invoiceAmount").html(web3.fromWei(_billAmount) + " ether");
              updateVals();
            });
          });
        });
      });
    });
  });
  }
); // update numbers on startup
