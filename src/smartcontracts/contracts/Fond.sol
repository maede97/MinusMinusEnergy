pragma solidity >=0.4.0 <0.6.0;
contract Fond {
  mapping(address => uint) investors; // how much did each investors send to me?
  address[] invs;

  address public _owner; // stores address of bill contract

  constructor() public {
    _owner = msg.sender;
  }

  function invest(address consumer) public payable {
    investors[consumer] += msg.value;
    invs.push(consumer);
  }

  function releaseFond() public {
    require(msg.sender == _owner);
    // reset all tokens invested
    for (uint i = 0; i < invs.length; i++)
      investors[invs[i]] = 0;
    delete invs;

    // send all money to owner
  }
}
