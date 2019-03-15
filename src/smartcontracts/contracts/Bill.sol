pragma solidity >=0.4.0 <0.6.0;

import "./MMEToken.sol";
import "./Fund.sol";

contract Bill {

  event AddEnergyProducer(address producer);
  event PayedBill(address issuer, address client, uint share, uint toFond);

  // Contract owner who can add new energy provider
  address public _owner;
  Fund private _fundContract;
  MMEToken private _tokenContract;

  // wei / MME (default 1)
  uint private _MMEExchangeRate;

  struct Invoice {
    address payable issuer;
    uint amount;
  }

  mapping(address => bool) trustedProducers;
  mapping(address => Invoice) openBills;

  constructor() public {
    _owner = msg.sender;
    _MMEExchangeRate = 1;
  }

  function setFundContract(address fund) public {
    _fundContract = Fund(fund);
  }

  function setMMETokenContract(address mmeToken) public {
    _tokenContract = MMEToken(mmeToken);
  }

  function setMMEExchangeRate(uint MMEExchangeRate) public {
    require(msg.sender == _owner);
    _MMEExchangeRate = MMEExchangeRate;
  }

  function addEnergyProducer(address producer) public {
    require(msg.sender == _owner);
    trustedProducers[producer] = true;
    emit AddEnergyProducer(producer);
  }

  function createBill(address customer, uint amount) public {
    require(trustedProducers[msg.sender]);
    // Check for existing bill
    require(openBills[customer].amount == 0);
    // build struct
    openBills[customer] = Invoice(msg.sender, amount);
  }

  function hasBill(address customer) public view returns (bool) {
    return (openBills[customer].amount > 0);
  }

  function getInvoiceAmount() public view returns (uint) {
    require(this.hasBill(msg.sender));
    return openBills[msg.sender].amount;
  }

  // NOTE:  This function was changed after the challange and is not backwards
  //        compatible
  // perToken = percetage of token to use for paying the bill (x 100)
  function payBill(uint perToken) public payable {
    // Check for open bill
    require(this.hasBill(msg.sender),"sender needs bill");
    uint tokenAmount = _tokenContract.balanceOf(msg.sender);
    uint billAmount = openBills[msg.sender].amount;

    require(msg.value >= (billAmount - (perToken * tokenAmount * _MMEExchangeRate) / 100), "Not enought ether send.");
    uint toFund = (_MMEExchangeRate * tokenAmount * (100-perToken)) / 100;
    require(address(this).balance >= toFund, 'Subsidizing pool empty!');

    _tokenContract.useToken(msg.sender);

    // Pay ether to issuer
    openBills[msg.sender].issuer.transfer(openBills[msg.sender].amount);
    openBills[msg.sender].amount = 0;

    // Pay ether to fund
    _fundContract.invest.value(toFund)(msg.sender);

    emit PayedBill(
      openBills[msg.sender].issuer,
      msg.sender,
      perToken,
      toFund
    );

  }

  function() external payable {
    require(msg.sender == _owner, "You are not the subsidizing party");
  }

}
