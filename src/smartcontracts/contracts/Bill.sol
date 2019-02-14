pragma solidity >=0.4.0 <0.6.0;

contract Bill {

  // Contract owner who can add new energy provider
  address public contractOwner;
  address public tokenAddress;
  address public fondContract

  struct Invoice {
    address issuer;
    uint amount;
  }

  mapping(address => bool) trustedProducers;
  mapping(address => Invoice) openBills;

  constructor(address MMEToken, address FC) public {
    contractOwner = msg.sender;
    tokenAddress = MMEToken;
    fondContract = FC;
  }

  function addEnergyProducer(address producer) public {
    require(msg.sender == contractOwner);
    trustedProducers[producer] = true;
  }

  function createBill(address customer, uint amount) public {
    require(trustedProducers[msg.sender]);
    //require(openBills[customer] == NULL);
    // build struct
  }

  function hasBill() public view returns (bool) {
    return //check struct
  }

  function getInvoiceAmount() public view returns (uint) {
    //require(hasBill());
    //return billAmounts[msg.sender];
  }

  function payBill(uint toFond) public payable {
    require(hat Rechnung));
    require(toFond <= ANZAHL_TOKENS);
    require(msg.value+toFond <= Invoice.amount);

    Rechnung kleiner um ANZAHL_TOKENS-toFond

    Zahle an Invoice.issuer verbleibender Wert

    Zahle an Fond toFond

  }

}
