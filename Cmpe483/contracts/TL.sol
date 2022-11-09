// contracts/GLDToken.sol
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";

contract TL is ERC20 {
    constructor(/*uint256 initialSupply*/) public ERC20("TurkishLira", "TL") {
        //_mint(msg.sender, initialSupply);
    }

    function giveMoney(address _to, uint256 _amnt) public payable returns (bool){
        _mint(_to,_amnt);
        return true;
    }

    function myTransfer(address _from, address _to, uint256 _amnt) public payable returns (bool){
        _transfer(_from,_to,_amnt);
        return true;
    }

    function myApprove(address _origin, address _spender, uint256 _amnt) public returns (bool){
        _approve(_origin, _spender, _amnt);
        return true;
    }
}