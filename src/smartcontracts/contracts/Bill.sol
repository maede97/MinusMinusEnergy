pragma solidity >=0.4.0 <0.6.0;
contract Bill {
  address public owner;
  mapping(address => bool) trustedProducers;

  constructor() public {
    owner = msg.sender;
  }

  function addEnergyProducer(address producer) public{
    require(msg.sender == owner);
    trustedProducers[producer] = true;
  }

  function createBill(address receiver, uint amount) public {
    require(trustedProducers[msg.sender]);
    
  }
}
