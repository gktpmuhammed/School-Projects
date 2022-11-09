// We require the Hardhat Runtime Environment explicitly here. This is optional
// but useful for running the script in a standalone fashion through `node <script>`.
//
// When running the script with `npx hardhat run <script>` you'll find the Hardhat
// Runtime Environment's members available in the global scope.
const hre = require("hardhat");

async function main() {
  // Hardhat always runs the compile task when running scripts with its command
  // line interface.
  //
  // If this script is run directly using `node` you may want to call compile
  // manually to make sure everything is compiled
  // await hre.run('compile');

  // We get the contract to deploy
  const TL = await hre.ethers.getContractFactory("TL");
  const tl = await TL.deploy();
  await tl.deployed();
  console.log("TL deployed to:", tl.address);

  const Lottery = await hre.ethers.getContractFactory("Lottery");
  const lottery = await Lottery.deploy(tl.address);
  await lottery.deployed();
  console.log("Lottery deployed to:", lottery.address);

  const User = await hre.ethers.getContractFactory("User");
  const user = await User.deploy(lottery.address, tl.address);
  await user.deployed();
  console.log("User deployed to:", user.address);
}

// We recommend this pattern to be able to use async/await everywhere
// and properly handle errors.
main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });