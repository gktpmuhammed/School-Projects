//pragma solidity ^0.4.21;
pragma solidity ^0.8.0;

import "./TL.sol";
import "./ticket.sol";
import "hardhat/console.sol";

contract Lottery {
    enum Status{ BOUGHT, REVEALED, WINNER, CANCELLED, REFUNDED, COLLECTED }
    mapping(uint256 => Status) status_of_tickets;
    mapping(uint256 => uint256) amount_of_tickets;

    uint256 MAX_INT = 2**256 - 1;
    mapping (address => uint) private balances;
    mapping(uint256 => bytes32) private ticket_hash_map;
    // lottery => ticket_hash)
    mapping(uint256 => bytes32[]) private hashed_tickets; // submitted randnumbers with buy_ticket function in submision period
    mapping(uint256 => uint256[]) private winner_tickets; // these are the indexes
    // lottery no => ticket nos

    mapping(address => mapping(uint256 => uint256[])) ticket_nfts_of_user; // user => [lottery no => [ticket nos]]

    mapping(uint256 => uint256[]) private buyed_tickets;
    mapping(uint256 => bool) private refund_tickets;
    mapping(uint256 => uint[]) private revealed_rnds;

    // lotttery_no => IsfindWinnerCalled
    //mapping(uint256 => bool) isFindWinnerCalled;
    uint256 lastCalculatedLotteryNo;

    mapping(uint256 => uint256[]) private revealed_tickets;
    uint purchaseInterval = 345600; // 4 days
    uint revealInterval = 259200; // 3 days
    TL tl_contract;
    Ticket ticket_contract;
    uint256 initialLotteryTime;

    constructor(address _tl_address) {
        tl_contract = TL(_tl_address);
        ticket_contract = new Ticket();
        initialLotteryTime = block.timestamp;
        lastCalculatedLotteryNo = 0;
    }


    function depositTL(uint _amnt) public payable{
        myGetLotteryNo(block.timestamp); // just to assure rewwards are calculated

        // first we check overflow and underflow conditions
        uint256 balance_of_contract = tl_contract.balanceOf(address(this));
        uint256 balance_of_caller = tl_contract.balanceOf(msg.sender);
        if (balance_of_caller < _amnt || MAX_INT - _amnt < balance_of_contract){
            // overflow, underflow check
            revert("Overflow or underflow danger!");
        }
        // then we transfer TL tokens from caller to this contract.
        //tl_contract.myTransfer(msg.sender, address(this), amnt);
        tl_contract.transferFrom(msg.sender, address(this), _amnt);
        // Finally we increase the balance of the caller
        balances[msg.sender] += _amnt;
    }

    function withdrawTL(uint _amnt) public payable{
        myGetLotteryNo(block.timestamp); // just to assure rewwards are calculated

        // first we check overflow and underflow conditions
        uint256 balance_of_caller = tl_contract.balanceOf(msg.sender);
        uint256 balance_of_contract = tl_contract.balanceOf(address(this));
        if (MAX_INT-_amnt < balance_of_caller || balance_of_contract < _amnt){
            // overflow, underflow check
            revert("Overflow or underflow danger!");
        }
        // then we transfer TL tokens from this contract to the caller.
        //tl_contract.myTransfer(address(this), msg.sender, _amnt);
        tl_contract.transfer(msg.sender, _amnt);
        // Finally we decrease the balance of the caller
        balances[msg.sender] -= _amnt;
    }

    function buyTicket(bytes32 hash_rnd_number) public payable{
        // we must be in submission stage!
        if (inRevealStage()) revert("We are not in submission stage!");
        uint _amnt = 10; // a ticket costs 10 TL
        uint256 balance_of_caller = balances[msg.sender];
        // we check whether caller has enough money to buy a ticket
        if (balance_of_caller < _amnt){
            revert("You don't have enough money to buy a ticket!");
        }
        // we check if this bytes32 number submitted previously, if yes revert.
        if (hashIndex(hash_rnd_number) != -1){
            revert("This number submitted previously!");
        }
        // we withdraw the ticket cost from callers balance
        balances[msg.sender] -= _amnt;
        // we push this hash_rnd_number to hashed_tickets array with current lottery number.
        if (hashed_tickets[myGetLotteryNo(block.timestamp)].length == 0){
            hashed_tickets[myGetLotteryNo(block.timestamp)] = [hash_rnd_number];
        }else{
            hashed_tickets[myGetLotteryNo(block.timestamp)].push(hash_rnd_number);
        }
        // We create and assign a new ticket to the user.
        uint256 ticket_no = ticket_contract.createTicket(msg.sender);
        // we push this ticket no to our buyed_tickets array with current lottery id.
        if(buyed_tickets[myGetLotteryNo(block.timestamp)].length==0){
            buyed_tickets[myGetLotteryNo(block.timestamp)] = [ticket_no];
        }else {
            buyed_tickets[myGetLotteryNo(block.timestamp)].push(ticket_no);
        }
        // we create a mapping between ticket no and submitted hash rnd number.
        ticket_hash_map[ticket_no] = hash_rnd_number;
        status_of_tickets[ticket_no] = Status.BOUGHT;
        // assign ticket to user with current lottery id
        if(ticket_nfts_of_user[msg.sender][myGetLotteryNo(block.timestamp)].length==0){
            ticket_nfts_of_user[msg.sender][myGetLotteryNo(block.timestamp)] = [ticket_no];
        }else {
            ticket_nfts_of_user[msg.sender][myGetLotteryNo(block.timestamp)].push(ticket_no);
        }

    }
    //function that used for collecting refund money.
    function collectTicketRefund(uint ticket_no) public payable{
        myGetLotteryNo(block.timestamp); // just to assure rewards are calculated
        //if ticket is not belong to user, revert.
        if(msg.sender != ticket_contract.ownerOf(ticket_no)) revert("This ticket is not belong to you.");
        //if ticket is available for refund, give refund money to users balance.
        if(refund_tickets[ticket_no]){
            balances[msg.sender] += 5;
            refund_tickets[ticket_no]=false;
            status_of_tickets[ticket_no] = Status.REFUNDED;
        }
    }
    function revealRndNumber(uint _ticket_no, uint _N) public {
        // we must be in reveal stage
        if (!inRevealStage()) revert("We are not in reveal stage!");
        if(msg.sender != ticket_contract.ownerOf(_ticket_no)) revert("This ticket is not belong to you.");
        // we take the hash of the number _N as user did in submission stage.
        // So this must be identical to the hash number submited by user while buying the ticket.
        bytes32 hash = keccak256(abi.encodePacked(_N,msg.sender));
        // we check whether this hash is associated with the ticket
        if (ticket_hash_map[_ticket_no] != hash){ 
            refund_tickets[_ticket_no]=true;
            status_of_tickets[_ticket_no] = Status.CANCELLED;
            return;
        }
        // store this ticket in revealed_tickets array and associate it with current ticket no
        // store random number in revealed_rnds array.
        if(revealed_tickets[myGetLotteryNo(block.timestamp)].length==0){
            revealed_tickets[myGetLotteryNo(block.timestamp)] = [_ticket_no];
            revealed_rnds[myGetLotteryNo(block.timestamp)]  = [_N];
        }else {
            revealed_tickets[myGetLotteryNo(block.timestamp)].push(_ticket_no);
            revealed_rnds[myGetLotteryNo(block.timestamp)].push(_N);
        }
    }

    // helper function, returns the index of _elem in _given _arr.
    function indexOf(uint256[] memory _arr, uint256 _elem) private returns (int){
        for (uint i=0; i < _arr.length; i++) {
            if (_arr[i] == _elem) return int(i);
        }
        return -1;
    }

    // helper function, returns the index of _hash in global array hashed_tickets[current_lottery_no].
    function hashIndex(bytes32 _hash) private returns(int){
        for (uint i=0; i < hashed_tickets[myGetLotteryNo(block.timestamp)].length; i++) {
            if (hashed_tickets[myGetLotteryNo(block.timestamp)][i] == _hash) return int(i);
        }
        return -1;
    }

    // helper function, returns whether we are in reveal stage.
    function inRevealStage() private view returns (bool ){
        return (block.timestamp - initialLotteryTime) % (purchaseInterval + revealInterval) > purchaseInterval;
    }
    //function which calculate lottery no.
    function myGetLotteryNo(uint unixtimeinweek) public returns (uint lottery_no){
        uint current_lottery_no = (unixtimeinweek - initialLotteryTime) / (purchaseInterval + revealInterval);
        // if the current lottery no is changed, this function calls findWinners function. Thus, winners are determined.
        if (current_lottery_no != lastCalculatedLotteryNo){
            findWinners();
            lastCalculatedLotteryNo++;
        }
        return current_lottery_no;
    }
    //function for lottery no.
    function getLotteryNo(uint unixtimeinweek) public view returns (uint lottery_no){
        uint current_lottery_no = (unixtimeinweek - initialLotteryTime) / (purchaseInterval + revealInterval);
        return current_lottery_no;
    }
    //helper function, controls whether the lottery is finished.
    function isFinished(uint lottery_no) private view returns (bool ){
        return (block.timestamp - initialLotteryTime) > lottery_no*(purchaseInterval + revealInterval);
    }
    //function that return last owned ticket.
    function getLastOwnedTicketNo(uint lottery_no) public view returns(uint,uint8 status){

        return (ticket_nfts_of_user[msg.sender][lottery_no][ticket_nfts_of_user[msg.sender][lottery_no].length-1],uint8(status_of_tickets[ticket_nfts_of_user[msg.sender][lottery_no][ticket_nfts_of_user[msg.sender][lottery_no].length-1]]));
    }

    function getIthOwnedTicketNo(uint i,uint lottery_no) public view returns(uint,uint8 status){
        //If there is not enough ticket, function will revert.
        if(i <= ticket_nfts_of_user[msg.sender][lottery_no].length){
            // if there is enough ticket, returns i'th ticket.
            return (ticket_nfts_of_user[msg.sender][lottery_no][i - 1], uint8(status_of_tickets[ticket_nfts_of_user[msg.sender][lottery_no][i - 1]]));
        }
        revert("You don't have enough ticket!");
    }

    function checkIfTicketWon(uint ticket_no) public view returns (uint amount){
        // if ticket is winner, this function returns the amount of the ticket.
        if(status_of_tickets[ticket_no] == Status.WINNER || status_of_tickets[ticket_no] == Status.COLLECTED){
            return amount_of_tickets[ticket_no];
        }
        return 0;
    }
    
    function collectTicketPrize(uint ticket_no) public{
        myGetLotteryNo(block.timestamp); // just to assure rewwards are calculated
        if(msg.sender != ticket_contract.ownerOf(ticket_no)) revert("This ticket is not belong to you.");
        // if the ticket belongs to the user, this function gives money to the balance of the user.
        if(status_of_tickets[ticket_no] == Status.WINNER){
            balances[msg.sender] += amount_of_tickets[ticket_no];
            status_of_tickets[ticket_no] = Status.COLLECTED;
        }
    }
    // helper function to determine winners with using hashing and the algorithm that stated in the description.
    function findWinners() private {
        uint lottery_no = getLotteryNo(block.timestamp)-1;
        // if there are not any ticket in this lottery, return
        if (revealed_rnds[lottery_no].length == 0) return;
        uint N_xor = revealed_rnds[lottery_no][0];
        uint participants = revealed_tickets[lottery_no].length;
        // calculating the xor value for all n's.
        for(uint k = 1; k < participants; k++){
            N_xor = N_xor ^ revealed_rnds[lottery_no][k];
        }
        uint i = 0;
        uint m = getTotalLotteryMoneyCollected(lottery_no);
        uint tmp = m;
        // calculating the logarithm.
        for (; tmp > 1; tmp >>= 1) i += 1;
        i += 1;
        // calculating the prize and determine each winner ticket
        for(uint a = 1; a <= i; a++){
            uint winner = N_xor % participants;
            status_of_tickets[revealed_tickets[lottery_no][winner]] = Status.WINNER;
            //value of the prize.
            uint prize = m / (2**a) + ((m / (2**(a-1))) % 2); 
            //winner ticket selected.
            if (winner_tickets[lottery_no].length==0){
                winner_tickets[lottery_no] = [revealed_tickets[lottery_no][winner]];
            }else{
                winner_tickets[lottery_no].push(revealed_tickets[lottery_no][winner]);
            }
            amount_of_tickets[revealed_tickets[lottery_no][winner]] += prize;
            N_xor = uint(keccak256(abi.encode(N_xor)));
        }   
    }
    //this function returns the i'th winner ticket in the specified lottery.
    function getIthWinningTicket(uint i, uint lottery_no) public view returns (uint ticket_no,uint amount){
        // if there is not enough ticket, revert
        if(i <= winner_tickets[lottery_no].length){
            return (winner_tickets[lottery_no][i - 1], amount_of_tickets[winner_tickets[lottery_no][i - 1]]);
        }
        revert("We dont have enough ticket.");
    }
    //this function returns the total money collected in the specified lottery.
    function getTotalLotteryMoneyCollected(uint lottery_no) public view returns (uint amount){
        return buyed_tickets[lottery_no].length * 10;
    }
}