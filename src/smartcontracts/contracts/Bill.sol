pragma solidity >=0.4.0 <0.6.0;

contract Bill {

  // Contract owner who can add new energy provider
  address public contractOwner;
  address public tokenAddress;

  mapping(address => bool) trustedProducers;
  mapping(address => bool) nodeHasBill;
  mapping(address => uint) billAmounts;

  constructor(address MMEToken) public {
    contractOwner = msg.sender;
    tokenAddress = MMEToken;
  }

  function addEnergyProducer(address producer) public {
    require(msg.sender == contractOwner);
    trustedProducers[producer] = true;
  }

  function createBill(address receiver, uint amount) public {
    require(trustedProducers[msg.sender]);
    require(!nodeHasBill[receiver]);
    nodeHasBill[receiver] = true;
    billAmounts[receiver] = amount;
  }

  function hasBill() public view returns (bool) {
    return nodeHasBill[msg.sender];
  }

  function billAmount() public view returns (uint) {
    return billAmounts[msg.sender];
  }

  function payBill(address receiver) public payable{
    require(trustedProducers[msg.sender]);
    require(nodeHasBill[receiver]);

    // require aufteilung von tokens

    //wie viel tokens im bill-topf?

    //tokenAddress.call();


    nodeHasBill[receiver] = false;
    billAmounts[receiver] = 0;
  }

}
