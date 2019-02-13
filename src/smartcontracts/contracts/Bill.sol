pragma solidity >=0.4.0 <0.6.0;

contract Bill {

  // Contract owner who can add new energy provider
  address public contractOwner;

  mapping(address => bool) trustedProducers;

  constructor() public {
    contractOwner = msg.sender;
  }

  function addEnergyProducer(address producer) public{
    require(msg.sender == contractOwner);
    trustedProducers[producer] = true;
  }

  /*
  function createBill(address receiver, uint amount) public {
    require(trustedProducers[msg.sender]);
  }
  */
}
