pragma solidity >=0.4.0 <0.6.0;

import "./MMEToken.sol";
import "./Fond.sol";

contract Bill {

  event AddEnergyProducer(address producer);

  // Contract owner who can add new energy provider
  address public _owner;
  Fond private _fondContract;
  MMEToken private _tokenContract;

  struct Invoice {
    address issuer;
    uint amount;
  }

  mapping(address => bool) trustedProducers;
  mapping(address => Invoice) openBills;

  constructor() public {
    _owner = msg.sender;
  }

  function setFondContract(address fond) public {
    _fondContract = Fond(fond);
  }

  function setMMETokenContract(address mmeToken) public {
    _tokenContract = MMEToken(mmeToken);
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

  function payBill(uint toFond) public payable {
    // Check for open bill
    require(this.hasBill(msg.sender));

    uint tokenAmount = _tokenContract.balanceOf(msg.sender);

    // Check for sufficant tokens
    require(toFond <= tokenAmount);

    // Check for sufficant ehter
    require(msg.value+toFond <= openBills[msg.sender].amount);

    // Reduce bill with tokens
    openBills[msg.sender].amount -= tokenAmount-toFond;
    _tokenContract.useToken(msg.sender);

    // Pay ether to issuer
    //openBills[msg.sender].issuer.transfer(openBills[msg.sender].amount);

    // Pay ether to fond
    _fondContract.invest.value(toFond)(msg.sender);
  }

}
