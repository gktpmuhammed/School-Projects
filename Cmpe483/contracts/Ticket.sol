
// contracts/GameItem.sol
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts/utils/Counters.sol";

contract Ticket is ERC721 {
    using Counters for Counters.Counter;
    Counters.Counter private _tokenIds;
    constructor() ERC721("Ticket", "TCK") {}

    function createTicket(address _owner) public returns (uint256){
        uint256 newItemId = _tokenIds.current();
        _mint(_owner, newItemId);
        _tokenIds.increment();
        return newItemId;
    }

}
