pragma solidity ^0.5.0;

import "openzeppelin-solidity/contracts/token/ERC20/ERC20Detailed.sol";
import "openzeppelin-solidity/contracts/token/ERC20/ERC20Mintable.sol";
import "openzeppelin-solidity/contracts/token/ERC20/ERC20Pausable.sol";

import { ECDSA } from "../libraries/ECDSA.sol";

contract MMEToken is ERC20Detailed, ERC20Mintable, ERC20Pausable {
  using ECDSA for bytes32;

  // Prevent Replay Attacks for claiming tokens
  mapping(address => mapping(uint256 => bool)) seenNonces;

  address public _billContract;
  address public _fundContract;
  address public _owner;

  constructor()
    ERC20Detailed('MinusMinusEnergy Coin', 'MME', 2)
    public
  {
    pause();
    _owner = msg.sender;
  }

  // https://yos.io/2018/11/16/ethereum-signatures/
  function claimToken(address sender, uint256 amount, uint256 nonce, bytes memory signature) public  {

    // Recreates the message hash that was signed on the client
    bytes32 hash = keccak256(abi.encodePacked(sender, amount, nonce));
    bytes32 messageHash = hash.toEthSignedMessageHash();

    // Verify that the message's signer is the owner of the order
    address signer = messageHash.recover(signature);
    require(signer == sender, "Invalid signature");

    // Prevent ReplayAttacks
    require(!seenNonces[signer][nonce], "Tokens already claimed");
    seenNonces[signer][nonce] = true;

    // Reward Token
    _mint(signer, amount);
  }

  function setFundContract(address fundContract) public {
    require(msg.sender == _owner);
    _fundContract = fundContract;
  }

  function setBillContract(address billContract) public {
    require(msg.sender == _owner);
    _billContract = billContract;
  }

  function useToken(address costumer) public {
    require(msg.sender == _billContract);
    _burn(costumer, this.balanceOf(costumer));
  }
}
