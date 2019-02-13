pragma solidity >=0.4.0 <0.6.0;
contract Fond {
  mapping(address => uint) investors; // how much did each investors send to me?
  address[] invs;

  address public billContractAddress; // stores address of bill contract

  constructor(address billContract) public{
    billContractAddress = billContract;
  }

  function receiveTokens(address investor, uint amount) public {
    require(msg.sender == billContractAddress);
    investors[investor] += amount;
    invs.push(investor);
  }

  function releaseFond() public {
    require(msg.sender == billContractAddress);
    // reset all tokens invested
    for (uint i = 0; i < invs.length; i++)
      investors[invs[i]] = 0;
    delete invs;
  }
}
