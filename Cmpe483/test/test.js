const hre = require("hardhat");

async function progressTime(days) {
    let time = days*3600*24
    await hre.ethers.provider.send("evm_increaseTime", [time])
    await hre.ethers.provider.send("evm_mine", [])
} 

const myTest = () => describe("Casino", function () {
    const predefined_limits = {gasLimit: 3000000, gasPrice: 586181643}
    var user_countlist = [5,10,100,200,400]
    for (let k = 0; k < user_countlist.length; k++){
        it("Testing with "+ user_countlist[k] +" accounts ", async function () {
            // Contract deployments
            const TL = await hre.ethers.getContractFactory("TL");
            const tl = await TL.deploy();
            await tl.deployed();
            const Lottery = await hre.ethers.getContractFactory("Lottery");
            const lottery = await Lottery.deploy(tl.address);
            await lottery.deployed();
            const User = await hre.ethers.getContractFactory("User");
            const user_contract = await User.deploy(lottery.address, tl.address);
            await user_contract.deployed();
            console.log("All contracts deployed!")
    
            const num_of_users = user_countlist[k]
            let gasCost = {}

            let user_accounts = await hre.ethers.getSigners()
            user_accounts = user_accounts.filter(_ => _.address.toLowerCase() !== lottery.address.toLowerCase()).slice(0,num_of_users)
            for (; user_accounts.length < num_of_users;){
                let new_user = web3.eth.personal.newAccount()
                user_accounts.push(new_user)
            }
            console.log(user_accounts.length + " users created!")

            let random_numbers = []
            let tickets = []

            // Deposit money and buy tickets
            for (let i = 0; i < num_of_users; i++) {
                let user_account = user_accounts[i]
                await user_contract.connect(user_account).giveMoney(1000, predefined_limits)  // give initial TLs to user
                user_contract.connect(user_account).approveContract(1000, predefined_limits)  // allow contract to spend money in the name of user

                let tx = await lottery.connect(user_account).depositTL(10, predefined_limits)                                         // deposit 10 TLs to lottery
                receipt = await ethers.provider.getTransactionReceipt(tx.hash)
                if (receipt !== null)  
                    gasCost.depositTL += receipt.gasUsed.toNumber()
                let random_number_N_to_be_hashed = Math.floor(Math.random() * 1E3)                                                
                random_numbers.push(random_number_N_to_be_hashed)                                                                                  // Note down user's random number
                let hash = web3.utils.soliditySha3(
                    {t: 'uint256', v: random_number_N_to_be_hashed},
                    {t: 'address', v: user_account.address},
                );
                tx = await lottery.connect(user).buyTicket(hash, predefined_limits)                                       // buy a ticket
                receipt = await ethers.provider.getTransactionReceipt(tx.hash)
                if (receipt !== null)  
                    gasCost.buyTicket += receipt.gasUsed.toNumber()
                //  console.log(Math.ceil(Date.now() / 1000))
                //  const lottery_no = await lottery.connect(user_account).getLotteryNo(Math.ceil(Date.now() / 1000),predefined_limits) 
                //  console.log(lottery_no)
                let ticketId = await lottery.connect(user_account).getLastOwnedTicketNo(0,predefined_limits)              // Get ticket id
                receipt = await ethers.provider.getTransactionReceipt(ticketId.hash)
                if (receipt !== null)  // There might be cases when transaction hasn't been mined yet, simply skip them
                    gasCost.getLastOwnedTicketNo  += receipt.gasUsed.toNumber()
                tickets.push(ticketId["0"])                                                                            // Note down user's ticket
            }

            await progressTime(4) 

            // Revealing random numbers
            for (let i = 0; i < num_of_users; i++) {
                let user = user_accounts[i]
                let random_number_that_hashed = random_numbers[i]
                // add some randomness so that some reveals will become wrong
                if (i > 2){
                    let random_number_that_hashed = random_numbers[i-Math.floor(Math.random()*2)]
                }
                let ticketId = tickets[i]
                let tx = await lottery.connect(user).revealRndNumber(ticketId, random_number_that_hashed, predefined_limits)   // Reveal random number
                let receipt = await ethers.provider.getTransactionReceipt(tx.hash)
                if (receipt !== null)  // There might be cases when transaction hasn't been mined yet, simply skip them
                    gasCost.revealRndNumber += receipt.gasUsed.toNumber()
            }
            console.log("Revealed random numbers that previously submitted.")
            await progressTime(3) 

            let extra_deposit = await lottery.connect(user_accounts[0]).depositTL(0, predefined_limits)   
            let extra_deposit_receipt = await ethers.provider.getTransactionReceipt(tx.hash)
            if (extra_deposit_receipt !== null) 
                gasCost.depositTL  += receipt.gasUsed.toNumber()

            // Check whether ticket won prize and claim it
            for (let i = 0; i < num_of_users; i++) {
            let user = user_accounts[i]
            let ticketId = tickets[i]
            let prize = await lottery.connect(user).checkIfTicketWon(ticketId, predefined_limits) 
            let receipt = await ethers.provider.getTransactionReceipt(tx.hash)
            if (receipt !== null)  
                gasCost.checkIfTicketWon  += receipt.gasUsed.toNumber()
            let tx = await lottery.connect(user).collectTicketPrize(ticketId, predefined_limits)
            receipt = await ethers.provider.getTransactionReceipt(tx.hash)
            if (receipt !== null)  
                gasCost.collectTicketPrize  += receipt.gasUsed.toNumber()
            }

            // Print out gas usages
            for (var func in gasCost) {
            console.log(`${func}: ${gasCost[func] / num_of_users}`)
            }
        }).timeout(1E5);
    }
});


myTest();