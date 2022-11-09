//pragma solidity ^0.4.21;
pragma solidity ^0.8.0;

import "./TL.sol";
import "hardhat/console.sol";
import "./Lottery.sol";

contract User {
    uint256 MAX_INT = 2**256 - 1;
    Lottery lottery_contract;
    TL tl_contract;
    mapping(address =>  uint256[]) my_ticket_numbers;

    constructor(address  _lottery_contract_address, address _tl_contract_address) {
        tl_contract = TL(_tl_contract_address);
        lottery_contract = Lottery(_lottery_contract_address);
    }
    fallback() external payable {
    //    balances[msg.sender] += msg.value;
    }
    receive() external payable {
    //    balances[msg.sender] += msg.value;
    }

    function giveMoney(uint _amnt) public payable{
        // give initial money to accounts
        tl_contract.giveMoney(msg.sender, _amnt);
    }
    function approveContract(uint _amnt) public {
        tl_contract.myApprove(msg.sender, address(lottery_contract), _amnt);
    }

    function hashNumber(uint256 _num) public returns (bytes32) {
        bytes32 hash = keccak256(abi.encodePacked(_num,msg.sender)); // now vs eklenebilir randomluğu artırmak için
        return hash;
    }


}