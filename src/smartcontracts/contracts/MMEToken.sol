pragma solidity ^0.5.0;

import "openzeppelin-solidity/contracts/token/ERC20/ERC20Detailed.sol";
import "openzeppelin-solidity/contracts/token/ERC20/ERC20Mintable.sol";

import { ECDSA } from "../libraries/ECDSA.sol";

contract MMEToken is ERC20Detailed, ERC20Mintable {
  using ECDSA for bytes32;

  event Error(string message);

  // Prevent Replay Attacks for claiming tokens
  mapping(address => mapping(uint256 => bool)) seenNonces;

  address public billContract;
  address public fondContract;
  address public owner;

  constructor()
    ERC20Detailed('MinusMinusEnergy Coin', 'MME', 2)
    public
    {
      addMinter(address(this));
      owner = msg.sender;

  }

  // Planned: Data Signature verificatioin with EEC
  // https://yos.io/2018/11/16/ethereum-signatures/
  function claimToken(address sender, uint256 amount, uint256 nonce, bytes memory signature) public  {

    // This recreates the message hash that was signed on the client.
    bytes32 hash = keccak256(abi.encodePacked(sender, amount, nonce));
    bytes32 messageHash = hash.toEthSignedMessageHash();

    // Verify that the message's signer is the owner of the order
    address signer = messageHash.recover(signature);
    //require(signer == sender, "Invalid signature");

    // Prevent ReplayAttacks
    require(!seenNonces[signer][nonce], "Tokens already claimed");
    seenNonces[signer][nonce] = true;

    // Reward Token
    //SCheis fucking z8iiile
    _mint(signer, amount);
  }

  function setFondContract(address FC) public {
    require(msg.sender == owner);
    fondContract = FC;
  }

  function setBillContract(address BC) public {
    require(msg.sender == owner);
    billContract = BC;
  }
}
